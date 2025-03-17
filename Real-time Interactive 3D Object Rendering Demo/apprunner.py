import bteve2 as eve
import os
import time
import struct

import board
import busio
import digitalio

import sdcardio
import storage

from busio import SPI

LVDS_EN             = 0x7F80_0300 
LVDSPLL_CFG         = 0x7F80_0304 
LVDS_CFG            = 0x7F80_0308 
LVDS_TGEN_HCFG_0    = 0x7F80_030C 
LVDS_TGEN_VCFG_0    = 0x7F80_0310 
LVDS_CTRL_CH0       = 0x7F80_0314 
LVDS_CTRL_CH1       = 0x7F80_0318 
LVDS_STAT           = 0x7F80_031C 
LVDS_ERR_STAT       = 0x7F80_0320 
LVDS_TGEN_HCFG_1    = 0x7F80_0324 
LVDS_TGEN_VCFG_1    = 0x7F80_0328 
LVDS_TGEN_HCFG_2    = 0x7F80_032C 
LVDS_TGEN_VCFG_2    = 0x7F80_0330 

def spilock(f):
    def wrapper(*args):
        spi = args[0].sp
        while not spi.try_lock():
            pass
        r = f(*args)
        spi.unlock()
        return r
    return wrapper

class EVE2(eve.EVE2):
    def __init__(self):

        mach = os.uname().machine
        print('mach', mach)
        if mach == 'Raspberry Pi Pico with rp2040':
            self.sp = SPI(board.GP2, MOSI=board.GP3, MISO=board.GP4)
            self.cs = self.pin(board.GP5) #cs of SPI for Eve
            self.pdn = self.pin(board.GP7) #power down pin of Eve
        else:
            self.sp = SPI(board.D12, MOSI=board.D13, MISO=board.D11)
            self.cs = self.pin(board.D10)
            self.pdn = self.pin(board.D6) #power down pin of Eve
        self.setup_spi()
 #       print('SD', self.setup_sd())
        self.boot()

    def pin(self,p):
        r = digitalio.DigitalInOut(p)
        r.direction = digitalio.Direction.OUTPUT
        r.value = True
        return r

    @spilock
    def setup_spi(self):
        self.sp.configure(baudrate=20_000_000, phase=0, polarity=0)

    def setup_sd(self):
        """ Setup sdcard"""
        spi_sdcard = busio.SPI(board.GP10, MOSI=board.GP11, MISO=board.GP12)
        sdcs = board.GP13 #cs of SPI for SD card
        try:
            self.sdcard = sdcardio.SDCard(spi_sdcard, sdcs)
        except OSError:
            return False
        self.vfs = storage.VfsFat(self.sdcard)
        storage.mount(self.vfs, "/sd")
        return True

    @spilock
    def transfer(self, wr, rd = 0):
        self.cs.value = False
        self.sp.write(wr)
        r = None
        if rd != 0:
            r = bytearray(rd)
            self.sp.readinto(r)
        self.cs.value = True
        return r

    abuf = bytearray(4)
    def addr(self, a):
        struct.pack_into(">I", self.abuf, 0, a)
        return self.abuf

    def reset(self):
        while 1:
            self.pdn.value = 0
            time.sleep(0.1)
            self.pdn.value = 1
            time.sleep(0.1)

            self.transfer(bytes([0xFF, 0xE9, 0xC1, 0x00, 0x00]))  # set BOOTCFGEN
            self.transfer(bytes([0xFF, 0xEB, 0x0A, 0x00, 0x00]))  # DDR
            self.transfer(bytes([0xFF, 0xE8, 0xE0, 0x00, 0x00]))  # SPIS_BOOTCFG = 60
            self.transfer(bytes([0xFF, 0xE9, 0xC0, 0x00, 0x00]))  # clear BOOTCFGEN
            syspll = 576e6
            FREQUENCY = 72e6
            freqbyte = int(round((syspll / FREQUENCY) - 1))
            self.transfer(bytes([0xFF, 0xE6, 0x10 | freqbyte, 0x00, 0x00]))
            self.transfer(bytes([0xFF, 0xE7, 0x00, 0x00, 0x00]))  # RESET_PULSE
            self.transfer(bytes([0, 0, 0, 0, 0]))
            time.sleep(.1)

            bb = self.transfer(self.addr(0), 128)
            t0 = time.monotonic_ns()
            fault = False
            if 1 in list(bb):
                while self.rd32(eve.REG_ID) != 0x7c:
                    pass
                while not fault and self.rd32(eve.BOOT_STATUS) != 0x522e2e2e:
                    fault = 1e-9 * (time.monotonic_ns() - t0) > 0.1
                if fault:
                    bs = self.rd32(eve.BOOT_STATUS)
                    print(f"[Timeout waiting for BOOT_STATUS, stuck at {bs:08x}, retrying...]")
                    continue
                actual = self.rd32(eve.REG_FREQUENCY)
                if actual != FREQUENCY:
                    print(f"[Requested {FREQUENCY/1e6} MHz, but actual is {actual/1e6} MHz after reset, retrying...]")
                    continue
                print(f"Boot time {self.rd32(eve.REG_CLOCK)/36e6:.3f} s")
                return
            print(f"[Boot fail after reset, retrying... reading={1 in list(bb)}]")

    @spilock
    def rd(self, a, nn):
        assert (a & 3) == 0
        assert (nn & 3) == 0
        if nn == 0:
            return b''
        a1 = a + nn
        r = b''
        while a != a1:
            n = min(a1 - a, 32)
            self.cs.value = False
            self.sp.write(self.addr(a))
            def recv(n):
                read_buffer = bytearray(n)
                self.sp.readinto(read_buffer)
                return read_buffer
            bb = recv(32 + n)
            if 1 in list(bb):            # Got READY byte in response
                i = list(bb).index(1)
                response = bb[i + 1:i + 1 + n]
            else:
                                    # Poll for READY byte
                while recv(1) == b'\x00':
                    pass
                response = b''
                                    # Handle case of full response not received
            if len(response) < n:
                response += recv(n - len(response))
            a += n
            r += response
            self.cs.value = True
        return r

    @spilock
    def wr(self, a, s):
        assert (a & 3) == 0
        t = len(s)
        assert (t & 3) == 0

        self.cs.value = False
        self.sp.write(self.addr(a | (1 << 31)))
        self.sp.write(s)
        self.cs.value = True

    def sleepclocks(self, n):
        time.sleep(n / 36e6)

    def setup_flash(self):
        pass

def run(app, minimal = False):
    gd = EVE2()
    gd.register(gd)

    if minimal == False:
        gd.cmd0(0)

        gd.cmd_regwrite(eve.REG_SC0_SIZE, 2)
        gd.cmd_regwrite(eve.REG_SC0_PTR0, 10 << 20)
        gd.cmd_regwrite(eve.REG_SC0_PTR1, 18 << 20)
        gd.framebuffer = eve.Surface(eve.SWAPCHAIN_0, eve.YCBCR, 1920, 1200)
        gd.panel(gd.framebuffer)
        gd.finish()

    app(gd)
    gd.finish()
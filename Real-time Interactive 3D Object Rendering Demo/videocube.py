import math
import sys
import bteve2 as eve
import apprunner
import time
import struct
import gc
import json

CP = (sys.implementation.name == "circuitpython")

if CP:
    import ulab.numpy as np
else:
    import numpy as np

from demo import Demo
import trackball

from lvdsrx_setup import Setup_Lvdsrx

DEMO_LOGO = 0
DEMO_VIDEOCUBE = 1
DEMO_TEAPOT = 2

LVDSRX_W = 1920
LVDSRX_H =1080
LVDSRX_CTRL = 0x7F80_0504
LVDSRX_STAT = 0x7f80_0508
LVDSRX_CHATTERING = 10
class VideoCube(Demo):
    def __init__(self):
        gc.collect()

    def run(self, gd):
        self.gd = gd
        P = 50
        N = -P

        vertices = [
        (P,P,P),
        (N,P,P),
        (P,N,P),
        (N,N,P),

        (P,P,N),
        (N,P,N),
        (P,N,N),
        (N,N,N)
        ]

        # each line is a face: count, normal, 4 vertices
        strips = [
        (0, 1, 3, 2),
        (6, 7, 5, 4),
        (4, 5, 1, 0),
        (2, 3, 7, 6),
        (0, 2, 6, 4),
        (3, 1, 5, 7),
        ]

        self.curquat = trackball.trackball(0, 0, 0, 0)
        
        def render(self, gd, sx, sy):
            apron = 4
            (bx1, by3, bx3, by1) = (-apron, -apron, self.src.w + apron, self.src.w + apron)

            gd.Begin(eve.BITMAPS)

            for s in strips:
                xx = np.array([sx[i] for i in s])
                yy = np.array([sy[i] for i in s])
                area = (int(xx[0]) - int(xx[2])) * (int(yy[1]) - int(yy[0])) - (int(xx[0]) - int(xx[1])) * (int(yy[2]) - int(yy[0]))
                if area > 0:
                    continue

                x0 = max(0, min(xx))
                y0 = max(0, min(yy))
                x1 = min(gd.w, max(xx))
                y1 = min(gd.h, max(yy))
                xx -= x0
                yy -= y0

                gd.cmd_bitmap_transform(
                    int(xx[0]), int(yy[0]),
                    int(xx[1]), int(yy[1]),
                    int(xx[2]), int(yy[2]),
                    int(bx1),
                    int(by1),
                    int(bx3),
                    int(by1),
                    int(bx3),
                    int(by3),
                    0)
                gd.ScissorXY(int(x0), int(y0))
                gd.ScissorSize(int(x1 - x0), int(y1 - y0))
                gd.Vertex2f(int(x0), int(y0))
        def xform(self, xyz):
            rr = trackball.build_rotmatrix(self.curquat)
            x = np.dot(xyz, rr[0])
            y = -np.dot(xyz, rr[1])
            q = (self.gd.h / 216)
            return (
                np.array(self.gd.w / 2 + x * q, dtype = np.int16),
                np.array(self.gd.h / 2 + y * q, dtype = np.int16)
            )

        def setup(self, gd):
            self.src1 = eve.Surface(0x100000, eve.RGB565, LVDSRX_W, LVDSRX_H)
            Setup_Lvdsrx(gd, self.src1)
            self.lvdsrx_setup_completed = 1                       
            self.src  = eve.Surface(0x600000, eve.RGB565, 0x120, 0x120)
            gd.cmd_setbitmap(*self.src)
            gd.BitmapSize(eve.NEAREST, eve.BORDER, eve.BORDER, 0, 0)
            gd.BitmapSizeH(0, 0)

            Demo.setup_scroll(self, gd)

        setup(self, gd)
        gd.swap()

        vertex_array    = 0
        draw_list       = 0x10000
        sxa             = 0x20000
        sya             = 0x30000
        background      = 0x40000

        self.stats      = (50 << 20)

        xyz = np.array(vertices)

        gc.collect()
        gd.finish()

        self.prev_touch = None
        self.spin = trackball.trackball(-.04, -.04, 0, 0)

        t0 = time.monotonic()
        N = 3_600
        frame = 0
        scroll_frame = 0
        c_lvdsrx_chattering = 0
        pre_lvdsrx_frame = 0
        while frame < N:
            if 0:
                lvdsrx_frame = gd.rd32(eve.REG_RX_FRAMES)
                print(f"LVDSRX_FRAMES = :     { lvdsrx_frame:#x}")            
                if (lvdsrx_frame == pre_lvdsrx_frame and self.lvdsrx_setup_completed == 1) :
                    c_lvdsrx_chattering +=1
                    if ((self.lvdsrx_setup_completed == 1) and c_lvdsrx_chattering >= LVDSRX_CHATTERING):
                        print(" sync lost. cleared")
                        gd.wr32(eve.REG_RX_CAPTURE, 0)
                        gd.wr32(eve.REG_RX_ENABLE, 0)
                        gd.wr32(eve.REG_RX_ENABLE, 0)
                        lvdsrx_ctrl = gd.rd32(LVDSRX_CTRL)
                        lvdsrx_ctrl = lvdsrx_ctrl & 0xFFFFFEFE
                        gd.wr32(LVDSRX_CTRL, lvdsrx_ctrl)
                        self.lvdsrx_setup_completed = 0
                        c_lvdsrx_chattering = 0
                else:
                    c_lvdsrx_chattering = 0
                    pre_lvdsrx_frame = lvdsrx_frame
                    if (self.lvdsrx_setup_completed == 0):
                        print(" sync recovered. re-setup")                        
                        Setup_Lvdsrx(gd, self.src1)
                        self.lvdsrx_setup_completed = 1            
            else:
                lvdsrx_sts = gd.rd32(LVDSRX_STAT)
                print(f"LVDSRX_STAT = :     { lvdsrx_sts:#x}")
                if ((lvdsrx_sts & 0xF0000000) != 0xF0000000) :
                    c_lvdsrx_chattering +=1
                    if ((self.lvdsrx_setup_completed == 1) and c_lvdsrx_chattering >= LVDSRX_CHATTERING):
                        print(" frame lost. cleared")
                        gd.wr32(eve.REG_RX_CAPTURE, 0)
                        gd.wr32(eve.REG_RX_ENABLE, 0)
                        gd.wr32(eve.REG_RX_ENABLE, 0)
                        lvdsrx_ctrl = gd.rd32(LVDSRX_CTRL)
                        lvdsrx_ctrl = lvdsrx_ctrl & 0xFFFFFEFE
                        gd.wr32(LVDSRX_CTRL, lvdsrx_ctrl)
                        self.lvdsrx_setup_completed = 0
                        c_lvdsrx_chattering = 0
                else:
                    c_lvdsrx_chattering = 0
                    if (self.lvdsrx_setup_completed == 0):
                        print(" frame recovered. re-setup")                        
                        Setup_Lvdsrx(gd, self.src1)
                        self.lvdsrx_setup_completed = 1            

            (sx, sy) = xform(self, xyz)

            gd.cmd_rendertarget(*self.src)
            gd.cmd_setbitmap(*self.src1)
            gd.BitmapSize(eve.BILINEAR, eve.BORDER, eve.BORDER, self.src1.w, self.src1.h)
            gd.BitmapSizeH(self.src1.w >> 9, self.src1.h >> 9)
            gd.cmd_scale(self.src.h / self.src1.h, self.src.h / self.src1.h)
            gd.cmd_setmatrix()
            gd.Begin(eve.BITMAPS)
            gd.Vertex2f(0, 0)
            gd.swap()
            gd.cmd_rendertarget(*gd.framebuffer)

            gd.cmd_setbitmap(*self.src)
            gd.BitmapSize(eve.NEAREST, eve.BORDER, eve.BORDER, 0, 0)
            gd.BitmapSizeH(0, 0)

            gd.VertexFormat(0)
            Demo.draw_scroll(self, gd, scroll_frame)
            render(self, gd, sx, sy)
            gd.Display()
            gd.swap()
            gd.finish()

            (ty, tx) = struct.unpack("hh", gd.rd(eve.REG_TOUCH_SCREEN_XY, 4))
            touching = (tx != -32768)
            sx = (2 * tx - gd.w) / gd.w
            sy = -(2 * ty - gd.h) / gd.h
            if touching:
                if self.prev_touch is not None:
                    if (tx < 100 and ty < 100):
                        self.demo_mode = DEMO_TEAPOT
                        break
                    self.spin = trackball.trackball(self.prev_touch[0], self.prev_touch[1], sx, sy)
                    frame = 0
                self.prev_touch = (sx, sy)
            else:
                frame = frame + 1
                self.prev_touch = None
            self.curquat = trackball.add_quats(self.curquat, self.spin)
            scroll_frame += 1

        gd.cmd_graphicsfinish()
        gd.finish()
        t1 = time.monotonic()
        took = t1 - t0
        print(f"{N} frames took {took:.3f} s. {N / took:.2f} fps")
        if (self.demo_mode != DEMO_TEAPOT):
            self.demo_mode = DEMO_LOGO


import bteve2 as eve

LVDSRX_W = 1920
LVDSRX_H =1080

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

LVDSRX_SETUP        = 0x7F80_0500
LVDSRX_CTRL         = 0x7F80_0504
LVDSRX_STAT         = 0x7f80_0508
LVDSRX_BIST         = 0x7f80_050C
LVDSRX_SCRSIZE      = 0x7f80_0510
LVDSRX_TESTCFG      = 0x7f80_0514
LVDSRX_SETUP_VALUE  = 0x16
LVDSRX_CTRL_VALUE   = 0x8989
LVDSRX_TESTCFG_VALUE = 0

def Setup_Lvdsrx(gd, dst):
    if 1:
        #(lvdsrx_w, lvdsrx_h) = (LVDSRX_W, LVDSRX_H)
        gd.wr32(eve.REG_RX_SETUP, 1)
        #eve.wr32(eve.REG_RX_DEST, eve.SWAPCHAIN_2)
        gd.wr32(eve.REG_RX_DEST, dst.addr)
        gd.wr32(eve.REG_RX_FORMAT, dst.fmt)
        gd.wr32(eve.REG_RX_DITHER, 1)
    
        gd.wr32(eve.REG_RX_CAPTURE, 1)
        gd.wr32(eve.REG_RX_ENABLE, 1)
    
        gd.wr32(LVDSRX_SETUP, LVDSRX_SETUP_VALUE)  
        gd.wr32(LVDSRX_CTRL, LVDSRX_CTRL_VALUE)

        print(f"LVDSRX_SETUP = :     {gd.rd32(LVDSRX_SETUP):#x}")
        print(f"LVDSRX_CTRL = :     {gd.rd32(LVDSRX_CTRL):#x}")
        print(f"LVDSRX_STAT = :     {gd.rd32(LVDSRX_STAT):#x}")
        print(f"LVDSRX_BIST = :     {gd.rd32(LVDSRX_BIST):#x}")
        print(f"LVDSRX_SCRSIZE = :     {gd.rd32(LVDSRX_SCRSIZE):#x}")
        print(f"LVDSRX_TESTCFG = :     {gd.rd32(LVDSRX_TESTCFG):#x}")

    else:
        mode = 1
        gd.cmd_regwrite(eve.REG_LVF_HCYCLE,   dst.w + 100)
        gd.cmd_regwrite(eve.REG_LVF_HOFFSET,  50)
        gd.cmd_regwrite(eve.REG_LVF_HSIZE,    dst.w)
        gd.cmd_regwrite(eve.REG_LVF_HSYNC0,   0)
        gd.cmd_regwrite(eve.REG_LVF_HSYNC1,   10)
        gd.cmd_regwrite(eve.REG_LVF_VCYCLE,   dst.h + 100)
        gd.cmd_regwrite(eve.REG_LVF_VOFFSET,  50)
        gd.cmd_regwrite(eve.REG_LVF_VSIZE,    dst.h)
        gd.cmd_regwrite(eve.REG_LVF_VSYNC0,   0)
        gd.cmd_regwrite(eve.REG_LVF_VSYNC1,   10)
        gd.cmd_regwrite(eve.REG_LVF_MODE,     mode)

        gd.cmd_regwrite(eve.REG_RX_ENABLE, 1)
        gd.cmd_regwrite(eve.REG_RX_CAPTURE, 1)
        gd.cmd_regwrite(eve.REG_RX_SETUP, mode)
        gd.cmd_regwrite(eve.REG_RX_DEST, dst.addr)
        gd.cmd_regwrite(eve.REG_RX_FORMAT, dst.fmt)
        gd.cmd_regwrite(eve.REG_RX_DITHER, 1)

        gd.cmd_regwrite(eve.REG_CYA1, 1 << 4)       # Enable LVF

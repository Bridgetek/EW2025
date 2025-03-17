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

import trackball
import logo
from teapot import Teapot
from videocube import VideoCube

from lvdsrx_setup import Setup_Lvdsrx

LCDSCREEN_W = 1920
LCDSCREEN_H = 1200

DEMO_LOGO = 0
DEMO_VIDEOCUBE = 1
DEMO_TEAPOT = 2

class EW_demo:
    def __init__(self, gd):
        self.gd = gd
        self.w = LCDSCREEN_W
        self.h = LCDSCREEN_H
        self.demo_mode = DEMO_LOGO
        self.lvdsrx_setup_completed = 0
        self.fetched = False
        self.visibility = 255

    def main_run(self):
        gd = self.gd
        gd.cmd_loadpatch(0)
        with open("assets/patch1.patch", "rb") as patch:
            gd.load(patch)
        def cmd_region():
            gd.cc(struct.pack("I", 0xffffff_8f))
        def cmd_endregion(x, y, w, h):
            gd.cc(struct.pack("Ihhhh", 0xffffff_90, x, y, w, h))
        def cmd_vxy(dst, src0, src1, n):
            gd.cc(struct.pack("I4I", 0xffffff_91, dst, src0, src1, n))
        gd.cmd_region = cmd_region
        gd.cmd_endregion = cmd_endregion
        gd.cmd_vxy = cmd_vxy
        print("patch finish")

        while 1:
            if (self.demo_mode == DEMO_LOGO):
                gd.cmd_coldstart()
                logo.rotologo(self, gd)
            elif (self.demo_mode == DEMO_VIDEOCUBE):
                gd.cmd_coldstart()
                VideoCube.run(self, gd)
            elif (self.demo_mode == DEMO_TEAPOT):    
                gd.cmd_coldstart()
                gc.collect()
                (self.vertices, self.strips) = json.load(open("assets/teapot-geometry.json"))
                self.curquat = trackball.trackball(0, 0, 0, 0)                            
                Teapot.run(self, gd)
            else:
                pass
def main(gd):
    EW_demo(gd).main_run()

apprunner.run(main, False)
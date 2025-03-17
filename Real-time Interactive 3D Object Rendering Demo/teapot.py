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
from demo import Demo

DEMO_LOGO = 0
DEMO_VIDEOCUBE = 1
DEMO_TEAPOT = 2

class Teapot(Demo):
    def __init__(self):
        gc.collect()
        (self.vertices, self.strips) = json.load(open("assets/teapot-geometry.json"))
        self.curquat = trackball.trackball(0, 0, 0, 0)

    def run(self, gd):
        self.gd = gd
        def xform(self, xyz):
            rr = trackball.build_rotmatrix(self.curquat)
            x = np.dot(xyz, rr[0])
            y = -np.dot(xyz, rr[1])
            if 0:
                z = np.dot(xyz, rr[2])
                D = 300
                d = D / (D - z)
                q = 8 * (self.gd.h / 150) * d
            else:
                q = 8 * (self.gd.h / 150)
            return (
                np.array(x * q, dtype = np.int16),
                np.array(y * q, dtype = np.int16)
            )

        gd.Clear()
        Demo.setup_scroll(self, gd)
        gd.swap()

        vertices = self.vertices
        strips = self.strips

        vertex_array    = 0x000000
        draw_list       = 0x4040000
        sxa             = 0x4080000
        sya             = 0x40C0000

        gd.cmd_newlist(draw_list)
        gd.ColorRGB(255, 255, 255)
        gd.VertexTranslateX(gd.w // 2)
        gd.VertexTranslateY(gd.h // 2)
        gd.LineWidth(gd.w / 2000)
        for s in strips:
            gd.Begin(eve.LINE_STRIP)
            for i in s:
                gd.cmd_append(vertex_array + 4 * i, 4)
        gd.cmd_endlist()

        xyz = np.array(vertices)

        gc.collect()

        self.prev_touch = None
        self.spin = trackball.trackball(-.04, -.04, 0, 0)

        t0 = time.monotonic()
        N = 3000
        frame = 0
        scroll_frame = 0            
        while frame < N:
            (sx, sy) = xform(self, xyz)
            gd.cmd_memwrite(sxa, 2 * len(sx))
            gd.cc(sx.tobytes())
            gd.cmd_memwrite(sya, 2 * len(sy))
            gd.cc(sy.tobytes())
            gd.cmd_vxy(0, sxa, sya, 4 * len(sx))

            Demo.draw_scroll(self, gd, scroll_frame)
            gd.VertexFormat(3)
            gd.cmd_calllist(draw_list)
            gd.swap() 

            gd.finish()
            (ty, tx) = struct.unpack("hh", gd.rd(eve.REG_TOUCH_SCREEN_XY, 4))
            touching = (tx != -32768)
            sx = (2 * tx - gd.w) / gd.w
            sy = (gd.h - 2 * ty) / gd.h
            if touching:
                if self.prev_touch is not None:
                    if (tx < 100 and ty < 100):
                        self.demo_mode = DEMO_LOGO
                        break                        
                    self.spin = trackball.trackball(self.prev_touch[0], self.prev_touch[1], sx, sy)
                    self.spin *= 8
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
        self.demo_mode = DEMO_LOGO

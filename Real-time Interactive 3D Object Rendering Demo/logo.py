import sys
import math
import random
import struct
import time
import gc

CP = (sys.implementation.name == "circuitpython")

if CP:
    import ulab.numpy as np
else:
    import numpy as np

import bteve2 as eve
import apprunner
from logo_geometry import loops

DEMO_LOGO = 0
DEMO_VIDEOCUBE = 1
DEMO_TEAPOT = 2

def smoothstep(t):
    return t * t * (3.0 - 2.0 * t)


def rotologo(self, gd):
    class Allocator:
        def __init__(self, a):
            self.a = a
            self.base = a
        def alloc(self, n):
            r = self.a
            self.a += n
            return r

    sxa = Allocator(0xe3000)
    sya = Allocator(0xe6000)
    vertex_array = Allocator(0xe9000)

    class Loop:
        def __init__(self, gd, xys):
            a = np.array([(x, y, random.uniform(-math.pi, math.pi)) for (x,y) in xys[:-1]])

            self.center = np.mean(a, axis=0)
            self.xyz = (a - self.center)
            n = a.shape[0]

            self.vertex_array = vertex_array.alloc(4 * n)

            gd.VertexTranslateX(self.center[0])
            gd.VertexTranslateY(self.center[1])
            gd.Begin(eve.LINE_STRIP)
            gd.cmd_append(self.vertex_array, 4 * n)
            gd.cmd_append(self.vertex_array, 4)

    with open("assets/logo_prep.cmd", "rb") as f:
        gd.load(f)

    draw_list       = 0xf0000
    prev_touch = None

    gd.cmd_newlist(draw_list)
    gd.SaveContext()
    gd.ColorA(255)
    gd.cmd_cgradient(
        eve.CORNER_ZERO,
        0, 0, gd.w, gd.h,
        0xa0a0a0, 0xffffff)
    gd.RestoreContext()

    gd.VertexFormat(3)

    gd.Begin(eve.BITMAPS)

    gd.SaveContext()
    gd.ColorRGB(56, 113, 193)
    gd.Cell(0)
    gd.Vertex2f(260, 395)

    gd.ColorRGB(0, 0, 0)
    gd.Cell(1)
    gd.Vertex2f(260, 395)

    gd.RestoreContext()

    gd.ColorA(255)
    ll = [Loop(gd, l) for l in loops[:]]
    gd.cmd_endlist()

    xyz = np.concatenate(tuple([l.xyz for l in ll]))

    def xform(r, o):
        z = xyz[:,2]
        th = z + o
        x = xyz[:,0] + np.cos(th) * r
        y = xyz[:,1] + np.sin(th) * r
        return (
            np.array(8 * x, dtype = np.int16),
            np.array(8 * y, dtype = np.int16)
        )

    gd.finish(); 
    tt0 = time.monotonic()
    N = 1000
    gc.collect()
    i = 0
    while 1:
        t = 0.5 + (math.cos(-math.pi + (i / 120)) / 2)
        A = 0.05
        u = smoothstep(max((A - t) / A, 0))

        r = t * 160
        (sx, sy) = xform(r, i / 20)

        gd.cmd_memwrite(sxa.base, 2 * len(sx))
        gd.cc(eve.align4(sx.tobytes()))
        gd.cmd_memwrite(sya.base, 2 * len(sy))
        gd.cc(eve.align4(sy.tobytes()))
        gd.cmd_vxy(vertex_array.base, sxa.base, sya.base, 4 * len(sx))

        gd.ColorA(int(255 * u))
        gd.ColorRGB(0, 0, 0)
        gd.LineWidth(1.5 + 3 * u)
        gd.cmd_calllist(draw_list)
        gd.swap()

        (ty, tx) = struct.unpack("hh", gd.rd(eve.REG_TOUCH_SCREEN_XY, 4))
        touching = (tx != -32768)
        sx = (2 * tx - gd.w) / gd.w
        sy = -(2 * ty - gd.h) / gd.h
        if touching:
            if prev_touch is not None:
                self.demo_mode = DEMO_VIDEOCUBE
                break
            prev_touch = (sx, sy)
        else:
            prev_touch = None
            i += 1
        if (i >= N):
            gd.cmd_graphicsfinish()
            gd.finish(); 
            tt1 = time.monotonic()
            took = tt1 - tt0
            print(f"{N} frames took {took:.3f} s. {N / took:.2f} fps")                               
            i = 0
            gc.collect()                    
            print("Garbage collected. Free memory:", gc.mem_free())
            tt0 = time.monotonic() 

    gd.cmd_graphicsfinish()
    gd.finish(); 
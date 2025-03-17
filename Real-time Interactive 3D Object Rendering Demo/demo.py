import bteve2 as eve
import array
import struct

class Demo:
    def setup_scroll(self, gd):
        gd.BitmapHandle(63)
        #gd.cmd_loadimage(0xffffffff, 0)
        gd.cmd_loadimage(80 << 20, 0)
        with open("assets/Bridgetek_FA_dark_512.png", "rb") as f:
            gd.load(f)
        gd.BitmapSize(eve.NEAREST, eve.REPEAT, eve.BORDER, 0, 129)
        gd.BitmapSizeH(0, 0)

    def draw_scroll(self, gd, frame):
        gd.ClearColorRGB(0x1a, 0x1a, 0x1a)
        gd.Clear(1,1,1)

        gd.SaveContext()
        gd.VertexFormat(0)
        gd.BitmapHandle(63)
        gd.Begin(eve.BITMAPS)

        gd.ColorRGB(0x80, 0x80, 0x80)
        gd.BitmapTransformC(frame*256)
        for i in (0, 2, 4):
            gd.Vertex2f(0, 40 + 200 * i)
        gd.BitmapTransformC(-frame*256)
        for i in (1, 3, 5):
            gd.Vertex2f(0, 40 + 200 * i)
        gd.RestoreContext()

import struct
from collections import namedtuple

__version__ = '0.2.1'

from .registers import *
from sys import implementation
if implementation.name == "circuitpython":
    from ._eve import _EVE
else:
    from ._eve import _EVE
    
from .eve import align4, CoprocessorException, EVE2 as e_EVE2

Surface = namedtuple('Surface', ['addr', 'fmt', 'w', 'h'])

class EVE2(e_EVE2, _EVE):

    # These should actually be implemented in the CircuitPython/eve C++ layer

    def PaletteSource(self, addr):
        self.cI((42 << 24) | (((addr) & 0xffffff)))
    def PaletteSourceH(self, addr):
        self.cI((50 << 24) | (((addr >> 24) & 255)))
    def BitmapHandle(self, handle):
        self.cI((5 << 24) | ((handle & 63)))
    def Tag(self, s):
        self.cI((3 << 24) | ((s & 0xffffff)))

    def cI(self, x):
        self.cc(struct.pack("I", x))



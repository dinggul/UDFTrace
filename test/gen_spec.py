import struct

def dq(x): return struct.pack('<Q', x)
def dd(x): return struct.pack('<I', x)
def dw(x): return struct.pack('<H', x)
def db(x): return struct.pack('<B', x)

with open('spec2', 'wb') as f:
    p  = dq(1)
    p += dq(0x400526)
    p += dq(0)
    p += db(3) + db(5) + dw(0) + dd(0)
    p += dq(0)*5
    f.write(p)

with open('spec3', 'wb') as f:
    p  = dq(1)
    p += dq(0x400596)
    p += dq(0)
    p += db(6) + db(5) + dw(0) + dd(0)
    p += db(7) + db(6) + dw(0) + dd(8)
    p += dq(0)*4
    f.write(p)

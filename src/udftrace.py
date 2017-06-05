#!/usr/bin/env python2
from yaml import load as y_load, dump as y_dump
from struct import pack
import colorlog
import sys, os, re

UDF_ARG_MAX = 6

log_fmt = colorlog.ColoredFormatter(
        '[%(log_color)s%(levelname)s%(reset)s|%(filename)s:%(lineno)s] %(asctime)s > %(message)s',
        datefmt=None,
        reset=True,
        log_colors={
        'DEBUG': 'cyan',
        'INFO': 'green',
        'WARNING': 'yellow',
        'ERROR': 'red',
        'CRITICAL': 'red,bg_white',
        },
        secondary_log_colors={},
        style='%'
)

handler = colorlog.StreamHandler()
handler.setFormatter(log_fmt)

logger = colorlog.getLogger('UDFTrace')
logger.addHandler(handler)
logger.setLevel(10) # logging.DEBUG

DEBUG_DUMP = 'debug.dump'

ENUMS = [
    r'void',
    r'char',
    r'i32',
    r'i64',
    r'u32',
    r'u64',
    r'str',
    r'mem\((\d+)\)',
    r'mem\((0x[a-f0-9])+\)',
]

def mk_UDFArg(arg_type, nth_arg): # nth_arg == 0 for return value
    '''
    typedef struct {
        uint8_t type;   // argument type.
        uint8_t loc;    // argument location (stack or register).
        uint16_t ofs;   // used for location = stack arguments.
        uint32_t size;  // used for type = memory arguments.
    } UDFArg;
    '''
    assert nth_arg <= UDF_ARG_MAX
    matching_res = [0 if i is None else 1 for i in
                        map(lambda x: re.match(x, arg_type), ENUMS)]
    type_enum = matching_res.index(1)

    # set loc following linux x64 argument passing
    loc = 1
    if nth_arg == 1:
        loc = 5         # rdi
    elif nth_arg == 2:
        loc = 6         # rsi
    elif nth_arg == 3:
        loc = 4         # rdx
    elif nth_arg == 4:
        loc = 3         # rcx
    elif nth_arg == 5:
        loc = 9         # r8
    elif nth_arg == 6:
        loc = 10        # r9

    # set size for dumping memory
    size = 0
    if type_enum in [7, 8]:
        size_val_str = re.findall(ENUMS[type_enum], arg_type)[0]

        if size_val_str.startswith('0x'):
            size = int(size_val_str, 16)
            type_enum -= 1
        else:
            size = int(size_val_str)

    res = ''
    res += pack('<B', type_enum)    # type
    res += pack('<B', loc)          # loc
    res += pack('<H', 0)            # ofs
    res += pack('<I', size)         # size

    return res

def save_debug_dump(debug_info):
    fp = open(DEBUG_DUMP, 'wb')
    fp.write(pack("<Q", len(debug_info)))
    for di in debug_info:
        # start of format checking
        if len(di) != 4:
            logger.error('invalid format(debug unit) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if not isinstance(di[0], str):
            logger.error('invalid format(function name) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if not isinstance(di[1], int):
            logger.error('invalid format(function address) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if not isinstance(di[2], list):
            logger.error('invalid format(function argument) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if not isinstance(di[3], str):
            logger.error('invalid format(return format) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if len(di[2]) != 0 and not all(any(re.match(regex, item) for regex in ENUMS) for item in di[2]):
            logger.error('invalid format(function argument type) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if not any(re.match(regex, di[3]) for regex in ENUMS):
            logger.error('invalid format(return type) -- {}'.format(di))
            fp.close()
            os._exit(1)

        if len(di[2]) > 255:
            logger.error('invalid format(too many function arguments) -- {}'.format(di))
            fp.close()
            os._exit(1)

        # end of format checking

        # start of dumping

        fp.write(pack('<Q', di[1]))                                 # address
        fp.write(mk_UDFArg(di[3], 0))                               # ret

        for nth_arg in xrange(1, UDF_ARG_MAX+1):                    # args
            if len(di[2]) >= nth_arg:
                fp.write(mk_UDFArg(di[2][nth_arg-1], nth_arg))
            else:
                fp.write(mk_UDFArg('void', nth_arg))

        # end of dumping

    fp.close()
    return

def main():
    inp_name = 'input.yml' if len(sys.argv) < 2 else sys.argv[1]
    out_name = 'output' if len(sys.argv) < 3 else sys.argv[2]

    with open(inp_name, 'r') as f:
        debug_info = y_load(f.read())

    save_debug_dump(debug_info)

    return

if __name__ == '__main__':
    main()

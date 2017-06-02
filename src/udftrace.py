#!/usr/bin/env python2
from yaml import load as y_load, dump as y_dump
from struct import pack
import colorlog
import sys, os, re

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

def save_debug_dump(debug_info):
    fp = open(DEBUG_DUMP, 'wb')
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

        fp.write(pack("<B", len(di[2])))    # number of argument
        fp.write(pack("<Q", di[1]))         # function address

        for arg in di[2]:                   # function arguments
            for i, enum in enumerate(ENUMS):
                if re.match(enum, arg):
                    break
            fp.write(pack("<B", i))

            if 'mem' in arg:
                dump_length = re.findall(enum, arg)[0]
                if dump_length.startswith('0x'):
                    dump_length = int(dump_length, 16)
                else:
                    dump_length = int(dump_length)

                if dump_length >= 2 ** 32:
                    logger.error('invalid format(too long dump size) -- {}'.format(di))
                    fp.close()
                    os._exit(1)

                fp.write(pack("<I", dump_length))

        for i, enum in enumerate(ENUMS):    # return value
            if re.match(enum, di[3]):
                break
        fp.write(pack("<B", i))

        if 'mem' in di[3]:
            dump_length = re.findall(enum, di[3])[0]
            if dump_length.startswith('0x'):
                dump_length = int(dump_length, 16)
            else:
                dump_length = int(dump_length)

            if dump_length >= 2 ** 32:
                logger.error('invalid format(too long dump size) -- {}'.format(di))
                fp.close()
                os._exit(1)

            fp.write(pack("<I", dump_length))

    fp.close()
    return

def main():
    inp_name = 'input.yaml' if len(sys.argv) < 2 else sys.argv[1]
    out_name = 'output.yaml' if len(sys.argv) < 3 else sys.argv[2]

    with open(inp_name, 'r') as f:
        debug_info = y_load(f.read())

    save_debug_dump(debug_info)

    return

if __name__ == '__main__':
    main()

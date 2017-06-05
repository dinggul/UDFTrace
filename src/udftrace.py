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

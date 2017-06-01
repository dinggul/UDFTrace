#!/usr/bin/env python2
from yaml import load as y_load, dump as y_dump
import sys

def main():
    if len(sys.argv) < 2:
        inp_name = 'input.yml'
    else:
        inp_name = sys.argv[1]

    with open(inp_name, 'r') as f:
        debug_info = y_load(f.read())

    return

if __name__ == '__main__':
    main()

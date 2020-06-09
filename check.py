#!/bin/python3
import json
from os import system
from itertools import zip_longest

def err(s):
    print('\033[1m\033[91m' + s + '\033[0m\033[0m')
    print(data_out)
    print(spim_out)
    exit(1)

spim_file = "workdir/a.s";
f_json    = "workdir/a.json";
program   = "spim"
spim_in   = "workdir/0s.txt"
spim_out  = 'workdir/spim_out'
flags = '-file'

def only_nums(g):
    for s in g:
        try:
            i = int(s)
            yield i
        except ValueError:
            pass

with open("workdir/gcc_out", 'r') as data_out:
    ret = system(f"{program} {flags} {spim_file} < {spim_in} 2>/dev/null | sed -e s/'\(Enter an integer:\)\+'/''/ > {spim_out}")
    if ret != 0:
        with open(spim_out, 'r') as from_spim_r:
            err("runtime error occured when running your IR code\n")
    with open(spim_out, 'r') as from_spim_r:
        data_out = list(map(int, data_out))
        spim_out = list(only_nums(from_spim_r))
        for idx, (expect, user_out) in enumerate(zip_longest(data_out, spim_out)):
            if expect == None:
                err("Output mismatch!(you output more than supposed)")
            if user_out == None:
                err("Output mismatch!(you output less than supposed)")
            if expect != user_out:
                err("Output mismatch! expected %d, found %d at line %d" % (expect, user_out, idx));
        else:
            pass

exit(0)

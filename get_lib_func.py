import os
from subprocess import Popen, PIPE
from collections import Counter

path = raw_input('path: ').strip()
os.chdir(path)

cmds = raw_input('cmds: ').strip().split(' ')
cmds = ['ltrace'] + cmds
p = Popen(cmds, stdout=PIPE, stderr=PIPE)
data = p.stderr.read().split('\n')
data = [i.split('(', 1)[0] for i in data if '(' in i]
c = Counter(data)
print c

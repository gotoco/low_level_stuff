#!/usr/bin/python3
import subprocess
import re

def run_command(command):
    p = subprocess.Popen(command, shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT)
    return iter(p.stdout.readline, b'')

def decode(r_list):
    for e in result:
        s = e.decode('ascii')
        if "real" in s:
            time = s[s.find('\t')+1 : s.find('\n')]
            m = time[0:time.find('m')]
            s = time[time.find('m')+1:time.find('s')]
            return float(m)*60.0+float(s)
    return -1

def build_cmd(N=1000, K=1000, T="V", bin_name="./a.out"):
    cmd = "time "
    cmd += bin_name
    cmd += " "
    cmd += T
    cmd += " "
    cmd += str(N)
    cmd += " "
    cmd += str(K)
    return cmd

res = []
print("LinkedLists: ")
for i in range(1000, 100000, 1000):
    cmd = build_cmd(i, i, "L")
    result = list(run_command(cmd))
    v = decode(result)
    print(v)
    res.append(v)

print(res)

vres = []
print("Vectors: ")
for i in range(1000, 100000, 1000):
    cmd = build_cmd(i, i, "V")
    result = list(run_command(cmd))
    v = decode(result)
    print(v)
    vres.append(v)

print(vres)

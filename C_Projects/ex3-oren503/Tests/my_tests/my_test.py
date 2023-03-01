import os
import shutil
import sys
import subprocess
from subprocess import Popen, PIPE
from random import randint

SEPARATOR = "\n======================================="
PATH = "../../tweetsGenerator.c"
NAME = "tweetsGenerator"

def print_error(test_name, input, return_got, return_get, print_got, print_get):
    print(f"Test name: {test_name}\n"
          f"Input was: '{input}'\n"
          f"Return value: Got - {return_got}; Should get - {return_get}\n"
          f"Print: Got - '{print_got}'; Should get - '{print_get}'\n")

def test_compilation():
    s = subprocess.run(f"gcc -std=c99 -Wall -Wextra -Wvla {PATH} -o {NAME}",
                       capture_output=True, text=True, shell=True)
    if s.returncode != 0:
        print("COMPILATION FAILED:\n" + s.stderr)
        exit()
    print("COMPILATION SUCCEEDED" + SEPARATOR)

def run_valgrind():
    s = subprocess.run(f"valgrind --leak-check=full --log-file=t2.txt {PATH}")
    leaks = True
    with open("t1.txt", 'r') as log:
        for line in log.readlines():
            if "no leaks are possible" in line:
                leaks = False
        if leaks:
            print("valgrind found memory leaks, check your code for allocation/freeing errors and run the tests again")
            return 1
    return 0


def test_arguments():
    inputs = ["454545 30", "234", "345 30 ./usr/bin 34 231", "", "234", "#$@#"]
    for input in inputs:
        s = subprocess.run(f"{PATH[1:-2]} {input}", capture_output=True, text=True, shell=True)
        if (not s.stdout.startswith("Usage:")) or (s.returncode != 1):
            print_error('test_arguments', input, s.returncode, 1, s.stdout, "Usage:....")
            exit()
    print("ARGUMENTS TEST PASSED" + SEPARATOR)


def test_open_file():
    # good = subprocess.run(f"{PATH[1:-2]} 454545 30 ../justdoit_tweets.txt 100",
    #                       capture_output=True, text=True, shell=True)
    # if (good.returncode != 0) or (good.stdout != ""):
    #     print_error('test_open_file', "454545 30 ../justdoit_tweets.txt 100", good.returncode, 0, good.stdout, "''")
    bads = ["454545 30  100", "454545 30 $# 100", "454545 30 ../t2.txt 34", "454545 30 ./foo.txt 34"]
    for bad in bads:
        s = subprocess.run(f"{PATH[1:-2]} {bad}", capture_output=True, text=True, shell=True)
        if (not s.stdout.startswith("Error:")) or (s.returncode != 1):
            print_error('test_open_file', bad, s.returncode, 1, s.stdout, "Error:....")
            exit()
    print("FILE OPENING TEST PASSED" + SEPARATOR)


if __name__ == "__main__":
    test_compilation()
    # run_valgrind()
    test_arguments()
    test_open_file()

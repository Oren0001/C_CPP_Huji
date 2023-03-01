import subprocess
import sys


SUCCESS = 0
FAIL = 1
NAME = "manageStudents"
SEPARATOR_L = "\n=========================================\n"
SEPARATOR_S = "\n---------------------------\n"


def test_compilation():
    program = subprocess.run(f"gcc -std=c99 -Wall -Wextra -Wvla ..\..\manageStudents.c -o {NAME}",
                             capture_output=True, text=True, shell=True)
    if not program.returncode:
        print("PROGRAM COMPILED SUCCESSFULLY." + SEPARATOR_L)
    else:
        print("COULD NOT COMPILE THE PROGRAM. DETAILS:\n"
              "STDOUT = " + program.stdout + "\n"
              "STDERR = " + program.stderr)
        exit()

def test_check_arguments():
    success = ["best", "bubble", "quick"]
    fail = ["", "    ", "bubbl", "best bubble", "1231$#23", "quicky", "besty", "use best"]

    for operation in success:
        p = subprocess.run(NAME + " " + operation, capture_output=True, text=True, shell=True)
        if p.returncode or p.stdout:
            print(f"INPUT: '{NAME} {operation}'\n"
                  f"GOT: '{p.stdout}' | returncode = {p.returncode} | stderr = {p.stderr}\n"
                  "SHOULD GET: '' | returncode = 0\n" + SEPARATOR_S)

    for operation in fail:
        p = subprocess.run(NAME + " " + operation, capture_output=True, text=True, shell=True)
        if (not p.returncode) or (not p.stdout.startswith("USAGE: ")):
            print(f"INPUT: '{NAME} {operation}'\n"
                  f"GOT: '{p.stdout}' | returncode = {p.returncode} | stderr = {p.stderr}\n"
                  "SHOULD GET: 'USAGE: ...' | returncode = 1" + SEPARATOR_S)



def test_get_number_of_students():
    p = subprocess.run(f"{NAME} best", stdin=open("my_test_files/num1.txt", "r"),
                       capture_output=True, text=True, shell=True)


if __name__ == "__main__":
    test_compilation()
    test_check_arguments()
    test_get_number_of_students()

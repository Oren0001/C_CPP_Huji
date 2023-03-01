import difflib
import os
import subprocess


def check_stdout_validity(cmp_file, out_file):
    with open(cmp_file, 'r') as cmp:
        with open(out_file, 'r') as out:
            cmp_lines = cmp.readlines()
            out_lines = out.readlines()
            if len(cmp_lines) != len(out_lines):
                print("Length of files should be equal, check for difference in outputs")
                print("Test failed!")
                return 1
            for i in range(len(cmp_lines) - 1):
                curr_cmp_line = cmp_lines[i]
                curr_out_line = out_lines[i]
                if curr_cmp_line.startswith("Enter ") and not curr_out_line.startswith("Enter "):
                    print("Message should start with Enter: ,like\n", curr_cmp_line)
                    print("Test failed!\n")
                    return 1
                if curr_cmp_line.startswith("ERROR: "):
                    if not curr_out_line.startswith("ERROR: "):
                        print("Message should start with \\'ERROR: \\' !")
                        print("Test failed!\n")
                        return 1
                    if "," in curr_cmp_line:
                        split = curr_cmp_line.strip("\n").split(',')
                        if split[1] not in curr_out_line and split[2] not in curr_out_line \
                                and split[3] not in curr_out_line:
                            print(
                                "Expected either {x} or {y} or {z} to appear in message".format(x=split[1], y=split[2],
                                                                                                z=split[3]))
            if cmp_lines[-1].strip("\\n") != out_lines[-1].strip("\n"):
                print("Best student info did not match, check output and compare")
                print("Test failed!")
                return 1
            return 0


def arg_tests():
    try:
        compiled_args = subprocess.run("manageStudents", timeout=5, capture_output=True, text=True, shell=True)
    except subprocess.TimeoutExpired:
        print("Timeout occurred, check for infinite loops when not inserting arguments")
        return 1
    if compiled_args.returncode != 1 or not compiled_args.stdout.startswith("USAGE: "):
        print("return code should be 1, and stdout should have a message beginning with USAGE: ")
        return 1
    try:
        compiled_args = subprocess.run("manageStudents bla", timeout=5, capture_output=True, text=True, shell=True)
    except subprocess.TimeoutExpired:
        print("Timeout occurred, check for infinite loops when inserting non-valid arguments")
        return 1
    if compiled_args.returncode != 1 or not compiled_args.stdout.startswith("USAGE: "):
        print("return code should be 1, and stdout should have a message beginning with USAGE: ")
        return 1
    return 0


def students_validity_tests():
    with open("out_files/student_list.out", 'w') as fOut:
        with open("in_files/student_list.in", 'r') as fIn:
            try:
                p = subprocess.run(
                    "valgrind --leak-check=full --tool=memcheck --show-leak-kinds=all --track-origins=yes --verbose "
                    "--error-exitcode=1 -v --log-file=valgrind-out.txt manageStudents best", stdin=fIn,
                    stdout=fOut, text=True, shell=True, timeout=5)
            except subprocess.TimeoutExpired:
                print("input validity " + "\nTest - FAIL")
                print("TIMEOUT Reached")
                return 1

    return check_stdout_validity("cmp_files/student_list.cmp", "out_files/student_list.out")


def check_leaks():
    leaks = True
    with open("valgrind-out.txt", 'r') as valgrind_log:
        val_lines = valgrind_log.readlines()
        for line in val_lines:
            if "no leaks are possible" in line:
                leaks = False
        if leaks:
            print("valgrind found memory leaks, check your code for allocation/freeing errors and run the tests again")
            return 1
    return 0


def valgrind_test():
    print("running valgrind with full check and debug mode")
    with open("in_files/val.in", 'r') as val_in:
        with open("out_files/val.out", 'w') as val_out:
            try:
                valgrind = subprocess.run(
                    "valgrind --leak-check=full --tool=memcheck --show-leak-kinds=all --track-origins=yes --verbose "
                    "--error-exitcode=1 -v --log-file=valgrind-out.txt manageStudents best", stdin=val_in,
                    stdout=val_out, text=True, shell=True, timeout=5)
            except subprocess.TimeoutExpired:
                print("Timeout occurred, test stopping")
                return 1
    return check_leaks()


def remove_files():
    for file in os.listdir("out_files"):
        path = "out_files/" + file
        os.remove(path)


def cmp_files(out_file, test_file, test_name):
    with open(test_file, 'r') as test:
        with open(out_file, 'r') as output:
            diff = difflib.ndiff(test.readlines(), output.readlines())
            maybe_error = False

            for i, line in enumerate(diff):
                if line.startswith("? "):
                    pass
                elif maybe_error:
                    if line.startswith("+ ERROR: "):
                        maybe_error = False
                    else:
                        print(test_name + "\nTest - FAIL")
                        print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                        return 1

                elif line.startswith("- "):
                    if line.startswith("- ERROR: "):
                        maybe_error = True
                    else:
                        print(test_name + "\nTest - FAIL")
                        print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                        return 1

                elif line.startswith("+ "):
                    print(test_name + "\nTest - FAIL")
                    print("Difference found in '" + out_file + "' at line " + str(i) + "\n")
                    return 1


def best():
    for file in os.listdir("in_files"):
        if "best" in file:
            out_name = "out_files/" + file[:-3] + ".out"
            cmp_name = "cmp_files/" + file[:-3] + ".cmp"
            with open("in_files/" + file, 'r') as fIn:
                with open(out_name, 'w') as fOut:
                    try:
                        p = subprocess.run(
                            "valgrind --leak-check=full --tool=memcheck --show-leak-kinds=all --track-origins=yes "
                            "--verbose "
                            "--error-exitcode=1 -v --log-file=valgrind-out.txt manageStudents best", stdin=fIn,
                            stdout=fOut, text=True, shell=True, timeout=5)
                    except subprocess.TimeoutExpired:
                        print(file + "\nTest - FAIL")
                        print("TIMEOUT Reached")
                        return 1
            if check_leaks():
                return 1
            if cmp_files(out_name, cmp_name, file):
                return 1
    return 0


def quick():
    for file in os.listdir("in_files"):
        if "quick" in file:
            out_name = "out_files/" + file[:-3] + ".out"
            cmp_name = "cmp_files/" + file[:-3] + ".cmp"
            with open("in_files/" + file, 'r') as fIn:
                with open(out_name, 'w') as fOut:
                    try:
                        p = subprocess.run(
                            "valgrind --leak-check=full --tool=memcheck --show-leak-kinds=all --track-origins=yes "
                            "--verbose "
                            "--error-exitcode=1 -v --log-file=valgrind-out.txt manageStudents quick", stdin=fIn,
                            stdout=fOut, text=True, shell=True, timeout=5)
                    except subprocess.TimeoutExpired:
                        print(file + "\nTest - FAIL")
                        print("TIMEOUT Reached")
                        return 1
            if check_leaks():
                return 1
            if cmp_files(out_name, cmp_name, file):
                return 1
    return 0


def bubble():
    for file in os.listdir("in_files"):
        if "bubble" in file:
            out_name = "out_files/" + file[:-3] + ".out"
            cmp_name = "cmp_files/" + file[:-3] + ".cmp"
            with open("in_files/" + file, 'r') as fIn:
                with open(out_name, 'w') as fOut:
                    try:
                        p = subprocess.run(
                            "valgrind --leak-check=full --tool=memcheck --show-leak-kinds=all --track-origins=yes "
                            "--verbose "
                            "--error-exitcode=1 -v --log-file=valgrind-out.txt manageStudents bubble", stdin=fIn,
                            stdout=fOut, text=True, shell=True, timeout=5)
                    except subprocess.TimeoutExpired:
                        print(file + "\nTest - FAIL")
                        print("TIMEOUT Reached")
                        return 1
            if check_leaks():
                return 1
            if cmp_files(out_name, cmp_name, file):
                return 1
    return 0


if __name__ == '__main__':
    print("cleaning files from out files")
    remove_files()
    print("Compiling file with debug option for valgrind memory leaks check...")

    compiled = subprocess.run('gcc -Wextra -Wall -Wvla -std=c99 -ggdb3 -lm D:\Drive_Backup\Projects\C_CPP\C_Project\ex2-oren503\manageStudents.c -o '
                              'manageStudents', capture_output=True, text=True, shell=True)

    if compiled.returncode != 0:
        print("Program failed compiling")
        exit(1)
    print("Program compiled successfully\n\n")
    if valgrind_test():
        exit(1)
    if compiled.returncode != 0:
        print("Program failed compiling")
        exit(1)
    print("Program compiled_args successfully\n\n")
    print("running argument tests:")
    if arg_tests():
        exit(1)
    print("passed argument tests!")
    print("running input validity tests:")
    if students_validity_tests():
        exit(1)
    print("passed input validity tests!")
    print("testing best tests:")
    if best():
        exit(1)
    print("passed best tests!")
    print("testing quick tests:")
    if quick():
        exit(1)
    print("passed quick tests!")
    print("testing bubble tests:")
    if bubble():
        exit(1)
    print("passed bubble tests!")
    print("passed all tests!")

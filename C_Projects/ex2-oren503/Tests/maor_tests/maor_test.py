import os
import shutil
import sys
import subprocess
from subprocess import Popen, PIPE
from random import randint

TEST_PATH = "test_files"
BIN_PATH = os.path.join("test_files", "../../manageStudents")
VALID_INFO_TEST_NUM = 1000
SMALL_TEST_NUM = 5000
BIG_TEST_NUM = 1000
GIANT_TEST_NUM = 500


def setup():
    try:
        if os.path.isdir(TEST_PATH):
            shutil.rmtree(TEST_PATH)
        os.mkdir(TEST_PATH)
    except:
        print("Failed to create test file directory. Exiting")
        exit()

    c = subprocess.run(
        "gcc -w -Wextra -Wall -Wvla -std=c99 -lm ../../manageStudents.c -o {0}".format(
            BIN_PATH),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        shell=True)

    if c.stdout != b'':
        shutil.rmtree(TEST_PATH)
        print("\nSomething didn't go right when compiling your C source "
              "code! Output was: \n" + str(c.stdout))
        exit()


def make_info_best(n):
    best_ratio = 0
    best_student = ''
    to_stdin = f"{n}\n"
    for i in range(n):
        tz = randint(10**9, 10**10)
        grade = randint(0, 100)
        age = randint(18, 120)
        if best_student == '':
            best_student = f"best student info is: {tz},{grade},{age}"
        if grade / age > best_ratio:
            best_ratio = grade / age
            best_student = f"best student info is: {tz},{grade},{age}"

        to_stdin += f"{tz},{grade},{age}\n"
    return to_stdin, best_student


def progress(count, total, suffix=''):
    bar_len = 60
    filled_len = int(round(bar_len * count / float(total)))

    percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)

    sys.stdout.write('[%s] %s%s ...%s\r' % (bar, percents, '%', suffix))
    sys.stdout.flush()


def check_return_code(actual, excepted, to_stdin):
    if actual != excepted:
        print(f"WRONG RETURN CODE ERROR:your program return {actual} instead of {excepted} ")
        print(f"students info was:\n {to_stdin}")
        keep = ''
        while keep != 'y':
            keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
            if keep == 'n':
                sys.exit()
        return 1
    return 0


def best_student_check(size, min_students, max_students):
    if size == SMALL_TEST_NUM:
        print("Small input test start")
    elif size == BIG_TEST_NUM:
        print("Big input test start")
    elif size == GIANT_TEST_NUM:
        print("Giant input test start")

    counter = 0
    for i in range(size):
        n = randint(min_students, max_students)
        to_stdin, best_student = make_info_best(n)
        p = Popen([BIN_PATH, "best"], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        stdout_data = p.communicate(input=to_stdin)[0]
        best_student_output = stdout_data.split('\n')[-2]
        p.kill()

        if best_student_output != best_student:
            counter += 1
            print(f"Fail test number: {i+1}/{size}")
            print(f"TEST DATA BELOW\n"
                  f"Number of students {n}\n"
                  f'excepted output was: {best_student}\n'
                  f'your output was: {stdout_data}\n'
                  f"students info was:\n {to_stdin}")

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        if check_return_code(p.returncode, 0, to_stdin) != 0:
            counter += 1
        else:
            progress(i, size-1)

    print(f"\nPass {size-counter} / {size}")


def bubble_check_order(output, ind_dict):
    try:
        for i in range(len(output)):
            s, grade, a = output[i].split(",")
            if i not in ind_dict[grade]:
                return "fail"
    except:
        print("TEST FAILED DUE TO PYTHON EXCEPTION. PROBABLY BECAUSE YOUR PRINT DIDN'T CONTAIN ALL OF THE STUDENTS OR"
              " YOU PRINTED IN WRONG FORMAT, FOR EXAMPLE FOR 'n' STUDENT YOUR 'n' LAST LINE OF THE OUTPUT EACH SHOULD "
              "CONTAIN ONE STUDENT INFO")


def make_info_bubble_sort(n):
    students_info_dict = {}
    to_stdin = f"{n}\n"
    for i in range(n):
        tz = randint(10**9, 10**10)
        grade = randint(0, 100)
        age = randint(18, 120)
        random_addition = randint(0, 5)
        if grade in students_info_dict:
            students_info_dict[grade] += [f"{tz},{grade},{age}"]
        else:
            students_info_dict[grade] = [f"{tz},{grade},{age}"]
        if random_addition == 0:
            to_stdin += f"{tz},{grade},{age}\n"
        elif random_addition == 1:
            to_stdin += f"{tz},0{grade},{age}\n"
        elif random_addition == 2:
            to_stdin += f"{tz},{grade},0{age}\n"
        elif random_addition == 3:
            to_stdin += f"{tz},00{grade},{age}\n"
        elif random_addition == 4:
            to_stdin += f"{tz},{grade},00{age}\n"
        else:
            to_stdin += f"{tz},000{grade},00{age}\n"

    dict_sorted_items = sorted(students_info_dict.items())
    sorted_dict = {}
    ind_dict = {} # {grade:index range}
    ind = 0
    for item in dict_sorted_items:
        sorted_dict[str(item[0])] = item[1]
        ind_dict[str(item[0])] = range(ind, ind + len(item[1]))
        ind += len(item[1])

    return to_stdin, ind_dict, sorted_dict


def bubble_sort_check(size, min_students, max_students):
    if size == SMALL_TEST_NUM:
        print("Small input test start")
    elif size == BIG_TEST_NUM:
        print("Big input test start")
    elif size == GIANT_TEST_NUM:
        print("Giant input test start")

    counter = 0
    for i in range(size):
        n = randint(min_students, max_students)
        to_stdin, ind_dict, sorted_dict = make_info_bubble_sort(n)
        p = Popen([BIN_PATH, "bubble"], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        stdout_data = p.communicate(input=to_stdin)[0]
        output = stdout_data.split('\n')[-n-1:-1]

        p.kill()
        status = bubble_check_order(output, ind_dict)
        if status == 'fail':
            counter += 1
            print(f"Fail test number: {i + 1}/{size}")
            print(f"TEST DATA BELOW\n"
                  f"Number of students {n}\n"
                  f'excepted output was: {sorted_dict}\n'
                  f'your output was: {output}\n'
                  f'"students info was:\n {to_stdin}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        if check_return_code(p.returncode, 0, to_stdin) != 0:
            counter += 1
        else:
            progress(i, size-1)

    print(f"\nPass {size - counter} / {size}")


def quick_check_order(output, ind_dict):
    try:
        for i in range(len(output)):
            s, g, age = output[i].split(",")
            if i not in ind_dict[age]:
                return "fail"
    except:
        print("TEST FAILED DUE TO PYTHON EXCEPTION. PROBABLY BECAUSE YOUR PRINT DIDN'T CONTAIN ALL OF THE STUDENTS OR"
              " YOU PRINTED IN WRONG FORMAT, FOR EXAMPLE FOR 'n' STUDENT YOUR 'n' LAST LINE OF THE OUTPUT EACH SHOULD "
              "CONTAIN ONE STUDENT INFO")


def make_info_quick_sort(n):
    students_info_dict = {}
    to_stdin = f"{n}\n"
    for i in range(n):
        tz = randint(10**9, 10**10)
        grade = randint(0, 100)
        age = randint(18, 120)
        random_addition = randint(0,5)
        if age in students_info_dict:
            students_info_dict[age] += [f"{tz},{grade},{age}"]
        else:
            students_info_dict[age] = [f"{tz},{grade},{age}"]
        if random_addition == 0:
            to_stdin += f"{tz},{grade},{age}\n"
        elif random_addition == 1:
            to_stdin += f"{tz},0{grade},{age}\n"
        elif random_addition == 2:
            to_stdin += f"{tz},{grade},0{age}\n"
        elif random_addition == 3:
            to_stdin += f"{tz},00{grade},{age}\n"
        elif random_addition == 4:
            to_stdin += f"{tz},{grade},00{age}\n"
        else:
            to_stdin += f"{tz},000{grade},00{age}\n"

    dict_sorted_items = sorted(students_info_dict.items())
    sorted_dict = {}
    ind_dict = {} # {grade:index range}
    ind = 0
    for item in dict_sorted_items:
        sorted_dict[str(item[0])] = item[1]
        ind_dict[str(item[0])] = range(ind, ind + len(item[1]))
        ind += len(item[1])

    return to_stdin, ind_dict, sorted_dict


def quick_sort_check(size, min_students, max_students):
    if size == SMALL_TEST_NUM:
        print("Small input test start")
    elif size == BIG_TEST_NUM:
        print("Big input test start")
    elif size == GIANT_TEST_NUM:
        print("Giant input test start")

    counter = 0
    for i in range(size):
        n = randint(min_students, max_students)
        to_stdin, ind_dict, sorted_dict = make_info_quick_sort(n)
        p = Popen([BIN_PATH, "quick"], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        stdout_data = p.communicate(input=to_stdin)[0]
        output = stdout_data.split('\n')[-n-1:-1]
        p.kill()

        status = quick_check_order(output, ind_dict)
        if status == 'fail':
            counter += 1
            print(f"Fail test number: {i + 1}/{size}")
            print(f"TEST DATA BELOW\n"
                  f"Number of students {n}\n"
                  f'excepted output was: {sorted_dict}\n'
                  f'your output was: {output}\n'
                  f'"students info was:\n {to_stdin}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        if check_return_code(p.returncode, 0, to_stdin) != 0:
            counter += 1
        else:
            progress(i, size-1)

    print(f"\nPass {size - counter} / {size}")


def check_command():
    invalid_cmd = ['quickk', 'bestt', 'bubblee', 'besta', 'buble', ' quic']
    counter = 0
    for i in range(len(invalid_cmd)):
        p = Popen([BIN_PATH, invalid_cmd[i]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.stdout.read()
        p.kill()
        if "USAGE: " not in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{len(invalid_cmd)}")
            print(f"TEST DATA BELOW\n"
                  f'excepted output was: USAGE: ...\n'
                  f'your output was: {output}\n'
                  f'"invalid command was:\n {invalid_cmd[i]}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        else:
            progress(i, len(invalid_cmd)-1)

    print(f"\nPass {len(invalid_cmd) - counter} / {len(invalid_cmd)}")


def check_id():
    cmd = ['best', 'bubble', 'quick']
    invalid_id = ['123456789,34,63', '12345678911,23,74', '0123456789,100,100', '01234567891,12,33',
                  '001234567891,123,18', '10000000000,33,24']
    counter = 0
    for i in range(len(invalid_id)):
        ind = randint(0, 2)
        student_num = randint(1, 50)
        p = Popen([BIN_PATH, cmd[ind]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.communicate(input=f"{student_num}\n{invalid_id[i]}\n")[0]
        p.kill()

        if "ERROR: " not in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{len(invalid_id)}")
            print(f"TEST DATA BELOW\n"
                  f'excepted output was: ERROR: ...\n'
                  f'your output was: {output}\n'
                  f'"invalid info was:\n {invalid_id[i]}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()

        else:
            progress(i, len(invalid_id)-1)

    print(f"\nPass {len(invalid_id) - counter} / {len(invalid_id)}")


def check_age():
    cmd = ['best', 'bubble', 'quick']
    invalid_age = ['1234567891,44,4', '1234567891,56,17', '1234567891,60,-2', '1234567891,40,123']
    counter = 0
    for i in range(len(invalid_age)):
        ind = randint(0,2)
        student_num = randint(1,50)
        p = Popen([BIN_PATH, cmd[ind]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.communicate(input=f"{student_num}\n{invalid_age[i]}\n")[0]
        p.kill()

        if "ERROR: " not in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{len(invalid_age)}")
            print(f"TEST DATA BELOW\n"
                  f'excepted output was: ERROR: ...\n'
                  f'your output was: {output}\n'
                  f'"invalid info was:\n {invalid_age[i]}')


            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        else:
            progress(i, len(invalid_age)-1)

    print(f"\nPass {len(invalid_age) - counter} / {len(invalid_age)}")


def check_grade():
    cmd = ['best', 'bubble', 'quick']
    invalid_grade = ['1234567891,-1,34', '1234567891,-51,18', '4544567891,1230,58', '3234567891,215,118']
    counter = 0
    for i in range(len(invalid_grade)):
        ind = randint(0, 2)
        student_num = randint(1, 50)
        p = Popen([BIN_PATH, cmd[ind]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.communicate(input=f"{student_num}\n{invalid_grade[i]}\n")[0]
        p.kill()

        if "ERROR: " not in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{len(invalid_grade)}")
            print(f"TEST DATA BELOW\n"
                  f'excepted output was: ERROR: ...\n'
                  f'your output was: {output}\n'
                  f'"invalid info was:\n {invalid_grade[i]}')


            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        else:
            progress(i, len(invalid_grade)-1)

    print(f"\nPass {len(invalid_grade) - counter} / {len(invalid_grade)}")


def check_format():
    cmd = ['best', 'bubble', 'quick']
    invalid_format = ['1234567891d,24,34', '1234a56791,51,18', '4544567891,3s0,58', '3234567891,25,19g', "10d,43,18", "sdfsdfsg,51,61",
                      ",,", "1234567891,,20", "1234567890,46,", ",54,23"]
    counter = 0
    for i in range(len(invalid_format)):
        ind = randint(0, 2)
        student_num = randint(1, 50)
        p = Popen([BIN_PATH, cmd[ind]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.communicate(input=f"{student_num}\n{invalid_format[i]}\n")[0]
        p.kill()

        if "ERROR: " not in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{len(invalid_format)}")
            print(f"TEST DATA BELOW\n"
                  f'excepted output was: ERROR: ...\n'
                  f'your output was: {output}\n'
                  f'"invalid info was:\n {invalid_format[i]}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()
        else:
            progress(i, len(invalid_format)-1)

    print(f"\nPass {len(invalid_format) - counter} / {len(invalid_format)}")


def check_valid(size, min_students, max_students):
    cmd = ['best', 'bubble', 'quick']
    counter = 0
    for i in range(size):
        num_students = randint(min_students, max_students)
        valid_info, g, g1 = make_info_bubble_sort(num_students)  # g, g1 are garbage
        ind = randint(0, 2)
        p = Popen([BIN_PATH, cmd[ind]], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
        output = p.communicate(input=valid_info)[0]
        p.kill()

        if "ERROR" in output or "USAGE" in output or "Error" in output or "Usage" in output:
            counter += 1
            print(f"Fail test number: {i + 1}/{size}")
            print(f"TEST DATA BELOW\n"
                  f'output should not contain  "Error:" or "Usage" and so...\n'
                  f'your output was: {output}\n'
                  f'"valid info was:\n {valid_info}')

            keep = ''
            while keep != 'y':
                keep = input("Enter 'n' to stop all tests, enter to keep going enter 'y'")
                if keep == 'n':
                    sys.exit()

        else:
            progress(i, size-1)

    print(f"\nPass {size - counter} / {size}")


if __name__ == "__main__":
    setup()
    print("BEST STUDENT TEST:")
    best_student_check(SMALL_TEST_NUM, 1, 20)
    best_student_check(BIG_TEST_NUM, 10, 1000)
    best_student_check(GIANT_TEST_NUM, 100, 10000)
    print("BUBBLE SORT TEST:")
    bubble_sort_check(SMALL_TEST_NUM, 1, 20)
    bubble_sort_check(BIG_TEST_NUM, 10, 500)
    bubble_sort_check(GIANT_TEST_NUM, 100, 1000)
    print("QUICK SORT TEST:")
    quick_sort_check(SMALL_TEST_NUM, 1, 20)
    quick_sort_check(BIG_TEST_NUM, 10, 500)
    quick_sort_check(GIANT_TEST_NUM, 100, 1000)
    print("VALID INFO TEST")
    check_valid(VALID_INFO_TEST_NUM, 1, 1000)
    print("INVALID AGE TEST:")
    check_age()
    print("INVALID GRADE TEST:")
    check_grade()
    print("INVALID FORMAT TEST:")
    check_format()
    print("INVALID ID TEST:")
    check_id()
    print("INVALID COMMAND TEST:")
    check_command()

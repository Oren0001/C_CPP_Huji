#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define OPERATION_1 "best"
#define OPERATION_2 "bubble"
#define OPERATION_3 "quick"
#define USAGE_FORMAT "USAGE: "
#define ERROR_FORMAT "ERROR: "
#define MAX_LINE_LEN 60
#define MAX_FIELD_LEN 19
#define ID_LENGTH 10
#define BAD_DIGIT_FIRST '0'
#define FIELDS_SEPARATOR ",|\n"
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define ID_ERROR "Id must have 10 digits and can not start with zero.\n"
#define GRADE_ERROR "Grade must be an integer between 0 and 100 (includes).\n"
#define AGE_ERROR "Age must be an integer between 18 and 120 (includes).\n"
#define BASE 10

/* A struct which represents students by their id, grade and age. */
typedef struct Student {
  long int id;
  int grade;
  int age;
} Student;

/**
 * Goes over the list of students given by the user, and prints their info.
 * @param students A pointer to the first student.
 * @param students_num The total number of students.
 */
void print_students_info (Student *students, long int students_num)
{
  for (long int i = 0; i < students_num; ++i)
    printf ("%ld,%d,%d\n", students[i].id, students[i].grade, students[i].age);
}

/**
 * Gets two pointers and swaps their values.
 * @param p_1 First pointer.
 * @param p_2 Second pointer.
 */
void swap (Student *p_1, Student *p_2)
{
  Student temp = *p_1;
  *p_1 = *p_2;
  *p_2 = temp;
}

/**
 * Uses the student at index end-1 as a pivot. All the students left to the
 * pivot will have lower age than the student at the pivot,
 * and all the students right to the pivot will have higher age than him.
 * @param start A pointer to the beginning of the array which contains
 *              the students.
 * @param end A pointer to the end of the array which contains the students.
 * @return The pivot.
 */
Student *partition (Student *start, Student *end)
{
  size_t students_num = end - start;
  long i = -1;
  for (size_t j = 0; j < students_num - 1; ++j)
    {
      if ((start + j)->age <= (end - 1)->age)
        {
          i++;
          swap (start + i, start + j);
        }
    }
  swap (start + i + 1, end - 1);
  return start + i + 1;
}

/**
 * Sorts the list of students that the user have inserted, by their age.
 * @param start A pointer to the beginning of the array which contains
 *              the students.
 * @param end A pointer to the end of the array which contains the students.
 */
void quick_sort (Student *start, Student *end)
{
  if (start < end)
    {
      Student *pivot = partition (start, end);
      quick_sort (start, pivot);
      quick_sort (pivot + 1, end);
    }
}

/**
 * Sorts the list of students that the user have inserted, by their grade.
 * @param start A pointer to the beginning of the array which contains
 *              the students.
 * @param end A pointer to the end of the array which contains the students.
 */
void bubble_sort (Student *start, Student *end)
{
  size_t students_num = end - start;
  for (size_t i = 0; i < students_num - 1; ++i)
    {
      int swapped = 0;
      for (size_t j = 0; j < students_num - 1 - i; ++j)
        {
          if ((start + j)->grade > (start + j + 1)->grade)
            {
              swap (start + j, start + j + 1);
              swapped = 1;
            }
        }
      if (!swapped)
        return;
    }
}

/**
 * Prints the id, grade and age of the best student.
 * @param start Pointer to the beginning of the array which holds the students.
 * @param end Pointer to the end of the array which holds the students.
 */
void best_student (Student *start, Student *end)
{
  Student *best_student;
  float ratio = -1;
  size_t students_num = end - start;
  for (size_t i = 0; i < students_num; ++i)
    {
      float grade = (float) (start + i)->grade;
      float age = (float) (start + i)->age;
      float s_ratio = grade / age;
      if (s_ratio > ratio)
        {
          best_student = start + i;
          ratio = s_ratio;
        }
    }
  printf ("%s%ld,%d,%d\n", "best student info is: ",
          best_student->id, best_student->grade, best_student->age);
}

/**
 * Checks whether or not the information given by the user is valid.
 * If not, it prints an informative message to the user.
 * @param field_1 Represents the ID value given by the user.
 * @param field_2 Represents the grade value given by the user.
 * @param field_3 Represents the age value given by the user.
 * @param ps A pointer to a structure of name "Student".
 * @return 1 if the given information is valid, 0 otherwise.
 */
int check_student_info (char *field_1, char *field_2, char *field_3,
                        Student *ps)
{
  if (field_1[0] == BAD_DIGIT_FIRST || strlen (field_1) != ID_LENGTH)
    {
      printf ("%s%s", ERROR_FORMAT, ID_ERROR);
      return false;
    }
  char *id_leftovers = NULL, *grade_leftovers = NULL, *age_leftovers = NULL;
  errno = 0;
  long int id = strtol (field_1, &id_leftovers, BASE);
  long int grade = strtol (field_2, &grade_leftovers, BASE);
  long int age = strtol (field_3, &age_leftovers, BASE);
  if ((!id || !grade || !age) && errno)
    {
      printf ("%s%s", ERROR_FORMAT,
              "One or more fields did not meet the conditions.\n");
      return false;
    }
  if (id_leftovers[0] || id < 0)
    {
      printf ("%s%s", ERROR_FORMAT, ID_ERROR);
      return false;
    }
  if (grade < MIN_GRADE || grade > MAX_GRADE || grade_leftovers[0])
    {
      printf ("%s%s", ERROR_FORMAT, GRADE_ERROR);
      return false;
    }
  if (age < MIN_AGE || age > MAX_AGE || age_leftovers[0])
    {
      printf ("%s%s", ERROR_FORMAT, AGE_ERROR);
      return false;
    }
  ps->id = id;
  ps->grade = (int) grade;
  ps->age = (int) age;
  return true;
}

/**
 * Assigns the different values in input to their corresponding fields.
 * @param field_1 Represents the ID value given by the user.
 * @param field_2 Represents the grade value given by the user.
 * @param field_3 Represents the age value given by the user.
 * @param input A string of ID, grade and age separated by a comma.
 * @return 0 if input is invalid, 1 otherwise.
 */
int parse_fields (char *field_1, char *field_2, char *field_3, char *input)
{
  char *token = strtok (input, FIELDS_SEPARATOR);
  if (token == NULL)
    {
      printf ("%s%s", ERROR_FORMAT, ID_ERROR);
      return false;
    }
  strcpy (field_1, token);

  token = strtok (NULL, FIELDS_SEPARATOR);
  if (token == NULL)
    {
      printf ("%s%s", ERROR_FORMAT, GRADE_ERROR);
      return false;
    }
  strcpy (field_2, token);

  token = strtok (NULL, FIELDS_SEPARATOR);
  if (token == NULL)
    {
      printf ("%s%s", ERROR_FORMAT, AGE_ERROR);
      return false;
    }
  strcpy (field_3, token);
  return true;
}

/**
 * Gets the valid information about each student, and assigns it to
 * structures of name 'Student'. All the student objects created
 * will be stored at 'students'.
 * @param students A pointer to structures of name "Student".
 * @param students_num The total number of students given by the user.
 * @return 0 if fgets fails, 1 otherwise.
 */
int get_students_info (Student *students, long int students_num)
{
  long int i = 0;
  char field_1[MAX_FIELD_LEN], field_2[MAX_FIELD_LEN], field_3[MAX_FIELD_LEN];
  char input[MAX_LINE_LEN];
  while (i < students_num)
    {
      printf ("%s", "Enter student info. Then enter\n");
      if (fgets (input, MAX_LINE_LEN, stdin) == NULL)
        return false;
      if (!parse_fields (field_1, field_2, field_3, input))
        continue;
      Student s;
      Student *ps = &s;
      if (!check_student_info (field_1, field_2, field_3, ps))
        {
          ps = NULL;
          continue;
        }
      students[i] = s;
      i++;
    }
  return true;
}

/**
 * Returns the number of students given by the user or -1 if fgets fails.
 */
long int get_number_of_students ()
{
  long int students_num = 0;
  char input[MAX_LINE_LEN];
  while (students_num < 1)
    {
      printf ("%s", "Enter number of students. Then enter\n");
      if (fgets (input, MAX_LINE_LEN, stdin) == NULL)
        return -1;
      char *end_ptr;
      students_num = strtol (input, &end_ptr, BASE);
      if (students_num < 1 || strcmp (end_ptr, "\n"))
        printf ("%s%s", ERROR_FORMAT,
                "Number of students must be a positive integer.\n");
    }
  return students_num;
}

/**
 * Makes sure that the program is been executed with a valid argument.
 * @param n Number of given arguments.
 * @param args Given arguments.
 * @return 1 if the given argument is valid, 0 otherwise.
 */
int check_argument (int n, char **args)
{
  if (n != 2)
    {
      printf ("%s%s", USAGE_FORMAT, "Only one argument is allowed.\n");
      return false;
    }
  if (strcmp (args[1], OPERATION_1) && strcmp (args[1], OPERATION_2) &&
      strcmp (args[1], OPERATION_3))
    {
      printf ("%s%s", USAGE_FORMAT, "The argument should be: 'best', "
                                    "'bubble' or 'quick'.\n");
      return false;
    }
  return true;
}

/**
 * The main function to run the program.
 * @param argc The program's arguments.
 * @param argv The number of given arguments.
 * @return 0 if program succeeds, 1 otherwise.
 */
int main (int argc, char *argv[])
{
  if (!check_argument (argc, argv))
    return EXIT_FAILURE;

  long int students_num = get_number_of_students ();
  if (students_num == -1)
    return EXIT_FAILURE;
  Student *students = malloc (sizeof (*students) * students_num);
  if (students == NULL)
    return EXIT_FAILURE;
  if (!get_students_info (students, students_num))
    {
      free (students);
      return EXIT_FAILURE;
    }
  Student *end = students + students_num;

  if (!strcmp (argv[1], OPERATION_1))
    best_student (students, end);
  else
    {
      if (!strcmp (argv[1], OPERATION_2))
        bubble_sort (students, end);
      else if (!strcmp (argv[1], OPERATION_3))
        quick_sort (students, end);
      print_students_info (students, students_num);
    }

  free (students);
  return EXIT_SUCCESS;
}
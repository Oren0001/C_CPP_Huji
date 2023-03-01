#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND_1 "encode"
#define COMMAND_2 "decode"
#define COMMAND_3 "check"
#define MIN_ARGS_SIZE 3
#define MAX_ARGS_SIZE 4
#define BUFFER_SIZE 40
#define ALPHABET_SIZE 26
#define INVALID_COMMAND "The given command is invalid\n"
#define INVALID_ARGS_SIZE_1 "Usage: cipher <encode|decode> <k> " \
                            "<source path file> <output path file>\n"
#define INVALID_ARGS_SIZE_2 "Usage: cipher <check> <source path file> " \
                            "<output path file>\n"
#define INVALID_FILE "The given file is invalid\n"
#define INVALID_ENCRYPTION "Invalid encrypting\n"
#define VALID_ENCRYPTION "Valid encrypting with k = %d\n"
#define FIRST_LOWER_ALPHABET 'a'
#define LAST_LOWER_ALPHABET 'z'
#define FIRST_UPPER_ALPHABET 'A'
#define LAST_UPPER_ALPHABET 'Z'

/**
 * Finds the correct encryption for the given character,
 * by using the given shift value.
 * @param k The shift value.
 * @param c A character.
 * @return The encoded character.
 */
int shift (int k, char c)
{
  k %= ALPHABET_SIZE;
  int t = c + k; // the character to encode with.
  char left, right;
  if (c >= FIRST_LOWER_ALPHABET && c <= LAST_LOWER_ALPHABET)
    {
      left = FIRST_LOWER_ALPHABET;
      right = LAST_LOWER_ALPHABET;
      if (t >= left && t <= right)
        return t;
    }
  else
    {
      left = FIRST_UPPER_ALPHABET;
      right = LAST_UPPER_ALPHABET;
      if (t >= left && t <= right)
        return t;
    }
  if (t > right)
    {
      k = k - (right - c + 1);
      t = left + k;
    }
  else if (t < left)
    {
      k = k + (c - left + 1);
      t = right + k;
    }
  return t;
}

/**
 * Checks if the given character is in the english alphabet.
 * @param c A character.
 * @return 0 if c is in the english alphabet, 1 otherwise.
 */
int is_alpha (char c)
{
  if ((c >= FIRST_LOWER_ALPHABET && c <= LAST_LOWER_ALPHABET) ||
      (c >= FIRST_UPPER_ALPHABET && c <= LAST_UPPER_ALPHABET))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/**
 * Goes over the input file to find characters that need to be shifted.
 * @param k The shift value.
 * @param input The input file.
 */
void parse_input (int k, char *input)
{
  for (int i = 0; i < BUFFER_SIZE; i++)
    {
      if (is_alpha (input[i])) // keep the character as is
        continue;
      else
        input[i] = shift (k, input[i]);
    }
}

/**
 * According to the user's choice, it encodes or decodes the file at the input
 * address, and the result will be shown in the file at the output address.
 * @param k The shift value which is used to encode with.
 * @param in_address The input file address.
 * @param out_address The output file address.
 * @return 1 if a file could not be opened, and 0 if the input file was
 *         encoded successfully.
 */
int encode_or_decode (int k, char *in_address, char *out_address)
{
  FILE *in_file = fopen (in_address, "r");
  FILE *out_file = fopen (out_address, "w");
  if (in_file == NULL || out_file == NULL)
    {
      fprintf (stderr, INVALID_FILE);
      return EXIT_FAILURE;
    }
  char input[BUFFER_SIZE];
  while (fgets (input, BUFFER_SIZE, in_file) != NULL)
    {
      parse_input (k, input);
      fputs (input, out_file);
    }
  fclose (in_file);
  fclose (out_file);
  return EXIT_SUCCESS;
}

/**
 * Finds the correct shift value, if it exists.
 * @param shift The shift value.
 * @param c_1 A character in the source file.
 * @param c_2 c1's equivalent in the encoded file.
 */
void get_shift_value (int *shift, char c_1, char c_2)
{
  if ((c_1 >= FIRST_LOWER_ALPHABET && c_1 <= LAST_LOWER_ALPHABET &&
       c_2 >= FIRST_LOWER_ALPHABET && c_2 <= LAST_LOWER_ALPHABET) ||
      (c_1 >= FIRST_UPPER_ALPHABET && c_1 <= LAST_UPPER_ALPHABET &&
       c_2 >= FIRST_UPPER_ALPHABET && c_2 <= LAST_UPPER_ALPHABET))
    {
      int left, right;
      if (c_1 >= FIRST_LOWER_ALPHABET)
        left = FIRST_LOWER_ALPHABET, right = LAST_LOWER_ALPHABET;
      else
        left = FIRST_UPPER_ALPHABET, right = LAST_UPPER_ALPHABET;
      *shift = (c_1 <= c_2) ? c_2 - c_1 : (right - c_1) + (c_2 - left + 1);
    }
}

/**
 * Goes over the source file and the encoded file,
 * to check if the latter is encoded properly.
 * @param shift The shift value.
 * @param source The source file.
 * @param encoded The encoded file.
 * @return 1 if the encryption is invalid, 0 otherwise.
 */
int parse_inputs (int *shift, char *source, char *encoded)
{
  for (int i = 0; i < BUFFER_SIZE; i++)
    {
      if (is_alpha (source[i]) && is_alpha (encoded[i]))
        {
          if (source[i] != encoded[i])
            return EXIT_FAILURE;
        }
      else if ((!is_alpha (source[i]) && is_alpha (encoded[i])) ||
               (is_alpha (source[i]) && !is_alpha (encoded[i])))
        return EXIT_FAILURE;
      else if (!is_alpha (source[i]) && !is_alpha (encoded[i]))
        {
          int temp = *shift;
          get_shift_value (shift, source[i], encoded[i]);
          if (*shift == -1 || (*shift != temp && temp != -1))
            return EXIT_FAILURE;
        }
    }
  return EXIT_SUCCESS;
}

/**
 * Checks if there is a shift value which is used to encode the source file,
 * and the result is the encoded file. if the value exists, it will be printed.
 * @param source_address The source file address.
 * @param encoded_address The encoded file address.
 * @return 1 if a file could not be opened, 0 otherwise.
 */
int check (char *source_address, char *encoded_address)
{
  FILE *source = fopen (source_address, "r");
  FILE *encoded = fopen (encoded_address, "r");
  if (source == NULL || encoded == NULL)
    {
      fprintf (stderr, INVALID_FILE);
      return EXIT_FAILURE;
    }
  char source_input[BUFFER_SIZE] = {0}, encoded_input[BUFFER_SIZE] = {0};
  int shift_value = -1;
  while ((fgets (source_input, BUFFER_SIZE, source) != NULL &&
          fgets (encoded_input, BUFFER_SIZE, encoded) != NULL))
    {
      int temp = shift_value;
      int res = parse_inputs (&shift_value, source_input, encoded_input);
      if ((temp != -1 && shift_value != temp) || res == EXIT_FAILURE)
        {
          fclose (source);
          fclose (encoded);
          printf (INVALID_ENCRYPTION);
          return EXIT_SUCCESS;
        }
    }
  if ((source_input[0] && !encoded_input[0]) ||
      (!source_input[0] && encoded_input[0]))
    {
      fclose (source);
      fclose (encoded);
      printf (INVALID_ENCRYPTION);
      return EXIT_SUCCESS;
    }
  printf (VALID_ENCRYPTION, (shift_value == -1) ? 0 : shift_value);
  fclose (source);
  fclose (encoded);
  return EXIT_SUCCESS;
}

/**
 * The main function to run the program.
 * @param argc The program's arguments.
 * @param argv The number of given arguments.
 * @return 0 if program succeeds, 1 otherwise.
 */
int main (int argc, char *argv[])
{
  if (argc < 2)
    {
      fprintf (stderr, INVALID_COMMAND);
      return EXIT_FAILURE;
    }
  if (!strcmp (argv[1], COMMAND_1) || !strcmp (argv[1], COMMAND_2))
    {
      if (argc - 1 != MAX_ARGS_SIZE)
        {
          fprintf (stderr, INVALID_ARGS_SIZE_1);
          return EXIT_FAILURE;
        }
      int shift = atoi (argv[2]);
      if (!strcmp (argv[1], COMMAND_1))
        return encode_or_decode (shift, argv[3], argv[4]);
      else
        return encode_or_decode (-shift, argv[3], argv[4]);
    }
  else if (!strcmp (argv[1], COMMAND_3))
    {
      if (argc - 1 != MIN_ARGS_SIZE)
        {
          fprintf (stderr, INVALID_ARGS_SIZE_2);
          return EXIT_FAILURE;
        }
      return check (argv[2], argv[3]);
    }
  else
    {
      fprintf (stderr, INVALID_COMMAND);
      return EXIT_FAILURE;
    }
}

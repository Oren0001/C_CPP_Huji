#ifndef _TEST_PAIRS_H_
#define _TEST_PAIRS_H_

#include <stdlib.h>

#define FIRST_CAPITAL_LETTER 65
#define LAST_CAPITAL_LETTER 90
#define FIRST_LOWER_CASE_LETTER 97
#define LAST_LOWER_CASE_LETTER 122

/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a capital letter, else - 0.
 */
int is_capital_letter (const_keyT elem)
{
  char c = *((char *) elem);
  return (c >= FIRST_CAPITAL_LETTER && c <= LAST_CAPITAL_LETTER);
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}

/**
 * @param elem A pointer to a char.
 * @return 1 if elem represents a letter, 0 otherwise.
 */
int is_letter (const_keyT elem)
{
  char c = *((char *) elem);
  return ((c >= FIRST_CAPITAL_LETTER && c <= LAST_CAPITAL_LETTER) ||
          (c >= FIRST_LOWER_CASE_LETTER && c <= LAST_LOWER_CASE_LETTER));
}

/**
 * Identity function.
 * @param elem A pointer to an integer.
 */
void keep_value (valueT elem)
{
  *((int *) elem) *= 1;
}

#endif //_TEST_PAIRS_H_

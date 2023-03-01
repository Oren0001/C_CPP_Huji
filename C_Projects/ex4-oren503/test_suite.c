#include "test_suite.h"
#include "test_pairs.h"
#include "hash_funcs.h"
#include <stdio.h>
#include <stdbool.h>

#define PAIRS_COUNT 26
#define INVALID_INPUT -1

/**
 * Frees all the pairs that were dynamically allocated.
 * @param pairs An array of pointers to pairs to deallocate.
 * @param pairs_size The says of 'pairs'.
 */
void free_pairs (pair *pairs[], int pairs_size)
{
  for (int i = 0; i < pairs_size; ++i)
    {
      pair_free ((void **) (pairs + i));
    }
}

/**
 * Will initialize 'pairs' - an array of pointers to pairs. each pair
 * consists of keys which are characters from 'l' to 'r', and their
 * corresponding values from 'l' to 'r'.
 * In addition, it will initialize a new hashmap if it's NULL.
 * @param hash_map A pointer to hashmap to initialize if NULL.
 * @param pairs A pointer to pair to initialize.
 * @param l The left bound of the keys and values.
 * @param r The right bound of the keys and values.
 */
void initializer (hashmap *hash_map, pair *pairs[], int l, int r)
{
  int j = 0;
  for (int i = l; i <= r; ++i)
    {
      char key = (char) i;
      int value = i;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      j++;
    }
  if (hash_map != NULL)
    {
      for (int i = 0; i < PAIRS_COUNT; ++i)
        hashmap_insert (hash_map, pairs[i]);
    }
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  char c_0 = 'T';
  assert(hashmap_at (hash_map, &c_0) == NULL);

  pair *pairs[PAIRS_COUNT];
  initializer (hash_map, pairs, FIRST_CAPITAL_LETTER, LAST_CAPITAL_LETTER);
  char c_1 = '&';
  assert(hashmap_at (hash_map, NULL) == NULL);
  assert(hashmap_at (NULL, &c_1) == NULL);
  assert(hashmap_at (NULL, NULL) == NULL);
  assert(hashmap_at (hash_map, &c_1) == NULL);

  for (int i = FIRST_CAPITAL_LETTER; i <= LAST_CAPITAL_LETTER; ++i)
    {
      char c = (char) i;
      assert(*((int *) hashmap_at (hash_map, &c)) == i);
    }
  hashmap_free (&hash_map);
  free_pairs (pairs, PAIRS_COUNT);
}

/**
 * This function checks the hashmap_insert function of the hashmap library. If
 * hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert (void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  pair *pairs[PAIRS_COUNT];
  initializer (hash_map, pairs, FIRST_CAPITAL_LETTER, LAST_CAPITAL_LETTER);

  // Test cases where hashmap_insert should return 0.
  for (int i = 0; i < PAIRS_COUNT; ++i)
    assert(hashmap_insert (hash_map, pairs[i]) == false);
  assert(hashmap_insert (NULL, pairs[17]) == false);
  assert(hashmap_insert (hash_map, NULL) == false);
  assert(hashmap_insert (NULL, NULL) == false);
  hashmap_free (&hash_map);

  // Test cases where hashmap_insert should return 1.
  // Make sure that that the size and capacity are updated correctly.
  hash_map = hashmap_alloc (hash_char);
  size_t capacity = HASH_MAP_INITIAL_CAP;
  size_t threshold = capacity * HASH_MAP_MAX_LOAD_FACTOR;
  for (size_t i = 0; i < PAIRS_COUNT; ++i)
    {
      assert(hashmap_insert (hash_map, pairs[i]) == true);
      assert(*((int *) hashmap_at (hash_map, pairs[i]->key)) ==
             FIRST_CAPITAL_LETTER + (int) i);
      assert((hash_map->size) == i + 1);
      if (i == (threshold - 1))
        {
          assert(hash_map->capacity == capacity);
        }
      else if (i == threshold)
        {
          capacity *= HASH_MAP_GROWTH_FACTOR;
          threshold = capacity * HASH_MAP_MAX_LOAD_FACTOR;
          assert((hash_map->capacity) == capacity);
        }
    }

  hashmap_free (&hash_map);
  free_pairs (pairs, PAIRS_COUNT);
}

/**
 * This function checks the hashmap_erase function of the hashmap library. If
 * hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{
  // Test cases where hashmap_erase should return 0.
  hashmap *hash_map = hashmap_alloc (hash_char);
  char c_0 = '!';
  assert(hashmap_erase (hash_map, &c_0) == false);
  pair *pairs[PAIRS_COUNT];
  initializer (hash_map, pairs, FIRST_CAPITAL_LETTER, LAST_CAPITAL_LETTER);
  char c_1 = 9;
  assert(hashmap_erase (hash_map, &c_1) == false);
  assert(hashmap_erase (NULL, pairs[12]->key) == false);
  assert(hashmap_erase (hash_map, NULL) == false);
  assert(hashmap_erase (NULL, NULL) == false);

  // Test cases where hashmap_erase should return 1.
  // Make sure that that the size and capacity are updated correctly.
  size_t capacity = HASH_MAP_INITIAL_CAP * 4;
  size_t threshold = capacity * HASH_MAP_MIN_LOAD_FACTOR;
  for (size_t i = (PAIRS_COUNT - 1); (long long) i >= 0; --i)
    {
      assert(hashmap_erase (hash_map, pairs[i]->key) == true);
      assert(hashmap_at (hash_map, pairs[i]->key) == NULL);
      assert(hash_map->size == i);
      if (i == threshold)
        {
          assert(hash_map->capacity == capacity);
        }
      else if (i == (threshold - 1))
        {
          capacity /= HASH_MAP_GROWTH_FACTOR;
          threshold = capacity * HASH_MAP_MIN_LOAD_FACTOR;
          assert((hash_map->capacity) == capacity);
        }
    }
  hashmap_free (&hash_map);
  free_pairs (pairs, PAIRS_COUNT);
}

/**
 * Prints assertion error if the load factor hasn't been updated properly
 * after a deletion of a pair from the given hash map.
 * @param hash_map A hash map to delete from.
 * @param pairs An array of pointers to pairs to delete from the hash map.
 */
void check_load_factor_after_deletion (hashmap *hash_map, pair *pairs[])
{
  int cap = HASH_MAP_INITIAL_CAP * 4;
  for (int i = (PAIRS_COUNT - 1); i >= 0; --i)
    {
      hashmap_erase (hash_map, pairs[i]->key);
      if (i >= 16)
        {
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
      else if ((i <= 15) && (i >= 8))
        {
          if (i == 15)
            cap /= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
      else if ((i <= 7) && (i >= 4))
        {
          if (i == 7)
            cap /= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
      else if ((i <= 3) && (i >= 2))
        {
          if (i == 3)
            cap /= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
      else if (i == 1)
        {
          cap /= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
      else if (i == 0)
        {
          cap /= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) == ((double) i / cap));
        }
    }
}

/**
 * Prints assertion error if the load factor hasn't been updated properly
 * after an insertion of a pair to the given hash map.
 * @param hash_map A hash map to insert to.
 * @param pairs An array of pointers to pairs to insert to the hash map.
 */
void check_load_factor_after_insertion (hashmap *hash_map, pair *pairs[])
{
  int cap = HASH_MAP_INITIAL_CAP;
  for (int i = 0; i < PAIRS_COUNT; ++i)
    {
      hashmap_insert (hash_map, pairs[i]);
      if ((i >= 0) && (i < 12))
        {
          assert(hashmap_get_load_factor (hash_map) ==
                 ((double) (i + 1) / cap));
        }
      else if ((i >= 12) && (i < 24))
        {
          if (i == 12)
            cap *= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) ==
                 ((double) (i + 1) / cap));
        }
      else if (i >= 24)
        {
          if (i == 24)
            cap *= HASH_MAP_GROWTH_FACTOR;
          assert(hashmap_get_load_factor (hash_map) ==
                 ((double) (i + 1) / cap));
        }
    }
}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap
 * library. If hashmap_get_load_factor fails at some points,
 * the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  assert(hashmap_get_load_factor (NULL) == -1);

  hashmap *hash_map = hashmap_alloc (hash_char);
  pair *pairs[PAIRS_COUNT];
  initializer (NULL, pairs, FIRST_CAPITAL_LETTER,
               LAST_CAPITAL_LETTER);

  check_load_factor_after_insertion (hash_map, pairs);
  check_load_factor_after_deletion (hash_map, pairs);

  hashmap_free (&hash_map);
  free_pairs (pairs, PAIRS_COUNT);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points,
 * the functions exits with exit code 1.
 */
void test_hash_map_apply_if ()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hashmap_apply_if (hash_map, is_capital_letter, double_value) == 0);
  assert(hashmap_apply_if (hash_map, is_capital_letter, NULL)
         == INVALID_INPUT);
  assert(hashmap_apply_if (hash_map, NULL, double_value) == INVALID_INPUT);
  assert(hashmap_apply_if (hash_map, NULL, NULL) == INVALID_INPUT);
  assert(hashmap_apply_if (NULL, is_capital_letter, double_value)
         == INVALID_INPUT);
  assert(hashmap_apply_if (NULL, is_capital_letter, NULL) == INVALID_INPUT);
  assert(hashmap_apply_if (NULL, NULL, double_value) == INVALID_INPUT);
  assert(hashmap_apply_if (NULL, NULL, NULL) == INVALID_INPUT);

  pair *pairs_1[PAIRS_COUNT];
  initializer (hash_map, pairs_1, FIRST_CAPITAL_LETTER, LAST_CAPITAL_LETTER);
  assert(hashmap_apply_if (hash_map, is_capital_letter, double_value)
         == PAIRS_COUNT);

  pair *pairs_2[PAIRS_COUNT];
  initializer (hash_map, pairs_2, FIRST_LOWER_CASE_LETTER,
               LAST_LOWER_CASE_LETTER);
  assert(hashmap_apply_if (hash_map, is_capital_letter, double_value)
         == PAIRS_COUNT);

  assert(hashmap_apply_if (hash_map, is_letter, keep_value)
         == (PAIRS_COUNT * 2));
  hashmap_free (&hash_map);
  free_pairs (pairs_1, PAIRS_COUNT);
  free_pairs (pairs_2, PAIRS_COUNT);
}

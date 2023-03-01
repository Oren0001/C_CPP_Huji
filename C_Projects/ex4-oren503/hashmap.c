#include "hashmap.h"
#include <stdlib.h>
#include <stdbool.h>

#define FAILED_TO_GET_LOAD_FACTOR -1
#define ELEMENT_NOT_FOUND -1
#define INVALID_INPUT -1

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    return NULL;
  hashmap *hash_map = malloc (sizeof (*hash_map));
  if (hash_map == NULL)
    return NULL;
  *hash_map = (hashmap) {NULL, 0, HASH_MAP_INITIAL_CAP, func};
  hash_map->buckets = calloc (hash_map->capacity, sizeof (vector *));
  if (hash_map->buckets == NULL)
    {
      free (hash_map);
      return NULL;
    }
  return hash_map;
}

/**
 * Frees the given buckets.
 * @param buckets Buckets to free.
 * @param buckets_num Number of buckets.
 */
void free_buckets (vector **buckets, size_t buckets_num)
{
  for (size_t i = 0; i < buckets_num; i++)
    {
      vector_free (buckets + i);
    }
  free (buckets);
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if ((p_hash_map == NULL) || (*p_hash_map == NULL))
    return;
  free_buckets ((*p_hash_map)->buckets, (*p_hash_map)->capacity);
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * Searches for the given key in the bucket positioned in bucket_idx.
 * @param hash_map A hash map which contains buckets (pointers to vectors).
 * @param key A key to search for.
 * @param bucket_idx The index of the bucket that we want to search in.
 * @return The index of the key if found, -1 otherwise.
 */
long long get_element_index (const hashmap *hash_map, const_keyT key,
                             size_t bucket_idx)
{
  pair *in_pair = NULL;
  for (size_t i = 0; i < (hash_map->buckets)[bucket_idx]->size; ++i)
    {
      in_pair = (pair *) vector_at ((hash_map->buckets)[bucket_idx], i);
      if (in_pair->key_cmp (in_pair->key, key))
        return (long long) i;
    }
  return ELEMENT_NOT_FOUND;
}

/**
 * Uses a hash function to find the appropriate index that the given key
 * will be stored in.
 * @param hash_map A hash map which contains buckets (pointers to vectors).
 * @param key A key to hash.
 * @param capacity The capacity of the hash map.
 * @return The bucket index that the key can be stored in.
 */
size_t get_bucket_index (const hashmap *hash_map, const_keyT key,
                         size_t capacity)
{
  size_t hashed_value = hash_map->hash_func (key);
  size_t bucket_idx = hashed_value & (capacity);
  return bucket_idx;
}

/**
 * Copys the bucket at index 'idx' which is stored in the 'buckets' of
 * hash_map, to 'new buckets' at the new appropriate index.
 * For example: We will use this function to copy the bucket at index 3
 *              which is stored in the buckets of hash_map, to 'new_buckets'
 *              at the appropriate new index.
 * @param hash_map A hash map to copy from.
 * @param new_buckets A pointer to vector pointers.
 * @param new_capacity Capacity of 'new_buckets'.
 * @param idx The index that stores the bucket we want to copy.
 * @return 1 if the copy has been done successfully, 0 otherwise.
 */
short copy_bucket (hashmap *hash_map, vector **new_buckets,
                   size_t new_capacity, size_t idx)
{
  const pair *in_pair;
  size_t bucket_idx;
  for (size_t j = 0; j < (hash_map->buckets)[idx]->size; ++j)
    {
      in_pair = vector_at ((hash_map->buckets)[idx], j);
      bucket_idx = get_bucket_index (hash_map, in_pair->key,
                                     new_capacity - 1);
      if (new_buckets[bucket_idx] == NULL)
        {
          if ((new_buckets[bucket_idx] = vector_alloc (
              pair_copy, pair_cmp, pair_free)) == NULL)
            {
              free_buckets (new_buckets, new_capacity);
              return false;
            }
        }
      if (!vector_push_back (new_buckets[bucket_idx], in_pair))
        {
          free_buckets (new_buckets, new_capacity);
          return false;
        }
    }
  return true;
}

/**
 * Updates the capacity of hash_map using 'factor'. It also means that
 * the elements stored in the buckets of hash_map will be rehashed.
 * @param hash_map A pointer to hash map.
 * @param factor A float number that is used to update the capacity.
 * @return 1 if the update has been done successfully, 0 otherwise.
 */
short update_hashmap_capacity (hashmap *hash_map, float factor)
{
  size_t new_capacity = (hash_map->capacity) * factor;
  vector **new_buckets = calloc (new_capacity, sizeof (vector *));
  if (new_buckets == NULL)
    return false;
  for (size_t i = 0; i < hash_map->capacity; ++i)
    {
      if ((hash_map->buckets)[i] != NULL)
        {
          if (!copy_bucket (hash_map, new_buckets, new_capacity, i))
            return false;
        }
    }
  free_buckets (hash_map->buckets, hash_map->capacity);
  hash_map->buckets = new_buckets;
  hash_map->capacity = new_capacity;
  return true;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if ((hash_map == NULL) || (hash_map->capacity == 0))
    return FAILED_TO_GET_LOAD_FACTOR;
  return (double) (hash_map->size) / (hash_map->capacity);
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise
 *         (the value itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if ((hash_map == NULL) || (key == NULL))
    return NULL;
  size_t bucket_idx = get_bucket_index (hash_map, key,
                                        (hash_map->capacity) - 1);
  if ((hash_map->buckets)[bucket_idx] == NULL)
    return NULL;

  long long element_idx = get_element_index (hash_map, key, bucket_idx);
  return (element_idx == ELEMENT_NOT_FOUND) ? NULL : ((pair *)
      (vector_at ((hash_map->buckets)[bucket_idx], element_idx)))->value;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if ((hash_map == NULL) || (in_pair == NULL))
    return false;
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    return false;

  size_t bucket_idx = get_bucket_index (hash_map, in_pair->key,
                                        (hash_map->capacity) - 1);
  vector **bucket = (hash_map->buckets) + bucket_idx;
  if (*bucket == NULL)
    {
      if ((*bucket = vector_alloc (pair_copy, pair_cmp, pair_free)) == NULL)
        return false;
    }
  if (!vector_push_back (*bucket, in_pair))
    return false;

  hash_map->size++;
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      if (!update_hashmap_capacity (hash_map, HASH_MAP_GROWTH_FACTOR))
        {
          hash_map->size--;
          vector_erase (*bucket, ((*bucket)->size) - 1);
          return false;
        }
    }
  return true;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 *           (if key not in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if ((hash_map == NULL) || (key == NULL))
    return false;
  size_t bucket_idx = get_bucket_index (hash_map, key,
                                        (hash_map->capacity) - 1);
  if ((hash_map->buckets)[bucket_idx] == NULL)
    return false;
  long long element_idx = get_element_index (hash_map, key, bucket_idx);
  if (element_idx == ELEMENT_NOT_FOUND)
    return false;

  pair *in_pair = (pair *) vector_at ((hash_map->buckets)[bucket_idx],
                                      element_idx);
  pair *pair_cpy = (hash_map->buckets)[bucket_idx]->elem_copy_func (in_pair);
  if (vector_erase ((hash_map->buckets)[bucket_idx], element_idx))
    {
      hash_map->size--;
      if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
        {
          if (!update_hashmap_capacity (hash_map, (float) 1 /
                                                  HASH_MAP_GROWTH_FACTOR))
            {
              hash_map->size++;
              vector_push_back ((hash_map->buckets)[bucket_idx], pair_cpy);
              return false;
            }
        }
      pair_free ((void **) &pair_cpy);
      return true;
    }
  pair_free ((void **) &pair_cpy);
  return false;
}

/**
 * This function receives a hashmap and 2 functions, the first checks
 * a condition on the keys, and the seconds apply some modification on
 * the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is
 * a capital letter (A-Z), and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map: {('C',2),('#',3),('X',5)},
 * to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and
 *                  return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func)
{
  if ((hash_map == NULL) || (keyT_func == NULL) || (valT_func == NULL))
    return INVALID_INPUT;
  int changed_values = 0;
  for (size_t i = 0; i < hash_map->capacity; ++i)
    {
      if ((hash_map->buckets)[i] != NULL)
        {
          for (size_t j = 0; j < (hash_map->buckets)[i]->size; ++j)
            {
              pair *in_pair = (pair *) vector_at ((hash_map->buckets)[i], j);
              if (keyT_func (in_pair->key))
                {
                  valT_func (in_pair->value);
                  changed_values++;
                }
            }
        }
    }
  return changed_values;
}

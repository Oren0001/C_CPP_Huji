#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

#define VALUE_NOT_FOUND -1
#define FAILED_TO_GET_LOAD_FACTOR -1

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector
 *                       (returns dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in
 *                      the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc (vector_elem_cpy elem_copy_func,
                      vector_elem_cmp elem_cmp_func,
                      vector_elem_free elem_free_func)
{
  if ((elem_copy_func == NULL) || (elem_cmp_func == NULL) ||
      (elem_free_func == NULL))
    return NULL;
  vector *v = malloc (sizeof (*v));
  if (v == NULL)
    return NULL;
  *v = (vector) {VECTOR_INITIAL_CAP, 0, NULL, elem_copy_func,
                 elem_cmp_func, elem_free_func};
  v->data = malloc (sizeof (void *) * v->capacity);
  if (v->data == NULL)
    {
      free (v);
      return NULL;
    }
  return v;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector)
{
  if ((p_vector == NULL) || (*p_vector) == NULL)
    return;
  for (size_t i = 0; i < (*p_vector)->size; i++)
    (*p_vector)->elem_free_func (((*p_vector)->data) + i);
  free ((*p_vector)->data);
  free (*p_vector);
  *p_vector = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself,
 *         not a copy of it), NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind)
{
  if ((vector == NULL) || (vector->size <= ind))
    return NULL;
  return (vector->data)[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 *         ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value)
{
  if ((vector == NULL) || (value == NULL))
    return VALUE_NOT_FOUND;
  for (size_t i = 0; i < vector->size; ++i)
    {
      if (vector->elem_cmp_func ((vector->data)[i], value))
        return (int) i;
    }
  return VALUE_NOT_FOUND;
}

/**
 * Updates a vector's capacity according to the giving factor.
 * @param v A pointer to a vector.
 * @param factor Will be multiplied by the vector's capacity.
 * @return 1 if the capacity has been updated successfully, 0 otherwise.
 */
short update_vector_capacity (vector *v, float factor)
{
  v->capacity *= factor;
  void **temp = realloc (v->data, sizeof (*(v->data)) * (v->capacity));
  if (temp == NULL)
    {
      vector_free (&v);
      return false;
    }
  v->data = temp;
  return true;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector)
{
  if ((vector == NULL) || (vector->capacity == 0))
    return FAILED_TO_GET_LOAD_FACTOR;
  return (double) (vector->size) / (vector->capacity);
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value)
{
  if ((vector == NULL) || (value == NULL))
    return false;
  vector->size++;
  if (vector_get_load_factor (vector) > VECTOR_MAX_LOAD_FACTOR)
    {
      if (!update_vector_capacity (vector, VECTOR_GROWTH_FACTOR))
        {
          vector->size--;
          return false;
        }
    }
  (vector->data)[(vector->size) - 1] = vector->elem_copy_func (value);
  return true;
}

/**
 * Removes the element at the given index from the vector. alters the indices
 * of the remaining elements so that there are no empty indices in the
 * range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind)
{
  if ((vector == NULL) || (vector->size <= ind))
    return false;
  vector->size--;
  if (vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR)
    {
      if (!update_vector_capacity (vector, (float) 1 /
                                           VECTOR_GROWTH_FACTOR))
        {
          vector->size++;
          return false;
        }
    }
  vector->elem_free_func ((vector->data) + ind);
  for (size_t i = ind; i < (vector->size); ++i)
    (vector->data)[i] = (vector->data)[i + 1];
  (vector->data)[vector->size] = NULL;
  return true;
}

/**
 * Deletes all the elements in the vector.
 * @param vector a pointer to a vector.
 */
void vector_clear (vector *vector)
{
  if (vector == NULL)
    return;
  for (size_t i = ((vector->size) - 1); (long long) i >= 0; i--)
    vector_erase (vector, i);
}
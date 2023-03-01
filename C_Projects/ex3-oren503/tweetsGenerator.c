#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define READ_ENTIRE_FILE -1
#define BASE 10
#define USAGE_FORMAT "Usage: "
#define ERROR_FORMAT "Error: "
#define TWEET_FORMAT "Tweet %lu: "
#define ALLOCATION_ERROR "Allocation failure: Couldn't allocate memory."
#define ARGS_COUNT 4
#define DELIMITER " \n"
#define REALLOC_SIZE 1
#define LAST_WORD_SUFFIX '.'
#define BASE 10
#define SPACE " "
#define MIN_WORDS_IN_SENTENCE_GENERATION 2

/** Represents a word in the dictionary. */
typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    unsigned int occurrences;
    unsigned int prob_list_size;
    unsigned char has_dot_suffix;
} WordStruct;

/** Represents a consecutive word to a word in the dictionary. */
typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    unsigned int occurrences;
} WordProbability;

/************ LINKED LIST ************/

/** Represents a node in a linked list.  */
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

/** Represents a linked list. */
typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
    int words_without_dot_suffix;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}
/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number Number of words without dot suffix in the dictionary.
 * @return Random number A number in [0, max_number).
 */
int get_random_number (int max_number)
{
  return rand () % max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  int rand_num = get_random_number (dictionary->words_without_dot_suffix);
  Node *cur = dictionary->first;
  int i = 0;
  while (i <= rand_num)
    {
      if (!cur->data->has_dot_suffix)
        {
          if (i == rand_num)
            break;
          i++;
        }
      cur = cur->next;
    }
  return cur->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int rand_num = get_random_number ((int) word_struct_ptr->occurrences);
  int occurrences_sum = 0;
  for (unsigned int i = 0; i < word_struct_ptr->prob_list_size; ++i)
    {
      occurrences_sum += (int) word_struct_ptr->prob_list[i].occurrences;
      if (rand_num < occurrences_sum)
        return word_struct_ptr->prob_list[i].word_struct_ptr;
    }
  return NULL;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  char sentence[MAX_WORDS_IN_SENTENCE_GENERATION * MAX_WORD_LENGTH + 1] = {0};
  WordStruct *first_word_struct = get_first_random_word (dictionary);
  strcpy (sentence, first_word_struct->word);
  strcat (sentence, SPACE);
  WordStruct *next_word_struct = get_next_random_word (first_word_struct);
  strcat (sentence, next_word_struct->word);
  int words_count = MIN_WORDS_IN_SENTENCE_GENERATION;
  while ((words_count < MAX_WORDS_IN_SENTENCE_GENERATION) &&
         (!next_word_struct->has_dot_suffix))
    {
      strcat (sentence, SPACE);
      next_word_struct = get_next_random_word (next_word_struct);
      strcat (sentence, next_word_struct->word);
      words_count++;
    }
  printf ("%s\n", sentence);
  return words_count;
}

/**
 * Prints a message regarding allocation failure, and exits with code 1.
 */
void allocation_failure ()
{
  printf ("%s", ALLOCATION_ERROR);
  exit (EXIT_FAILURE);
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word)
{
  for (unsigned int i = 0; i < first_word->prob_list_size; ++i)
    {
      if (first_word->prob_list[i].word_struct_ptr == second_word)
        {
          first_word->prob_list[i].occurrences++;
          return false;
        }
    }

  size_t alloc_size = (first_word->prob_list_size + REALLOC_SIZE) *
                      sizeof (WordProbability);
  WordProbability *temp = realloc (first_word->prob_list, alloc_size);
  if (temp == NULL)
    allocation_failure ();
  first_word->prob_list = temp;

  first_word->prob_list[first_word->prob_list_size] = (WordProbability)
      {second_word, 1};
  first_word->prob_list_size += REALLOC_SIZE;
  return true;
}

/**
 * Adds the given word to the dictionary. If it already exists then it's
 * number of occurrences will be updated, otherwise a new word struct which
 * represents the word will be added to the dictionary.
 * @param word A word from the text corpus.
 * @param dictionary A linked list that contains all the words.
 * @return A word struct which represents the given word.
 */
WordStruct *add_word_to_dictionary (char *word, LinkList *dictionary)
{
  Node *cur = dictionary->first;
  while (cur != NULL)
    {
      if (!strcmp (cur->data->word, word))
        {
          cur->data->occurrences++;
          return cur->data;
        }
      cur = cur->next;
    }

  char *word_copy = malloc (MAX_WORD_LENGTH + 1);
  if (word_copy == NULL)
    allocation_failure ();
  strcpy (word_copy, word);

  WordStruct *word_struct = malloc (sizeof (*word_struct));
  if (word_struct == NULL)
    allocation_failure ();
  *word_struct = (WordStruct) {word_copy, NULL, 1,
                               0, 1};
  if (add (dictionary, word_struct) == 1)
    allocation_failure ();
  return word_struct;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char input[MAX_SENTENCE_LENGTH + 1] = {0};
  char *word = NULL, *consecutive = NULL;
  WordStruct *word_struct_1 = NULL, *word_struct_2 = NULL;
  int words_read = 0;
  while (fgets (input, MAX_SENTENCE_LENGTH + 1, fp) != NULL)
    {
      word = strtok (input, DELIMITER);
      if (word != NULL)
        {
          word_struct_1 = add_word_to_dictionary (word, dictionary);
          words_read++;
          if (words_read == words_to_read)
            return;
        }
      while (word != NULL)
        {
          consecutive = strtok (NULL, DELIMITER);
          if (consecutive != NULL)
            {
              word_struct_2 = add_word_to_dictionary (consecutive, dictionary);
              words_read++;
              if (word_struct_1->word[strlen (word) - 1] != LAST_WORD_SUFFIX)
                {
                  word_struct_1->has_dot_suffix = 0;
                  if (word_struct_1->occurrences == 1)
                    dictionary->words_without_dot_suffix++;
                  add_word_to_probability_list (word_struct_1, word_struct_2);
                }
              if (words_read == words_to_read)
                return;
              word_struct_1 = word_struct_2;
            }
          word = consecutive;
        }
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *pre = NULL;
  Node *cur = dictionary->first;
  while (cur != NULL)
    {
      free (cur->data->word);
      free (cur->data->prob_list);
      free (cur->data);
      pre = cur;
      cur = cur->next;
      free (pre);
    }
}

/**
 * Opens the file in the given path.
 * @param fp Double pointer to a file.
 * @param path A path to a file.
 * @return 0 if it failed to open the file, 1 otherwise.
 */
int open_file (FILE **fp, char *path)
{
  *fp = fopen (path, "r");
  if (*fp == NULL)
    {
      printf ("%s%s", ERROR_FORMAT, "File does not exist.");
      return false;
    }
  return true;
}

/**
 * Gets the number of words that needs to be read.
 * @param args_count Number of arguments given during the call to the program.
 * @param argv The arguments values.
 * @return -1 if the fourth argument does not exist.
 *          0 if argc is invalid.
 *          otherwise, return the number of words to read.
 */
int get_words_to_read (int args_count, char *argv[])
{
  if (args_count == ARGS_COUNT - 1)
    return READ_ENTIRE_FILE;
  else if (args_count == ARGS_COUNT)
    return (int) strtol (argv[ARGS_COUNT], NULL, BASE);
  else
    {
      printf ("%s%s", USAGE_FORMAT, "Arguments must be <Seed,"
                                    "#of sentences,Path[,#of words]>");
      return false;
    }
}

/**
 * The main function to run the program.
 * @param argc The number of given arguments.
 * @param argv 1) Seed - An integer.
 *             2) Number of sentences to generate - An integer bigger than 0.
 *             3) Path to file
 *             4) Optional - Number of words to read. An integer bigger than 0
 */
int main (int argc, char *argv[])
{
  int words_to_read = get_words_to_read (argc - 1, argv);
  FILE *fp = NULL;
  if (!words_to_read || !open_file (&fp, argv[3]))
    return EXIT_FAILURE;
  size_t seed = strtol (argv[1], NULL, BASE);
  srand (seed);
  LinkList dictionary = {NULL, NULL, 0, 0};

  fill_dictionary (fp, words_to_read, &dictionary);
  fclose (fp);
  size_t sentences_to_generate = strtol (argv[2], NULL, BASE);
  for (size_t i = 1; i <= sentences_to_generate; ++i)
    {
      printf (TWEET_FORMAT, i);
      generate_sentence (&dictionary);
    }

  free_dictionary (&dictionary);
  return EXIT_SUCCESS;
}
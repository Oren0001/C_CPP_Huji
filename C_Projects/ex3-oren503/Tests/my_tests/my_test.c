#include "../../tweetsGenerator.c"

#define JUSTDOIT_PATH "/home/orenmotiei/C_CPP/C_Projects/ex3-oren503/Tests/justdoit_tweets.txt"
#define T1_PATH "/home/orenmotiei/C_CPP/C_Projects/ex3-oren503/Tests/my_tests/t2.txt"
#define SEPARATOR "\n========================================\n"

void test_word_and_probability_structs (LinkList *dict)
{
  FILE *fp = NULL;
  if (!open_file (&fp, T1_PATH))
    exit (EXIT_FAILURE);
  fill_dictionary (fp, -1, dict);
  if (dict->size != 15)
    {
      printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
              "Reason - Dictionary size should be 15. Actual size was: ", dict->size);
      exit (EXIT_FAILURE);
    }
  char *word = NULL;
  WordProbability *problist = NULL;
  unsigned int occurrences = NULL;
  unsigned int problist_size = NULL;
  Node *cur = dict->first;
  while (cur != NULL)
    {
      word = cur->data->word;
      problist = cur->data->prob_list;
      occurrences = cur->data->occurrences;
      problist_size = cur->data->prob_list_size;

      if (!strcmp (word, "done"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'done' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'done' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "is"))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'done' prob_list does not contain 'is'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'is' occurrences after 'done' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (!strcmp (word, "is"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'is' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'is' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "better"))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'is' prob_list does not contain 'better'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'better' occurrences after 'is' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (!strcmp (word, "better"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'better' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'is' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "than"))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'better' prob_list does not contain 'than'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'than' occurrences after 'better' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (!strcmp (word, "than"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'than' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'than' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "perfect."))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'than' prob_list does not contain 'perfect.'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'perfect.' occurrences after 'than' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (!strcmp (word, "perfect."))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'perfect.' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 0)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'perfect.' prob_list size should be 0. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          if (problist != NULL)
            {
              printf ("%s\n%s\n%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'perfect.'s prob_list is not NULL");
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "my"))
        {
          if (occurrences != 3)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'my' occurrences should be 3. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 2)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'my' prob_list size should be 2. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *cons_1 = problist[0].word_struct_ptr->word;
          char *cons_2 = problist[1].word_struct_ptr->word;
          if (strcmp (cons_1, "favorites") || strcmp (cons_2, "kinda"))
            {
              printf ("%s\n%s\n%s%s%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'my' prob_list does not contain 'favorites' or 'kinda. "
                      "Actual was: ", cons_1, " and ", cons_2);
              exit (EXIT_FAILURE);
            }
          unsigned int cons_count_1 = problist[0].occurrences;
          unsigned int cons_count_2 = problist[1].occurrences;
          if ((cons_count_1 != 2) || (cons_count_2 != 1))
            {
              printf ("%s\n%s\n%s%u%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'favorites' occurrences after 'my' should be 2,\n          "
                      "and #of 'kinda' occurents after 'my' should be 1 .\n          Actual was: ",
                      cons_count_1, cons_count_2);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "favorites"))
        {
          if (occurrences != 2)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'favorites' occurrences should be 2. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 2)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'favorites' prob_list size should be 2. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *cons_1 = problist[0].word_struct_ptr->word;
          char *cons_2 = problist[1].word_struct_ptr->word;
          if (strcmp (cons_1, "so") || strcmp (cons_2, "#lol."))
            {
              printf ("%s\n%s\n%s%s%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'favorites' prob_list does not contain 'so' or '#lol. "
                      "Actual was: ", cons_1, " and ", cons_2);
              exit (EXIT_FAILURE);
            }
          unsigned int cons_count_1 = problist[0].occurrences;
          unsigned int cons_count_2 = problist[1].occurrences;
          if ((cons_count_1 != 1) || (cons_count_2 != 1))
            {
              printf ("%s\n%s\n%s%u%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'so' occurrences after 'favorites' should be 1,\n          "
                      "and #of '#lol.' occurents after 'favorites' should be 1 ."
                      "\n          Actual was: ", cons_count_1, cons_count_2);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "so"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'so' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'so' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "far"))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'so' prob_list does not contain 'far'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'far' occurrences after 'so' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "far"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'far' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'far' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "#justdoit."))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'far' prob_list does not contain '#justdoit.'. Actual was: ",
                      consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of '#justdoit.' occurrences after 'far' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "#justdoit."))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit.' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 0)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit.' prob_list size should be 0. Actual was: ",
                      problist_size);
              exit (EXIT_FAILURE);
            }
          if (problist != NULL)
            {
              printf ("%s\n%s\n%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit.'s prob_list is not NULL.");
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "kinda"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'kinda' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'kinda' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "lady."))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'kinda' prob_list does not contain 'lady.'. Actual was: ", consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of 'lady.' occurrences after 'kinda' should be 1. Actual was: ",
                      consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "lady."))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit.' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 0)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit.' prob_list size should be 0. Actual was: ",
                      problist_size);
              exit (EXIT_FAILURE);
            }
          if (problist != NULL)
            {
              printf ("%s\n%s\n%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - 'lady.'s prob_list is not NULL.");
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "#justdoit"))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit' prob_list size should be 1. Actual was: ", problist_size);
              exit (EXIT_FAILURE);
            }
          char *consecutive = problist[0].word_struct_ptr->word;
          if (strcmp (consecutive, "#nike!!!."))
            {
              printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#justdoit' prob_list does not contain '#nike!!!.'. Actual was: ",
                      consecutive);
              exit (EXIT_FAILURE);
            }
          unsigned int consecutive_count = problist[0].occurrences;
          if (consecutive_count != 1)
            {
              printf ("%s\n%s\n%s%u", "TEST FAILED:", "File - t2.txt",
                      "Reason - #of '#nike!!!.' occurrences after '#justdoit' should be 1."
                      " Actual was: ", consecutive_count);
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "#nike!!!."))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#nike!!!.' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 0)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#nike!!!.' prob_list size should be 0. Actual was: ",
                      problist_size);
              exit (EXIT_FAILURE);
            }
          if (problist != NULL)
            {
              printf ("%s\n%s\n%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#nike!!!.'s prob_list is not NULL.");
              exit (EXIT_FAILURE);
            }
        }
      else if (strcmp (word, "#lol."))
        {
          if (occurrences != 1)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#lol.' occurrences should be 1. Actual was: ", occurrences);
              exit (EXIT_FAILURE);
            }
          if (problist_size != 0)
            {
              printf ("%s\n%s\n%s%d", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#lol.' prob_list size should be 0. Actual was: ",
                      problist_size);
              exit (EXIT_FAILURE);
            }
          if (problist != NULL)
            {
              printf ("%s\n%s\n%s", "TEST FAILED:", "File - t2.txt",
                      "Reason - '#lol.'s prob_list is not NULL.");
              exit (EXIT_FAILURE);
            }
        }
      else
        {
          printf ("%s\n%s\n%s%s", "TEST FAILED:", "File - t2.txt",
                  "Reason - the word does not exist in the file, and it is: ", word);
          exit (EXIT_FAILURE);
        }
      cur = cur->next;
    }
  printf ("%s%s", "TESTING WORD AND PROBABILITY STRUCT PASSED SUCCESSFULLY", SEPARATOR);
  fclose (fp);
}

void print_specific_word_struct (LinkList *dict, char *path,
                                 char *word_to_find)
{
  FILE *fp = NULL;
  if (!open_file (&fp, path))
    exit (EXIT_FAILURE);
  fill_dictionary (fp, 100, dict);
  Node *cur = dict->first;
  printf ("PRINTING INFO ON - '%s':\n", word_to_find);
  char *word = NULL;
  WordProbability *problist = NULL;
  unsigned int occurrences = 0, problist_size = 0;
  while (cur != NULL)
    {
      word = cur->data->word;
      if (strcmp (word, word_to_find) == 0)
        {
          problist = cur->data->prob_list;
          occurrences = cur->data->occurrences;
          problist_size = cur->data->prob_list_size;
          for (int i = 0; i < problist_size; i++)
            {
              printf ("Consecutive %d: %s\n", i, problist[i].word_struct_ptr->word);
            }
          printf ("%s'%s'\n", "word: ", word);
          printf ("%s%u\n%s%u", "occurrences: ", occurrences, "prob_list size: ", problist_size);
          printf ("%s", SEPARATOR);
        }
      cur = cur->next;
    }

  fclose (fp);
}

void print_word_structs (LinkList *dict, char *path)
{
  FILE *fp = NULL;
  if (!open_file (&fp, path))
    exit (EXIT_FAILURE);
  fill_dictionary (fp, -1, dict);
  Node *cur = dict->first;
  printf ("%s", "PRINTING ALL OF THE WORD STRUCTS:\n\n");
  char *word = NULL;
  WordProbability *problist = NULL;
  unsigned int occurrences = 0, problist_size = 0;
  while (cur != NULL)
    {
      word = cur->data->word;
      problist = cur->data->prob_list;
      occurrences = cur->data->occurrences;
      problist_size = cur->data->prob_list_size;
      for (int i = 0; i < problist_size; i++)
        {
          printf ("Consecutive %d: %s\n", i, problist[i].word_struct_ptr->word);
        }
      printf ("%s'%s'\n", "word: ", word);
      printf ("%s%u\n%s%u", "occurrences: ", occurrences, "prob_list size: ", problist_size);
      printf ("%s", SEPARATOR);
      cur = cur->next;
    }

  fclose (fp);
}

void print_dictionary (LinkList *dict, char *path)
{
  FILE *fp = NULL;
  if (!open_file (&fp, path))
    exit (EXIT_FAILURE);
  fill_dictionary (fp, -1, dict);
  Node *cur = dict->first;
  while (cur != NULL)
    {
      printf ("%s\n", cur->data->word);
      cur = cur->next;
    }

  fclose (fp);
}

int main (int argc, char *argv[])
{
  LinkList dict = {NULL, NULL, 0};

//  print_dictionary (&dict, JUSTDOIT_PATH);
//  print_word_structs (&dict, T1_PATH);
//  test_word_and_probability_structs (&dict);
  char *word_to_find = "#justdoit";
  print_specific_word_struct (&dict, JUSTDOIT_PATH, word_to_find);

  return EXIT_SUCCESS;
}


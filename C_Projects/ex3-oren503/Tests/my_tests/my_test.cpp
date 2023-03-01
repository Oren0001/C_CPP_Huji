#define BOOST_TEST_MODULE ex3
#include <boost/test/included/unit_test.hpp>
#include "../../tweetsGenerator.c"
#include "./my_test.c"
//#include "../../../../lib/FakeIt-master/single_header/boost/fakeit.hpp"


BOOST_AUTO_TEST_CASE(test_fill_dictionary) {
  LinkList dict = {NULL, NULL, 0};
  fill_dictionary (get_file_pointer(), -1, &dict);
  for (int i=0; i<dict.size; i++) {

  }
  Node *cur = dict.first;
  while (cur != NULL) {
    printf("%s", cur->data->word);
    cur = cur->next;
  }
  BOOST_CHECK_EQUAL(1, 1);
}

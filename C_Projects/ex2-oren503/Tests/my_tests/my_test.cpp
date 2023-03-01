#define BOOST_TEST_MODULE ex2
#include <boost/test/included/unit_test.hpp>
#include "../../manageStudents.c"
//#include "../../../../lib/FakeIt-master/single_header/boost/fakeit.hpp"


BOOST_AUTO_TEST_CASE(test_get_number_of_students) {
  freopen("D:\\Drive_Backup\\Projects\\C_CPP\\C_Project\\ex2-oren503\\Tests\\my_tests\\my_test_files\\num1.txt", "r", stdin);
  BOOST_CHECK_EQUAL(get_number_of_students(), 3);
}

#include "tc_isJSONObject.h"
#include "utils/json.h"

#include <stdlib.h>

START_TEST(test_emptyObject) {
  const char* json = "{}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

START_TEST(test_noObject) {
  const char* json = "[]";
  ck_assert_msg(isJSONObject(json) == 0, "Did falsly parse as json object");
  json = "just a string";
  ck_assert_msg(isJSONObject(json) == 0, "Did falsly parse as json object");
}
END_TEST

START_TEST(test_objectWithStringElement) {
  const char* json = "{\"key\":\"value\"}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

START_TEST(test_objectWithNumberElement) {
  const char* json = "{\"key\":42}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

START_TEST(test_objectWithArrayElement) {
  const char* json = "{\"key\":[10,20]}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

START_TEST(test_objectWithObjectElement) {
  const char* json = "{\"key\":{\"innerkey\":42}}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

START_TEST(test_objectWithMultipleElement) {
  const char* json =
      "{\"key\":\"value\",\"key2\":\"value2\", \"key3\" : \"value3\"\n}";
  ck_assert_msg(isJSONObject(json) == 1, "Did not parse as json object");
}
END_TEST

TCase* test_case_isJSONObject() {
  TCase* tc_isJSONObject = tcase_create("isJSONObject");
  tcase_add_test(tc_isJSONObject, test_emptyObject);
  tcase_add_test(tc_isJSONObject, test_noObject);
  tcase_add_test(tc_isJSONObject, test_objectWithStringElement);
  tcase_add_test(tc_isJSONObject, test_objectWithNumberElement);
  tcase_add_test(tc_isJSONObject, test_objectWithArrayElement);
  tcase_add_test(tc_isJSONObject, test_objectWithObjectElement);
  tcase_add_test(tc_isJSONObject, test_objectWithMultipleElement);
  return tc_isJSONObject;
}

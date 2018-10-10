#ifndef OIDC_JSON_H
#define OIDC_JSON_H

#include "key_value.h"
#include "oidc_error.h"

#include "../lib/cJSON/cJSON.h"
#include "../lib/list/src/list.h"

char*  jsonToString(cJSON* cjson);
cJSON* stringToJson(const char* json);
void   clearFreeJson(cJSON* cjson);
char*  getJSONValue(const cJSON* cjson, const char* key);
char*  getJSONValueFromString(const char* json, const char* key);

// char* getJSONValue(const char* json, const char* key);
// int   getJSONValues(const char* json, struct key_value* pairs, size_t size);
// char* getValuefromTokens(jsmntok_t t[], int r, const char* key,
//                          const char* json);
// int   jsoneq(const char* json, jsmntok_t* tok, const char* s);
// int   checkParseResult(int r, jsmntok_t t);
// char* json_addValue(char* json, const char* key, const char* value);
// char* json_addStringValue(char* json, const char* key, const char* value);
// char* json_arrAdd(char* json, const char* value);
// int   json_hasKey(char* json, const char* key);
// oidc_error_t checkArrayParseResult(int r, jsmntok_t t);
// int          JSONArrrayToArray(const char* json, char** arr);
// char*        JSONArrrayToDelimitedString(const char* json, char delim);
// list_t*      JSONArrayToList(const char* json);
// int          isJSONObject(const char* json);
// char*        generateJSONObject(char* k1, char* v1, int isString1, ...);
// char*        generateJSONArray(char* v1, ...);
// char*        mergeJSONObjects(char* j1, char* j2);
// list_t*      getKeysfromTokens(jsmntok_t t[], int r, const char* json,
//                                int strHasToBeValid);
// list_t*      getJSONKeys(const char* json, int strHasToBeValid);
// char*        mergeJSONObject(const char* j1, const char* j2);

#endif  // OIDC_JSON_H

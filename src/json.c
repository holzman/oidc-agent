#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "json.h"
#include "oidc_error.h"
#include "oidc_utilities.h"


/** @fn char* getJSONValue(const char* json, const char* key)
 * @brief parses a json string and returns the value for a given \p key
 * @param json the json string
 * @param key the key
 * @return the value for the \p key
 */
char* getJSONValue(const char* json, const char* key) {
  if(NULL==json || NULL==key) {
    oidc_errno = OIDC_EARGNULL;
    return NULL;
  }
  int r;
  jsmn_parser p;
  jsmn_init(&p);
  int token_needed = jsmn_parse(&p, json, strlen(json), NULL, 0);
  syslog(LOG_AUTHPRIV|LOG_DEBUG, "Token needed for parsing: %d",token_needed);
  if(token_needed < 0) {
    oidc_errno = OIDC_EJSONPARS;
    return NULL;
  }
  jsmntok_t t[token_needed]; 
  jsmn_init(&p);
  r = jsmn_parse(&p, json, strlen(json), t, sizeof(t)/sizeof(t[0]));

  if(checkParseResult(r, t[0])!=OIDC_SUCCESS)	{
    return NULL;
  }
  char* value;
  if((value = getValuefromTokens(t, r, key, json))==NULL) {
    oidc_errno = OIDC_EJSONNOFOUND;
  }
  return value;
}

/** @fn int getJSONValues(const char* json, struct key_value* pairs, size_t size)
 * @brief gets multiple values from a json string
 * @param json the json string to be parsed
 * @param pairs an array of key_value pairs. The keys are used as keys. A
 * pointer to the result is stored in the value field. The previous pointer is
 * not freed, thus it should be NULL.
 * @param size the number of key value pairs
 * return the number of set values or -1 on failure
 */
oidc_error_t getJSONValues(const char* json, struct key_value* pairs, size_t size) {
  oidc_error_t e;
  if(NULL==json || NULL==pairs || size==0) {
    oidc_errno = OIDC_EARGNULL;
    return OIDC_EARGNULL;
  }
  int r;
  jsmn_parser p;
  jsmn_init(&p);
  int token_needed = jsmn_parse(&p, json, strlen(json), NULL, 0);
  if(token_needed < 0) {
    oidc_errno = OIDC_EJSONPARS;
    return OIDC_EJSONPARS;
  }
  syslog(LOG_AUTHPRIV|LOG_DEBUG, "Token needed for parsing: %d",token_needed);
  jsmntok_t t[token_needed]; 
  jsmn_init(&p);
  r = jsmn_parse(&p, json, strlen(json), t, sizeof(t)/sizeof(t[0]));

  if((e = checkParseResult(r, t[0]))!=OIDC_SUCCESS) {
    return e;
  }
  unsigned int i;
  for(i=0; i<size;i++){
    pairs[i].value = getValuefromTokens(t, r, pairs[i].key, json);
  }
  return i;
}

char* getValuefromTokens(jsmntok_t t[], int r, const char* key, const char* json) {
  /* Loop over all keys of the root object */
  int i;
  for (i = 1; i < r; i++) {
    if (jsoneq(json, &t[i], key) == 0) {
      if(i==r-1) {
        return NULL;
      }
      /* We may use strndup() to fetch string value */
      char* value = oidc_sprintf("%.*s", t[i+1].end-t[i+1].start, json + t[i+1].start);
      return value;
    } 
  }
  return NULL;
}

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if(tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

oidc_error_t checkParseResult(int r, jsmntok_t t) {
  if(r < 0) {
    syslog(LOG_AUTHPRIV|LOG_ERR, "Failed to parse JSON: %d\n", r);
    oidc_errno = OIDC_EJSONPARS;
    return OIDC_EJSONPARS;
  }

  /* Assume the top-level element is an object */
  if(r < 1 || t.type != JSMN_OBJECT) {
    syslog(LOG_AUTHPRIV|LOG_ERR, "Object expected\n");
    oidc_errno = OIDC_EJSONOBJ;
    return OIDC_EJSONOBJ;
  }
  return OIDC_SUCCESS;
}

char* json_arrAdd(char* json, const char* value) {
  if(json==NULL || value==NULL) {
    oidc_errno = OIDC_EARGNULL;
    return json;
  }
  const char* const fmt = "%s, \"%s\"]";
  int len = strlen(json);
  if(json[len-1]!=']') {
    oidc_errno = OIDC_EJSONADD;
    return json;
  }
  json[len-1] = '\0';
  char* tmp = oidc_sprintf(fmt, json, value);
  if(tmp==NULL) {
    return json;
  }
  if(tmp[1]==',') {
    memmove(tmp+1, tmp+3, strlen(tmp+3)); //removes the the added comma and space if there was no element in the array
    tmp[strlen(tmp)-1]='\0';
    tmp[strlen(tmp)-1]='\0'; //removes the two last char (we moved everything two chars to the front)
  }
  clearFreeString(json);
  oidc_errno = OIDC_SUCCESS;
  return tmp;

}

char* json_addValue(char* json, const char* key, const char* value) {
  if(json==NULL || key==NULL || value==NULL) {
    oidc_errno = OIDC_EARGNULL;
    return json;
  }
  const char* const fmt = "%s, \"%s\":%s}";
  int len = strlen(json);
  if(json[len-1]!='}') {
    oidc_errno = OIDC_EJSONADD;
    return json;
  }
  json[len-1] = '\0';
  char* tmp = oidc_sprintf(fmt, json, key, value);
  if(tmp==NULL) {
    return json;
  }
  if(tmp[1]==',') {
    tmp[1]=' ';
  }
  clearFreeString(json);
  oidc_errno = OIDC_SUCCESS;
  return tmp;
}

char* json_addStringValue(char* json, const char* key, char* value) {
  if(json==NULL || key==NULL || value==NULL) {
    oidc_errno = OIDC_EARGNULL;
    return json;
  }
  char* tmp = oidc_sprintf("\"%s\"", value);
  if(tmp==NULL) {
    return json;
  }
  char* res = json_addValue(json, key, tmp);
  clearFreeString(tmp);
  return res;
}

int json_hasKey(char* json, const char* key) {
  char* value = getJSONValue(json, key);
  if(value) {
    clearFreeString(value);
    return 1;
  } else {
    return 0;
  }
}

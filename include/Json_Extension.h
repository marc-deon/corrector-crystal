#ifndef JSON_EXTENSION_H
#define JSON_EXTENSION_H

#include <stdbool.h>

int json_get_default_int(const struct json_object* sourceObject, const char* key, int defaultValue);
int json_get_int(const struct json_object* sourceObject, const char* key);
bool json_get_default_bool(const struct json_object* sourceObject, const char* key, bool defaultValue);
const char* json_get_default_string(const struct json_object* sourceObject, const char* key, char* defaultValue);
const char* json_get_string(const struct json_object* sourceObject, const char* key);
bool json_get_default_int_array(const struct json_object* sourceObject, const char* key, int* target, int defaultSize);
int json_get_int_array(const struct json_object* sourceObject, const char* key, int* target);

#endif
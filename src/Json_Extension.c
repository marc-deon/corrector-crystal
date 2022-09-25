#include <json-c/json.h>
#include "Json_Extension.h"

int json_get_default_int(const struct json_object* sourceObject, const char* key, int defaultValue) {
    struct json_object* tempObject;
    json_bool success = json_object_object_get_ex(sourceObject, key, &tempObject);
    if (success) {
        return json_object_get_int(tempObject);
    }
    return defaultValue;
    
}

int json_get_int(const struct json_object* sourceObject, const char* key) {
    struct json_object* tempObject;
    json_object_object_get_ex(sourceObject, key, &tempObject);
    return json_object_get_int(tempObject);
}

bool json_get_default_bool(const struct json_object* sourceObject, const char* key, bool defaultValue) {
    struct json_object* tempObject;
    json_bool success = json_object_object_get_ex(sourceObject, key, &tempObject);
    if (success) {
        return json_object_get_boolean(tempObject);
    }
    return defaultValue;
}
const char* json_get_default_string(const struct json_object* sourceObject, const char* key, char* defaultValue) {
    struct json_object* tempObject;
    json_bool success = json_object_object_get_ex(sourceObject, key, &tempObject);
    if (success) {
        return json_object_get_string(tempObject);
    }
    return defaultValue;
}

const char* json_get_string(const struct json_object* sourceObject, const char* key) {
    struct json_object* tempObject;
    json_object_object_get_ex(sourceObject, key, &tempObject);
    return json_object_get_string(tempObject);
}


// returns true if defaulted
bool json_get_default_int_array(const struct json_object* sourceObject, const char* key, int* target, int defaultSize) {
    struct json_object* tempObject;
    json_bool success = json_object_object_get_ex(sourceObject, key, &tempObject);
    if (success) {
        struct array_list* arrlist = json_object_get_array(tempObject);
        int len = array_list_length(arrlist);
        for(int i = 0; i < array_list_length(arrlist); i++) {
            target[i] = json_object_get_int((const json_object*)array_list_get_idx(arrlist, i));
        }
        // return len;
        return 0;
    }
    for(int i = 0; i < defaultSize; i++)
        target[i] = 0;
    return 1;
}

// Return the size of array
int json_get_int_array(const struct json_object* sourceObject, const char* key, int* target) {
    struct json_object* tempObject = NULL;
    json_bool exists = json_object_object_get_ex(sourceObject, key, &tempObject);
    struct array_list* arrlist = json_object_get_array(tempObject);

    for(int i = 0; i < array_list_length(arrlist); i++) {
        target[i] = json_object_get_int((const json_object*)array_list_get_idx(arrlist, i));
    }

    return array_list_length(arrlist);
}
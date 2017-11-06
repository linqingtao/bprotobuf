#include "proto_utils.h"
#include "proto_types.h"
#include <string.h>

bool is_spliter(char c) {
    return (c == ' ' || c == '\t' || c == '[' | c == ']' || c == '=' || c == ';' || c == '\"');
}

int parse(const std::string& str, std::vector<std::string>& vals) {
    //
    int len = str.length();
    int prev = -1;
    int i = 0;
    for (; i < len; ++i) {
       if (str[i] == '\r' ||str[i] == '\n') {
            break;
       }
       if (is_spliter(str[i])) {
            if (prev != -1) {
                vals.push_back(std::string(str.c_str() + prev, i - prev));
                prev = -1;
            }
       } else {
            if (prev == -1) {
                prev = i;
            }
       }
    }
    if (prev != -1) {
        vals.push_back(std::string(str.c_str() + prev, i - prev));
    }
    return 0;
}

int key_type(const std::string& key) {
    if (key == "int8") { return OBJECT_INT8;}
    else if (key == "uint8") { return OBJECT_UINT8;}
    else if (key == "int16") {  return OBJECT_INT16;}
    else if (key == "uint16") { return OBJECT_UINT16;}
    else if (key == "int32") { return OBJECT_INT32;}
    else if (key == "uint32") { return OBJECT_UINT32;}
    else if (key == "int64") { return OBJECT_INT64;}
    else if (key == "uint64") { return OBJECT_UINT64;}
    else if (key == "float") { return OBJECT_FLOAT;}
    else if (key == "double") { return OBJECT_DOUBLE;}
    else if (key == "string") { return OBJECT_STRING;}
    else if (key == "bits") { return OBJECT_BITS;}
    else if (key == "bytes") { return OBJECT_BYTES;}
    else if (key == "var") { return OBJECT_PROTOCOL;}
    else if (strstr(key.c_str(), "array<") != NULL) {
        return OBJECT_ARRAY;
    } else {
        return OBJECT_MESSAGE;
    }
}


std::string upper(const std::string& key) {
    char buffer[1024];
    for (int i = 0; i < key.length(); ++i) {
        if ('a' <= key[i] && key[i] <= 'z') {
            buffer[i] = key[i] + 'A' - 'a';
        } else {
            buffer[i] = key[i];
        }
    }
    buffer[key.length()] = 0;
    return buffer;
}

int parse_spliter(const char* str, char spliter, std::vector<std::string>& vals) {
    if (*str == 0) { return 0;}
    char* ptr = (char*)str;
    char* prev = ptr;
    while(*ptr != 0) {
        if (*ptr == spliter) {
            vals.push_back(std::string(prev, ptr-prev));
            ++ptr;
            prev = ptr;
        } else {
            ++ptr;
        }
    }
    vals.push_back(std::string(prev, ptr-prev));
    return vals.size();
}




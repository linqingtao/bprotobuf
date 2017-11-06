#ifndef __PROTO_UTILS_H__
#define __PROTO_UTILS_H__
#include <string>
#include <vector>

extern int parse(const std::string& str, std::vector<std::string>& vals);

extern int key_type(const std::string& key);
extern std::string upper(const std::string& key);
extern int parse_spliter(const char* str, char spliter, std::vector<std::string>& vals);

#endif


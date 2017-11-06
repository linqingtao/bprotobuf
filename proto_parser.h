#ifndef __PROTO_PARSER_H__
#define __PROTO_PARSER_H__
#include <string>
#include <map>
#include <vector>



struct FileParam {
    bool small_endian;
    bool attach;
    std::vector<std::string> packages;
    std::vector<std::string> import;
};

struct KeyParam {
    std::string name;
    int id;
    int type;
    std::string type_name;
    bool has_default;
    bool has_fixed;
    bool has_max;
    long long default_int;
    std::string default_str;
    double default_float;
    int len;
    short key_type;
    short protocol_key_type;
    std::string protocol_key;
    std::string key_type_name;
    std::map<std::string, std::string> child_protocols;
    std::string len_name;
    bool operator<(const KeyParam& param) const {
        return id<param.id;
    }
    bool operator==(const KeyParam& param) const {
        return id==param.id;
    }
    KeyParam()
    : id(0)
    , type(0)
    , has_default(false)
    , has_fixed(false)
    , has_max(false)
    , default_int(0)
    , default_float(0)
    , len(0)
    , key_type(0)
    , protocol_key_type(0) {}
};


class BProtocol {
public:
    BProtocol();
    ~BProtocol();
public:
    std::string make_struct();
    std::string make_class();
    std::string make_header();
    std::string make_cpp_header();
    std::string make_cc();
    std::string make_cpp();
    bool init(bool small_endian, bool attach, const std::string& name, const std::vector<KeyParam>& keys);
    bool is_plain() const;
private:
    std::vector<KeyParam> m_keys;
    std::string m_name;
    bool m_attach;
    bool m_small_endian;
};

class BProtoParser {
public:
    BProtoParser() {}
    ~BProtoParser();
public:
    bool init(const char* path, const char* file);
    void make_file(const char* path);
    void make_cpp_file(const char* path);
private:
    FileParam m_param;
    std::string m_name;
    std::string m_prefix;
    std::map<std::string, BProtocol*> m_protocols;
};

#endif


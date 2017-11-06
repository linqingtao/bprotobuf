#include <string.h>
#include <sstream>
#include <set>
#include "proto_parser.h"
#include "proto_types.h"
#include "proto_utils.h"
std::string BProtocol::make_class() {
    std::stringstream ss;
    ss << "class " << m_name << " : public BMessage {\r\n";
    // 构造析构函数
    ss << "public:\r\n\t" << m_name << "();\r\n\t~" << m_name << "();\r\n";
    ss << "\t" << m_name << "(const " << m_name << "& msg);\r\n";
    // 成员函数
    ss << "public:\r\n";
    std::vector<KeyParam>::iterator iter = m_keys.begin();
    std::vector<KeyParam>::iterator end = m_keys.end();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        switch(key.type)
        {
            case OBJECT_INT8:
            {
                ss << "\tint8_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(int8_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_UINT8:
            {
                ss << "\tuint8_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(uint8_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_INT16:
            {
                ss << "\tint16_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(int16_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_UINT16:
            {
                ss << "\tuint16_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(uint16_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_INT32:
            {
                ss << "\tint32_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(int32_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_UINT32:
            {
                ss << "\tuint32_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(uint32_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_INT64:
            {
                ss << "\tint64_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(int64_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_UINT64:
            {
                ss << "\tuint64_t get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(uint64_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_FLOAT:
            {
                ss << "\tfloat get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(float val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_DOUBLE:
            {
                ss << "\tdouble get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(double val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_STRING:
            {
                if (key.len> 0) {
                    ss << "\tstd::string get_" << key.name << "() const { return " << key.name << ";}\r\n";
                    if (key.has_max) {
                        ss << "\tvoid set_" << key.name << "(const std::string& str_) { int _tmp = snprintf(" << key.name << ", sizeof(" << key.name << "), \"%s\", str_.c_str()); " << key.len_name  << " += _tmp; }\r\n";
                    } else {
                        ss << "\tvoid set_" << key.name << "(const std::string& str_) { snprintf(" << key.name << ", sizeof(" << key.name << "), \"%s\", str_.c_str());}\r\n";
                    }
                } else {
                    ss << "\tstd::string get_" << key.name << "() const { return " << key.name << ";}\r\n";
                    ss << "\tvoid set_" << key.name << "(const std::string& str_) { " << key.name << " = str_; " << key.len_name << " = str_.length(); }\r\n";
                }
                break;
            }
            case OBJECT_BITS:
            {
                ss << "\tint get_" << key.name << "() const { return " << key.name << ";}\r\n";
                ss << "\tvoid set_" << key.name << "(uint64_t val) { " << key.name << " = val;}\r\n";
                break;
            }
            case OBJECT_BYTES:
            {
                if (key.len> 0) {
                    ss << "\tconst char* get_" << key.name << "() const { return " << key.name << ";}\r\n";
                    if (key.has_max) {
                        ss << "\tvoid set_" << key.name << "(const char* data_, int len) { memcpy(" << key.name << ", data_, len > sizeof(" << key.name << ") ? sizeof(" << key.name << ") : len); " << key.len_name << " += len > sizeof(" << key.name << ") ? sizeof(" << key.name <<") : len; }\r\n";
                    } else {
                        ss << "\tvoid set_" << key.name << "(const char* data_, int len) { memcpy(" << key.name << ", data_, len > sizeof(" << key.name << ") ? sizeof(" << key.name << ") : len);}\r\n";
                    }
                } else {
                    ss << "\tconst char* get_" << key.name << "() const { return " << key.name << ";}\r\n";
                    ss << "\tvoid set_" << key.name << "(const char* data, int len);\r\n";
                }
                break;
            }
            case OBJECT_ARRAY: {
                switch(key.key_type)
                {
                    case OBJECT_INT8:
                    {
                        ss << "\tint8_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0) { ss << "\tvoid add_" << key.name << "(int idx, int8_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(int8_t val);\r\n";}
                        break;
                                      }
                    case OBJECT_UINT8: 
                    {
                        ss << "\tuint8_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0) { ss << "\tvoid add_" << key.name << "(int idx, uint8_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(uint8_t val);\r\n";}
                        break;
                    }
                    case OBJECT_INT16:
                    {
                        ss << "\tint16_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0) { ss << "\tvoid add_" << key.name << "(int idx, int16_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(int16_t val);\r\n";}
                        break;
                    }
                    case OBJECT_UINT16:
                    {
                        ss << "\tuint16_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0) { ss << "\tvoid add_" << key.name << "(int idx, uint16_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(uint16_t val);\r\n";}
                        break;
                    }
                    case OBJECT_INT32:
                    {
                        ss << "\tint32_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, int32_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(int32_t val);\r\n";}
                        break;
                    }
                    case OBJECT_UINT32:
                    {
                        ss << "\tuint32_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, uint32_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(uint32_t val);\r\n";}
                        break;
                    }
                    case OBJECT_INT64:
                    {
                        ss << "\tint64_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, int64_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(int64_t val);\r\n";}
                        break;
                    }
                    case OBJECT_UINT64:
                    {
                        ss << "\tuint64_t get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, uint64_t val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(uint64_t val);\r\n";}
                        break;
                    }
                    case OBJECT_FLOAT:
                    {
                        ss << "\tfloat get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, float val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(float val);\r\n";}
                        break;
                    }
                    case OBJECT_DOUBLE:
                    {
                        ss << "\tdouble get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, double val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(double val);\r\n";}
                        break;
                    }
                    case OBJECT_MESSAGE:
                    {
                        ss << "\tconst " << key.key_type_name << "& get_" << key.name << "(int i) const { return " << key.name << "[i]; }\r\n";
                        if (key.len > 0 && !key.has_max) { ss << "\tvoid add_" << key.name << "(int idx, const " << key.key_type_name << "& val) { " << key.name << "[idx] = val; }\r\n";}
                        else { ss << "\tvoid add_" << key.name << "(const " << key.key_type_name << "& val);\r\n";}
                        break;
                    }
                }
            }break;
            case OBJECT_MESSAGE:
            {
                ss << "\tconst " << key.type_name << "& get_" << key.name << "() const { return " << key.name << "; }\r\n";
                ss << "\tvoid set_" << key.name << "(const " << key.type_name << "& msg) { " << key.name << " = msg; }\r\n";break;
            }
            case OBJECT_PROTOCOL:
            {
                ss << "\tconst BMessage* get_" << key.name << "() const { return " << key.name << "; }\r\n";
                std::vector<KeyParam>::iterator itr = m_keys.begin();
                for (; itr != iter; ++itr) {
                    if (itr->name == iter->protocol_key) {
                        break;
                    }
                }
                // set all protocols
                std::map<std::string, std::string>::const_iterator it = itr->child_protocols.begin();
                std::map<std::string, std::string>::const_iterator it_end = itr->child_protocols.end();
                for (; it != it_end; ++it) {
                    ss << "void set_" << key.name << "(const " << it->second << "& msg);\r\n";
                }
                break;
            }
        }
    }
    // virtual func
    ss << "public:\r\n";
    ss << "\tvirtual int ByteSize() const;\r\n";
    ss << "\tvirtual int SerializeToArray(char* buffer, int len) const;\r\n";
    ss << "\tvirtual int ParseFromArray(const char* buffer, int len);\r\n";
    // CopyTo and =
    ss << "public:\r\n";
    ss << "\tvoid CopyTo(" << m_name << "& msg) const;\r\n";
    ss << "\t" << m_name << "& operator=(const " << m_name << "& msg);\r\n";
    // for protected params
    ss << "protected:\r\n";
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        switch(key.type)
        {
            case OBJECT_INT8:
            {
                ss << "\tint8_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_UINT8:
            {
                ss << "\tuint8_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_INT16:
            {
                ss << "\tint16_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_UINT16:
            {
                ss << "\tuint16_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_INT32:
            {
                ss << "\tint32_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_UINT32:
            {
                ss << "\tuint32_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_INT64:
            {
                ss << "\tint64_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_UINT64:
            {
                ss << "\tuint64_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_FLOAT:
            {
                ss << "\tfloat " << key.name << ";\r\n";
                break;
            }
            case OBJECT_DOUBLE:
            {
                ss << "\tdouble " << key.name << ";\r\n";
                break;
            }
            case OBJECT_STRING:
            {
                if (key.len> 0) {
                    ss << "\tchar " << key.name << "[" << key.len + 1 << "];\r\n";
                } else {
                    ss << "\tstd::string " << key.name << ";\r\n";
                }
                break;
            }
            case OBJECT_BITS:
            {
                ss << "\tuint64_t " << key.name << ";\r\n";
                break;
            }
            case OBJECT_BYTES:
            {
                if (key.len> 0) {
                    ss << "\tchar " << key.name << "[" << key.len << "];\r\n";
                } else {
                    ss << "\tchar* " << key.name << ";\r\n";
                }
                break;
            }
            case OBJECT_ARRAY: {
                switch(key.key_type)
                {
                    case OBJECT_INT8: {
                        if (key.len > 0) { ss << "\tchar " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tchar* " << key.name << ";\r\n";}
                        break;
                                      }
                    case OBJECT_UINT8: 
                    {
                        if (key.len > 0) {ss << "\tunsigned char " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tunsigned char* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_INT16:
                    {
                        if (key.len > 0) {ss << "\tshort " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tshort* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_UINT16:
                    {
                        if (key.len > 0) {ss << "\tunsigned short " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tunsigned short* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_INT32:
                    {
                        if (key.len > 0) {ss << "\tint " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tint* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_UINT32:
                    {
                        if (key.len > 0) {ss << "\tunsigned int " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tunsigned int* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_INT64:
                    {
                        if (key.len > 0) {ss << "\tlong long " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tlong long* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_UINT64:
                    {
                        if (key.len > 0) {ss << "\tunsigned long long " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tunsigned long long* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_FLOAT:
                    {
                        if (key.len > 0) {ss << "\tfloat " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tfloat* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_DOUBLE:
                    {
                        if (key.len > 0) {ss << "\tdouble " << key.name << "["<< key.len << "];\r\n";}
                        else { ss << "\tint __" << key.name << "_len;\r\n\tdouble* " << key.name << ";\r\n";}
                        break;
                    }
                    case OBJECT_MESSAGE:
                    {
                        if (key.len > 0) {
                            ss << "\t" << key.key_type_name << " " << key.name << "[" << key.len << "];\r\n";
                        } else {
                            ss << "\tstd::vector<" << key.key_type_name << "> " << key.name << ";\r\n";
                        }
                        break;
                    }
                }
            }break;
            case OBJECT_MESSAGE:
            {
                ss << "\t" << key.type_name << " " << key.name << ";\r\n";break;
            }
            case OBJECT_PROTOCOL:
            {
                ss << "\tBMessage* " << key.name << ";\r\n";break;
            }
        }
    }
    //end
    ss << "};\r\n\r\n";
    return ss.str();
}

std::string BProtocol::make_cpp() {
    std::stringstream ss;
    // constructor and destructure
    ss << m_name << "::" << m_name << "()\r\n: BMessage()\r\n";
    // init params
    std::vector<KeyParam>::iterator iter = m_keys.begin();
    std::vector<KeyParam>::iterator end = m_keys.end();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        switch(key.type)
        {
            case OBJECT_INT8:
            case OBJECT_UINT8:
            case OBJECT_INT16:
            case OBJECT_UINT16:
            case OBJECT_INT32:
            case OBJECT_UINT32:
            case OBJECT_INT64:
            case OBJECT_UINT64:
            case OBJECT_FLOAT:
            case OBJECT_DOUBLE:
            case OBJECT_BITS: ss << ", " << key.name << "(0)\r\n";break;
            case OBJECT_PROTOCOL: ss << ", " << key.name << "(NULL)\r\n"; break;
            case OBJECT_MESSAGE: ss << ", " << key.name << "()\r\n";break;
            case OBJECT_ARRAY:
                if (key.len == 0) {
                    switch(key.key_type)
                    {
                        case OBJECT_INT8:
                        case OBJECT_UINT8:
                        case OBJECT_INT16:
                        case OBJECT_UINT16:
                        case OBJECT_INT32:
                        case OBJECT_UINT32:
                        case OBJECT_INT64:
                        case OBJECT_UINT64:
                        case OBJECT_FLOAT:
                        case OBJECT_DOUBLE:
                            ss << ", __" << key.name << "_len(0)\r\n";
                            ss << ", " << key.name << "(NULL)\r\n";
                            break;
                    }
                }
                break;
            case OBJECT_BYTES:
                if (key.len == 0) {
                    ss << ", " << key.name << "(NULL)\r\n";
                }
                break;
        }
    }
    ss << "{\r\n";
    // string init
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type == OBJECT_STRING) {
            if (key.len > 0) {
                ss << "\t" << key.name << "[0] = 0;\r\n";
            }
        }
    }
    ss << "}\r\n";
    ss << m_name << "::~" << m_name << "() {\r\n";
    // clear all the protocol and array and bytes
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;   
        if (key.type == OBJECT_BYTES) {
            if (key.len == 0) {
                ss << "\tif (" << key.name << " != NULL) {\r\n";
                ss << "\t\tdelete[] " << key.name << "; " << key.name << " = NULL;\r\n\t}\r\n";
            }
        } else if (key.type == OBJECT_PROTOCOL) {
            ss << "\tif (" << key.name << " != NULL) {\r\n";
            ss << "\t\tdelete " << key.name << "; " << key.name << " = NULL;\r\n\t}\r\n";
        } else if (key.type == OBJECT_ARRAY) {
            switch (key.key_type)
            {
                case OBJECT_INT8:
                case OBJECT_UINT8:
                case OBJECT_INT16:
                case OBJECT_UINT16:
                case OBJECT_INT32:
                case OBJECT_UINT32:
                case OBJECT_INT64:
                case OBJECT_UINT64:
                case OBJECT_FLOAT:
                case OBJECT_DOUBLE:
                    if (key.len == 0) {
                        ss << "\tif (" << key.name << " != NULL) {\r\n";
                        ss << "\t\tfree(" << key.name << "); " << key.name << " = NULL;\r\n\t}\r\n";
                    }
                    break;
            }
        }
    }
    ss << "}\r\n\r\n";
    ss << m_name << "::" << m_name << "(const " << m_name << "& msg) {\r\n";
    ss << "\tmsg.CopyTo(*this);\r\n}\r\n";
    // =
    ss << m_name << "& " << m_name << "::operator=(const " << m_name << "& msg) {\r\n";
    ss << "\tmsg.CopyTo(*this);\r\n}\r\n";
    // copyto
    ss << "void " << m_name << "::CopyTo(" << m_name << "& msg) const {\r\n";
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        switch(key.type)
        {
            case OBJECT_INT8:
            case OBJECT_UINT8:
            case OBJECT_INT16:
            case OBJECT_UINT16:
            case OBJECT_INT32:
            case OBJECT_UINT32:
            case OBJECT_INT64:
            case OBJECT_UINT64:
            case OBJECT_FLOAT:
            case OBJECT_DOUBLE:
            case OBJECT_BITS: ss << "\tmsg.set_" << key.name << "(" << key.name << ");\r\n";break;
            case OBJECT_PROTOCOL:
            {
                std::vector<KeyParam>::iterator itr = m_keys.begin();
                for (; itr != iter; ++itr) {
                    if (itr->name == iter->protocol_key) {
                        break;
                    }
                }
                // set all protocols
                std::map<std::string, std::string>::const_iterator it = itr->child_protocols.begin();
                std::map<std::string, std::string>::const_iterator it_end = itr->child_protocols.end();
                bool first = true;
                for (; it != it_end; ++it) {
                    if (first) {
                        ss << "\tif ";
                        first = false;
                    } else {
                        ss << "else if ";
                    }
                    if (key.protocol_key_type == OBJECT_STRING) {
                        ss << "(" << key.protocol_key << " == " << "\"" << it->first << "\") {\r\n";
                    } else {
                        ss << "(" << key.protocol_key << " == " << it->first << ") {\r\n";
                    }
                    ss << "\t\tmsg.set_" << key.name << "(*(" << it->second << "*)" << key.name << ");\r\n\t} ";
                }
                ss << "\r\n";
                break;
            }
            case OBJECT_MESSAGE: ss << "\tmsg.set_" << key.name << "(" << key.name << ");\r\n";break;
            case OBJECT_ARRAY:
                ss << "\t{\r\n\t\tint i = 0;\r\n";
                if (key.len > 0 && !key.has_max) {
                    ss << "\t\tint __len = " << key.len << ";\r\n";
                } else {
                    ss << "\t\tint __len = " << key.len_name << ";\r\n";
                }
                ss << "\t\tfor (; i < __len; ++i) {\r\n";
                if (key.len > 0 && !key.has_max) {
                    ss << "\t\t\tmsg.add_" << key.name << "(i, " << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                } else {
                    ss << "\t\t\tmsg.add_" << key.name << "(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                }
                break;
        }
    }
    ss << "}\r\n";
    // serialize&&parse
    ss << "int " << m_name << "::" << "ByteSize() const {\r\n";
    ss << "\tint size = 0;\r\n\tint i = 0;\r\n";
    int plain_size = 0;
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;   
        switch(key.type)
        {
            case OBJECT_INT8:
            case OBJECT_UINT8:plain_size += 1;break;
            case OBJECT_INT16:
            case OBJECT_UINT16:plain_size += 2;break;
            case OBJECT_INT32:
            case OBJECT_UINT32:plain_size += 4;break;
            case OBJECT_INT64:
            case OBJECT_UINT64:plain_size += 8;break;
            case OBJECT_FLOAT: plain_size += sizeof(float);break;
            case OBJECT_DOUBLE:plain_size += sizeof(double);break;
            case OBJECT_BITS:
            //find the near bits together
            {
                int bits = key.len;
                std::vector<KeyParam>::iterator it = iter;
                ++it;
                while (it != end) {
                    if (it->type == OBJECT_BITS) {
                        bits += it->len;
                    } else {
                        break;
                    }
                    ++it;
                }
                iter = --it;
                plain_size += ((bits + 7)/8);
                break;
            }
            case OBJECT_BYTES:
            case OBJECT_STRING:
                if (key.len > 0 && !key.has_max) {
                    plain_size += key.len;
                } else {
                    ss << "\tsize += " << key.len_name << ";\r\n";
                }
                break;
            case OBJECT_MESSAGE: ss << "\tsize += " << key.name << ".ByteSize();\r\n";break;
            case OBJECT_PROTOCOL:
            {
                ss << "\tsize += " << key.name << "->ByteSize();\r\n";break;
                break;
            }
            case OBJECT_ARRAY:
                switch(key.key_type)
                {
                    case OBJECT_INT8:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(char) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(char) * "<<key.len_name << ");\r\n";
                        }
                        break;
                    case OBJECT_UINT8:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned char) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned char) * "<<key.len_name << ");\r\n";
                        }
                        break;
                    case OBJECT_INT16:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(short) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(short) * "<<key.len_name << ");\r\n";
                        }
                        break;
                     case OBJECT_UINT16:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned short) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned short) * "<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_INT32:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(int) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(int) * "<<key.len_name << ");\r\n";
                        }
                        break;
                       case OBJECT_UINT32:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned int) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned int) * "<<key.len_name << ");\r\n";
                        }
                        break;             
                     case OBJECT_INT64:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(long long) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(long long) * "<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_UINT64:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned long long) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned long long) * "<<key.len_name << ");\r\n";
                        }
                        break;              
                     case OBJECT_FLOAT:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(float) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(float) * "<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_DOUBLE:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(double) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(double) * "<<key.len_name << ");\r\n";
                        }
                        break;              
                     case OBJECT_MESSAGE:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tfor (i = 0; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\tsize += "<< key.name << "[i].ByteSize();\r\n\t}\r\n";
                        } else {
                            ss << "\tfor (i = 0; i < " << key.len_name << "; ++i) {\r\n";
                            ss << "\t\tsize += "<< key.name << "[i].ByteSize();\r\n\t}\r\n";
                        }
                        break;
                }
            break;
        }
    }
    // add plain size
    ss << "\tsize += " << plain_size << ";\r\n";
    ss << "\treturn size;\r\n";
    ss << "}\r\n\r\n";

    ss << "int " << m_name << "::ParseFromArray(const char* buffer, int len) {\r\n";
    ss <<"\tint pos = 0;\r\n\tint plain_size = " << plain_size << ";\r\n";
    ss << "\tint diff_size = len - plain_size;\r\n";
    ss << "\tif (len < plain_size) { return PARSE_BUFFER_SHORT;}\r\n";
    int bits_size = 0;
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_BITS) {
            if (bits_size > 0) {
                ss << "\tpos += " << (bits_size + 7)/8 << ";\r\n";
            }
            bits_size = 0;
        }
        switch(key.type)
        {
            case OBJECT_INT8:
                ss <<"\t" << key.name << " = *(char*)(buffer + pos); pos += sizeof(char);\r\n";
                break;
            case OBJECT_UINT8:
                ss <<"\t" << key.name << " = *(unsigned char*)(buffer + pos); pos += sizeof(unsigned char);\r\n";
                break;
            case OBJECT_INT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_16(*(short*)(buffer + pos)); pos += sizeof(short);\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(short*)(buffer + pos); pos += sizeof(short);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(short);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_16(*(short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(short);\r\n";
                }
                break;
            case OBJECT_UINT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_16(*(unsigned short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned short);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_16(*(unsigned short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned short);\r\n";
                }
                break;
            case OBJECT_INT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(int);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(int);\r\n";
                }
                break;
            case OBJECT_UINT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(unsigned int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned int);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(unsigned int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned int);\r\n";
                }
                break;
            case OBJECT_INT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64(*(long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(long long);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64*(long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(long long);\r\n";
                }
                break;
            case OBJECT_UINT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64(*(unsigned long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned long long);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64(*(unsigned long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(unsigned long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned long long);\r\n";
                }
                break;
            case OBJECT_FLOAT:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(float*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(float*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(float);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_32(*(float*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(float*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(float);\r\n";
                }
                break;
            case OBJECT_DOUBLE:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64(*(double*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(double*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(double);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\t" << key.name << " = bswap_64(*(double*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\t" << key.name << " = *(double*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(double);\r\n";
                }
                break;
            case OBJECT_BITS:
                {
                    ss << "\t{\r\n\t\tint i = " << key.len - 1 << ";\r\n\t\tfor (; i >= 0; --i) {\r\n"; 
                    ss << "\t\t\t" << key.name << " <<= 1;\r\n";
                    ss << "\t\t\t" << key.name << " |= (buffer[ pos + (" << bits_size << " + i)/8] & (1 << (((" << bits_size << " + i)%8)))) ? 1 : 0;\r\n\t\t}\r\n\t}\r\n";
                    bits_size += key.len;
                break;
                }
            case OBJECT_STRING:
                if (key.len > 0 && !key.has_max) {
                    ss <<"\tmemcpy(" << key.name << ", buffer + pos, " << key.len << "); " << key.name << "[" << key.len << "] = 0; pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tdiff_size -= " << key.len_name << ";\r\n\tif (diff_size < 0) {\r\n\t\treturn PARSE_BUFFER_SHORT;\r\n\t}\r\n";
                    // if len > max then return false
                    if (key.len > 0 && key.has_max) {
                        ss << "\tif (" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
                    }
                    if (key.len > 0) {
                        ss <<"\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name << "); " << key.name << "[" << key.len_name << "] = 0; pos += " << key.len_name << ";\r\n";
                    } else {
                        ss <<"\t" << key.name << " = std::string(buffer + pos, " << key.len_name << "); pos += " << key.len_name << ";\r\n";
                    }
                }
                break;
            case OBJECT_BYTES:
                if (key.len > 0 && !key.has_max) {
                    ss <<"\tmemcpy(" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tdiff_size -= " << key.len_name << ";\r\n\tif (diff_size < 0) {\r\n\t\treturn PARSE_BUFFER_SHORT;\r\n\t}\r\n";
                    // if len > max then return false
                    if (key.len > 0 && key.has_max) {
                        ss << "\tif (" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
                    }
                    if (!key.has_max) {
                        ss << "\t" << key.name << " = new(std::nothrow) char[" << key.len_name << "];\r\n";
                    }
                    ss <<"\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name << "); pos += " << key.len_name << ";\r\n";
                }
                break;
            case OBJECT_MESSAGE:
                ss << "\t{\r\n\t\tint tmp_" << key.name << "_size = " << key.name << ".ParseFromArray(buffer + pos, len - pos);\r\n";
                ss << "\t\tif (tmp_" << key.name << "_size <= 0) { return tmp_" << key.name << "_size;}\r\n";
                ss << "\t\tdiff_size -= tmp_" << key.name << "_size;\r\n\t\tif (diff_size < 0) {\r\n\t\t\treturn PARSE_BUFFER_SHORT;\r\n\t\t}\r\n";
                ss << "\t\tpos += tmp_" << key.name << "_size;\r\n\t}\r\n";
                break;
            case OBJECT_PROTOCOL:
            { 
                // find the protocol type
                std::vector<KeyParam>::iterator it = m_keys.begin();
                while (it != iter) {
                    if (it->name == key.protocol_key) { break;}
                    ++it;
                }
                // check the protocol
                std::map<std::string, std::string>::iterator itr = it->child_protocols.begin();
                std::map<std::string, std::string>::iterator itr_end = it->child_protocols.end();
                bool first = true;
                for (; itr != itr_end; ++itr) {
                    if (first) {
                        ss << "\tif ";
                        first = false;
                    } else {
                        ss << "else if ";
                    }
                    if (key.protocol_key_type == OBJECT_STRING) {
                        ss << "(" << key.protocol_key << " == \"" << itr->first << "\") {\r\n";
                    } else {
                        ss << "(" << key.protocol_key << " == " << itr->first << ") {\r\n";
                    }
                    ss << "\t\t" << key.name << " = new(std::nothrow) " << itr->second << "();\r\n";
                    ss << "\t\tint tmp_" << itr->second << "_size = " << key.name << "->ParseFromArray(buffer + pos, len - pos);\r\n";
                    ss << "\t\tif (tmp_" << itr->second << "_size <= 0) { return PARSE_INVALID;}\r\n\t\tdiff_size -= tmp_" << itr->second << "_size;\r\n\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                    ss << "\t\tpos += tmp_" << itr->second << "_size;\r\n\t} ";
                }
                ss<< "\r\n";
                break;
            }
            case OBJECT_ARRAY:
            // if len > max then return false
            if (key.len > 0 && key.has_max) {
                ss << "\tif (" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
            }
            switch(key.key_type)
                {
                    case OBJECT_INT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                        } else {
                            ss << "\tdiff_size -= " << key.len_name << ";\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                            if (!key.has_max) {
                                ss << "\t" << key.name << " = (char*)malloc(" << key.len_name << " * sizeof(char));\r\n";
                            }
                            ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name << "); pos += " << key.len_name << ";\r\n";
                        }
                        break;
                    }

                    case OBJECT_UINT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                        } else {
                            ss << "\tdiff_size -= " << key.len_name << ";\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                            if (!key.has_max) {
                                ss << "\t" << key.name << " = (unsigned char*)malloc(" << key.len_name << " * sizeof(unsigned char));\r\n";
                            }
                            ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name << "); pos += " << key.len_name << ";\r\n";
                        }
                        break;
                    }
                case OBJECT_INT16:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(short));\r\n";
                        ss << "\tpos += (sizeof(short) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_16(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(short) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (short*)malloc(" << key.len_name << " * sizeof(short));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(short));\r\n";
                        ss << "\tpos += (sizeof(short) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_16(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_UINT16:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned short));\r\n";
                        ss << "\tpos += (sizeof(unsigned short) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_16(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned short) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (unsigned short*)malloc(" << key.len_name << " * sizeof(unsigned short));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(unsigned short));\r\n";
                        ss << "\tpos += (sizeof(unsigned short) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_16(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_INT32:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(int));\r\n";
                        ss << "\tpos += (sizeof(int) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(int) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (int*)malloc(" << key.len_name << " * sizeof(int));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(int));\r\n";
                        ss << "\tpos += (sizeof(int) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_UINT32:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned int));\r\n";
                        ss << "\tpos += (sizeof(unsigned int) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned int) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (unsigned int*)malloc(" << key.len_name << " * sizeof(unsigned int));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(unsigned int));\r\n";
                        ss << "\tpos += (sizeof(unsigned int) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_INT64:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(long long));\r\n";
                        ss << "\tpos += (sizeof(long long) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(long long) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = nre(std::nothrow) long long[" << key.len_name << "];\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(long long));\r\n";
                        ss << "\tpos += (sizeof(long long) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_UINT64:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned long long));\r\n";
                        ss << "\tpos += (sizeof(unsigned long long) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned long long) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (unsigned long long*)malloc(" << key.len_name << " * sizeof(unsigned long long));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(unsigned long long));\r\n";
                        ss << "\tpos += (sizeof(unsigned long long) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_FLOAT:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(float));\r\n";
                        ss << "\tpos += (sizeof(float) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(float) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " = (float*)malloc(" << key.len_name << " * sizeof(float));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(float));\r\n";
                        ss << "\tpos += (sizeof(float) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_32(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_DOUBLE:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len << " * sizeof(double));\r\n";
                        ss << "\tpos += (sizeof(double) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(double) * " << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (!key.has_max) {
                            ss << "\t" << key.name << " =  (double*)malloc(" << key.len_name << " * sizeof(double));\r\n";
                        }
                        ss << "\tmemcpy(" << key.name << ", buffer + pos, " << key.len_name <<" * sizeof(double));\r\n";
                        ss << "\tpos += (sizeof(double) * " << key.len_name << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n\t\t";
                        ss << "\t" << key.name << "[i] = bswap_64(" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                case OBJECT_MESSAGE:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.name << "[i].ParseFromArray(buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tdiff_size -= msg_size;\r\n\t\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\t{\r\n";
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n";
                        if (!key.has_max) {
                            ss << "\t\t\t" << key.name << ".push_back(" << key.key_type_name << "());\r\n";
                        }
                        ss << "\t\t\tint msg_size = " << key.name << "[i].ParseFromArray(buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tdiff_size -= msg_size;\r\n\t\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                }
                break;
        }

    }
    ss << "\treturn pos;\r\n}\r\n";
    ss << "int " << m_name << "::SerializeToArray(char* buffer, int len) const {\r\n";
    bits_size = 0;
    ss << "\tint pos = 0;\r\n";
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_BITS) {
            if (bits_size > 0) {
                ss << "\tpos += " << (bits_size + 7)/8 << ";\r\n";
            }
            bits_size = 0;
        }
        switch(key.type)
        {
            case OBJECT_INT8:
                ss <<"\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(char)); pos += sizeof(char);\r\n";
                break;
            case OBJECT_UINT8:
                ss <<"\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(unsigned char)); pos += sizeof(unsigned char);\r\n";
                break;
            case OBJECT_INT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tshort tmp = bswap_16(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(short)); pos += sizeof(short);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(short)); pos += sizeof(short);\r\n\t}\r\n";
                break;
            case OBJECT_UINT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned short tmp = bswap_16(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned short)); pos += sizeof(unsigned short);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(unsigned short)); pos += sizeof(unsigned short);\r\n\t}\r\n";
                break;
            case OBJECT_INT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tint tmp = bswap_32(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(int)); pos += sizeof(int);\r\n\t}\r\n";
                break;
            case OBJECT_UINT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned int tmp = bswap_32(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned int)); pos += sizeof(unsigned int);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(unsigned int)); pos += sizeof(unsigned int);\r\n\t}\r\n";
                break;
            case OBJECT_INT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tlong long tmp = bswap_64(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(long long)); pos += sizeof(long long);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(long long)); pos += sizeof(long long);\r\n\t}\r\n";
                break;
            case OBJECT_UINT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned long long tmp = bswap_64(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned long long)); pos += sizeof(unsigned long long);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(unsigned long long)); pos += sizeof(unsigned long long);\r\n\t}\r\n";
                break;
            case OBJECT_FLOAT:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tfloat tmp = bswap_32(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(float)); pos += sizeof(float);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(float)); pos += sizeof(float);\r\n\t}\r\n";
                break;
            case OBJECT_DOUBLE:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tdouble tmp = bswap_64(" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(double)); pos += sizeof(double);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &" << key.name << ", sizeof(double)); pos += sizeof(double);\r\n\t}\r\n";
                break;
            case OBJECT_BITS:
                {
                    int begin = bits_size%8;
                    int cur_bits = 0;
                    int bits = key.len;
                    int idx = 0;
                    while (bits > 0) {
                        cur_bits = 8 - begin;
                        cur_bits = cur_bits > bits ? bits : cur_bits;
                        ss << "\tbuffer[ pos + " << bits_size/8 << "] |= ((" << key.name << ">>" << key.len - bits << ")<<" << begin << ");\r\n";
                        begin = 0;
                        bits -= cur_bits;
                        bits_size += cur_bits;
                        ++idx;
                    }
                    break;
                }
            case OBJECT_STRING:
                if (key.len > 0 && !key.has_max) {
                    ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    if (key.len > 0) {
                        ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len_name << "); pos += " << key.len_name << ";\r\n"; 
                    } else {
                        ss << "\tmemcpy(buffer + pos, " << key.name << ".c_str(), " << key.len_name << "); pos += " << key.len_name << ";\r\n"; 
                    }
                }
                break;
            case OBJECT_BYTES:
                if (key.len > 0 && !key.has_max) {
                    ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len_name << "); pos += " << key.len_name << ";\r\n"; 
                }
                break;
            case OBJECT_MESSAGE:
                ss << "\tpos += " << key.name << ".SerializeToArray(buffer+pos, len-pos);\r\n";
                break;
            case OBJECT_PROTOCOL:
            { 

                ss << "\t{\r\n\t\tint tmp_len = 0;\r\n\t\tif (" << key.name << " != NULL) {\r\n\t\t\ttmp_len = " << key.name << "->SerializeToArray(buffer + pos, len - pos);\r\n\t\t}\r\n";
                ss << "\t\tpos += tmp_len;\r\n\t}\r\n";
                break;
            }
            case OBJECT_ARRAY:
                switch(key.key_type)
                {
                    case OBJECT_INT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(char)); pos += " << sizeof(char) * key.len << ";\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(char)); pos += (" << key.len_name << " * " << sizeof(char) << ");\r\n";
                        }
                        break;
                    }
                    case OBJECT_UINT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned char)); pos += " << sizeof(unsigned char) * key.len << ";\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned char)); pos += (" << key.len_name << " * " << sizeof(char) << ");\r\n";
                        }
                        break;
                    }
                case OBJECT_INT16:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(short)); pos += " << sizeof(short) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(short*)(buffer + pos + i) = bswap_16(*(short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(short)); pos += (" << key.len_name << " * " << sizeof(short) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(short*)(buffer + pos + i) = bswap_16(*(short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                        break;
                    }
                    break;
                case OBJECT_UINT16:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned short)); pos += " << sizeof(unsigned short) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned short*)(buffer + pos + i) = bswap_16(*(unsigned short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned short)); pos += (" << key.len_name << " * " << sizeof(unsigned short) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(unsigned short*)(buffer + pos + i) = bswap_16(*(unsigned short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_INT32:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(int)); pos += " << sizeof(int) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(int*)(buffer + pos + i) = bswap_32(*(int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(int)); pos += (" << key.len_name << " * " << sizeof(int) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(int*)(buffer + pos + i) = bswap_32(*(int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_UINT32:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned int)); pos += " << sizeof(unsigned int) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned int*)(buffer + pos + i) = bswap_32(*(unsigned int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned int)); pos += (" << key.len_name << " * " << sizeof(unsigned int) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(unsigned int*)(buffer + pos + i) = bswap_32(*(unsigned int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_INT64:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(long long)); pos += " << sizeof(long long) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(long long*)(buffer + pos + i) = bswap_64(*(long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(long long)); pos += (" << key.len_name << " * " << sizeof(long long) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(long long*)(buffer + pos + i) = bswap_64(*(long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_UINT64:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned long long)); pos += " << sizeof(unsigned long long) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned long long*)(buffer + pos + i) = bswap_64(*(unsigned long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(unsigned long long)); pos += (" << key.len_name << " * " << sizeof(unsigned long long) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(unsigned long long*)(buffer + pos + i) = bswap_64(*(unsigned long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_FLOAT:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(float)); pos += " << sizeof(float) * key.len<< ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(float*)(buffer + pos + i) = bswap_32(*(float*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(float)); pos += (" << key.len_name << " * " << sizeof(float) << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(float*)(buffer + pos + i) = bswap_32(*(float*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_DOUBLE:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(double)); pos += " << sizeof(double) * key.len<< ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(double*)(buffer + pos + i) = bswap_64(*(double*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, " << key.name << ", " << key.len << " * sizeof(double)); pos += (" << key.len_name << " * " << sizeof(double) <<");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; i += 2) {\r\n";
                            ss << "\t\t\t*(double*)(buffer + pos + i) = bswap_64(*(double*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        }
                    break;
                case OBJECT_MESSAGE:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.name << "[i].SerializeToArray(buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < " << key.len_name << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.name << "[i].SerializeToArray(buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                }
                break;
        }

    }
    ss << "\treturn pos;\r\n}\r\n";

    // set and get func
    // bytes
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_BYTES) { continue;}
        if (key.len == 0) {
            ss << "void " << m_name << "::set_" << key.name << "(const char* buffer, int len) {\r\n";
            ss << "\tif (" << key.name << " != NULL) { delete " << key.name << "; }\r\n";
            ss << "\t" << key.name << " = new(std::nothrow) char[len];\r\n";
            ss << "\tmemcpy("<< key.name << ", buffer, len);\r\n";
            ss << "\t"<< key.len_name << " = len;\r\n";
            ss << "}\r\n";
        }
    }
    // protocol
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_PROTOCOL) { continue;}
        std::vector<KeyParam>::iterator itr = m_keys.begin();
        for (; itr != iter; ++itr) {
            if (itr->name == iter->protocol_key) {
                break;
            }
        }
        // set all protocols
        std::map<std::string, std::string>::const_iterator it = itr->child_protocols.begin();
        std::map<std::string, std::string>::const_iterator it_end = itr->child_protocols.end();
        for (; it != it_end; ++it) {
            ss << "void " << m_name << "::set_" << key.name << "(const " << it->second << "& msg) {\r\n";
            // set protocol afn
            ss << "\t" << key.name << " = " << "new(std::nothrow) " << it->second <<"();\r\n";
            ss << "\tmsg.CopyTo(*(" << it->second << "*)" << key.name << ");\r\n\t";
            switch(key.protocol_key_type)
            {
                case OBJECT_INT8:
                case OBJECT_UINT8:
                case OBJECT_INT16:
                case OBJECT_UINT16:
                case OBJECT_INT32:
                case OBJECT_UINT32:
                case OBJECT_INT64:
                case OBJECT_UINT64:
                case OBJECT_BITS:
                    ss << "" << key.protocol_key << " = " << strtoull(it->first.c_str(), 0, 10);break;
                case OBJECT_FLOAT:
                case OBJECT_DOUBLE:
                    ss << "" << key.protocol_key << " = " << strtod(it->first.c_str(), 0);break;
                case OBJECT_STRING:
                    ss << "" << key.protocol_key << " = \"" << it->first << "\"";break;
            }
            // set protocol
            ss << ";\r\n}\r\n\r\n";
        }
    }
    // array
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_ARRAY) { continue;}
        switch(key.key_type)
        {
            case OBJECT_INT8:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(char val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1)* sizeof(char));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (char*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(char val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_UINT8:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned char val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(unsigned char));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (unsigned char*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned char val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_INT16: 
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(short val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(short));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (short*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(short val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_UINT16:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned short val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" <<  key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(unsigned short));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (unsigned short*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned short val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_INT32:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(int val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" <<  key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(int));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (int*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(int val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_UINT32:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned int val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" <<  key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(unsigned int));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (unsigned int*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned int val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_INT64:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(long long val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(long long));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (long long*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(long long val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_UINT64:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned long long val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(unsigned long long));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (unsigned long long*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(unsigned long long val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_FLOAT:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(float val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(float));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (float*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(float val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_DOUBLE:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(double val) {\r\n";
                    ss << "\tif (__" << key.name << "_len == " << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(" << key.name << ", (" << key.len_name << " * 2 + 1) * sizeof(double));\r\n";
                    ss << "\t\t__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != " << key.name << ") { free(" << key.name << "); " << key.name << " = (double*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(double val) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                 }
                break;
            case OBJECT_MESSAGE:
                if (key.len == 0) {
                    ss << "void " << m_name << "::add_" << key.name << "(const "<< key.key_type_name << "& msg) {\r\n";
                    ss << "\t" << key.name << ".push_back(msg);\r\n";
                    ss << "\t" << key.len_name << "++;\r\n";
                    ss << "}\r\n";
                 } else if (key.has_max) {
                    ss << "void " << m_name << "::add_" << key.name << "(const "<< key.key_type_name << "& msg) {\r\n";
                    ss << "\t" << key.name << "["<< key.len_name << "++] = msg;\r\n";
                    ss << "}\r\n";
                 }
                break;
        }
    }
    return ss.str();
}

void BProtoParser::make_cpp_file(const char* path) {
    // header
    std::stringstream ss;
    ss << "#ifndef __PROTOCOL_" << upper(m_prefix) << "_H__\r\n";
    ss << "#define __PROTOCOL_" << upper(m_prefix) << "_H__\r\n";
    ss << "#include \"proto_cpp.h\"\r\n";
    // import
    std::vector<std::string>::iterator it = m_param.import.begin();
    std::vector<std::string>::iterator it_end = m_param.import.end();
    for (; it != it_end; ++it) {
        ss << "#include \"" << *it << ".bpb.h\"\r\n";
    }

    ss << "#ifdef __cplusplus\r\n" << "extern \"C\" {\r\n";
    ss << "#else\r\n" << "#define const\r\n";
    ss << "#endif\r\n";
    // namespace
    std::vector<std::string>::iterator itr = m_param.packages.begin();
    std::vector<std::string>::iterator itr_end = m_param.packages.end();
    for (; itr != itr_end; ++itr) {
        ss << "namespace " << *itr << " {\r\n";
    }
    
    ss << "// This file is generated by bprotoc. You can not edit it anyway\r\n";
    ss << "// Include the header file to project and then you can use it\r\n";
    std::map<std::string, BProtocol*>::iterator iter = m_protocols.begin();
    std::map<std::string, BProtocol*>::iterator end = m_protocols.end();
    for (; iter != end; ++iter) {
        ss << iter->second->make_class();
    }
    // namespace
    itr = m_param.packages.begin();
    for (; itr != itr_end; ++itr) {
        ss << "}\r\n";
    }
    ss << "#ifdef __cplusplus\r\n" << "}\r\n#endif\r\n";
    ss << "#endif\r\n";
    // write to file
    std::string header_file = path + m_prefix + ".bpb.h";
    FILE* fp = fopen(header_file.c_str(), "w");
    if (fp == NULL) {
        printf("write header file[%s] error\n", header_file.c_str());
        return;
    }
    std::string data = ss.str();
    fwrite(data.c_str(), data.length(), 1, fp);
    fclose(fp);
    // cpp file
    ss.str("");
    ss << "#include \"" << m_prefix << ".bpb.h\"\r\n";
    // namespace
    itr = m_param.packages.begin();
    for (; itr != itr_end; ++itr) {
        ss << "namespace " << *itr << " {\r\n";
    }
    
    iter = m_protocols.begin();
    for (; iter != end; ++iter) {
        ss << iter->second->make_cpp();
    }
    // namespace
    itr = m_param.packages.begin();
    for (; itr != itr_end; ++itr) {
        ss << "}\r\n";
    }
    // write to file
    std::string c_file = path + m_prefix + ".bpb.cpp";
    fp = fopen(c_file.c_str(), "w");
    if (fp == NULL) {
        printf("write cpp file[%s] error\n", c_file.c_str());
        return;
    }
    data = ss.str();
    fwrite(data.c_str(), data.length(), 1, fp);
    fclose(fp);
}




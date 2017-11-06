#include <algorithm>
#include <string.h>
#include <sstream>
#include <set>
#include "proto_parser.h"
#include "proto_types.h"
#include "proto_utils.h"



BProtocol::BProtocol()
: m_attach(false)
, m_small_endian(true) {

}

BProtocol::~BProtocol() {}


bool BProtocol::init(bool small_endian, bool attach, const std::string& name, const std::vector<KeyParam>& keys) {
    m_attach = attach;
    m_small_endian = small_endian;
    m_name = name;
    m_keys = keys;
    std::sort(m_keys.begin(), m_keys.end());
    return true;
}

std::string BProtocol::make_struct() {
    std::stringstream ss;
    ss << "typedef struct ";
    ss << "tag_" << m_name << " {\r\n";
    std::vector<KeyParam>::iterator iter = m_keys.begin();
    std::vector<KeyParam>::iterator end = m_keys.end();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        switch(key.type)
        {
            case OBJECT_INT8: {ss << "\tchar ";}break;
            case OBJECT_UINT8: {ss << "\tunsigned char ";}break;
            case OBJECT_INT16: {ss << "\tshort ";}break;
            case OBJECT_UINT16: {ss << "\tunsigned short ";}break;
            case OBJECT_INT32: {ss << "\tint ";}break;
            case OBJECT_UINT32: {ss << "\tunsigned int ";}break;
            case OBJECT_INT64: {ss << "\tlong long ";}break;
            case OBJECT_UINT64: {ss << "\tunsigned long long ";}break;
            case OBJECT_FLOAT: {ss << "\tfloat ";}break;
            case OBJECT_DOUBLE: {ss << "\tdouble ";}break;
            case OBJECT_STRING:
            {
                if (key.len> 0) {
                    ss << "\tchar " << key.name << "["<< key.len+1 << "];\r\n";
                } else {
                    ss << "\tchar* "<< key.name <<";\r\n";
                }
                break;
            }
            case OBJECT_BITS: {ss << "\tint ";}break;
            case OBJECT_BYTES:
            {
                if (key.len > 0) {
                    ss << "\tchar " << key.name << "["<< key.len << "];\r\n";
                } else {
                    ss << "\tchar* " << key.name <<";\r\n";
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
                            ss << "\tint __" << key.name << "_len;\r\n\t" << key.key_type_name << "* " << key.name << ";\r\n";
                        }
                        break;
                    }
                }
            }break;
            case OBJECT_MESSAGE: {ss << "\t" << key.type_name << " ";}break;
            case OBJECT_PROTOCOL: {ss << "\tvoid* ";}break;
        }
        if (key.type != OBJECT_ARRAY && key.type != OBJECT_BYTES && key.type != OBJECT_STRING) {
            ss << key.name << ";\r\n";
        }
    }
    ss << "} " << m_name << ";\r\n\r\n";
    return ss.str();
}

std::string BProtocol::make_header() {
    std::stringstream ss;
    // default
    ss << "// " << m_name << " default value of keys\r\n";
    std::vector<KeyParam>::iterator iter = m_keys.begin();
    std::vector<KeyParam>::iterator end = m_keys.end();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (!key.has_default && !key.has_fixed) { continue;}
        switch(key.type)
        {
            case OBJECT_INT8: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_int ;}break;
            case OBJECT_UINT8: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << (unsigned int)key.default_int;}break;
            case OBJECT_INT16: {ss << "#defineDEFAULT_" << m_name << "_" << key.name << "  " << key.default_int;}break;
            case OBJECT_UINT16: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << (unsigned int)key.default_int;}break;
            case OBJECT_INT32: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_int;}break;
            case OBJECT_UINT32: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << (unsigned int)key.default_int;}break;
            case OBJECT_INT64: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_int << "ll";}break;
            case OBJECT_UINT64: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << (unsigned long long)key.default_int << "ull";}break;
            case OBJECT_FLOAT: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_float;}break;
            case OBJECT_DOUBLE: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_float << "lf";}break;
            case OBJECT_STRING: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  \"" << key.default_str << "\"";}break;
            case OBJECT_BITS: {ss << "#define DEFAULT_" << m_name << "_" << key.name << "  " << key.default_int;}break;
        }
        ss << "\r\n";
    }
    // create and destroy
    ss << "// " << m_name << " create and destroy functions\r\n";
    ss << "extern void " << m_name << "_Init("<< m_name << "* proto);\r\n";
    ss << "extern void " << m_name << "_Destroy(" << m_name << "* proto);\r\n";
    // serialize&&parse
    ss << "// " << m_name << " stream functions\r\n";
    ss << "// " << m_name << " serialize and parse func for protocol\r\n";
    ss << "extern int " << m_name << "_ParseFromArray(" << m_name << "* proto, char* buffer, int len);\r\n";
    ss << "extern int " << m_name << "_SerializeToArray(" << m_name << "* proto, char* buffer, int len);\r\n";
    ss << "extern int " << m_name << "_ByteSize(" << m_name << "* proto);\r\n";
    ss << "extern int " << m_name << "_CopyTo(" << m_name << "* from, " << m_name << "* to);\r\n";
    // set and get func
    ss << "// " << m_name << " set and get functions\r\n";
    // bytes
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_BYTES) { continue;}
        ss << "extern void " << m_name << "_set_" << key.name << "(" << m_name << "* proto, char* buffer, int len);\r\n";
        ss << "\r\n";
    }
    // string
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_STRING) { continue;}
        ss << "extern void " << m_name << "_set_" << key.name << "(" << m_name << "* proto, const char* buffer, int len);\r\n";
        ss << "\r\n";
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
            ss << "extern void " << m_name << "_set_" << key.name << "_" << it->second << "(" << m_name << "* proto, " << it->second 
                << "* child);\r\n\t";
            ss << "\r\n";
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
                ss << "extern char " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, char val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, char val);\r\n";
                }
                break;
            case OBJECT_UINT8:
                ss << "extern unsigned char " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, unsigned char val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, unsigned char val);\r\n";
                }
                break;
            case OBJECT_INT16: 
                ss << "extern short " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, short val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, short val);\r\n";
                }
                break;
            case OBJECT_UINT16:
                ss << "extern unsigned short " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, unsigned short val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, unsigned short val);\r\n";
                }
                break;
            case OBJECT_INT32:
                ss << "extern int " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, int val);\r\n";
                }
                break;
            case OBJECT_UINT32:
                ss << "extern unsigned int " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, unsigned int val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, unsigned int val);\r\n";
                }
                break;
            case OBJECT_INT64:
                ss << "extern long long " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, long long val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, long long val);\r\n";
                }
                break;
            case OBJECT_UINT64:
                ss << "extern unsigned long long " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, unsigned long long val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, unsigned long long val);\r\n";
                }
                break;
            case OBJECT_FLOAT:
                ss << "extern float " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, float val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, float val);\r\n";
                }
                break;
            case OBJECT_DOUBLE:
                ss << "extern double " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, double val);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, double val);\r\n";
                }
                break;
            case OBJECT_MESSAGE:
                ss << "extern " << key.key_type_name << "* " << m_name << "_get_" << key.name << "(" << m_name << "* proto, int i);\r\n";
                if (key.len == 0 || key.has_max) {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, " << key.key_type_name << "* msg);\r\n";
                } else {
                    ss << "extern void " << m_name << "_add_" << key.name << "(" << m_name << "* proto, int i, " << key.key_type_name << "* msg);\r\n";
                }
                break;
        }
    }
    return ss.str();
}

std::string BProtocol::make_cc() {
    std::stringstream ss;
    // create and destroy
    ss << "// " << m_name << " init and destroy functions\r\n";
    ss << "void "<< m_name << "_Init("<< m_name << "* proto) {\r\n";
    ss << "\tmemset(proto, 0, sizeof(" << m_name << "));\r\n}\r\n\r\n";
    ss << "void " << m_name << "_Destroy(" << m_name << "* proto) {\r\n";
    // clear all the protocol and msg
    std::vector<KeyParam>::iterator iter = m_keys.begin();
    std::vector<KeyParam>::iterator end = m_keys.end();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;   
        if (key.type == OBJECT_MESSAGE) {
            ss << "\t" << key.type_name << "_Destroy(&proto->" << key.name << ");\r\n";
        } else if (key.type == OBJECT_PROTOCOL) {
            // find the afn key
            std::vector<KeyParam>::iterator it = m_keys.begin();
            for (; it != iter; ++it) {
                if (it->name == iter->protocol_key) {
                    break;
                }
            }
            ss << "\tif (proto->" << key.name << " != 0) {\r\n";
            // protocols
            std::map<std::string, std::string>::iterator itr = it->child_protocols.begin();
            std::map<std::string, std::string>::iterator itr_end = it->child_protocols.end();
            bool first = true;
            for (; itr != itr_end; ++itr) {
                switch (it->type)
                {
                    case OBJECT_STRING:
                        if (first) {
                            ss << "\t\tif";
                            first = false;
                        } else {
                            ss << "else if";
                        }
                        ss << " (strcmp(\"" << itr->first.c_str() << "\", proto->" << iter->protocol_key << ") == 0) {\r\n";
                        ss << "\t\t\t" << itr->second << "_Destroy(proto->" << key.name << ");\r\n\t\t} ";
                        break;
                    default:
                        if (first) {
                            ss << "\t\tif";
                            first = false;
                        } else {
                            ss << "else if";
                        }
                        ss << " (" << itr->first.c_str() << " == proto->" << iter->protocol_key << ") {\r\n";
                        ss << "\t\t\t" << itr->second << "_Destroy(proto->" << key.name << ");\r\n\t\t} ";
                        break;
                }
            }
            ss << "\r\n\t}\r\n";
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
                case OBJECT_MESSAGE:
                case OBJECT_DOUBLE:
                    if (key.len == 0) {
                        if (m_attach) {
                            if (m_small_endian) {
                               ss << "\tif (!g_small_endian) {\r\n";
                               ss << "\t\tif (proto->" << key.name << " != 0) { free(proto->" << key.name << ");}\r\n\t}\r\n";
                            } else {
                               ss << "\tif (g_small_endian) {\r\n";
                               ss << "\t\tif (proto->" << key.name << " != 0) { free(proto->" << key.name << ");}\r\n\t}\r\n";
                            }
                        } else {
                            ss << "\tif (proto->" << key.name << " != 0) { free(proto->" << key.name << ");}\r\n";
                        }
                        ss << "\tproto->" <<key.name << " = 0;\r\n";
                    }
                    break;
            }

        }
    }
    // if attach = false then clear bytes and string
    if (!m_attach) {
        iter = m_keys.begin();
        for (; iter != end; ++iter) {
            KeyParam& key = *iter;   
            if (key.type == OBJECT_BYTES || key.type == OBJECT_STRING) {
                if (key.len == 0) {
                    if (!m_attach) {
                        ss << "\tif (proto->" << key.name << " != 0) { free(proto->" << key.name << ");}\r\n";
                    }
                    ss << "\tproto->" <<key.name << " = 0;\r\n";
                }
            }
        }
    }
    ss << "}\r\n\r\n";
    // serialize&&parse
    ss << "// " << m_name << " stream functions\r\n";
    ss << "// " << m_name << " serialize and parse func for protocol\r\n";
    ss << "int " << m_name << "_ByteSize(" << m_name << "* proto) {\r\n";
    ss << "\tint size = 0;\r\n\tint i = 0;\r\n\tif (proto == 0) { return 0;}\r\n";
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
                    ss << "\tsize += proto->" << key.len_name << ";\r\n";
                }
                break;
            case OBJECT_MESSAGE: ss << "\tsize += " << key.type_name << "_ByteSize(&proto->" << key.name << ");\r\n";break;
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
                    if (key.protocol_key_type == OBJECT_STRING) {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(strcmp(proto->" << key.protocol_key << ", " << itr->first << ") == 0) {\r\n";
                        ss << "\t\tsize += " << itr->second << "_ByteSize((" << itr->second << "*)proto->" << key.name << ");\r\n\t} ";
                    } else {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(proto->" << key.protocol_key << " == " << itr->first << ") {\r\n";
                        ss << "\t\tsize += " << itr->second << "_ByteSize((" << itr->second << "*)proto->" << key.name << ");\r\n\t} ";
                    }
                }
                ss << "\r\n";
                break;
            }
            case OBJECT_ARRAY:
                switch(key.key_type)
                {
                    case OBJECT_INT8:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(char) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(char) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;
                    case OBJECT_UINT8:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned char) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned char) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;
                    case OBJECT_INT16:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(short) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(short) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;
                     case OBJECT_UINT16:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned short) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned short) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_INT32:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(int) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(int) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;
                       case OBJECT_UINT32:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned int) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned int) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;             
                     case OBJECT_INT64:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(long long) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(long long) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_UINT64:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(unsigned long long) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(unsigned long long) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;              
                     case OBJECT_FLOAT:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(float) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(float) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;               
                      case OBJECT_DOUBLE:
                        if (key.len > 0 && !key.has_max) {
                            plain_size += (sizeof(double) * key.len);
                        } else {
                            ss << "\tsize += (sizeof(double) * proto->"<<key.len_name << ");\r\n";
                        }
                        break;              
                     case OBJECT_MESSAGE:
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tfor (i = 0; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\tsize += "<< key.key_type_name << "_ByteSize(&proto->"<< key.name <<"[i]);\r\n\t}\r\n";
                        } else {
                            ss << "\tfor (i = 0; i < proto->" << key.len_name << "; ++i) {\r\n";
                            ss << "\t\tsize += "<< key.key_type_name << "_ByteSize(&proto->"<< key.name <<"[i]);\r\n\t}\r\n";
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

    ss << "int " << m_name << "_ParseFromArray(" << m_name << "* proto, char* buffer, int len) {\r\n";
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
                ss <<"\tproto->" << key.name << " = *(char*)(buffer + pos); pos += sizeof(char);\r\n";
                break;
            case OBJECT_UINT8:
                ss <<"\tproto->" << key.name << " = *(unsigned char*)(buffer + pos); pos += sizeof(unsigned char);\r\n";
                break;
            case OBJECT_INT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_16(*(short*)(buffer + pos)); pos += sizeof(short);\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(short*)(buffer + pos); pos += sizeof(short);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(short);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_16(*(short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(short);\r\n";
                }
                break;
            case OBJECT_UINT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_16(*(unsigned short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned short);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_16(*(unsigned short*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned short*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned short);\r\n";
                }
                break;
            case OBJECT_INT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(int);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(int);\r\n";
                }
                break;
            case OBJECT_UINT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(unsigned int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned int);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(unsigned int*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned int*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned int);\r\n";
                }
                break;
            case OBJECT_INT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64(*(long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(long long);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64*(long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(long long);\r\n";
                }
                break;
            case OBJECT_UINT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64(*(unsigned long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned long long);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64(*(unsigned long long*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(unsigned long long*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(unsigned long long);\r\n";
                }
                break;
            case OBJECT_FLOAT:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(float*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(float*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(float);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_32(*(float*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(float*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(float);\r\n";
                }
                break;
            case OBJECT_DOUBLE:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64(*(double*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(double*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(double);\r\n";
                } else {
                    ss << "\tif (g_small_endian) {\r\n";
                    ss <<"\t\tproto->" << key.name << " = bswap_64(*(double*)(buffer + pos));\r\n\t} else {\r\n";
                    ss <<"\t\tproto->" << key.name << " = *(double*)(buffer + pos);\r\n\t}\r\n";
                    ss << "\tpos += sizeof(double);\r\n";
                }
                break;
            case OBJECT_BITS:
                {
                    ss << "\t{\r\n\t\tint i = " << key.len - 1 << ";\r\n\t\tfor (; i >= 0; --i) {\r\n"; 
                    ss << "\t\t\tproto->" << key.name << " <<= 1;\r\n";
                    ss << "\t\t\tproto->" << key.name << " |= (buffer[ pos + (" << bits_size << " + i)/8] & (1 << (((" << bits_size << " + i)%8)))) ? 1 : 0;\r\n\t\t}\r\n\t}\r\n";
                    bits_size += key.len;
                break;
                }
            case OBJECT_STRING:
                if (key.len > 0 && !key.has_max) {
                    ss <<"\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << "); proto->" << key.name << "[" << key.len << "] = 0; pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tdiff_size -= proto->" << key.len_name << ";\r\n\tif (diff_size < 0) {\r\n\t\treturn PARSE_BUFFER_SHORT;\r\n\t}\r\n";
                    if (m_attach && !key.has_max) {
                        ss << "\tproto->" << key.name << " = buffer + pos; pos += proto->" << key.len_name << ";\r\n";
                    } else {
                        // if len > max then return false
                        if (key.len > 0 && key.has_max) {
                            ss << "\tif (proto->" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
                        }
                        if (!key.has_max) {
                            ss << "\tproto->" << key.name << " = (char*)malloc(proto->" << key.len_name << " + 1);\r\n";
                        }
                        ss <<"\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name << "); proto->" << key.name << "[proto->" << key.len_name << "] = 0; pos += proto->" << key.len_name << ";\r\n";
                    }
                }
                break;
            case OBJECT_BYTES:
                if (key.len > 0 && !key.has_max) {
                    ss <<"\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tdiff_size -= proto->" << key.len_name << ";\r\n\tif (diff_size < 0) {\r\n\t\treturn PARSE_BUFFER_SHORT;\r\n\t}\r\n";
                    if (m_attach && !key.has_max) {
                        ss << "\tproto->" << key.name << " = buffer + pos; pos += proto->" <<  key.len_name << ";\r\n";
                    } else {
                        // if len > max then return false
                        if (key.len > 0 && key.has_max) {
                            ss << "\tif (proto->" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
                        }
                        if (!key.has_max) {
                            ss << "\tproto->" << key.name << " = (char*)malloc(proto->" << key.len_name << ");\r\n";
                        }
                        ss <<"\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name << "); pos += proto->" << key.len_name << ";\r\n";
                    }
                }
                break;
            case OBJECT_MESSAGE:
                ss << "\t{\r\n\t\tint tmp_" << key.name << "_size = " << key.type_name << "_ParseFromArray(&proto->" << key.name << ", buffer + pos, len - pos);\r\n";
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
                    if (key.protocol_key_type == OBJECT_STRING) {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(strcmp(proto->" << key.protocol_key << ", " << itr->first << ") == 0) {\r\n";
                        ss << "\t\tproto->" << key.name << " = (" << itr->second << "*)malloc(sizeof(" << itr->second << ")); memset(proto->" << key.name << ", 0, sizeof(" << itr->second << "));\r\n";
                        ss << "\t\tint tmp_" << itr->second << "_size = " << itr->second << "_ParseFromArray(proto->" << key.name << ", buffer + pos, len - pos);\r\n";
                        ss << "\t\tif (tmp_" << itr->second << "_size <= 0) { return PARSE_INVALID;}\r\n\t\tdiff_size -= tmp_" << itr->second << "_size;\r\n\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\tpos += tmp_" << itr->second << "_size;\r\n\t} ";
                    } else {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(proto->" << key.protocol_key << " == " << itr->first << ") {\r\n";
                        ss << "\t\tproto->" << key.name << " = (" << itr->second << "*)malloc(sizeof(" << itr->second << ")); memset(proto->" << key.name << ", 0, sizeof(" << itr->second << "));\r\n";
                        ss << "\t\tint tmp_" << itr->second << "_size = " << itr->second << "_ParseFromArray(proto->" << key.name << ", buffer + pos, len - pos);\r\n";
                        ss << "\t\tif (tmp_" << itr->second << "_size <= 0) { return PARSE_INVALID;}\r\n\t\tdiff_size -= tmp_" << itr->second << "_size;\r\n\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\tpos += tmp_" << itr->second << "_size;\r\n\t} ";
                    }
                }
                ss<< "\r\n";
                break;
            }
            case OBJECT_ARRAY:
            // if len > max then return false
            if (key.len > 0 && key.has_max) {
                ss << "\tif (proto->" << key.len_name << " > " << key.len << ") { return PARSE_INVALID; }\r\n";
            }
            switch(key.key_type)
                {
                    case OBJECT_INT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                        } else {
                            ss << "\tdiff_size -= proto->" << key.len_name << ";\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                            if (m_attach && !key.has_max) {
                                ss << "\tproto->" << key.name << " = buffer + pos;\r\n";
                            } else {
                                if (!key.has_max) {
                                    ss << "\tproto->" << key.name << " = (char*)malloc(proto->" << key.len_name << ");\r\n";
                                }
                                ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name << "); pos += proto->" << key.len_name << ";\r\n";
                            }
                        }
                        break;
                    }

                    case OBJECT_UINT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << "); pos += " << key.len << ";\r\n";
                        } else {
                            ss << "\tdiff_size -= proto->" << key.len_name << ";\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                            if (m_attach && !key.has_max) {
                                ss << "\tproto->" << key.name << " = (unsigned char*)(buffer + pos);\r\n";
                            } else {
                                if (!key.has_max) {
                                    ss << "\tproto->" << key.name << " = (unsigned char*)malloc(proto->" << key.len_name << ");\r\n";
                                }
                                ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name << "); pos += proto->" << key.len_name << ";\r\n";
                            }
                        }
                        break;
                    }
                case OBJECT_INT16:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(short));\r\n";
                        ss << "\tpos += (sizeof(short) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(short) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (short*)(buffer + pos); pos += (sizeof(short) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (short*)(buffer + pos); pos += (sizeof(short) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (short*)malloc(proto->" << key.len_name << " * sizeof(short));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(short));\r\n";
                            ss << "\t\tpos += (sizeof(short) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (short*)malloc(proto->" << key.len_name << " * sizeof(short));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(short));\r\n";
                            ss << "\tpos += (sizeof(short) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_UINT16:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned short));\r\n";
                        ss << "\tpos += (sizeof(unsigned short) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned short) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned short*)(buffer + pos); pos += (sizeof(unsigned short) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned short*)(buffer + pos); pos += (sizeof(unsigned short) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (unsigned short*)malloc(proto->" << key.len_name << " * sizeof(unsigned short));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned short));\r\n";
                            ss << "\t\tpos += (sizeof(unsigned short) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (unsigned short*)malloc(proto->" << key.len_name << " * sizeof(unsigned short));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned short));\r\n";
                            ss << "\tpos += (sizeof(unsigned short) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_16(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_INT32:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(int));\r\n";
                        ss << "\tpos += (sizeof(int) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(int) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (int*)(buffer + pos); pos += (sizeof(int) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (int*)(buffer + pos); pos += (sizeof(int) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (int*)malloc(proto->" << key.len_name << " * sizeof(int));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(int));\r\n";
                            ss << "\t\tpos += (sizeof(int) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (int*)malloc(proto->" << key.len_name << " * sizeof(int));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(int));\r\n";
                            ss << "\tpos += (sizeof(int) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_UINT32:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned int));\r\n";
                        ss << "\tpos += (sizeof(unsigned int) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned int) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned int*)(buffer + pos); pos += (sizeof(unsigned int) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned int*)(buffer + pos); pos += (sizeof(unsigned int) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (unsigned int*)malloc(proto->" << key.len_name << " * sizeof(unsigned int));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned int));\r\n";
                            ss << "\t\tpos += (sizeof(unsigned int) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (unsigned int*)malloc(proto->" << key.len_name << " * sizeof(unsigned int));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned int));\r\n";
                            ss << "\tpos += (sizeof(unsigned int) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_INT64:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(long long));\r\n";
                        ss << "\tpos += (sizeof(long long) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(long long) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (long long*)(buffer + pos); pos += (sizeof(long long) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (long long*)(buffer + pos); pos += (sizeof(long long) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (long long*)malloc(proto->" << key.len_name << " * sizeof(long long));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(long long));\r\n";
                            ss << "\t\tpos += (sizeof(long long) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (long long*)malloc(proto->" << key.len_name << " * sizeof(long long));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(long long));\r\n";
                            ss << "\tpos += (sizeof(long long) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_UINT64:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(unsigned long long));\r\n";
                        ss << "\tpos += (sizeof(unsigned long long) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(unsigned long long) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned long long*)(buffer + pos); pos += (sizeof(unsigned long long) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (unsigned long long*)(buffer + pos); pos += (sizeof(unsigned long long) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (unsigned long long*)malloc(proto->" << key.len_name << " * sizeof(unsigned long long));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned long long));\r\n";
                            ss << "\t\tpos += (sizeof(unsigned long long) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (unsigned long long*)malloc(proto->" << key.len_name << " * sizeof(unsigned long long));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(unsigned long long));\r\n";
                            ss << "\tpos += (sizeof(unsigned long long) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_FLOAT:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(float));\r\n";
                        ss << "\tpos += (sizeof(float) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(float) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (float*)(buffer + pos); pos += (sizeof(float) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (float*)(buffer + pos); pos += (sizeof(float) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (float*)malloc(proto->" << key.len_name << " * sizeof(float));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(float));\r\n";
                            ss << "\t\tpos += (sizeof(float) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (float*)malloc(proto->" << key.len_name << " * sizeof(float));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(float));\r\n";
                            ss << "\tpos += (sizeof(float) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_32(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_DOUBLE:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, " << key.len << " * sizeof(double));\r\n";
                        ss << "\tpos += (sizeof(double) * " << key.len << ");\r\n";
                        if (m_small_endian) {
                            ss << "\tif (!g_small_endian) {\r\n";
                        } else {
                            ss << "\tif (g_small_endian) {\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n\t\t";
                        ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tdiff_size -= (sizeof(double) * proto->" << key.len_name << ");\r\n\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        if (m_attach && !key.has_max) {
                            if (m_small_endian) {
                                ss << "\tif (g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (double*)(buffer + pos); pos += (sizeof(double) * proto->" << key.len_name << ");\r\n\t}\r\n";
                            } else {
                                ss << "\tif (!g_small_endian) {\r\n";
                                ss << "\t\tproto->" << key.name << " = (double*)(buffer + pos); pos += (sizeof(double) * proto->" << key.len_name << ");\r\n\t}\r\n";           
                            }
                            ss << "\telse {\r\n";
                            if (!key.has_max) {
                                ss << "\t\tproto->" << key.name << " = (double*)malloc(proto->" << key.len_name << " * sizeof(double));\r\n";
                            }
                            ss << "\t\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(double));\r\n";
                            ss << "\t\tpos += (sizeof(double) * proto->" << key.len_name << ");\r\n";
                            ss << "{\r\n\t\t\tint i = 0;\r\n\t\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\t\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t\t}\r\n\t\t}\r\n";
                            ss << "\t}\r\n";
                        } else {
                            if (!key.has_max) {
                                ss << "\tproto->" << key.name << " = (double*)malloc(proto->" << key.len_name << " * sizeof(double));\r\n";
                            }
                            ss << "\tmemcpy(proto->" << key.name << ", buffer + pos, proto->" << key.len_name <<" * sizeof(double));\r\n";
                            ss << "\tpos += (sizeof(double) * proto->" << key.len_name << ");\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n\t\t";
                            ss << "\tproto->" << key.name << "[i] = bswap_64(proto->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                        }
                    }
                    break;
                case OBJECT_MESSAGE:
                    if (key.len > 0 && !key.has_max) {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.key_type_name << "_ParseFromArray(&proto->" << key.name << "[i], buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tdiff_size -= msg_size;\r\n\t\t\tif (diff_size < 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\t{\r\n";
                        if (!key.has_max) {
                            ss << "\t\tproto->" << key.name << " = (" << key.key_type_name << "*)malloc(proto->" << key.len_name << " * sizeof(" << key.key_type_name << ")); memset(proto->" << key.name << ", 0, sizeof(" << key.key_type_name << ") * proto->" << key.len_name  << ");\r\n";
                        }
                        ss << "\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.key_type_name << "_ParseFromArray(&proto->" << key.name << "[i], buffer + pos, len - pos);\r\n";
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
    ss << "int " << m_name << "_SerializeToArray(" << m_name << "* proto, char* buffer, int len) {\r\n";
    //ss <<"\tint total = " << m_name << "_ByteSize(proto);\r\n";
    //ss << "\tif (len < total) { return PARSE_BUFFER_SHORT;}\r\n\tint pos = 0;\r\n";
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
                ss <<"\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(char)); pos += sizeof(char);\r\n";
                break;
            case OBJECT_UINT8:
                ss <<"\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(unsigned char)); pos += sizeof(unsigned char);\r\n";
                break;
            case OBJECT_INT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tshort tmp = bswap_16(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(short)); pos += sizeof(short);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(short)); pos += sizeof(short);\r\n\t}\r\n";
                break;
            case OBJECT_UINT16:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned short tmp = bswap_16(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned short)); pos += sizeof(unsigned short);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(unsigned short)); pos += sizeof(unsigned short);\r\n\t}\r\n";
                break;
            case OBJECT_INT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tint tmp = bswap_32(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(int)); pos += sizeof(int);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(int)); pos += sizeof(int);\r\n\t}\r\n";
                break;
            case OBJECT_UINT32:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned int tmp = bswap_32(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned int)); pos += sizeof(unsigned int);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(unsigned int)); pos += sizeof(unsigned int);\r\n\t}\r\n";
                break;
            case OBJECT_INT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tlong long tmp = bswap_64(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(long long)); pos += sizeof(long long);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(long long)); pos += sizeof(long long);\r\n\t}\r\n";
                break;
            case OBJECT_UINT64:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tunsigned long long tmp = bswap_64(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(unsigned long long)); pos += sizeof(unsigned long long);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(unsigned long long)); pos += sizeof(unsigned long long);\r\n\t}\r\n";
                break;
            case OBJECT_FLOAT:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tfloat tmp = bswap_32(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(float)); pos += sizeof(float);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(float)); pos += sizeof(float);\r\n\t}\r\n";
                break;
            case OBJECT_DOUBLE:
                // endian
                if (m_small_endian) {
                    ss << "\tif (!g_small_endian) ";
                } else {
                    ss << "\tif (g_small_endian) ";
                }
                ss << "{\r\n\t\tdouble tmp = bswap_64(proto->" << key.name << ");\r\n";
                ss <<"\t\tmemcpy(buffer + pos, &tmp, sizeof(double)); pos += sizeof(double);\r\n\t} else {\r\n";
                ss <<"\t\tmemcpy(buffer + pos,  &proto->" << key.name << ", sizeof(double)); pos += sizeof(double);\r\n\t}\r\n";
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
                        ss << "\tbuffer[ pos + " << bits_size/8 << "] |= ((proto->" << key.name << ">>" << key.len - bits << ")<<" << begin << ");\r\n";
                        begin = 0;
                        bits -= cur_bits;
                        bits_size += cur_bits;
                        ++idx;
                    }
                    break;
                }
            case OBJECT_STRING:
                if (key.len > 0 && !key.has_max) {
                    ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", proto->" << key.len_name << "); pos += proto->" << key.len_name << ";\r\n"; 
                }
                break;
            case OBJECT_BYTES:
                if (key.len > 0 && !key.has_max) {
                    ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << "); pos += " << key.len << ";\r\n";
                } else {
                    ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", proto->" << key.len_name << "); pos += proto->" << key.len_name << ";\r\n"; 
                }
                break;
            case OBJECT_MESSAGE:
                ss << "\tpos += " << key.type_name << "_SerializeToArray(&proto->" << key.name << ", buffer+pos, len-pos);\r\n";
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
                    if (key.protocol_key_type == OBJECT_STRING) {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(strcmp(proto->" << key.protocol_key << ", " << itr->first << ") == 0) {\r\n";
                        ss << "\t\tpos += " << itr->second << "_SerializeToArray(proto->" << key.name << ", buffer + pos, len - pos);\r\n\t} ";
                    } else {
                        if (first) {
                            ss << "\tif ";
                            first = false;
                        } else {
                            ss << "else if ";
                        }
                        ss << "(proto->" << key.protocol_key << " == " << itr->first << ") {\r\n";
                        ss << "\t\tpos += " << itr->second << "_SerializeToArray(proto->" << key.name << ", buffer + pos, len - pos);\r\n\t} ";
                    }
                }
                ss << "\r\n";
                break;
            }
            case OBJECT_ARRAY:
                switch(key.key_type)
                {
                    case OBJECT_INT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(char)); pos += " << sizeof(char) * key.len << ";\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(char)); pos += (proto->" << key.len_name << " * " << sizeof(char) << ");\r\n";
                        }
                        break;
                    }
                    case OBJECT_UINT8:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned char)); pos += " << sizeof(unsigned char) * key.len << ";\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned char)); pos += (proto->" << key.len_name << " * " << sizeof(char) << ");\r\n";
                        }
                        break;
                    }
                case OBJECT_INT16:
                    {
                        if (key.len > 0 && !key.has_max) {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(short)); pos += " << sizeof(short) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(short*)(buffer + pos + i) = bswap_16(*(short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(short)); pos += (proto->" << key.len_name << " * " << sizeof(short) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned short)); pos += " << sizeof(unsigned short) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned short*)(buffer + pos + i) = bswap_16(*(unsigned short*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned short)); pos += (proto->" << key.len_name << " * " << sizeof(unsigned short) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(int)); pos += " << sizeof(int) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(int*)(buffer + pos + i) = bswap_32(*(int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(int)); pos += (proto->" << key.len_name << " * " << sizeof(int) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned int)); pos += " << sizeof(unsigned int) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned int*)(buffer + pos + i) = bswap_32(*(unsigned int*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned int)); pos += (proto->" << key.len_name << " * " << sizeof(unsigned int) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(long long)); pos += " << sizeof(long long) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(long long*)(buffer + pos + i) = bswap_64(*(long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(long long)); pos += (proto->" << key.len_name << " * " << sizeof(long long) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned long long)); pos += " << sizeof(unsigned long long) * key.len << ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(unsigned long long*)(buffer + pos + i) = bswap_64(*(unsigned long long*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(unsigned long long)); pos += (proto->" << key.len_name << " * " << sizeof(unsigned long long) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(float)); pos += " << sizeof(float) * key.len<< ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(float*)(buffer + pos + i) = bswap_32(*(float*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(float)); pos += (proto->" << key.len_name << " * " << sizeof(float) << ");\r\n";
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
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(double)); pos += " << sizeof(double) * key.len<< ";\r\n";
                            if (m_small_endian) {
                                ss << "\tif (!g_small_endian) {\r\n";
                            } else {
                                ss << "\tif (g_small_endian) {\r\n";
                            }
                            ss << "\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n";
                            ss << "\t\t\t*(double*)(buffer + pos + i) = bswap_64(*(double*)(buffer + pos + i));\r\n\t\t}\r\n\t}\r\n";
                        } else {
                            ss << "\tmemcpy(buffer + pos, proto->" << key.name << ", " << key.len << " * sizeof(double)); pos += (proto->" << key.len_name << " * " << sizeof(double) <<");\r\n";
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
                        ss << "\t\t\tint msg_size = " << key.key_type_name << "_SerializeToArray(&proto->" << key.name << "[i], buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < proto->" << key.len_name << "; ++i) {\r\n";
                        ss << "\t\t\tint msg_size = " << key.key_type_name << "_SerializeToArray(&proto->" << key.name << "[i], buffer + pos, len - pos);\r\n";
                        ss << "\t\t\tif (msg_size <= 0) { return PARSE_BUFFER_SHORT; }\r\n";
                        ss << "\t\t\tpos += msg_size;\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
                }
                break;
        }

    }
    ss << "\treturn pos;\r\n}\r\n";

    // CopyTo func
    ss << "int " << m_name << "_CopyTo(" << m_name << "* from, " << m_name << "* to) {\r\n";
    // for message and string and array and protocol
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type == OBJECT_BYTES) {
            if (!m_attach) {
                if (key.len > 0) {
                    ss << "\tmemcpy(to->" << key.name << ", from->" << key.name << ", " << key.len << ");\r\n";                   
                } else {
                    ss << "\tto->" << key.name << " = (char*)malloc(from->" << key.len_name << ");\r\n";
                    ss << "\tmemcpy(to->" << key.name << ", from->" << key.name << ", from->" << key.len_name << ");\r\n";
                }
            }
        } else if (key.type == OBJECT_STRING) {
            if (!m_attach) {
                if (key.len > 0) {
                    ss << "\tmemcpy(to->" << key.name << ", from->" << key.name << ", " << key.len+1 << ");\r\n";                   
                } else {
                    ss << "\tto->" << key.name << " = (char*)malloc(from->" << key.len_name << " + 1);\r\n";
                    ss << "\tmemcpy(to->" << key.name << ", from->" << key.name << ", from->" << key.len_name << "+ 1);\r\n";
                }
            }
        } else if (key.type == OBJECT_MESSAGE) {
            ss << "\t" << key.type_name << "_CopyTo(" << "&from->" << key.name << ", &to->" << key.name << ");\r\n";           
        } else if (key.type == OBJECT_PROTOCOL) {
            // find the protocol type
            std::vector<KeyParam>::iterator it = m_keys.begin();
            while (it != iter) {
                if (it->name == key.protocol_key) { break;}
                ++it;
            }
            // check the protocol
            bool first = true;
            std::map<std::string, std::string>::iterator itr = it->child_protocols.begin();
            std::map<std::string, std::string>::iterator itr_end = it->child_protocols.end();
            for (; itr != itr_end; ++itr) {
                if (key.protocol_key_type == OBJECT_STRING) {
                    if (first) {
                        ss << "\tif ";
                        first = false;
                    } else {
                        ss << "else if ";
                    }
                    ss << "(strcmp(to->" << key.protocol_key << ", " << itr->first << ") == 0) {\r\n";
                    ss << "\t\tto->" << key.name << " = (" << itr->second << "*)malloc(sizeof(" << itr->second  << "));\r\n";
                    ss << "\t\t" << itr->second << "_CopyTo(from->" << key.name << ", to->" << key.name << ");\r\n\t} ";
                } else {
                    if (first) {
                        ss << "\tif ";
                        first = false;
                    } else {
                        ss << " else if ";
                    }
                    ss << "(to->" << key.protocol_key << " == " << itr->first << ") {\r\n";
                    ss << "\t\tto->" << key.name << " = (" << itr->second << "*)malloc(sizeof(" << itr->second  << "));\r\n";
                    ss << "\t\t" << itr->second << "_CopyTo(from->" << key.name << ", to->" << key.name << ");\r\n\t} ";
                }
            }
            ss << "\r\n";
        } else if (key.type == OBJECT_ARRAY) {
            switch (key.key_type)
            {
                case OBJECT_INT8:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << ");\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (char*)malloc(from->" << key.len_name << ");\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<");\r\n";
                    }
                    break;
                case OBJECT_UINT8:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << ");\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (unsigned char*)malloc(from->" << key.len_name << ");\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<");\r\n";
                    }
                    break;
                case OBJECT_INT16:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(short));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (short*)malloc(from->" << key.len_name << " * sizeof(short));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(short));\r\n";
                    }
                    break;
                case OBJECT_UINT16:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(unsigned short));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (unsigned short*)malloc(from->" << key.len_name << " * sizeof(unsigned short));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(unsigned short));\r\n";
                    }
                    break;
                case OBJECT_INT32:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(int));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (int*)malloc(from->" << key.len_name << " * sizeof(int));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(int));\r\n";
                    }
                    break;
                case OBJECT_UINT32:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(unsigned int));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (unsigned int*)malloc(from->" << key.len_name << " * sizeof(unsigned int));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(unsigned int));\r\n";
                    }
                    break;
                case OBJECT_INT64:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(long long));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (long long*)malloc(from->" << key.len_name << " * sizeof(long long));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(long long));\r\n";
                    }
                    break;
                case OBJECT_UINT64:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(unsigned long long));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (unsigned long long*)malloc(from->" << key.len_name << " * sizeof(unsigned long long));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(unsigned long long));\r\n";
                    }
                    break;
                case OBJECT_FLOAT:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(float));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (float*)malloc(from->" << key.len_name << " * sizeof(float));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(float));\r\n";
                    }
                    break;
                case OBJECT_DOUBLE:
                    if (key.len > 0) {
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", " << key.len << " * sizeof(double));\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (double*)malloc(from->" << key.len_name << " * sizeof(double));\r\n";
                        ss << "\tmemcpy((void*)to->" << key.name << ", (void*)from->" << key.name << ", from->" << key.len_name <<" * sizeof(double));\r\n";
                    }
                    break;
                case OBJECT_MESSAGE:
                    // copy all arr
                    if (key.len > 0) {
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < " << key.len << "; ++i) {\r\n" <<  "\t\t\t" << key.key_type_name << "_CopyTo(&from->" << key.name << "[i], &to->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    } else {
                        ss << "\tto->" << key.name << " = (" << key.key_type_name << "*)malloc(sizeof(" << key.key_type_name << ") * from->" << key.len_name << ");\r\n";
                        ss << "\t{\r\n\t\tint i = 0;\r\n\t\tfor (; i < from->" << key.len_name << "; ++i) {\r\n" <<  "\t\t\t" << key.key_type_name << "_CopyTo(&from->" << key.name << "[i], &to->" << key.name << "[i]);\r\n\t\t}\r\n\t}\r\n";
                    }
                    break;
            }
        } else {
            ss <<"\tto->" << key.name << " = from->" << key.name << ";\r\n";
        }
    }
    ss << "\treturn 0;\r\n}\r\n\r\n";
    // set and get func
    ss << "// " << m_name << " set and get functions\r\n";
    // bytes
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_BYTES) { continue;}
        ss << "void " << m_name << "_set_" << key.name << "(" << m_name << "* proto, const char* buffer, int len) {\r\n";
        if (key.len > 0) {
            ss << "\tint copy_len = len > "<< key.len << " ? "<< key.len << " : len;\r\n";
            ss << "\tmemcpy(proto->"<< key.name << ", buffer, copy_len);\r\n";
            if (key.has_max) {
                ss << "\tproto->" << key.len_name << " = copy_len;\r\n";
            }
        } else {
            ss << "\tproto->" << key.name << " = (char*)malloc(len);\r\n";
            ss << "\tmemcpy(proto->"<< key.name << ", buffer, len);\r\n";
        }
        if (key.len == 0) {
            ss << "\tproto->"<< key.len_name << " = len;\r\n";
        }
        ss << "}\r\n";
    }
    // string
    iter = m_keys.begin();
    for (; iter != end; ++iter) {
        KeyParam& key = *iter;
        if (key.type != OBJECT_STRING) { continue;}
        ss << "void " << m_name << "_set_" << key.name << "(" << m_name << "* proto, const char* buffer, int len) {\r\n";
        if (key.len > 0) {
            ss << "\tint copy_len = len > "<< key.len << " ? "<< key.len << " : len;\r\n";
            ss << "\tmemcpy(proto->"<< key.name << ", buffer, copy_len);\r\n";
            ss << "\tproto->"<< key.name <<"[copy_len] = 0;\r\n";
            if (key.has_max) {
                ss << "\tproto->" << key.len_name << " = copy_len;\r\n";
            }
        } else {
            ss << "\tproto->" << key.name << " = (char*)malloc(len+1);\r\n";
            ss << "\tmemcpy(proto->"<< key.name << ", buffer, len);\r\n";
            ss << "\tproto->"<< key.name << "[len] = 0;\r\n";
        }
        if (key.len == 0) {
            ss << "\tproto->"<< key.len_name << " = len;\r\n";
        }
        ss << "}\r\n";
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
            ss << "void " << m_name << "_set_" << key.name << "_" << it->second << "(" << m_name << "* proto, " << it->second << "* child) {\r\n";
            // set protocol afn
            ss << "\tproto->" << key.name << " = " << "(" << it->second <<"*)malloc(sizeof(" << it->second << "));\r\n";
            ss << "\t" << it->second << "_CopyTo(child, proto->" << key.name << ");\r\n\t";
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
                    ss << "proto->" << key.protocol_key << " = " << strtoull(it->first.c_str(), 0, 10);break;
                case OBJECT_FLOAT:
                case OBJECT_DOUBLE:
                    ss << "proto->" << key.protocol_key << " = " << strtod(it->first.c_str(), 0);break;
                case OBJECT_STRING:
                    ss << "proto->" << key.protocol_key << " = \"" << it->first << "\"";break;
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
                ss << "char " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, char val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1) * sizeof(char));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (char*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, char val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, char val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_UINT8:
                ss << "unsigned char " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned char val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1) * sizeof(unsigned char));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (unsigned char*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned char val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, unsigned char val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_INT16: 
                ss << "short " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, short val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1) * sizeof(short));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (short*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, short val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, short val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_UINT16:
                ss << "unsigned short " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned short val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" <<  key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(unsigned short));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (unsigned short*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned short val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, unsigned short val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_INT32:
                ss << "int " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" <<  key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(int));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (int*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, int val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_UINT32:
                ss << "unsigned int " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned int val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" <<  key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(unsigned int));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (unsigned int*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned int val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, unsigned int val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_INT64:
                ss << "long long " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, long long val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(long long));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (long long*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, long long val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, long long val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_UINT64:
                ss << "unsigned long long " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned long long val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(unsigned long long));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (unsigned long long*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, unsigned long long val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, unsigned long long val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_FLOAT:
                ss << "float " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, float val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(float));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (float*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, float val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, float val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_DOUBLE:
                ss << "double " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn proto->"<< key.name << "[i];\r\n}\r\n\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, double val) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(double));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (double*)tmp; }\r\n\t}\r\n";
                    ss << "\tproto->" << key.name << "[proto->"<< key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, double val) {\r\n";
                    ss << "\tproto->" << key.name << "[proto->" << key.len_name << "++] = val;\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, double val) {\r\n";
                    ss << "\tproto->" << key.name << "[i] = val;\r\n";
                    ss << "}\r\n";
                }
                break;
            case OBJECT_MESSAGE:
                ss << key.key_type_name << "* " << m_name << "_get_" << key.name << "("<< m_name << "* proto, int i) {\r\n";
                ss << "\treturn &proto->" << key.name << "[i];\r\n}\r\n";
                if (key.len == 0) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, "<< key.key_type_name << "* msg) {\r\n";
                    ss << "\tif (proto->__" << key.name << "_len == proto->" << key.len_name << ") {\r\n";
                    ss << "\t\tvoid* tmp = realloc(proto->" << key.name << ", (proto->" << key.len_name << " * 2 + 1)* sizeof(" << key.key_type_name << "));\r\n";
                    ss << "\t\tproto->__" << key.name << "_len <<= 1;\r\n";
                    ss << "\t\t++proto->__" << key.name << "_len;\r\n";
                    ss << "\t\tif (tmp != proto->" << key.name << ") { free(proto->" << key.name << "); proto->" << key.name << " = (" << key.key_type_name << "*)tmp; }\r\n\t}\r\n";
                    ss << "\t" << key.key_type_name << "_CopyTo(msg, &proto->" << key.name << "[proto->" << key.len_name << "++]);\r\n";
                    ss << "}\r\n";
                } else if (key.has_max) {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, " << key.key_type_name << "* msg) {\r\n";
                    ss << "\t" << key.key_type_name << "_CopyTo(msg, &proto->" << key.name << "[proto->" << key.len_name << "++]);\r\n";
                    ss << "}\r\n";                     
                } else {
                    ss << "void " << m_name << "_add_" << key.name << "("<< m_name << "* proto, int i, "<< key.key_type_name << "* msg) {\r\n";
                    ss << "\t" << key.key_type_name << "_CopyTo(msg, &proto->" << key.name << "[i]);\r\n";
                    ss << "}\r\n";
                }
                break;
        }
    }
    return ss.str();
}

bool BProtoParser::init(const char* path, const char* file) {
    char buffer[10240] = {0};
    if (path[0] != 0) {
        sprintf(buffer, "%s/%s", path, file);
    } else {
        sprintf(buffer, "%s", file);
    }
    FILE* fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("file %s open error\n", buffer);
        return false;
    }
    // find name and prefix
    m_name = buffer;
    size_t prefix_pos = m_name.find_last_of(".");
    if (prefix_pos != std::string::npos) {
        m_prefix = m_name.substr(0, prefix_pos);
    } else {
        m_prefix = m_name;
    }
    prefix_pos = m_prefix.find_last_of("/");
    if (prefix_pos != std::string::npos) {
        m_prefix = m_prefix.substr(prefix_pos);
    }
    bool iRet = true;
    // read data
    std::string name;
    bool flag = false;
    int line = 0;
    std::set<int> ids;
    std::set<std::string> cols;
    std::vector<KeyParam> keys;
    while(fgets(buffer, 10240, fp)) {
        ++line;
        std::vector<std::string> vals;
        parse(buffer, vals);
        int size = vals.size();
        if (size > 0) {
            if (vals[0] == "{") { vals.erase(vals.begin());}
            if (vals[0] == "}") { flag = false; vals.erase(vals.begin());}
        }
        size = vals.size();
        if (size > 0) {
            KeyParam param;
            std::string tail = vals[vals.size() - 1];
            std::string key = vals[0];
            if (tail == "{") {vals.pop_back();}
            if (tail == "}") { flag = false; vals.pop_back();}
            size = vals.size();
            if (size == 2) {
                if (key == "endian") {
                    m_param.small_endian = (vals[1] == "small");
                } else if (key == "attach") {
                    m_param.attach = (vals[1] == "true");
                } else if (key == "package") {
                    parse_spliter(vals[1].c_str(), '.', m_param.packages);
                } else if (key == "import") {
                    // find .proto
                    size_t pos = vals[1].find(".proto");
                    if (pos == std::string::npos) {
                        printf("line[%d] import error\n", line);
                        iRet = false;
                        break;
                    }
                    m_param.import.push_back(vals[1].substr(0, pos));
                }
            }
            if (size >= 2) {
                if (key == "protocol") {
                    name = vals[1];
                    flag = true;
                }
            }
            if (size >= 3) {
                // type name id  (default/len/keys vals)
                param.type_name = key;
                // check type
                param.type = key_type(key);
                if (param.type == OBJECT_ARRAY) {
                    // find key
                    const char* phead = strstr(key.c_str(), "array<");
                    const char* ptail = strstr(key.c_str(), ">");
                    if (ptail == NULL) {
                        printf("line[%d] array has no end >\n", line);
                        iRet = false;
                        break;
                    } else {
                        param.key_type_name = std::string(key.c_str() + strlen("array<"), ptail - phead - strlen("array<"));
                        // key type
                        param.key_type = key_type(param.key_type_name);
                    }
                }
                // name
                param.name = vals[1];
                // id
                param.id = atoi(vals[2].c_str());
                if (param.id <= 0) {
                    printf("line[%d] param id error\n", line);
                    iRet = false;
                    break;
                }
                // check ids
                if (!ids.insert(param.id).second) {
                    printf("line[%d] id conflict\n", line);
                    iRet = false;
                    break;
                }
                // default len and protocol
                int diff = size - 3;
                if (0 == (diff%2)) {
                    for (int i = 0; i < diff; i += 2) {
                        if (vals[3 + i] == "default") {
                            param.has_default = true;
                            param.default_int = strtoull(vals[4+i].c_str(), NULL, 10);
                            param.default_str = vals[4+i];
                            param.default_float = strtod(vals[4+i].c_str(), NULL);
                        } else if (vals[3 + i] == "fixed") {
                            param.has_fixed = true;
                            param.default_int = strtoull(vals[4+i].c_str(), NULL, 10);
                            param.default_str = vals[4+i];
                            param.default_float = strtod(vals[4+i].c_str(), NULL);
                        } else if (vals[3 + i] == "afn") {
                            std::string col = vals[4+i];
                            if (cols.find(col) == cols.end()) {
                                printf("line[%d] afn col[%s] not exist\n", line, col.c_str());
                                iRet = false;
                                break;
                            }
                            param.protocol_key = col;
                            std::vector<KeyParam>::iterator iter = keys.begin();
                            for (; iter != keys.end(); ++iter) {
                                if (iter->name == col) {
                                    param.protocol_key_type = iter->type;
                                    break;
                                }
                            }
                        }
                        else if (vals[3 + i] == "len") {
                            int tmp = atoi(vals[4+i].c_str());
                            if (tmp > 0) {
                                param.len = tmp;
                            }
                            param.len_name = vals[4+i];
                        } else if (vals[3 + i] == "max") {
                            param.len = atoi(vals[4+i].c_str());
                            param.has_max = true;
                        } else {
                            param.child_protocols[vals[3 + i]] = vals[4+i];
                        }
                    }
                }
            } //end size > 3
            // add param
            if (param.id != 0) {
                if (param.type == OBJECT_ARRAY || param.type == OBJECT_BYTES || param.type == OBJECT_STRING) {
                    if (param.len == 0 && param.len_name.empty()) {
                        printf("line[%d] param has no len\n", line);
                        iRet = false;
                        break;
                    }
                }
                // array now allow protocol and bytes and string
                if (param.type == OBJECT_ARRAY) {
                    if (param.key_type == OBJECT_BITS || param.key_type == OBJECT_BYTES || param.key_type == OBJECT_STRING || param.key_type == OBJECT_PROTOCOL) {
                        printf("line[%d] array key type is not allowed, only number and message os allowed\n", line);
                        iRet = false;
                        break;
                    }
                }
                if (param.type == OBJECT_PROTOCOL) {
                    if (param.protocol_key.empty()) {
                        printf("line[%d] protocol has no afn\n", line);
                        iRet = false;
                        break;
                    }
                }
                keys.push_back(param);
                cols.insert(param.name);
            }
        } // end size > 0
        if (!flag) {
            if (keys.size() > 0) {
                //create protocol
                BProtocol* protocol = new BProtocol();
                if (!protocol->init(m_param.small_endian, m_param.attach, name, keys)) {
                    printf("create protocol[%s] error\n", name.c_str());
                    iRet = false;
                    break;
                }
                m_protocols[name] = protocol;
                keys.clear();
                ids.clear();
                cols.clear();
            }
        }
    } // while read
    fclose(fp);
    fp = NULL;
    return iRet;
}

BProtoParser::~BProtoParser() {
    std::map<std::string, BProtocol*>::iterator iter = m_protocols.begin();
    for (; iter != m_protocols.end(); ++iter) {
        delete iter->second;
    }
}

void BProtoParser::make_file(const char* path) {
    // header
    std::stringstream ss;
    ss << "#ifndef __PROTOCOL_" << upper(m_prefix) << "_H__\r\n";
    ss << "#define __PROTOCOL_" << upper(m_prefix) << "_H__\r\n";
    ss << "#include \"proto_cc.h\"\r\n";
    // import
    std::vector<std::string>::iterator it = m_param.import.begin();
    std::vector<std::string>::iterator it_end = m_param.import.end();
    for (; it != it_end; ++it) {
        ss << "#include \"" << *it << ".bpb.h\"\r\n";
    }
    ss << "#ifdef __cplusplus\r\n" << "extern \"C\" {\r\n";
    ss << "#else\r\n" << "#define const\r\n";
    ss << "#endif\r\n";
    ss << "// This file is generated by bprotoc. You can not edit it anyway\r\n";
    ss << "// Include the header file to project and then you can use it\r\n";
    std::map<std::string, BProtocol*>::iterator iter = m_protocols.begin();
    std::map<std::string, BProtocol*>::iterator end = m_protocols.end();
    for (; iter != end; ++iter) {
        ss << iter->second->make_struct();
    }
    // make header
    iter = m_protocols.begin();
    for (; iter != end; ++iter) {
        ss << iter->second->make_header();
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
    // c file
    ss.str("");
    ss << "#include \"" << m_prefix << ".bpb.h\"\r\n";
    iter = m_protocols.begin();
    for (; iter != end; ++iter) {
        ss << iter->second->make_cc();
    }
    // write to file
    std::string c_file = path + m_prefix + ".bpb.c";
    fp = fopen(c_file.c_str(), "w");
    if (fp == NULL) {
        printf("write c file[%s] error\n", c_file.c_str());
        return;
    }
    data = ss.str();
    fwrite(data.c_str(), data.length(), 1, fp);
    fclose(fp);
}



#include <stdio.h>
#include <getopt.h>
#include "proto_parser.h"


#if defined(__DATE__) && defined(__TIME__)
static const char SERVER_BUILT[] = __DATE__ " " __TIME__;
#else
static const char SERVER_BUILT[] = "unknown";
#endif

#ifdef VERSION
const char *VERSION_ID = VERSION;
#else
const char *VERSION_ID = "unknown";
#endif


static void print_version(void)
{                       
        printf(" Version\t:\t%s\n", VERSION_ID);
        printf(" Built date\t:\t%s\n", SERVER_BUILT);
}

void print_help(void)   
{               
        printf( "%s", "Usage: bprotoc [OPTION] PROTO_FILES \n");
        printf( "%s", "    --help    | -h       :   print help message\n");
        printf( "%s", "    --version | -v       :   print version\n");
        printf( "%s", "    --cc_out=OUT_DIR     :   generate c file\n");
        printf( "%s", "    --cpp_out=OUT_DIR    :   generate cpp file\n");
        printf( "%s", "    --js_out=OUT_DIR     :   generate js file\n");
        printf( "%s", "    --java_out=OUT_DIR   :   generate java file\n");
        printf( "%s", "    --proto=IN_DIR       :   input proto file\n");
} 

int main(int argc, char **argv)
{
    char c;
    const char* const short_options = "c:p:j:a:i:vh?"; 
    const struct option long_options[] = 
    {
         {"version", no_argument, NULL, 'v'},
         {"help", no_argument, NULL, 'h'},
         {"cc_out", optional_argument, NULL, 'c'},
         {"cpp_out", optional_argument, NULL, 'p'},
         {"js_out", optional_argument, NULL, 'j'},
         {"java_out", optional_argument, NULL, 'a'},
         {"proto", optional_argument, NULL, 'i'},
         {0, 0, 0, 0}
    };
    int type = -1;
    std::string path = "./";
    std::string file;
    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
    {
        switch (c)
        {
            case 'h':
                print_help();
                exit(1);
            case 'v':
                print_version();
                exit(1);
            case 'c':
                type = 0;
                path = optarg;
                break;
            case 'p':
                type = 1;
                path = optarg;
                break;
            case 'j':
                type = 2;
                path = optarg;
                break;
            case 'a':
                type = 3;
                path = optarg;
                break;
            case 'i':
                file = optarg;
                break;
            default:
                print_help();
                exit(1);
        }
    }
    if (type == -1) {
        printf("need input out type!!!\n");
        exit(1);
    }
    // generate files
    if (type == 0) {
        BProtoParser parser;
        if (!parser.init("", file.c_str())) {
            printf("Input file %s open fail\n", file.c_str());
            exit(1);
        }
        // make file
        parser.make_file(path.c_str());
        printf("Generate c file success!!!\n");
    } else if (type == 1) {
        BProtoParser parser;
        if (!parser.init("", file.c_str())) {
            printf("Input file %s open fail\n", file.c_str());
            exit(1);
        }
        // make file
        parser.make_cpp_file(path.c_str());
        printf("Generate cpp file success!!!\n");
    } else if (type == 2) {
        printf("js is not supported now!!!\n");
    } else if (type == 3) {
        printf("java is not supported now!!!\n");
    }
    return 0;
}

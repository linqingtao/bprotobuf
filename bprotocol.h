#ifndef __BPROTOCOL_H__
#define __BPROTOCOL_H__

class BMessage {
public:
    BMessage() {}
    virtual ~BMessage() {}
public:
    virtual int SerializeToArray(char* buffer, int len) = 0;
    virtual int ParseFromArray(char* buffer, int len) = 0;
    virtual int ByteSize() { return 0;}
};
#endif


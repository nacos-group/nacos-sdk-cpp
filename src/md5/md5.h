#ifndef MD5_H
#define MD5_H

#include <string>
#include <string.h>
#include <fstream>
#include "NacosString.h"

namespace nacos{
/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
    MD5();

    MD5(const void *input, size_t length);

    MD5(const NacosString &str);

    MD5(ifstream &in);

    void update(const void *input, size_t length);

    void update(const NacosString &str);

    void update(ifstream &in);

    const byte *digest();

    NacosString toString();

    void reset();

private:
    void update(const byte *input, size_t length);

    void final();

    void transform(const byte block[64]);

    void encode(const uint32 *input, byte *output, size_t length);

    void decode(const byte *input, uint32 *output, size_t length);

    NacosString bytesToHexString(const byte *input, size_t length);

    /* class uncopyable */
    MD5(const MD5 &);

    MD5 &operator=(const MD5 &);

private:
    uint32 _state[4]; /* state (ABCD) */
    uint32 _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
    byte _buffer[64]; /* input buffer */
    byte _digest[16]; /* message digest */
    bool _finished;   /* calculate finished ? */

    static const byte PADDING[64]; /* padding for calculate */
    static const char HEX[16];
    enum {
        BUFFER_SIZE = 1024
    };
};
}//namespace nacos

#endif
#include <iostream>
#include <string.h>
#include "DebugAssertion.h"
#include "src/crypto/MACProvider.h"

using namespace std;
using namespace nacos;

typedef struct _HMACSHA1TestCase {
    const char *key;
    size_t key_len;
    const char *data;
    size_t data_len;
    const char *digest;
} HMACSHA1TestCase;

HMACSHA1TestCase HMACSHA1TestCases[] = {
    {"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b", 20, "Hi There", 8, "\xb6\x17\x31\x86\x55\x05\x72\x64\xe2\x8b\xc0\xb6\xfb\x37\x8c\x8e\xf1\x46\xbe\x00"}
};

bool testHMACSHA1() {
    //Actually, I've no idea why Alibaba is still using this old-school MAC algorithm :(
    cout << "In function testHMACSHA1()" << endl;
    cout << "Getting MAC algorithm" << endl;
    cout << "Test case acquired at https://datatracker.ietf.org/doc/html/rfc2202" << endl;

    IMAC *digester = MACProvider::getMAC(MACProvider::HMAC_SHA1);
    SHOULD_BE_TRUE(digester != NULL, "Should be able to get a IMAC* from MACProvider");

    for (size_t i = 0; i < sizeof(HMACSHA1TestCases) / sizeof(HMACSHA1TestCase); i++) {
        const void *k = (const void*)HMACSHA1TestCases[i].key;
        size_t kl = HMACSHA1TestCases[i].key_len;

        const void *data = (const void*)HMACSHA1TestCases[i].data;
        size_t dl = HMACSHA1TestCases[i].data_len;

        char out[256] = {0};
        size_t out_l = sizeof(out);

        digester->getMac(k, kl, data, dl, out, &out_l);
        SHOULD_BE_TRUE(out_l == 20, "Digested data should be 20-bytes long");

        SHOULD_BE_TRUE(memcmp(out, HMACSHA1TestCases[i].digest, 20)  == 0, "Checking digested data with test vector");

        cout << "Test case " << i + 1 << ": out_l=" << out_l << " passed." <<endl;
    }

    return true;
}
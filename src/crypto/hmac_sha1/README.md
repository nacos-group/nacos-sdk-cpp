hmac-sha1
=========

[![Build Status](https://travis-ci.org/Akagi201/hmac-sha1.svg)](https://travis-ci.org/Akagi201/hmac-sha1)

Standalone implementation of `HMAC()` + `EVP_sha1()` in `OpenSSL`

## API

```
#include "hmac/hmac.h"

void hmac_sha1(const uint8_t *k,   /* secret key */
        size_t lk,  /* length of the key in bytes */
        const uint8_t *d,   /* data */
        size_t ld,  /* length of data in bytes */
        uint8_t *out, /* output buffer, at least "t" bytes */
        size_t *t);
```
#include "hash.h"
#include<stdio.h>
#include<openssl/sha.h>

void sha256(uint8_t *digest, const uint8_t *message, size_t len) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, message, len);
    SHA256_Final(digest, &ctx);
}

void hash256(uint8_t *digest, const uint8_t *message, size_t len) {
    uint8_t tmp[SHA256_DIGEST_LENGTH];
    sha256(tmp, message, len);
    sha256(digest, tmp, SHA256_DIGEST_LENGTH);
}

void printHash(uint8_t *hash) {
    for(int i = 0; i<32; i++) {
        printf("%02x", (hash[i]));
    }
    printf("\n");
}

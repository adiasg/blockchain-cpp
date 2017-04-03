#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stddef.h>

/**
	Computes SHA-256 hash of message and stores in digest.

	@param digest Array to store computed hash
	@param message Message to be hashed
	@param len Length of message
*/
void sha256(uint8_t *digest, const uint8_t *message, size_t len);

/**
	Computes double SHA-256 hash of message and stores in digest.

	@param digest Array to store computed hash
	@param message Message to be hashed
	@param len Length of message
*/
void hash256(uint8_t *digest, const uint8_t *message, size_t len);

/**
	Prints 256-bit hash.

	@param hash Array to stored hash
*/
void printHash(uint8_t *hash);

#endif

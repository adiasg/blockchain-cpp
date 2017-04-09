#ifndef BLOCK_H
#define BLOCK_H

#include "hash.h"
#include <openssl/rsa.h>

typedef struct block {		//1448-bit
	uint8_t difficulty;//block difficulty
	char src[8];		//identifier for source
	uint64_t ts; 		//timestamp
	uint32_t nonce;		//nonce
	uint8_t pl[128];	// payload
	uint8_t phash[32];	//previous hash
} block;

/**
	Compares two hashes.

	@param hash1 First hash
	@param hash2 Second hash
	@return true if distinct, false if identical
*/
bool hashcmp(uint8_t *hash1, uint8_t *hash2);

/**
	Generate genesis block. Memory should be allocated to passed block pointer.

	@param blk Pointer to a block
*/
void genesisblk(block* blk);

/**
	Prints the block.

	@param b Block to print
*/
void printblk(block blk);

/**
	Computes hash of the block and stores in bhash.

	@param bhash Array to store computed hash
	@param hash2 Second hash
*/
void hashblk(uint8_t* bhash, block* blk);

/**
	Generates block with given parameters. Memory should be allocated to passed newblk pointer.

	@param newblk Pointer to newly generated block
	@param prevblk Pointer to previous block in the chain
	@param msg Pointer to message
	@param len Length of message
	@param rsa_pri RSA structure initialized with private key
*/
void genblk(block* newblk, block* prevblk, char* msg, uint8_t len, RSA *rsa_pri);

/**
	Generates block with given parameters. Memory should be allocated to passed newblk pointer.

	@param newblk Pointer to newly generated block
	@param phash Pointer to hash of previous block in the chain
	@param msg Pointer to message
	@param len Length of message
	@param rsa_pri RSA structure initialized with private key
*/
void genblk_hash(block* newblk, uint8_t* phash, char* msg, uint8_t len, RSA *rsa_pri);

/**
	Verifies that difficulty is reflected in block hash.

	@param blk Pointer to block that is to be verified
	@return true if verified, false if block is faulty
*/
bool verifyHash(block *blk);

#endif

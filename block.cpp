#include "block.h"
#include "id.h"
#include "hash.h"
#include "crypto.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

char pub_pem_ext[] = "-pub.pem";

bool hashcmp(uint8_t *hash1, uint8_t *hash2) {  // returns true if hashes are different
    for(int i = 0; i<32; i++) {
        if( hash1[i]!=hash2[i] ) {
            return true;
        }
    }
    return false;
}

bool verifyHash(block* blk) {
    uint8_t blkhash[128];
    hashblk(blkhash, blk);
    for(int i = 0; i< blk->difficulty; i++) {
        if(blkhash[i]!=0x00) {
            return false;
        }
    }
    return true;
}

void genesisblk(block* blk) {   // generates genesis block
    //blk = (block*)malloc(sizeof(block));
	(*blk).difficulty = 0x00;
	(blk->src)[0] = 0x00;
	(*blk).ts = time(NULL);
	(*blk).nonce = 0x00;
	for(int i = 0; i<32; i++) {
		(*blk).phash[i] = 0x00;
		(*blk).pl[i] = 0x00;
	}
	for(int i = 31; i<128; i++) {
		(*blk).pl[i] = 0x00;
	}
}

void printblk(block blk) {  // prints details of the block
    printf("-------------------------------------------\n");
    printf("block difficulty: %u\n", blk.difficulty);
	printf("src: %s\n", blk.src);
	printf("timestamp: %lu\n", blk.ts);
	if(blk.src[0]!=0x00) {
        RSA *rsa_pub;
        char *pub_key_file = (char*) malloc(15);
        memcpy(pub_key_file, blk.src, strlen(blk.src));
        memcpy(pub_key_file+strlen(blk.src), pub_pem_ext, strlen(pub_pem_ext)+1);
        rsa_pub = createRSAWithFilename(pub_key_file, 1);
		unsigned char *dec = (unsigned  char*)malloc(RSA_size(rsa_pub));
	    int dec_len;
	    dec_len = RSA_public_decrypt(128, blk.pl, dec, rsa_pub, RSA_PKCS1_PADDING);
	    if(dec_len<0) {
	        printf("Error in RSA_public_decrypt()\n");
	    }
		printf("message: ");
	    for(int i = 0; i<dec_len; i++) {
			printf("%c", *(dec+i));
		}
		printf("\n");
		free(dec);
	}
	printf("nonce: %u\n", blk.nonce);
	printf("previous hash: ");
    for(int i=0;i<5;i++) {
        printf("%02x", blk.phash[i]);
    }
    printf("\n");
	printf("-------------------------------------------\n");
}

void hashblk(uint8_t* bhash, block* blk) { // returns hash of nonce, pl and phash
	uint8_t blkbitrep[164];
    memcpy(blkbitrep, &(blk->nonce), sizeof(blk->nonce));
	memcpy(blkbitrep+sizeof(blk->nonce), blk->pl, sizeof(blk->pl));
	memcpy(blkbitrep+sizeof(blk->nonce)+sizeof(blk->pl), blk->phash, sizeof(blk->phash));
	sha256(bhash, blkbitrep, sizeof(blkbitrep));
}

void genblk(block* newblk, block* prevblk, char* msg, uint8_t len, RSA *rsa_pri) { //generate new linked block
    //newblk = (block*)malloc(sizeof(block));
	newblk->difficulty = 0x01;
    newblk->nonce = 0x00;
	hashblk(newblk->phash, prevblk);
	newblk->ts = time(NULL);
	memcpy(newblk->src, host_id, 8);
	unsigned char *cip = (unsigned  char*)malloc(RSA_size(rsa_pri));
    if(RSA_private_encrypt(strlen(msg), (const unsigned  char*)msg, cip, rsa_pri, RSA_PKCS1_PADDING) != 128 ) {
		printf("Error in RSA_private_encrypt()\n");
	}
    memcpy(newblk->pl, cip, 128);
    while(!verifyHash(newblk)) {
        (newblk->nonce)++;
    }
}

void genblk_hash(block* newblk, uint8_t* phash, char* msg, uint8_t len, RSA *rsa_pri) { //generate new linked block with previous hash as phash
    //newblk = (block*)malloc(sizeof(block));
    newblk->difficulty = 0x01;
    newblk->nonce = 0x00;
    memcpy(newblk->phash, phash, 32);
	newblk->ts = time(NULL);
	memcpy(newblk->src, host_id, 8);
	unsigned char *cip = (unsigned  char*)malloc(RSA_size(rsa_pri));
    if(RSA_private_encrypt(strlen(msg), (const unsigned  char*)msg, cip, rsa_pri, RSA_PKCS1_PADDING) != 128 ) {
		printf("Error in RSA_private_encrypt()\n");
	}
    memcpy(newblk->pl, cip, 128);
    printf("Generate true block?\n");
    int x;
    scanf(" %d", &x);
    if(x!=0) {
        while(!verifyHash(newblk)) {
            (newblk->nonce)++;
        }
    }
}

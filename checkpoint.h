#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "hash.h"

#include "block.h"
#include "crypto.h"
#include "bdb.h"
#include "id.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/rsa.h>
//uint8_t * hardcodehash=0x00; //00000069e244f73d78e8fd29ba2fd2ed618bd6fa2ee92559f542fdb26e7c1d;

void checkpoint1(block* prevblk,DB * db,RSA *rsa_pri);

void genblk_hash1(block* newblk, uint8_t* phash, char* msg, uint8_t len, RSA *rsa_pri,uint8_t* comparehash);

#endif

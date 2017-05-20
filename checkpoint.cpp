

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

void genblk_hash1(block* newblk, block* prevblk, char* msg, uint8_t len, RSA *rsa_pri,uint8_t * comparehash) { //generate new linked block
    //newblk = (block*)malloc(sizeof(block));
	newblk->difficulty = 0x01;
    newblk->nonce = 0x00;
	hashblk(newblk->phash, prevblk);
    printf("3\n");
	newblk->ts = time(NULL);
	memcpy(newblk->src, host_id, 8);
    printf("3.5\n");
	unsigned char *cip = (unsigned  char*)malloc(RSA_size(rsa_pri));
    printf("3.55\n");
    if(RSA_private_encrypt(strlen(msg), (const unsigned  char*)msg, cip, rsa_pri, RSA_PKCS1_PADDING) != 128 ) {
		printf("Error in RSA_private_encrypt()\n");
	}
    printf("3.6\n");
    memcpy(newblk->pl, cip, 128);
    printf("3.7\n");
    while(1) {
        uint8_t blkhash[32];
        hashblk(blkhash, newblk);
        if(!hashcmp(comparehash,blkhash))
            break;
        (newblk->nonce)++;
    }
    printf("4\n");
}

void checkpoint1(block* prevblk,DB * db,RSA *rsa_pri)
{
    //uint8_t hardcodehash[128]=0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8;
    uint8_t hardcodehash[32];
    for(int i = 0; i<32; i++) {
		hardcodehash[i]=0x00;
	}
    int ret;
    ret = db_create(&db, NULL, 0);
    if(ret!=0) {
        printf("Error in db creation.\n");
    }
    ret = db->open(db,NULL,"chain.db", NULL, DB_HASH, DB_CREATE, 0);
    if(ret!=0) {
        printf("Error in db opening.\n");
    }
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    char msg[100];
    //scanf(" %s[^\n]", msg);//="Hi";
    block* newblk=(block *)malloc(sizeof(block));
    printf("2\n");
    //genblk_hash1(newblk, prevblk,msg, strlen(msg),rsa_pri,hardcodehash);
    genblk_hash1(newblk, prevblk,"Hi",2,rsa_pri,hardcodehash);
    printf("5\n");
    key.data = hardcodehash;
    key.size = 32;
    data.data = newblk;
    data.size = sizeof(block);
    if(db->put(db, NULL, &key, &data, DB_NOOVERWRITE)!=0) {
        printf("db->put() returns non-zero\n");
    }
    else {
        printf("Added block to local db\n");
    }
    printf("6\n");
}

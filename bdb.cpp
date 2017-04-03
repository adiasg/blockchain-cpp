#include "bdb.h"
#include "block.h"
#include "crypto.h"
#include "id.h"
#include <string.h>
#include <string>

using namespace std;

void printchain(uint8_t *tophash, DB *db) {
    printf("******************************\n");
    uint8_t tophash_temp[32];
    memcpy(tophash_temp, tophash, 32);

    RSA *rsa_pub;

    DBT key, data;
    memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
    uint8_t genesis_hash[32];
    memset(genesis_hash, 0x00, 32);
    block blk;

    while(hashcmp(tophash_temp, genesis_hash)) {
        key.data = tophash_temp;
    	key.size = 32;
    	data.data = &blk;
    	data.ulen = sizeof(block);
    	data.flags = DB_DBT_USERMEM;
        if(db->get(db, NULL, &key, &data, 0)==0) {
            printf("-------------------------------------------\n");
            printf("blk.src: %s\n", blk.src);
            printblk(blk);
            memcpy(tophash_temp, blk.phash, 32);
        }
        else {
            printf("db->get() returns non-zero\n");
            break;
        }
    }
    printf("******************************\n");

}

uint32_t sumOfDiff(uint8_t *tophash, DB *db) {
    uint8_t tophash_temp[32];
    memcpy(tophash_temp, tophash, 32);

    DBT key, data;
    memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
    uint8_t genesis_hash[32];
    memset(genesis_hash, 0x00, 32);
    block blk;

    uint32_t sumofdiff = 0x00;

    printf("tophash_temp: ");
    for(int i = 0; i<32; i++) {
        printf("%02x", (tophash_temp[i]));
    }
    printf("\n");
    printf("********************************************\n");
    while(hashcmp(tophash_temp, genesis_hash)) {
        key.data = tophash_temp;
    	key.size = 32;
    	data.data = &blk;
    	data.ulen = sizeof(block);
    	data.flags = DB_DBT_USERMEM;
        if(db->get(db, NULL, &key, &data, 0)==0) {
            //printf("-------------------------------------------\n");
            printf("At block with hash:\t");
            for(int i=0;i<5;i++) {
                printf("%02x", tophash_temp[i]);
            }
            printf("\n");
            sumofdiff++;
            printf("Sum of Difficulties:\t%04x\n", sumofdiff);
            memcpy(tophash_temp, blk.phash, 32);
        }
        else {
            printf("db->get() returns non-zero\n");
            break;
        }
    }
    printf("********************************************\n");
    return sumofdiff;
}

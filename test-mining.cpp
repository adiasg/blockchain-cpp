#include "hash.h"
#include "block.h"
#include "crypto.h"
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <string.h>

using namespace std;
using namespace std::chrono;

int main() {
    char host_id[] = "a";
    RSA *rsa_pri;
    char pri_pem_ext[] = "-pri.pem";
    char *pri_key_file = (char*)malloc(16);
    memcpy(pri_key_file, &host_id, strlen(host_id));
    memcpy(pri_key_file+strlen(host_id), pri_pem_ext, strlen(pri_pem_ext));
    pri_key_file[strlen(host_id)+strlen(pri_pem_ext)] = '\0';
    rsa_pri = createRSAWithFilename(pri_key_file, 0);

    block *blk, *top;
    blk = (block*) malloc(sizeof(block));
    top = (block*) malloc(sizeof(block));
    genesisblk(blk);
    printblk(*blk);
    char msg[] = "this is a test message";
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    genblk(top, blk, msg, strlen(msg), rsa_pri);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    printblk(*top);
    uint8_t hash[128];
    hashblk(hash, top);
    printHash(hash);
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "duration: " << duration/1000000.0 << endl;
    return 0;
}

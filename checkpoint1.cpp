#include "block.h"
#include "crypto.h"
#include "bdb.h"
#include "id.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main()
{
    DB *db1;
    int ret1;
    ret1 = db_create(&db1, NULL, 0);
    if(ret1!=0) {
        printf("Error in db creation.\n");
    }
    ret1 = db1->open(db1,NULL,"checkpoint.db", NULL, DB_HASH, DB_CREATE, 0);
    if(ret1!=0) {
        printf("Error in db opening.\n");
    }
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    uint8_t hardcodehash[32];
    for(int i = 0; i<32; i++) {
        hardcodehash[i]=0x00;
    }
    int x=5;
    data.data = hardcodehash;
    data.size = 32;
    key.data = &x;
    key.size = sizeof(int);
    if(db1->put(db1, NULL, &key, &data, DB_NOOVERWRITE)!=0) {
        printf("db->put() returns non-zero\n");
    }
    else {
        printf("Added block to local db\n");
    }
    return 0;
}

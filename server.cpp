#include "block.h"
#include "crypto.h"
#include "bdb.h"
#include "id.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {
        char pub_pem_ext[] = "-pub.pem";
        RSA *rsa_pub;
        RSA *rsa_pri;

        DB *db;
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

        int sockfd, newsockfd, portNo;
        unsigned int clilen;


        struct sockaddr_in serv_addr, cli_addr;

        if(argc < 2){
      		printf("Port not specified\n");
      		return -1;
      	}

      	sockfd = socket(AF_INET, SOCK_STREAM, 0);

      	if(sockfd < 0){
      		printf("Socket not opened\n");
      		return -1;
      	}

      	bzero((char *) &serv_addr, sizeof(serv_addr));
      	portNo = atoi(argv[1]);
      	serv_addr.sin_family = AF_INET;
      	serv_addr.sin_port = htons(portNo);
      	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("127.0.0.1");
              if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      		printf("ERROR on binding\n");
      	listen(sockfd, 5);
      	clilen = (unsigned int)sizeof(cli_addr);

        uint8_t tophash[32], tophash_rec[32], tophash_temp[32];
        block a;
        FILE *fp;
        uint8_t req_type;
        uint32_t maxSumOfDiff = 0x00, sumdiff_temp = 0x00;

        printf("Generating genesis block\n");
        genesisblk(&a);
        printf("Generated genesis block\n");
        printblk(a);
        hashblk(tophash, &a);
        fp = fopen("tophash.txt", "w");
        for(int i = 0; i<32; i++) {
            fprintf(fp, "%02x ", (tophash[i]));
        }
        printf("Written to tophash.txt\n");
        fclose(fp);
        printf("Adding block to local db\n");
        key.data = tophash;
        key.size = 32;
        data.data = &a;
        data.size = sizeof(block);
        if(db->put(db, NULL, &key, &data, DB_NOOVERWRITE)!=0) {
            printf("db->put() returns non-zero\n");
        }
        else {
            printf("Added block to local db\n");
        }


        while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
            fp = fopen("tophash.txt", "r");
            for(int i = 0; i<32; i++) {
                fscanf(fp, " %hhx", (tophash+i));
            }
            fclose(fp);

            printf("tophash updated from file \n");
            printf("tophash: ");
            for(int i = 0; i<32; i++) {
                printf("%02x", (tophash[i]));
            }
            printf("\n");

            read(newsockfd, tophash_rec,sizeof(tophash_rec));

            printf("Recieved tophash_rec\ntophash_rec: ");
            for(int i = 0; i<32; i++) {
                printf("%02x", (tophash_rec[i]));
            }
            printf("\n");

            if(hashcmp(tophash_rec,tophash)) {
                printf("tophash_rec not identical to tophash\nSending block request\n");
                req_type = 0x01;
                printf("Sending req_type 0x01 to client\n");
                write(newsockfd, &req_type, sizeof(req_type));
                read(newsockfd, &a,sizeof(block));
                if(verifyHash(&a)) {
                    printf("Block hash verified\n");
                    if((a.src)[0]==0x00) {
                        printblk(a);
                        hashblk(tophash_temp, &a);
                    }
                    else {
                        printblk(a);
                        hashblk(tophash_temp, &a);
                    }
                    printf("Adding block to local db\n");
                    key.data = tophash_temp;
                    key.size = 32;
                    data.data = &a;
                    data.size = sizeof(block);
                    if(db->put(db, NULL, &key, &data, DB_NOOVERWRITE)!=0) {
                        printf("db->put() returns non-zero\n");
                    }
                    else {
                        printf("Added block to local db\n");
                    }

                    sumdiff_temp = sumOfDiff(tophash_temp, db);
                    printf("sumdiff_temp: %04x\n", sumdiff_temp);
                    if(sumdiff_temp > maxSumOfDiff) {
                        maxSumOfDiff = sumdiff_temp;
                        printf("maxSumOfDiff: %04x\n", maxSumOfDiff);
                        memcpy(tophash, tophash_temp, 32);
                        printf("tophash updated\n");
                        printf("tophash: ");
                        for(int i = 0; i<32; i++) {
                            printf("%02x", (tophash[i]));
                        }
                        printf("\n");

                        fp = fopen("tophash.txt", "w");
                        for(int i = 0; i<32; i++) {
                            fprintf(fp, "%02x ", (tophash[i]));
                        }
                        printf("Written to tophash.txt\n");
                        fclose(fp);
                    }
                    else {
                        printf("maxSumOfDiff: %04x\n", maxSumOfDiff);
                        printf("tophash not updated\n");
                        printf("tophash: ");
                        for(int i = 0; i<32; i++) {
                            printf("%02x", (tophash[i]));
                        }
                        printf("\n");
                    }

                }
                else {
                    printf("Block rejected!\n");
                }
            }
            else {
                req_type = 0x02;
                printf("tophash_rec identical to tophash\n");
                printf("Sending req_type 0x02 to client\n");
                write(newsockfd, &req_type, sizeof(req_type));
            }

            close(newsockfd);
        }

        if (newsockfd < 0) {
      		printf("Error accepting\n");
      		return -1;
      	}

        close(sockfd);

        printchain(tophash, db);

        return 0;
}

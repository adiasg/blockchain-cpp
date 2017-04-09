#include "block.h"
#include "crypto.h"
#include "id.h"
#include "bdb.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

void read_ip_addr(int &no_conn, char** &ip_addr, int* &portno) {
    FILE *fp;
    fp = fopen("ip_addr.txt", "r");
    fscanf(fp, "%d", &no_conn);
    ip_addr = (char**)malloc(no_conn*sizeof(char*));
    portno = (int*)malloc(no_conn*sizeof(int));

    for(int i = 0; i<no_conn; i++) {
        ip_addr[i] = (char*)malloc(8*sizeof(char));
        fscanf(fp, "%s %d", ip_addr[i], &portno[i]);
        //printf("ip_addr[%d]: %s portno[%d]: %d\n", i, ip_addr[i], i, portno[i]);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{

    int no_conn;
    int *sockFD, *portno, n, status;
    char **ip_addr;
    read_ip_addr(no_conn, ip_addr, portno);
    sockFD = (int*)malloc(no_conn*sizeof(int));
    for(int i = 0; i<no_conn; i++) {
        printf("ip_addr[%d]: %s portno[%d]: %d\n", i, ip_addr[i], i, portno[i]);
    }

    set_host_id();
    RSA *rsa_pri;
    char pri_pem_ext[] = "-pri.pem";
    char *pri_key_file = (char*)malloc(16);
    memcpy(pri_key_file, &host_id, strlen(host_id));
    memcpy(pri_key_file+strlen(host_id), pri_pem_ext, strlen(pri_pem_ext));
    pri_key_file[strlen(host_id)+strlen(pri_pem_ext)] = '\0';
    rsa_pri = createRSAWithFilename(pri_key_file, 0);

    uint8_t blkhash[32], tophash[32], tophash_ret[32];
    block *top;
    top = (block*)malloc(sizeof(block));
    char exit_str[] = "exit", gen_blk_str[] = "genesis", msg[100];
    uint8_t req_type;

    printf("Enter message/control string:\n");
    scanf(" %s[^\n]", msg);
    //fgets(msg, sizeof(msg), stdin);

    FILE *fp;

    while(strcmp(msg, exit_str)!=0) {
        printf("Generating block\n");

        fp = fopen("tophash.txt", "r");
        for(int i = 0; i<32; i++) {
            fscanf(fp, " %hhx", (tophash+i));
        }
        fclose(fp);

        printf("tophash updated from file \n");
        printf("tophash: ");
        printHash(tophash);
    	genblk_hash(top, tophash, msg, strlen(msg), rsa_pri);
        printf("Generated block\n");
        hashblk(blkhash, top);
        printf("blkhash: ");
    	printHash(blkhash);
        memcpy(tophash, blkhash, 32);
        printblk(*top);

        struct sockaddr_in serv_addr;
        struct hostent *server;
        for(int i = 0; i < no_conn; i++) {
            printf("Send to socket %d?\n", i);
            int x;
            scanf(" %d", &x);
            if(x!=0) {
                sockFD[i] = socket(AF_INET, SOCK_STREAM, 0);
                if (sockFD[i] < 0){
                    printf("Error opening socket %d\n", i);
                }
                server = gethostbyname(ip_addr[i]);
                if (server == NULL) {
                    printf("ERROR, no such host %d\n", i);
                }
                bzero((char *) &serv_addr, sizeof(serv_addr));
                serv_addr.sin_family = AF_INET;
                bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
                serv_addr.sin_port = htons(portno[i]);
                if (connect(sockFD[i],(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
                printf("ERROR connecting to %d\n", i);
                }
                else {
                    printf("Connected to socket %d\n", i);
                }

                printf("Sending tophash\n");
                n = write(sockFD[i],tophash,sizeof(tophash));
                if(n<0) {
                    printf("Error wrting to socket %d\n", i);
                }
                else {
                    printf("tophash written to socket %d\n", i);
                }
                printf("Waiting to read req_type from server\n");
                // debugging, to check that req_type actually changes
                req_type = 0x32;
                read(sockFD[i], &req_type, sizeof(req_type));
                printf("req_type: %02x\n", req_type);
                if(req_type==0x01) {
                    printf("req_type 0x01 from socket %d\n", i);
                    printf("Sending block\n");
                    n = write(sockFD[i],top,sizeof(block));
                    if(n<0) {
                        printf("Error wrting to socket %d\n", i);
                    }
                    else {
                        printf("Written block to socket %d\n", i);
                    }
                }
                else if(req_type==0x02){
                    printf("req_type 0x00 from socket %d\n", i);
                }
                else {
                    printf("Invalid req_type recieved.\n");
                }
            }
        }

        for(int i = 0; i < no_conn; i++) {
            status = close(sockFD[i]);
        }
        printf("Connections closed\n");

        printf("Enter message/control string:\n");
        scanf(" %s", msg);
    }

    return 0;
}

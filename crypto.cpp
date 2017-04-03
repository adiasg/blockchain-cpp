#include "crypto.h"
#include <openssl/pem.h>
using namespace std;

RSA *createRSAWithFilename(string filename,int public_1) { // create RSA structure from given file
    //public_1==1 for read public key
    FILE *fp = fopen(filename.c_str(),"r");
    if(fp == NULL)
    {
        printf("Unable to open file %s \n",filename.c_str());
        return NULL;
    }

    RSA *rsa = NULL;

    if(public_1) {
        rsa = PEM_read_RSAPublicKey(fp, NULL,NULL, NULL);
        if(rsa==NULL) {
            printf("rsa==NULL\n");
        }
        BIO *pub = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pub, rsa);
        size_t pub_len = BIO_pending(pub);
        char *pub_key = (char*)malloc(pub_len + 1);
        BIO_read(pub, pub_key, pub_len);
        pub_key[pub_len] = '\0';
        //printf("Read public key as:\n%s\n", pub_key);
        BIO_free_all(pub);
    }
    else {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
        BIO *pri = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(pri, rsa, NULL, NULL, 0, NULL, NULL);
        size_t pri_len = BIO_pending(pri);
        char *pri_key = (char*)malloc(pri_len + 1);
        BIO_read(pri, pri_key, pri_len);
        //printf("Read private key as:\n%s\n", pri_key);
        BIO_free_all(pri);
    }

    return rsa;
}

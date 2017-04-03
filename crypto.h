#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/rsa.h>
#include <string>

/**
	Generates RSA structure from file.

	@param filename Name of file containing the key
	@param publ Set as 1 for public key, 0 for private key
    @return RSA structure corresponding to the key
*/
RSA *createRSAWithFilename(std::string filename, int publ);

#endif

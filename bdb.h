#ifndef BDB_H
#define BDB_H

#include <db.h>
#include <string>

/**
	Prints chain till genesis block.

	@param tophash Hash to top of the chain
	@param db DB structure of database
*/
void printchain(uint8_t *tophash, DB *db);

/**
	Computes sum of difficulties in the chain.

	@param tophash Hash to top of the chain
	@param db DB structure of database
    @return Sum of difficulties in the chain
*/
uint32_t sumOfDiff(uint8_t *tophash, DB *db);

#endif

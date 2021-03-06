#include "cache.h"
#include "util.h"

/* cache.c : Implement your functions declared in cache.h */


/***************************************************************/
/*                                                             */
/* Procedure: setupCache                  		       */
/*                                                             */
/* Purpose: Allocates memory for your cache                    */
/*                                                             */
/***************************************************************/

void setupCache(int capacity, int num_way, int block_size)
{
/*	code for initializing and setting up your cache	*/
/*	You may add additional code if you need to	*/
	
	int i,j,k; //counter
	int nset=0; // number of sets
	int _wpb=0; //words per block   
	nset=capacity/(block_size*num_way);
	_wpb = block_size/BYTES_PER_WORD;

	Cache = (uint32_t  ***)malloc(nset*sizeof(uint32_t **));
	Cache_info = (uint32_t ***)malloc(nset*sizeof(uint32_t **));
	
	for (i=0;i<nset;i++) {
		Cache[i] = (uint32_t ** )malloc(num_way*sizeof(uint32_t*));
		Cache_info[i] = (uint32_t **)malloc(num_way*sizeof(uint32_t*));
	}
	for (i=0; i<nset; i++){	
		for (j=0; j<num_way; j++){
			Cache[i][j]=(uint32_t*)malloc(sizeof(uint32_t)*(_wpb));
			Cache_info[i][j]=(uint32_t*)malloc(sizeof(uint32_t)*4);
		}
	}
	for (i = 0; i < nset; i++) {
		for (j = 0; j < num_way; j++) {
			for (k = 0; k < 4; k++) {
				Cache_info[i][j][k] = 0;
			}
		}		
	}

}


/***************************************************************/
/*                                                             */
/* Procedure: setCacheMissPenalty                  	       */
/*                                                             */
/* Purpose: Sets how many cycles your pipline will stall       */
/*                                                             */
/***************************************************************/

void setCacheMissPenalty(int penalty_cycles)
{
/*	code for setting up miss penaly			*/
/*	You may add additional code if you need to	*/	
	//miss_penalty = penalty_cycles;
	
//ff
	miss_penalty = 0;

}

/* Please declare and implement additional functions for your cache */

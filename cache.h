#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* cache.h : Declare functions and data necessary for your project*/

int miss_penalty; // number of cycles to stall when a cache miss occurs
uint32_t ***Cache; // data cache storing data [set][way][byte]
uint32_t ***Cache_info; // same as Cache, 0th = valid, 1st = tag, 2nd = order, 3rd = dirtybit

void setupCache(int, int, int);
void setCacheMissPenalty(int);

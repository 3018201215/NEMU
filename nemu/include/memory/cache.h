#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

#define Cache_block_size 64
#define Cache_size 64 * 1024
#define Cache_way 3
#define Cache_block 6
#define Cache_group 7
#define Cache_group_size (1 << Cache_group)
#define Cache_way_size (1 << Cache_way)

#define Cache2_block_size 64
#define Cache2_size 4 * 1024 * 1024
#define Cache2_way 4
#define Cache2_block 6
#define Cache2_group 12
#define Cache2_group_size (1 << Cache_group)
#define Cache2_way_size (1 << Cache_way)


typedef struct CacheBlock{
	uint8_t data[Cache_block_size];
	uint32_t tag;
	bool valid;
}CacheBlock;

typedef struct CacheBlock2{
	uint8_t data[Cache2_block_size];
	uint32_t tag;
	bool valid, dirty;
}CacheBlock2;

CacheBlock cache[Cache_size / Cache_block_size];
CacheBlock2 cache2[Cache2_size / Cache2_block_size];

void init_cache();
int read_cache(hwaddr_t);
void write_cache(hwaddr_t, size_t, uint32_t);
uint32_t read_cache2(hwaddr_t);
void write_cache2(hwaddr_t, size_t, uint32_t);

#endif


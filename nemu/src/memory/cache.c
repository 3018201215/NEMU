#include "common.h"
#include "memory/cache.h"
#include <stdio.h>
#include <stdlib.h>
#include "burst.h"

void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t dram_read(hwaddr_t, size_t);
void ddr3_read_public(hwaddr_t, void* );
void ddr3_write_public(hwaddr_t, void* , uint8_t* );

void init_cache(){
	int i;
	for(i = 0; i < Cache_size / Cache_block_size; i ++){
		cache[i].valid = false;
	}
	for(i = 0; i < Cache2_size / Cache_block_size; i ++){
		cache2[i].valid = false;
	}
}

uint32_t read_cache(hwaddr_t addr){
	uint32_t group_index = (addr >> Cache_block) & (Cache_group_size - 1);
	uint32_t tag_index = addr >> (Cache_block + Cache_group);
	// uint32_t block = (addr >> Cache2_block) << Cache2_block;

	int i, group = group_index * Cache_way_size;
	for(i = group; i < group + Cache_way_size; i ++){
		if(cache[i].valid == 1 && cache[i].tag == tag_index)
			return i;
	}
	srand(i);
	i = group + rand() % Cache_way_size;
	int j = read_cache2(addr);
	cache[i].valid = 1;
	cache[i].tag = tag_index;
	memcpy(cache[i].data, cache2[j].data, Cache_block_size);
	return i;
}

uint32_t read_cache2(hwaddr_t addr){
	uint32_t group_index = (addr >> Cache2_block) & (Cache2_group_size - 1);
	uint32_t tag_index = addr >> (Cache2_block + Cache2_group);
	uint32_t block = (addr >> Cache2_block) << Cache2_block;

	int i, group = group_index * Cache2_way_size;
	for(i = group; i < group + Cache2_way_size; i ++){
		if(cache2[i].valid == 1 && cache2[i].tag == tag_index){
			return i;
		}
	}
	int j;
	srand(0);
	i = group + rand() % Cache2_way_size;
	if(cache2[i].dirty == 1 && cache2[i].valid == 1){
		uint8_t mask[BURST_LEN * 2];
		uint32_t block2 = (cache2[i].tag << (Cache2_block + Cache2_group)) | (group_index << Cache2_block);
		memset(mask, 1, sizeof(mask));
		for(j = 0; j < Cache_block_size / BURST_LEN; j ++)
			ddr3_write_public(block2+j*BURST_LEN, cache2[i].data+j*BURST_LEN, mask);
	}
	for(j = 0; j < Cache_block_size / BURST_LEN; j ++)
		ddr3_read_public(block+j*BURST_LEN, cache2[i].data+j*BURST_LEN);
	cache2[i].valid = 1;
	cache2[i].tag = tag_index;
	cache2[i].dirty = 0;
	return i;
}

void write_cache(hwaddr_t addr, size_t size, uint32_t data){
	uint32_t group_index = (addr >> Cache_block) & (Cache_group_size - 1);
	uint32_t tag_index = addr >> (Cache_block + Cache_group);
	uint32_t offset = addr & (Cache_block_size - 1);

	int i, group = group_index * Cache_way_size;
	for(i = group; i < group + Cache_way_size; i ++){
		if(cache[i].valid == 1 && cache[i].tag == tag_index){
			if(offset + size > Cache_block_size){
				memcpy(cache[i].data+offset, &data, Cache_block_size-offset);
				write_cache2(addr, Cache_block_size-offset, data);
				write_cache(addr+Cache_block_size-offset, size-(Cache_block_size-offset), data>>(Cache_block_size-offset));
			}else{
				memcpy(cache[i].data+offset, &data, size);
				write_cache2(addr, size, data);
			}
			return ;
		}
	}
	write_cache2(addr, size, data);
}

void write_cache2(hwaddr_t addr, size_t size, uint32_t data){
	uint32_t group_index = (addr >> Cache2_block) & (Cache2_group_size - 1);
	uint32_t tag_index = addr >> (Cache2_block + Cache2_group);
	uint32_t offset = addr & (Cache_block_size - 1);

	int i, group = group_index * Cache2_way_size;
	for(i = group; i < group + Cache2_way_size; i ++){
		if(cache2[i].valid == 1 && cache2[i].tag == tag_index){
			cache2[i].dirty = 1;
			if(offset + size > Cache_block_size){
				memcpy(cache2[i].data+offset, &data, Cache_block_size-offset);
				write_cache2(addr+Cache_block_size-offset, size-(Cache_block_size-offset), data>>(Cache_block_size-offset));
			}else{
				memcpy(cache2[i].data+offset, &data, size);
			}
			return ;
		}
	}
	int j = read_cache2(addr);
	cache2[j].dirty = 1;
	memcpy(cache2[j].data + offset, &data, size);
}
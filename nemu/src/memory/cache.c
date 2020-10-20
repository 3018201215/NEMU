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

uint32_t read_cache(hwaddr_t addr) {
	uint32_t tag = addr>>(Cache_group+Cache_block);
	uint32_t set = addr>>(Cache_block);
	set &=(Cache_group_size-1);
	//uint32_t block = (addr>>Cache_block)<<Cache_block;

	int i;
	for(i=set*Cache_way_size;i<(set+1)*Cache_way_size;i++) {
		if(cache[i].valid&&cache[i].tag==tag) {
			return i;
		}
	}
	// hit miss
	int j = read_cache2(addr);
	srand(i);
	i = Cache_way_size *set + rand()%Cache_way_size;
	memcpy(cache[i].data,cache2[j].data,Cache_block_size);
	
	/*	
	srand(i);
	i = Cache_way_size *set + rand()%Cache_way_size;
	int j;
	for(j=0;j<Cache_block_size/BURST_LEN;j++) {
		ddr3read(block+j*BURST_LEN,cache[i].data+j*BURST_LEN);	
	}
	*/
	cache[i].valid = true;
	cache[i].tag = tag;
	return i;
}

uint32_t read_cache2(hwaddr_t addr) {
	uint32_t tag = addr>>(Cache2_group+Cache2_block);
	uint32_t set = addr>>(Cache2_block);
	set &=(Cache2_group_size-1);
	uint32_t block = (addr>>Cache2_block)<<Cache2_block;

	int i;
	for(i=set*Cache2_way_size;i<(set+1)*Cache2_way_size;i++) {
		if(cache2[i].valid&&cache2[i].tag==tag) {
			return i;
		}
	}
	// hit miss	
	srand(i);
	i = Cache2_way_size *set + rand()%Cache2_way_size;
	if(cache2[i].dirty&&cache2[i].valid) {
		uint32_t addr2 = (cache2[i].tag<<(Cache2_group+Cache2_block)) | (set<<(Cache2_block));
		uint8_t mask[2*BURST_LEN];
		memset(mask,1,2*BURST_LEN);
		int j;
		for(j=0;j<Cache_block_size/BURST_LEN;j++) {
			ddr3_write_public(addr2+j*BURST_LEN,cache2[i].data+j*BURST_LEN,mask);		
		}
	}
	int j;
	for(j=0;j<Cache_block_size/BURST_LEN;j++) {
		ddr3_read_public(block+j*BURST_LEN,cache2[i].data+j*BURST_LEN);	
	}
	
	cache2[i].valid = true;
	cache2[i].tag = tag;
	cache2[i].dirty = false;
	
	return i;
}

void write_cache(hwaddr_t addr,size_t len,uint32_t data) {
	uint32_t tag = addr>>(Cache_group+Cache_block);
	uint32_t set = addr>>(Cache_block);
	set &=(Cache_group_size-1);
	uint32_t offset = addr&(Cache_block_size-1);
	
	int i;
	for(i=set*Cache_way_size;i<(set+1)*Cache_way_size;i++) {
		if(cache[i].valid&&cache[i].tag==tag) {
			if(offset+len>Cache_block_size) {
				//dram_write(addr,Cache_block_size-offset,data); //write through
				memcpy(cache[i].data+offset,&data,Cache_block_size-offset);
				write_cache2(addr,Cache_block_size-offset,data);
				write_cache(addr+Cache_block_size-offset,len-(Cache_block_size-offset),data>>(Cache_block_size-offset));
			}	
			else {	
				//dram_write(addr,len,data); //write through 
				memcpy(cache[i].data+offset,&data,len);	
				write_cache2(addr,len,data);		
			}
			return ;
		}
	}
	
	write_cache2(addr,len,data);	
}

void write_cache2(hwaddr_t addr,size_t len,uint32_t data) {
	uint32_t tag = addr>>(Cache2_group+Cache2_block);
	uint32_t set = addr>>(Cache2_block);
	set &=(Cache2_group_size-1);
	uint32_t offset = addr&(Cache2_block_size-1);
	
	int i;
	for(i=set*Cache2_way_size;i<(set+1)*Cache2_way_size;i++) {
		if(cache2[i].valid&&cache2[i].tag==tag) {
			cache2[i].dirty = true;
			if(offset+len>Cache2_block_size) {
				//dram_write(addr,Cache_block_size-offset,data);
				memcpy(cache2[i].data+offset,&data,Cache2_block_size-offset);
				write_cache2(addr+Cache2_block_size-offset,len-(Cache2_block_size-offset),data>>(Cache2_block_size-offset));
			}	
			else {	
				//dram_write(addr,len,data);
				memcpy(cache2[i].data+offset,&data,len);			
			}
			return ;
		}
	}
	
	int j = read_cache2(addr);
	cache2[j].dirty = true;
	memcpy(cache2[j].data+offset,&data,len);
	//dram_write(addr,len,data);	
}

// int read_cache2(hwaddr_t addr){
// 	uint32_t group_index = (addr >> Cache2_block) & (Cache2_group_size - 1);
// 	uint32_t tag_index = addr >> (Cache2_block + Cache2_group);
// 	uint32_t block = (addr >> Cache2_block) << Cache2_block;

// 	int i, group = group_index * Cache2_way_size;
// 	for(i = group; i < group + Cache2_way_size; i ++){
// 		if(cache2[i].valid == 1 && cache2[i].tag == tag_index){
// 			return i;
// 		}
// 	}
// 	int j;
// 	srand(0);
// 	i = group + rand() % Cache2_way_size;
// 	if(cache2[i].dirty == 1 && cache2[i].valid == 1){
// 		uint8_t mask[BURST_LEN * 2];
// 		uint32_t block2 = (cache2[i].tag << (Cache2_block + Cache2_group)) | (group_index << Cache2_block);
// 		memset(mask, 1, sizeof(mask));
// 		for(j = 0; j < Cache_block_size / BURST_LEN; j ++)
// 			ddr3_write_public(block2+j*BURST_LEN, cache2[i].data+j*BURST_LEN, mask);
// 	}
// 	for(j = 0; j < Cache_block_size / BURST_LEN; j ++)
// 		ddr3_read_public(block+j*BURST_LEN, cache2[i].data+j*BURST_LEN);
// 	cache2[i].valid = 1;
// 	cache2[i].tag = tag_index;
// 	cache2[i].dirty = 0;
// 	return i;
// }

// void write_cache(hwaddr_t addr, size_t size, uint32_t data){
// 	uint32_t group_index = (addr >> Cache_block) & (Cache_group_size - 1);
// 	uint32_t tag_index = addr >> (Cache_block + Cache_group);
// 	uint32_t offset = addr & (Cache_block_size - 1);

// 	int i, group = group_index * Cache_way_size;
// 	for(i = group; i < group + Cache_way_size; i ++){
// 		if(cache[i].valid == 1 && cache[i].tag == tag_index){
// 			if(offset + size > Cache_block_size){
// 				memcpy(cache[i].data+offset, &data, Cache_block_size-offset);
// 				write_cache2(addr, Cache_block_size-offset, data);
// 				write_cache(addr+Cache_block_size-offset, size-(Cache_block_size-offset), data>>(Cache_block_size-offset));
// 			}else{
// 				memcpy(cache[i].data+offset, &data, size);
// 				write_cache2(addr, size, data);
// 			}
// 			return ;
// 		}
// 	}
// 	write_cache2(addr, size, data);
// }

// void write_cache2(hwaddr_t addr, size_t size, uint32_t data){
// 	uint32_t group_index = (addr >> Cache2_block) & (Cache2_group_size - 1);
// 	uint32_t tag_index = addr >> (Cache2_block + Cache2_group);
// 	uint32_t offset = addr & (Cache_block_size - 1);

// 	int i, group = group_index * Cache2_way_size;
// 	for(i = group; i < group + Cache2_way_size; i ++){
// 		if(cache2[i].valid == 1 && cache2[i].tag == tag_index){
// 			cache2[i].dirty = 1;
// 			if(offset + size > Cache_block_size){
// 				memcpy(cache2[i].data+offset, &data, Cache_block_size-offset);
// 				write_cache2(addr+Cache_block_size-offset, size-(Cache_block_size-offset), data>>(Cache_block_size-offset));
// 			}else{
// 				memcpy(cache2[i].data+offset, &data, size);
// 			}
// 			return ;
// 		}
// 	}
// 	int j = read_cache2(addr);
// 	cache2[j].dirty = 1;
// 	memcpy(cache2[j].data + offset, &data, size);
// }
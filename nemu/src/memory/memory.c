#include "common.h"
#include "burst.h"
#include "memory/cache.h"
#include "nemu.h"
#include <stdlib.h>
#include "cpu/reg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int id = read_cache(addr);
	uint32_t offset = addr&(Cache_block_size-1);
	uint8_t tmp[2*BURST_LEN];
	if(offset+len>Cache_block_size) {
		int id2 = read_cache(addr+Cache_block_size-offset);	
		memcpy(tmp,cache[id].data+offset,Cache_block_size-offset);
		memcpy(tmp+Cache_block_size-offset,cache[id2].data,len-(Cache_block_size-offset));
	}
	else {
		memcpy(tmp,cache[id].data+offset,len);
	}
	int zero=0;
	uint32_t ans = unalign_rw(tmp+zero,4)&(~0u >> ((4 - len) << 3));
	return ans;
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	write_cache(addr, len, data);
	//dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}


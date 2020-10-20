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
	int l1 = read_cache(addr);
	uint32_t offset = addr & (Cache_block_size - 1);
	uint8_t ret[BURST_LEN * 2];
	memset(ret, 0, sizeof(ret));
	if(offset + len > Cache_block_size){
		int l2 = read_cache(addr + Cache_block_size - offset);
		memcpy(ret, cache[l1].data + offset, Cache_block_size - offset);
		memcpy(ret+Cache_block_size-offset, cache[l2].data, len-(Cache_block_size-offset));
	}else{
		memcpy(ret, cache[l1].data+offset, len);
	}
	int tmp = 0;
	uint32_t ans = unalign_rw(ret + tmp, 4) & (~0u >> ((4 - len) << 3));
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


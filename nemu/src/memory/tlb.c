#include "common.h"
#include "memory/tlb.h"
#include <stdlib.h>

void init_tlb(){
	int i;
	for(i = 0; i < tlb_size; i ++)
		tlb[i].valid = 0;
}

int read_tlb(uint32_t addr){
	uint32_t tag = addr >> 12;
	int i;
	for(i = 0; i < tlb_size; i ++){
		if(tlb[i].tag == tag && tlb[i].valid == 1)
			return i;
	}
	return -1;
}

void write_tlb(uint32_t addr1, uint32_t addr2){
	uint32_t tag = addr1 >> 12;
	uint32_t page = addr2 >> 12;
	int i;
	for(i = 0; i < tlb_size; i ++){
		if(tlb[i].valid == 0){
			tlb[i].valid = 1;
			tlb[i].tag = tag;
			tlb[i].page = page;
			return ;
		}
	}
	srand(i);
	i = rand() % tlb_size;
	tlb[i].valid = 1;
	tlb[i].tag = tag;
	tlb[i].page = page;
}
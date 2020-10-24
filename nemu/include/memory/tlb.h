#ifndef __TLB_H__
#define __TLB_H__

#include "common.h"

#define tlb_size 64

typedef struct{
	bool valid;
	uint32_t tag;
	uint32_t page;
}TLB;

TLB tlb[tlb_size];

void init_tlb();
int read_tlb(uint32_t);
void write_read(uint32_t, uint32_t);

#endif
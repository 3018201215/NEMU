#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *regss[] = {"es", "cs", "ss", "ds", "fs", "gs"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

void load_sreg(uint8_t sreg){
	SEG_descriptor seg_des;
	Assert(cpu.cr0.protect_enable, "Not in PM");
	uint32_t index = cpu.sreg[sreg].selector >> 3;
	Assert(index*8 < cpu.GDTR.sreg_limit, "OUT LIMIT");
	seg_des.first_part = lnaddr_read(cpu.GDTR.base_addr + index*8, 4);
	seg_des.second_part = lnaddr_read(cpu.GDTR.base_addr + index*8+4, 4);
	Assert(seg_des.p == 1, "segment error");
	cpu.sreg[sreg].base_addr1 = seg_des.base_addr1;
	cpu.sreg[sreg].base_addr2 = seg_des.base_addr2;
	cpu.sreg[sreg].base_addr3 = seg_des.base_addr3;
	cpu.sreg[sreg].sreg_limit1 = seg_des.sreg_limit1;
	cpu.sreg[sreg].sreg_limit2 = seg_des.sreg_limit2;
	cpu.sreg[sreg].sreg_limit3 = 0xfff;
	if(seg_des.g) cpu.sreg[sreg].sreg_limit <<= 12;
}

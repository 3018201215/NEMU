#include "cpu/exec/template-start.h"

#define instr test

make_helper(concat(test_r_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	uint32_t val = swaddr_read(addr, 1);
	uint32_t result = val && val;
	if(result == 0 ) cpu.EFLAGS = 0x00000042;
	print_asm("test" str(SUFFIX) " addr: 0x%X, result: %d", addr, result);
	return result;
}


#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr test

make_helper(concat(test_r_, SUFFIX)) {
	swaddr_t addr = op_src->val;
	uint32_t val = swaddr_read(addr, 4);
	uint32_t result = val && val;
	if(result == 0 ) cpu.EFLAGS = 0x00000042;
	print_asm("test" str(SUFFIX) " addr: 0x%X, result: %d", val, result);
	return 2;
}


#include "cpu/exec/template-end.h"

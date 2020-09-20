#include "cpu/exec/template-start.h"

#define instr test

make_helper(concat(test_r_, SUFFIX)) {
	swaddr_t val = op_src->val;
	uint32_t result = val && val;
	if(result == 0 ) cpu.EFLAGS = 0x00000042;
	print_asm("test" str(SUFFIX) " value: 0x%X, result: %d, 0x%X, 0x%X", val, result, cpu.eax, cpu.EFLAGS);
	return 2;
}


#include "cpu/exec/template-end.h"

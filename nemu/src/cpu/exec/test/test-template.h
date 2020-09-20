#include "cpu/exec/template-start.h"

#define instr test

make_helper(concat(test_r_, SUFFIX)) {
	swaddr_t val = op_src2->val;
	uint32_t result = val && val;
	if(result == 0 ) cpu.EFLAGS = 0x00000042;
	print_asm("test" str(SUFFIX) " value: 0x%X, result: %d", val, result);
	return 2;
}


#include "cpu/exec/template-end.h"

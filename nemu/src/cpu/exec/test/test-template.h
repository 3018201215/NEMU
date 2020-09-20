#include "cpu/exec/template-start.h"

#define instr test

make_helper(concat(test_r_, SUFFIX)) {
	swaddr_t val1 = op_src->val;
	swaddr_t val2 = op_src2->val;
	op_dest->val = val1 && val2;
	if(op_dest == 0 ) cpu.EFLAGS = 0x00000042;
	print_asm("test" str(SUFFIX) " value: 0x%X, result: %d, 0x%X, 0x%X", val1, op_dest->val, cpu.eax, cpu.EFLAGS);
	return 2;
}


#include "cpu/exec/template-end.h"

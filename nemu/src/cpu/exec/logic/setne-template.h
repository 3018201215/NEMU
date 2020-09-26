#include "cpu/exec/template-start.h"

#define instr setne

make_helper(concat(setne_r_, SUFFIX)){
	if(cpu.ZF == 0) op_src->val = 0;
	print_asm_template1();
	return 2;
}

#include "cpu/exec/template-end.h"
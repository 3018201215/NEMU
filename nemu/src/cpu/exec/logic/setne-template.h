#include "cpu/exec/template-start.h"

#define instr setne

make_helper(concat(setne_rm_, SUFFIX)){
	if(cpu.ZF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
	return 2;
}

#include "cpu/exec/template-end.h"
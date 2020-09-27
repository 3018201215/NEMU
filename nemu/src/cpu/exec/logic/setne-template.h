#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute(){
	if(cpu.ZF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	//print_asm_template1();
	print_asm("setne  0x%X",reg_b(R_AL) );
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
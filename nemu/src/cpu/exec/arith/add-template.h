#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){
	DATA_TYPE result = op_dest->val + op_src->val;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = result < op_dest->val;
	int a = op_src->val >> len;
	int b = op_dest->val >> len;
	cpu.SF = result >> len;
	cpu.OF = (a == b && b != cpu.SF);
	OPERAND_W(op_dest, result);
	cpu.ZF = !result;
	result ^= (result>>4);
	result ^= (result>>2);
	result ^= (result>>1);
	cpu.PF = !(result & 1);
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif

make_instr_helper(i2a);
make_instr_helper(i2rm);
make_instr_helper(r2rm);
make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"
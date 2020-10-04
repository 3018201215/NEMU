#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE val1 = op_src->val;
	DATA_TYPE val2 = op_dest->val;
	DATA_TYPE result = val2 - val1;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = val2 < val1;
	cpu.SF = result >> len;
	int a = val1 >> len;
	int b = val2 >> len;
	cpu.OF = (a != b && b != cpu.SF);
	cpu.ZF = !result;
	result ^= (result>>4);
	result ^= (result>>2);
	result ^= (result>>1);
	cpu.PF = !(result & 1);
	//print_asm("cmp" str(SUFFIX) " value1: 0x%X, value2: 0x%X, result: %d", val1, val2, result);
	print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif

make_instr_helper(r2rm);
make_instr_helper(i2rm);
make_instr_helper(rm2r);
make_instr_helper(i2a);

#include "cpu/exec/template-end.h"

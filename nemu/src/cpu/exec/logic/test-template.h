#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
	swaddr_t val1 = op_src->val;
	swaddr_t val2 = op_dest->val;
	uint32_t result = val1 & val2;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.SF = result >> len;
	cpu.ZF = !result;
	result ^= (result>>4);
	result ^= (result>>2);
	result ^= (result>>1);
	cpu.PF = !(result & 1);
	print_asm("test" str(SUFFIX) " value1: 0x%X, value2: 0x%X, result: %d, eflags: 0x%X", val1, val2, result, cpu.EFLAGS);
	//print_asm_template2();
}
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

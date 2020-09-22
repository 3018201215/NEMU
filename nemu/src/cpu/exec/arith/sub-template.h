#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	if(DATA_BYTE == 1) op_src->val = (int8_t) op_src->val;
	uint32_t result = op_dest->val - op_src->val;
	//op_dest->val = result;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.SF = result >> len;
	uint32_t s = result;
	int i;
	for(i=1; i<8; i++){
		s ^= result >> i;
	}
	cpu.PF = !(s && 1);
	if(result == 0){
		cpu.ZF = 1;
	}else cpu.ZF = 0;
	print_asm("sub" str(SUFFIX) " val1: 0x%X, val2: 0x%X, result: 0x%X", op_src->val, op_dest->val, result);
}

make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
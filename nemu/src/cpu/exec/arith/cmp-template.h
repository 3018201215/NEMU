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
	cpu.OF = (a != b && a == cpu.SF);
	uint32_t s = result;
	int i;
	for(i=1; i<8; i++){
		s ^= result >> i;
	}
	cpu.PF = !(s && 1);
	if(result == 0){
		cpu.ZF = 1;
	}else cpu.ZF = 0;
	print_asm("cmp" str(SUFFIX) " value1: 0x%X, value2: 0x%X, result: %d", val1, val2, result);
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif

make_instr_helper(r2rm);
make_instr_helper(i2rm);

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
	if(op_src->size == 1 && op_dest->size != 1)
		op_src->val = (int8_t)op_src->val;
	DATA_TYPE result = op_dest->val - (op_src->val + cpu.CF);
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = op_dest->val < op_src->val;
	int a = op_src->val >> len;
	int b = op_dest->val >> len;
	cpu.SF = result >> len;
	cpu.OF = (a != b && a == cpu.SF);
	uint32_t s = result;
	int i;
	for(i=1; i<8; i++){
		s ^= result >> i;
	}
	cpu.PF = !(s & 1);
	if(result == 0){
		cpu.ZF = 1;
	}else cpu.ZF = 0;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
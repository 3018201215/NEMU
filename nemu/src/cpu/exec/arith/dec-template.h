#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = op_src->val < 1;
	int a = op_src->val >> len;
	cpu.ZF = !result;
	cpu.SF = result >> len;
	cpu.OF = (a != 0 && 0 == cpu.SF);
	DATA_TYPE s = result;
	int i;	
	for(i=1; i<8; i++){
		s ^= result >> i;	
	}
	cpu.PF = !(s & 1);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

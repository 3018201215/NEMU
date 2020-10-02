#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	DATA_TYPE val = op_dest->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	// panic("please implement me");
	cpu.CF = 0;
	cpu.OF = 0;
	int len = (DATA_BYTE << 3) - 1;
	cpu.SF = result >> len;
	cpu.ZF = !result;
	DATA_TYPE s = result;
	int i;
	for(i = 1; i < 8; i ++)
		s ^= result >> 1;
	cpu.PF = !(s & 1);
	print_asm("or" str(SUFFIX) " old: 0x%X new: 0x%X eax: 0x%X", val, result, cpu.eax);
	//print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

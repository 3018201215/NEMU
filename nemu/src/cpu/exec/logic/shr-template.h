#include "cpu/exec/template-start.h"

#define instr shr

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;
	int len = (DATA_BYTE << 3) - 1;
	uint8_t count = src & 0x1f;
	cpu.CF = !!(dest & (1 << (count - 1)));
	dest >>= count;
	OPERAND_W(op_dest, dest);
	cpu.OF = 0;
	cpu.SF = dest >> len;
	cpu.ZF = !dest;
	dest ^= (dest >> 4);
	dest ^= (dest >> 2);
	dest ^= (dest >> 1);
	cpu.PF = !(dest & 1);
	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"

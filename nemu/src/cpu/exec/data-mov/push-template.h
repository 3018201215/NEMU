#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	if(DATA_BYTE == 1) op_src->val = (int8_t)op_src->val;
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, op_src->val);
	print_asm("0x%X  0x%X",cpu.esp,cpu.ebp);
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

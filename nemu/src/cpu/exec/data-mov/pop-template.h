#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	op_dest->val = swaddr_read(reg_l(R_ESP), DATA_BYTE);
	reg_l(R_ESP) += 4;
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
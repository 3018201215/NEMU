#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	OPERAND_W(op_src, swaddr_read(reg_l(R_ESP), DATA_BYTE));
	MEM_W(reg_l(R_ESP), 0);
	reg_l(R_ESP) += DATA_BYTE;
	//print_asm_template1();
	print_asm("push" str(SUFFIX) " sp:0x%X", REG(R_ESP));
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

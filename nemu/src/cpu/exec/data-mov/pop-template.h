#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	OPERAND_W(op_src,  MEM_R(REG(R_ESP)));
	MEM_W(reg_l(R_ESP), 0);
	reg_l(R_ESP) += DATA_BYTE;
	//print_asm_template1();
	print_asm("push" str(SUFFIX) " bx:0x%X", REG(R_EBX));
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif
 
#include "cpu/exec/template-end.h"

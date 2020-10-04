#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	if(DATA_BYTE == 1) op_src->val = (int8_t)op_src->val;
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, op_src->val);
	print_asm_template1();
	//print_asm("push" str(SUFFIX) " sp:0x%X", REG(R_ESP));
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)
#include "cpu/exec/template-end.h"

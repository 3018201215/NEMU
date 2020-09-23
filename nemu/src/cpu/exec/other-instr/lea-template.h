#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute(){
	//OPREND_W(op_dest, op_src);
	print_asm_template2();
}

make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"
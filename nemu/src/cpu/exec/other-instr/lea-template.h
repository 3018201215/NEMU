#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute(){
	// if(DATA_BYTE == 2){
	// 	op_src = op_src && 0xFFFF;
	// }
	// OPERAND_W(op_dest, op_src);
	print_asm_template2();
}

make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"
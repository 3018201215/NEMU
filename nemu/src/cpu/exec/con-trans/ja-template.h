#include "cpu/exec/template-start.h"

#define instr ja

static void do_execute(){
	DATA_TYPE_S addr = op_src->val;
	print_asm("ja 0x%X ", cpu.eip+1+DATA_BYTE+addr);
	if(cpu.ZF == 0 && cpu.CF == 0) cpu.eip += addr;
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"

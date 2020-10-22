#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
	if(op_src->size == 2){
		cpu.GDTR.sreg_limit = lnaddr_read(op_src->addr, 2);
		cpu.GDTR.base_addr = lnaddr_read(op_src->addr+2, 3);	
	}
	if(op_src->size == 4){
		cpu.GDTR.sreg_limit = lnaddr_read(op_src->addr, 2);
		cpu.GDTR.base_addr = lnaddr_read(op_src->addr+2, 4);	
	}
	print_asm_template1();
	//print_asm("push" str(SUFFIX) " bx:0x%X", REG(R_EBX));
}

make_instr_helper(rm)
 
#include "cpu/exec/template-end.h"
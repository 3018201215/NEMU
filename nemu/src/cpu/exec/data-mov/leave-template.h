#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_, SUFFIX)){
	swaddr_t i;
	for(i = REG(R_ESP); i<REG(R_EBP); i++)
		MEM_W(i,0);
	cpu.esp = cpu.ebp;
	cpu.ebp = MEM_R(cpu.esp);
	reg_l(R_ESP) += DATA_BYTE;
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
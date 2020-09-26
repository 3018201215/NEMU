#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_, SUFFIX)){
	//DATA_TYPE val = MEM_R(cpu.ebp);
	cpu.esp = cpu.ebp;
	reg_l(R_EBP) += 4;
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
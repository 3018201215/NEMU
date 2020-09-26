#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_, SUFFIX)){
	cpu.esp = cpu.ebp;
	cpu.ebp = MEM_R(cpu.esp);
	reg_l(R_ESP) += DATA_BYTE;
	print_asm("leave  0x%X  0x%X", cpu.ebp, cpu.esp);
	return 1;
}

#include "cpu/exec/template-end.h"
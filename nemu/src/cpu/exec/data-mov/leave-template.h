#include "cpu/exec/template-start.h"

#define instr leave

make_helper(concat(leave_, SUFFIX)){
	DATA_TYPE val = MEM_R(cpu.ebp);
	cpu.esp = val;
	reg_l(R_EBP) += 4;
	print_asm("leave  0x%X  0x%X", val, cpu.esp);
	return 1;
}

#include "cpu/exec/template-end.h"
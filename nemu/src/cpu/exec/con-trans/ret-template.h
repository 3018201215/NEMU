#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)){
	cpu.eip = swaddr_read(reg_l(R_ESP), DATA_BYTE);
	reg_l(R_ESP) += DATA_BYTE;
	print_asm("ret to 0x%X", cpu.eip);
	return 1;
}


#include "cpu/exec/template-end.h"
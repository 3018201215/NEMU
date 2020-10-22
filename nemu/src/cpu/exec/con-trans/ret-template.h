#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)){
	cpu.eip = swaddr_read(reg_l(R_ESP), DATA_BYTE, R_SS);
	MEM_W(reg_l(R_ESP),0, R_SS);
	reg_l(R_ESP) += DATA_BYTE;
	print_asm("ret to 0x%X ", cpu.eip);
	return 1;
}

make_helper(concat(ret_i_, SUFFIX)){
	int val = instr_fetch(eip+1, 2);
	cpu.eip = swaddr_read(reg_l(R_ESP), DATA_BYTE, R_SS);
	if(DATA_BYTE == 2) cpu.eip = cpu.eip & 0xfffff;
	reg_l(R_ESP) += DATA_BYTE;
	int i;
	for(i = 0; i < val; i += DATA_BYTE) MEM_W(reg_l(R_ESP)+i, 0, R_SS);
	cpu.esp += val;
	print_asm("ret $0x%X", val);
	return 1;
} 

#include "cpu/exec/template-end.h"

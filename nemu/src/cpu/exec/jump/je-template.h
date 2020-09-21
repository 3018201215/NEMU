#include "cpu/exec/template-start.h"

#define instr je

make_helper(concat(je_i_, SUFFIX)){
	if(cpu.EFLAGS == 0x42){
		uint32_t addr = op_src2->val;
		cpu.eip = addr - 2;
		print_asm("jump" str(SUFFIX) " 0x%X",addr);
	}
	return 2;
}


#include "cpu/exec/template-end.h"

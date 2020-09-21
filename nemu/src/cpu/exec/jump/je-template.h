#include "cpu/exec/template-start.h"

#define instr je

make_helper(concat(je_i_, SUFFIX)){
	int len = concat(decode_i_, SUFFIX)(eip + 1);
	if(cpu.EFLAGS == 0x42){
		uint32_t addr = op_src->val;
		cpu.eip = addr - len - 1;
		print_asm("jump" str(SUFFIX) " 0x%X",addr);
	}
	return len + 1;
}


#include "cpu/exec/template-end.h"

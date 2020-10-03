#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_, SUFFIX)) {
	if(DATA_BYTE == 2){
		DATA_TYPE val1 = reg_w(R_AX);
		swaddr_write(reg_w(R_DI), DATA_BYTE, val1);
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
		}else
			reg_w(R_DI) -= DATA_BYTE;
	}else{
		DATA_TYPE val1 = REG(R_EAX);
		swaddr_write(reg_l(R_EDI), DATA_BYTE, val1);
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
		}else
			reg_l(R_EDI) -= DATA_BYTE;
	}
	print_asm("stos" str(SUFFIX));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

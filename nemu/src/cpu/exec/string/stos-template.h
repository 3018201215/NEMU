#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
		swaddr_t val = swaddr_read(reg_w(R_DI), DATA_BYTE);
		DATA_TYPE val1 = REG(R_EAX);
		if(DATA_BYTE == 1)
			val1 &= 0xff;
		else if(DATA_BYTE == 2)
			val1 &= 0xffff;
		swaddr_write(val, DATA_BYTE, val1);
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
		}else
			reg_w(R_DI) -= DATA_BYTE;
	}else{
		swaddr_t val = swaddr_read(reg_l(R_EDI), DATA_BYTE);
		DATA_TYPE val1 = REG(R_EAX);
		if(DATA_BYTE == 1)
			val1 &= 0xff;
		else if(DATA_BYTE == 2)
			val1 &= 0xffff;
		swaddr_write(val, DATA_BYTE, val1);
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

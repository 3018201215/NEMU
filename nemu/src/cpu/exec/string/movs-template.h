#include "cpu/exec/template-start.h"

#define instr mov

make_helper(concat(movs_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
		//DATA_TYPE val1 = reg_w(R_SI);
		swaddr_write(reg_w(R_DI), DATA_BYTE, swaddr_read(reg_w(R_SI), 4));
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
			reg_w(R_SI) += DATA_BYTE;
		}else{
			reg_w(R_DI) -= DATA_BYTE;
			reg_w(R_SI) -= DATA_BYTE;
		}
		print_asm("movs" str(SUFFIX) "si:0x%X, di:0x%X", reg_w(R_SI), reg_w(R_DI));
	}else{
		DATA_TYPE val1 = reg_l(R_ESI);
		swaddr_write(reg_l(R_EDI), DATA_BYTE, swaddr_read(val1, 4));
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
			reg_l(R_ESI) += DATA_BYTE;
		}else{
			reg_l(R_EDI) -= DATA_BYTE;
			reg_l(R_ESI) -= DATA_BYTE;
		}
		print_asm("movs" str(SUFFIX) "si:0x%X, di:0x%X", swaddr_read(val1, 4), reg_l(R_EDI));
	}
	
	//print_asm("movs" str(SUFFIX) "si:0x%X, di:0x%X", swaddr_read(val1, 4), reg_l(R_EDI));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 0;
}



#include "cpu/exec/template-end.h"

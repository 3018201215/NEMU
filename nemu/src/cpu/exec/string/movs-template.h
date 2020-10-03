#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
		//DATA_TYPE val1 = reg_w(R_SI);
		swaddr_write(reg_w(R_DI), 2, swaddr_read(reg_w(R_SI), 4));
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
			reg_w(R_SI) += DATA_BYTE;
		}else{
			reg_w(R_DI) -= DATA_BYTE;
			reg_w(R_SI) -= DATA_BYTE;
		}
	}else{
		DATA_TYPE val1 = reg_l(R_ESI);
		swaddr_write(reg_l(R_EDI), 4, swaddr_read(val1, 4));
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
			reg_l(R_ESI) += DATA_BYTE;
		}else{
			reg_l(R_EDI) -= DATA_BYTE;
			reg_l(R_ESI) -= DATA_BYTE;
		}
	}
	
	//print_asm("movs" str(SUFFIX));
	print_asm("movs si:0x%X de:0x%X", swaddr_read(REG(R_ESI),4), REG(R_EDI));
	return 1;
}



#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr mov

make_helper(concat(movs_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
		DATA_TYPE val1 = reg_w(R_SI);
		DATA_TYPE val3 = swaddr_read(val1, 4);
		swaddr_write(reg_w(R_DI), DATA_BYTE, val3);
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
			reg_w(R_SI) += DATA_BYTE;
		}else{
			reg_w(R_DI) -= DATA_BYTE;
			reg_w(R_SI) -= DATA_BYTE;
		}
	}else{
		DATA_TYPE val1 = reg_l(R_ESI);
		DATA_TYPE val3 = swaddr_read(val1, 4);
		swaddr_write(reg_w(R_EDI), DATA_BYTE, val3);
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
			reg_l(R_ESI) += DATA_BYTE;
		}else{
			reg_l(R_EDI) -= DATA_BYTE;
			reg_l(R_ESI) -= DATA_BYTE;
		}
	}
	
	print_asm("movs" str(SUFFIX));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

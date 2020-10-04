#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {
	DATA_TYPE val1=0 ;
	DATA_TYPE val2=0;
	if(ops_decoded.is_operand_size_16){
		swaddr_write(reg_w(R_DI), 2, swaddr_read(reg_w(R_SI), 4));
		val1 = MEM_R(reg_w(R_SI));
		val2 = MEM_R(reg_w(R_DI));
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
			reg_w(R_SI) += DATA_BYTE;
		}else{
			reg_w(R_DI) -= DATA_BYTE;
			reg_w(R_SI) -= DATA_BYTE;
		}
	}else{
		swaddr_write(reg_l(R_EDI), 4, swaddr_read(reg_l(R_ESI), 4));
		val1 = MEM_R(reg_l(R_ESI));
		val2 = MEM_R(reg_l(R_EDI));
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
			reg_l(R_ESI) += DATA_BYTE;
		}else{
			reg_l(R_EDI) -= DATA_BYTE;
			reg_l(R_ESI) -= DATA_BYTE;
		}
	}
	
	print_asm("movs " str(SUFFIX) "si:0x%X, di:0x%X", val1,val2);
	//print_asm("movs");
	return 1;
}



#include "cpu/exec/template-end.h"

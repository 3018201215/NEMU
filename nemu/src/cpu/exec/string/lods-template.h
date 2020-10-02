#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
			REG(R_EAX) = swaddr_read(reg_w(R_SI), DATA_BYTE);
			if(cpu.DF == 0){
				reg_w(R_SI) += DATA_BYTE;
			}else
				reg_w(R_SI) -= DATA_BYTE;
	}else{
		REG(R_EAX) = swaddr_read(reg_l(R_ESI), DATA_BYTE);
		if(cpu.DF == 0){
			reg_l(R_ESI) += DATA_BYTE;
		}else
			reg_l(R_ESI) -= DATA_BYTE;
	}
	print_asm("lods" str(SUFFIX) "0x%X 0x%X",MEM_R(reg_l(R_ESI)),REG(R_EAX));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

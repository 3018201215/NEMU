#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX)) {
	if(ops_decoded.is_operand_size_16){
			if(DATA_BYTE == 1){
				swaddr_write(reg_b(R_AL), DATA_BYTE, swaddr_read(reg_w(R_SI), 4));
				if(cpu.DF == 0){
					reg_b(R_AL) += DATA_BYTE;
				}else
					reg_b(R_AL) -= DATA_BYTE;
			}else if(DATA_BYTE == 2){
				swaddr_write(REG(R_AX), DATA_BYTE, swaddr_read(reg_w(R_SI), 4));
				if(cpu.DF == 0){
					reg_w(R_AX) += DATA_BYTE;
				}else
					reg_w(R_AX) -= DATA_BYTE;
			}else{
				swaddr_write(REG(R_EAX), DATA_BYTE, swaddr_read(reg_w(R_SI), 4));
				if(cpu.DF == 0){
					reg_l(R_EAX) += DATA_BYTE;
				}else
					reg_l(R_EAX) -= DATA_BYTE;
			}
	}else{
		if(DATA_BYTE == 1){
				swaddr_write(REG(R_AL), DATA_BYTE, swaddr_read(reg_l(R_ESI), 4));
				if(cpu.DF == 0){
					reg_b(R_AL) += DATA_BYTE;
				}else
					reg_b(R_AL) -= DATA_BYTE;
			}else if(DATA_BYTE == 2){
				swaddr_write(REG(R_AX), DATA_BYTE, swaddr_read(reg_l(R_ESI), 4));
				if(cpu.DF == 0){
					reg_w(R_AX) += DATA_BYTE;
				}else
					reg_w(R_AX) -= DATA_BYTE;
			}else{
				swaddr_write(REG(R_EAX), DATA_BYTE, swaddr_read(reg_l(R_ESI), 4));
				if(cpu.DF == 0){
					reg_l(R_EAX) += DATA_BYTE;
				}else
					reg_l(R_EAX) -= DATA_BYTE;
			}
	}
	print_asm("scas" str(SUFFIX) "0x%X 0x%X",swaddr_read(reg_l(R_ESI), 4),REG(R_AL));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

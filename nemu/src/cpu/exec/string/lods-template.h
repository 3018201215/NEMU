#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_, SUFFIX)) {
// 	if(DATA_BYTE == 2){
// 		reg_w(R_AX) = swaddr_read(reg_w(R_SI), DATA_BYTE);
// 		if(cpu.DF == 0){
// 			reg_w(R_SI) += DATA_BYTE;
// 		}else
// 			reg_w(R_SI) -= DATA_BYTE;
// 	}else{
// 		REG(R_EAX) = swaddr_read(reg_l(R_ESI), DATA_BYTE);
// 		if(cpu.DF == 0){
// 			reg_l(R_ESI) += DATA_BYTE;
// 		}else
// 			reg_l(R_ESI) -= DATA_BYTE;
// 	}
// 	print_asm("lods" str(SUFFIX) " 0x%X 0x%X",swaddr_read(reg_l(R_ESI), DATA_BYTE),REG(R_EAX));
// 	return 1;
	switch(DATA_BYTE){
		case 2:
				reg_w(R_AX) = swaddr_read(reg_w(R_SI), DATA_BYTE);
				if(cpu.DF) reg_w(R_SI) -= DATA_BYTE;
				else reg_w(R_SI) += DATA_BYTE;
				break;
		default:
				REG(R_EAX) = swaddr_read(reg_l(R_ESI), DATA_BYTE);
				if(cpu.DF) reg_l(R_ESI) -= DATA_BYTE;
				else reg_l(R_ESI) += DATA_BYTE;		
	}
	print_asm("lods");
	return 1;
}



#include "cpu/exec/template-end.h"

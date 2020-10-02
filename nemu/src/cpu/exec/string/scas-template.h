#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX)) {
	DATA_TYPE result;
	int a, b;
	int len = (DATA_BYTE << 3) - 1;
	// if(ops_decoded.is_operand_size_16){
	// 		if(DATA_BYTE == 1){
	// 			swaddr_t val = swaddr_read(reg_w(R_DI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_w(R_DI) += DATA_BYTE;
	// 			}else
	// 				reg_w(R_DI) -= DATA_BYTE;
	// 			result = reg_b(R_AL) - val;
	// 			cpu.CF = reg_b(R_AL) < val;
	// 			b = reg_b(R_AL) >> len;
	// 			a = val >> len;
	// 		}else if(DATA_BYTE == 2){
	// 			swaddr_t val =  swaddr_read(reg_w(R_DI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_w(R_DI) += DATA_BYTE;
	// 			}else
	// 				reg_w(R_DI) -= DATA_BYTE;
	// 			result = reg_w(R_AX) - val;
	// 			cpu.CF = reg_w(R_AX) < val;
	// 			b = reg_w(R_AX) >> len;
	// 			a = val >> len;
	// 		}else{
	// 			swaddr_t val =  swaddr_read(reg_w(R_DI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_w(R_DI) += DATA_BYTE;
	// 			}else
	// 				reg_w(R_DI) -= DATA_BYTE;
	// 			result = reg_l(R_EAX) - val;
	// 			cpu.CF = reg_l(R_EAX) < val;
	// 			b = reg_l(R_EAX) >> len;
	// 			a = val >> len;
	// 		}
	// }else{
	// 	if(DATA_BYTE == 1){
	// 			swaddr_t val = swaddr_read(reg_l(R_EDI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_l(R_EDI) += DATA_BYTE;
	// 			}else
	// 				reg_l(R_EDI) -= DATA_BYTE;
	// 			result = reg_b(R_AL) - val;
	// 			cpu.CF = reg_b(R_AL) < val;
	// 			b = reg_b(R_AL) >> len;
	// 			a = val >> len;
	// 		}else if(DATA_BYTE == 2){
	// 			swaddr_t val =  swaddr_read(reg_l(R_EDI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_l(R_EDI) += DATA_BYTE;
	// 			}else
	// 				reg_l(R_EDI) -= DATA_BYTE;
	// 			result = reg_w(R_AX) - val;
	// 			cpu.CF = reg_w(R_AX) < val;
	// 			b = reg_w(R_AX) >> len;
	// 			a = val >> len;
	// 		}else{
	// 			swaddr_t val =  swaddr_read(reg_l(R_EDI), DATA_BYTE);
	// 			if(cpu.DF == 0){
	// 				reg_l(R_EDI) += DATA_BYTE;
	// 			}else
	// 				reg_l(R_EDI) -= DATA_BYTE;
	// 			result = reg_l(R_EAX) - val;
	// 			cpu.CF = reg_l(R_EAX) < val;
	// 			b = reg_l(R_EAX) >> len;
	// 			a = val >> len;
	// 		}
	// }
	if(DATA_BYTE == 2){
		swaddr_t val = swaddr_read(REG(R_DI), DATA_BYTE);
		result = REG(R_AX) - val;
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
		}else
			reg_w(R_DI) -= DATA_BYTE;
		a = val >> len;
		b = REG(R_AX) >> len;
	}else{
		swaddr_t val = swaddr_read(REG(R_EDI), DATA_BYTE);
		DATA_TYPE val1 = REG(R_EAX);
		if(DATA_BYTE == 1){
			val1 &= 0xff;
		}
		result = val1 - val;
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
		}else
			reg_l(R_EDI) -= DATA_BYTE;
		a = val1 >> len;
		b = val >> len;
	}
	cpu.SF = result >> len;
	cpu.OF = (a != b && a == cpu.SF);
	uint32_t s = result;
	int i;
	for(i=1; i<8; i++){
		s ^= result >> i;
	}
	cpu.PF = !(s & 1);
	if(result == 0){
		cpu.ZF = 1;
	}else cpu.ZF = 0;
	print_asm("scas" str(SUFFIX) " 0x%X 0x%X %d",swaddr_read(reg_l(R_EDI), DATA_BYTE),reg_l(R_EAX), result);
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX)) {
// 	DATA_TYPE result;
// 	int a, b;
// 	int len = (DATA_BYTE << 3) - 1;
// 	swaddr_t val;
// 	DATA_TYPE val1 = REG(R_EAX);
// 	if(DATA_BYTE == 2){
// 		val = swaddr_read(reg_w(R_DI), DATA_BYTE);
// 		result = val1 - val;
// 		if(cpu.DF == 0){
// 			reg_w(R_DI) += DATA_BYTE;
// 		}else
// 			reg_w(R_DI) -= DATA_BYTE;

// 	}else{
// 		val = swaddr_read(reg_l(R_EDI), DATA_BYTE);
// 		result = val1 - val;
// 		if(cpu.DF == 0){
// 			reg_l(R_EDI) += DATA_BYTE;
// 		}else
// 			reg_l(R_EDI) -= DATA_BYTE;
// 	}
// 	a = val >> len;
// 	b = val1 >> len;
// 	cpu.CF = val1 < val;
// 	cpu.SF = result >> len;
// 	cpu.OF = (a != b && a == cpu.SF);
// 	cpu.ZF = !result;
// 	result ^= (result>>4);
// 	result ^= (result>>2);
// 	result ^= (result>>1);
// 	cpu.PF = !(result & 1);
// 	print_asm("scas" str(SUFFIX) " 0x%X 0x%X %d", val,val1, result);
// 	rc->val, cpu.eax);
// 	return 1;
	DATA_TYPE a,dest;
	switch(DATA_BYTE){
		case 2:
				a = reg_w(R_AX);
				dest = swaddr_read(reg_w(R_DI), DATA_BYTE);
				if(cpu.DF) reg_w(R_DI) -= DATA_BYTE;
				else reg_w(R_DI) += DATA_BYTE;
				break;
		default:
				a = REG(R_EAX);
				dest = swaddr_read(reg_l(R_EDI), DATA_BYTE);
				if(cpu.DF) reg_l(R_EDI) -= DATA_BYTE;
				else reg_l(R_EDI) += DATA_BYTE;
	}
	DATA_TYPE result = a - dest;
	int len = (DATA_BYTE << 3) - 1;
	int s1 = a >> len;
	int s2 = dest >> len;
	cpu.CF = a < dest;
	cpu.SF = result >> len;
	cpu.ZF = !result;
	cpu.OF = (s1 != s2 && s2 == cpu.SF);
	result ^= (result >> 4);
	result ^= (result >> 2);
	result ^= (result >> 1);
	print_asm("scas");
	return 1;
}




#include "cpu/exec/template-end.h"

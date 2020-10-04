#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX)) {
	DATA_TYPE result;
	int a, b;
	int len = (DATA_BYTE << 3) - 1;
	swaddr_t val;
	DATA_TYPE val1 = REG(R_EAX);
	if(DATA_BYTE == 2){
		val = swaddr_read(reg_w(R_DI), DATA_BYTE);
		result = val1 - val;
		if(cpu.DF == 0){
			reg_w(R_DI) += DATA_BYTE;
		}else
			reg_w(R_DI) -= DATA_BYTE;

	}else{
		val = swaddr_read(reg_l(R_EDI), DATA_BYTE);
		result = val1 - val;
		if(cpu.DF == 0){
			reg_l(R_EDI) += DATA_BYTE;
		}else
			reg_l(R_EDI) -= DATA_BYTE;
	}
	a = val >> len;
	b = val1 >> len;
	cpu.CF = val1 < val;
	cpu.SF = result >> len;
	cpu.OF = (a != b && a == cpu.SF);
	cpu.ZF = !result;
	result ^= (result>>4);
	result ^= (result>>2);
	result ^= (result>>1);
	cpu.PF = !(result & 1);
	//print_asm("scas" str(SUFFIX) " 0x%X 0x%X %d", val,val1, result);
	print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_rm_, SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	reg_l(R_ESP) -= DATA_BYTE;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + len);
	DATA_TYPE_S addr = op_src->val;
	print_asm("call 0x%X sp:0x%X", addr, REG(R_ESP));
	cpu.eip = addr - len - 1;
	return len+1;
}

make_helper(concat(call_i_, SUFFIX)){
	int len = concat(decode_i_, SUFFIX)(eip + 1);
	reg_l(R_ESP) -= DATA_BYTE;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + len);
	DATA_TYPE_S addr = op_src->val;
	print_asm("call 0x%X sp:0x%X", cpu.eip+1+len+addr,REG(R_ESP));
	cpu.eip += addr;
	return len+1;
}

#include "cpu/exec/template-end.h"

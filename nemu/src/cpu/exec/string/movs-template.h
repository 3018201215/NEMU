#include "cpu/exec/template-start.h"

#define instr mov

make_helper(concat(movs_, SUFFIX)) {
	DATA_TYPE val1 = MEM_R(op_src->val);
	DATA_TYPE val3 = swaddr_read(val1, 4);
	swaddr_write(MEM_R(op_dest->val), DATA_BYTE, val3);
	if(cpu.DF == 0){
		OPERAND_W(op_dest, op_dest->val + DATA_BYTE);
		OPERAND_W(op_src, op_src->val + DATA_BYTE);
	}else{
		OPERAND_W(op_dest, op_dest->val - DATA_BYTE);
		OPERAND_W(op_src, op_src->val - DATA_BYTE);
	}
	print_asm("movs" str(SUFFIX));
	//print_asm("mov 0x%X 0x%X", op_src->val, cpu.eax);
	return 1;
}



#include "cpu/exec/template-end.h"

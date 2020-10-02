#include "cpu/exec/template-start.h"

#define instr test

// make_helper(concat(test_r2rm_, SUFFIX)) {
// 	swaddr_t val1 = op_src->val;
// 	swaddr_t val2 = op_dest->val;
// 	uint32_t result = val1 && val2;
// 	int len = (DATA_BYTE << 3) - 1;
// 	cpu.CF = 0;
// 	cpu.OF = 0;
// 	cpu.SF = result >> len;
// 	uint32_t s = result;
// 	int i;
// 	for(i=1; i<8; i++){
// 		s ^= result >> i;
// 	}
// 	cpu.PF = !(s && 1);
// 	if(result == 0){
// 		cpu.ZF = 1;
// 	}else cpu.ZF = 0;
// 	//print_asm("test" str(SUFFIX) " value1: 0x%X, value2: 0x%X, result: %d, eflags: 0x%X", val1, val2, result, cpu.EFLAGS);
// 	print_asm_template2();
// 	return 2;
// }

static void do_execute(){
	swaddr_t val1 = op_src->val;
	swaddr_t val2 = op_dest->val;
	uint32_t result = val1 & val2;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.SF = result >> len;
	uint32_t s = result;
	int i;
	for(i=1; i<8; i++){
		s ^= result >> i;
	}
	cpu.PF = !(s && 1);
	if(result == 0){
		cpu.ZF = 1;
	}else cpu.ZF = 0;
	print_asm("test" str(SUFFIX) " value1: 0x%X, value2: 0x%X, result: %d, eflags: 0x%X", val1, val2, result, cpu.EFLAGS);
	//print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

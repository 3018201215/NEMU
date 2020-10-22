#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	DATA_TYPE_S addr = op_src->val;
	if(op_src->type == OP_TYPE_REG || op_src->type == OP_TYPE_MEM){
		cpu.eip = addr - concat(decode_rm_, SUFFIX)(cpu.eip+1) - 1;
		print_asm_template1();
	}else{
		print_asm("jmp 0x%X", cpu.eip+1+DATA_BYTE+addr);
		cpu.eip += addr;
	}
}

make_instr_helper(i);

make_instr_helper(rm);

#if DATA_BYTE == 4
make_helper(ljmp) {
	extern SEG_descriptor *seg_des;
	SEG_descriptor seg;
	seg_des = &seg;
	uint32_t op_first = instr_fetch(eip+1,4);
	uint16_t op_second = instr_fetch(eip+5,2);
	cpu.eip = op_first;
	cpu.cs.selector = op_second;
	Assert(((cpu.cs.selector>>3)<<3) <= cpu.GDTR.sreg_limit, "OUT LIMIT %d, %d", ((cpu.cs.selector>>3)<<3), cpu.GDTR.sreg_limit);
	seg_des->first_part = instr_fetch(cpu.GDTR.base_addr + ((cpu.cs.selector>>3)<<3), 4);
	seg_des->second_part = instr_fetch(cpu.GDTR.base_addr + ((cpu.cs.selector>>3)<<3)+4, 4);
	Assert(seg_des->p == 1, "segment ERROR");
	cpu.cs.base_addr1 = seg_des->base_addr1;
	cpu.cs.base_addr2 = seg_des->base_addr2;
	cpu.cs.base_addr3 = seg_des->base_addr3;
	cpu.cs.sreg_limit1 = seg_des->sreg_limit1;
	cpu.cs.sreg_limit2 = seg_des->sreg_limit2;
	cpu.cs.sreg_limit3 = 0xfff;
	print_asm("ljmp %x,%x", op_second, op_first);
	return 0;
}
#endif

#include "cpu/exec/template-end.h"

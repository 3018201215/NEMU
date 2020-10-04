#include "cpu/exec/helper.h"

make_helper(leave) {
	swaddr_t i;
	for(i = cpu.esp; i< cpu.ebp; i += 4) swaddr_write(i, 4, 0);
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;

	print_asm("leave");
	return 1;
}

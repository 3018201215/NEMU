#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "memory/memory.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
}StackFrame;

char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
	uint32_t n;
	if(args == NULL) n = 1;
	else sscanf(args , "%u" , &n);
	cpu_exec(n);
	return 0;
}

static int cmd_info(char *args){
	char c;
	sscanf(args , "%c" , &c);
	if(c == 'r'){
		int i;
		for( i = R_EAX ; i <= R_EDI ; i ++){
			printf("$%s is 0x%X\n", regsl[i] , reg_l(i));
		}
	}else if(c == 'w'){
		print_watchpoint();
	}
	return 0;
}

static int cmd_x(char *args){
	int a,i;
	char b;
	sscanf(args , "%d %s", &a , &b);
	bool suc;
	uint32_t num = expr(&b , &suc);
	if(!suc) assert(0);
	for( i = 0; i < a; i ++){
		printf("Address: 0x%X  Value: 0x%X\n" , num+i*4 , swaddr_read(num+i*4 , 4, R_DS));
	}
	return 0;
}

static int cmd_p(char *args){
	bool a;
	uint32_t n = expr(args , &a);
	if(a) printf("%u\n" , n);
	else assert(0);
	return 0;
}

static int cmd_d(char *args){
	if(args == NULL) assert(0);
	int n;
	sscanf(args, "%d", &n);
	delete_wp(n);
	return 0;
}

void getfunc(swaddr_t addr, char *s);

static int cmd_bt(char *args){
	StackFrame s;
	swaddr_t addr = reg_l(R_EBP);
	s.ret_addr = cpu.eip;
	char ss[32];
	int cnt = 0;
	while(addr){
		getfunc(s.ret_addr, ss);
		if(ss[0] == '\0') break;
		printf("NO:%d addr:0x%X ", cnt++, s.ret_addr);
		printf("%s (", ss);
		int i;
		for(i = 0; i < 4; i++){
			s.args[i] = swaddr_read(addr+8+4*i, 4, R_SS);
			printf("%d", s.args[i]);
			printf("%c ",i == 3 ? ')' : ',');
		}
		s.ret_addr = swaddr_read(addr+4, 4, R_SS);
		s.prev_ebp = swaddr_read(addr, 4, R_SS);
		addr = s.prev_ebp;
		printf("\n"); 
	}
	return 0;
}

static int cmd_w(char *args){
	if(args == NULL) assert(0);
	WP* p = new_wp(args);
	bool b;
	p->value = expr(args, &b);
	strcpy(p->info, args);
	if(!b) assert(0);
	printf("%d %s %d\n", p->NO, p->info, p->value);
	return 0;
}

uint32_t to_page(uint32_t addr, int *flag);

static int cmd_page(char *agrs){
	uint32_t addr;
	int flag = 0;
	sscanf(agrs, "%x", &addr);
	uint32_t page_addr = to_page(addr, &flag);
	if(flag == 0) printf("the address is 0x%X, and the page is 0x%X\n", addr, page_addr);
	else if(flag == 1) printf("Dir is not used!\n");
	else printf("Page is not used!\n");
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute the next n steps of the program", cmd_si},
	{ "info", "Print the value of the register or watchpoint", cmd_info},
	{ "x", "Print the next n addresses of the address", cmd_x},
	{ "p", "Calculate the value of the expression", cmd_p},
	{ "d", "Delete a watchpoint through it's NO", cmd_d},
	{ "w", "Set up a watchpoint", cmd_w},
	{ "bt", "Print the part of the stackframe", cmd_bt},
	{ "page", "Translate the address to the page address", cmd_page},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

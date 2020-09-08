#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char* info;
	uint32_t value;
	/* TODO: Add more members if necessary */


} WP;
void new_wp(char *);
void free_wp(WP*);
bool check(WP*);
void print_watchpoint();
void delete_wp(char *);
#endif

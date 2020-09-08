#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i].value = 0;
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	WP *p = free_;
	if(p == NULL) assert(0);
	free_= p->next;
	p->next = NULL;
	if(head == NULL){ head = p;}
	else{
		WP *q = head;
		while(q->next != NULL) q = q->next;
		q->next = p;
	}
	return p;
}

void free_wp(WP* wp){
	WP *p = head;
	WP *s = free_;
	while(p->next != wp) p = p->next;
	while(s->next != NULL) s = s->next;
	p->next = wp->next;
	wp->next = NULL;
	wp->info[0] = '\0';
	wp->value = 0;
	s->next = wp;
}

bool check(){
	WP *p = head;
	bool c = true;
	while(p){
		bool b;
		uint32_t val = expr(p->info , &b);
		if(!b) assert(0);
		if(p->value != val){
			printf("CheckPoint hit! NO: %d, expr: %s old value: %d, new value: %d\n", p->NO, p->info, p->value, val);
			c = false;
			p->value = val;
		}
		p = p->next;
	}
	return c;
}

void print_watchpoint(){
	WP *p = head;
	WP *q = free_;
	printf("busy: \n");
	while(p!= NULL){
		printf("NO: %d expr: %s value:%d\n", p->NO, p->info, p->value);
		p = p->next;
	}
	printf("free: \n");
	while(q!= NULL){
		printf("NO: %d expr: %s value:%d\n", q->NO, q->info, q->value);
		q = q->next;
	}
}

void delete_wp(char *c){
	WP *p = head;
	while(strcmp(p->info, c) != 0 && p) p = p->next;
	if(p == NULL) assert(0);
	else free_wp(p);
}
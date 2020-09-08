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
		wp_pool[i].info = NULL;
		wp_pool[i].value = 0;
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *c){
	WP *p = free_;
	if(p == NULL) assert(0);
	bool b;
	p->info = c;
	p->value = expr(c, &b);
	if(!b) assert(0);
	free_->next = p->next;
	p->next = NULL;
	if(head == NULL) head = p;
	else head->next = p;
	return p;
}

void free_wp(WP* wp){
	WP *p = head;
	WP *s = free_;
	while(p->next != wp) p = p->next;
	while(s->next != NULL) s = s->next;
	p->next = wp->next;
	wp->next = NULL;
	wp->info = NULL;
	wp->value = 0;
	s->next = wp;
}

bool check(WP *wp){
	WP *p = head;
	while(p != wp) p = p->next;
	bool b , c = true;
	uint32_t val = expr(p->info , &b);
	if(!b) assert(0);
	if(p->value != val){
		printf("CheckPoint hit! NO: %d, expr: %s old value: %d, new value: %d\n", p->NO, p->info, p->value, val);
		c = false;
		p->value = val;
	}
	return c;
}

void print_checkpoint(){
	WP *p = head;
	WP *q = free_;
	printf("busy: \n");
	while(p->next != NULL)
		printf("NO: %d expr: %s value:%d\n", p->NO, p->info, p->value);
	printf("free: \n");
	while(q->next != NULL)
		printf("NO: %d info: %s value:%d\n", q->NO, q->info, q->value);
}

void delete_wp(int n){
	WP *p = &wp_pool[n];
	free_wp(p);
}
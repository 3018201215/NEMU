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

void new_wp(char *c){
	WP *p = free_;
	if(p == NULL) assert(0);
	bool b;
	strcpy(p->info, c);
	p->value = expr(c, &b);
	if(!b) assert(0);
	free_= p->next;
	p->next = NULL;
	if(head == NULL){ head = p;printf("%s\n", head->info);}
	else{
		WP *q = head;
		while(q->next != NULL) q = q->next;
		q->next = p;
		printf("%s\n", q->next->info);
	}
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
	while(p->info != c && p) p = p->next;
	if(p == NULL) assert(0);
	else free_wp(p);
}
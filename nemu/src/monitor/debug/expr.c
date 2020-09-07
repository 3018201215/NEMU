#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, AND, OR, REG, NO, NOE, NUM, HENUM, P, M

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"[0-9]+", NUM},
	{"!=", NOE},
	{"\\|\\|", OR},
	{"\\&\\&", AND},
	{"\\$[a-zA-Z]+", REG},
	{"!", NO},
	{"\\0[xX][0-9a-fA-F]+" , HENUM},
	{"==", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE: break;
					case REG: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str , substr_start+1 , substr_len-1);
						tokens[nr_token].str[substr_len-1] = '\0';
						nr_token ++;
						break;
					}
					case NUM: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str , substr_start , substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						nr_token ++;
						break;
					}
					case HENUM: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str , substr_start , substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						nr_token ++;
						break;
					}
					default: {
						tokens[nr_token].type = rules[i].token_type;
						nr_token ++;
						break;
					}
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t eval(int p, int q);
int check_parentheses(int p, int q);
int match(int p,int q);

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int i;
	for( i = 0; i< nr_token ; i ++){
		if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != HENUM && tokens[i-1].type != ')' && tokens[i-1].type != REG)))  tokens[i].type = P;
		if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != HENUM && tokens[i-1].type != ')' && tokens[i-1].type != REG)))  tokens[i].type = M;
	}
	*success = true ;
	/* TODO: Insert codes to evaluate the expression. */
	return eval(0 , nr_token-1);
}

uint32_t eval(int p, int q){
	if(p > q) assert(0);
	else if(p == q){
		uint32_t a = 0;
		switch(tokens[p].type){
 			case NUM:{
				sscanf(tokens[p].str , "%u" , &a);
				break;
			}
			case HENUM: {
				sscanf(tokens[p].str , "%X" , &a);
				break;
			}
			case REG: {
				int i;
				if(strlen(tokens[p].str) == 3){
					for(i = R_EAX; i <= R_EDI; i ++)
						if(strcmp(tokens[p].str , regsl[i]) == 0) break;
					if(i > R_EDI){
						if(strcmp(tokens[p].str , "eip") == 0) a = cpu.eip;
						else assert(0);
					}else a = reg_l(i);
				}else if(strlen(tokens[p].str) == 2){
					if(tokens[p].str[1] == 'x' || tokens[p].str[1] == 'p' || tokens[p].str[1] == 'i'){
						for(i = R_AX; i <= R_DI; i ++){
							if(strcmp(tokens[p].str , regsw[i]) == 0) break;
						if(i <= R_DI) a = reg_w(i);
						else assert(0);
						}
					}else if(tokens[p].str[1] == 'l' || tokens[p].str[1] == 'h'){
						for(i = R_AL; i <= R_BH; i ++)
							if(strcmp(tokens[p].str , regsb[i]) == 0) break;
						if(i <= R_BH) a = reg_b(i);
						else assert(0);
					}
				}
				break;
			}
		}
		return a;
	}else if(check_parentheses(p , q) == 1){
		return eval(p+1 , q-1);
	}else{
		int op=0,i;
		for(i = p; i <= q; i ++){
			if(tokens[i].type == '('){
				if(match(i , q) == -1) assert(0);
				else i = match(i , q);
			}else if(tokens[i].type == OR){
				op = i;
			}else if(tokens[i].type == AND){
				if(tokens[op].type == OR) continue;
				op = i;
			}else if(tokens[i].type == EQ || tokens[i].type == NOE){
				if(tokens[op].type == AND || tokens[op].type == OR) continue;
				op = i;
			}else if(tokens[i].type == '+' || tokens[i].type == '-'){
				if(tokens[op].type == AND || tokens[op].type == OR) continue;
				if(tokens[op].type == EQ || tokens[op].type == NOE) continue;
				op = i;
			}else if(tokens[i].type == '*' || tokens[i].type == '/'){
				if(tokens[op].type == AND || tokens[op].type == OR) continue;
				if(tokens[op].type == EQ || tokens[op].type == NOE) continue;
				if(tokens[op].type == '+' || tokens[i].type == '-') continue;
				op = i;
			}else if(tokens[i].type == NO || tokens[i].type == M || tokens[i].type == P){
				if(tokens[op].type == AND || tokens[op].type == OR) continue;
				if(tokens[op].type == EQ || tokens[op].type == NOE) continue;
				if(tokens[op].type == '+' || tokens[i].type == '-') continue;
				if(tokens[op].type == '*' || tokens[i].type == '/') continue;
				op = i;
			}
		}

		if( p == op || tokens[op].type == M || tokens[op].type == P || tokens[op].type == NO){
			uint32_t val = eval(op+1 , q);
			switch(tokens[op].type){
				case P: return swaddr_read(val , 4);
				case M: return -val;
				case NO: return !val;
				default: assert(0);
			}
		}

		uint32_t val1 = eval(p , op-1);
		uint32_t val2 = eval(op+1 , q);

		switch(tokens[op].type){
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case OR: return val1 || val2;
			case AND: return val1 && val2;
			case EQ: return val1 == val2;
			case NOE: return val1 != val2;
			default: assert(0);
		}
	}
	return 2;
}

int check_parentheses(int p, int q){
	if(tokens[p].type != '(' || tokens[q].type != ')') return 0;
	else if(match(p , q) != -1) return 1;
	else return 0;
}

int match(int p, int q){
	int i, lc = 0, m = 0;
	for(i = p ; i <= q ; i ++){
		if(tokens[i].type == '(') lc ++;
		if(tokens[i].type == ')') {
			lc --;
			m = i;
		}
		if(lc < 0) return -1;
	}
	if(lc == 0) return m;
	return -1;
}

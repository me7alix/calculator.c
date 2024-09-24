#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {number, operator, bracket} mtype;

typedef struct {
    char op;
    float num;
    char br;
    mtype t;
} mobj;

mobj* tokenizer(char* s, int* size){
    int slen = strlen(s);
    mobj* marr = malloc(sizeof(mobj) * slen);
    int cnt = 0;
    for(int i = 0; i < slen; i++){
        if(s[i] == '(' || s[i] == ')'){
            mobj n;
            n.br = s[i];
            n.t = bracket;
            marr[cnt++] = n;
        }
        if(s[i] == '*' || s[i] == '/' || s[i] == '+' || s[i] == '-'){
        	mobj n;
        	n.op = s[i];
        	n.t = operator;
        	marr[cnt++] = n;
        }
        if(s[i] - '0' < 10 && s[i] - '0' >= 0){
            for(int j = i+1; j < slen; j++){
                if(!((s[j] - '0' < 10 && s[j] - '0' >= 0) || s[j] == '.')){
                    char fst[j-i+1];
                    memcpy(fst, s+i, j-i+1);
                    fst[j-i] = '\0';
                    float num = atof(fst);
                    mobj n;
                    n.num = num;
                    n.t = number;
                    marr[cnt++] = n;
                    i = j-1;
                    break;
                }
            }
        }
    }
    *size = cnt;
    return marr;
}

float calc(mobj* marr, int mlen){
	mobj* nm = malloc(sizeof(mobj) * mlen);
	int cnt = 0;
	int bop, bcnt = 0;
	for(int i = 0; i < mlen; i++){
		nm[cnt++] = marr[i];
		if(marr[i].t == bracket && marr[i].br == '(') bcnt++;
		if(marr[i].t == bracket && marr[i].br == ')') bcnt--;
		if(marr[i].t == bracket && marr[i].br == '(' && bcnt == 1){
			bop = cnt;
		}
		if(marr[i].t == bracket && marr[i].br == ')' && bcnt == 0){
			mobj* nnm = malloc(sizeof(mobj) * (i-bop));
			memcpy(nnm, nm+bop, sizeof(mobj) * (i-bop));
			float res = calc(nnm, i-bop);
			nm[bop-1].t = number;
			nm[bop-1].num = res;
			cnt = bop;
		}
	}
	memcpy(marr, nm, cnt * sizeof(mobj));
	mlen = cnt;
	cnt = 0;
	for(int i = 0; i < mlen; i++){
		nm[cnt++] = marr[i];
		if(cnt >= 3) {
			if(nm[cnt-3].t == number && nm[cnt-2].t == operator && (nm[cnt-2].op == '*' || nm[cnt-2].op == '/')) {
				mobj num;
				num.num = (nm[cnt-2].op == '*') ? (nm[cnt-3].num * nm[cnt-1].num) : (nm[cnt-3].num / nm[cnt-1].num);
				num.t = number;
				nm[cnt-3] = num;
				cnt = cnt-2;
			}
		}
	}
	memcpy(marr, nm, cnt * sizeof(mobj));
	mlen = cnt;
	cnt = 0;
	for(int i = 0; i < mlen; i++){
		nm[cnt++] = marr[i];
		if(cnt >= 3) {
			if(nm[cnt-3].t == number && nm[cnt-2].t == operator && (nm[cnt-2].op == '+' || nm[cnt-2].op == '-')){
				mobj num;
				num.num = (nm[cnt-2].op == '+') ? (nm[cnt-3].num + nm[cnt-1].num) : (nm[cnt-3].num - nm[cnt-1].num);
				num.t = number;
				nm[0] = num;
				cnt = 1;
			}
		}
	}
	return nm[0].num;
}

void remove_spaces(char* s){
    char* d = s;
     do {
        while(*d == ' ') {
            ++d;
    	}
    } while(*s++ = *d++);
}

int main(int argc, char *argv[]){
	char *s = malloc(128);
	fgets(s, 128, stdin);
	remove_spaces(s);
	printf("%s\n", s);
	strcat(s, "<");
	int size = 0;
	mobj* ms = tokenizer(s, &size);
	free(s);
	printf("%f\n", calc(ms, size));
	free(ms);
	return 0;
}

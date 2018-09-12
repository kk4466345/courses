#include<stdio.h>
#include<string.h>
struct stack {
	int top;
	char arr[20];
};
int is_empty(struct stack* s) {
	return (s->top == -1);
}
void push(struct stack* s,char ch) {
	s->top++;
	s->arr[s->top]=ch;
	printf("pushed element %c\n",ch);
}
char pop(struct stack* s) {
	char kk=s->arr[s->top];
	s->top--;
	printf("poped %c\n",kk);
	return kk;
}
int is_space(char ch) {
	if(ch==' ') {
		return 1;
	} else {
		return 0;
	}
}
int is_oprator(char ch) {
	if(ch =='+' || ch == '-' || ch == '*' || ch =='/' || ch =='%' || ch =='^') {
		return 1;
	} else {
		return 0;
	}
}
int pre(char ch) {
	if( ch == '+' || ch =='-') {
		return 1;
	} else if(ch == '*') {
		return 2;
	} else if( ch == '/') {
		return 3;
	} else if(ch == '%') {
		return 4;
	} else {
		return 0;
	}
	
}
int main() {
	// struct stack oprator;
	// //define structure;
	// oprator.top=-1;
	// printf("structure defined\n");
	int t;
	printf("enter number of test case:");
	scanf("%d",&t);
	while(t--) {
		struct stack* oprator;
		//define structure;
		oprator->top=-1;
		//printf("structure defined\n current top=%d\n",oprator->top);
		char str[100];
		printf("enter string:");
		scanf("%s",str);
		// printf("enter string:");
		int len = strlen(str);
		char output[len];
		int i=0;
		int o=0;
		while(str[i] != '\0') {
			char ch=str[i];
			if(is_space(ch)) {
				output[o++]=ch;
			} else if(is_oprator(ch)) {
				while(1) {
					if(is_empty(oprator)) {
						printf("pushing due to empty ---%c\n",ch);
						push(oprator,ch);
						break;
					} else if(pre(ch) >= pre(oprator->arr[oprator->top])) {
						output[o++]=pop(oprator);
					}else if(pre(ch) < pre(oprator->arr[oprator->top])) {
						push(oprator,ch);
						break;	
					}
				}


			} else {
				output[o++]=ch;
			}
			i++;
		}
		while(!is_empty(oprator)) {
			output[o++]=pop(oprator);
		}
		printf("#############################################3\n");
		printf("%s\n",output);
		printf("#############################################\n");

	}
	return 0;
}

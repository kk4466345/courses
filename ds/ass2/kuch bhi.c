#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct stack {
    int top;
    char arr[10];
};


int is_empty(struct stack* s) {
	return (s->top == -1);
}

void push(struct stack* s,char ch) {
	s->top++;
	s->arr[s->top]=ch;
	//printf("pushed element %c\n",ch);
}
char pop(struct stack* s) {
	char kk=s->arr[s->top];
	s->top--;
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
	} else if(ch =='^') {
		return 5;
	} else {
        return 0;
    }
	
}

char* fun(char *str,int l) {
    char *output=(char*)malloc(100*sizeof(char));
    //strcpy(output,str);
    struct stack* oprator=(struct stack *)malloc(sizeof(struct stack));
    oprator->top=-1;
    int i=l+1;
    int o=0;
   while(str[i] != '\0') {

       char ch=str[i];
       if(str[i] == '(') {
           char *kk;
           printf("\ncalling fun  i==%d\n",i);
           kk=fun(str,i);
           strcat(output,kk);
           printf("return by fun=%s\noutput still=%s\n\n",kk,output);
           o=strlen(output);
           i=i+strlen(kk)+1;
       } else if(str[i] == ')') {
           while(!is_empty(oprator)) {
               output[o++]=' ';
               output[o++]=pop(oprator);
           }
           printf("\nreturning output=%s\n",output);

           return output;
       } else if(is_space(ch)) {
           output[o++]=ch;
       } else if(is_oprator(ch)) {
           while(1) {
               if(is_empty(oprator)) {
                   push(oprator,ch);
                   break;
               } else if(pre(ch) < pre(oprator->arr[oprator->top])) {
                   output[o++]=pop(oprator);
               } else if(pre(ch) > pre(oprator->arr[oprator->top])) {
                   push(oprator,ch);
                   break;
               } else {
                   output[o++]=ch;
                   break;
               }
           }
       } else {
           output[o++]=ch;
       }

       i++;
   }

    while(!is_empty(oprator)) {
			output[o++]=' ';
			output[o++]=pop(oprator);
		}
    return output; 
}


int main() {
    int t;
    char a;
    scanf("%d%c",&t,&a);
    while(t--) {
        char str[100];
        scanf("%[^\n]s",str);
        scanf("%c",&a);
        printf(" given string==%s\n",str);
        struct stack* s=(struct stack*)malloc(sizeof(struct stack));
        s->top=-1;
        char* temp=strdup(str);
    	char *tmp=strdup(str);
        char kk[]="(";
     	strcpy(str,kk);
     	strcat(str,tmp);
    	strcat(str,")");    	printf("%s\n",str);
    	//printf("bracket added before and end of the string");

        int len=strlen(str);

        char *output;
        output=fun(str,0);
        printf("\n\n%s\n",output);
    }

    return 0;
}
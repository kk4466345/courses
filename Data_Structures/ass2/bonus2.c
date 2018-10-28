#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct stack {
    int top;
    int *array;
};

void push(struct stack* s,int value) {
    s->top++;
    s->array[s->top]=value;
    //////printf("\tpushing and top=%d\n",s->top);
}



int pop(struct stack* s) {
    int kk=s->array[s->top];
    
    //////printf("\tpoping ang top=%d\n",s->top);
    s->top=s->top-1;
    return kk;
}

int is_empty(struct stack* s) {
    return (s->top==-1);
}

int minimum(int *arr,int n) {
    int min=arr[0];
    for(int i=0;i<n;i++) {
        if(arr[i] <min) {
            min=arr[i];
        }
    }
    return min;
}


void fun(int *arr,int n) {
    struct stack* mid=(struct stack*)malloc(sizeof(struct stack));
    mid->top=-1;
    mid->array=(int *)malloc(n*sizeof(int));
    
    int end[n];
    int e=0;
    int min=minimum(arr,n);
    int i=0; 
    for(i=0;i<n;i++) {
        if(arr[i]==min) {
            end[e++]=min;     
            break;
        } else {
            push(mid,arr[i]);
        }
    }
    i=i+1;
   
    while((e < n) && (i < n)) {
        if(is_empty(mid)) { 
            
            push(mid,arr[i]);
            i++;
        } else {
            int poped=pop(mid);
            if(arr[i] <  poped) {   /////
                
                push(mid,poped);
                push(mid,arr[i++]);
            } else {
                end[e++]=poped;
               
            }
        } 
    }
    while(!is_empty(mid)) {
        end[e++]=pop(mid);
        
    }

    for(int j=0;j<n;j++) {
        printf("%dv",end[j]);
    }
    printf("\n");
    
    

}
int main() {
    int t;
    scanf("%d\n",&t);
    while(t--) {
        int n;
        scanf("%d\n",&n);
        int arr[n];
        for(int i=0;i<n;i++) {
            
            if(i==n-1) {
                scanf("%d",&arr[i]);
            } else {
                scanf("%d\n",&arr[i]);
            }
        }

        fun(arr,n);
    }

    //printf("\n");
   // fun();
    return 0;
}

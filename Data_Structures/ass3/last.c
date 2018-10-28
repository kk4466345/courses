#include<stdio.h>
#include<stdlib.h>
struct node
{
	int min;
	int low;
	int hi;
	struct node *left;
	struct node *right;
	int max;
};


struct node* insert(struct node* root,int low,int hi) {
	if(root == NULL) {
		struct node* node=(struct node*)malloc(sizeof(struct node));
		node->low=low;
		node->hi=hi;
		return node;
	}
	if(root->low > low) {
		root->left=insert(root->left,low,hi);
	} else {
		root->right=insert(root->right,low,hi);
	}
	return root;
}

struct node* hi_insert(struct node* root,int low,int hi) {
	if(root == NULL) {
		struct node* node=(struct node*)malloc(sizeof(struct node));
		node->low=low;
		node->hi=hi;
		return node;
	}
	if(root->hi > hi) {
		root->left=hi_insert(root->left,low,hi);
	} else {
		root->right=hi_insert(root->right,low,hi);
	}
	return root;
}


struct node* delete(struct node* root,int low,int hi) {
	if(root==NULL) {
		return root;
	}
	if(root->low > low) {
		root->left=delete(root->left,low,hi);
	} else if(root->low < low){
		root->right=delete(root->right,low,hi);
	} else {
		if(root->hi == hi) {
			if(root->right == NULL) {
				struct node* temp=root->left;
				free(root);
				return temp;
			} else if(root->left == NULL){
				struct node* temp=root->right;
				free(root);
				return temp;
			} else {
				struct node* current=root->right;
				while(current->left != NULL) {
					current=current->left;
				}
				root->low=current->low;
				root->hi=current->hi;
				root->left=delete(root->right,current->low,current->hi);
			}
		} else {
			root->right=delete(root->right,low,hi);
		}
	}
}


struct node* hi_delete(struct node* root,int low,int hi) {
	if(root==NULL) {
		return root;
	}
	if(root->hi > hi) {
		root->left=delete(root->left,low,hi);
	} else if(root->hi < hi){
		root->right=delete(root->right,low,hi);
	} else {
		if(root->low == low) {
			if(root->right == NULL) {
				struct node* temp=root->left;
				free(root);
				return temp;
			} else if(root->left == NULL){
				struct node* temp=root->right;
				free(root);
				return temp;
			} else {
				struct node* current=root->right;
				while(current->left != NULL) {
					current=current->left;
				}
				root->low=current->low;
				root->hi=current->hi;
				root->left=delete(root->right,current->low,current->hi);
			}
		} else {
			root->right=delete(root->right,low,hi);
		}
	}
}

void min(struct node* root) {
	while(root->left != NULL) {
		root->left;
	}
	printf("[%d %d]\n",root->low,root->hi);
}
void max(struct node* root) {
	while(root->right != NULL) {
		root=root->right;
	}
	printf("[%d %d]\n",root->low,root->hi);
}


void low_succ(struct node* root,int low,int hi) {
	if(root==NULL) {
		printf("INVALID\n");
	} else {
		struct node* current=root;
		struct node* gtc=root;
		//printf("created node gtc and current\n");
		while(current != NULL && current->low != low) {
			if(current->low > low) {
				gtc->low=current->low;
				gtc->hi=current->hi;
				current=current->left;
			} else {
				current=current->right;
			}
		}
		// printf("find node current %d %d\n",current->low,current->hi);
		if(current==NULL) {
			printf("INVALID\n");
		}else if(current->right == NULL) {
			if(gtc->low > current->low) {
				printf("[%d %d]\n",gtc->low,gtc->hi);
			} else {
				printf("INVALID\n");
			}
		} else {
			current=current->right;
			while(current->left != NULL) {
				current=current->left;
			}
			printf("[%d %d]\n",current->low,current->hi);
		}
	}
}


void hi_succ(struct node* root,int low,int hi) {
	if(root==NULL) {
		printf("INVALID\n");
	} else {
		struct node* current=root;
		struct node* gtc=root;

		while(current != NULL && current->hi != hi) {
			if(current->hi > hi) {
				//printf("frd\n");
				gtc->low=current->low;
				gtc->hi=current->hi;
				current=current->left;
			} else {
				current=current->right;
			}
		}
		//printf("#######   %d %d\n",current->low,current->hi);
		if(current==NULL) {
			printf("INVALID\n");
		}else if(current->right == NULL) {
			if(gtc->hi > current->hi) {
				printf("[%d %d]\n",gtc->low,gtc->hi);
			} else {
				printf("INVALID\n");
			}
		} else {
			current=current->right;
			while(current->left != NULL) {
				current=current->left;
			}
			printf("[%d %d]\n",current->low,current->hi);
		}
	}
}

int isoverlap(struct node* root,int low,int hi) {
	if(root == NULL) {
		return 0;
	}
	if(root->low <= hi && low <= root->hi) {
		return 1;
	}
	if(root->hi >= low) {
		return isoverlap(root->left,low,hi);
	}
	return isoverlap(root->right,low,hi);
}


void inorder(struct node *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%d\t%d\t%d\n", root->low,root->hi,root->max); 
        inorder(root->right); 
    } 
}

//###################################################333
// ##################################################
// ################################################
//############################################
int main() {
	// struct node *root =(struct node*)malloc(sizeof(struct node));
	struct node *lowroot=(struct node*)malloc(sizeof(struct node));
	struct node *hiroot=(struct node*)malloc(sizeof(struct node));
	lowroot=hiroot=NULL;
	 int t;
	 scanf("%d\n",&t);
	 // printf("t is == %d\tscaned\n",t);
	 while(t--) {
	 	int n;
	 	scanf("%d\n",&n);
	 	// printf("n is scaned\n");
	 	struct node *lowroot=(struct node*)malloc(sizeof(struct node));
		struct node *hiroot=(struct node*)malloc(sizeof(struct node));
		lowroot=hiroot=NULL;
	 	while(n--) {
	 		char str[10];
	 		scanf("%s",str);
	 		if(str[0] == '+') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			hiroot=hi_insert(hiroot,low,hi);
	 			lowroot=insert(lowroot,low,hi);
	 		} else if(str[0] == '-') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			lowroot=delete(lowroot,low,hi);
	 			hiroot=hi_delete(hiroot,low,hi);
	 		} else if(str[2] == 'n') {
	 			min(lowroot);
	 		} else if(str[2] == 'x') {
	 			max(hiroot);
	 		} else if(str[0] == 'l') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			low_succ(lowroot,low,hi);
	 		} else if(str[0]=='h') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			hi_succ(hiroot,low,hi);
	 		} else if(str[0] == 'o') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			printf("%d\n",isoverlap(lowroot,low,hi));
	 		}
	 		// if(n == 0) {
	 		//     inorder(lowroot);
	 		//     printf("\n\n\n");
	 		//     inorder(hiroot);
	 		// }
	 	}
	 }
	return 0;
}
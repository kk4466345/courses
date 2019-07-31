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

struct node* insert_root(int a,int b) {
	struct node *node =(struct node*)malloc(sizeof(struct node));
	 node->hi=b;
	 node->low=a;
	 node->left=NULL;
	 node->right=NULL;
	 node->min=a;
	//printf("creating a node with %d\t%d\t%d\n",a,b,node->min);
	 return node;
}

struct node* insert(struct node *node,int a,int b) {
	if(node == NULL) {
		return insert_root(a,b);
	} else if(node->low > a) {
		if(a < node->min) {
			node->min=a;
		}
		node->left=insert(node->left,a,b);
	} else {
		node->right=insert(node->right,a,b);
	}
	return node;
}


 struct node* minvaluenode(struct node* node) {
 	struct node* current = node;
 	while(current->left != NULL) {
 		current=current->left;
 	}
 	return current;
 }
 
  struct node *newminvalue(struct node* node) {
 	struct node* current = node;//->left;
 	while(current->right != NULL) {
 		current=current->right;
 	}
 	return current;
 }

 int change_min(struct node* root) {
 	struct node *kk=root;
 	if(root->left != NULL) {
 	struct node *first=root->left;
 	while(first->left != NULL) {
 		first=first->left;
 		kk=kk->left;
 	}
 }
 	return kk->low;
 }

struct node* delete(struct node* root,int low,int hi) {
	if(root->min == low) {
		struct node* kk=root;
			while(kk->left != NULL) {
				kk=kk->left;
			}
			if(kk->hi == hi) {
				root->min=change_min(root);
				}
			}
	if(root == NULL) {
		return root;
	}
	if(root->low > low) {
		root->left=delete(root->left,low,hi);
	} else if(root->low < low) {
		root->right=delete(root->right,low,hi);
	} else if(root->low == low) {
		if(root->hi == hi) {
			if(root->left == NULL) {
				struct node* temp=root->right;
				root=NULL;
				return temp;
			} else if(root->right == NULL) {
				struct node* temp=root->left;
				root=NULL;
				return temp;
			}

			struct node* temp;
			if(root->right != NULL) {
			    temp = minvaluenode(root->right);
			} else {
			    temp = newminvalue(root->left);
			}
			// node with two children: Get the inorder successor (smallest 
        	// in the right subtree) 
			root->low=temp->low;
			root->hi=temp->hi;
			root->min=temp->min;
			root->right=delete(root->right,temp->low,temp->hi);
		} else {
			root->right=delete(root->right,low,hi);
		}
	}

}



int find_min(struct node* node) {
	int min_key=node->min;
	while(node->min == min_key && node->left != NULL) {
		node=node->left;
	}
	printf("[%d %d]\n",node->low,node->hi);
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


struct node* hi_insert_root(int a,int b) {
	struct node *node =(struct node*)malloc(sizeof(struct node));
	 node->hi=b;
	 node->low=a;
	 node->left=NULL;
	 node->right=NULL;
	 node->max=b;
   	 //printf("creating a hi node %d\t%d\t\t%d\n",node->low,node->hi,node->max);
	 return node;
}

struct node* hi_insert(struct node *node,int a,int b) {
	if(node == NULL) {
		return hi_insert_root(a,b);
	} else if(node->hi > b) {
		node->left=hi_insert(node->left,a,b);
	} else {
		if(b > node->max) {
			node->max=b;
			//printf("changing max for %d %d #################---->>>%d\n",node->low,node->hi,b);
		}
		node->right=hi_insert(node->right,a,b);
	}
	return node;
}


int change_max(struct node* root) {
 	struct node *kk=root;
 	if(root->right != NULL) {
 	struct node *first=root->right;
 	while(first->right != NULL) {
 		first=first->right;
 		kk=kk->right;
 	}
 	if(first->left != NULL) {
 		first=first->left;
 		while(first->right != NULL) {
 			first=first->right;
 		}
 		return first->hi;
 	}
 }
 	return kk->hi;
 }
 

 struct node *maxvalue(struct node* node) {
 	struct node* current = node;//->left;
 	while(current->right != NULL) {
 		current=current->right;
 	}
 	return current;
 }
 
 struct node *newmaxvalue(struct node* node) {
 	struct node* current = node;//->left;
 	while(current->right != NULL) {
 		current=current->left;
 	}
 	return current;
 }
 
 

 struct node* hi_delete(struct node* root,int low,int hi) {
	if(root->max == hi) {
		struct node* kk=root;
			while(kk->right != NULL) {
				kk=kk->right;
			}
			if(kk->low == low) {
				root->max=change_max(root);
				}
	}
	if(root == NULL) {
		return root;
	}
	if(root->hi > hi) {
		root->left=hi_delete(root->left,low,hi);
	} else if(root->hi < hi) {
		root->right=hi_delete(root->right,low,hi);
	} else if(root->hi == hi) {
		if(root->low == low) {
			//printf("hi");
			if(root->left == NULL) {
				struct node* temp=root->right;
				root=NULL;
				return temp;
			} else if(root->right == NULL) {
				struct node* temp=root->left;
				root=NULL;
				return temp;
			}
			struct node *temp;
            if(root->left != NULL) {
			    temp = maxvalue(root->left);
            } else {
                temp = newmaxvalue(root->right);
            }
			
			// node with two children: Get the inorder successor (smallest 
        	// in the right subtree) 
			root->low=temp->low;
			root->hi=temp->hi;
			root->left=hi_delete(root->left,root->low,root->hi);
		} else {
			root->right=hi_delete(root->right,low,hi);
		}
	}

}

int find_max(struct node* node) {
	int max_key=node->max;
	while(node->right != NULL) {
		node=node->right;
	}
	printf("[%d %d]\n",node->low,node->hi);
}



void succer(struct node* root) {
    if(root->right == NULL) {
        printf("INVALID\n");
    } else {
        root=root->right;
        while(root->left != NULL) {
            root=root->left;
        }
        printf("[%d %d]\n",root->low,root->hi);
    }
}


void low_k_succ(struct node* root,int low,int hi) {
    struct node* current=(struct node*)malloc(sizeof(struct node));
    struct node* succe=(struct node*)malloc(sizeof(struct node));
    current=root;
    if(root->low == low) {
        succer(root);
    }
    while(current->low != low && current != NULL ) {
        if(current->low > low) {
            succe->low=current->low;
            succe->hi=current->hi;
            current=current->left;
        } else {
            current=current->right;
        }
    }
    if(current == NULL) {
        printf("INVALID\n");
    }else if(current->right == NULL) {
        printf("[%d %d]\n",succe->low,succe->hi);
    } else {
        succer(current);
    }
}





void k_succ(struct node* root,int low,int hi) {
    struct node* current=(struct node*)malloc(sizeof(struct node));
    struct node *gt=(struct node*)malloc(sizeof(struct node));
    current=root;
    if(current->hi == hi) {
        succer(root);
    }
    //printf("yes\n");
    while(current->hi != hi && current != NULL ) {
        if(current->hi > hi) {
        	//printf("assigning\n");
            gt->low=current->low;
            gt->hi=current->hi;
            current=current->left;
            //printf("assigned\n");
        } else {
            current=current->right;
        }
    }
    if(current == NULL) {
        printf("INVALID\n");
    }else if(current->right == NULL) {
        printf("[%d %d]\n",gt->low,gt->hi);
    } else {
        succer(current);
    }
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
	 			find_min(lowroot);
	 		} else if(str[2] == 'x') {
	 			find_max(hiroot);
	 		} else if(str[0] == 'l') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			low_k_succ(lowroot,low,hi);
	 		} else if(str[0]=='h') {
	 			int low,hi;
	 			scanf("%d %d\n",&low,&hi);
	 			k_succ(hiroot,low,hi);
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
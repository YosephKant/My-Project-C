#include <stdio.h>
#include <stdlib.h>
/*sets the max hash size and max circular queue size*/
int HASH_SIZE=100003;
#define QUEUE_SIZE 50000

typedef struct pnode {/*puzzle struct*/
	struct pnode *back; /*position from which this one came*/
	int cost; /*number of moves to get to this position*/
	char piece; /*piece that moved to this position*/
	int dir; /*direction moved to enter this position*/
	char *board; /*this position's board*/
	} PositionBody;
typedef PositionBody *TypePosition;
/*Allocate memory for a new position.*/
TypePosition NEWPOSITION() {
    TypePosition p = (TypePosition) malloc(sizeof(PositionBody));
    if (p==NULL) {
        printf("Malloc for a new position failed.");
        exit(1);
    }
    return p;
}
/*global varuables for queue for rear, front, current queue size and max queue size at anypoint*/
int rear = - 1;
int front = - 1;
int itemsInQueue=0;
int maxQueueItems=0;
/*struct for a list at every hash bucket*/
typedef struct hList{
	char *string;
	struct hList *next;
} hList;
typedef struct hashTable{/*struct for hash table*/
	int size;
	hList **table;
} hashTable;
hashTable *createHashTable(int sizeOfHash)/*creating null hashtable*/
{
    hashTable *new=malloc(sizeof(hashTable));

    new->table=malloc(sizeof(hList)*sizeOfHash);

    /*initialize the elements of table*/
    for(int i=0; i<sizeOfHash; i++) new->table[i] = NULL;
    
    /*set hash table's size to the size given*/
    new->size = sizeOfHash;
    
    return new;
}
int hash(char* board){/*hash function that returns hash value*/
	int i=0;
	int Hvalue=0;
	while(i<12){
		Hvalue=((Hvalue*128)+(int)board[i++])%HASH_SIZE;
	}
	return Hvalue;
}
int stringCompare(char *x, char*y){/*string compare. If theyre are equal it returns in 12*/
	int count=0;
	for(int i=0; i<12; i++){
		if (x[i] == y[i]){
			count++;
		}
	}
	return count;
}
int lookUpString(hashTable *hashtable, char *board){/*checks if the string is in the hash table return 0 if is in hash table and 1 if not in hash table*/
    hList *list;
    int hashval = hash(board);

    for(list = hashtable->table[hashval]; list != NULL; list = list->next) {
        if (stringCompare(board, list->string) ==12) return 0;
    }
    return 1;
}
/*statistics for hash table*/
int numOfItemsHash(hashTable *hashtable){
	hList *list;
	int numItems=0;
	for(int i=0;i<hashtable->size;i++){
		list=hashtable->table[i];
		while(list != NULL){
			if(list!=NULL){
				numItems++;
				list = list->next;
			}
        
    	}
	}
	return numItems;
}
int numOfSingleHashBuckets(hashTable *hashtable){
	hList *list;
	int numSingItems=0;
	for(int i=0;i<hashtable->size;i++){
		list = hashtable->table[i];
		if(list!=NULL){
			list=list->next;
			if(list==NULL){
				numSingItems++;
			}
    	}
	}
	return numSingItems;
}
int numOfMaxeItemsInHashBuckets(hashTable *hashtable){
	hList *list;
	int maxItems=0;
	int numItems=0;
	for(int i=0;i<hashtable->size;i++){
		numItems=0;
		list=hashtable->table[i];
		while(list != NULL){
			if(list!=NULL){
				numItems++;
				list = list->next;
			}
			if(numItems>maxItems){
				maxItems=numItems;
			} 
    	}
	}
	return maxItems;
}
int numOfEmptyHashBuckets(hashTable *hashtable){
	int numItems=0;
	for(int i=0;i<hashtable->size;i++){
		if(hashtable->table[i]==NULL){
			numItems++;
		}
	}
	return numItems;
}
/*adds the string to the hash table by first checking if it is not already in the hash table and then adding it to the list of the bucket at the value*/
void addString(hashTable *hashtable, char *board)/*adds string if not in table*/
{
    hList *newList=malloc(sizeof(hList));
    int hashval = hash(board);
    int exists = lookUpString(hashtable, board);
    if (exists != 0){
    	newList->string = board;
    	newList->next = hashtable->table[hashval];
    	hashtable->table[hashval] = newList;
    }
    else{}
}

void stringCopy(char *x, char *y){/*copies a string into a new location*/
	for(int i=0; i<12; i++){
		x[i] = y[i];
	}
}
/*move functions which assign all the parts of the pnode and switch the index of the spacce with whatever direction listed*/
TypePosition moveRight(TypePosition a, int space){
	TypePosition new=NEWPOSITION();
	new->board=malloc(sizeof(char)*13);
	new->back=a;
	stringCopy(new->board,a->board);
	new->piece=new->board[space+1];
	new->board[space]=new->board[space+1];
	new->board[space+1]=' ';
	new->dir=1;
	struct pnode *temp;
	temp=new;
	int cost=0;
	while(temp->back!=NULL){
		temp=temp->back;
		cost++;
	}
	new->cost=cost;
	return new; 
}
TypePosition moveLeft(TypePosition a, int space){
	TypePosition new=NEWPOSITION();
	new->board=malloc(sizeof(char)*13);
	new->back=a;
	stringCopy(new->board,a->board);
	new->piece=new->board[space-1];
	new->board[space]=new->board[space-1];
	new->board[space-1]=' ';
	new->dir=2;
	struct pnode *temp;
	temp=new;
	int cost=0;
	while(temp->back!=NULL){
		temp=temp->back;
		cost++;
	}
	new->cost=cost;
	return new; 
}
TypePosition moveDown(TypePosition a, int space){
	TypePosition new=NEWPOSITION();
	new->board=malloc(sizeof(char)*13);
	new->back=a;
	stringCopy(new->board,a->board);
	new->piece=new->board[space+6];
	new->board[space]=new->board[space+6];
	new->board[space+6]=' ';
	new->dir=3;
	struct pnode *temp;
	temp=new;
	int cost=0;
	while(temp->back!=NULL){
		temp=temp->back;
		cost++;
	}
	new->cost=cost;
	return new; 
}
TypePosition moveUp(TypePosition a, int space){
	TypePosition new=NEWPOSITION();
	new->board=malloc(sizeof(char)*13);
	new->back=a;
	stringCopy(new->board,a->board);
	new->piece=new->board[space-6];
	new->board[space]=new->board[space-6];
	new->board[space-6]=' ';
	new->dir=4;
	struct pnode *temp;
	temp=new;
	int cost=0;
	while(temp->back!=NULL){
		temp=temp->back;
		cost++;
	}
	new->cost=cost;
	return new; 
}
void printPosition(TypePosition x);
/*intializes the queue */
TypePosition queue_array[QUEUE_SIZE];
/* Enqueing the queue */
void ENQUEUE(PositionBody* a){
	if (itemsInQueue == QUEUE_SIZE-1){
		printf("Queue overflow\n");
	}
	else{
		if(front==-1){
			front=0;
		}
		if(rear==QUEUE_SIZE-1){
			rear=0;
			itemsInQueue++;
		}
		else{
			rear=rear+1;
			itemsInQueue++;
		}
		if(itemsInQueue>maxQueueItems){
			maxQueueItems=itemsInQueue;
		}
		queue_array[rear]=a;
	}
}
/* Dequeing the queue */
TypePosition dequeue(){
	int temp=front;
	if (front==QUEUE_SIZE-1){
		front=0;
		itemsInQueue=itemsInQueue-1;
	}
	else{
		front=front+1;
		itemsInQueue=itemsInQueue-1;
	}
	return queue_array[temp];

}
void printPosition(TypePosition x){
	for(int i = 0; i<6;i++){
		printf("%c ", x->board[i]);
	
	}
	printf("\n");
	for(int j = 0; j<6;j++){
		printf("%c ", x->board[j+6]);
	
	}
	printf("\n\n");
}
/*prints the direction based on pnode->char and pnode->dir*/
void direction(int dir, char letter){
	if(dir==1){
		printf("move %c west\n", letter); 
	}
	else if(dir==2){
		printf("move %c east\n", letter);
	}
	else if(dir==3){
		printf("move %c north\n", letter);
	}
	else if(dir==4){
		printf("move %c south\n", letter);
	}
	else {
		printf("\n");
	}
}


/*finds the index of the space*/
int findSpace(PositionBody* a){
	for(int i=0; i < 13; i++){
		if(a->board[i]==' '){
			return i;
		}
	}
	return -1;
}
/*main function that uses the index of the space to check the possible positions and then enqueues and hashes those positions if they are not already in the hash table*/
void enqueueNextPositions(TypePosition a, int space, hashTable* b){
	
		if(space<5&&space>0){
			if(lookUpString(b,moveRight(a,space)->board)!=0){
 				ENQUEUE(moveRight(a,space));
 				addString(b,moveRight(a,space)->board);
 			}
 			if(lookUpString(b,moveLeft(a,space)->board)!=0){
 				ENQUEUE(moveLeft(a,space));
 				addString(b,moveLeft(a,space)->board);
 			}
 			if(lookUpString(b,moveDown(a,space)->board)!=0){
 				ENQUEUE(moveDown(a,space));
 				addString(b,moveDown(a,space)->board);
 			}		
		}else if(space<11&&space>6){
			if(lookUpString(b,moveRight(a,space)->board)!=0){
 				ENQUEUE(moveRight(a,space));
 				addString(b,moveRight(a,space)->board);
 			}
 			if(lookUpString(b,moveLeft(a,space)->board)!=0){
 				ENQUEUE(moveLeft(a,space));
 				addString(b,moveLeft(a,space)->board);
 			}
 			if(lookUpString(b,moveUp(a,space)->board)!=0){
 				ENQUEUE(moveUp(a,space));
 				addString(b,moveUp(a,space)->board);
 			}			
		}else if(space==5){
 			if(lookUpString(b,moveLeft(a,space)->board)!=0){
 				ENQUEUE(moveLeft(a,space));
 				addString(b,moveLeft(a,space)->board);
 			}
 			if(lookUpString(b,moveDown(a,space)->board)!=0){
 				ENQUEUE(moveDown(a,space));
 				addString(b,moveDown(a,space)->board);
 			}
		}else if(space==6){
			if(lookUpString(b,moveRight(a,space)->board)!=0){
 					ENQUEUE(moveRight(a,space));
 					addString(b,moveRight(a,space)->board);
 				}
 			if(lookUpString(b,moveUp(a,space)->board)!=0){
 				ENQUEUE(moveUp(a,space));
 				addString(b,moveUp(a,space)->board);
 			}
		}else if(space==11){
 			if(lookUpString(b,moveLeft(a,space)->board)!=0){
 				ENQUEUE(moveLeft(a,space));
 				addString(b,moveLeft(a,space)->board);
 			}
 			if(lookUpString(b,moveUp(a,space)->board)!=0){
 				ENQUEUE(moveUp(a,space));
 				addString(b,moveUp(a,space)->board);
 			}
		}
		else if (space==0){
			if(lookUpString(b,moveRight(a,space)->board)!=0){
 				ENQUEUE(moveRight(a,space));
 				addString(b,moveRight(a,space)->board);
 			}
 			if(lookUpString(b,moveDown(a,space)->board)!=0){
 				ENQUEUE(moveDown(a,space));
 				addString(b,moveDown(a,space)->board);
 			}
		}

}
/*prints the reversed list*/
void printList(TypePosition a){
	struct pnode *temp;
	temp=a;
	while(temp!=NULL){
		printf("Step %d:", temp->cost);
		direction(temp->dir,temp->piece);
		printPosition(temp);
		temp=temp->back;
	}
}
/*reverses the list*/
TypePosition reverseList(TypePosition a){
	TypePosition curr=a;
	TypePosition next=a->back;
	TypePosition nextNext=a->back->back;
	curr->back=NULL;
	while(nextNext!=NULL){
		next->back=curr;
		curr=next;
		next=nextNext;
		nextNext=next->back;
	}
	next->back=curr;
	return next;
}


int main(){
	/*creates new hash table and starting position pnodes and then enqueus and hashes them*/
	hashTable *puzzleHash=createHashTable(HASH_SIZE);
	TypePosition pnode1=NEWPOSITION();
	TypePosition currpnode;
	pnode1->board="canamapanal ";
	pnode1->dir=0;
	pnode1->cost=0;
	pnode1->back=NULL;
	pnode1->piece=' ';
	addString(puzzleHash,pnode1->board);
	ENQUEUE(pnode1);
	/*keeps dequeuing and enqueuing next positions until the final position pnode is found in the hash table*/
	while(lookUpString(puzzleHash,"panamacanal ")!=0){
		currpnode=dequeue();
		enqueueNextPositions(currpnode,findSpace(currpnode),puzzleHash);
	}
	/*prints stats*/
	printf("Number of items in hash table:%d\nNumber of empty hash buckets:%d\nNumber of single hash buckets:%d\n",numOfItemsHash(puzzleHash),numOfEmptyHashBuckets(puzzleHash),numOfSingleHashBuckets(puzzleHash));
	printf("Max number of items in hash bucket:%d\nNumber of items left in the queue:%d\nMax number of items in the queue:%d\n",numOfMaxeItemsInHashBuckets(puzzleHash),itemsInQueue,maxQueueItems);
	printList(reverseList(moveRight(currpnode,findSpace(currpnode))));
	return -1;
}
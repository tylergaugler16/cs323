#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

ofstream outputFile;

class listNode{
public:
	string data;
	listNode* next;
	listNode(void);
	listNode(string data);
	~listNode(void);
};
listNode::listNode(){
	next=NULL;
	data="";
}
listNode::listNode(string d){
	next=NULL;
	data=d;
}
listNode::~listNode(void){
	
}

class linkedListStack{
public:
	listNode* top;
	linkedListStack(void);
	void push(listNode* newNode);
	listNode* pop();
	int isEmpty();
	void printStack(string fileName1);
};
linkedListStack::linkedListStack(){
	top= new listNode();
}
void linkedListStack::push(listNode* newNode){
	newNode->next= top->next;
	top->next= newNode;
}
listNode* linkedListStack::pop(){
	listNode* nodeToPop= top->next;
	top->next= nodeToPop->next;
	return nodeToPop;
}
int linkedListStack::isEmpty(){
	if(top->next == NULL) return 1;
	else return 0;
}
void linkedListStack::printStack(string fileName1){
	// ofstream outputFile1;
	// outputFile1.open(fileName1, std::ofstream::out | std::ofstream::app);
	listNode* current= top->next;
	while(current!=NULL ){
		outputFile<<current->data<<endl;
		current=current->next;
	}
	outputFile<<endl<<endl;
}

class linkedListQueue{
public:
	listNode* tail;
	listNode* head;
	linkedListQueue(void);
	void addTail(listNode* newNode);
	listNode* deleteHead();
	int isEmpty();
	void printQueue(int tableIndex, int queueIndex,string fileName1);
};
linkedListQueue::linkedListQueue(){
	head= new listNode();
	head->next = new listNode("dummy");
	tail = new listNode();
	tail->next = head->next;

}
void linkedListQueue::addTail(listNode* newNode){
	listNode* tailNode = tail->next;
	newNode->next= tailNode;
	tail->next= newNode;
}
listNode* linkedListQueue::deleteHead(){
	listNode* headNode = tail->next;
	while(headNode->next){
		headNode=headNode->next;
	}
	listNode* current = tail;
	listNode* newHeadNode;
	// cout<<"\n current: "<<current->data<<" head: "<<head->next->data<<"\n\n";
	// cout<< headNode->data;
	// if(headNode==NULL)cout<<"yeet";
	while(current->next){
		// cout<<current->next->data<<endl<<headNode->data<<endl<<endl;
		if(current->next->data == headNode->data){
			// cout<<"yeeet";
			newHeadNode=current;	
		} 
		current=current->next;
	}
	
	newHeadNode->next=NULL;
	head->next=newHeadNode;
	return headNode; 
}
int linkedListQueue::isEmpty(){
	if(tail->next == NULL || tail->next->data == "dummy") return 1;
	else return 0;
}
void linkedListQueue::printQueue(int tableIndex, int queueIndex,string fileName1){
	listNode* current= tail->next;
	outputFile<<"Table["<<tableIndex<<"]["<<queueIndex<<"]: ";
	while(current!= NULL){
		outputFile<<current->data<<", ";
		current = current->next;
	}
	outputFile<<endl;
}

class radixSort{
public:
	int tableSize;
	linkedListQueue** hashTable;
	int currentTable;
	int tableIndex;
	radixSort(void);
	void padString(listNode* node, int longestStringLength);
	char getVal(listNode* node, int currentDigit, int longestStringLength);
	int hashIndex(char value);
	void printTable(int currentTableIndex,string filename);
};
radixSort::radixSort(){
	tableSize=256;
	hashTable= new linkedListQueue*[2];
	for (int i = 0; i < 2; ++i)  hashTable[i] = new linkedListQueue[tableSize];
  
	
}
void radixSort::padString(listNode* node, int longestStringLength){
	while(node->data.length() < longestStringLength){
		node->data= " "+node->data;
	}
}
char radixSort::getVal(listNode* node, int currentDigit, int longestStringLength){
	return node->data[longestStringLength-currentDigit];
}
int radixSort::hashIndex(char value){
	return (int)value;
}
void radixSort::printTable(int currentTableIndex,string filename){
	for(int queue=0;queue<tableSize;queue++){
			if(hashTable[currentTableIndex][queue].isEmpty() != 1){
				hashTable[currentTableIndex][queue].printQueue(currentTableIndex,queue,filename);
			}
		}
}

int main(int argc, char* argv[]){
	outputFile.open(argv[2]);
	linkedListStack* stack= new linkedListStack();
	int longestStringLength=0;

	ifstream inputFile;
	inputFile.open(argv[1]);
	if(inputFile.is_open()){
		string data;
		while(inputFile >> data){
			listNode* node= new listNode(data);
			stack->push(node);
			if(data.length() > longestStringLength) longestStringLength= data.length();
		}
	}

	stack->printStack(argv[2]);
	cout<<longestStringLength;
	inputFile.close();
	
	radixSort* radix= new radixSort();
	int currentDigit=0;
	int currentTable=0;

	while(stack->isEmpty() != 1){
		listNode* node= stack->pop();
		radix->padString(node,longestStringLength);
		char val= radix->getVal(node, currentDigit, longestStringLength-1);
		int hashIndex= radix->hashIndex(val);
		radix->hashTable[currentTable][hashIndex].addTail(node);
	}

	radix->printTable(currentTable,argv[2]);

	currentDigit++;
	currentTable=1;
	int previousTable=0;
	int currentQueue=0;

	cout<<endl;
	while(currentDigit < longestStringLength){ 
		while(currentQueue < radix->tableSize){ 
			while(radix->hashTable[previousTable][currentQueue].isEmpty() != 1){
				listNode* node= radix->hashTable[previousTable][currentQueue].deleteHead();
				char val= radix->getVal(node,currentDigit,longestStringLength-1);
				int hashIndex= radix->hashIndex(val);
				radix->hashTable[currentTable][hashIndex].addTail(node);
			}
			currentQueue++;
		}

		int temp= currentTable;
		currentTable=previousTable;
		previousTable=temp;
		currentQueue=0;
		currentDigit++;
	}
	outputFile<<endl<<endl;
	radix->printTable(previousTable,argv[2]);

	outputFile.close();
}



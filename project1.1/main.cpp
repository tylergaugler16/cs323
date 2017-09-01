#include <iostream>
#include <fstream>
#include <string>
using namespace std;



class listNode{
public:
	listNode(void);
	listNode(string d);
	listNode(string d, listNode* n);
	listNode* next;
	string data;
	int counter;
private:
	
};
listNode::listNode(){
	data	= "";
	next 	= NULL;
	counter = 1;
};
listNode::listNode(string d){
	data 	= d;
	next 	= NULL;
	counter = 1;
};
listNode::listNode(string d, listNode* n){
	data 	= d;
	next 	= n;
	counter = 1;
};




class linkedList{
public:
	linkedList();
	listNode *listHead;
	listNode* findSpot(string d);
	void listInsert(listNode *spot, listNode *newNode);
	string printList();
};
linkedList::linkedList(){
	listHead = new listNode();
	listNode *first= new listNode();
	first -> data = "dummy";
	listHead -> next = first;
};
listNode* linkedList::findSpot(string d){
	listNode *current = listHead -> next;
	while(current->next){
		listNode *nextNode = current -> next;
		// cout<< nextNode->data<<endl;
		if( nextNode -> data > d ){
			return current;
		}
		else if( current -> data == d){
			return nextNode;
		}
		else{
			current = nextNode;
		}
	}
	return current;
};
void linkedList::listInsert(listNode *spot, listNode *newNode){
	newNode -> next = spot -> next;
	spot -> next 	= newNode;
};
string linkedList::printList(){
	listNode *current = listHead -> next;
	string text="";
	while(current->next){
		listNode *nextNode = current -> next;
		text.append("("+current->data+"," + nextNode->data + ")->");
		current= nextNode;
	}
	text.append("("+current->data +", null)\n");
	return text;

};


int main(int argc, char* argv[])
{
	linkedList newList;

    ifstream inFile;
    ofstream outFile;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    if(inFile.is_open()){
    	string word;
 
    	while(inFile >> word){
   
    		listNode *spot = newList.findSpot(word);
    		// cout<< spot->data <<endl;
    		if(spot -> data == word){
    			spot -> counter= 1;
    		}
    		else{
    			listNode *newNode = new listNode();
    			newNode -> data = word;
    			newList.listInsert(spot,newNode);
    			outFile << "Insert data("+word+")\n";
    			outFile << newList.printList()+"\n";
    			
    		}
    	
    	}
    }
  
  inFile.close();
  outFile.close();
  

    return 0;
}

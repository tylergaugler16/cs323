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
	
};
listNode::listNode(){
	data	= "";
	next 	= NULL;
	counter = 0;
};
listNode::listNode(string d){
	data 	= d;
	next 	= NULL;
	counter = 0;
};
listNode::listNode(string d, listNode* n){
	data 	= d;
	next 	= n;
	counter = 0;
};

class linkedList{
public:
	linkedList();
	listNode *listHead;
	listNode* findSpot(string d);
	void listInsert(listNode *spot, listNode *newNode);
	void debugPrint(ofstream *output, listNode *ln);
	void printList(ofstream *output, listNode *ln);
	bool is_commonWord(string w);
};
linkedList::linkedList(){
	listHead = new listNode();
	listNode *first= new listNode("dummy");
	listHead -> next = first;
};
listNode* linkedList::findSpot(string d){
	listNode *current = listHead -> next;
	while(current->next){
		listNode *nextNode = current -> next;
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
void linkedList::debugPrint(ofstream *output, listNode *ln){
	listNode *current = ln -> next;
	while(current->next){
		listNode *nextNode = current -> next;
		*output << "("<<current->data<<", "<<nextNode->data<<")-->";
		current= nextNode;
	}
	*output<<"("<<current->data<<", null)"<<endl;
};
void linkedList::printList(ofstream *output, listNode *ln){
	listNode *current = ln -> next -> next;
	while(current->next){
		listNode *nextNode = current -> next;
		*output << current->data<<": "<<current->counter <<"\n";
		current= current->next;
	}
};
bool linkedList::is_commonWord(string w){
	listNode* current = listHead -> next;
	while(current->next){
		if(current->data == w)return true;
		current= current->next;
	}
	return false;
}
int main(int argc, char* argv[]){
	linkedList commonWordList;
    ifstream commonWordFile;
    ofstream output1;
    commonWordFile.open(argv[1]);
    output1.open(argv[3]);
    if(commonWordFile.is_open()){
    	string word;
 
    	while(commonWordFile >> word){
   
    		listNode *spot = commonWordList.findSpot(word);
    		// cout<< spot->data <<endl;
    		if(spot -> data == word){
    			spot -> counter= 1;
    		}
    		else{
    			listNode *newNode = new listNode();
    			newNode -> data = word;
    			commonWordList.listInsert(spot,newNode);
    			output1<<"Adding: "<<word<<endl;
    			commonWordList.debugPrint(&output1, commonWordList.listHead);
    			
    		}
    	
    	}
    }
  
	  commonWordFile.close();

	  linkedList englishTextList;
	  ifstream englishTextFile;
	  ofstream output2;
	  englishTextFile.open(argv[2]);
	  if(englishTextFile.is_open()){
	  	output1<<endl<<endl;
	  	string word;
	  	int counter=0;
	  	while(englishTextFile >> word){
	  		if(!commonWordList.is_commonWord(word)){
		  		listNode *spot= englishTextList.findSpot(word);
		  		if(spot->data == word){
		  			spot->counter++;
		  		}
		  		else{
		  			listNode *newNode= new listNode(word);
		  			englishTextList.listInsert(spot,newNode);
		  			counter++;
		  			if(counter > 5){
		  				output1<<"Adding: "<<word<<endl;
	    				englishTextList.debugPrint(&output1, englishTextList.listHead);
	    				counter=0;
		  			}
		  		}
	  		}
	  	}

	  	englishTextFile.close();

	  }

	 output1.close();
	 if(argv[4] != NULL){
	 	output2.open(argv[4]);
	 	englishTextList.printList(&output2, englishTextList.listHead);
	 }
    return 0;
}

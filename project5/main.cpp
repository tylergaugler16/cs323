#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

ifstream inputFile;
ofstream outputFile2;

class graphNode{
public:
	int nodeID;
	graphNode* next;
	graphNode* isEmpty;

	graphNode(int n);
	~graphNode(void);
	void insert(graphNode* head, graphNode* newNode);
	bool inSet(graphNode* set, int nj);
	void printGraph(graphNode* set, string filename, string header);
	void deleteNode(graphNode* head, int id);
	bool empty(graphNode* head);
	int getSize(graphNode* head);
};
graphNode::graphNode(int n){
	nodeID= n;
	next=NULL;
}
void graphNode::insert(graphNode* head, graphNode* newNode){
	newNode->next=head->next;
	head->next= newNode;	
}
bool graphNode::inSet(graphNode* set, int id){
	graphNode* current= set->next;
	while(current){
		if(current->nodeID == id) return true;
		current=current->next;
	}
	return false;

}
void graphNode::printGraph(graphNode* set, string filename, string header){
	
	if(set->next){
		outputFile2.open(filename,true);
		graphNode* current= set->next;
		outputFile2<<header;
		while(current){
			outputFile2<<current->nodeID<<" ";
			current=current->next;
		}
		outputFile2<<endl;
		outputFile2.close();
	}
	
}
void graphNode::deleteNode(graphNode* head, int id){
	graphNode* current= head->next;
	while(current->next){
		graphNode* nextNode= current->next;
		if(current->next->nodeID == id){
			current->next = nextNode->next;
			return;
		}
		current= nextNode;
	}
}
bool graphNode::empty(graphNode* head){
	if(head->next == NULL) return true;
	else return false;
}
int graphNode::getSize(graphNode* head){
	int count=0;
	graphNode* current= head->next;
	if(current->next){
		current=current->next;
		while(current){
			count++;
			current=current->next;
		}
	}
	return count;
}



class edgeNode{
public:
	int Ni;
	int Nj;
	int edgeCost;
	bool isDeleted;
	edgeNode* next;
	void printEdgeMethod(edgeNode* node);
	edgeNode(int i, int j, int cost);
	void insert(edgeNode* head, edgeNode* newNode);
	edgeNode* deleteEdge(edgeNode* head);
	void insertInFront(edgeNode* head, edgeNode* newNode);
	void printMSTofG(edgeNode* head, string filename);
	int getSize(edgeNode* head);
	
};
edgeNode::edgeNode(int i, int j, int cost){
	Ni=i;
	Nj=j;
	edgeCost=cost;
	next= NULL;
	isDeleted=false;
}
void edgeNode::insert(edgeNode* head, edgeNode* newNode){
	edgeNode* current= head;
	bool inserted=false;
	while(inserted== false && current->next != NULL){
		edgeNode* nextNode= current->next;
		if(nextNode->next==NULL){
			newNode->next= nextNode->next;
			nextNode->next=newNode;
			inserted=true;
		}
		else if(nextNode->next->edgeCost >= newNode->edgeCost){
			newNode->next= nextNode->next;
			nextNode->next=newNode;
			inserted=true;
		}
		current= nextNode;
	}

}
void edgeNode::printEdgeMethod(edgeNode* node){
	outputFile2<<"<"<<node->Ni<<", "<<node->Nj<<","<<node->edgeCost<<">"<<endl;
}
edgeNode* edgeNode::deleteEdge(edgeNode* head){
	cout<<this->getSize(head)<<endl;
	edgeNode* current= head->next;
	while(current){
		if(!current->isDeleted){
			return current;
		}
		current=current->next;
	}
	return NULL;
}
void edgeNode::insertInFront(edgeNode* head, edgeNode* newNode){
	newNode->next= head->next;
	head->next= newNode;
}
void edgeNode::printMSTofG(edgeNode* head, string filename){
	edgeNode* current= head->next;
	outputFile2.open(filename,true);
	outputFile2<<endl<<"MST of G:"<<endl;
	while(current){
		outputFile2<<"<"<<current->Ni<<", "<<current->Nj<<", "<<current->edgeCost<<" >\n";
		current=current->next;
	}
	outputFile2<<endl;
}
int edgeNode::getSize(edgeNode* head){
	int count=0;
	edgeNode* current= head->next;
	if(current->next){
		current=current->next;
		while(current){
			count++;
			current=current->next;
		}
	}
	return count;
}



class primMST{
 public:
	int N;
	int totalCost;
	int* graphNodeIDArray;
	edgeNode* edgeList;
	graphNode* setA ;
	graphNode* setB;
	edgeNode* MSTofG;
	primMST(int n);
	void PrimsMST(string filename);
	void printEdgeList(string filename);
	void printArray(string filename);
};
primMST::primMST(int n){
	N=n;
	totalCost=0;
	graphNodeIDArray= new int[N];
	for(int i=0;i<N;i++){
		graphNodeIDArray[i]=0;
	}
	edgeList= new edgeNode(0,0,0);
	edgeNode* dummy= new edgeNode(0,0,0);
	edgeList->next= dummy;
	setA= new graphNode(0);
	setB= new graphNode(0);
	setA->next= new graphNode(0);
	setB->next= new graphNode(0);
	MSTofG = new edgeNode(0,0,0);
	MSTofG->next = new edgeNode(0,0,0);
}
void primMST::printEdgeList(string filename){
	outputFile2.open(filename,true);
	int count=1;
	edgeNode* current = edgeList->next->next;
	while(current!=NULL && count<=10){
		current->printEdgeMethod(current);
		current=current->next;
		count++;
	}
	outputFile2.close();
}
void primMST::printArray(string filename){
	outputFile2.open(filename);
	for(int i=1;i<=N;i++){
		outputFile2<<i<<": "<< graphNodeIDArray[i]<<endl;
	}
	outputFile2<<endl;
	outputFile2.close();
}
void primMST::PrimsMST(string filename){
	int k=1;	
	while(graphNodeIDArray[k] == 0){
		k++;
	}

	graphNode* firstNode= new graphNode(k);
	setA->insert(setA->next, firstNode);
	// setA->printGraph(setA);
	for(int i=k+1;i<=N;i++){
		graphNode* newNode= new graphNode(i);
		setB->insert(setB->next,newNode);
	}
	// setB->printGraph(setB->next);
	

	while(!setB->empty(setB->next)){ 
		this->printEdgeList(filename);
		// cout<<"size of edg list:" <<edgeList->getSize(edgeList)<<endl;
		edgeNode* newEdge= edgeList->deleteEdge(edgeList->next);
		cout<<edgeList->getSize(edgeList);
		while( (setA->inSet(setA, newEdge->Ni) && setA->inSet(setA, newEdge->Nj)) ){
			newEdge= edgeList->deleteEdge(edgeList->next);
		}

			newEdge->isDeleted=true;
			// cout<<newEdge->edgeCost<<endl;
			MSTofG->insertInFront(MSTofG->next, newEdge);
			totalCost+=newEdge->edgeCost;
			// cout<<"adding to A: "<<newEdge->Ni<<endl;
			// cout<<setB->next->next->nodeID<<endl;
			// cout<<"size: "<<setB->getSize(setB)<<endl;
			setB->deleteNode(setB, newEdge->Ni);
			setA->insert(setA->next, new graphNode(newEdge->Ni));
			setA->printGraph(setA->next, filename, "Set A:\n");
			setB->printGraph(setB->next, filename, "Set B: \n"); // remove later
			// MSTofG->printMSTofG(MSTofG->next, filename);

		
		

	}
	// setB->printGraph(setB->next, filename, "Set B final:::: \n");
}


int main(int argc, char* argv[]){
	primMST* prim=NULL;
	inputFile.open(argv[1]);
	if(inputFile.is_open()){
		string data;
		int line =1;
		while(inputFile>>data){
			if(line==1) prim = new primMST(stoi(data));
			else {
				int ni= stoi(data);
				inputFile>>data;
				int nj= stoi(data);
				inputFile>>data;
				int edgeCost=stoi(data);
				edgeNode* newNode= new edgeNode(ni,nj,edgeCost);
				// newNode->printEdgeMethod(newNode);
				prim->edgeList->insert(prim->edgeList, newNode);
				prim->graphNodeIDArray[ni]++;
				prim->graphNodeIDArray[nj]++;
			}
			line++;
		}
		
		inputFile.close();

		prim->printArray(argv[3]);
		prim->printEdgeList(argv[3]);
		prim->PrimsMST(argv[3]);

	}

	
}












#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
using namespace std;

ifstream inputFile1;
ifstream inputFile2;
ofstream outputFile1;


class graphNode{
	friend class hashNode;
public:
	int jobId;
	graphNode* next ;
	graphNode(int id);
	graphNode();
	void insert(graphNode* newNode);
	void print(graphNode* current);
	int popJob(graphNode* nodeToBeRemoved);
};
graphNode::graphNode(int id){
	jobId = id;
	next= NULL;
}
graphNode::graphNode(){
	jobId=0;
	next=NULL;
}
void graphNode::insert(graphNode* newNode){
	newNode->next= this->next;
	this->next= newNode;
}
void graphNode::print(graphNode* current){
	while(current){
		cout<<"job("<< current->jobId<<")-> ";
		if(current->next) current=current->next;
		else break;
	}
	cout<<endl;
}
int graphNode::popJob(graphNode* nodeToBeRemoved){
	graphNode* temp = nodeToBeRemoved->next;
	this->next= temp;
	return nodeToBeRemoved->jobId;

}
class hashNode{
public:
	int jobTime;
	int fatherCount;
	int childCount;
	graphNode* stackTop;
	hashNode();
	void push(graphNode* child);
	bool hasNodes(hashNode* graphHashTable, int numJobs);
	void removeKids(hashNode* graphHashTable, int jobDone);
};
hashNode::hashNode(){
	jobTime=0;
	fatherCount=0;
	childCount=0;
	stackTop= NULL;
}
void hashNode::push(graphNode* child){
	if(this->stackTop == NULL){
		this->stackTop = child;
	}
	else{
		graphNode* temp = this->stackTop;
		this->stackTop = child;
		child->next = temp;
	}
}
bool hashNode::hasNodes(hashNode* graphHashTable, int numJobs){
	int count=0;
	int index=0;
	for(int i=1; i <= numJobs; i++){
		if(graphHashTable[i].fatherCount > -2){
			count++;
			index= i;
		} 
	}
	if(count==1) graphHashTable[index].fatherCount--;
	if(count > 0) return true;
	return false;
}
void hashNode::removeKids(hashNode* graphHashTable, int jobDone){
	graphNode* current= graphHashTable[jobDone].stackTop;
	while(current){
		graphHashTable[current->jobId].fatherCount--;
		current= current->next;
	}
}

void insert(graphNode* open_queue, graphNode* newNode, int newNodeindex, hashNode* graphHashTable);

int main(int argc, char* argv[]){
	int numJobs=0;
	int numProcessors= 1;
	if(argv[3])numProcessors= stoi(argv[3]);
	int totalJobTime=0;
	int time=1;

	int** processorSchedule;

	hashNode* graphHashTable;
	graphNode* OPEN = new graphNode();

	int index=0;
	inputFile1.open(argv[1]);
	if(inputFile1.is_open()){
		string data;
		while(inputFile1 >> data ){
			if(index==0){
				numJobs = stoi(data);
				if(numJobs < numProcessors) numProcessors =numJobs;
				graphHashTable = new hashNode[numJobs+1];
				for(int i=1;i<numJobs+1;i++){
					graphHashTable[i] = hashNode();
				}
				OPEN = new graphNode[numJobs+1];
			}
			else{
				int job= stoi(data);
				inputFile1 >> data;
				int child= stoi(data);
				graphNode* newNode= new graphNode(child);
				graphHashTable[job].push(newNode);
				graphHashTable[job].childCount++;
				graphHashTable[child].fatherCount++;
			}
			
			index++;
		}
		time=1;

	}

	inputFile2.open(argv[2]);
	if(inputFile2.is_open()){
		string data;
		inputFile2 >>data;
		while(inputFile2 >> data){
			int job= stoi(data);
			inputFile2 >> data;
			int jobTime = stoi(data);
			totalJobTime+=jobTime;
			graphHashTable[job].jobTime= jobTime;
		}
	}

// instantiate processor schedule
	processorSchedule = new int*[numProcessors+1];
	for(int i=0;i<numProcessors+1;i++){
		processorSchedule[i] = new int[totalJobTime+1];
	}
	for(int r=0;r<numProcessors+1;r++){
		for(int c=0;c<totalJobTime+1;c++){
			processorSchedule[r][c] = 0;
		}
	}


 int count=0;
 while(graphHashTable->hasNodes(graphHashTable, numJobs)){
	 	//get orphans 

		int job = 1;
		int orphan = 0;
		while(job <= numJobs){
			if(graphHashTable[job].fatherCount == 0){
				cout<<"job: "<<job<<" is an orphan"<<endl;
				orphan = job;
				graphNode* orphanNode = new graphNode(orphan);
				insert(OPEN, orphanNode, job, graphHashTable);
				OPEN->print(OPEN);
				graphHashTable[orphan].fatherCount--;
				// cout<<" job has "
			} 
			
			job++;
		}

		OPEN->print(OPEN);

	// put on processor
		int processor= 1;
		int availableProcessor=0;
		int jobToPutOn=0;
		int jobTime=0;
		int jobDone=0;
		while( OPEN->next && (processor <= numProcessors) ){
			if(OPEN->next){
				if(processorSchedule[processor][0] <= 0){
					availableProcessor = processor;
					processorSchedule[availableProcessor][0]++;
					jobToPutOn = OPEN->popJob(OPEN->next);
					cout<<"job to be put on: "<<jobToPutOn<<endl;
					OPEN->print(OPEN);
					jobTime = graphHashTable[jobToPutOn].jobTime;
					} 
			}
			int slot = time;
			while (slot < time + jobTime){
				processorSchedule[availableProcessor][slot] = jobToPutOn;
				slot++;
			} 
			processor++;
		}
		time++;

		processor=1;
		while (processor <= numProcessors){
			if ((processorSchedule[processor][0] > 0) && (processorSchedule[processor][time] <= 0) ){
				jobDone= processorSchedule[processor][time-1];
				graphHashTable[jobDone].fatherCount--;
				graphHashTable->removeKids(graphHashTable, jobDone);
				processorSchedule[processor][0] = 0;
				cout<<"job "<<jobDone<<" just finished"<<endl<<endl;
				}
			processor++;
		}
		for(int r=1;r<numProcessors+1;r++){
			for(int c=1;c<totalJobTime+1;c++){
				cout<< processorSchedule[r][c]<<" ";
			}
			cout<<endl;
		}
		for(int i=1;i<numJobs+1;i++){
			cout<<"job "<<i<<" has "<<graphHashTable[i].fatherCount<<" fathers"<<endl;
		}
		cout<<endl;
		count++;
 	}// while there are still nodes left
	
	outputFile1.open(argv[4]);
	if(outputFile1.is_open()){
		for(int r=0;r<numProcessors+1;r++){
			for(int c=1;c<time;c++){
				if(r==0) outputFile1<< setw(5)<< left <<"_____";
				else if(c==1)outputFile1<<"p"<<r<<"|"<< setw(5)<< left << processorSchedule[r][c];
				else  outputFile1<< setw(5)<< left << processorSchedule[r][c];
			}
			outputFile1<<endl;
		}
		for(int r=0;r<time-1;r++) outputFile1<< setw(5)<< left <<"-----";

	}

}

void insert(graphNode* open_queue, graphNode* newNode, int newNodeindex, hashNode* graphHashTable){
	bool inserted = false;

	if(open_queue->next != NULL){
		graphNode* current= open_queue;
		while(current->next && !inserted){
			if(graphHashTable[current->next->jobId].jobTime  > graphHashTable[newNode->jobId].jobTime){
				current->insert(newNode);
				inserted=true;
			}
			else if(graphHashTable[current->next->jobId].jobTime == graphHashTable[newNode->jobId].jobTime){

					if(graphHashTable[current->next->jobId].childCount < graphHashTable[newNode->jobId].childCount){
						current->insert(newNode);
						inserted = true;
					}
					else{
						while(current->next && (graphHashTable[current->next->jobId].jobTime == graphHashTable[newNode->jobId].jobTime) && !inserted){

							if(graphHashTable[current->next->jobId].childCount <= graphHashTable[newNode->jobId].childCount){
								current->insert(newNode);
								inserted= true;
							}
							if(current->next && !inserted){
								current = current->next;
								if(current->next == NULL){
									current->insert(newNode);
									inserted= true;
								}
							} 
							else break;
						} // while loop for number of kids spot
					}

			} // if they have the same jobTime

			if(current->next && !inserted){
				current = current->next;
				
			} 
			if( current->next == NULL){
				current->insert(newNode);
				inserted=true;
			} 
			// else break;
		} // outer while loop
	}
	else{
		open_queue->next = newNode;
	}

}











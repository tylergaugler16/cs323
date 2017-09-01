#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class PQSort{
public:
	PQSort(int size);
	~PQSort();
	int* PQArray;
	void buildPQArray(string fileName1, string filename2);
	void deletePQArray(string fileName1, string filename2);
	
private:
	void insertOneDataItem(int value);
	string printPQ();
	void swapValues(int v1, int v2);
	int deleteRoot();
	void DeleteRoot();
	void bubbleUp(int index);
	void bubbleDown();
};
PQSort::PQSort(int size){
	PQArray= new int[size];
	PQArray[0]=0;
}
PQSort::~PQSort(){
	delete[] PQArray;
}
void PQSort::buildPQArray(string fileName1, string filename2){
	ifstream inputFile;
	ofstream outputFile;
	inputFile.open(fileName1);
	outputFile.open(filename2);
	if(inputFile.is_open() && outputFile.is_open()){
		string data;
		while(inputFile >> data){
			int value= std::stoi(data);
			insertOneDataItem(value);
			outputFile<<printPQ();
		}
		outputFile<<"\n\n";
	}
	inputFile.close();
	// outputFile.close();
}

void PQSort::insertOneDataItem(int value){
	int size= PQArray[0];
	PQArray[size+1]= value;
	PQArray[0]++;
	bubbleUp(size+1);
}
string PQSort::printPQ(){
	int size=((PQArray[0]<10) ? PQArray[0] : 10);
	int i=0;
	string line="";
	while(i <= size){
		line= line.append(std::to_string(PQArray[i])+" ");
		i++;
	}
	line= line.append("\n");
	return line;
}
void PQSort::bubbleUp(int position){
	while(PQArray[position] < PQArray[position/2]){
		if(position == 1)break;

		swapValues(position,position/2);
		
		position=position/2;
	}
}
void PQSort::swapValues(int v1, int v2){
	int temp= PQArray[v1];
	PQArray[v1]=PQArray[v2];
	PQArray[v2]=temp;
}
void PQSort::deletePQArray(string fileName1, string filename2){
	ofstream outputFile1;
	ofstream outputFile2;
	outputFile1.open(fileName1, std::ofstream::out | std::ofstream::app);
	outputFile2.open(filename2);
	if(outputFile1.is_open() && outputFile2.is_open()){
		while(PQArray[0] > 0){
			outputFile2 << deleteRoot() <<endl;
			bubbleDown();
			outputFile1 << printPQ();
		}	
	}
	outputFile1.close();
	outputFile2.close();
}
int PQSort::deleteRoot(){i
	if(PQArray[0] > 0){
		int root = PQArray[1];
		int size= PQArray[0];
		PQArray[1]= PQArray[size];
		PQArray[0]--;
		return root;
	}
	return 0;
}
void PQSort::bubbleDown(){
	int position= 1;
	while(position < PQArray[0]){

		int left_child= (position*2);
		if(left_child >= PQArray[0])return;
		int right_child= (position*2)+1;
		int smaller_child= (PQArray[left_child] < PQArray[right_child])? left_child : right_child;
		if(PQArray[position] > PQArray[smaller_child]){
			swapValues(position, smaller_child);
		}
		else {break;}
		position= smaller_child;
		
	}
}

int main(int argc, char* argv[]){

	ifstream inputFile;
	inputFile.open(argv[1]);
	int size=0;
	if(inputFile.is_open()){
		string data;
		while(inputFile >> data){
			size++;
		}
	}
	inputFile.close();
	
	PQSort* myPriorityQueue= new PQSort(size);
	myPriorityQueue->buildPQArray(argv[1], argv[2]);
	myPriorityQueue->deletePQArray(argv[2], argv[3]);

}
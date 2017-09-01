#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
using namespace std;

ifstream inputFile;
ofstream outputFile1;
ofstream outputFile2;

class Point{
public:
	int xCoordinate;
	int yCoordinate;
	int clusterID;
	double distance;
	void printPoint(void);
	Point(int x, int y);
	Point(void);
};
Point::Point(int x, int y){
	xCoordinate=x;
	yCoordinate=y;
}
Point::Point(){

}
// void Point::printPoint(){

// }

class K_mean{

struct XYCordinate{
	int xCord;
	int yCord;
};
public:
	int K;
	XYCordinate* kCentroids;
	int numPoints;
	Point* pointSet;
	int pointSetIndex;
	int numRow;
	int numCol;
	int** imageArray;
	void loadPointSet(Point* p);
	void assignLabel(Point* p);
	void mapPoint2Image();
	void kMeanClustering();
	void printPointSet();
	void displayImage();
	void printToOutput1();
	void printToOutput2();
	void findCentroids();
	int findLength(Point p, int cluster_id);
	K_mean(int k, int np, int nr, int nc);
	

};
K_mean::K_mean(int k, int np, int nr, int nc){
	K=k;
	numPoints=np;
	numRow=nr;
	numCol=nc;
	pointSet= new Point[numPoints];
	pointSetIndex=0;
	imageArray= new int*[nr];
	for(int i = 0; i < nr; ++i){
		imageArray[i] = new int[nc];
	}
	kCentroids= new XYCordinate[K];
}
void K_mean::loadPointSet(Point* p){
	assignLabel(p);
	pointSet[pointSetIndex]= *p;
	pointSetIndex++;
}
void K_mean::assignLabel(Point* p){
	p->clusterID= (pointSetIndex % K) +1;
}
void K_mean::mapPoint2Image(){
	for(int i=0;i<pointSetIndex;i++){
		imageArray[pointSet[i].xCoordinate][pointSet[i].yCoordinate]= pointSet[i].clusterID;
	}
}
void K_mean::displayImage(){
	for(int row=0;row<numRow;row++){
		for(int col=0;col<numCol;col++){
			if(imageArray[row][col]) outputFile2<< imageArray[row][col]<<" ";
			else outputFile2<<" ";
		}
		outputFile2<<endl;
	}
}
void K_mean::printToOutput1(){
	outputFile1<<K<<endl<<numPoints<<endl<<"\t"<<numRow<<" "<<numCol<<endl;
	for(int i=0;i<pointSetIndex;i++){
		outputFile1<<"\t"<<pointSet[i].xCoordinate<<" "<<pointSet[i].yCoordinate<<" "<<pointSet[i].clusterID<<endl;
	}
}
void K_mean::printToOutput2(){
	for(int row=0;row<numRow;row++){
		for(int col=0;col<numCol;col++){
			if(imageArray[row][col]) outputFile2<< imageArray[row][col]<<" ";
			else outputFile2<<" 1";
		}
		outputFile2<<endl;
	}
}
void K_mean::findCentroids(){
	for(int i=0;i<pointSetIndex;i++){
		kCentroids[pointSet[i].clusterID].xCord+= (pointSet[i].xCoordinate)/K;
		kCentroids[pointSet[i].clusterID].yCord+= (pointSet[i].yCoordinate)/K;
	}
	
	bool ChangesOccur=false;
	while(!ChangesOccur){
		ChangesOccur=false;
		for(int i=0;i<pointSetIndex;i++){
			int min_i;
			for(int c=1;c<=K;c++){
				if(findLength(pointSet[i], c) < findLength(pointSet[i], pointSet[i].clusterID)){
					pointSet[i].clusterID= c;
					ChangesOccur=true;
				}
			}
		}
		printToOutput1();
		displayImage();

	}
	
}
int K_mean::findLength(Point p, int cluster_id){
	int centroidx= kCentroids[cluster_id].xCord;
	int centroidy= kCentroids[cluster_id].yCord;
	return sqrt(((p.xCoordinate - centroidx)^2)- ((p.yCoordinate - centroidy)^2));
}

int main(int argc, char* argv[]){
	outputFile1.open(argv[2]);
	outputFile2.open(argv[3]);
	K_mean* kmean;
	int line=1;
	int k;
	int numRow;
	int numCol;
	int numPoints;
	inputFile.open(argv[1]);
	if(inputFile.is_open() && line <= 4){
		string data;
		while(inputFile >> data){
			// cout<<data<<endl;
			if(line == 1) k= std::stoi(data);
			else if(line == 2) numPoints = std::stoi(data);
			else if(line == 3) numRow= std::stoi(data);
			else if(line == 4) {
				numCol= std::stoi(data);
				kmean= new K_mean(k,numPoints,numRow,numCol);
			}
			else {
				int x= std::stoi(data);
				inputFile >> data;
				int y= std::stoi(data);
				Point* newPoint= new Point(x, y);
				kmean->loadPointSet(newPoint);
			}
			line++;
		}
		
		kmean->printToOutput1();
		kmean->mapPoint2Image();	
		kmean->displayImage();
		kmean->findCentroids();
	}


	outputFile2.close();
	outputFile1.close();
}
























#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MAX 50

//Author:Berk Limoncu ID:2243541

struct graphHead{
	int count;							//#of vertice in the graph.		
	struct graphVertices *first;		
};

struct graphVertices{
	struct graphVertices *next;			
	char scanPath[MAX]; 				//The data that we are storing.
	int verticeIndex;					//Each vertice has a unique index.
	struct graphEdges *firstEdges;
};

struct graphEdges{
	struct graphVertices *destination;  //Destination from the edge
	struct graphEdges *next;			//Next edge
	double weight;						//I'm storing the similarityScore
};

struct edgeRecord{				//I'm using this linked list to sort edges with respect to simalirity score.
	int firstVerticeIndex;		//First vertice which is  connected by the edge
	int secondVerticeIndex;	    //Second vertice which is  connected by the edge
	double similarityScore; 	//Similarity score of 2 vertices a kind of weight.
	struct edgeRecord *next;
};

struct graphHead* readData(char *);   							//Get the name of the file and create head and inside of this function I'm calling createVertice.
void createVertice(struct graphHead*,char *);					//Get head of graph and scanpath of the edge and create edge.	
int createEdge(struct graphHead*,char *,char *);				//Get head of graph and 2 strings and create edges.
void printSimilarScanPaths(struct graphHead*);					//Firstly, iterate all edges and create new edgeRecord and sorts it then prints it.
void sortEdgeRecord(struct edgeRecord **);						//Get references of edgeRecord linked list and sorts(Bubble).
void swapEdgeRecord(struct edgeRecord *,struct edgeRecord *);	//Get 2 references of edgeRecord and swap their values.
int max(int,int);							
int min(int,int,int);  											//Get 3 integer and returns minimum of 3 integer.
int editDist(char *,char *,int,int); 							//Gets 2 string and their lengths and returns similarity btw two scanpaths.
double similarityScore(char*,char*);							//Calls editDist then calculates similarity score with S=100*(1-(d/N)).

int main()
{
	struct graphHead *head=NULL;
	struct graphVertices *tempVertices=NULL;		//tempVertices->head->next;
	struct graphVertices *nextOfTempVertices=NULL;	//tempVertices->head->next;
	
	char fileName[MAX];
	
	printf("Enter a file name > ");
	strcpy(fileName,"scanpaths.txt"); 
	gets(fileName);
	
	head=readData(fileName);	//Create  GraphHead & read data from .txt file and then create vertices.

	if(head==NULL)		//In case of empty file
	{
		printf("Empty File!\n");
		exit(0);
	}
	
	tempVertices=head->first;
	while(tempVertices != NULL)
	{
		nextOfTempVertices = tempVertices->next; //next pointer of tempVertice
		while(nextOfTempVertices != NULL)
		{
			if(similarityScore(tempVertices->scanPath,nextOfTempVertices->scanPath)>50.0)		//If similarityScore is >50 between 2 vertices.Then, they will be adjacent.
				createEdge(head,tempVertices->scanPath,nextOfTempVertices->scanPath);			//createEdge
			
			nextOfTempVertices = nextOfTempVertices->next; //Iteration.		
		}
		tempVertices = tempVertices->next;	//Iteration.
	}
	
	printSimilarScanPaths(head); //Print similarity score.
}


struct graphHead* readData(char *fileName)
{
	FILE *myFile;
	myFile=fopen(fileName,"r");
	
	if(myFile == NULL)		//In case of can't be read!
	{
		printf("Failed While Reading File!\n");
		exit(0);
	}
	
	struct graphHead *head=(struct graphHead *) malloc(sizeof(struct graphHead));
	head->first= NULL;
	head->count= 0;
	
	char readedData[MAX];
	while(!feof(myFile)) //I'm reading each line of *.txt file
	{
		fscanf(myFile,"%s\n",readedData);
		createVertice(head,readedData); //While I'm reading each line I'm also creating each verticeses.
	}
	
	fclose(myFile);
	return head; //Return type is head of graph.
}


void createVertice(struct graphHead *head,char *scanPath)
{
	struct graphVertices *vertices=(struct graphVertices*)malloc(sizeof(struct graphVertices));
	vertices->next=NULL; 					//Initialize next of vertice with null.
	strcpy(vertices->scanPath,scanPath);	//Copy the string into new vertice.
	vertices->firstEdges=NULL;				//Initialize firstedge of vertice with null.
	head->count++;					       //increment #of vertice
	vertices->verticeIndex=head->count;
	
	if(head->first == NULL)
		head->first=vertices;
	else
	{
		struct graphVertices *temp =head->first;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=vertices;
	}
}

int createEdge(struct graphHead *head,char *fromScanPath,char *toScanPath)
{
	struct graphEdges *edges = (struct graphEdges *) malloc(sizeof(struct graphEdges));
	struct graphVertices *firstVerticeIndexs = NULL;
	struct graphVertices *secondVerticeIndexs = NULL;
	
	struct graphEdges *tempEdges = NULL;
	firstVerticeIndexs=head->first;
	
	while(firstVerticeIndexs!=NULL && strcmp(firstVerticeIndexs->scanPath,fromScanPath)!=0)
		firstVerticeIndexs = firstVerticeIndexs->next;
		
	if(firstVerticeIndexs == NULL)
		return -2;
	
	secondVerticeIndexs = head->first;
	
	while(secondVerticeIndexs!=NULL && strcmp(secondVerticeIndexs->scanPath,toScanPath)!=0)
		secondVerticeIndexs = secondVerticeIndexs->next;
		
	if(secondVerticeIndexs == NULL)
		return -3;
	
	edges->destination = secondVerticeIndexs;
	edges->weight=similarityScore(fromScanPath,toScanPath);
	
	
	tempEdges=firstVerticeIndexs->firstEdges;
	if(tempEdges==NULL)
	{
		firstVerticeIndexs->firstEdges=edges;
		edges->next=NULL;
		return 1;	
	}
	else
	{
		while(tempEdges->next != NULL)
			tempEdges = tempEdges->next;
				
		tempEdges->next=edges;
		edges->next=NULL;
		return 1;
	}	
}

void printSimilarScanPaths(struct graphHead *head)
{
	struct graphVertices *tempVertices = NULL; //To avoid from address losing.
	struct graphEdges * tempEdges = NULL;
	struct edgeRecord *record,*headRecord=NULL; 
	
	
	tempVertices = head->first;
	
	while(tempVertices != NULL)	//Vertices
	{
	
		tempEdges = tempVertices->firstEdges;
		while(tempEdges != NULL)
		{
			record = (struct edgeRecord* )malloc(sizeof(struct edgeRecord));		//While I'm iterating all edges I'm also creating edge records as a linked list to sort the data properly.
			record->firstVerticeIndex=tempVertices->verticeIndex;						   
			record->secondVerticeIndex=tempEdges->destination->verticeIndex;
			record->similarityScore=tempEdges->weight;
			record->next=headRecord;
			headRecord=record;
			
			//printf("Scanpath %d and Scanpath %d - Similarity: %0.1lf\n",tempVertices->verticeIndex,tempEdges->destination->verticeIndex,tempEdges->weight);
		
			tempEdges = tempEdges->next;
		}
		tempVertices = tempVertices->next;
	}
	
	struct edgeRecord *temp=record->next;

	
	sortEdgeRecord(&record);	//After I'm creating all edges info, I'm sorting these data in descending order.
	
	while(record!=NULL) //Print part!
	{
	
		printf("Scanpath %d and Scanpath %d - Similarity: %0.1lf%\n",record->firstVerticeIndex,record->secondVerticeIndex,record->similarityScore);
		record=record->next;
	}
}

void sortEdgeRecord(struct edgeRecord **head) //Bubble sort of edgeRecord.
{											  //Typical bubblesort code.
    
    struct edgeRecord *tempHead=*head;					//tempHead=head;
    struct edgeRecord *nextOfTempHead=tempHead->next;   //nextOfTempHead=head->next;

    while(tempHead!=NULL)
    {
        while(nextOfTempHead!=NULL)
        { 
            if(nextOfTempHead->similarityScore > tempHead->similarityScore)
            {
                swapEdgeRecord(tempHead,nextOfTempHead);
            }
            nextOfTempHead=nextOfTempHead->next;
        }
        tempHead=tempHead->next;
    }
}

void swapEdgeRecord(struct edgeRecord *a,struct edgeRecord *b) //In bubble sort, we have to swap 2 Edge Record.
{															  //Typical swap code with pass by reference.
	struct edgeRecord temp;
	
	temp.firstVerticeIndex=a->firstVerticeIndex;
	temp.secondVerticeIndex=a->secondVerticeIndex;
	temp.similarityScore=a->similarityScore;
	
	a->firstVerticeIndex=b->firstVerticeIndex;
	a->secondVerticeIndex=b->secondVerticeIndex;
	a->similarityScore=b->similarityScore;
	
	b->firstVerticeIndex=temp.firstVerticeIndex;
	b->secondVerticeIndex=temp.secondVerticeIndex;
	b->similarityScore=temp.similarityScore;
}

//**Helper Functions**
int max(int x,int y) //Return maximum of two numbers.
{
	int max=x;
	
	if(y>x)
		max=y;
	
	return max;
}

int min(int x,int y,int z) //Returns minimum of three numbers.
{
	int minimum=0;
	
	while(x!=0 && y!=0 && z!=0) //The one that firstly reaches the 0 is minimum.
	{
		x--;
		y--;
		z--;
		minimum++;
	}
	return minimum;	//Returns the number of decrement of minimum number to reach 0.
}

int editDist(char s1[], char s2[], int lengthS1, int lengthS2) //To calculate difference between two string.
{
	if (lengthS1 == 0)	
		return lengthS2;
	if (lengthS2 == 0)
		return lengthS1;
	if (s1[lengthS1 - 1] == s2[lengthS2 - 1])
		return editDist (s1, s2, lengthS1 - 1, lengthS2 - 1);
	
	return 1 + min(editDist(s1, s2, lengthS1, lengthS2 - 1), editDist(s1, s2, lengthS1 - 1, lengthS2), editDist(s1, s2, lengthS1 - 1, lengthS2 - 1));
}

double similarityScore(char *s1,char *s2) //
{
	int lengthS1=strlen(s1);							//Length of string1.
	int lengthS2=strlen(s2);							//Length of string2.
	int d=editDist(s1,s2,lengthS1,lengthS2);		   //Calculating D.
	
	return (100*(1-(1.0*d/(1.0*max(lengthS1,lengthS2))))); //Returns Similarity Score.
}
//Author:Berk Limoncu ID:2243541

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE* output;

struct Grid{
	char** grid;
	int m;
	int n;
};

struct Point{
	int x;
	int y;
};

struct Path { 
    struct Point p;
    struct Path* next; 
}; 
struct PathNode{
	struct Path* path;
    struct Point start;
    struct Point end;
    struct Grid pathGrid;
    int length;
    char character;
    int index;
	struct PathNode* next;
}*Paths;

void AddPath(struct PathNode *path)
{
	path->next=NULL;
	if(Paths==NULL){
		Paths=path;
		Paths->next=NULL;
		return;
	}
	struct PathNode*temp=Paths;
	while(temp!=NULL&&temp->next!=NULL){
		temp=temp->next;
	}
    temp->next = path;
} 
void swapPoint(struct Path *p1, struct Path *p2) 
{ 
	struct Point tmp = p1->p;
	p1->p=p2->p;
	p2->p=tmp;
} 
void sortPoints(struct Path *start) 
{ 
    int isSwapped;
	int i; 
    struct Path *tmp; 
    struct Path *toSort = NULL; 
    if (start == NULL) {
    	return;
	}
    do
    { 
        isSwapped = 0; 
        tmp = start; 
        while (tmp->next != toSort) 
        { 
            if (tmp->p.x > tmp->next->p.x) 
            {  
                swapPoint(tmp, tmp->next); 
                isSwapped = 1; 
            } 
            else if(tmp->p.y>tmp->next->p.y&&tmp->p.x==tmp->next->p.x){
            	swapPoint(tmp, tmp->next); 
                isSwapped = 1; 
			}
            tmp = tmp->next; 
        } 
        toSort = tmp; 
    } 
    while (isSwapped==1); 
} 
void swapPath(struct PathNode *p1, struct PathNode *p2) 
{ 
	char tmpCharacter = p1->character;
	struct Point tmpEnd = p1->end;
	int tmpIndex=p1->index;
	int tmpLength=p1->length;
	struct Path* tmpPath=p1->path;
	struct Grid tmpGrid=p1->pathGrid;
	struct Point tmpStart = p1->start;
	p1->character=p2->character;
	p1->end=p2->end;
	p1->index=p2->index;
	p1->length=p2->length;
	p1->path=p2->path;
	p1->pathGrid=p2->pathGrid;
	p1->start=p2->start;
	p2->character=tmpCharacter;
	p2->end=tmpEnd;
	p2->index=tmpIndex;
	p2->length=tmpLength;
	p2->path=tmpPath;
	p2->pathGrid=tmpGrid;
	p2->start=tmpStart;
} 
void sortPaths(struct PathNode *start,char criteria) 
{ 
    int isSwapped;
	int i; 
    struct PathNode *tmp; 
    struct PathNode *toSort = NULL; 
    if (start == NULL) {
    	return;
	}
    do
    { 
        isSwapped = 0; 
        tmp = start; 
        while (tmp->next != toSort) 
        { 
            if (tmp->length > tmp->next->length||tmp->character==criteria) 
            {  
                swapPath(tmp, tmp->next); 
                isSwapped = 1; 
            } 
            tmp = tmp->next; 
        } 
        toSort = tmp; 
    } 
    while (isSwapped==1); 
} 
void addPoint(struct Path** head, struct Point p)
{ 
    struct Path* newPath = (struct Path*) malloc(sizeof(struct Path)); 
    newPath->p  = p; 
	if((*head)==NULL){
		newPath->next=NULL;
		(*head)=newPath;
		return;
	}
    newPath->next = (*head); 
    (*head)=newPath; 
} 
struct Grid mallocGrid(int m,int n){
	int i,j;
	struct Grid grid;
	grid.grid = (char**)malloc(sizeof(char*)*m);
	grid.m=m;
	grid.n=n;
	for(i=0;i<m;i++){
		grid.grid[i] = (char*)malloc(sizeof(char)*n);
		for(j=0;j<grid.n;j++){
			grid.grid[i][j]=' ';
		}
	}
	return grid;
}
struct Grid matrix(char fname[20]){
	FILE* f = fopen(fname,"r");
	if(!f){
		fprintf(output,"File read error.\n");
		printf("File read error.\n");
		exit(0);
	}
	int m=0,n=0;
	int i=0,j=0;
	char buf;
	while(!feof(f)){
		fscanf(f,"%c",&buf);
		if(buf=='\n'&&!feof(f))m++;
		else if(buf!='\n'&&feof(f))m++;
		else if(m==0&&buf!='\t')n++;
	}
	fclose(f);
	f=fopen(fname,"r");
	struct Grid grid=mallocGrid(m,n);
	i=0,j=0;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			buf = fgetc(f);
			if(buf!='\t'&&buf!='\n'){
				grid.grid[i][j]=buf;
			}
			else j--;
		}
		fgetc(f);
	}
	fclose(f);
	return grid;
}
void printGrid(struct Grid grid){
	int i,j;
	for(i=0;i<grid.m;i++){
		for(j=0;j<grid.n;j++){
			fprintf(output,"%c ",grid.grid[i][j]);
			printf("%c ",grid.grid[i][j]);
		}
		fprintf(output,"\n");
		printf("\n");
	}
}

void resetGrid(struct Grid g){
	int i,j;
	for(i=0;i<g.m;i++){
		for(j=0;j<g.n;j++){
			g.grid[i][j]='*';
		}
	}
}
 
void printPathInfo(struct Grid grid,struct PathNode* path){
	printf("--Path Info--\n");
	fprintf(output,"--Path Info--\n");
		printf("Starts at: %d %d '%c'\n",path->start.x,path->start.y,grid.grid[path->start.x][path->start.y]);
		printf("Ends at: %d %d '%c'\n",path->end.x,path->end.y,grid.grid[path->end.x][path->end.y]);
		printf("ID: %c%d\n",path->character,path->index);
		printf("Length: %d\n",path->length);
		printf("Path:\n");
		
		fprintf(output,"Starts at: %d %d '%c'\n",path->start.x,path->start.y,grid.grid[path->start.x][path->start.y]);
		fprintf(output,"Ends at: %d %d '%c'\n",path->end.x,path->end.y,grid.grid[path->end.x][path->end.y]);
		fprintf(output,"ID: %c%d\n",path->character,path->index);
		fprintf(output,"Length: %d\n",path->length);
		fprintf(output,"Path:\n");
		struct Path* pathNode=path->path;
		while (pathNode != NULL) 
		{ 
		    printf("(%d,%d)", pathNode->p.x,pathNode->p.y); 
		    fprintf(output,"(%d,%d)", pathNode->p.x,pathNode->p.y); 
		    path->pathGrid.grid[pathNode->p.x][pathNode->p.y]='#';
		    pathNode = pathNode->next;
		} 
	  	printf("\n");
	  	fprintf(output,"\n");
	    printGrid(path->pathGrid);
}
void printAllPaths(struct Grid grid){
	struct PathNode* temp = Paths;
	while(temp!=NULL){
		printPathInfo(grid,temp);
		temp=temp->next;
	}
}
void findPathsBetween(struct Grid grid, int i, int j,struct Point source,struct Point target,struct Grid visited,struct Path* pathList)
{
    if ((i == target.x && j == target.y))
    {
    	struct PathNode* newPath=(struct PathNode*)malloc(sizeof(struct PathNode));
    	newPath->start=source;
    	newPath->end=target;
    	newPath->length=0;
    	newPath->pathGrid=mallocGrid(grid.m,grid.n);
    	newPath->path=pathList;
    	sortPoints(newPath->path);
    	newPath->character=grid.grid[source.x][source.y];
    	int k,l,count=0;
		for(k=0;k<grid.m;k++){
			for(l=0;l<grid.n;l++){
				if(grid.grid[k][l]==newPath->character){
					count++;
					if(k==source.x&&l==source.y){
						newPath->index=count;
					}
				}
				if(newPath->pathGrid.grid[k][l]!='#'){
					newPath->pathGrid.grid[k][l]=grid.grid[k][l];
				}
			}
		}
    	struct Path* temp = newPath->path;
    	while(temp!=NULL){
    		newPath->length++;
    		newPath->pathGrid.grid[temp->p.x][temp->p.y]='#';
    		temp=temp->next;
		}
    	AddPath(newPath);
        return;
    }
    //All eight directions
 	int xDir[8]={0, 0, 1, 1, 1,-1,-1,-1};
 	int yDir[8]={1,-1, 1,-1, 0, 1,-1, 0};
	struct Point p;
	p.x=i;
	p.y=j;
	addPoint(&pathList,p);
    int k;
	for(k=0;k<8;k++){
		      int ti=i+xDir[k];
		      int tj=j+yDir[k];
			if(ti >= 0 && ti < grid.m && tj >= 0 && tj < grid.n&&visited.grid[ti][tj]!='#'){
		        if(grid.grid[ti][tj]!='*'){
			        visited.grid[ti][tj]='#';
			        findPathsBetween(grid, ti,tj,source,target,visited,pathList);
				}
			}
	}
	visited.grid[i][j]='*';
}
void CalculateAllPaths(struct Grid grid){
	int i,j,entityCount=0;
	struct Point *entities=(struct Point*)malloc(sizeof(struct Point));
	for(i=0;i<grid.m;i++){
		for(j=0;j<grid.n;j++){
			if(grid.grid[i][j]!='*'&&grid.grid[i][j]!=' '){
				struct Point p;
				p.x=i;
				p.y=j;
				entities[entityCount] = p;
				entityCount++;
				entities= realloc(entities,(entityCount+1)*sizeof(struct Point));
			}
		}
	}
	for(i=0;i<entityCount;i++){
		for(j=0;j<entityCount;j++){
			if(i!=j){
				struct Path* pathList=NULL;
				struct Grid visited = mallocGrid(grid.m,grid.n);
				findPathsBetween(grid, entities[i].x,entities[i].y,entities[i],entities[j],visited,pathList);
			}
		}
	}
	free(entities);
}

struct Point findPointOfEntity(struct Grid gr,char character,int index){
	int i,j,count=0;
	struct Point p;
	for(i=0;i<gr.m;i++){
		for(j=0;j<gr.n;j++){
			if(gr.grid[i][j]==character){
				count++;
				if(count==index){
					p.x=i;
					p.y=j;
				}
			}
		}
	}
	return p;
}

void allPossiblePathsBetweenTwoEntities(struct Grid gr){
	char source,target;
	int sourceNo,targetNo,length;
	printf("Enter entities and max path length. Ex: h1 p2 5 :\n");
	fprintf(output,"Enter entities and max path length. Ex: h1 p2 5 :\n");
	scanf(" %c%d %c%d %d",&source,&sourceNo,&target,&targetNo,&length);
	fprintf(output,"%c%d %c%d %d\n",source,sourceNo,target,targetNo,length);
	int i,j;
	struct Point sourcePoint=findPointOfEntity(gr,source,sourceNo);
	struct Point targetPoint=findPointOfEntity(gr,target,targetNo);
	struct PathNode* temp = Paths;
	int printed=0;
	int pathCount=0;
		struct PathNode* tempP = Paths;
		while(tempP!=NULL){
			if(tempP->index==sourceNo&&tempP->character==source){
				if(targetPoint.x==tempP->end.x&&targetPoint.y==tempP->end.y){
					printed=1;
					if(length==-1||tempP->length>=length){
						printPathInfo(gr,tempP);
					}
				}
			}
			tempP=tempP->next;
		}
	if(printed==0){
		printf("Invalid entity info.\n");
		fprintf(output,"Invalid entity info.\n");
	}
}

void ShortestPathBetweenTwoEntities(struct Grid gr){
	char source,target;
	int sourceNo,targetNo;
	printf("Enter entities. Ex: h1 p2 :\n");
	fprintf(output,"Enter entities. Ex: h1 p2 :\n");
	scanf(" %c%d %c%d",&source,&sourceNo,&target,&targetNo);
	fprintf(output,"%c%d %c%d\n",source,sourceNo,target,targetNo);
	struct PathNode* shortest=NULL;
	struct PathNode* temp = Paths;
	struct Point sourcePoint= findPointOfEntity(gr,source,sourceNo);
	struct Point targetPoint= findPointOfEntity(gr,target,targetNo);
	while(temp!=NULL){
		if(sourcePoint.x==temp->start.x&&sourcePoint.y==temp->start.y&&targetPoint.x==temp->end.x&&targetPoint.y==temp->end.y){
			if(shortest==NULL||temp->length<shortest->length){
				shortest=temp;
			}
		}
		temp=temp->next;
	}
	if(shortest==NULL)
	{
		printf("Invalid entity info.\n");
		fprintf(output,"Invalid entity info.\n");
	}
	else{
		printf("Shortest path found:\n");
		fprintf(output,"Shortest path found:\n");
		printPathInfo(gr,shortest);
	}
}
int getIndex(struct Grid gr,struct Point p,char target){
	int i,j,cnt=0;
	for(i=0;i<gr.m;i++){
		for(j=0;j<gr.n;j++){
			if(gr.grid[i][j]==target){
				cnt++;
			}
			if(p.x==i&&p.y==j){
				break;
			}
		}
	}
	return cnt;
}

void printPossibleTargetAndSources(struct Grid gr){
	int length;
	printf("Enter max length:");
	fprintf(output,"Enter max length:");
	scanf("%d",&length);
	fprintf(output,"%d\n",length);
	struct PathNode* outer = Paths;
	while(outer!=NULL){
		char target=gr.grid[outer->end.x][outer->end.y];
		int targetIndex=getIndex(gr,outer->end,target);
		if(outer->character=='@'&&target!='@'&&(outer->length<=length||length==-1)){
			printf("%c%d->\t%c%d\t%d\n",outer->character,outer->index,target,targetIndex,outer->length);
			fprintf(output,"%c%d->\t%c%d\t%d\n",outer->character,outer->index,target,targetIndex,outer->length);
		}
		outer=outer->next;
	}
}

void printPossibleTargetAndSourcesSorted(struct Grid gr){
	int length;
	char entity;
	printf("Enter max length:");
	fprintf(output,"Enter max length:");
	scanf("%d",&length);
	printf("Enter entity order ex(h):\n");
	fprintf(output,"%d\nEnter entity order ex(h):\n",length);
	scanf(" %c",&entity);
	fprintf(output,"%c\n",entity);
	sortPaths(Paths,entity);
	struct PathNode* outer = Paths;
	while(outer!=NULL){
		char target=gr.grid[outer->end.x][outer->end.y];
		int targetIndex=getIndex(gr,outer->end,target);
		if(outer->character=='@'&&target!='@'&&(outer->length<=length||length==-1)){
			printf("%c%d->\t%c%d\t%d\n",outer->character,outer->index,target,targetIndex,outer->length);
			fprintf(output,"%c%d->\t%c%d\t%d\n",outer->character,outer->index,target,targetIndex,outer->length);
		}
		outer=outer->next;
	}
}

int main(int argc, char *argv[]) {
	output=fopen("output.txt","w");
	char* fname = (char*)malloc(sizeof(char)*50);
	printf("Enter file name:");
	scanf("%s",fname);
	fprintf(output,"Enter file name:\n%s",fname);
	struct Grid grid = matrix(fname);
	Paths=NULL;
	CalculateAllPaths(grid);
	int choice;
	while(1){
		fprintf(output,"0-Print Grid\n1-Print all paths\n2-Print all possible paths between two entities\n3-Print shortest path between two entities\n4-Print possible target&source types\n5-Print possible target&source types sorted\n6-Exit\n>");
		printf("0-Print Grid\n1-Print all paths\n2-Print all possible paths between two entities\n3-Print shortest path between two entities\n4-Print possible target&source types\n5-Print possible target&source types sorted\n6-Exit\n>");
		scanf("%d",&choice);
		fprintf(output,"%d\n",choice);
		if(choice==0){
			printGrid(grid);
		}
		else if(choice==1){
			printAllPaths(grid);
		}
		else if(choice==2){
			allPossiblePathsBetweenTwoEntities(grid);
		}
		else if(choice==3){
			ShortestPathBetweenTwoEntities(grid);
		}
		else if(choice==4){
			printPossibleTargetAndSources(grid);
		}
		else if(choice==5){
			printPossibleTargetAndSourcesSorted(grid);
		}
		else if(choice==6){
			break;
		}
		else{
			fprintf(output,"Invalid choice.\n");
			printf("Invalid choice.\n");
		}
	}
	fclose(output);
	return 0;
}

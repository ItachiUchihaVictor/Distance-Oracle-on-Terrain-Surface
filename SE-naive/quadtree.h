#include<stdio.h>
#include<stdlib.h>
//Removed windows-specific header and functions
//-------------------------------------
// STRUCTURES
//-------------------------------------
#define poi 60 
#define upper_poi 4000
#define INF 100000000.0
struct Point
{
double x;
double y;
};
struct Node
{
double posX;
double posY;
double width;
double height;
int size;
std::vector<int> indexArray;
//int indexArray[upper_poi];
Node *child[4];         //Changed to Node *child[4] rather than Node ** child[4]
Node *parent;
std::list<bool> mcode;
};

std::vector<Node*> nodevector;
struct NodePair
{
    Node *node1;
    Node *node2;
    double distance;
};
std::list<NodePair*> quadpairs;

std::vector<NodePair *> pairvector;
bool compair_quadpair(NodePair *x, NodePair *y){
    std::list<bool>::iterator itex=x->node1->mcode.begin();
    std::list<bool>::iterator itey=y->node1->mcode.begin();
    while(itex!=x->node1->mcode.end()&&itey!=y->node1->mcode.end()){
        if(*itex<*itey)return true;
        if(*itex>*itey)return false;
        itex++;
        itey++;
    }
    if(x->node1->mcode.size()<y->node1->mcode.size())return true;
    if(x->node1->mcode.size()>y->node1->mcode.size())return false;
    itex=x->node2->mcode.begin();
    itey=y->node2->mcode.begin();
    while(itex!=x->node2->mcode.end()&&itey!=y->node2->mcode.end()){
        if(*itex<*itey)return true;
        if(*itex>*itey)return false;
        itex++;
        itey++;
    }
    if(x->node2->mcode.size()<y->node2->mcode.size())return true;
    if(x->node2->mcode.size()>y->node2->mcode.size())return false;

}
//-------------------------------------
// DEFINITIONS
//-------------------------------------
void BuildQuadTree(Node *n, geodesic::Mesh &mesh);
void PrintQuadTree(Node *n, int depth = 0);
void DeleteQuadTree(Node *n);
Node *BuildNode(Node *n, Node  *nParent, int index, geodesic::Mesh &mesh);
//-------------------------------------
// FUNCTIONS
//-------------------------------------
void setnode(Node *xy,double x, double y, double w, double h)
{
int i;
xy->posX = x;
xy->posY = y;
xy->width= w;
xy->height= h;
//Initialises child-nodes to NULL - better safe than sorry
for (int i = 0; i < 4; i++)
xy->child[i] = NULL;
}

int randn(int k)
{
int a;
a=rand()%k;
return a;
}

//-------------------------------------
// BUILD QUAD TREE
//-------------------------------------
void BuildQuadTree(Node *n, geodesic::Mesh &mesh)
{
Node * nodeIn = new Node;   //Initialised node
int points = n->size;
if(points > 1)
{
    for(int k =0; k < 4; k++){
        n->child[k] = new Node;      //Initialised node
        n->child[k]->parent=n;
        n->child[k]->mcode.clear();
        n->child[k]->mcode.assign(n->mcode.begin(),n->mcode.end());
        nodeIn = BuildNode(n->child[k], n, k, mesh);
        BuildQuadTree(nodeIn, mesh);
    }
}
else{
    if(n->size==1)nodevector.push_back(n);
    for(int k=0;k<4;k++){
        n->child[k]=NULL;
    }
}
}
//-------------------------------------
// PRINT QUAD TREE
//-------------------------------------
void PrintQuadTree(Node *n, int depth)
{
for (int i = 0; i < depth; i++)
printf(" ");
if (n->child[0] == NULL)
{
int points = n->size;
printf("%d p: %f %f\n", points, n->posX, n->posY);
return;
}
else if (n->child[0] != NULL)
{
printf("Node (%d):\n", n->size);
for (int i = 0; i < 4; i++)
PrintQuadTree(n->child[i], depth + 1);
return;
}
}
void PrintQuadPair(){
    FILE *fp=fopen("quadpairs.txt","w");
    for(std::list<NodePair*>::iterator gite=quadpairs.begin();gite!=quadpairs.end();gite++){
        fprintf(fp,"%d %d\n",(*gite)->node1->indexArray[0], (*gite)->node1->size);
        for(std::list<bool>::iterator mite=(*gite)->node1->mcode.begin();mite!=(*gite)->node1->mcode.end();mite++)
            fprintf(fp,"%d ",(*mite));
        fprintf(fp,"\n");
        fprintf(fp,"%d %d\n",(*gite)->node2->indexArray[0], (*gite)->node2->size);
        for(std::list<bool>::iterator mite=(*gite)->node2->mcode.begin();mite!=(*gite)->node2->mcode.end();mite++)
            fprintf(fp,"%d ",(*mite));
        fprintf(fp,"\n");
    }
    fclose(fp);
}
//-------------------------------------
// DELETE QUAD TREE
//-------------------------------------
void DeleteQuadTree(Node *n)
{
if (n->child[0] == NULL)
{
delete n;
return;
}
else if (n->child[0] != NULL)
{
for (int i = 0; i < 4; i++)
DeleteQuadTree(n->child[i]);
return;
}
}
//-------------------------------------
// BUILD NODE
//-------------------------------------
Node *BuildNode(Node *n, Node *nParent, int index, geodesic::Mesh &mesh)
{
int numParentPoints, i,j = 0;
// 1) Creates the bounding box for the node
// 2) Determines which points lie within the box
/*
Position of the child node, based on index (0-3), is determined in this order:
| 1 | 0 |
| 2 | 3 |
*/
setnode(n, 0.0, 0.0, 0.0, 0.0);
switch(index)
{
case 0: // NE
n->posX = nParent->posX+nParent->width/2;
n->posY = nParent->posY+nParent->height/2;
n->mcode.push_back(0);
n->mcode.push_back(0);
break;
case 1: // NW
n->posX = nParent->posX;
n->posY = nParent->posY+nParent->height/2;
n->mcode.push_back(0);
n->mcode.push_back(1);
break;
case 2: // SW
n->posX = nParent->posX;
n->posY = nParent->posY;
n->mcode.push_back(1);
n->mcode.push_back(0);
break;
case 3: // SE
n->posX = nParent->posX+nParent->width/2;
n->posY = nParent->posY;
n->mcode.push_back(1);
n->mcode.push_back(1);
break;
}
// Width and height of the child node is simply 1/2 of the parent node's width and height
n->width = nParent->width/2;
n->height = nParent->height/2;
// The points within the child node are also based on the index, similiarily to the position
numParentPoints = nParent->size;
switch(index)
{
case 0: // NE
for(i = 0; i < numParentPoints; i++)
{
// Check all parent points and determine if it is in the top right quadrant
if(mesh.vertices()[nParent->indexArray[i]].getx()<INF && mesh.vertices()[nParent->indexArray[i]].getx() > nParent->posX+nParent->width/2 && mesh.vertices()[nParent->indexArray[i]].gety() > nParent->posY + nParent->height/2 && mesh.vertices()[nParent->indexArray[i]].getx() <= nParent->posX + nParent->width && mesh.vertices()[nParent->indexArray[i]].gety() <= nParent->posY + nParent-> height)
{
// Add the point to the child node's point array
n->indexArray.push_back( nParent ->indexArray[i]);
j++;
}
n->size=j;
}
break;
case 1: // NW
for(i = 0; i < numParentPoints; i++)
{
// Check all parent points and determine if it is in the top left quadrant
if(mesh.vertices()[nParent->indexArray[i]].getx()<INF && mesh.vertices()[nParent->indexArray[i]].getx() > nParent->posX && mesh.vertices()[nParent->indexArray[i]].gety() > nParent->posY+ nParent-> height/2 && mesh.vertices()[nParent->indexArray[i]].getx() <= nParent->posX + nParent->width/2 && mesh.vertices()[nParent->indexArray[i]].gety() <= nParent->posY + nParent->height)
{
// Add the point to the child node's point array
n->indexArray.push_back( nParent ->indexArray[i]);
j++;
}
n->size=j;
}
break;
case 2: // SW
for(i = 0; i < numParentPoints; i++)
{
// Check all parent points and determine if it is in the bottom left quadrant
if(mesh.vertices()[nParent->indexArray[i]].getx()<INF && mesh.vertices()[nParent->indexArray[i]].getx() > nParent->posX && mesh.vertices()[nParent->indexArray[i]].gety() > nParent->posY && mesh.vertices()[nParent->indexArray[i]].getx() <= nParent->posX + nParent->width/2 && mesh.vertices()[nParent->indexArray[i]].gety() <= nParent->posY + nParent->height/2)
{  
// Add the point to the child node's point array
n->indexArray.push_back( nParent ->indexArray[i]);
j++;
}
n->size=j;
}
break;
case 3: // SE
for(i = 0; i < numParentPoints; i++)
{
// Check all parent points and determine if it is in the bottom right quadrant
if(mesh.vertices()[nParent->indexArray[i]].getx()<INF && mesh.vertices()[nParent->indexArray[i]].getx() > nParent->posX +  nParent->width/2 && mesh.vertices()[nParent->indexArray[i]].gety() > nParent->posY && mesh.vertices()[nParent->indexArray[i]].getx() <= nParent->posX + nParent->width && mesh.vertices()[nParent->indexArray[i]].gety() <= nParent->posY + nParent->height/2)
{
// Add the point to the child node's point array
n->indexArray.push_back( nParent ->indexArray[i]);
j++;
}
n->size=j;
}
break;
}
return n;
}


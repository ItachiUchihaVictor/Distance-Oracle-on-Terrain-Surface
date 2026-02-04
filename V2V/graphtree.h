#include<stdio.h>
#include<stdlib.h>
#include<list>
#include "geodesic_algorithm_exact.h"
#include "quadtree.h"
//-------------------------------------
// STRUCTURES
//-------------------------------------
class GraphNode
{
    public:
        double maxdis;
        int index;
        int size;
        std::vector<int> indexArray;
        std::list<bool> mcode;
        GraphNode *child1;
        GraphNode *child2;
        GraphNode *parent;
        ~GraphNode(){
        };
};

class GraphPair
{
    public:
        GraphNode *node1;
        GraphNode *node2;
        double distance;
};
std::vector<GraphNode*> graphnodevector;
std::list<GraphPair*> graphpairs;
std::vector<GraphPair*> graphpairsvector;
bool compair_graphpair(GraphPair *x, GraphPair *y){
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
void BuildGraphTree(GraphNode &n, geodesic::Mesh &mesh, geodesic::GeodesicAlgorithmExact &algorithm);
void PrintGraphTree(GraphNode &n, int depth = 0);
void DeleteGraphTree(GraphNode &n);

//-------------------------------------
// FUNCTIONS
//-------------------------------------
//-------------------------------------
// BUILD TREE
//-------------------------------------
void BuildGraphTree(GraphNode &n, geodesic::Mesh &mesh, geodesic::GeodesicAlgorithmExact &algorithm )
{
    if(&n==NULL||n.size==0)return;
    if(n.size==1){
        n.index=n.indexArray[0];
        n.maxdis=0;
        graphnodevector.push_back(&n);
        return;
    }
    int miny_index=n.indexArray[0];
    for(int i=0;i<n.size;i++){
        if(mesh.vertices()[n.indexArray[i]].gety()<mesh.vertices()[miny_index].gety()){
            miny_index=n.indexArray[i];
        }        
    }
    n.index=miny_index;
	geodesic::SurfacePoint source(&mesh.vertices()[miny_index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);
	double const distance_limit = 0;
	geodesic::SurfacePoint target(&mesh.vertices()[n.indexArray[0]]); 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);	
    for(int i=1;i<n.size;i++){
        stop_points.push_back(geodesic::SurfacePoint(&mesh.vertices()[n.indexArray[i]]));
    }
	algorithm.propagate(all_sources, distance_limit, &stop_points);	//"propagate(all_sources)" is also fine, but take more time because covers the whole mesh
    n.maxdis= 0;
    double distance;
    for(int i=0;i<n.size;i++){
        geodesic::SurfacePoint p(&mesh.vertices()[n.indexArray[i]]);
        algorithm.best_source(p,distance);
        n.maxdis=std::max(distance,n.maxdis);
    }
    std::cout<<"n.size n.maxdis n.index:"<<n.size<<" "<<n.maxdis<<" "<<n.index<<std::endl;
    GraphNode *child1=new GraphNode(); 
    GraphNode *child2=new GraphNode();
    child1->parent=&n;
    child2->parent=&n;
    child1->mcode.assign(n.mcode.begin(),n.mcode.end());
    child2->mcode.assign(n.mcode.begin(),n.mcode.end());
    child1->mcode.push_back(0);
    child2->mcode.push_back(1);

    n.child1=child1;
    n.child2=child2;
    int j=0;
    int k=0;
    for(int i=0;i<n.size;i++){
        geodesic::SurfacePoint p(&mesh.vertices()[n.indexArray[i]]);
        algorithm.best_source(p,distance);
        std::cout<<"maxdis dis:"<<n.maxdis<<" "<<distance<<std::endl;
        if(distance<n.maxdis/2.0){
            child1->indexArray.push_back(n.indexArray[i]);
            j++;
        }
        else{
            child2->indexArray.push_back(n.indexArray[i]);
            k++;
        }
    }  
    child1->size=j;
    child2->size=k;
    std::cout<<"1: size "<<child1->size<<std::endl;
    std::cout<<"2: size "<<child2->size<<std::endl;
    if(j==0){
        DeleteGraphTree(*child1);
        child1==NULL;
    }
    else{
        BuildGraphTree(*n.child1,mesh,algorithm);
    }
    if(k==0){
        DeleteGraphTree(*child2);
        child2=NULL;
    }
    else{
        BuildGraphTree(*n.child2,mesh,algorithm);
    }
}
//-------------------------------------
// PRINT TREE
//-------------------------------------
void PrintGraphTree(GraphNode &n, int depth)
{
    if(&n==NULL||n.size==0)return;
    for (int i = 0; i < depth; i++)
        printf(" ");
        printf("index: %d maxdis: %f\n", n.index,n.maxdis);
    if (n.size>1)
    {
            PrintGraphTree(*n.child1, depth + 1);
            PrintGraphTree(*n.child2, depth + 1);
    }
    return;
}

void PrintGraphPair(){
    FILE *fp=fopen("graphpairs.txt","w");
    for(std::list<GraphPair*>::iterator gite=graphpairs.begin();gite!=graphpairs.end();gite++){
        fprintf(fp,"%d %d\n",(*gite)->node1->index, (*gite)->node1->size);
        for(std::list<bool>::iterator mite=(*gite)->node1->mcode.begin();mite!=(*gite)->node1->mcode.end();mite++)
            fprintf(fp,"%d ",(*mite));
        fprintf(fp,"\n");
        fprintf(fp,"%d %d\n",(*gite)->node2->index, (*gite)->node2->size);
        for(std::list<bool>::iterator mite=(*gite)->node2->mcode.begin();mite!=(*gite)->node2->mcode.end();mite++)
            fprintf(fp,"%d ",(*mite));
        fprintf(fp,"\n");
    }
    fclose(fp);
}
//-------------------------------------
// DELETE QUAD TREE
//-------------------------------------
void DeleteGraphTree(GraphNode &n)
{
    if (n.size==0||1)
    {
        delete &n;
        return;
    }
    else
    {
        DeleteGraphTree(*n.child1);
        return;
    }
}

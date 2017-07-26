#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<cmath>
#include "geodesic_algorithm_exact.h"
double s=200;
int pairs=0;
int remainedPOI;
std::vector<double> points;	
std::vector<unsigned> faces;
geodesic::Mesh mesh;
//-------------------------------------
// STRUCTURES
//-------------------------------------

class GeoPair
{
    public:
        GeoNode *node1;
        GeoNode *node2;
        float distance;
};

double distance_max;
std::list<GeoPair*> geopairs;
std::vector<GeoPair*> geopairsvector;
std::vector<GeoNode*> geonodevector;
/*
bool compair_geopair(GeoPair *x, GeoPair *y){
    std::list<int>::iterator itex=x->node1->mcode.begin();
    std::list<int>::iterator itey=y->node1->mcode.begin();
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
*/
bool compair_geonode(GeoNode *x, GeoNode *y){
    if(x->index<y->index)return true;
    return false;
}
bool compair_geoconnect(GeoConnect *x, GeoConnect *y){
    if(x->index<y->index)return true;
    return false;
}
//-------------------------------------
// DEFINITIONS
//-------------------------------------
void BuildGeoTree(GeoNode &n, int POI, stx::btree<int, GeoNode*> &bplustree, geodesic::GeodesicAlgorithmExact &algorithm);
void BuildLevel(int depth, stx::btree<int, GeoNode*> &bplustree, geodesic::GeodesicAlgorithmExact &algorithm);
void PrintGeoTree(GeoNode &n, int depth = 0);
void DeleteGeoTree(GeoNode &n);

GeoNode &BuildGeoNode(GeoNode &n, GeoNode &nParent, int index);
//-------------------------------------
// FUNCTIONS
//-------------------------------------
//-------------------------------------
// BUILD TREE
//-------------------------------------
void BuildGeoTree(GeoNode &n, int POI, stx::btree<int, GeoNode*> &bplustree, geodesic::GeodesicAlgorithmExact &algorithm )
{
    int depth=0;
    remainedPOI=POI;
    bplusparenttree.clear();
    bplusparenttree.insert(n.index, &n);
    distance_max=n.radius;

    while(bplusparenttree.size()!=POI){
        remainedPOI=bplustree.size();
        if(remainedPOI==0)break;
        std::cout<<"depth: "<<depth<<" ParentList size: "<<bplusparenttree.size()<<" remianed POI: "<<remainedPOI<<std::endl;
        BuildLevel(depth, bplustree, algorithm);
        depth++;
    }

    for(stx::btree<int, GeoNode*>::iterator ite=bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
        (*((*ite).second)).radius=0;
        geonodevector.push_back((*ite).second);
    }
/*    std::cout<<"geonodevector: ";
    for(std::vector<GeoNode*>::iterator vite=geonodevector.begin();vite!=geonodevector.end();vite++){
        std::cout<<(*vite)->index<<" ";
    }
    std::cout<<std::endl;*/
}
/*
void calculate_size(GeoNode &n){
    if(n.children.empty()){
        n.size=1;
        return;
    }
    n.size=0;
        for (std::list<GeoNode*>::iterator ite = n.children.begin(); ite != n.children.end(); ite++){
            calculate_size((**ite));
            n.size=n.size+(**ite).size;
        }        
}
*/
//-------------------------------------
// PRINT TREE
//-------------------------------------
void PrintGeoTree(GeoNode &n, int depth)
{
    for (int i = 0; i < depth; i++)
        printf(" ");
 //       printf("index: %d radius: %f size: %d\n", n.index,n.radius,n.size);
 /*   if(!n.connect.empty()){
        for(std::vector<GeoConnect*>::iterator ite=n.connect.begin();ite!=n.connect.end();ite++){
            for (int i = 0; i < depth; i++)
                printf(" ");
            std::cout<<"index distance:"<<(*ite)->index<<" "<<(*ite)->dist<<std::endl;
        }
    }*/
    if (!n.children.empty())
    {
        for (std::list<GeoNode*>::iterator ite = n.children.begin(); ite != n.children.end(); ite++)
            PrintGeoTree((**ite), depth + 1);
    }
    return;
}

void PrintGeoPair(){
    FILE *fp=fopen("geopairs.txt","w");
    for(std::list<GeoPair*>::iterator gite=geopairs.begin();gite!=geopairs.end();gite++){
     //   fprintf(fp,"%d %d %f\n",(*gite)->node1->index, (*gite)->node1->size, (*gite)->node1->radius);
      //  for(std::list<int>::iterator mite=(*gite)->node1->mcode.begin();mite!=(*gite)->node1->mcode.end();mite++)
     //       fprintf(fp,"%d ",(*mite));
     //   fprintf(fp,"\n");
       // fprintf(fp,"%d %d %f\n",(*gite)->node2->index, (*gite)->node2->size, (*gite)->node2->radius);
     //   for(std::list<int>::iterator mite=(*gite)->node2->mcode.begin();mite!=(*gite)->node2->mcode.end();mite++)
   //         fprintf(fp,"%d ",(*mite));
    //    fprintf(fp,"\n");
        fprintf(fp,"index %d index %d distance %f\n", (*gite)->node1->index, (*gite)->node2->index, (*gite)->distance);
    }
    fclose(fp);
}
//-------------------------------------
// DELETE QUAD TREE
//-------------------------------------
void DeleteGeoTree(GeoNode &n)
{
    if (n.children.empty())
    {
        delete &n;
        return;
    }
    else
    {
        for (std::list<GeoNode*>::iterator ite = n.children.begin(); ite != n.children.end(); ite++)
            DeleteGeoTree(**ite);
        return;
    }
}
//-------------------------------------
// BUILD LEVEL 
//-------------------------------------
void BuildLevel(int depth, stx::btree<int, GeoNode*> &bplustree, geodesic::GeodesicAlgorithmExact &algorithm){
    tem_tree= stx::btree<int, GeoNode*>(bplustree);
    std::vector<std::pair<int, GeoNode*> > currentnodes;
    currentnodes.clear();
    for(stx::btree<int, GeoNode*>::iterator ite= bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
         GeoNode *n= new GeoNode(*(*ite).second);
         n->setparent((*ite).second);
         n->radius=((*ite).second)->radius/2.0;
         bplustree.erase((*ite).first);
         tem_tree.erase((*ite).first);
         std::pair<int, GeoNode*> m(n->index,n);
         currentnodes.push_back(m);
         if(n->parent->chain||n->parent->children.size()==1){
             n->chain=true;
             continue;
         }
	     geodesic::SurfacePoint source(&mesh.vertices()[n->index]);
	     std::vector<geodesic::SurfacePoint> all_sources(1,source);
	     double const distance_limit = (*bplusparenttree.begin()).second->radius;
         //std::cout<<"distance_limit: "<<distance_limit<<std::endl;
	     algorithm.range(all_sources, distance_limit/2.0*1.01);
	  /*   algorithm.propagate(all_sources, distance_limit/2.0*1.01);
         stx::btree<int,GeoNode*>::iterator bite;
         for(bite=tem_tree.begin();bite!=tem_tree.end();){
            double distance;
            int tem=(*bite).first;
            geodesic::SurfacePoint p(&mesh.vertices()[(*bite).first]);
            algorithm.best_source(p,distance);
         //   std::cout<<"distance: "<<distance<<std::endl;
            if(distance<=distance_limit/2.0){
                bite++;
                tem_tree.erase(tem);
                n->chain=false;
            }
            else{
                bite++;
            }
        }*/
    }
    

    while(!tem_tree.empty()){
 //       std::cout<<tem_tree.size()<<std::endl;
        GeoNode *n=new GeoNode(*(*(tem_tree.begin())).second);
        std::pair<int, GeoNode*> m(n->index, n);
        currentnodes.push_back(m);

        bplustree.erase(n->index);
        tem_tree.erase(n->index);
	    geodesic::SurfacePoint source(&mesh.vertices()[n->index]);
	    std::vector<geodesic::SurfacePoint> all_sources(1,source);
	    double const distance_limit = (*bplusparenttree.begin()).second->radius;
        //std::cout<<"distance_limit: "<<distance_limit<<std::endl;
	    n->chain=algorithm.range(all_sources, distance_limit*1.01,n );
//        std::cout<<"range query finished"<<std::endl;
        stx::btree<int, GeoNode*>::iterator ite;
/*	    algorithm.propagate(all_sources, distance_limit*1.01);
        std::vector<int> erasearray;
        erasearray.clear();
        if(!tem_tree.empty()){
        for(ite=++tem_tree.begin();;){
            if(ite==tem_tree.end())break;
            double distance;
            geodesic::SurfacePoint p(&mesh.vertices()[(*ite).first]);
            algorithm.best_source(p,distance);
            int tem=(*ite).first;
         //   std::cout<<"distance: "<<distance<<std::endl;
            if(distance<=distance_limit/2.0){
                ite++;
                erasearray.push_back(tem);
                n->chain=false;
            }
            else{
                ite++;
            }
        }
        
        for(std::vector<int>::iterator intite=erasearray.begin();intite!=erasearray.end();intite++){
                tem_tree.erase((*intite));
        }
        erasearray.clear();
        }
        GeoNode* node_pointer;
        for(ite= bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
            double distance,distance_min;
            distance_min=distance_limit;
            geodesic::SurfacePoint p(&mesh.vertices()[(*((*ite).second)).index]);
            algorithm.best_source(p,distance);
            if(distance<=distance_min){
                distance_min=distance;
                node_pointer=(*ite).second;
            }
        }
            n->setparent(node_pointer);*/
            n->radius=distance_limit/2.0;
    }
/*        std::cout<<"big while finished"<<std::endl;
        for(stx::btree<int, GeoNode*>::iterator ite=bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
            int mcode_appen=0;
            for(std::list<GeoNode*>::iterator bite=((*ite).second)->children.begin();bite!=((*ite).second)->children.end();bite++){
                (*bite)->mcode.assign(((*ite).second)->mcode.begin(),((*ite).second)->mcode.end());
                (*bite)->mcode.push_back(mcode_appen);
                mcode_appen++;
            }
        }  
        */
        bplusparenttree.clear();      
        bplusparenttree = stx::btree<int, GeoNode*>(currentnodes.begin(),currentnodes.end());

        for(stx::btree<int, GeoNode*>::iterator ite=bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
         //   if(depth<log10(4*s-4)/log10(2)-2)break;
            geodesic::SurfacePoint p(&mesh.vertices()[(*ite).first]);
          //  std::cout<<"ite index: "<< (**ite).index<< std::endl;
	        std::vector<geodesic::SurfacePoint> all_sources(1,p);
            algorithm.propagate(all_sources,(*(*ite).second).radius*(4*s-4));
            for(stx::btree<int, GeoNode*>::iterator bite=ite;bite!=bplusparenttree.end();bite++){
            if(bite==ite)continue;
            double dist;
            //std::cout<<"  bite index: "<<(**bite).index<< std::endl;
            geodesic::SurfacePoint q(&mesh.vertices()[(*((*bite).second)).index]);
            algorithm.best_source(q,dist);
           // std::cout<<"  dist: "<<dist<<std::endl;
            if(dist<(*(*ite).second).radius*(4*s-4)){
                    GeoConnect *c=new GeoConnect;
                    c->index=((*bite).second)->index;
                    c->dist=dist;
                    (*((*ite).second)).connect.push_back(c);
                }
            }
            if(!(*((*ite).second)).connect.empty())std::sort((*((*ite).second)).connect.begin(), (*((*ite).second)).connect.end(),compair_geoconnect);
        }
}

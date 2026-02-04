#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<cmath>
#include "geodesic_algorithm_exact.h"
double s=20;
int pairs=0;
int remainedPOI;
std::vector<double> points;	
std::vector<unsigned> faces;
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
/*class GridCell
{
    public:
        int index;
//        float posX;
//        float posY;
//        float width;
//        float height;

        void GridCell(int index, float x, float y){
            this->index = index;
            this->posX = x;
            this->posY = y;
        }
        void GridCell(int index){
            this->index = index;
//            this->posX = x;
//            this->posY = y;
        }
};*/
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
	     geodesic::SurfacePoint source(&mymesh.vertices()[n->index]);
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
    
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                cells[i][j] = NULL;
                arr[i][j] = NULL;
            }
        }
        for(stx::btree<int, GeoNode*>::iterator ite=tem_tree.begin();ite!=tem_tree.end();ite++){
            int x = ((mymesh.vertices()[(*ite).first]).getx()-mymesh.m_xmin)
            /(*bplusparenttree.begin()).second->radius/1.414;
            int y = ((mymesh.vertices()[(*ite).first]).gety()-mymesh.m_ymin)
            /(*bplusparenttree.begin()).second->radius/1.414;
            if(cells[x][y]==NULL){
             //   GridCell* newcell = new GridCell(Gridtree.size() );
                const int newID = (*ite).first;
                stx::btree<int, int>* newbtree = new stx::btree<int, int >(newID, 1);
//                (*newbtree).insert2(newID, 1);
                cells[x][y] = newbtree; 
                std::cout << "newbtree size: " << (*newbtree).size() << " ";
 //               std::pair<int, int> newtree
//                Gridtree.insert(stx::btree<int, int> );
            }
            std::cout << " x y cell[x][y] cell[x][y].size: " << x << " " << y << " " << cells[x][y] << " " << (*cells[x][y]).size();
/*            else{
                const int newID = (*ite).first;
                std::cout << "newID x y: " << newID << " " << x << " " << y << std::endl;
//                std::pair<int, int> newpair((*ite).first, 1);
                (*cells[x][y]).insert2(newID, 1);
            }*/
        }
        
        FibonacciHeap<stx::btree<int, int>* > fh;  
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
             if(cells[i][j]!=NULL)   std::cout << " i j cell[x][y] cell[x][y].size: " << i << " " << j << " " << cells[i][j] << " " << (*cells[i][j]).size();
                if(cells[i][j]!=NULL&&(*(cells[i][j])).size()!=0){
                    arr[i][j] = fh.push(cells[i][j]);
                    std::cout << "i j size: " << i << " " << j << " " << (*cells[i][j]).size() << std::endl;
                }else{
                    arr[i][j] = NULL;
                }
            }
        }
        std::cout << "heap size: " << fh.size() << std::endl;
     if(fh.size()>1)  fh.printHeap(); 
       /* 
        cout << fh.top() << endl;
        fh.pop();
        cout << fh.top() << endl;
        fh.decreaseKey(arr[3], 2);
        cout << fh.top() << endl;
        fh.pop();
        cout << fh.top() << endl;

        FibonacciHeap<int>::FibNode * tmp = fh.topNode();
        cout << tmp->getKey() << ' ' << tmp->getID() << endl;

        fh.push(-100, 77);
        tmp = fh.topNode();
        cout << tmp->getKey() << ' ' << tmp->getID() << endl;
*/
    

    while(!tem_tree.empty()){
	    double const distance_limit = (*bplusparenttree.begin()).second->radius;
        GeoNode *n;
        if(fh.size()>2&&!fh.empty()){
            int ID;// = (*(*fh.top()).begin()).first;
            int size = (*fh.top()).size();
            n=new GeoNode(ID, distance_limit);
            while(ID<0||ID>mymesh.vertices().size()) ID = (*tem_tree.begin()).second->index;
            std::cout << "ID size: " << ID << " " << size  << std::endl;
            int x = ((mymesh.vertices()[ID]).getx()-mymesh.m_xmin)
            /(*bplusparenttree.begin()).second->radius/1.414;
            int y = ((mymesh.vertices()[ID]).gety()-mymesh.m_ymin)
            /(*bplusparenttree.begin()).second->radius/1.414;
          //  arr[x][y]->setKey(cells[x][y]);
       //     if(cells[x][y]->exists(ID)){
         //       (*fh.top()).erase(ID);
           //     fh.decreaseKey(arr[x][y], cells[x][y]);
           // }
    /*    while(fh.size()>2&&(*fh.top()).size()==0){
           std::cout << "heap.size top-btree.size: " << fh.size()  << std::endl;
           fh.pop();
       //    if(fh.empty())de
      //     if(fh.size()==8) break;
           std::cout << "heap size: " << fh.size() << std::endl;
        }*/
        }else{
            n=new GeoNode(*(*(tem_tree.begin())).second);
        }
        n=new GeoNode(*(*(tem_tree.begin())).second);
    
 //       std::cout<<tem_tree.size()<<std::endl;
//        std::btree< int,  stx::btree<int, int>* > Gridtree;
//        Gridtree.clear();
        //setnode(rootNode, mesh.m_xmin, mesh.m_ymin, mesh.m_width, mesh.m_height);

        std::pair<int, GeoNode*> m(n->index, n);
        currentnodes.push_back(m);
        bplustree.erase(n->index);
        tem_tree.erase(n->index);
	    geodesic::SurfacePoint source(&mymesh.vertices()[n->index]);
	    std::vector<geodesic::SurfacePoint> all_sources(1,source);
        //std::cout<<"distance_limit: "<<distance_limit<<std::endl;
	    //n->chain=algorithm.range(all_sources, fh, n,mymesh,  distance_limit );
	    n->chain=algorithm.range(all_sources,  distance_limit, n );
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
            std::cout << "heap size: " << fh.size() << std::endl;
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
//            while(!fh.empty())fh.pop();
        bplusparenttree.clear();      
        bplusparenttree = stx::btree<int, GeoNode*>(currentnodes.begin(),currentnodes.end());

        for(stx::btree<int, GeoNode*>::iterator ite=bplusparenttree.begin();ite!=bplusparenttree.end();ite++){
         //   if(depth<log10(4*s-4)/log10(2)-2)break;
            geodesic::SurfacePoint p(&mymesh.vertices()[(*ite).first]);
          //  std::cout<<"ite index: "<< (**ite).index<< std::endl;
	        std::vector<geodesic::SurfacePoint> all_sources(1,p);
            algorithm.propagate(all_sources,(*(*ite).second).radius*(4*s-4));
            for(stx::btree<int, GeoNode*>::iterator bite=ite;bite!=bplusparenttree.end();bite++){
            if(bite==ite)continue;
            double dist;
            //std::cout<<"  bite index: "<<(**bite).index<< std::endl;
            geodesic::SurfacePoint q(&mymesh.vertices()[(*((*bite).second)).index]);
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

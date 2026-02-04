#include"graphtree.h"
#include<cmath>
#include <iostream>
#include <fstream>
#include "geotree.h"
#include "geodesic_algorithm_subdivision.h"
double max(double x, double y){
    if(x>y)return x;
    return y;
}

double distance_geo(Node *x, Node *y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(x==y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x->indexArray[0]]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	unsigned target_vertex_index = y->indexArray[0];
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

double distance_geo(GraphNode &x, GraphNode &y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(&x==&y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x.index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	int target_vertex_index = y.index;
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

double distance_geo(GeoNode &x, GeoNode &y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(&x==&y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x.index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	unsigned target_vertex_index = y.index;
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

bool find_distance(GeoNode *x, GeoNode *y, double &dist){
    int i=0;
    int j=y->connect.size()-1;
    if(x==y){
        dist=0;
        return true;
    }
 //   std::cout<<"x y: "<<x<<y<<"xindex yindex:"<<x->index<<" "<<y->index<<std::endl;
    if(x==NULL||y==NULL){
        std::cout<<"NULL"<<std::endl;
    }
    while(true){
        if(y->connect.size()==0)return false;
        if(x->index==y->connect[(i+j)/2]->index){
            dist=y->connect[(i+j)/2]->dist;
            return true;
        }
        if(i==j)return false;
        if(x->index==y->connect[(i+j)/2+1]->index){
            dist=y->connect[(i+j)/2+1]->dist;
            return true;
        }
        if(j-i<=1)return false;
        if(x->index<y->connect[(i+j)/2]->index){
            j=(i+j)/2;
        }
        else{
            i=(i+j)/2+1;
        }
    }

}


double distance_geo(GeoNode *x, GeoNode *y){
    if(x==y)return 0;
    if(x->index==y->index)return 0;
    double distance;
    GeoNode *xx, *yy;
    int i,j;
    i=0;
    j=bplusparenttree.size()-1;
    while(true){
        if(x->index==geonodevector[(i+j)/2]->index){
            xx=geonodevector[(i+j)/2];
            break;
        }
        if(x->index==geonodevector[(i+j)/2+1]->index){
            xx=geonodevector[(i+j)/2+1];
            break;
        }
        if(x->index<geonodevector[(i+j)/2]->index){
            j=(i+j)/2;
        }
        else{
            i=(i+j)/2+1;
        }
    }

    i=0;
    j=geonodevector.size()-1;
    while(true){
        if(y->index==geonodevector[(i+j)/2]->index){
            yy=geonodevector[(i+j)/2];
            break;
        }
        if(y->index==geonodevector[(i+j)/2+1]->index){
            yy=geonodevector[(i+j)/2+1];
            break;
        }
        if(y->index<geonodevector[(i+j)/2]->index){
            j=(i+j)/2;
        }
        else{
            i=(i+j)/2+1;
        }
    }
    while(true){
        if(find_distance(xx,yy,distance)||find_distance(yy,xx,distance)){
            return distance;
        }
        xx=xx->parent;
        yy=yy->parent;
    }
}
double distance_geo(Node *x, Node *y, geodesic::GeodesicAlgorithmSubdivision &algorithm){
    if(x==y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x->indexArray[0]]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	unsigned target_vertex_index = y->indexArray[0];
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

double distance_geo(GraphNode &x, GraphNode &y, geodesic::GeodesicAlgorithmSubdivision &algorithm){
    if(&x==&y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x.index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	int target_vertex_index = y.index;
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

double distance_geo(GeoNode &x, GeoNode &y, geodesic::GeodesicAlgorithmSubdivision &algorithm){
    if(&x==&y)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x.index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	unsigned target_vertex_index = y.index;
	geodesic::SurfacePoint target(&mesh.vertices()[target_vertex_index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);
    algorithm.propagate(all_sources, 0, &stop_points);
    double distance;
	algorithm.best_source(target, distance); //find a single source-target path
    return distance;
}

double distance_eu(Node *x, Node *y){
    if(x==y)return 0;
    double cxx=x->posX+x->width/2;
    double cxy=x->posY+x->height/2;
    double cyx=y->posX+y->width/2;
    double cyy=y->posY+y->height/2;
    return sqrt(pow(cxx-cyx,(double)2.0)+pow(cxy-cyy,(double)2.0));
}

double dia_eu(Node *x){
    if(x->size==1)return 0;
    return 0.25*sqrt(pow(x->width,(double)2.0)+pow(x->height,(double)2.0));
}

double dia_geo(Node *x, geodesic::GeodesicAlgorithmExact &algorithm){
    if(x->size==1)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x->indexArray[0]]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	//double const distance_limit = geodesic::GEODESIC_INF;			// no limit for propagation
	double const distance_limit = 0;
    double distance=0;
    double maxdis=0;
	geodesic::SurfacePoint target(&mesh.vertices()[x->indexArray[1]]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);	//stop propagation when the target is covered
    for(int i=2;i<x->size;i++){
        stop_points.push_back(geodesic::SurfacePoint(&mesh.vertices()[x->indexArray[i]]));
    }
	algorithm.propagate(all_sources, distance_limit, &stop_points);	//"propagate(all_sources)" is also fine, but take more time because covers the whole mesh
    for(int i=1;i<x->size;i++){
        geodesic::SurfacePoint p(&mesh.vertices()[x->indexArray[i]]);
        algorithm.best_source(p, distance);
        maxdis=std::max(distance, maxdis);
    }
    return maxdis;
}

double dia_geo(GeoNode &x, geodesic::GeodesicAlgorithmExact &algorithm){
    if(x.radius==0)return 0;
	geodesic::SurfacePoint source(&mesh.vertices()[x.index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
	//double const distance_limit = geodesic::GEODESIC_INF;			// no limit for propagation
	double const distance_limit = 0;
    double distance;
    double maxdis;

	geodesic::SurfacePoint target(&mesh.vertices()[x.children.front()->index]);		//create source 
	std::vector<geodesic::SurfacePoint> stop_points(1, target);	//stop propagation when the target is covered
    std::list<GeoNode*>::iterator ite=x.children.begin();
    for(ite++;ite!=x.children.end();ite++){
        stop_points.push_back(geodesic::SurfacePoint(&mesh.vertices()[(*ite)->index]));
    }
	algorithm.propagate(all_sources, distance_limit, &stop_points);	//"propagate(all_sources)" is also fine, but take more time because covers the whole mesh
    ite=x.children.begin();
    for(ite++;ite!=x.children.end();ite++){
        geodesic::SurfacePoint p(&mesh.vertices()[(*ite)->index]);
        algorithm.best_source(p,distance);
        maxdis=std::max(distance,maxdis);
    }
    
    return maxdis;
}

double dia_geo(GeoNode &x){
    if(x.radius==0)return 0;
    double maxdis;

    std::list<GeoNode*>::iterator ite=x.children.begin();
    for(;ite!=x.children.end();ite++){
        maxdis=std::max(distance_geo(&x,(*ite)),maxdis);
    }
    
    return maxdis;
}
void generate_pair_eu(Node *x, Node *y){
    if(x==NULL||y==NULL){
//        printf("generate pair fails.\n");
        return;
    }
    if(x->size==0||y->size==0)return;
    if(distance_eu(x, y)<s*max(dia_eu(x), dia_eu(y))&&(dia_eu(x)!=0.0||dia_eu(y)!=0.0)){
        if(dia_eu(x)>dia_eu(y)){
            for(int i=0;i<4;i++){
                    generate_pair_eu(x->child[i],y);
            }
        }
        else{
            for(int j=0;j<4;j++){
                generate_pair_eu(x,y->child[j]);
            }
        }
    }
    else{
       if(x==y)return;
       pairs++;
       NodePair *nodepair= new NodePair;
       nodepair->node1=x;
       nodepair->node2=y;
       nodepair->distance=distance_eu(x,y);
       quadpairs.push_back(nodepair);
       printf("%d-th:{(%f,%f), (%f, %f)},Dia:%f %f\n", pairs, x->posX,x->posY,y->posX,y->posY,dia_eu(x),dia_eu(y));
    }
}
void generate_pair_geo(Node *x, Node *y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(x==NULL||y==NULL){
//        printf("generate pair fails.\n");
        return;
    }
    if(x->size==0||y->size==0)return;
    double distancexy=distance_geo(x,y,algorithm);
    double diax=dia_geo(x,algorithm);
    double diay=dia_geo(y,algorithm);
    if(distancexy<s*max(diax, diay)&&diax!=0.0||diay!=0.0){
        if(diax>diay){
            for(int i=0;i<4;i++){
                generate_pair_geo(x->child[i],y,algorithm);
            }
        }
        else{
            for(int j=0;j<4;j++){
                generate_pair_geo(x,y->child[j],algorithm);
            }
        }
    }
    else{
       if(x==y)return;
       pairs++;
       NodePair *nodepair= new NodePair;
       nodepair->node1=x;
       nodepair->node2=y;
       nodepair->distance=distancexy;
       quadpairs.push_back(nodepair);
       printf("%d-th:{(%f,%f), (%f, %f)},Dia:%f %f\n", pairs, x->posX,x->posY,y->posX,y->posY,dia_geo(x,algorithm),dia_geo(y,algorithm));
    }
}
void generate_pair_geo(GeoNode &x, GeoNode &y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(&x==NULL||&y==NULL){
//        printf("generate pair fails.\n");
        return;
    }
    double xdia=dia_geo(x);
    double ydia=dia_geo(y);
    double distancexy=distance_geo(&x,&y);
  //  std::cout<<"xdia ydia: "<<xdia<< " "<<ydia<<" xradius yradius: "<<x.radius<<" "<<y.radius<<std::endl; 
    if(distancexy<s*max(xdia, ydia)&&(x.radius||y.radius)){
        if(xdia>ydia){
            for(std::list<GeoNode*>::iterator ite=x.children.begin();ite!=x.children.end();ite++){
                generate_pair_geo((**ite),y,algorithm);
            }
        }
        else{
            for(std::list<GeoNode*>::iterator jte=y.children.begin();jte!=y.children.end();jte++){
                generate_pair_geo(x,(**jte),algorithm);
            }
        }
    }
    else{
       if(&x==&y)return;
       if(x.index==y.index)return;
       pairs++;
       GeoPair *nodepair= new GeoPair();
       nodepair->node1=&x;
       nodepair->node2=&y;
       nodepair->distance=distancexy;
       geopairs.push_back(nodepair);
    //   printf("%d-th: index: %d %d,Dia:%f %f\n", pairs, x.index,y.index,xdia,ydia);
    }
}

void generate_pair_geo(GraphNode &x, GraphNode &y, geodesic::GeodesicAlgorithmExact &algorithm){
    if(&x==NULL||&y==NULL){
//        printf("generate pair fails.\n");
        return;
    }
    double distancexy=distance_geo(x,y,algorithm);
    if(x.size==0||y.size==0)return;
    if(distancexy<s*max(x.maxdis,y.maxdis)&&(x.maxdis!=0.0||y.maxdis!=0.0)){
        if(x.maxdis>y.maxdis){
                generate_pair_geo(*x.child1,y,algorithm);
                generate_pair_geo(*x.child2,y,algorithm);
        }
        else{
                generate_pair_geo(x,*y.child1,algorithm);
                generate_pair_geo(x,*y.child2,algorithm);
        }
    }
    else{
       if(&x==&y)return;
       pairs++;
       GraphPair *nodepair= new GraphPair();
       nodepair->node1=&x;
       nodepair->node2=&y;
       nodepair->distance=distancexy;
       graphpairs.push_back(nodepair);
       printf("%d-th: index: %d %d,Dia:%f %f\n", pairs, x.index,y.index,x.maxdis,y.maxdis);
    }
}
bool distance_query_eu(Node &x, Node &y,double &distance_get){
    std::cout<<"distance_query_eu("<<x.indexArray[0]<<", "<<y.indexArray[0]<<")"<<std::endl;
    if(&x==&y)return 0;
    int i=0;
    int j=quadpairs.size()-1;
    NodePair * newpair=new NodePair;
    newpair->node1=&x;
    newpair->node2=&y;
    while(true){
        bool key1=true;
        bool key2=true;
        std::cout<<"i,j:"<<i<<","<<j<<std::endl;
        int k=(int)((i+j)/2);
            std::list<bool>::iterator qitex=x.mcode.begin();
            for(std::list<bool>::iterator qite1=pairvector[k]->node1->mcode.begin();qite1!=pairvector[k]->node1->mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }

            std::list<bool>::iterator qitey=y.mcode.begin();
            for(std::list<bool>::iterator qite2=pairvector[k]->node2->mcode.begin();qite2!=pairvector[k]->node2->mcode.end();qite2++){
                if(*qite2!=*qitey){
                    key2=false;
                    break;
                }
                qitey++;
            }
            if(key1&&key2){
                distance_get= pairvector[k]->distance;
                return true;
            }
        if(i==j)break;
        if(j-i==1){
            i=j;
            continue;
        }
        if(compair_quadpair(newpair,pairvector[k])){
            j=(int)((i+j)/2);

        }
        else{
            i=(int)((i+j)/2);
            
        }
    }
    return false;
}
double distance_query_eu(Node &x, Node &y){
    double distance_get;
    Node *p;
    if(&x==&y)return 0;
    if(&x==NULL||&y==NULL){
        std::cout<<"distance query graph failed."<<std::endl;
    }
    if(distance_query_eu(x,y,distance_get))return distance_get;
    if(distance_query_eu(y,x,distance_get))return distance_get;
    p=&x;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_eu(*(p),y,distance_get))return distance_get;
        if(distance_query_eu(y,*(p),distance_get))return distance_get;
    }
    p=&y;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_eu(x,*(p),distance_get))return distance_get;
        if(distance_query_eu(*(p),x,distance_get))return distance_get;
    }
 //   if(x.parent!=NULL&&distance_query_eu(*(x.parent),y,distance_get))return distance_get;
  //  if(y.parent!=NULL&&distance_query_eu(x,*(y.parent),distance_get))return distance_get;
    return distance_query_eu(*(x.parent),*(y.parent));
}
/*
bool distance_query_geo(GeoNode &x, GeoNode &y,double &distance_get){
    std::cout<<"distance_query_geo("<<x.index<<", "<<y.index<<")"<<std::endl;
    if(&x==&y)return 0;
    int i=0;
    int j=geopairs.size()-1;
    GeoPair * newpair=new GeoPair;
    newpair->node1=&x;
    newpair->node2=&y;
    while(true){
        bool key1=true;
        bool key2=true;
        std::cout<<"i,j:"<<i<<","<<j<<std::endl;
        int k=(int)((i+j)/2);
            std::list<int>::iterator qitex=x.mcode.begin();
            for(std::list<int>::iterator qite1=geopairsvector[k]->node1->mcode.begin();qite1!=geopairsvector[k]->node1->mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }

            std::list<int>::iterator qitey=y.mcode.begin();
            for(std::list<int>::iterator qite2=geopairsvector[k]->node2->mcode.begin();qite2!=geopairsvector[k]->node2->mcode.end();qite2++){
                if(*qite2!=*qitey){
                    key2=false;
                    break;
                }
                qitey++;
            }
            if(key1&&key2){
                distance_get=geopairsvector[k]->distance;
                return true;
            }
        if(i==j)break;
        if(j-i==1){
            i=j;
            continue;
        }
        if(compair_geopair(newpair,geopairsvector[k])){
            j=(int)((i+j)/2);

        }
        else{
            i=(int)((i+j)/2);
            
        }
    }
    return false;
}

double distance_query_geo(GeoNode &x, GeoNode &y){
    double distance_get;
    GeoNode *p;
    
    if(&x==&y)return 0;
    if(&x==NULL||&y==NULL){
        std::cout<<"distance query geo failed."<<std::endl;
        return 0;
    }
    if(distance_query_geo(x,y,distance_get))return distance_get;
    if(distance_query_geo(y,x,distance_get))return distance_get;
    p=&x;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_geo(*(p),y,distance_get))return distance_get;
        if(distance_query_geo(y,*(p),distance_get))return distance_get;
    }
    p=&y;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_geo(x,*(p),distance_get))return distance_get;
        if(distance_query_geo(*(p),x,distance_get))return distance_get;
    }
    return distance_query_geo(*x.parent, *y.parent);
}
*/

bool distance_query_graph(GraphNode &x, GraphNode &y, double &distance_get){
    std::cout<<"distance_query_graph("<<x.index<<", "<<y.index<<")"<<std::endl;
    if(&x==&y)return 0;
    int i=0;
    int j=graphpairs.size()-1;
    GraphPair * newpair=new GraphPair;
    newpair->node1=&x;
    newpair->node2=&y;
    while(true){
        bool key1=true;
        bool key2=true;
        std::cout<<"i,j:"<<i<<","<<j<<std::endl;
        int k=(int)((i+j)/2);
            std::list<bool>::iterator qitex=x.mcode.begin();
            for(std::list<bool>::iterator qite1=graphpairsvector[k]->node1->mcode.begin();qite1!=graphpairsvector[k]->node1->mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }

            std::list<bool>::iterator qitey=y.mcode.begin();
            for(std::list<bool>::iterator qite2=graphpairsvector[k]->node2->mcode.begin();qite2!=graphpairsvector[k]->node2->mcode.end();qite2++){
                if(*qite2!=*qitey){
                    key2=false;
                    break;
                }
                qitey++;
            }
            if(key1&&key2){
                distance_get=graphpairsvector[k]->distance;
                return true;
            }
        if(i==j)break;
        if(j-i==1){
            i=j;
            continue;
        }
        if(compair_graphpair(newpair,graphpairsvector[k])){
            j=(int)((i+j)/2);

        }
        else{
            i=(int)((i+j)/2);
            
        }
    }
    return false;
}

double distance_query_graph(GraphNode &x, GraphNode &y){
    double distance_get;
    GraphNode *p;
    if(&x==&y)return 0;
    if(&x==NULL||&y==NULL){
        std::cout<<"distance query graph failed."<<std::endl;
    }
    if(distance_query_graph(x,y,distance_get))return distance_get;
    if(distance_query_graph(y,x,distance_get))return distance_get;
    p=&x;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_graph(*(p),y,distance_get))return distance_get;
        if(distance_query_graph(y,*(p),distance_get))return distance_get;
    }
    p=&y;
    while(p->parent!=NULL){
        p=p->parent;
        if(distance_query_graph(x,*(p),distance_get))return distance_get;
        if(distance_query_graph(*(p),x,distance_get))return distance_get;
    }
//    if(x.parent!=NULL&&distance_query_graph(*(x.parent),y,distance_get))return distance_get;
//    if(y.parent!=NULL&&distance_query_graph(x,*(y.parent),distance_get))return distance_get;
    return distance_query_graph(*(x.parent),*(y.parent));
}

bool compair_quaddistance(NodePair *x, NodePair *y){
    return (x->distance<y->distance);
}
bool compair_graphdistance(GraphPair *x, GraphPair *y){
    return (x->distance<y->distance);
}
bool compair_geodistance(GeoPair *x, GeoPair *y){
    return (x->distance<y->distance);
}
bool is_contained(Node &x, NodePair &p){
        bool key1=true;
        bool key2=true;
        std::list<bool>::iterator qitex=x.mcode.begin();
        if(p.node1!=&x&&p.node2!=&x)return false;
        for(std::list<bool>::iterator qite1=p.node1->mcode.begin();qite1!=p.node1->mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }
        for(std::list<bool>::iterator qite2=p.node2->mcode.begin();qite2!=p.node2->mcode.end();qite2++){
                if(*qite2!=*qitex){
                    key2=false;
                    break;
                }
                qitex++;
            }
            return (key1||key2);
}
double knn(Node &n, int k){
    std::list<NodePair*> klist;
    klist.clear();
    for(std::vector<NodePair*>::iterator pite=pairvector.begin();pite!=pairvector.end();pite++){
        if(is_contained(n,**pite))klist.push_back(*pite);
    }
    klist.sort(compair_quaddistance);
    std::list<NodePair*>::iterator piter=klist.begin();
    int j=0;
    for(int i=0;i<k&&j<klist.size();){
        i+=std::max((*piter)->node1->size,(*piter)->node2->size);
        piter++;
        j++;
    }
    return (*piter)->distance;
}
bool is_contained(GraphNode &x, GraphPair &p){
        bool key1=true;
        bool key2=true;
        if(p.node1!=&x&&p.node2!=&x)return false;
        std::list<bool>::iterator qitex=x.mcode.begin();
        for(std::list<bool>::iterator qite1=p.node1->mcode.begin();qite1!=p.node1->mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }
        for(std::list<bool>::iterator qite2=p.node2->mcode.begin();qite2!=p.node2->mcode.end();qite2++){
                if(*qite2!=*qitex){
                    key2=false;
                    break;
                }
                qitex++;
            }
            return (key1||key2);
}
double knn(GraphNode &n, int k){
    std::list<GraphPair*> klist;
    klist.clear();
    for(std::vector<GraphPair*>::iterator pite=graphpairsvector.begin();pite!=graphpairsvector.end();pite++){
        if(is_contained(n,**pite))klist.push_back(*pite);
    }
    klist.sort(compair_graphdistance);
    std::list<GraphPair*>::iterator piter=klist.begin();
    int j=0;
    for(int i=0;i<k&&j<klist.size();){
        i+=std::max((*piter)->node1->size,(*piter)->node2->size);
        piter++;
        j++;
    }
    return (*piter)->distance;
}
/*
bool is_contained(GeoNode &x, GeoPair &p, int k){
        bool key1=true;
        bool key2=true;
        if(p.node1!=&x&&p.node2!=&x)return false;
        std::list<int>::iterator qitex=x.mcode.begin();
        for(std::list<int>::iterator qite1=p.node1->mcode.begin();qite1!=p.node1->mcode.end();qite1++){
                if(p.node1->size>k){
                    key1=false;
                    break;
                }
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }
        for(std::list<int>::iterator qite2=p.node2->mcode.begin();qite2!=p.node2->mcode.end();qite2++){
                if(p.node2->size>k){
                    key2=false;
                    break;
                }
                if(*qite2!=*qitex){
                    key2=false;
                    break;
                }
                qitex++;
            }
            return (key1||key2);
}
*/
double knn(GeoNode &n, int k){
    std::list<float> klist;
    klist.clear();
    for(std::vector<GeoNode*>::iterator pite=geonodevector.begin();pite!=geonodevector.end();pite++){
        klist.push_back(distance_geo(*(pite), &n));
    }
    klist.sort();
    
    std::list<float>::iterator piter=klist.begin();
    //int j=0;
    for(int i=0;i<k;i++){
 //       i+=std::max((*piter)->node1->size,(*piter)->node2->size);
  //      piter++;
   //     j++;
    }
    return (*piter);
}
bool is_contained(Node &x, Node &y){
        bool key1=true;
        std::list<bool>::iterator qitex=x.mcode.begin();
        for(std::list<bool>::iterator qite1=y.mcode.begin();qite1!=y.mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }
        return key1;
}
bool is_contained(GraphNode &x, GraphNode &y){
        bool key1=true;
        std::list<bool>::iterator qitex=x.mcode.begin();
        for(std::list<bool>::iterator qite1=y.mcode.begin();qite1!=y.mcode.end();qite1++){
                if(*qite1!=*qitex){
                    key1=false;
                    break;
                }
                qitex++;
            }
        return key1;
}
bool is_contained(GeoNode &x, GeoNode &y){
        bool key1=false;
        GeoNode* xx=&x;
        while(xx->parent!=NULL){
            if(&y==xx){
                key1=true;
                break;
            }
            xx=xx->parent;
        }
//        std::list<int>::iterator qitex=x.mcode.begin();
//        for(std::list<int>::iterator qite1=y.mcode.begin();qite1!=y.mcode.end();qite1++){
//                if(*qite1!=*qitex){
 //                   key1=false;
  //                  break;
  //              }
   //             qitex++;
    //        }
        return key1;
}
/*

void k_closest_pairs(std::vector<Node*> &x, std::vector<Node*> &y, int k){
    if(x.empty()||y.empty())return;
    std::list<NodePair*> klist;
    for(std::vector<NodePair*>::iterator pite=pairvector.begin();pite!=pairvector.end();pite++){
        for(std::vector<Node*>::iterator itex=x.begin();itex!=x.end();itex++){
            for(std::vector<Node*>::iterator itey=y.begin();itey!=y.end();itey++){
                if(is_contained(**itex,*((*pite)->node1))&&is_contained(**itey,*((*pite)->node2)))klist.push_back(*pite);
                if(is_contained(**itey,*((*pite)->node1))&&is_contained(**itex,*((*pite)->node2)))klist.push_back(*pite);
            }
        }
    }
    klist.sort(compair_quaddistance);
    
}
void k_closest_pairs(std::vector<GraphNode*> &x, std::vector<GraphNode*> &y, int k){
    if(x.empty()||y.empty())return;
    std::list<GraphPair*> klist;
    for(std::vector<GraphPair*>::iterator pite=graphpairsvector.begin();pite!=graphpairsvector.end();pite++){
        for(std::vector<GraphNode*>::iterator itex=x.begin();itex!=x.end();itex++){
            for(std::vector<GraphNode*>::iterator itey=y.begin();itey!=y.end();itey++){
                if(is_contained(**itex,*((*pite)->node1))&&is_contained(**itey,*((*pite)->node2)))klist.push_back(*pite);
                if(is_contained(**itey,*((*pite)->node1))&&is_contained(**itex,*((*pite)->node2)))klist.push_back(*pite);
            }
        }
    }
    klist.sort(compair_graphdistance);
    
}*/

void k_closest_pairs(std::vector<GeoNode*> &x, std::vector<GeoNode*> &y, int k){
    if(x.empty()||y.empty())return;
    std::list<GeoPair*> klist;
    for(std::vector<GeoPair*>::iterator pite=geopairsvector.begin();pite!=geopairsvector.end();pite++){
        for(std::vector<GeoNode*>::iterator itex=x.begin();itex!=x.end();itex++){
            for(std::vector<GeoNode*>::iterator itey=y.begin();itey!=y.end();itey++){
                if(is_contained(**itex,*((*pite)->node1))&&is_contained(**itey,*((*pite)->node2)))klist.push_back(*pite);
                if(is_contained(**itey,*((*pite)->node1))&&is_contained(**itex,*((*pite)->node2)))klist.push_back(*pite);
            }
        }
    }
    klist.sort(compair_geodistance);
    
}
bool compair_double(double x, double y){
    return x<y;
}
double surface_knn(int index,int k){
    int i;
	std::vector<geodesic::SurfacePoint> stop_points;
    stop_points.clear();
	geodesic::GeodesicAlgorithmExact algorithm(&mesh);	//create exact algorithm for the mesh
	geodesic::SurfacePoint source(&mesh.vertices()[index]);		//create source 
	std::vector<geodesic::SurfacePoint> all_sources(1,source);					//in general, there could be multiple sources, but now we have only one
    FILE* fp=fopen("POINT.C","r");
    int x[poi];
    for(int i=0;i<poi;i++){
        fscanf(fp,"%d\n",&x[i]);

	    geodesic::SurfacePoint target(&mesh.vertices()[x[i]]);		//create source 
        stop_points.push_back(target);
    }
    fclose(fp);
	algorithm.propagate(all_sources, 0, &stop_points, k);	//"propagate(all_sources)" is also fine, but take more time because covers the whole mesh
    std::list<double> klist;
    for(int i=0;i<poi;i++){
			geodesic::SurfacePoint p(&mesh.vertices()[x[i]]);		
			double distance;
            algorithm.best_source(p,distance);
            klist.push_back(distance);
    }
    klist.sort(compair_double);
    std::list<double>::iterator listite=klist.begin();
    for(int i=0;i<=k;i++)listite++;
    return *listite;
}

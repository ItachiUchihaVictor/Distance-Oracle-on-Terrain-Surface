#include "distance.h"
#include<sstream>
#include<unistd.h>
#include <ctime>
#include <cmath>
#include <sys/resource.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/times.h>
#include "geodesic_algorithm_subdivision.h"
#include "qtree.h"
#define qtimes 100
//-------------------------------------
// MAIN
//-------------------------------------
int query_type=0;
int clpsize=6;
int algo_type=0;
int algnum;
int k=3;
int i;
int num_poi;
std::vector<int> poilist;
FILE *fp;
char prefix[255];
double Time_preprocess=0;
double  Time_dquery=0, Time_knnquery, Time_clpquery;
double  Space_preprocess=0;
double  Space_query=0;
double errorbound_dis, errorbound_knn=0;
//struct rusage myTime_program_start, myTime_preprocess_end, myTime_query_begin, myTime_query_end;
double qbegin, qend;
    float eps;


void mano(geodesic::GeodesicAlgorithmExact &algorithm, geodesic::GeodesicAlgorithmSubdivision &mano_algo){
        double radius=0;
        double distance;
        tml::qtree<float, int> tree(mesh.m_xmin, mesh.m_ymin, mesh.m_xmin+mesh.m_width, mesh.m_ymin+mesh.m_height);
      //  fp=fopen("POINT.C","r");
        for(i=0;i<mesh.vertices().size();i++)
        {
                tree.add_node((&mesh.vertices()[i])->getx(), (&mesh.vertices()[i])->gety(), i);// (&mesh.vertices()[i])->id());
        }
      // fclose(fp);
       printf("file read finished\n");
       /* std::ostringstream cmd;
        cmd.clear();
        cmd.str("");
        cmd << "/proc/" << getpid() << "/statm";
        fp=fopen(cmd.str().c_str(),"r");
        fscanf(fp,"%f", Space_preprocess);
        fclose(fp);*/
//        Space_preprocess=2*geopairs.size()*sizeof(GeoNode)+sizeof(mesh)+sizeof(algorithm);
     //  geopairs.sort(compair_geopair);
     //  PrintGeoPair();
/*       for(int m=0;m<geopairsvector.size();m++){
           for(std::list<int>::iterator bite=geopairsvector[m]->node1->mcode.begin();bite!=geopairsvector[m]->node1->mcode.end();bite++)
               std::cout<<*bite;
           std::cout<<", ";
           for(std::list<int>::iterator bite=geopairsvector[m]->node2->mcode.begin();bite!=geopairsvector[m]->node2->mcode.end();bite++)
               std::cout<<*bite;
           std::cout<<std::endl;
       }*/
       for(int q=0;q<qtimes;q++){
       int qindex1=randn(mesh.vertices().size());
       int qindex2=randn(mesh.vertices().size());
       if(qindex1==qindex2)qindex2=(qindex1+1)%mesh.vertices().size();
       
       if(algnum == 0){
           while(true){
                qindex1 = poilist[randn(poilist.size())];
                qindex2 = poilist[randn(poilist.size())];
                if(qindex1 !=qindex2) break;
           }
         //  if(qindex1==qindex2)qindex2=(qindex1+1)%poilist.size();
       }
      
      if(algnum == 2){
          while(true){
           double posX1 = mesh.m_xmin + mesh.m_width*((double) rand())/((double) RAND_MAX); 
           double posY1 = mesh.m_ymin + mesh.m_height*((double) rand())/((double) RAND_MAX); 
           double posX2 = mesh.m_xmin + mesh.m_width*((double) rand())/((double) RAND_MAX); 
           double posY2 = mesh.m_ymin + mesh.m_height*((double) rand())/((double) RAND_MAX); 
            double NNradius = mesh.shortest_edge;
            const tml::qtree<float, int>::node_type *NNnode = NULL;
            while(NNnode==NULL){
                NNnode = tree.find_nearest(posX1, posY1, NNradius);// mesh.shortest_edge);
                NNradius*=2.0;
            }
            qindex1 = NNnode->data;
           NNradius = mesh.shortest_edge;
           NNnode = NULL;
            while(NNnode==NULL){
                NNnode = tree.find_nearest(posX2, posY2, NNradius);// mesh.shortest_edge);
                NNradius*=2.0;
            }
            qindex2 = NNnode->data;
            if(qindex1 !=qindex2) break;
          }
      } 
       

/*       for(std::list<int>::iterator bite=geonodevector[qindex1]->mcode.begin();bite!=geonodevector[qindex1]->mcode.end();bite++)
           std::cout<<*bite;
       std::cout<<", ";
       for(std::list<int>::iterator bite=geonodevector[qindex2]->mcode.begin();bite!=geonodevector[qindex2]->mcode.end();bite++)
           std::cout<<*bite;
       std::cout<<std::endl;*/

/*
 * Distance Query
 * */

//#ifndef WIN32
//       getrusage(RUSAGE_SELF,&myTime_query_begin);
//#endif
       qbegin = clock();
       double distance_return=distance_geo(qindex1, qindex2, mano_algo); 
       std::cout<<"distance_return:"<<distance_return<<std::endl;
       qend = clock();
       Time_dquery += (-qbegin+qend)*1e6/CLOCKS_PER_SEC;
//       Time_dquery+=myTime_query_end.ru_utime.tv_usec-myTime_query_begin.ru_utime.tv_usec;
       double realdistance;
       realdistance=distance_geo(qindex1,qindex2,algorithm);
       std::cout<<"realdistance:"<<realdistance<<std::endl;
       if(realdistance!=0&&distance_return>realdistance)errorbound_dis+=std::abs((double)((distance_return/realdistance)-(double)1.0));
       std::cout<<errorbound_dis<<std::endl;

/*
 * KNN Query
 * */

/*#ifndef WIN32
       getrusage(RUSAGE_SELF,&myTime_query_begin);
#endif
       distance_return=knn(*geonodevector[qindex1],k);
       std::cout<<"distance_return:"<<distance_return<<std::endl;
#ifndef WIN32
       getrusage(RUSAGE_SELF,&myTime_query_end);
       Time_knnquery+=myTime_query_end.ru_utime.tv_usec-myTime_query_begin.ru_utime.tv_usec;
#endif
       realdistance=surface_knn(geonodevector[qindex1]->index,k);
       std::cout<<"realdistance:"<<realdistance<<std::endl;
       if(realdistance!=0&&realdistance<distance_return)errorbound_knn+=std::abs((double)((distance_return/realdistance)-(double)1.0));
       std::cout<<errorbound_knn<<std::endl;
*/
       /*
        * B-closest pair
        * */
/*
#ifndef WIN32
       getrusage(RUSAGE_SELF,&myTime_query_begin);
#endif
       std::vector<GeoNode*> v1;
       v1.clear();
       for(i=0;i<clpsize/2;i++){
           v1.push_back(geonodevector[qindex1]);
           qindex1=(qindex1+1)%geonodevector.size();
       }
       std::vector<GeoNode*> v2;
       v2.clear();
       for(i=0;i<clpsize/2;i++){
           v2.push_back(geonodevector[qindex2]);
           qindex2=(qindex2+1)%geonodevector.size();
       }*/
   //    k_closest_pairs(v1,v2,k);
//#ifndef WIN32
//       getrusage(RUSAGE_SELF,&myTime_query_end);
  //     Time_clpquery+=myTime_query_end.ru_utime.tv_usec-myTime_query_begin.ru_utime.tv_usec;
//#endif
       }
       errorbound_dis/=qtimes;
       Time_dquery/=qtimes;
    //   errorbound_knn/=qtimes;
//       fp=fopen("output.txt","a");
//      fprintf(fp,"%f %f %f %f %f %f %f %f\n",Time_preprocess,Time_dquery, Space_query, errorbound_dis); 
//       fclose(fp); 
       std::ofstream disquerytime("mano.txt", std::ios::out | std::ios::app );
        disquerytime << eps << " " << num_poi << " " << mesh.vertices().size() << " "  <<  Time_dquery << " " << errorbound_dis << std::endl; 
}
/*-------------------------------------------------------------------------
 * ------------------------MAIN--------------------------------------------
 *  -------------------------------------------------------------------------
 *
 * */


int main(int argc, char **argv) 
{
	if(argc < 3)
	{
		std::cout << "usage: mesh_file_name error_bound algorithm_number" << std::endl; //try: "hedgehog_mesh.txt 3 14" or "flat_triangular_mesh.txt 1"
		return 0;
	}

	bool success = geodesic::read_mesh_from_file(argv[1],points,faces);
	if(!success)
	{
		std::cout << "something is wrong with the input file" << std::endl;
		return 0;
	}
//    sscanf(argv[2], "%f", &s );
    eps = atof(argv[2]);
	mesh.initialize_mesh_data(points, faces);		//create internal mesh data structure including edges
    strcpy(prefix, argv[1]);

    geodesic::GeodesicAlgorithmExact algorithm(&mesh);	//create exact algorithm for the mesh
    std::cout << eps << std::endl;
    float constant = mesh.longest_edge/mesh.shortest_edge*(1.0/eps+1.0)*2.0;//*std::sqrt(1-std::cos(mesh.smallest_angle));
	geodesic::GeodesicAlgorithmSubdivision mano_algo(&mesh,constant);	//with subdivision_level=0 this algorithm becomes Dijkstra, with subdivision_level->infinity it becomes exact
    // WRITE THE RANDOM POINT FILE 
/*    fp = fopen("POINT.C","w");
    if ( fp == NULL )
    {
        puts ( "Cannot open file" );
        exit(1);
    }
    for(i=0;i<poi;i++)
    {
        bool key=true;
        while(key){
            key=false;
            x[i]=randn(mesh.vertices().size());
            for(int j=0;j<i;j++){
                if(x[i]==x[j]){
                    key=true;
                    break;
                }
            }
        }
        fprintf(fp,"%d\n",x[i]);
    }
    fclose(fp);*/
// READ THE RANDOM POINT FILE AND ASSIGN TO ROOT Node
//    fp=fopen("output.txt","w");
//    fclose(fp);
    
    algnum = atoi(argv[3]);

    if(algnum==0){
        std::ifstream input("poilist.txt", std::ios::in);
        input >> num_poi;
        poilist.resize(num_poi);
        for(int i=0;i<num_poi;i++) input >> poilist[i];
    } 
    std::cout<<"---------- Mano's Algorithm ------------"<<std::endl;

        mano(algorithm, mano_algo);
}

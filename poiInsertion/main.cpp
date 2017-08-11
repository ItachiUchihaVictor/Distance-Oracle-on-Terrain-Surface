#include<sstream>
#include<unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <iostream>
#include <sstream>
#include<stdio.h>
#include<stdlib.h>
#include<list>
#include<iomanip>
#include<cmath>
#include<climits>
#include "geodesic_algorithm_exact.h"
#include "geodesic_mesh.h"
#include "qtree.h"
#include "btree.h"
//#define poi 4000 
//#define upper_poi 4000
#define INF 100000000.0
double s=200;
int pairs=0;
int remainedPOI;
std::vector<double> points;	
std::vector<unsigned> faces;
geodesic::Mesh mesh;
#include "geodesic_algorithm_subdivision.h"
#define qtimes 100
//-------------------------------------
// MAIN
//-------------------------------------
int query_type=0;
int clpsize=6;
int algo_type=0;
int k=3;
int i;
int poi = 0;
FILE *fp;
char prefix[255];
stx::btree<int, int> poitree;
#ifndef WIN32
    double Time_preprocess=0;
    double  Time_dquery=0, Time_knnquery, Time_clpquery;
    double  Space_preprocess=0;
    double  Space_query=0;
    double errorbound_dis, errorbound_knn=0;
    struct rusage myTime_program_start, myTime_preprocess_end, myTime_query_begin, myTime_query_end;
#endif

float dot(float x0, float y0, float x1, float y1){
    return x0*x1+y0*y1;
}


std::vector<float> CrossProduct1D(std::vector<float> const &a, std::vector<float> const &b)
{
    std::vector<float> r(a.size());  
        r[0] = a[1]*b[2]-a[2]*b[1];
          r[1] = a[2]*b[0]-a[0]*b[2];
            r[2] = a[0]*b[1]-a[1]*b[0];
              return r;
}
bool esp(float x1, float x2, float eps){
    return (x1-x2<eps*mesh.shortest_edge) || (x2-x1<eps*mesh.shortest_edge);
}
bool is_p_inface(geodesic::Face &f, float px, float py){
    //float x[3], y[3];
    double x[3], y[3];
    for(int i=0;i <3;i++){
        x[i] = (&mesh.vertices()[f.adjacency[i]])->getx();
        y[i] = (&mesh.vertices()[f.adjacency[i]])->gety();
    }
    //return 1;
    // Compute vectors        
    // v0 = C - A (x[0]-x[1], y[0]-y[1])
    // v1 = B - A (x[2]-x[1], y[2]-y[1])
    // v2 = P - A (px-x[1], py-y[1])
    //
    // // Compute dot products
    // dot00 = dot(v0, v0)
    // dot01 = dot(v0, v1)
    // dot02 = dot(v0, v2)
    // dot11 = dot(v1, v1)
    // dot12 = dot(v1, v2)
/*    float dot00 = dot(x[0]-x[1], y[0]-y[1], x[0]-x[1], y[0]-y[1]);
    float dot01 = dot(x[0]-x[1], y[0]-y[1], x[2]-x[1], y[2]-y[1]);
    float dot02 = dot(x[0]-x[1], y[0]-y[1], px-x[1], py-y[1]);
    float dot11 = dot(x[2]-x[1], y[2]-y[1], x[2]-x[1], y[2]-y[1]);
    float dot12 = dot(x[2]-x[1], y[2]-y[1], px-x[1], py-y[1]);*/
    double dot00 = dot(x[0]-x[1], y[0]-y[1], x[0]-x[1], y[0]-y[1]);
    double dot01 = dot(x[0]-x[1], y[0]-y[1], x[2]-x[1], y[2]-y[1]);
    double dot02 = dot(x[0]-x[1], y[0]-y[1], px-x[1], py-y[1]);
    double dot11 = dot(x[2]-x[1], y[2]-y[1], x[2]-x[1], y[2]-y[1]);
    double dot12 = dot(x[2]-x[1], y[2]-y[1], px-x[1], py-y[1]);
    //
    // // Compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    //
    // // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

int zcoor(geodesic::Face &f, float x, float y){
    std::vector<float> p1, p2, p3;
    p1.resize(3);
    p2.resize(3);
    p3.resize(3);
    for(int i=0;i <3;i++){
        p1[i] = (&mesh.vertices()[f.adjacency[i]])->getx();
        p2[i] = (&mesh.vertices()[f.adjacency[i]])->gety();
        p3[i] = (&mesh.vertices()[f.adjacency[i]])->getz();
     //   p2[i] = f.adjacent_vertices()[i]->gety();
     //   p3[i] = f.adjacent_vertices()[i]->getz();
    }
    if(p3[0]==p3[1]&&p3[2]==p3[1])return p3[0];
    std::vector<float> v1(3);
    std::vector<float> v2(3);
//    for(int i=0;i<3;i++){
        v1[0] = p1[0] - p1[1];
        v1[1] = p2[0] - p2[1];
        v1[2] = p3[0] - p3[1];
        v2[0] = p1[0] - p1[2];
        v2[1] = p2[0] - p2[2];
        v2[2] = p3[0] - p3[2];
//        v2[i] = p1[i] - p3[i];
//    }
    std::vector<float> norm = CrossProduct1D(v1, v2); 
    float k = norm[0]*p1[0] + norm[1]*p2[0] + norm[2]*p3[0];
    if(norm[2]==0) return p3[0];
    return (k - norm[0]*x - norm[1]*y)/norm[2];
}

double DistanceFromLine(double cx, double cy, double ax, double ay ,
                              double bx, double by) //, double &distanceSegment)
        //                                          double &distanceLine)
{
// find the distance from the point (cx,cy) to the line
// determined by the points (ax,ay) and (bx,by)
//
// distanceSegment = distance from the point to the line segment
// distanceLine = distance from the point to the line (assuming
//                  infinite extent in both directions
//

/*

Subject 1.02: How do I find the distance from a point to a line?


Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
Let P be the point of perpendicular projection of C on AB.  The parameter
r, which indicates P's position along AB, is computed by the dot product 
of AC and AB divided by the square of the length of AB:

(1)     AC dot AB
r = ---------  
||AB||^2

r has the following meaning:

r=0      P = A
r=1      P = B
r<0      P is on the backward extension of AB
r>1      P is on the forward extension of AB
0<r<1    P is interior to AB

The length of a line segment in d dimensions, AB is computed by:

L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 + ... + (Bd-Ad)^2)

so in 2D:   

L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )

and the dot product of two vectors in d dimensions, U dot V is computed:

D = (Ux * Vx) + (Uy * Vy) + ... + (Ud * Vd)

so in 2D:   

D = (Ux * Vx) + (Uy * Vy) 

So (1) expands to:

(Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
r = -------------------------------
L^2

The point P can then be found:

Px = Ax + r(Bx-Ax)
Py = Ay + r(By-Ay)

And the distance from A to P = r*L.

Use another parameter s to indicate the location along PC, with the 
following meaning:
s<0      C is left of AB
s>0      C is right of AB
s=0      C is on AB

Compute s as follows:

(Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
s = -----------------------------
L^2


Then the distance from C to P = |s|*L.

*/

double distanceSegment;
double r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
double r = r_numerator / r_denomenator;
//
double px = ax + r*(bx-ax);
double py = ay + r*(by-ay);
//     
double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;

double distanceLine = fabs(s)*sqrt(r_denomenator);

//
// (xx,yy) is the point on the lineSegment closest to (cx,cy)
//
double xx = px;
double yy = py;

if ( (r >= 0) && (r <= 1) )
{
distanceSegment = distanceLine;
}
else
{

double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
if (dist1 < dist2)
{
xx = ax;
yy = ay;
distanceSegment = sqrt(dist1);
}
else
{
xx = bx;
yy = by;
distanceSegment = sqrt(dist2);
}


}

return distanceSegment;
}
void insertPointonFaces(int fid, int newvertex){
		for(unsigned j=0; j<3; ++j){
            geodesic::Face f = *(new geodesic::Face());
            //geodesic::Face& f = (mesh.faces()[mesh.faces().size()-1]);
            f.id() = mesh.faces().size();
		 //   f.adjacent_vertices().set_allocation(mesh.allocate_pointers(3),3);	//allocate three units of memory
//		    f.adjacent_vertices().resize(3);	//allocate three units of memory
//            f1.push_back(newvertex);
//            f.adjacent_vertices()[0] = &mesh.vertices()[mesh.vertices().size()-1];
            f.adjacency.push_back(mesh.vertices().size()-1);
            mesh.vertices()[newvertex].adjacency.push_back(f.id());

//            f2.push_back(mesh.faces()[fid].adjacency[(j+1)%3]);
            //f.adjacent_vertices()[1] = mesh.faces()[fid].adjacent_vertices()[(j+1)%3]; 
            f.adjacency.push_back( mesh.faces()[fid].adjacency[(j+1)%3]); 
            (mesh.vertices()[mesh.faces()[fid].adjacency[(j+1)%3]]).adjacency.push_back(f.id()); 

//            f3.push_back(mesh.faces()[fid].adjacency[(j+2)%3]);
          //  f.adjacent_vertices()[2] = mesh.faces()[fid].adjacent_vertices()[(j+2)%3];
          //  f.adjacent_vertices()[2] = mesh.faces()[fid].adjacent_vertices()[(j+2)%3];
          //  mesh.faces()[fid].adjacent_vertices()[(j+2)%3]->additional.push_back(f.id()); 
            f.adjacency.push_back( mesh.faces()[fid].adjacency[(j+2)%3]); 
            (&mesh.vertices()[mesh.faces()[fid].adjacency[(j+2)%3]])->adjacency.push_back(f.id()); 
            mesh.faces().push_back(f);
            mesh.ftrue.push_back(1);
        }
        mesh.edges().resize(mesh.edges().size()+3);
}

int main(int argc, char **argv) 
{
	if(argc < 5)
	{
		std::cout << "usage: mesh_file_name poi_list output_mesh_file output_poi_list" << std::endl; //try: "hedgehog_mesh.txt 3 14" or "flat_triangular_mesh.txt 1"
		return 0;
	}

//    s = atof(argv[2]);
	bool success = geodesic::read_mesh_from_file(argv[1],points,faces);
	if(!success)
	{
		std::cout << "something is wrong with the input file" << std::endl;
		return 0;
	}

    strcpy(prefix, argv[1]);
	mesh.initialize_mesh_data(points, faces);		//create internal mesh data structure including edges

    geodesic::GeodesicAlgorithmExact algorithm(&mesh);	//create exact algorithm for the mesh
//    std::cout << s << std::endl;
    tml::qtree<float, int> tree(mesh.m_xmin, mesh.m_ymin, mesh.m_xmin+mesh.m_width, mesh.m_ymin+mesh.m_height);
    std::ofstream terrain(argv[3], std::ios::out );
    for(int i=0;i<mesh.vertices().size();i++){
        tree.add_node((&mesh.vertices()[i])->getx(), (&mesh.vertices()[i])->gety(), i);// (&mesh.vertices()[i])->id());
    //    terrain << (&mesh.vertices()[i])->getx() << " " << (&mesh.vertices()[i])->gety() << " " << (&mesh.vertices()[i])->getz() << std::endl;
    }
    std::cout << "QuadTree finished" << std::endl;
    std::ifstream points(argv[2], std::ios::in);
    std::vector<double> xcor, ycor, zcor;
    std::vector<int> pindex;
//    std::vector<int> f1;
//    std::vector<int> f2;
//    std::vector<int> f3;

    while(!points.eof()){
        double xx, yy;
        points >> xx >> yy;
        printf("%f %f\n", xx, yy);
//        std::cout << xx << " " << yy << std::endl;
        if(points.eof()) break;
        xcor.push_back(xx);
        ycor.push_back(yy);
    }
    points.close();
    //for(int i=0;i<xcor.size();i++) std::cout << i << "-th point: " << xcor[i] << " " << ycor[i] << std::endl;
    std::cout << "Points Read Finished" << std::endl;
    std::ofstream poilist(std::string(argv[4]) + "_tmp", std::ios::out);
    int num_poi=0;
    int outofrange = 0;
    int notfound = 0;
    int newvertex = mesh.vertices().size();
    std::cout << "# of pois: " << xcor.size() << std::endl;
    for(int i=0;i<xcor.size();i++){
 //       std::cout << "insert " << i << "-th POI" << std::endl;
     //   std::cout << "xcor ycor: " << xcor[i] << " " << ycor[i] << std::endl;
        if(xcor[i] < mesh.m_xmin || xcor[i] > mesh.m_xmin + mesh.m_width || ycor[i] < mesh.m_ymin || ycor[i] > mesh.m_ymin + mesh.m_height){ 
            outofrange++;
            continue;
        }
    //for(int i=0;i<mesh.vertices().size();i++) std::cout << "First adjacnet face of vertex " << i << ": " << mesh.vertices()[i].adjacent_faces()[0]->id()<< std::endl;
        std::vector<const tml::qtree<float, int>::node_type *> nodes;
        nodes.clear();
        int fid;
        float zcor = INF;
        
        const tml::qtree<float, int>::node_type *NNnode = NULL;
        float NNradius = mesh.shortest_edge;
        while(NNnode==NULL){
            NNnode = tree.find_nearest(xcor[i], ycor[i], NNradius);// mesh.shortest_edge);
            NNradius*=2.0;
        }
        if(esp(NNnode->x,xcor[i],0.1)&&esp(NNnode->y,ycor[i],0.1)){
      //  if(esp(NNnode->x,xcor[i], 0.1)&&esp(NNnode->y,ycor[i],0.1)){
            //poilist << NNnode->data << " ";
            if(!poitree.exists(NNnode->data)){
                poitree.insert2(NNnode->data, 0);
                poilist << NNnode->data << " "; // << std::endl;
                num_poi++;
            }
            continue;
        }
        float radius = mesh.shortest_edge;
        while(zcor==INF&&radius<100*mesh.shortest_edge){
          nodes.clear();
          tree.search(xcor[i], ycor[i], radius, nodes);
          radius *=2;
          if(nodes.size()==0)continue;
          std::cout << "nodes.size: " << nodes.size() << std::endl;
      //  tree.search(xcor[i], ycor[i], 2.0*mesh.longest_edge, nodes);
      //  tree.search(xcor[i], ycor[i], 2.0*mesh.longest_edge, nodes);
            for(int m=0; m<nodes.size(); m++){
                const tml::qtree<float, int>::node_type* node = nodes[m];
          //      std::cout << "nodes.size m: " << nodes.size() << " " << m << std::endl;
               // std::cout << "node->data #vertices newvertex: " << node->data << " " << mesh.vertices().size() << " " << newvertex << std::endl;
                if(node->data > mesh.vertices().size()) continue;
               for(int j=0;j<(mesh.vertices()[node->data].adjacency).size();j++){
             //       std::cout << "j: " << j << std::endl;
              // for(int j=0;;j++){
                    if(!mesh.ftrue[mesh.vertices()[node->data].adjacency[j]])continue;
                    geodesic::Face f = (mesh.faces()[mesh.vertices()[node->data].adjacency[j]]);
//                geodesic::Vertex = mesh.vertices()[node->data];
                    if(is_p_inface(f, xcor[i], ycor[i])){
                        zcor = zcoor(f, xcor[i], ycor[i]);  
                        fid = f.id();    
     /*                   std::cout << "x y z: " << xcor[i] << " " << ycor[i] << " " << zcor << std::endl;
                        std::cout << "fid: " << fid << std::endl;
                        for(int k = 0;k<3;k++)std::cout << "x" << k <<  " y" << k << " z" << k << ": " 
                            << mesh.vertices()[mesh.faces()[f.id()].adjacency[k]].getx() << " "
                            << mesh.vertices()[mesh.faces()[f.id()].adjacency[k]].gety() << " "
                            << mesh.vertices()[mesh.faces()[f.id()].adjacency[k]].getz()  
                            << std::endl; */
                        break;     
                        }
                }
               if(zcor!=INF)break;
            }
               if(zcor!=INF)break;
            }
        /*
        if(zcor==INF){
            for(int j=0;j<mesh.vertices()[node->data].additional.size();j++){
                geodesic::Face f = (mesh.faces()[mesh.faces()[node->data].additional[j]]);
                if(is_p_inface(f, xcor[i], ycor[i])){
                    zcor = zcoor(f, xcor[i], ycor[i]);       
                    break;     
                }
            }
        }*/
       if(zcor==INF){
        if(esp(NNnode->x,xcor[i], 0.1)&&esp(NNnode->y,ycor[i],0.1)){
            //poilist << NNnode->data << " ";
            if(!poitree.exists(NNnode->data)){
                poitree.insert2(NNnode->data, 0);
                poilist << NNnode->data << " "; // << std::endl;
                num_poi++;
            }
        }
           notfound++;
           continue; 
       }
       bool inserted = false;
       for(int adj=0;adj<3;adj++){
           double x1 = mesh.vertices()[mesh.faces()[fid].adjacency[adj]].getx();
           double y1 = mesh.vertices()[mesh.faces()[fid].adjacency[adj]].gety();
           double x2 = mesh.vertices()[mesh.faces()[fid].adjacency[(adj+1)%3]].getx();
           double y2 = mesh.vertices()[mesh.faces()[fid].adjacency[(adj+1)%3]].gety();
        if(DistanceFromLine(xcor[i], ycor[i], x1, y1, x2, y2)< 0.1*mesh.shortest_edge){
            //poilist << NNnode->data << " ";
            inserted = true;
        }
        if(inserted) break;
       }
       if(inserted){
           for(int adj=0;adj<3;adj++)
            if(!poitree.exists(mesh.faces()[fid].adjacency[adj])){
                poitree.insert2(mesh.faces()[fid].adjacency[adj], 0);
                poilist << mesh.faces()[fid].adjacency[adj] << " "; // << std::endl;
                num_poi++;
                break;
            }
           continue;
       }
        pindex.push_back(newvertex);
        poilist << newvertex << " "; // << std::endl;
        num_poi++;
        poitree.insert2(newvertex, 0);
        tree.add_node(xcor[i], ycor[i], newvertex);
        geodesic::Vertex vnew = *(new geodesic::Vertex());
        mesh.vertices().push_back(vnew);
        (&mesh.vertices()[mesh.vertices().size()-1])->id() = newvertex;
        (&mesh.vertices()[mesh.vertices().size()-1])->x() = xcor[i];
        (&mesh.vertices()[mesh.vertices().size()-1])->y() = ycor[i];
        (&mesh.vertices()[mesh.vertices().size()-1])->z() = zcor;
//		mesh.vertices()[mesh.vertices().size()-1].adjacency.resize(3);//set_allocation(mesh.allocate_pointers(3),	  3);	
//		mesh.vertices()[mesh.vertices().size()-1].adjacent_faces().set_allocation(mesh.allocate_pointers(3),	  3);	
        std::cout << "newvertex size: " << newvertex << " " << mesh.vertices().size()-1 << std::endl;
        mesh.ftrue[fid] = 0;
        insertPointonFaces( fid, newvertex );
        newvertex++;
        std::cout << "insertion finished." << std::endl;
    }
    std::cout << "#faces #ftrue: " <<  mesh.faces().size() << " " << mesh.ftrue.size() << std::endl;
    int sum = 0;
    for(int i=0;i<mesh.faces().size();i++)sum+=mesh.ftrue[i];
    terrain << "OFF" << std::endl;
    terrain << mesh.vertices().size() << " "
            << sum// mesh.faces().size() 
            << " "
            << mesh.edges().size() 
            << std::endl;
    for(int i=0;i<mesh.vertices().size();i++){
    //    tree.add_node((&mesh.vertices()[i])->getx(), (&mesh.vertices()[i])->gety(), (&mesh.vertices()[i])->id());
        terrain << std::setprecision(16) << double ((&mesh.vertices()[i])->getx()) << " " << std::setprecision(16) << double ((&mesh.vertices()[i])->gety()) << " " << std::setprecision(16) << double ((&mesh.vertices()[i])->getz()) << std::endl;
        //std::cout << double ((&mesh.vertices()[i])->getx()) << " " << double ((&mesh.vertices()[i])->gety()) << " " << double ((&mesh.vertices()[i])->getz()) << std::endl;
    }
    for(int i=0;i<mesh.faces().size();i++){
        if(mesh.ftrue[i])terrain << "3 " 
            << mesh.faces()[i].adjacency[0] << " "  
            << mesh.faces()[i].adjacency[1] << " "  
            << mesh.faces()[i].adjacency[2]   
            //<< mesh.faces()[i].adjacent_vertices()[0]->id() 
//            << " " << mesh.faces()[i].adjacent_vertices()[1]->id() 
//            << " " << mesh.faces()[i].adjacent_vertices()[2]->id()
            << std::endl;
    }
    while(poitree.size()<xcor.size()){
        int xnew = rand()*rand()%(mesh.vertices().size());
        if(!poitree.exists(xnew)){
            poilist << xnew << " ";// << std::endl;
            num_poi++;
            poitree.insert2(xnew, 0);
        }
    }
    std::cout << "# POIs out of range: " << outofrange << std::endl;
    std::cout << "# POIs not found zcor: " << notfound << std::endl;
//    for(unsigned i=0;i<f1.size();i++)terrain << "3 " << f1[i] << " " << f2[i] << " " << f3[i] << std::endl;
    terrain.close();
 //   poilist.seekp(0, poilist.beg);
 //   poilist << num_poi << std::endl;
    poilist.close();

    std::ifstream poitmp(std::string(argv[4])+"_tmp", std::ios::in);
    std::ofstream outputpoi(argv[4], std::ios::out);

    outputpoi << num_poi << std::endl;
    outputpoi << poitmp.rdbuf();
    poitmp.close();
    outputpoi.close();
    remove((std::string(argv[4])+"_tmp").c_str());
    return 1;
	//geodesic::GeodesicAlgorithmSubdivision subdivision_algorithm(&mesh,2);	//with subdivision_level=0 this algorithm becomes Dijkstra, with subdivision_level->infinity it becomes exact
    // WRITE THE RANDOM POINT FILE 
/*    fp = fopen("POINT.C","w");
    if ( fp == NULL )
    {
        puts ( "Cannot open file" );
        exit(1);
    }*/
//    x[0]=randn(mesh.vertices().size());
//    for(i=1;i<poi;i++)
//    {
      //  bool key=true;
     //   while(key){
        //    key=false;
//            x[i]=(x[i-1]+1)%(mesh.vertices().size());
     /*       for(int j=0;j<i;j++){
                if(x[i]==x[j]){
                    key=true;
                    break;
                }
            }*/
      //  }
 //       fprintf(fp,"%d\n",x[i]);
  //  }
 //   fclose(fp);
}

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
#include<random>
#include<climits>
#include "geodesic_mesh.h"
//#define poi 4000 
//#define upper_poi 4000
#define INF 100000000.0
std::vector<double> points;	
std::vector<unsigned> faces;
geodesic::Mesh mesh;
//-------------------------------------
// MAIN
//-------------------------------------

bool is_valid(double x, double y){
    return (x<mesh.m_xmin+mesh.m_width&&x>mesh.m_xmin&&y<mesh.m_ymin+mesh.m_height&&y>mesh.m_ymin);
}

int main(int argc, char **argv) 
{
	if(argc < 4)
	{
		std::cout << "usage: 1st_input_file(terrain_data) 2nd_input_file(a list of x y coordinates) #_of_output_poi" << std::endl; //try: "hedgehog_mesh.txt 3 14" or "flat_triangular_mesh.txt 1"
		return 0;
    }
//    s = atof(argv[2]);
	bool success = geodesic::read_mesh_from_file(argv[1],points,faces);
	if(!success)
	{
		std::cout << "something is wrong with the input file" << std::endl;
		return 0;
	}

	mesh.initialize_mesh_data(points, faces);		//create internal mesh data structure including edges

    int num_poi = atoi(argv[3]);
    std::ifstream points(argv[2], std::ios::in);
    std::vector<double> xcor, ycor, zcor;
    std::vector<int> pindex;
    double xavg, yavg, xsq_avg, ysq_avg;
    double xdev, ydev;

    while(!points.eof()){
        double xx, yy;
        points >> xx >> yy;
        printf("%f %f\n", xx, yy);
//        std::cout << xx << " " << yy << std::endl;
        if(points.eof()) break;
        xcor.push_back(xx);
        ycor.push_back(yy);
    }
    for(int i=0;i<xcor.size();i++){
        xavg+=xcor[i]/xcor.size();
        yavg+=ycor[i]/ycor.size();
        xsq_avg+=xcor[i]*xcor[i]/xcor.size();
        ysq_avg+=ycor[i]*ycor[i]/ycor.size();
    }
    xdev = std::sqrt(xsq_avg - (xavg)*(xavg)); 
    ydev = std::sqrt(ysq_avg - (yavg)*(yavg)); 
    points.close();
    //for(int i=0;i<xcor.size();i++) std::cout << i << "-th point: " << xcor[i] << " " << ycor[i] << std::endl;
    std::cout << "Points Read Finished" << std::endl;
    std::ofstream poilist(std::string(argv[3])+".txt", std::ios::out); 
    
    std::default_random_engine generator;
    std::normal_distribution<double> xdis(xavg,xdev); 
    std::normal_distribution<double> ydis(yavg,ydev); 
    int num = 0;
    while(num<num_poi){
        double xx = xdis(generator);
        double yy = ydis(generator);
        if(!is_valid(xx, yy)) continue; 
        poilist << xx << " " << yy << std::endl;       
        num++;
    }
    poilist.close();

    return 1;
}

# Arbitrary Point-to-Arbitrary Point Distance Oracle on Terrain Surface

The main source code is main.cpp. 

# Compiling Command

g++4 -o main main.cpp -std=c++11

# How to Use It

main "terrain_data" s xycoor

The 2nd parameter is the terrain data file and the third parameter is the separation factor. xycoor is a file containing the x and y coordinates of each POIs. 

Format of xycoor:

x_coordinate_of_1st_POI y_coordinate_of_1st_POI 

x_coordinate_of_2nd_POI y_coordinate_of_2nd_POI

......

The relationship between the separation factor and the error bound. 

e = 0.05 <=> s = 40 

e = 0.1 <=> s = 20

e = 0.15 <=> s = 13

e = 0.2 <=> s = 10

e = 0.25 <=> s = 8

An example: 

main small_terrain.off 8

In this example, samll_terrain.off is the terrain data file and 8 is the separation factor and the error bound e is equal to 0.25.

# Experimental Result

The program will save result in a file "SE.txt". 

The result contains "error bound" "# of POIs" "preprocessing time (seconds)" "P2P query time (micor seconds)" "space (MB)" "error" "A2A query time". 

In the aforementioned example, the result file is "small_terrain.off.txt".

e# Point-to-Point Distance Oracle on Terrain Surface

The main source code is main.cpp. 

# Compiling Command

g++4 -o main main.cpp -std=c++11

# How to Use It

main "terrain_data" s query_type

The 2nd parameter is the terrain data file and the third parameter is the error bound. The 4th parameter is the indicator of query type. 

The corresponding indicator of each query type:

P2P ----  0

V2V ----  1

A2A ----  2

Note that P2P query assumes that there is a "poilist.txt" file under the folder. 


An example: 

main small_terrain.off 0.25 0

In this example, samll_terrain.off is the terrain data file and the error bound e is equal to 0.25. The query type is P2P. 

The program assumes that there is a poilist.txt file under the same folder. poilist.txt contains the information of the POIs to be indexed.

The format of the poilist.txt: 

the number of POIs

"the ID of the 1st POI in the terrain data" "the ID of the 2nd POI in the terrain data" ...... 

# Experimental Result

The program will save result in a file "data-file-name_mano.txt". 

The result contains "error bound" "query time (micro seconds)"  "error". 

In the aforementioned example, the result file is "small_terrain.off_mano.txt".


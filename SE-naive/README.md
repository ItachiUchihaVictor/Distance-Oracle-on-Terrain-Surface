# Point-to-Point Distance Oracle on Terrain Surface (SE-naive)

The main source code is main.cpp. This is the implementation of a baseline algorithm SE-naive in our paper.

# Compiling Command

g++4 -o main main.cpp -std=c++11

# How to Use It

main "terrain_data" s

The 2nd parameter is the terrain data file and the third parameter is the separation factor. 

The relationship between the separation factor and the error bound. 

e = 0.05 <=> s = 40 

e = 0.1 <=> s = 20

e = 0.15 <=> s = 16

e = 0.2 <=> s = 10

e = 0.25 <=> s = 8

An example: 

main small_terrain.off 8

In this example, samll_terrain.off is the terrain data file and 8 is the separation factor and the error bound e is equal to 0.25.

The program assumes that there is a poilist.txt file under the same folder. poilist.txt contains the information of the POIs to be indexed.

The format of the poilist.txt: 

the number of POIs

"the ID of the 1st POI in the terrain data" "the ID of the 2nd POI in the terrain data" ...... 

# Experimental Result

The program will save result in a file "data-file-name.txt". 

The result contains "error bound" "preprocessing time (seconds)" "query time (micor seconds)" "space (MB)" "error". 

In the aforementioned example, the result file is "small_terrain.off.txt".


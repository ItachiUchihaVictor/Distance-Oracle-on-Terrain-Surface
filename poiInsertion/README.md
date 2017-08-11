# Insert POIs into a Terrain Surface

The main source code is main.cpp. 

# Compiling Command

g++4 -o main main.cpp -std=c++11

# How to Use It

main "original_terrain_data" "a_list_containing_x_y_coordinates" "output_terrain_data" "POI_index_list(output)"

The 2nd parameter is the input terrain data file, the third parameter is a list containing x and y coodinates of all POIs, the fourth parameter is the output terrain data with POIs inserted and the last parameter is a list containing the indexes of all POIs in the new terrain data. 


An example: 

main small_terrain.off xycoor.txt newterrain.off poilist.txt

In this example, samll_terrain.off is the input terrain data file, xycoor.txt is a listing containing x and y coodinates of all POIs, newterrain.off is the output terrain data with POIs inserted and poilist.txt is a list containing the indexes of all POIs in the new terrain data. 

The format of the xycoor.txt

"Latitude of the 1st POI" "Longitude of the 1st POI"
"Latitude of the 2nd POI" "Longitude of the 2nd POI"
......

The format of the poilist.txt: 

the number of POIs

"the ID of the 1st POI in the terrain data" "the ID of the 2nd POI in the terrain data" ...... 

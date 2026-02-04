# A Tool for Generating POIs 

The tool generate POIs based on a list of given POIs on a given terrain surface. The details of the generation could be found in our paper. 

The main source code is main.cpp. 

# Compiling Command

g++4 -o main main.cpp -std=c++11

# How to Use It

main "original_terrain_data" "a_list_containing_x_y_coordinates_of_given_POIs" "number_of_POIs_to_be_generated"

The 2nd parameter is the input terrain data file, the third parameter is a list containing x and y coodinates of all given POIs, the fourth parameter is the number of POIs to be generated. 

The program will output a file called "number_of_POIs_to_be_generated.txt" containing the x and y coordinates of all generated POIs.

An example: 

main small_terrain.off xycoor.txt 1000

In this example, samll_terrain.off is the input terrain data file, xycoor.txt is a listing containing x and y coodinates of all given POIs 
and 1000 is the number of POIs to be generated. 

The program will output a file "1000.txt" containing the x and y coordinates of all generated POIs. 

The format of the xycoor.txt and 1000.txt:

"Latitude of the 1st POI" "Longitude of the 1st POI"

"Latitude of the 2nd POI" "Longitude of the 2nd POI"

......


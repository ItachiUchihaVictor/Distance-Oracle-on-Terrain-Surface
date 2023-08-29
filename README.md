# Distance-Oracle-on-Terrain-Surface

The project provides the implementation of the distance oracle on terrain surface stated in Reference. Please kindly cite the paper in the Reference if you used our code. 

There are three different types of distance queries on terrain surface, namely Vertex-to-Vertex (V2V) distance queries, Point-to-Point (P2P) distance queries and Arbitrary Point-to-Arbitrary Point (A2A) distance queries. We refer the readers to the paper in Reference for more details. 

There are three sub-projects on V2V, P2P and A2A distance quereis, respectively. Besides, there is another sub-project "poiInsertion" for inserting Point-of-Interest (POIs) into a terrain surface and another sub-project "scale_up" for scale the size of the terrain up by inserting a vertex at the center of each face (as stated in the experiment of the paper). There are also some other sub-projects and we refer the readers to our paper for the details.

# Reference

"Distance Oracle on Terrain Surface", Victor Junqiu Wei, Raymond Chi-Wing Wong, Cheng Long, David M. Mount. SIGMOD 2017, Chicago, USA.

# How to Compile the Code 

Execute the script "Compile".

# How to Prepare the Datasets

First download the file through the following link:

[https://www.dropbox.com/s/ofa9ddk138x91w3/dataset.tar.gz?dl=0](https://www.dropbox.com/scl/fi/ykutpuruep1996i1gx51k/TerrainDataset.zip?rlkey=9mm5b0qy4o59s5isx7vj49jts&dl=0)

Then, unzip the file and go to the folder "dataset/" and then execute the script "preprocessing". Note that to smoothly run all experiment, the folder "dataset/" must be put under the top-most folder of this project (i.e., the top-most folder is the folder containing the script "Compile").

Data Format:

We used the .off format in the experiment. The content of the .off file is as follows: 

OFF

Number_of_vertices Number_of_faces Number_of_edges

x_coordinate_of_1st_vertex y_coordinate_of_1st_vertex z_coordinate_of_1st_vertex

x_coordinate_of_2nd_vertex y_coordinate_of_2nd_vertex z_coordinate_of_2nd_vertex

......

x_coordinate_of_the_last_vertex y_coordinate_of_the_last_vertex z_coordinate_of_the_last_vertex

ID_of_the_1st_vertex_of_the_1st_face ID_of_the_2nd_vertex_of_the_1st_face ID_of_the_3td_vertex_of_the_1st_face

ID_of_the_1st_vertex_of_the_2nd_face ID_of_the_2nd_vertex_of_the_2nd_face ID_of_the_3td_vertex_of_the_2nd_face

......

ID_of_the_1st_vertex_of_the_last_face ID_of_the_2nd_vertex_of_the_last_face ID_of_the_3td_vertex_of_the_last_face

Each .off data could be visualized by the terrain tool (http://rwcpu1.cse.ust.hk/terrain/).

# How to Perform Experiment

Under the folder "result", for each experimental figure in our paper, there is a sub-folder named after the name of the corresponding experimental figure, e.g. "Figure8/". In each sub-folder, there is a README.md which tells you how to perform the corresponding experiment. 

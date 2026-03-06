# Distance-Oracle-on-Terrain-Surface

Please find our most updated implementation of this algorithm (SE-Oracle) in the repository: 

https://github.com/ItakEjgo/weighted_distance_oracle

There are three different types of distance queries on terrain surface, namely Vertex-to-Vertex (V2V) distance queries, Point-to-Point (P2P) distance queries and Arbitrary Point-to-Arbitrary Point (A2A) distance queries. We refer the readers to the paper in Reference for more details. Note that in the repository above, the default setting is A2A query and please feel free to modify the code a bit if you need P2P or V2V queries. 


# Reference

"Distance Oracle on Terrain Surface", Victor Junqiu Wei, Raymond Chi-Wing Wong, Cheng Long, David M. Mount. SIGMOD 2017, Chicago, USA.

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

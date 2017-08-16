To perform experiment, you first need to generate a high resolution BH dataset by scaling up the BH_high.off 
and then use the terrain tool to generate 5 terrain datasets with 0.5M, 1M, 1.5M, 2M and 2.5M vertices, respectively, 
each of which corresponds to a sub-region of the high resolution BH dataset. Next, put the 5 datasets under this folder.
The terrain tool could be found through the following link: 

http://rwcpu1.cse.ust.hk/terrain/

The "scale up" program could be found in the ../../scale_up folder. The BH_high.off is under the ../../dataset/BH_high folder.

Then, you execute the script "exp" to get the experimental result. 

Finally, plot the figure with the gnuplot script (.plt file). 

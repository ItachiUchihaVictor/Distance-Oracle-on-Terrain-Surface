
#The template for generating multiple plots
#sharing the same legend in the same figure 

#The figure setting (applied to all plots)
#=========================================

set out "n_syn_sf_POI.eps"
set terminal postscript portrait enhanced mono "Helvetica" 28

set size 2.80000, 0.6300000
set pointsize 3.000000

set multiplot layout 2,4
#The first plot (which generates the common legend)
#===========================================================
#Note the NaN variable.

set size 2.000000,0.100000;  
set origin 0,0.58;

set key center top horizontal samplen 2
set yrange [0.0000000001:0.0000000002]

unset border
unset tics
#unset label

plot NaN title 'SE' with linespoints linetype 1 pointtype 1, \
NaN title 'SP-Oracle' with linespoints linetype 1 pointtype 10, \
NaN title 'K-Algo' with linespoints linetype 1 pointtype 6
#NaN title 'Theoretical bound ({/Symbol e})' with lines linetype 2
#NaN title 'BL2' with linespoints linetype 1 pointtype 8, \
#NaN title 'SE' with linespoints linetype 1 pointtype 2, \
#, \NaN title 'Hybrid' with linespoints linetype 1 pointtype 6, \
#NaN title 'BL1' with linespoints linetype 1 pointtype 4, \
#NaN title 'Cao-Appro2-Adapt2' with linespoints linetype 1 pointtype 12, \
#NaN title 'Long-Appro-Adapt2' with linespoints linetype 1 pointtype 14
 
set border
set tics
#set label

#The 2nd plot (notitle)
#=========================================
set size 0.7500000,0.6000000;  ;  
set origin 0.0,0.0;

set xlabel  "n (k)"
set ylabel  "Building Time (s)"
set key above

set xrange [55: 185]
set yrange [100000: 10000000]
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5
set format x "%g"
set format y "10^{%T}"

set xtics ("60" 60, "90" 90, "120" 120, "150" 150, "180" 180)
set log y

plot"SE.txt"  using 2:3 notitle with linespoints linetype 1 pointtype 1, \
"sp-oracle.txt"  using 2:4 notitle with linespoints linetype 1 pointtype 10
#"n_syn_sf.txt"  using 1: notitle with linespoints linetype 1 pointtype 2, \
#, \"n_syn_sf.txt"  using 1:7 notitle with linespoints linetype 1 pointtype 6, \
#"n_syn_sf.txt"  using 1:12 notitle with linespoints linetype 1 pointtype 4, \
#"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"n_syn_sf.txt"  using 1:9 notitle with linespoints linetype 1 pointtype 10, \
#
#"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14


#The 4th plot (notitle)
#=========================================
set size 0.7500000,0.6000000;  ;  
set origin 1.40,0.0;

set xlabel  "n (k)"
set ylabel  "Query Time (ms)"
set key above

set xrange [55: 185]
set yrange [0.50: 3000000]
set label 11 center at graph 0.5,char 1 "(c)" 
set bmargin 5
set format x "%g"
#set format y "10^{%T}"

set xtics ("60" 60, "90" 90, "120" 120, "150" 150, "180" 180)
set log y

plot "SE.txt"  using 2:($4/1000) notitle with linespoints linetype 1 pointtype 1, \
"mano.txt"  using 2:($4/1000) notitle with linespoints linetype 1 pointtype 6, \
"sp-oracle.txt"  using 2:($6/1000) notitle with linespoints linetype 1 pointtype 10
#, \"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 6, \
#"n_syn_sf.txt"  using 1:13 notitle with linespoints linetype 1 pointtype 4, \
#"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 2, \
#"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"n_syn_sf.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 12, \
#"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14

#The 5th plot (notitle)
#=========================================
set size 0.7500000,0.6000000;  ;  
set origin 0.70,0.0;

set xlabel  "n (k)"
set ylabel  "Size (MB)"
set key above

set xrange [55: 185]
set yrange [100.000: 100000.000]
set label 11 center at graph 0.5,char 1 "(b)" 
set bmargin 5
set format x "%g"
#set format y "10^{%T}"

set xtics ("60" 60, "90" 90, "120" 120, "150" 150, "180" 180)
set log y

plot"SE.txt"  using 2:5 notitle with linespoints linetype 1 pointtype 1, \
"sp-oracle.txt"  using 2:($5/1000) notitle with linespoints linetype 1 pointtype 10
#"n_syn_sf.txt"  using 1:15 notitle with linespoints linetype 1 pointtype 4, \
#"n_syn_sf.txt"  using 1:12 notitle with linespoints linetype 1 pointtype 2, \
#"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"n_syn_sf.txt"  using 1:9 notitle with linespoints linetype 1 pointtype 10, \
#"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14

#The 6th plot (notitle)
#=========================================
set size 0.7500000,0.6000000;  ;  
set origin 2.10,0.0;

set xlabel  "n (k)"
set ylabel  "Error"
set key above

set xrange [55: 185]
set yrange [0.0050: 0.29]
set label 11 center at graph 0.5,char 1 "(d)" 
set bmargin 5
set format x "%g"
set format y "10^{%T}"
unset log y
set xtics ("60" 60, "90" 90, "120" 120, "150" 150, "180" 180)

set ytics ("0.05" 0.05, "0.1" 0.1, "0.15" 0.15, "0.2" 0.2, "0.25" 0.25)

plot "SE.txt"  using 2:6 notitle with linespoints linetype 1 pointtype 1, \
"mano.txt"  using 2:5 notitle with linespoints linetype 1 pointtype 6, \
"sp-oracle.txt"  using 2:7 notitle with linespoints linetype 1 pointtype 10, \
x notitle with lines linetype 2

#plot "n_syn_sf.txt"  using 1:6 notitle with linespoints linetype 1 pointtype 1, \
#"n_syn_sf.txt"  using 1:21 notitle with linespoints linetype 1 pointtype 10, \
#0.1 notitle with lines linetype 2
#, \"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 6, \
#"n_syn_sf.txt"  using 1:16 notitle with linespoints linetype 1 pointtype 4, \
#"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 2, \
#"n_syn_sf.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"n_syn_sf.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 12, \
#"n_syn_sf.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14

unset multiplot

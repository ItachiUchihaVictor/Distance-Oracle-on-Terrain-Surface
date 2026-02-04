
#The template for generating multiple plots
#sharing the same legend in the same figure 

#The figure setting (applied to all plots)
#=========================================

set out "e_a2a.eps"
set terminal postscript portrait enhanced mono "Helvetica" 35 

set size 3.620000, 0.7500000
set pointsize 3.5000000

set multiplot layout 2,4
#The first plot (which generates the common legend)
#===========================================================
#Note the NaN variable.

set size 3.6000000,0.100000;  
set origin 0,0.69;

set key center top horizontal samplen 2
set yrange [0.0000000001:0.0000000002]

unset border
unset tics
#unset label

plot NaN title 'SE' with linespoints linetype 1 pointtype 1, \
NaN title 'SP-Oracle' with linespoints linetype 1 pointtype 10, \
NaN title 'K-Algo' with linespoints linetype 1 pointtype 6
#NaN title 'Theoretical bound ({/Symbol e})' with lines linetype 2
#NaN title 'SE' with linespoints linetype 1 pointtype 2, \
#NaN title 'Hybrid' with linespoints linetype 1 pointtype 6, \
#, \NaN title 'RN-Adapt' with linespoints linetype 1 pointtype 4, \
#NaN title 'Cao-Appro2-Adapt2' with linespoints linetype 1 pointtype 12, \
#NaN title 'Long-Appro-Adapt2' with linespoints linetype 1 pointtype 14
 
set border
set tics
#set label

#The 2nd plot (notitle)
#=========================================
set size 1.00000,0.7000000;  
set origin 0.0,0.0;

set xlabel  "{/Symbol e}"
set ylabel  "Building Time (s)"
set key above

set xrange [0.040000: 0.26000000]
set yrange [50000: 10000000]
set label 11 center at graph 0.5,char 1 "(a)" 
set bmargin 5
set format x "%g"
set format y "10^{%T}"

set xtics ( "0.05" 0.05, "0.1" 0.1, "0.15" 0.15, "0.2" 0.2, "0.25" 0.25)

set log y

plot"SE.txt"  using 1:3 notitle with linespoints linetype 1 pointtype 1, \
"sp-oracle.txt"  using 1:4 notitle with linespoints linetype 1 pointtype 10

#"e_small.txt"  using 1: notitle with linespoints linetype 1 pointtype 2, \
#, \"e_small.txt"  using 1:7 notitle with linespoints linetype 1 pointtype 6, \
#"e_small.txt"  using 1:12 notitle with linespoints linetype 1 pointtype 4, \
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"e_small.txt"  using 1:9 notitle with linespoints linetype 1 pointtype 10, \
#"e_small.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 12, \
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14
#"e_small.txt"  using 1:22 notitle with linespoints linetype 1 pointtype 2, \

#The 4th plot (notitle)
#=========================================
set size 1.00000,0.700000;  
set origin 0.90,0.0;

set xlabel  "{/Symbol e}"
set ylabel  "Size (MB)"
set key above

set xrange [0.040000: 0.26000000]
set yrange [1000.000: 100000.000]
set label 11 center at graph 0.5,char 1 "(b)" 
set bmargin 5
set format x "%g"
#set format y "10^{%T}"

set xtics ( "0.05" 0.05, "0.1" 0.1, "0.15" 0.15, "0.2" 0.2, "0.25" 0.25)

set log y

plot"SE.txt"  using 1:5 notitle with linespoints linetype 1 pointtype 1, \
"sp-oracle.txt"  using 1:($5/1000) notitle with linespoints linetype 1 pointtype 10
#"e_small.off.txt"  using 1:($4*1.05) notitle with linespoints linetype 1 pointtype 8
#, \"e_small.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 6, \
#"e_small.txt"  using 1:15 notitle with linespoints linetype 1 pointtype 4, \
#"e_small.txt"  using 1:12 notitle with linespoints linetype 1 pointtype 2, \
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"e_small.txt"  using 1:9 notitle with linespoints linetype 1 pointtype 10, \
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14
#"e_small.txt"  using 1:25 notitle with linespoints linetype 1 pointtype 2, \

#The 5th plot (notitle)
#=========================================
set size 1.00000,0.7000000;  
set origin 1.80,0.0;

set xlabel  "{/Symbol e}"
set ylabel  "P2P Query Time (ms)"
set key above

set xrange [0.040000: 0.26000000]
set yrange [0.50: 30000000]
set label 11 center at graph 0.5,char 1 "(c)" 
set bmargin 5
set format x "%g"
#set format y "10^{%T}"

set xtics ( "0.05" 0.05, "0.1" 0.1, "0.15" 0.15, "0.2" 0.2, "0.25" 0.25)

set log y

plot "SE.txt"  using 1:($4/1000) notitle with linespoints linetype 1 pointtype 1, \
"mano.txt"  using 1:($4/1000) notitle with linespoints linetype 1 pointtype 6, \
"sp-oracle.txt"  using 1:($6/1000) notitle with linespoints linetype 1 pointtype 10
#, \"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 6, \
#"e_small.txt"  using 1:13 notitle with linespoints linetype 1 pointtype 4, \
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 2, \
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"e_small.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 12, \
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 2, \

#The 6th plot (notitle)
#=========================================
set size 1.00000,0.7000000;  
set origin 2.70,0.0;

set xlabel  "{/Symbol e}"
set ylabel  "A2A Query Time (ms)"
set key above

set xrange [0.040000: 0.26000000]
set yrange [0.50: 30000000]
set label 11 center at graph 0.5,char 1 "(d)" 
set bmargin 5
set format x "%g"
#set format y "10^{%T}"
set xtics ( "0.05" 0.05, "0.1" 0.1, "0.15" 0.15, "0.2" 0.2, "0.25" 0.25)


plot "SE.txt"  using 1:($7/1000) notitle with linespoints linetype 1 pointtype 1, \
"mano.txt"  using 1:($4/1000) notitle with linespoints linetype 1 pointtype 6, \
"sp-oracle.txt"  using 1:($6/1000) notitle with linespoints linetype 1 pointtype 10
#x notitle with lines linetype 2
#, \"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 6, \
#"e_small.txt"  using 1:16 notitle with linespoints linetype 1 pointtype 4, \
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 2, \
#"e_small.txt"  using 1:8 notitle with linespoints linetype 1 pointtype 8, \
#"e_small.txt"  using 1:10 notitle with linespoints linetype 1 pointtype 12, \
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 14
#"e_small.txt"  using 1:11 notitle with linespoints linetype 1 pointtype 2, \

unset multiplot

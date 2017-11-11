# Note you need gnuplot 4.4 for the pdfcairo terminal.

set terminal pdfcairo font "Gill Sans,9" linewidth 2 rounded fontscale 1.0

# Line style for axes
set style line 80 lt rgb "#808080"

# Line style for grid
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey

set grid back linestyle 81
set border 3 back linestyle 80 # Remove border on top and right.  These
             # borders are useless and make it harder
             # to see plotted lines near the border.
    # Also, put it in grey; no need for so much emphasis on a border.
set xtics nomirror
set ytics nomirror

#set log x
#set mxtics 10    # Makes logscale look good.

# Line styles: try to pick pleasing colors, rather
# than strictly primary colors or hard-to-see colors
# like gnuplot's default yellow.  Make the lines thick
# so they're easy to see in small plots in papers.
set style line 1 lt rgb "#A00000" lw 2 pt 1
set style line 2 lt rgb "#00A000" lw 2 pt 6
set style line 3 lt rgb "#5060D0" lw 2 pt 2
set style line 4 lt rgb "#F25900" lw 2 pt 9
set style line 5 lt rgb "#5F209B" lw 2 pt 4
set style line 6 lt rgb "#3D98AA" lw 2 pt 3
set style line 7 lt rgb "#979281" lw 2 pt 7
set style line 8 lt rgb "#D55258" lw 2 pt 5

set key Left reverse samplen 3 font ",8"
set key inside horizontal top center
# set key inside vertical top left reverse Left
set mxtics 10
set logscale x


set output "agg/agg-simdrownum.pdf"
# Get the directory of the benchmark
set xlabel "\# K-V arrays" offset 0,0,0
set ylabel "Update percentage (%)" offset 1,0,0

set ytics 10
set xtics 1000


set autoscale x
set autoscale y

unset logscale x

# set style arrow 1 lt 3 lc rgb "#AAAAAA" lw 1 nohead 
# set arrow from 10,4.856 to 10000,4.856 as 1 dt 4

set arrow from 10,4.856 to 5000,4.856 nohead ls 3


# set style label lt rgb "#808080"
# set label 'Ideal' at 100.0,8.0 rotate by 90

set label 'Ideal' at 200.0,7.0 font ", 8pt" textcolor rgb "#5060D0"
# set arrow from 100,4.856 to 5000,4.856 nohead ls 81

# set yrange[0:]
plot './draw_data/agg-simdrownum.csv'	using 1:($2*100) title 'LRU' w lp ls 2, \
	 './draw_data/agg-simdrownum.csv'	using 1:($3*100) title 'GRR' w lp ls 4






set style data histogram
set style histogram clustered gap 1
# set style fill transparent pattern 1 border
set style fill solid 0.4 border



# set key Left reverse samplen 3 font ",8"
set key inside horizontal top center

set output "agg/agg-flowid.pdf"
# Get the directory of the benchmark
unset xlabel
# set xlabel "Different flow IDs" offset 0,0,0
set ylabel "Update percentage (%)" offset 1,0,0



# set autoscale x
# set autoscale y
set ytics 4
set yrange [:20]

set xtics rotate by 15 offset -2.3,-1.2,0

unset logscale x

plot './draw_data/agg-flowid.csv' using ($2*100):xticlabels(1) title "LRU" lt rgb "#00A000" fs pattern 1,\
	 '' using ($3*100):xticlabels(1) title "GRR" lt rgb "#F25900" fs pattern 2, \
	 '' using ($4*100):xticlabels(1) title "Ideal" lt rgb "#5060D0" fs pattern 0


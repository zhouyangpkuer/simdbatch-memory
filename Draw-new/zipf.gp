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
set key inside horizontal bottom center
# set key inside vertical top left reverse Left
set mxtics 10
# set logscale x


set output "zipf/speed-zipf.pdf"
# Get the directory of the benchmark
set xlabel "Dataset skewness" offset 0,0,0
set ylabel "Throughput (Mpps)" offset 1,0,0

set ytics 4
# set xtics 1000
set yrange[0:]


# set style arrow 1 lt 3 lc rgb "#AAAAAA" lw 1 nohead 
# set arrow from 10,4.856 to 10000,4.856 as 1 dt 4
# set arrow from 10,4.856 to 5000,4.856 nohead ls 3


# set style label lt rgb "#808080"
# set label 'Ideal' at 100.0,8.0 rotate by 90

# set label 'Ideal' at 200.0,7.0 font ", 8pt" textcolor rgb "#5060D0"
# set arrow from 100,4.856 to 5000,4.856 nohead ls 81

plot './draw_data/speed-zipf.csv'	using 1:($2/1000000.0) title 'w/o aggregator' w lp ls 3, \
	 './draw_data/speed-zipf.csv'	using 1:($4/1000000.0) title 'w/ aggregator' w lp ls 2






set key Left reverse samplen 3 font ",8"
set key inside horizontal bottom center
# set key inside vertical top left reverse Left
set mxtics 10
# set logscale x


set output "zipf/99tail-zipf.pdf"
# Get the directory of the benchmark
set xlabel "Dataset skewness" offset 0,0,0
set ylabel "99th Latency (ns)" offset 1,0,0

set ytics 40
# set xtics 1000
set yrange[0:]


# set style arrow 1 lt 3 lc rgb "#AAAAAA" lw 1 nohead 
# set arrow from 10,4.856 to 10000,4.856 as 1 dt 4
# set arrow from 10,4.856 to 5000,4.856 nohead ls 3


# set style label lt rgb "#808080"
# set label 'Ideal' at 100.0,8.0 rotate by 90

# set label 'Ideal' at 200.0,7.0 font ", 8pt" textcolor rgb "#5060D0"
# set arrow from 100,4.856 to 5000,4.856 nohead ls 81

plot './draw_data/99tail-zipf.csv'	using 1:($2/2.2) title 'w/o aggregator' w lp ls 3, \
	 './draw_data/99tail-zipf.csv'	using 1:($3/2.2) title 'w/ aggregator' w lp ls 2







set key Left reverse samplen 3 font ",8"
set key inside horizontal bottom center
# set key inside vertical top left reverse Left
set mxtics 10
# set logscale x


set output "zipf/avgtail-zipf.pdf"
# Get the directory of the benchmark
set xlabel "Dataset skewness" offset 0,0,0
set ylabel "Avg. Latency (ns)" offset 1,0,0

set ytics 20
# set xtics 1000
set yrange[0:]


# set style arrow 1 lt 3 lc rgb "#AAAAAA" lw 1 nohead 
# set arrow from 10,4.856 to 10000,4.856 as 1 dt 4
# set arrow from 10,4.856 to 5000,4.856 nohead ls 3


# set style label lt rgb "#808080"
# set label 'Ideal' at 100.0,8.0 rotate by 90

# set label 'Ideal' at 200.0,7.0 font ", 8pt" textcolor rgb "#5060D0"
# set arrow from 100,4.856 to 5000,4.856 nohead ls 81

plot './draw_data/avgtail-zipf.csv'	using 1:($2/2.2) title 'w/o aggregator' w lp ls 3, \
	 './draw_data/avgtail-zipf.csv'	using 1:($3/2.2) title 'w/ aggregator' w lp ls 2









set key Left reverse samplen 3 font ",8"
set key inside horizontal bottom center
# set key inside vertical top left reverse Left
set mxtics 10
# set logscale x


set output "zipf/latency-zipf.pdf"
# Get the directory of the benchmark
set xlabel "Dataset skewness" offset 0,0,0
set ylabel "Latency (ns)" offset 1,0,0

set ytics 20
# set xtics 1000
set yrange[0:]


# set style arrow 1 lt 3 lc rgb "#AAAAAA" lw 1 nohead 
# set arrow from 10,4.856 to 10000,4.856 as 1 dt 4
# set arrow from 10,4.856 to 5000,4.856 nohead ls 3


# set style label lt rgb "#808080"
# set label 'Ideal' at 100.0,8.0 rotate by 90

# set label 'Ideal' at 200.0,7.0 font ", 8pt" textcolor rgb "#5060D0"
# set arrow from 100,4.856 to 5000,4.856 nohead ls 81

plot './draw_data/avgtail-zipf.csv'	using 1:($2/2.2) title 'w/o aggregator' w lp ls 3, \
	 './draw_data/avgtail-zipf.csv'	using 1:($3/2.2) title 'w/ aggregator' w lp ls 2, \
     './draw_data/avgtail-zipf.csv' using ($1):($2/2.2):($2/2.2):($4/2.2) notitle w errorbars ls 3 lw 1,\
     './draw_data/avgtail-zipf.csv' using ($1):($3/2.2):($3/2.2):($5/2.2) notitle w errorbars ls 2 lw 1,\






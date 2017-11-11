# Note you need gnuplot 4.4 for the pdfcairo terminal.

set terminal pdfcairo font "Gill Sans,6.5" linewidth 2 rounded fontscale 1.0 size 6,2
# set size ratio 0.3


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




set style data histogram
set style histogram clustered gap 1
# set style fill transparent pattern 1 border
set style fill solid 0.4 border


set key inside horizontal top center reverse


set output "./nine/speed-nine.pdf"
# Get the directory of the benchmark
unset xlabel
# set xlabel "Different flow IDs" offset 0,0,0
set ylabel "Throughput (Mpps)" offset 1,0,0



# set autoscale x
# set autoscale y

set ytics 5
set yrange [0:20]

set xtics rotate by 15 offset -5.0,-1.2,0

unset logscale x

plot './draw_data/speed-nine.csv' using ($2/1000000):xticlabels(1) title "w/o aggregator" lt rgb "#5060D0" fs pattern 6,\
	 '' using ($4/1000000):xticlabels(1) title "w/ aggregator" lt rgb "#00A000" fs pattern 1

	 # '' using ($3/1000000):xticlabels(1) title columnheader(3) lt rgb "#5060D0" fs pattern 0, \









set output "./nine/99tail-nine.pdf"
# Get the directory of the benchmark
unset xlabel
# set xlabel "Different flow IDs" offset 0,0,0
set ylabel "99th latency (ns)" offset 1,0,0



# set autoscale x
# set autoscale y

set ytics 100
set yrange [0:500]

set xtics rotate by 15 offset -5.0,-1.2,0

unset logscale x
# set log y

plot './draw_data/99tail-nine.csv' using (($2/(2.2))):xticlabels(1) title "w/o aggregator" lt rgb "#5060D0" fs pattern 6,\
	 '' using (($4/(2.2))):xticlabels(1) title "w/ aggregator" lt rgb "#00A000" fs pattern 1
	 # '' using (($3/(2.2))):xticlabels(1) title columnheader(3) lt rgb "#5060D0" fs pattern 0, \







set output "./nine/avgtail-nine.pdf"
# Get the directory of the benchmark
unset xlabel
# set xlabel "Different flow IDs" offset 0,0,0
set ylabel "Avg. latency (ns)" offset 1,0,0



# set autoscale x
# set autoscale y

set ytics 100
set yrange [0:500]

set xtics rotate by 15 offset -5.0,-1.2,0

unset logscale x
# set log y

plot './draw_data/avgtail-nine.csv' using (($2/(2.2))):xticlabels(1) title "w/o aggregator" lt rgb "#5060D0" fs pattern 6,\
	 '' using (($4/(2.2))):xticlabels(1) title "w/ aggregator" lt rgb "#00A000" fs pattern 1
	 # '' using (($3/(2.2))):xticlabels(1) title columnheader(3) lt rgb "#5060D0" fs pattern 0, \







set style histogram errorbars gap 1 lw 1
set style data histogram
set boxwidth 1 absolute



set output "./nine/latency-nine.pdf"
# Get the directory of the benchmark
unset xlabel
# set xlabel "Different flow IDs" offset 0,0,0
set ylabel "Latency (ns)" offset 1,0,0



# set autoscale x
# set autoscale y

set ytics 100
set yrange [0:400]

set xtics rotate by 15 offset -5.0,-1.2,0

unset logscale x
# set log y

plot './draw_data/avgtail-nine.csv' using ($2/2.2):($5/2.2):($2/2.2):xticlabels(1) title "w/o aggregator" lt rgb "#5060D0" fs pattern 6,\
	 '' using ($4/2.2):($7/2.2):($4/2.2):xticlabels(1) title "w/ aggregator" lt rgb "#00A000" fs pattern 1
	 # '' using (($3/(2.2))):xticlabels(1) title columnheader(3) lt rgb "#5060D0" fs pattern 0, \

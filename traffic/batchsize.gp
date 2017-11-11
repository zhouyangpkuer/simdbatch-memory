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

# set key samplen 2 font ",8"

# set key inside horizon bottom center
set logscale x

# set mxtics 10
# set yrange [0:200]
# set xrange [1:1024]

set ylabel "Avg. #flow / #packets" offset 0,0,0
set xlabel "Batch size (#packet)" offset 0,0,0
# Get the directory of the benchmark
set output "flownum_batchsize.pdf"
plot \
     'flownum_batchsize.txt' using ($1):($5) title "sip+dip" w lp ls 3,\
     'flownum_batchsize.txt' using ($1):($3) title "5-tuple" w lp ls 2,\


# set key samplen 2 font ",8"

set key inside vertical top left
# set logscale x

# set mxtics 10
# set yrange [0:200]
# set xrange [1:1024]
set ytics 20

set ylabel "Latency (ns)" offset 0,0,0
set xlabel "Batch size (#packet)" offset 0,0,0
# Get the directory of the benchmark
set output "latency_batchsize.pdf"
plot \
     'latency_batchsize.txt' using ($1):($3/2.2) title "sip+dip" w lp ls 3,\
     'latency_batchsize.txt' using ($1):($2/2.2) title "5-tuple" w lp ls 2,\
     'latency_batchsize.txt' using ($1):($3/2.2):($3/2.2):($5/2.2) notitle w errorbars ls 3 lw 1,\
     'latency_batchsize.txt' using ($1):($2/2.2):($2/2.2):($4/2.2) notitle w errorbars ls 2 lw 1,\


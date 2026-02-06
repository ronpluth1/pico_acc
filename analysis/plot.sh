#!/usr/bin/gnuplot

# set term jpeg size 2000,1200
set term qt persist
# set output "foo.jpg"
set lt 1 lc rgb "red" lw 2 pt 7

set xdata time
set timefmt "%s"
set format x "%m/%d %H:%M:%S"
set xlabel "Time"
set ylabel "Value"
set title "Data vs. Time Plot"
set boxwidth 3

plot "processed_log.txt" using 1:2 with boxes lt 1 title "My Data" watch mouse
pause mouse close

# plot 'processed_log.txt' title "401k" axis x1y1 with lines lt 2 watch mouse


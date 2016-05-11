set xrange [1000:*]
set style data points
#plot 'trolley840.dat' using 22 pt 7 lc -1, 'trolley840.dat' using 23 pt 6 lc -1
plot 'trolley840.dat' using 1:22 w dot lc -1, 'trolley840.dat' using 1:23 w dot lc 4
#plot 'head.dat' using 1:21
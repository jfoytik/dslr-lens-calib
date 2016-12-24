set terminal png size 1600,1200 enhanced font "Helvetica,20"
set output 'calibration_results.png'
set autoscale
set offsets graph 0.05, 0.05, 0, 0
unset key

plot "aggregated_results.dat" using 1:2 pt 7 ps 2,\
     "ref.dat" using 1:2 pt 7 ps 2

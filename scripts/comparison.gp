reset                                                                           
set ylabel 'variations'
set style fill solid 
set title 'hash fuction comparison'
set term png enhanced font 'Verdana,10'
set output 'comparison.png'

plot [:][:0.3] '30000(1).txt' using 3:xtic(1) with histogram title 'variations' ,\

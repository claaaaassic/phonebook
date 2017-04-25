reset
set ylabel 'the numbers of data in hash[index]'
set xlabel 'hash table'
set style fill solid
set title 'Distribution'
set term png enhanced font 'Verdana,10'
set output 'distribution.png'
set xtic 50000
set xrange [:]

plot [:][:25]'SDBM.txt' using 1:2 with points title 'SDBM',\
'RS.txt' using 1:2 with points title 'RS',\
'JS.txt' using 1:2 with points title 'JS',\
'PJW.txt' using 1:2 with points title 'PJW',\
'ELF.txt' using 1:2 with points title 'ELF',\
'BKDR.txt' using 1:2 with points title 'BKDR',\
'DJB.txt' using 1:2 with points title 'DJB',\
'AP.txt' using 1:2 with points title 'AP'


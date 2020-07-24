set xlabel "Run Number"
set ylabel "Buggy Tracks (limited denominator)"

set term postscript eps color solid

set title "Finding the ReEntry bug in data"

set output "data.eps"
plot [108000:134000][0:0.4] "lf_bemb_db5.log" t "Data 5","lf_bemb_db6.log" t "Data 6","lf_bemb_db7.log" t "Data 7","lf_bemb_db8.log" t "Data 8","lf_bemb_db9.log" t "Data 9","lf_bemb_db10.log" t "Data 10","lf_bemb_db11.log" t "Data 11","lf_bemb_db12.log" t "Data 12","lf_bemb_db13.log" t "Data 13","lf_bemb_db14.log" t "Data 14","lf_bemb_db15.log" t "Data 15","lf_bemb_db16.log" t "Data 16","lf_bemb_db17.log" t "Data 17","lf_bemb_db18.log" t "Data 18","lf_bemb_db19.log" t "Data 19","lf_bemb_db20.log" t "Data 20","lf_bemb_db21.log" t "Data 21","lf_bemb_db22.log" t "Data 22","lf_bemb_db23.log" t "Data 23","lf_bemb_db24.log" t "Data 24"

set title "Finding the ReEntry bug in Monte Carlo"

set output "bbar.eps"
plot [110000:118000][0:0.4] "lf_data6bbar.log" t "Data 6 BBar", "lf_data7bbar.log" t "Data 7 BBar", "lf_data8bbar.log" t "Data 8 BBar", "lf_data8bbar2.log" t "Data 8 BBar (2)", "lf_data11bbar.log" t "Data 11 BBar"

set output "u3s.eps"
plot [121200:123400][0:0.4] "lf_data16u3s.log" t "Data 16 U(3S)", "lf_data17u3s.log" t "Data 17 U(3S)"

set output "cont.eps"
plot [110000:130000][0:0.4] "lf_data6cont.log" t "Data 6 Continuum", "lf_data6cont2.log" t "Data 6 Continuum (2)", "lf_data7cont.log" t "Data 7 Continuum", "lf_data8cont.log" t "Data 8 Continuum", "lf_data8cont2.log" t "Data 8 Continuum (2)", "lf_data11cont.log" t "Data 11 Continuum", "lf_data11cont2.log" t "Data 11 Continuum (2)", "lf_data16cont.log" t "Data 16 Continuum", "lf_data16cont2.log" t "Data 16 Continuum (2)", "lf_data17cont.log" t "Data 17 Continuum"

set output "bhabha.eps"
plot [112800:112810][0:0.4] "lf_data7bhabha.log" t "Data 7 Bhabha"

set output "mupair.eps"
plot [112000:124000][0:0.4] "lf_data7mupair.log" t "Data 7 Mupair", "lf_data11mupair.log" t "Data 11 Mupair", "lf_data16mupair.log" t "Data 16 Mupair", "lf_data17mupair.log" t "Data 17 Mupair"

set output "tau.eps"
plot [110000:128000][0:0.4]"lf_data6tau.log" t "Data 6 Taupair", "lf_data7tau.log" t "Data 7 Taupair", "lf_data8tau.log" t "Data 8 Taupair", "lf_data11tau.log" t "Data 11 Taupair", "lf_data16tau.log" t "Data 16 Taupair", "lf_data17tau.log" t "Data 17 Taupair"

set output "dpilnu.eps"
plot [112200:114200][0:0.4] "lf_dpilnu.log" t "D to pi l nu", "lf_dpilnu2.log" t "D to pi l nu (2)"

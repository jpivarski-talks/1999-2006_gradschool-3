set xlabel "Run Number"
set term postscript eps color solid

set ylabel "Mean reduced chi2"
set output "meanredchi.eps"
plot "other_meanredchi.dat" t "Unused", "peak_meanredchi.dat" t "Peak", "cont_meanredchi.dat" t "Cont", "scan_meanredchi.dat" t "Scan", "high_meanredchi.dat" t "High"

set ylabel "Truncated mean reduced chi2"
set output "meancutredchi.eps"
plot "other_meancutredchi.dat" t "Unused", "peak_meancutredchi.dat" t "Peak", "cont_meancutredchi.dat" t "Cont", "scan_meancutredchi.dat" t "Scan", "high_meancutredchi.dat" t "High"

set ylabel "Reduced chi2 fraction above cut"
set output "fracredchi.eps"
plot "other_fracredchi.dat" t "Unused", "peak_fracredchi.dat" t "Peak", "cont_fracredchi.dat" t "Cont", "scan_fracredchi.dat" t "Scan", "high_fracredchi.dat" t "High"

set ylabel "Mean err cottheta"
set output "meanerrcotth.eps"
plot "other_meanerrcotth.dat" t "Unused", "peak_meanerrcotth.dat" t "Peak", "cont_meanerrcotth.dat" t "Cont", "scan_meanerrcotth.dat" t "Scan", "high_meanerrcotth.dat" t "High"

set ylabel "Truncated mean err cottheta"
set output "meancuterrcotth.eps"
plot "other_meancuterrcotth.dat" t "Unused", "peak_meancuterrcotth.dat" t "Peak", "cont_meancuterrcotth.dat" t "Cont", "scan_meancuterrcotth.dat" t "Scan", "high_meancuterrcotth.dat" t "High"

set ylabel "err cottheta fraction above cut"
set output "fracerrcotth.eps"
plot "other_fracerrcotth.dat" t "Unused", "peak_fracerrcotth.dat" t "Peak", "cont_fracerrcotth.dat" t "Cont", "scan_fracerrcotth.dat" t "Scan", "high_fracerrcotth.dat" t "High"

set ylabel "Mean err z0"
set output "meanerrz0.eps"
plot "other_meanerrz0.dat" t "Unused", "peak_meanerrz0.dat" t "Peak", "cont_meanerrz0.dat" t "Cont", "scan_meanerrz0.dat" t "Scan", "high_meanerrz0.dat" t "High"

set ylabel "Truncated mean err z0"
set output "meancuterrz0.eps"
plot "other_meancuterrz0.dat" t "Unused", "peak_meancuterrz0.dat" t "Peak", "cont_meancuterrz0.dat" t "Cont", "scan_meancuterrz0.dat" t "Scan", "high_meancuterrz0.dat" t "High"

set ylabel "err z0 fraction above cut"
set output "fracerrz0.eps"
plot "other_fracerrz0.dat" t "Unused", "peak_fracerrz0.dat" t "Peak", "cont_fracerrz0.dat" t "Cont", "scan_fracerrz0.dat" t "Scan", "high_fracerrz0.dat" t "High"

exit

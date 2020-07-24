(# - {5100, 6600})& /@ {{9491.501, 6066.659}, {5100, 9600}, {3896, 6745}, {4382, 3687}}

Out[5]= {{0, 3000}, {-1204, 145}, {-718, -2913}}

rot[n_] := {{Cos[n*2 Pi/13], Sin[n*2 Pi/13]}, {-Sin[n*2 Pi/13], Cos[n*2 Pi/13]}};

TableForm[Table[{N[#[[1]]], Round[#[[2]]], Round[#[[3]]], Round[#[[4]]]}&[(rot[n].(# - {5100, 6600}) + {5100, 6600})& /@ {{9491.501, 6066.659}, {5100, 9600}, {3896, 6745}, {4382, 3687}}], {n, 0, 12}], TableDepth->1]

9491.50 6066.66 5100 9600 3896 6745 4382 3687
8740.63 4086.92 6494 9256 4101 7288 3111 4354
7155.73 2682.89 7569 8304 4535 7673 2295 5536
5099.88 2176.23 8078 6962 5099 7813 2122 6962
3044.07 2683.00 7905 5536 5663 7674 2631 8304
1459.24 4087.11 7089 4354 6097 7290 3706 9257
708.471 6066.89 5818 3687 6304 6747 5100 9600
963.745 8168.80 4382 3687 6234 6171 6494 9257
2166.58 9911.32 3111 4354 5905 5693 7569 8304
4041.43 10895.3 2295 5536 5391 5423 8078 6962
6158.79 10895.2 2122 6962 4811 5422 7905 5536
8033.59 9911.16 2631 8304 4297 5691 7089 4354
9236.34 8168.58 3706 9256 3967 6169 5818 3687

Exit

[Mathematica finished.]
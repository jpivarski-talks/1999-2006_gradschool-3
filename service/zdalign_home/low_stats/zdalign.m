mat = {{aa, ab, ac, ad},
       {ba, bb, bc, bd},
       {ca, cb, cc, cd},
       {da, db, dc, dd}};

test0 = {0, 0, 0, 0};
testrotz = {0.001, 0, 0, 0};
testx = {0, 0.001, 0, 0};
testy = {0, 0, 0.001, 0};
testz = {0, 0, 0, 0.001};

(* with phish = (u+) + (v+) + (u-) + (v-) *)

res0 = {0.000482194, 2.01522*^-5, 0.00102265, -2.54564*^-5};
resrotz = {0.000716039, -6.14805*^-5, 0.00111589, -0.000123216};
resx = {0.000487503, -0.00242568, 0.00101449, -9.39095*^-5};
resy = {-0.000766776, 6.43981*^-5, 0.00359198, -0.000310914};
resz = {0.000475814, -3.16352*^-5, 0.00101873, -0.000120116};

sol = Solve[{(testrotz - test0) == mat . (resrotz - res0),
	     (testx - test0) == mat . (resx - res0),
	     (testy - test0) == mat . (resy - res0),
	     (testz - test0) == mat . (resz - res0)},
	    {aa, ab, ac, ad, ba, bb, bc, bd, ca, cb, cc, cd, da, db, dc, dd}];

MatrixForm[mat /. sol]

Out[7]//MatrixForm= 3.48827       -0.0517212    -0.131586     -3.16829
                    0.010699      -0.415446     -0.00126039   0.293182
                    1.66107       0.00761654    0.324831      -2.70055
                    -0.309748     0.230458      -0.00389338   -10.3992

pretend = DiagonalMatrix[{aa, bb, cc, dd}] /. sol

MatrixForm[pretend]

Out[16]//MatrixForm= 3.48827     0           0           0
                     0           -0.415446   0           0
                     0           0           0.324831    0
                     0           0           0           -10.3992

pretend . ({0, 0, 0, 0} - res0)

                                  -6
Out[20]= {{-0.00168202, 8.37216 10  , -0.000332188, -0.000264726}}

(mat /. sol) . ({0, 0, 0, 0} - res0)

Out[21]= {{-0.00338882, 0.0000313894, -0.000268812, 0.00401881}}

Exit

[Mathematica finished.]

values = {{3, 7, 4},
	  {9, 1, 6},
	  {6, 2, 2},
	  {5, 2, 6}};
nbinsX = 3;
nbinsY = 4;

(* This is translating Mathematica to C: v[a,b] returns what would in
   C be v[a][b]--- an ordinary array extraction *)
v[sbx_, sby_] := If[sbx != Floor[sbx]  ||  sby != Floor[sby], segfault,
		    If[sbx < 0  ||  sby < 0, segfault,
		       If[sbx >= nbinsX  ||  sby >= nbinsY, segfault,
			  values[[sby+1, sbx+1]]
			 ]]];

closest1[sb_, nbins_] := If[sb < 0, {0, 1},
			    If[sb >= nbins-1, {nbins-2, nbins-1},
			       If[sb == Floor[sb], {sb, sb+1},
				  {Floor[sb], Ceiling[sb]}
				 ]]];

interp1[sbx_, by_] :=
    Module[{x1, x2, f1, f2},
	   {x1, x2} = closest1[sbx, nbinsX];
	   {f1, f2} = {v[x1, by], v[x2, by]};
	   (f2 - f1)*(sbx - x1) + f1
	  ];

Plot[interp1[x, 0], {x, -10, 10}];

interp2[sbx_, sby_] :=
    Module[{y1, y2, f1, f2},
	   {y1, y2} = closest1[sby, nbinsY];
	   {f1, f2} = {interp1[sbx, y1], interp1[sbx, y2]};
	   (f2 - f1)*(sby - y1) + f1
	  ];

Plot[interp2[0, y], {y, -0.5, nbinsY-0.5}];

TableForm[Table[interp2[sbx, sby], {sbx, -0.5, nbinsX-0.5, 0.5}, {sby, -0.5, nbinsY-0.5, 0.5}]]

Out[70]//TableForm= -5.    1.    7.    13.   10.5   8.   7.25   6.5   5.75
			  ...	      ...      	   ... 	       ...
                    0.    .3.    6.   .9.    7.5   .6.   5.5   .5.    4.5
			  '''  	      '''      	   ''' 	       '''
                    5.     5.    5.    5.    4.5    4.   3.75   3.5   3.25
			  ...  	      ...      	   ... 	       ...
                    10.   .7.    4.   .1.    1.5   .2.   2.    .2.    2.
			  '''  	      '''      	   ''' 	       '''
                    6.5    5.5   4.5   3.5   2.75   2.   3.     4.    5.
			  ...  	      ...      	   ... 	       ...
                    3.    .4.    5.   .6.    4.    .2.   4.    .6.    8.
			  '''  	      '''      	   ''' 	       '''
                    -0.5   2.5   5.5   8.5   5.25   2.   5.     8.    11.

dense = Table[interp2[sbx, sby], {sbx, -0.5, nbinsX-0.5, 0.1}, {sby, -0.5, nbinsY-0.5, 0.1}];

<<Graphics`Graphics3D`

BarChart3D[dense];

(* I'm convinced. *)


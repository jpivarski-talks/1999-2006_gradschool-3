<< Statistics`ContinuousDistributions`;
<< Statistics`DescriptiveStatistics`;
<< Graphics`MultipleListPlot`;

numberWires = {64, 64, 72, 72, 88, 88, 96, 96, 112, 112, 128, 128,
	       136, 136, 152, 152, 180, 180, 180, 180, 204, 204, 204,
	       204, 228, 228, 228, 228, 252, 252, 252, 252, 276, 276,
	       276, 276, 300, 300, 300, 300, 324, 324, 324, 324, 348,
	       348, 348};
radius = {13.21, 14.59, 16.01, 17.39, 18.84, 20.22, 21.63, 23.01,
	  24.45, 25.83, 27.26, 28.64, 30.04, 31.42, 32.84, 34.22,
	  37, 38.39, 39.78, 41.18, 42.61, 44, 45.4, 46.79, 48.22,
	  49.61, 51.01, 52.4, 53.82, 55.22, 56.61, 58, 59.43, 60.82,
	  62.21, 63.6, 65.02, 66.42, 67.81, 69.2, 70.62, 72.01, 73.4,
	  74.8, 76.21, 77.6, 79} / 100.;
phi0east = {-0.002, -2.811, -0.001, -2.499, -0.0009, -2.0449, -0.001,
	    -1.874, -0.0003, -1.6063, -0.0005, -1.4055, -0.0001, -1.323,
	    0.0004, -1.1844, -3.599, -4.603, -3.61, -4.618, 3.365, 2.486,
	    3.376, 2.501, -3.169, -3.965, -3.182, -3.978, 3.005, 2.298,
	    3.018, 2.311, -2.865, -3.523, -2.877, -3.535, 2.742, 2.148,
	    2.754, 2.16, -2.634, -3.195, -2.645, -3.207, 2.553, 2.04, 2.563} * Pi/180.;
phi0west = {-0.002, 2.814, -0.001, 2.501, -0.0009, 2.046, -0.001,
	    1.876, -0.0003, 1.608, -0.0005, 1.407, -0.0001, 1.324,
	    -0.0004, 1.184, -3.599, -2.603, -3.61, -2.618, 3.365,
	    4.25, 3.376, 4.265, -3.169, -2.386, -3.182, -2.4, 3.005,
	    3.726, 3.018, 3.739, -2.865, -2.219, -2.877, -2.231, 2.742,
	    3.348, 2.754, 3.36, -2.634, -2.084, -2.645, -2.096, 2.553,
	    3.075, 2.563} * -Pi/180.;

axialXY = Table[Table[
    Module[{r = radius[[layer]],
	    phi = phi0east[[layer]] + n * 2*Pi/numberWires[[layer]]},
	   {r * Sin[phi], r * Cos[phi]}],
    {n, 1, numberWires[[layer]]}], {layer, 1, 16}];

(* phi is in +/-pi *)
cross = -0.00763007;
randomPair[phi_] := Module[{c = Random[Real, {0.04, 0.1}],
			    x0 = Random[NormalDistribution[-0.000060, 0.00065]],
			    y0 = Random[NormalDistribution[0.000896, 0.00075]]},
			   {Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}],
			    Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}]}
			  ];
dca[{hitx_, hity_}, {r_, x_, y_, phi_}] :=
    If[Cos[phi] * hitx + Sin[phi] * hity > 0,
       Abs[Sqrt[(hitx - x)^2 + (hity - y)^2] - Abs[r]],
       1000];
dcaLayer[layer_, track_] :=
    Module[{potentials = Table[{i, dca[axialXY[[layer, i]], track]}, {i, 1, numberWires[[layer]]}],
	    besti, bestdca = 10000, j},
	   For[j = 1, j <= numberWires[[layer]], j++,
	       If[potentials[[j, 2]] < bestdca,
		  {besti, bestdca} = potentials[[j]]]
	      ];
	   {besti, bestdca}];
generateHits[{track1_, track2_}] :=
    {{track1, Table[dcaLayer[layer, track1], {layer, 1, 16}]},
     {track2, Table[dcaLayer[layer, track2], {layer, 1, 16}]}};
viewSetup[{{track1_, hits1_}, {track2_, hits2_}}] :=
    Show[Graphics[{
	GrayLevel[0.7],	Point[#]& /@ Flatten[axialXY, 1],
	RGBColor[0,0,1],
	Circle[{track1[[2]], track1[[3]]}, Abs[track1[[1]]]],
	Table[Point[axialXY[[layer, hits1[[layer, 1]]]]], {layer, 1, 16}],
	Table[Circle[axialXY[[layer, hits1[[layer, 1]]]], hits1[[layer, 2]]], {layer, 1, 16}],
	RGBColor[1,0,0],
	Circle[{track2[[2]], track2[[3]]}, Abs[track2[[1]]]],
	Table[Point[axialXY[[layer, hits2[[layer, 1]]]]], {layer, 1, 16}],
	Table[Circle[axialXY[[layer, hits2[[layer, 1]]]], hits2[[layer, 2]]], {layer, 1, 16}]
		  }]
	 , AspectRatio -> 1
	 , PlotRange -> {{-0.4, 0.4}, {-0.4, 0.4}}
	];

viewSetup[generateHits[randomPair[5]]];


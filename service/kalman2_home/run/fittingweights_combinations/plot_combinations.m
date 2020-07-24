layer4 = ReadList["version4.drweightlayer", {Real, Real}];
layer12 = ReadList["version12.drweightlayer", {Real, Real}];
layer13 = ReadList["version13.drweightlayer", {Real, Real}];
layer14 = ReadList["version14.drweightlayer", {Real, Real}];
layer15 = ReadList["version15.drweightlayer", {Real, Real}];
layer16 = ReadList["version16.drweightlayer", {Real, Real}];

drift4 = ReadList["version4.drweightdrift", {Real, Real, Real}];
drift11 = ReadList["version11.drweightdrift", {Real, Real, Real}];
drift12 = ReadList["version12.drweightdrift", {Real, Real, Real}];
drift13 = ReadList["version13.drweightdrift", {Real, Real, Real}];
drift14 = ReadList["version14.drweightdrift", {Real, Real, Real}];
drift15 = ReadList["version15.drweightdrift", {Real, Real, Real}];

drift4 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift4]/47-1), #[[3]]}& /@ drift4;
drift11 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift11]/47-1), #[[3]]}& /@ drift11;
drift12 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift12]/47-1), #[[3]]}& /@ drift12;
drift13 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift13]/47-1), #[[3]]}& /@ drift13;
drift14 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift14]/47-1), #[[3]]}& /@ drift14;
drift15 = {#[[1]], (#[[2]]-1)*1.5/(Length[drift15]/47-1), #[[3]]}& /@ drift15;

theta5 = ReadList["version5.drweighttheta", {Real, Real, Real}];
theta12 = ReadList["version12.drweighttheta", {Real, Real, Real}];
theta13 = ReadList["version13.drweighttheta", {Real, Real, Real}];
theta14 = ReadList["version14.drweighttheta", {Real, Real, Real}];
theta15 = ReadList["version15.drweighttheta", {Real, Real, Real}];
theta16 = ReadList["version16.drweighttheta", {Real, Real, Real}];

theta5 = {#[[1]], (#[[2]]-1)*2./(Length[theta5]/47-1)-1., #[[3]]}& /@ theta5;
theta12 = {#[[1]], (#[[2]]-1)*2./(Length[theta12]/47-1)-1., #[[3]]}& /@ theta12;
theta13 = {#[[1]], (#[[2]]-1)*2./(Length[theta13]/47-1)-1., #[[3]]}& /@ theta13;
theta14 = {#[[1]], (#[[2]]-1)*2./(Length[theta14]/47-1)-1., #[[3]]}& /@ theta14;
theta15 = {#[[1]], (#[[2]]-1)*2./(Length[theta15]/47-1)-1., #[[3]]}& /@ theta15;
theta16 = {#[[1]], (#[[2]]-1)*2./(Length[theta16]/47-1)-1., #[[3]]}& /@ theta16;

MakeWholeMap[layer_, drift_, theta_] :=
    Table[Module[{lfw = Select[layer, (#[[1]] == l)&],
		  drfw = {#[[2]], #[[3]]}& /@ Select[drift, (#[[1]] == l)&],
		  thfw = {#[[2]], #[[3]]}& /@ Select[theta, (#[[1]] == l)&]},
		 Table[
		     Table[{l, drfw[[d, 1]], thfw[[t, 1]], lfw[[1, 2]]*drfw[[d, 2]]*thfw[[t, 2]]}, {t, 1, Length[thfw]}],
		     {d, 1, Length[drfw]}]
		], {l, 1, Length[layer]}];

FlattenWholeMap[map_] := Flatten[map, 2];

weightDrTh[dr_, th_] := If[dr <= 1., 1 + th^2, 0.];
weightLaTh[l_, th_] := 1 + th^2;
weightLaDr[l_, dr_] := If[dr <= 1., 1, 0.];

fwForLayer[map_, l_] := 
    Module[{fws = Flatten[map[[l]], 1]},
	   {l,
	    (Plus @@ ((#[[4]] * weightDrTh[#[[2]], #[[3]]])& /@ fws)) /
	    (Plus @@ ((weightDrTh[#[[2]], #[[3]]])& /@ fws))
	   }];

fwForDrift[map_, d_] :=
    Module[{fws = Flatten[#[[d]]& /@ map, 1]},
	   {fws[[1,2]],
	    (Plus @@ ((#[[4]] * weightLaTh[#[[1]], #[[3]]])& /@ fws)) /
	    (Plus @@ ((weightLaTh[#[[1]], #[[3]]])& /@ fws))
	   }];

fwForTheta[map_, t_] :=
    Module[{fws = Flatten[((#[[t]])& /@ #)& /@ map, 1]},
	   {fws[[1,3]],
	    (Plus @@ ((#[[4]] * weightLaDr[#[[1]], #[[2]]])& /@ fws)) /
	    (Plus @@ ((weightLaDr[#[[1]], #[[2]]])& /@ fws))
	   }];

<< Graphics`Graphics`;
<< Graphics`MultipleListPlot`;

plotLayer[map_, title_, display_] := 
    BarChart[#[[2]]& /@ Table[fwForLayer[map, l], {l, 1, 47}],
	     Frame -> True,
	     FrameLabel -> {"Layer", "Average FittingWeight", title, None},
	     FrameTicks -> {Automatic, Automatic, None, None},
	     AspectRatio -> 1/3,
	     Axes -> False,
	     PlotRange -> All,
	     DisplayFunction -> display];
plotLayer[map_, title_] := plotLayer[map, title, $DisplayFunction];

plotDrift[map_, title_, display_] :=
    MultipleListPlot[Table[fwForDrift[map, d], {d, 1, Length[map[[1]]]}],
		     PlotJoined -> True,
		     Frame -> True,
		     FrameLabel -> {"Drift Distance (multiples of cell width)", "Average FittingWeight", title, None},
		     FrameTicks -> {Automatic, Automatic, None, None},
		     AspectRatio -> 1/3,
		     Axes -> False,
		     PlotRange -> All,
		     DisplayFunction -> display];
plotDrift[map_, title_] := plotDrift[map, title, $DisplayFunction];

plotTheta[map_, title_, display_] :=
    MultipleListPlot[Table[fwForTheta[map, t], {t, 1, Length[map[[1,1]]]}],
		     PlotJoined -> True,
		     Frame -> True,
		     FrameLabel -> {"Cos(theta)", "Average FittingWeight", title, None},
		     FrameTicks -> {Automatic, Automatic, None, None},
		     AspectRatio -> 1/3,
		     Axes -> False,
		     PlotRange -> All,
		     DisplayFunction -> display];
plotTheta[map_, title_] := plotTheta[map, title, $DisplayFunction];

plotAll[map_, title_] :=
    Show[GraphicsArray[{{plotLayer[map, None, Identity]},
			{plotDrift[map, None, Identity]},
			{plotTheta[map, None, Identity]}}],
	 PlotLabel -> title,
	 DisplayFunction -> $DisplayFunction];


(* Now for the real thing *)

v1 = MakeWholeMap[layer4, drift4, theta5];
v2 = MakeWholeMap[layer12, drift11, theta12];
v3 = MakeWholeMap[layer13, drift12, theta13];
v4 = MakeWholeMap[layer14, drift13, theta14];
v5 = MakeWholeMap[layer15, drift14, theta15];
v6 = MakeWholeMap[layer16, drift15, theta16];

MultipleListPlot[{Table[{0, 0} + fwForLayer[v1, l], {l, 1, 47}],
		  Table[{0.1, 0} + fwForLayer[v2, l], {l, 1, 47}],
		  Table[{0.2, 0} + fwForLayer[v3, l], {l, 1, 47}],
		  Table[{0.3, 0} + fwForLayer[v4, l], {l, 1, 47}],
		  Table[{0.4, 0} + fwForLayer[v5, l], {l, 1, 47}],
		  Table[{0.5, 0} + fwForLayer[v6, l], {l, 1, 47}]}
		 , PlotJoined -> True
		 , PlotStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , SymbolStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , Frame -> True
		 , FrameLabel -> {"Layer", "Average FittingWeight", None, None}
		 , FrameTicks -> {Automatic, Automatic, None, None}
		 , Axes -> False
		 , AspectRatio -> 1.2
		 , PlotLegend -> {"db5-8 and 9b", "db9a, 10-11, 14+", "db12b", "db13b", "db12a", "db13a"}
		 , LegendPosition -> {-0.6, 0.75}
		 , LegendSize -> {1, 0.3}
		 , PlotRange -> {{-1, 48}, {0, 2}}
		 , ImageSize -> 5*72
		 , DisplayFunction -> (Display["plot_layer.eps", #, "EPS"]&)
		];

MultipleListPlot[{Table[{0, 0} + fwForDrift[v1, l], {l, 1, Length[v1[[1]]]}],
		  Table[{0.003, 0} + fwForDrift[v2, l], {l, 1, Length[v2[[1]]]}],
		  Table[{0.006, 0} + fwForDrift[v3, l], {l, 1, Length[v3[[1]]]}],
		  Table[{0.009, 0} + fwForDrift[v4, l], {l, 1, Length[v4[[1]]]}],
		  Table[{0.012, 0} + fwForDrift[v5, l], {l, 1, Length[v5[[1]]]}],
		  Table[{0.015, 0} + fwForDrift[v6, l], {l, 1, Length[v6[[1]]]}]}
		 , PlotJoined -> True
		 , PlotStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , SymbolStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , Frame -> True
		 , FrameLabel -> {"Drift Distance (multiples of cell width)", "Average FittingWeight", None, None}
		 , FrameTicks -> {Automatic, Automatic, None, None}
		 , Axes -> False
		 , AspectRatio -> 1.2
		 , PlotLegend -> {"db5-8 and 9b", "db9a, 10-11, 14+", "db12b", "db13b", "db12a", "db13a"}
		 , LegendPosition -> {-0.6, 0.75}
		 , LegendSize -> {1, 0.3}
		 , PlotRange -> {{-0.02, 1.22}, {0, 3}}
		 , ImageSize -> 5*72
		 , DisplayFunction -> (Display["plot_drift.eps", #, "EPS"]&)
		];

MultipleListPlot[{Table[{0, 0} + fwForTheta[v1, l], {l, 1, Length[v1[[1,1]]]}],
		  Table[{0.003, 0} + fwForTheta[v2, l], {l, 1, Length[v2[[1,1]]]}],
		  Table[{0.006, 0} + fwForTheta[v3, l], {l, 1, Length[v3[[1,1]]]}],
		  Table[{0.009, 0} + fwForTheta[v4, l], {l, 1, Length[v4[[1,1]]]}],
		  Table[{0.012, 0} + fwForTheta[v5, l], {l, 1, Length[v5[[1,1]]]}],
		  Table[{0.015, 0} + fwForTheta[v6, l], {l, 1, Length[v6[[1,1]]]}]}
		 , PlotJoined -> True
		 , PlotStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , SymbolStyle -> {RGBColor[1,0,0], RGBColor[0,0,1], RGBColor[0.5,0.25,0], RGBColor[0.75,0,0.75], RGBColor[0,0,0], RGBColor[0,0.5,0]}
		 , Frame -> True
		 , FrameLabel -> {"Cos(theta)", "Average FittingWeight", None, None}
		 , FrameTicks -> {Automatic, Automatic, None, None}
		 , Axes -> False
		 , AspectRatio -> 1.2
		 , PlotLegend -> {"db5-8 and 9b", "db9a, 10-11, 14+", "db12b", "db13b", "db12a", "db13a"}
		 , LegendPosition -> {-0.6, 0.75}
		 , LegendSize -> {1, 0.3}
		 , PlotRange -> {{-1.1, 1.1}, {0, 2}}
		 , ImageSize -> 5*72
		 , DisplayFunction -> (Display["plot_theta.eps", #, "EPS"]&)
		];





Show[
    MultipleListPlot[{Table[{0.003, 0} + fwForDrift[v2, l], {l, 1, Length[v2[[1]]]}]}
		     , PlotJoined -> True
		     , PlotRange -> {{-0.02, 1.22}, {0, 3}}
		     , DisplayFunction -> Identity
		    ],
    Graphics[{RGBColor[1,0,0], Circle[{1., 2}, 0.2]}]
    , Frame -> True
    , FrameLabel -> {"Drift Distance (multiples of cell width)", "Average FittingWeight", None, None}
    , FrameTicks -> {Automatic, Automatic, None, None}
    , Axes -> False
    , AspectRatio -> 1.2
    , ImageSize -> 4*72
    , DisplayFunction -> (Display["plot_drift_14.eps", #, "EPS"]&)
    ];


Off[General::spell1];
<< Graphics`Graphics`;
<< Statistics`DescriptiveStatistics`;

SetDirectory[ Environment["USER_DAFE"] <> "/fittingweights" ];
ntuple = ReadList[ "resid_edrift_edca.dat", { Number, Number, Number }, 40000 ];
(* sample = ntuple[[Range[1,1000]]]; *)

Histogram[ ( #[[1]] / Sqrt[ #[[2]] + #[[3]] ] )& /@ sample,
	   DisplayFunction -> ( Display[ "tmp.gif", #, "GIF" ]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Clear[Analyze];
Analyze[ dataset:{ { ( _Real | _Integer ), ( _Real | _Integer ),
		     ( _Real | _Integer ) } .. }, fw_Real ] :=
    Module[ {calc, cut, normresid, mean, rms },
	    calc = Compile[ {resid_Real, edrift_Real, edca_Real, tryfw_Real},
			    resid / Sqrt[ tryfw * edrift + edca ] ];
	    cut = Compile[ {normresid_Real}, Abs[normresid] < 3.5 ];
	    normresid = Select[ calc[#[[1]],#[[2]],#[[3]],fw]& /@ dataset, cut ];
	    mean = Mean[normresid];
	    rms = RootMeanSquare[ (# - mean)& /@ normresid ];
	    Print[ "mean = ", mean, " +/- ", rms / Sqrt[Length[dataset]] ];
	    Print[ "rms  = ", rms, " +/- ", rms / Sqrt[2 * Length[dataset]] ];
	  ];

Analyze[ ntuple, 1.0 ]

Analyze[ ntuple, 1.03 ]
mean = -0.00561136 +/- 0.00501417
rms  = 1.00283 +/- 0.00354556

Analyze[ ntuple, 1.035 ]
mean = -0.0056052 +/- 0.00500675
rms  = 1.00135 +/- 0.00354031

Exit
[Mathematica finished.]

(* layer 3 (ladder 32) *)

(* rphires, x, z *)
buggyRPHI = {
    { -1.28803*^-05, -0.0074106, 0.0586343 },
    { -1.56063*^-05, -0.0042697, 0.064227 },
    { -3.53962*^-05, -0.0054481, 0.0310393 },
    { -3.6561*^-05, 0.0121737, 0.065096 },
    { 1.55625*^-05, 0.0078021, 0.0646403 },
    { 1.70398*^-05, 0.000367901, 0.0495871 },
    { 1.80381*^-05, -0.008183, 0.0446807 },
    { 2.1584*^-05, 0.0060972, 0.0927861 },
    { 2.27731*^-07, 0.007229, 0.0419566 },
    { 2.36788*^-05, 0.000402001, 0.100204 },
    { 2.59764*^-05, 0.0102711, 0.0447387 },
    { 2.7747*^-05, -0.0029311, 0.0620542 },
    { 3.33304*^-05, 0.0068503, 0.0590743 },
    { 3.69138*^-05, 0.0075107, 0.0682168 },
    { 3.70138*^-05, 0.0027286, 0.0552251 },
    { 3.91777*^-05, -0.0079718, 0.0491541 },
    { 5.36078*^-05, 0.000129301, 0.0452763 },
    { 5.58155*^-05, -0.0014677, 0.0752357 },
    { 5.78214*^-06, 0.0034432, 0.0578134 },
    { 6.32486*^-05, -0.0024751, 0.0780876 },
    { 6.62933*^-05, -0.0020636, 0.0704736 },
    { 7.65431*^-05, 0.0055774, 0.0843281 },
    { 8.24309*^-05, 0.0121188, 0.0783238 }
	    };
(* zres, z *)
buggyZ = {
    { -0.000899741, 0.0310393 },
    { 0.000108525, 0.0419566 },
    { 3.15702*^-05, 0.0552251 },
    { -0.000468717, 0.0578134 },
    { -0.000373505, 0.0586343 },
    { 0.00071223, 0.0590743 },
    { -0.00101919, 0.0620542 },
    { 0.000138438, 0.064227 },
    { -0.000355639, 0.0646403 },
    { -0.000404516, 0.065096 },
    { -0.000711242, 0.0843281 },
    { -0.000126413, 0.0927861 }
	 };
(* rphires, x, z *)
buglessRPHI = {
    { -1.25679*^-05, -0.0054481, 0.0309095 },
    { -1.28429*^-05, -0.0074106, 0.0589493 },
    { -1.85435*^-05, -0.0042697, 0.0639459 },
    { -3.82372*^-05, 0.0121737, 0.0649941 },
    { 1.34788*^-05, -0.008183, 0.0445112 },
    { 1.51193*^-06, 0.007229, 0.0420427 },
    { 1.5537*^-05, 0.0078021, 0.0646399 },
    { 1.87854*^-05, 0.000367901, 0.0498995 },
    { 2.03504*^-05, 0.0060972, 0.0923785 },
    { 2.21276*^-05, 0.000402001, 0.100008 },
    { 2.98784*^-05, 0.0102711, 0.0455413 },
    { 3.10625*^-05, -0.0029311, 0.0625273 },
    { 3.37281*^-05, -0.0079718, 0.0495975 },
    { 3.69029*^-05, 0.0075107, 0.0682171 },
    { 3.70726*^-05, 0.0068503, 0.0587133 },
    { 3.81143*^-05, 0.0027286, 0.0554602 },
    { 5.3394*^-05, -0.0086547, 0.104433 },
    { 5.36836*^-05, 0.000129301, 0.0456194 },
    { 5.41036*^-05, -0.0014677, 0.075009 },
    { 6.42846*^-05, -0.0024751, 0.0787288 },
    { 6.63302*^-06, 0.0034432, 0.0577033 },
    { 6.77623*^-05, -0.0020636, 0.0707354 },
    { 7.65764*^-05, 0.0055774, 0.0843274 },
    { 8.16528*^-05, 0.0121188, 0.0784712 }
	      };
(* zres, z *)
buglessZ = {
    { -0.0010297, 0.0309095 },
    { 0.000194658, 0.0420427 },
    { -0.00111914, 0.0495975 },
    { 0.000266603, 0.0554602 },
    { -0.000578848, 0.0577033 },
    { 0.00035127, 0.0587133 },
    { -5.85117*^-05, 0.0589493 },
    { -0.000546139, 0.0625273 },
    { -0.000142574, 0.0639459 },
    { -0.000356082, 0.0646399 },
    { -0.000506321, 0.0649941 },
    { -0.000712005, 0.0843274 },
    { -0.000534027, 0.0923785 }
	   };

matchupRPHI = Table[
    { Select[ buglessRPHI, ( Abs[ buggyRPHI[[i]][[2]] - #[[2]] ] < 0.00001 )& ],
      buggyRPHI[[i]]
    }, {i,1,Length[buggyRPHI]}
		   ];
matchedRPHI = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2,
		( #[[1,1,3]] + #[[2,3]] ) / 2 }&
    /@ Select[ matchupRPHI, ( #[[1]] != {} )& ];

matchupZ = Table[
    { Select[ buglessZ, ( Abs[ buggyZ[[i]][[2]] - #[[2]] ] < 0.0005 )& ],
      buggyZ[[i]]
    }, {i,1,Length[buggyZ]}
		   ];
matchedZ = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2 }&
    /@ Select[ matchupZ, ( #[[1]] != {} )& ];

(* difference plots: rphi diff vs x, rphi diff vs ave z, zres diff vs z *)

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
	  PlotRange -> { { -15.00001, 15 }, { -5.00001, 5 } },
	  FrameLabel -> {"SV rphi hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 32 rphi res DIFF vs x", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer3_rphiresdiff_v_x.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[3]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -5.00001, 5 } },
	  FrameLabel -> {"matched z hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 32 rphi res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer3_rphiresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedZ,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
	  FrameLabel -> {"SV z hit coordinate in millimeters",
			 "difference in track z residual in microns",
			 "Ladder 32 z res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer3_zresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

(* plots we want: rphi vs x, rphi vs z, zres vs z *)

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -15.00001, 15 }, { -100.00001, 100 } },
      FrameLabel -> {"SV rphi hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 32, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer3_rphires_v_x.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
      FrameLabel -> {"matched z hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 32, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer3_rphires_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyZ,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessZ,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -1200.00001, 1200 } },
      FrameLabel -> {"SV z hit coordinate in millimeters",
		     "track z residual in microns",
		     "Ladder 32, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer3_zres_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

(* layer 4 (ladder 56) *)

(* rphires, x, z *)
buggyRPHI = {
    { -1.0525*^-05, 0.0016636, 0.0500315 },
    { -1.73664*^-05, -0.0100987, 0.0783649 },
    { -2.44943*^-05, 0.0066754, -0.112363 },
    { -3.59213*^-05, 0.0045237, 0.091679 },
    { -6.36931*^-06, 2.77013*^-05, 0.0935297 },
    { -7.01155*^-06, 0.0055112, 0.0717629 },
    { 2.42424*^-05, -0.0064927, 0.138444 },
    { 3.39421*^-05, -0.001875, 0.0967196 },
    { 4.0864*^-05, -0.0030876, 0.107255 },
    { 4.71361*^-05, -0.0064427, 0.0711518 },
    { 5.39707*^-05, -0.0038447, 0.101683 },
    { 5.50123*^-05, -0.0024408, 0.114066 },
    { 6.11452*^-06, -0.0111501, 0.09192 },
    { 7.02779*^-05, 0.0057532, 0.0774152 },
    { 9.26611*^-06, -0.006663, 0.074675 }
	    };
(* zres, z *)
buggyZ = {
    { 0.000344571, 0.0935297 },
    { -0.00117759, 0.0774152 },
    { 0.000290078, 0.074675 },
    { 0.00055058, -0.112363 },
    { -0.000365292, 0.09192 },
    { 0.000572758, 0.0717629 },
    { 0.00114071, 0.114066 },
    { 0.000102853, 0.0711518 },
    { 0.000530389, 0.138444 },
    { -0.000119645, 0.101683 },
    { 0.000122906, 0.0783649 },
    { -0.00121352, 0.107255 },
    { -0.000783141, 0.0967196 },
    { -0.00028971, 0.091679 },
    { -0.000721317, 0.0500315 }
	 };
(* rphires, x, z *)
buglessRPHI = {
    { -1.73025*^-05, -0.0100987, 0.0785886 },
    { -2.22946*^-05, 0.0066754, -0.112391 },
    { -3.66009*^-05, 0.0045237, 0.0919756 },
    { -7.15652*^-06, 2.77013*^-05, 0.0932634 },
    { -9.51062*^-06, 0.0055112, 0.071601 },
    { 1.24894*^-05, 0.0016636, 0.0499095 },
    { 2.38173*^-05, -0.0064927, 0.138258 },
    { 3.50553*^-05, -0.001875, 0.0971376 },
    { 4.08221*^-05, -0.0030876, 0.107503 },
    { 4.63018*^-05, -0.0064427, 0.0714737 },
    { 5.3478*^-05, -0.0024408, 0.114642 },
    { 5.36425*^-05, -0.0038447, 0.101489 },
    { 6.59497*^-05, 0.0057532, 0.0778141 },
    { 7.07659*^-06, -0.0111501, 0.0918266 },
    { 7.73309*^-05, 0.006628, 0.142822 },
    { 9.53695*^-06, -0.006663, 0.0749709 }
	      };
(* zres, z *)
buglessZ = {
    { 7.8321*^-05, 0.0932634 },
    { -0.000778736, 0.0778141 },
    { 0.000585979, 0.0749709 },
    { 0.000522743, -0.112391 },
    { -0.000458737, 0.0918266 },
    { 0.000410795, 0.071601 },
    { 0.000424774, 0.0714737 },
    { 0.000345, 0.138258 },
    { -0.000312949, 0.101489 },
    { 0.000346612, 0.0785886 },
    { -0.000966112, 0.107503 },
    { -0.000365146, 0.0971376 },
    { 6.87722*^-06, 0.0919756 },
    { -0.000843325, 0.0499095 }
	   };

matchupRPHI = Table[
    { Select[ buglessRPHI, ( Abs[ buggyRPHI[[i]][[2]] - #[[2]] ] < 0.00001 )& ],
      buggyRPHI[[i]]
    }, {i,1,Length[buggyRPHI]}
		   ];
matchedRPHI = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2,
		( #[[1,1,3]] + #[[2,3]] ) / 2 }&
    /@ Select[ matchupRPHI, ( #[[1]] != {} )& ];

matchupZ = Table[
    { Select[ buglessZ, ( Abs[ buggyZ[[i]][[2]] - #[[2]] ] < 0.0005 )& ],
      buggyZ[[i]]
    }, {i,1,Length[buggyZ]}
		   ];
matchedZ = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2 }&
    /@ Select[ matchupZ, ( #[[1]] != {} )& ];

(* difference plots: rphi diff vs x, rphi diff vs ave z, zres diff vs z *)

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
	  PlotRange -> { { -15.00001, 15 }, { -5.00001, 5 } },
	  FrameLabel -> {"SV rphi hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 56 rphi res DIFF vs x", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer4_rphiresdiff_v_x.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[3]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -5.00001, 5 } },
	  FrameLabel -> {"matched z hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 56 rphi res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer4_rphiresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedZ,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
	  FrameLabel -> {"SV z hit coordinate in millimeters",
			 "difference in track z residual in microns",
			 "Ladder 56 z res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["layer4_zresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

(* plots we want: rphi vs x, rphi vs z, zres vs z *)

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -15.00001, 15 }, { -100.00001, 100 } },
      FrameLabel -> {"SV rphi hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 56, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer4_rphires_v_x.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
      FrameLabel -> {"matched z hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 56, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer4_rphires_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyZ,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessZ,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -1200.00001, 1200 } },
      FrameLabel -> {"SV z hit coordinate in millimeters",
		     "track z residual in microns",
		     "Ladder 56, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["layer4_zres_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

(* this is just a test of ladder 36 *)

buggyRPHI = {
    { -1.14078*^-05, -0.0103175, -0.0795085 },
    { -1.40395*^-05, -0.004729, -0.0963122 },
    { -1.64698*^-05, -0.000729199, -0.0912474 },
    { -1.81342*^-05, 0.0122091, 0.0450057 },
    { -1.82532*^-05, 0.001337, 0.102839 },
    { -1.99779*^-05, -0.0076398, -0.112078 },
    { -2.29866*^-05, 0.0125854, 0.10406 },
    { -2.51707*^-05, 0.0091295, -0.0833773 },
    { -2.77134*^-05, 0.0069084, 0.118801 },
    { -2.97499*^-06, 0.000414201, -0.119517 },
    { -3.75968*^-06, 0.0016355, 0.0994182 },
    { -4.06928*^-06, -0.0105483, 0.0986174 },
    { -4.21051*^-06, 0.0018716, -0.0397802 },
    { -4.64861*^-06, -0.0023386, -0.0879224 },
    { -4.8409*^-05, 0.0121374, -0.0787838 },
    { -5.35516*^-05, -0.0026407, -0.0795608 },
    { -5.66958*^-06, -0.0063271, -0.0635122 },
    { -6.07125*^-06, 0.0093402, -0.0313217 },
    { -7.04317*^-06, -0.0092501, -0.124532 },
    { -8.94659*^-06, 0.000868001, -0.0614925 },
    { -9.76941*^-06, 0.0062431, 0.0762478 },
    { 1.09783*^-06, -0.000631399, -0.0571188 },
    { 1.3372*^-05, 0.0056617, 0.0844382 },
    { 1.72324*^-06, 0.0102761, 0.10478 },
    { 1.72976*^-05, 0.0014188, -0.0690354 },
    { 1.79733*^-06, 0.0110982, 0.0824666 },
    { 1.80773*^-05, -0.000629999, -0.113111 },
    { 2.16457*^-05, 0.0058721, -0.0500191 },
    { 2.25501*^-06, -0.000866599, -0.0884308 },
    { 2.48468*^-05, -0.000419399, -0.101054 },
    { 2.89268*^-05, -0.0011316, -0.0654723 },
    { 3.23611*^-05, -0.0082637, -0.112309 },
    { 3.41669*^-05, 0.0059374, -0.0840609 },
    { 3.49561*^-06, 0.0044695, -0.0469784 },
    { 3.63723*^-05, 0.0069288, -0.0833356 },
    { 4.55477*^-06, -0.0082832, -0.0562538 },
    { 4.58646*^-06, -0.0072001, -0.000837391 },
    { 4.63598*^-05, 0.0089534, -0.0976261 },
    { 4.70761*^-05, -0.000595799, 0.00589538 },
    { 6.47927*^-06, 0.0099115, 0.0773937 },
    { 7.98825*^-06, -0.000498699, -0.0173304 }
	    };
buggyZ = {
    { 0.000166697, -0.112309 },
    { -0.000207273, 0.0844382 },
    { -0.000203496, -0.0469784 },
    { -0.000345209, -0.0795608 },
    { -0.000472413, -0.0912474 },
    { 0.000261111, -0.0500191 },
    { 0.000379966, 0.118801 },
    { 0.000212504, -0.0690354 },
    { -2.75724*^-05, -0.0976261 },
    { -0.000403126, -0.124532 },
    { 0.000198233, 0.10478 },
    { -0.000378971, 0.102839 },
    { -0.000487848, -0.119517 },
    { 0.000108039, 0.0773937 },
    { 0.00016113, -0.0795085 },
    { -0.000256934, -0.0397802 },
    { -0.000222152, 0.0994182 },
    { -0.000224955, -0.0635122 },
    { 0.000115123, 0.0762478 },
    { -2.05105*^-06, -0.0840609 },
    { -0.000165097, 0.0450057 },
    { -0.000618842, -0.0833356 },
    { -0.000485394, -0.101054 },
    { 1.24359*^-05, 0.0824666 },
    { -0.000814097, -0.0879224 },
    { 0.000404085, -0.0562538 },
    { 9.45136*^-05, -0.0614925 },
    { -7.10791*^-05, -0.112078 },
    { -0.000150304, -0.0963122 },
    { -2.50682*^-05, -0.0173304 },
    { 0.000388456, -0.000837391 },
    { -0.000197231, -0.0833773 },
    { 0.000401571, -0.113111 },
    { -0.000315288, -0.0571188 },
    { -1.09029*^-05, -0.0884308 },
    { 0.00049486, 0.00589538 },
    { -0.000100604, -0.0313217 },
    { 0.000388253, -0.0787838 },
    { 0.000183595, 0.10406 }
	 };
buglessRPHI = {
    { -1.06436*^-05, -0.0103175, -0.0798652 },
    { -1.36675*^-05, -0.004729, -0.0967261 },
    { -1.67609*^-05, -0.000729199, -0.0916476 },
    { -1.91684*^-05, 0.001337, 0.103311 },
    { -1.94534*^-06, 0.0093402, -0.031627 },
    { -2.04654*^-05, -0.0076398, -0.112821 },
    { -2.30223*^-05, 0.0125854, 0.10406 },
    { -2.32561*^-05, 0.0122091, 0.0448208 },
    { -2.51883*^-05, 0.0091295, -0.0833784 },
    { -2.64192*^-05, 0.0069084, 0.119478 },
    { -2.96022*^-06, 0.000414201, -0.119517 },
    { -3.75862*^-06, 0.0016355, 0.0994181 },
    { -4.03862*^-06, -0.0063271, -0.0635024 },
    { -4.86137*^-05, 0.0121374, -0.0787244 },
    { -5.06619*^-06, -0.0105483, 0.098639 },
    { -5.25957*^-06, 0.0018716, -0.0399075 },
    { -5.3137*^-05, -0.0026407, -0.0798465 },
    { -5.52862*^-06, -0.0023386, -0.0882136 },
    { -6.87788*^-06, -0.0092501, -0.124233 },
    { -9.23481*^-06, 0.000868001, -0.0616906 },
    { -9.60796*^-06, 0.0062431, 0.0762534 },
    { 1.04367*^-06, 0.0044695, -0.0470708 },
    { 1.20244*^-05, 0.0056617, 0.0840916 },
    { 1.59794*^-05, 0.0058721, -0.0495658 },
    { 1.80025*^-05, -0.000629999, -0.113169 },
    { 1.80898*^-05, 0.0014188, -0.0690444 },
    { 2.24623*^-06, -0.000866599, -0.0884303 },
    { 2.41359*^-06, 0.0102761, 0.104865 },
    { 2.4747*^-05, -0.000419399, -0.101056 },
    { 2.94098*^-05, -0.0011316, -0.065388 },
    { 3.10295*^-05, -0.0082637, -0.11258 },
    { 3.17002*^-06, 0.0110982, 0.0828198 },
    { 3.41753*^-05, 0.0059374, -0.0840611 },
    { 3.90389*^-06, -0.0072001, -0.000860547 },
    { 4.70699*^-05, -0.000595799, 0.0058954 },
    { 4.73894*^-05, 0.0089534, -0.0980145 },
    { 5.78641*^-06, -0.0082832, -0.0566599 },
    { 6.6418*^-06, 0.0099115, 0.0772789 },
    { 6.80589*^-07, -0.000631399, -0.0573176 },
    { 7.79359*^-06, -0.000498699, -0.017357 }
	    };
buglessZ = {
    { -0.000104461, -0.11258 },
    { -0.000553899, 0.0840916 },
    { -0.000295947, -0.0470708 },
    { -0.000630935, -0.0798465 },
    { -0.000872664, -0.0916476 },
    { 0.000714439, -0.0495658 },
    { 0.00105712, 0.119478 },
    { 0.000203545, -0.0690444 },
    { -0.000415904, -0.0980145 },
    { -0.000103712, -0.124233 },
    { 0.000283654, 0.104865 },
    { 9.3215*^-05, 0.103311 },
    { -0.000488599, -0.119517 },
    { -6.75024*^-06, 0.0772789 },
    { -0.000195535, -0.0798652 },
    { -0.000384272, -0.0399075 },
    { -0.000222219, 0.0994181 },
    { -0.000215247, -0.0635024 },
    { 0.000120694, 0.0762534 },
    { -2.21674*^-06, -0.0840611 },
    { -0.000350032, 0.0448208 },
    { -0.000487329, -0.101056 },
    { 0.000365661, 0.0828198 },
    { -0.00110523, -0.0882136 },
    { -2.02101*^-06, -0.0566599 },
    { -0.000103599, -0.0616906 },
    { -0.000814807, -0.112821 },
    { -0.00056414, -0.0967261 },
    { -5.16999*^-05, -0.017357 },
    { 0.0003653, -0.000860547 },
    { -0.000198403, -0.0833784 },
    { 0.00034339, -0.113169 },
    { -0.000514048, -0.0573176 },
    { -1.04247*^-05, -0.0884303 },
    { 0.000494873, 0.0058954 },
    { -0.000405937, -0.031627 },
    { 0.000447632, -0.0787244 },
    { 0.0001839, 0.10406 }
	   };

matchupRPHI = Table[
    { Select[ buglessRPHI, ( Abs[ buggyRPHI[[i]][[2]] - #[[2]] ] < 0.00001 )& ],
      buggyRPHI[[i]]
    }, {i,1,Length[buggyRPHI]}
		   ];
matchedRPHI = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2,
		( #[[1,1,3]] + #[[2,3]] ) / 2 }&
    /@ Select[ matchupRPHI, ( #[[1]] != {} )& ];

matchupZ = Table[
    { Select[ buglessZ, ( Abs[ buggyZ[[i]][[2]] - #[[2]] ] < 0.0005 )& ],
      buggyZ[[i]]
    }, {i,1,Length[buggyZ]}
		   ];
matchedZ = { #[[1,1,1]] - #[[2,1]],
		( #[[1,1,2]] + #[[2,2]] ) / 2 }&
    /@ Select[ matchupZ, ( #[[1]] != {} )& ];

(* difference plots: rphi diff vs x, rphi diff vs ave z, zres diff vs z *)

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
	  PlotRange -> { { -15.00001, 15 }, { -5.00001, 5 } },
	  FrameLabel -> {"SV rphi hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 36 rphi res DIFF vs x", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["ladder36_rphiresdiff_v_x.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[3]], 1000000 * #[[1]] }& /@ matchedRPHI,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -5.00001, 5 } },
	  FrameLabel -> {"matched z hit coordinate in millimeters",
			 "difference in track rphi residual in microns",
			 "Ladder 36 rphi res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["ladder36_rphiresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

ListPlot[ {1000* #[[2]], 1000000 * #[[1]] }& /@ matchedZ,
	  Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
 	  PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
	  FrameLabel -> {"SV z hit coordinate in millimeters",
			 "difference in track z residual in microns",
			 "Ladder 36 z res DIFF vs z", None },
	  ImageSize -> 72 * 7,
	  DisplayFunction -> ( Display["ladder36_zresdiff_v_z.eps", #, "EPS"]& ) ]
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

(* absolute plots *)

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -15.00001, 15 }, { -100.00001, 100 } },
      FrameLabel -> {"SV rphi hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 36, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["ladder36_rphires_v_x.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buggyRPHI,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[3]], 1000000 * #[[1]]}& /@ buglessRPHI,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -100.00001, 100 } },
      FrameLabel -> {"matched z hit coordinate in millimeters",
		     "track rphi residual in microns",
		     "Ladder 36, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["ladder36_rphires_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];

Show[ { ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buggyZ,
		  PlotStyle -> { RGBColor[1,0,0] },
		  DisplayFunction -> Identity ],
	ListPlot[ {1000 * #[[2]], 1000000 * #[[1]]}& /@ buglessZ,
		  PlotStyle -> { RGBColor[0,0,1] },
		  DisplayFunction -> Identity ]
      },
      Axes -> {True, True}, AxesOrigin -> {0,0}, Frame -> True,
      PlotRange -> { { -120.00001, 120 }, { -1200.00001, 1200 } },
      FrameLabel -> {"SV z hit coordinate in millimeters",
		     "track z residual in microns",
		     "Ladder 36, red = before bug fix, blue = after", None },
      ImageSize -> 72 * 7,
      DisplayFunction -> ( Display["ladder36_zres_v_z.eps", #, "EPS"]& ) ];
Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];


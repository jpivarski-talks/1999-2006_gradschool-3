<< Statistics`ContinuousDistributions`;
<< Graphics`MultipleListPlot`;
<< Graphics`Graphics`;

cross = -0.00763007;
l1 = 0.37;
l2 = 0.58;
l3 = 0.79;

poscirc[phi_] := Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]},
			{r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}];
negcirc[phi_] := Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]},
			{r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}];

randomPair[phi_] := Module[{c = Random[Real, {0.04, 0.1}],
			    x0 = Random[NormalDistribution[-0.000060, 0.00065]],
			    y0 = Random[NormalDistribution[0.000896, 0.00075]]},
			   {Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}],
			    Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}]}
			  ]

d0miss[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] :=
    (* The minus sign STAYS! *)
    (Sqrt[xc1^2 + yc1^2] - r1) + (-Sqrt[xc2^2 + yc2^2] - r2);

phi0miss[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] :=
    (xc1*yc2 - yc1*xc2)/Sqrt[xc1^2 + yc1^2]/Sqrt[xc2^2 + yc2^2];

curvmiss[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] := r1+r2;

bisect[f_, lo_, hi_] := Module[{flo = N[f[lo]], fhi = N[f[hi]], mid},
			       mid = N[lo - (hi - lo)*flo/(fhi - flo)];
			       If[Abs[f[mid]] > 1*^-5,  (* tolerance stops recursion *)
				  If[f[mid]*flo < 0, bisect[f, lo, mid], bisect[f, mid, hi]],
				  mid]
			      ];

hit[{r_,xc_,yc_,phi_}, L_] := Module[
    {sp = bisect[((r*Sin[#/r+phi]+xc)^2 + (-r*Cos[#/r+phi]+yc)^2 - L^2)&, 0, 2*L]},
    {r*Sin[sp/r+phi]+xc, -r*Cos[sp/r+phi]+yc}
				    ];
hits[circle_, l1t_, l2t_, l3t_] :=
    {l1t.hit[circle, l1], l2t.hit[circle, l2], l3t.hit[circle, l3]};
hits[circle_] := hits[circle,
		      DiagonalMatrix[{1,1}],
		      DiagonalMatrix[{1,1}],
		      DiagonalMatrix[{1,1}]];

circResid2[{r_, xc_, yc_}, {{x1_, y1_}, {x2_, y2_}, {x3_, y3_}}] :=
    (Sqrt[(x1 - xc)^2 + (y1 - yc)^2] - Abs[r])^2 +
    (Sqrt[(x2 - xc)^2 + (y2 - yc)^2] - Abs[r])^2 +
    (Sqrt[(x3 - xc)^2 + (y3 - yc)^2] - Abs[r])^2;

reFit[circle_, l1t_, l2t_, l3t_] :=
    Module[{minr, minxc, minyc, minphi},
	   {minr, minxc, minyc} = {r, xc, yc} /.
	   Module[{eqn = circResid2[{r, xc, yc}, hits[circle, l1t, l2t, l3t]]},
		  FindMinimum[eqn,
			      {r, {1,1.01}*circle[[1]]},
			      {xc, {1,1.01}*circle[[2]]},
			      {yc, {1,1.01}*circle[[3]]}
			     ][[2]]
		 ];
	   minphi = (phi /.
		     FindMinimum[minr^2 + minxc^2 + minyc^2 +
				 2*minr*(Sin[phi]*minxc - Cos[phi]*minyc),
				 {phi, circle[[4]]}][[2]]
		    );
	   {minr, minxc, minyc, minphi}
	  ];

layer[l_] := Table[l*{Cos[t],Sin[t]}, {t, 0, 2*Pi, 2*Pi/100}];

RotationMatrix[angle_] := {{Cos[angle], Sin[angle]}, {-Sin[angle], Cos[angle]}};

seeItAll[{acirc_, bcirc_}, l1t_, l2t_, l3t_] :=
    Show[
	Module[{r1, xc1, yc1, phi1, r2, xc2, yc2, phi2},
	       {r1, xc1, yc1, phi1} = acirc;
	       {r2, xc2, yc2, phi2} = bcirc;
	       ParametricPlot[{{r1*Sin[sp/r1+phi1]+xc1, -r1*Cos[sp/r1+phi1]+yc1},
			       {r2*Sin[sp/r2+phi2]+xc2, -r2*Cos[sp/r2+phi2]+yc2}},
			      {sp, 0, 1.1}
			      , PlotPoints -> 1000
			      , AxesOrigin -> {0,0}
			      , PlotStyle -> RGBColor[0,0,1]
			      , DisplayFunction -> Identity
			     ]
	      ],
	Module[{r1, xc1, yc1, phi1, r2, xc2, yc2, phi2},
	       {r1, xc1, yc1, phi1} = reFit[acirc, l1t, l2t, l3t];
	       {r2, xc2, yc2, phi2} = reFit[bcirc, l1t, l2t, l3t];
	       ParametricPlot[{{r1*Sin[sp/r1+phi1]+xc1, -r1*Cos[sp/r1+phi1]+yc1},
			       {r2*Sin[sp/r2+phi2]+xc2, -r2*Cos[sp/r2+phi2]+yc2}},
			      {sp, 0, 1.1}
			      , PlotPoints -> 1000
			      , AxesOrigin -> {0,0}
			      , PlotStyle -> RGBColor[1,0,0]
			      , DisplayFunction -> Identity
			     ]
	      ]
	, Graphics[{
	    RGBColor[0,0,1],
	    Point[#]& /@ hits[acirc],
	    Point[#]& /@ hits[bcirc],
	    RGBColor[1,0,0],
	    Point[#]& /@ hits[acirc, l1t, l2t, l3t],
	    Point[#]& /@ hits[bcirc, l1t, l2t, l3t],
	    RGBColor[0,0,0],
	    Line[layer[l1]],
	    Line[layer[l2]],
	    Line[layer[l3]],
	    Dashing[{0.01}],
	    Line[(l1t.#)& /@ layer[l1]],
	    Line[(l2t.#)& /@ layer[l2]],
	    Line[(l3t.#)& /@ layer[l3]],
	    RGBColor[1,1,1],
	    Line[{{-1.1,-1.1},{-1.1,1.1},{1.1,1.1},{1.1,-1.1},{-1.1,-1.1}}]
		   }]
	, AspectRatio -> 1
	, ImageSize -> 72*8
	, DisplayFunction -> (Display["squish_form-2.eps", #, "EPS"]&)
	];
seeItAll[randomPair[Pi*3/8],
	 RotationMatrix[0.08*Pi] . DiagonalMatrix[{1,1.2}],
	 DiagonalMatrix[{1.2,1}],
	 DiagonalMatrix[{1,1.2}]];

d0missAve[phi_, n_, l1t_, l2t_, l3t_] := {Mean[#], StandardDeviation[#]/Sqrt[n]}&[
    Table[d0miss @@ (reFit[#, l1t, l2t, l3t]& /@ randomPair[phi]), {n}]
							       ];

curvmissAve[phi_, n_, l1t_, l2t_, l3t_] := {Mean[#], StandardDeviation[#]/Sqrt[n]}&[
    Table[curvmiss @@ (reFit[#, l1t, l2t, l3t]& /@ randomPair[phi]), {n}]
							       ];

innerTrans = DiagonalMatrix[{1,1}];
midTrans   = RotationMatrix[-0.0004];
outerTrans = DiagonalMatrix[{1,1}];
MultipleListPlot[{{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, curvmissAve[phi, 20, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/10.}]
		 , Axes -> False
		 , Frame -> True
		 , PlotRange -> {{0, 2*Pi},{-0.0003, 0.}}
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , FrameLabel -> {"phi0 of the positive track", "1/2/curv1 + 1/2/curv2",
				  "Middle layer twisted -0.4 mrad", ""}
		 , ImageSize -> 72*8
		 , DisplayFunction -> (Display["squish_curvform.eps", #, "EPS"]&)
		];

nnerTrans = DiagonalMatrix[{1,1}];
midTrans   = DiagonalMatrix[{1,1}];
outerTrans = DiagonalMatrix[{1,1}];
MultipleListPlot[{{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, d0missAve[phi, 20, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/10.}]
		 , Axes -> False
		 , Frame -> True
		 , PlotRange -> {{0, 2*Pi},{-0.000010, 0.000010}}
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , FrameLabel -> {"phi0 of the positive track", "d0miss",
				  "No transformations", ""}
		 , ImageSize -> 72*8
		 , DisplayFunction -> (Display["squish_form-1.eps", #, "EPS"]&)
		];

makeTransforms[l1ang_, l1x_, l1y_, l2ang_, l2x_, l2y_, l3ang_, l3x_, l3y_] :=
    {RotationMatrix[l1ang] . DiagonalMatrix[{1 + l1x/l1, 1 + l1y/l1}],
     RotationMatrix[l2ang] . DiagonalMatrix[{1 + l2x/l2, 1 + l2y/l2}],
     RotationMatrix[l3ang] . DiagonalMatrix[{1 + l3x/l3, 1 + l3y/l3}]};


sinterm[trials_, points_, {l1t_, l2t_, l3t_}] :=
    Sum[d0missAve[phi, trials, l1t, l2t, l3t][[1]] * Sin[2*phi],
	{phi, 0, 2*Pi*(1 - 1/points), 2*Pi/points}] * 2*Pi/points;

costerm[trials_, points_, {l1t_, l2t_, l3t_}] :=
    Sum[d0missAve[phi, trials, l1t, l2t, l3t][[1]] * Cos[2*phi],
	{phi, 0, 2*Pi*(1 - 1/points), 2*Pi/points}] * 2*Pi/points;

angMax = 0.0001;
xMax = 0.000050;
yMax = 0.000050;
sinfiddles = Table[{sinterm[30, 20, makeTransforms[l1ang, l1x, l1y, l2ang, l2x, l2y, l3ang, l3x, l3y]],
		    l1ang, l1x, l1y, l2ang, l2x, l2y, l3ang, l3x, l3y},
		   {l1ang, 0, angMax, angMax},
		   {l1x, 0, xMax, xMax},
		   {l1y, 0, yMax, yMax},
		   {l2ang, 0, angMax, angMax},
		   {l2x, 0, xMax, xMax},
		   {l2y, 0, yMax, yMax},
		   {l3ang, 0, angMax, angMax},
		   {l3x, 0, xMax, xMax},
		   {l3y, 0, yMax, yMax}
		  ];
DumpSave["squish_sinfiddles.mx", sinfiddles];

angMax = 0.0001;
xMax = 0.000050;
yMax = 0.000050;
cosfiddles = Table[{costerm[30, 20, makeTransforms[l1ang, l1x, l1y, l2ang, l2x, l2y, l3ang, l3x, l3y]],
		    l1ang, l1x, l1y, l2ang, l2x, l2y, l3ang, l3x, l3y},
		   {l1ang, 0, angMax, angMax},
		   {l1x, 0, xMax, xMax},
		   {l1y, 0, yMax, yMax},
		   {l2ang, 0, angMax, angMax},
		   {l2x, 0, xMax, xMax},
		   {l2y, 0, yMax, yMax},
		   {l3ang, 0, angMax, angMax},
		   {l3x, 0, xMax, xMax},
		   {l3y, 0, yMax, yMax}
		  ];
DumpSave["squish_cosfiddles.mx", cosfiddles];

Show[
    Histogram[#[[1]]& /@ Flatten[sinfiddles, 8]
	      , HistogramCategories -> 100
	      , PlotLabel -> "sin(2x) term for all possible stretch/twist combinations (2^9 = 512)"
	      , DisplayFunction -> Identity
	     ]
    , Graphics[{
	Text["2^3 = 8", {-0.00015, 15}]
	, Text["2^3 = 8", {0.00015, 15}]
	       }]
    , PlotRange -> {{-0.0002, 0.0002}, {0, 100}}
    , AxesOrigin -> {-0.0002, 0}
    , AspectRatio -> 1/4
    , ImageSize -> 72*8
    , DisplayFunction -> (Display["squish_form0.eps", #, "EPS"]&)
    ];

Select[Flatten[sinfiddles, 8], (Abs[#[[1]]] > 0.000125)&] // TableForm

    SINE TERM      L1ANG    L1X       L1Y       L2ANG    L2X       L2Y       L3ANG    L3X       L3Y
==========================================================================================================

    0.00015672     0        0         0.00005   0        0         0.00005   0        0         0.00005

    0.0001537      0        0         0.00005   0        0         0.00005   0.0001   0         0.00005

    0.000156773    0        0         0.00005   0.0001   0         0.00005   0        0         0.00005

    0.000154955    0        0         0.00005   0.0001   0         0.00005   0.0001   0         0.00005

    0.000154073    0.0001   0         0.00005   0        0         0.00005   0        0         0.00005

    0.000156662    0.0001   0         0.00005   0        0         0.00005   0.0001   0         0.00005

    0.000157141    0.0001   0         0.00005   0.0001   0         0.00005   0        0         0.00005

    0.000156548    0.0001   0         0.00005   0.0001   0         0.00005   0.0001   0         0.00005

    -0.000155518   0        0.00005   0         0        0.00005   0         0        0.00005   0

    -0.000160197   0        0.00005   0         0        0.00005   0         0.0001   0.00005   0

    -0.000154427   0        0.00005   0         0.0001   0.00005   0         0        0.00005   0

    -0.000155155   0        0.00005   0         0.0001   0.00005   0         0.0001   0.00005   0

    -0.000151401   0.0001   0.00005   0         0        0.00005   0         0        0.00005   0

    -0.000154729   0.0001   0.00005   0         0        0.00005   0         0.0001   0.00005   0

    -0.000154748   0.0001   0.00005   0         0.0001   0.00005   0         0        0.00005   0

    -0.000155294   0.0001   0.00005   0         0.0001   0.00005   0         0.0001   0.00005   0

{innerTrans, midTrans, outerTrans} = makeTransforms[0, 0, -0.0003, -0.0004, 0, -0.0003, 0, 0, -0.0003];
MultipleListPlot[{{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, d0missAve[phi, 20, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/20}]
		 , Axes -> False
		 , Frame -> True
		 , PlotRange -> {{0, 2*Pi},{-0.000500, 0.000500}}
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , FrameLabel -> {"phi0 of the positive track", "d0miss",
				  "300 micron Y squish; -0.4 mrad middle rotation", ""}
		 , ImageSize -> 72*8
		 , DisplayFunction -> (Display["squish_form1.eps", #, "EPS"]&)
		];

{innerTrans, midTrans, outerTrans} = makeTransforms[0, 0.0003, 0, -0.0004, 0.0003, 0, 0, 0.0003, 0];
MultipleListPlot[{{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, d0missAve[phi, 20, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/20}]
		 , Axes -> False
		 , Frame -> True
		 , PlotRange -> {{0, 2*Pi},{-0.000500, 0.000500}}
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , FrameLabel -> {"phi0 of the positive track", "d0miss",
				  "300 micron X stretch; -0.4 mrad middle rotation", ""}
		 , ImageSize -> 72*8
		 , DisplayFunction -> (Display["squish_form2.eps", #, "EPS"]&)
		];

{innerTrans, midTrans, outerTrans} = makeTransforms[0, 0.00015, -0.00015, -0.0004, 0.00015, -0.00015, 0, 0.00015, -0.00015];
MultipleListPlot[{{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, d0missAve[phi, 20, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/20}]
		 , Axes -> False
		 , Frame -> True
		 , PlotRange -> {{0, 2*Pi},{-0.000500, 0.000500}}
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , FrameLabel -> {"phi0 of the positive track", "d0miss",
				  "150 micron Y squish; 150 micron X stretch; -0.4 mrad middle rotation", ""}
		 , ImageSize -> 72*8
		 , DisplayFunction -> (Display["squish_form3.eps", #, "EPS"]&)
		];


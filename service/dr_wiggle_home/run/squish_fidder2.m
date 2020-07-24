x0 = -0.000060;
y0 = 0.000896;
cross = -0.00763007;
c = 0.04;
l1 = 0.37;
l2 = 0.58;
l3 = 0.79;

poscirc[phi_] := Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]},
			{r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}];
negcirc[phi_] := Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]},
			{r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}];

seeCircles[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] :=
    Show[
	ParametricPlot[{{r1*Sin[sp/r1+phi1]+xc1, -r1*Cos[sp/r1+phi1]+yc1},
			{r2*Sin[sp/r2+phi2]+xc2, -r2*Cos[sp/r2+phi2]+yc2}},
		       {sp, 0, .9*Pi*r1},
		       AspectRatio -> 1,
		       PlotRange -> {{-1,1},{-1,1}},
		       PlotPoints -> 1000,
		       AxesOrigin -> {0,0},
		       PlotStyle -> {RGBColor[1,0,0], RGBColor[0,0,1]},
		       DisplayFunction -> Identity
		      ],
	Graphics[{
	    PointSize[0.02],
	    RGBColor[1,0,0],
	    Point[{r1*Sin[phi1]+xc1, -r1*Cos[phi1]+yc1}],
	    Point[{xc1,yc1}],
	    PointSize[0.01],
	    RGBColor[0,0,1],
	    Point[{r2*Sin[phi2]+xc2, -r2*Cos[phi2]+yc2}],
	    Point[{xc2,yc2}]
		 }],
	DisplayFunction -> Identity
	];
seeCircles[poscirc[Pi/4], negcirc[Pi/4]];

d0miss[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] :=
    (* The minus sign STAYS! *)
    (Sqrt[xc1^2 + yc1^2] - r1) + (-Sqrt[xc2^2 + yc2^2] - r2);
Plot[d0miss[poscirc[phi], negcirc[phi]], {phi,0,2*Pi}];

phi0miss[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] :=
    (xc1*yc2 - yc1*xc2)/Sqrt[xc1^2 + yc1^2]/Sqrt[xc2^2 + yc2^2];
Plot[phi0miss[poscirc[phi], negcirc[phi]], {phi,0,2*Pi}];



bisect[f_, lo_, hi_] := Module[{flo = N[f[lo]], fhi = N[f[hi]], mid},
			       mid = N[lo - (hi - lo)*flo/(fhi - flo)];
			       If[Abs[f[mid]] > 1*^-5,  (* tolerance stops recursion *)
				  If[f[mid]*flo < 0, bisect[f, lo, mid], bisect[f, mid, hi]],
				  mid]
			      ];

Show[Plot[(-1/3*x^3-2*x+8), {x,2.20738,2.2074}, DisplayFunction -> Identity],
     Graphics[{PointSize[0.02], Point[{2.207389723362058,0}]}],
     DisplayFunction -> $DisplayFunction];

bisect[(-1/3*#^3-2*#+8)&, -5, 5] // InputForm


hit[{r_,xc_,yc_,phi_}, L_] := Module[
    {sp = bisect[((r*Sin[#/r+phi]+xc)^2 + (-r*Cos[#/r+phi]+yc)^2 - L^2)&, 0, 2*L]},
    {r*Sin[sp/r+phi]+xc, -r*Cos[sp/r+phi]+yc}
				    ];


hit[poscirc[Pi/8], 0.8]

Out[33]= {0.728097, 0.331459}


hit[poscirc[Pi/8], 0.8]


Table[
Show[seeCircles[poscirc[i*Pi/24], negcirc[i*Pi/24]],
     Graphics[{
	 PointSize[0.02],
	 Point[hit[negcirc[i*Pi/24], 0.8]],
	 Circle[{0,0}, 0.8]
	      }],
     DisplayFunction -> $DisplayFunction
    ],
    {i,0,48}]


hits[circle_, l1t_, l2t_, l3t_] :=
    {l1t.hit[circle, l1], l2t.hit[circle, l2], l3t.hit[circle, l3]};
hits[circle_] := hits[circle,
		      DiagonalMatrix[{1,1}],
		      DiagonalMatrix[{1,1}],
		      DiagonalMatrix[{1,1}]];


hits[poscirc[Pi/8],
     DiagonalMatrix[{1,1}],
     DiagonalMatrix[{1,1}],
     DiagonalMatrix[{1,1}]]

Out[8]= {{0.339141, 0.147899}, {0.529868, 0.235864}, {0.719125, 0.327039}}

Clear[circResid2];
circResid2[{r_, xc_, yc_}, {{x1_, y1_}, {x2_, y2_}, {x3_, y3_}}] :=
    (Sqrt[(x1 - xc)^2 + (y1 - yc)^2] - Abs[r])^2 +
    (Sqrt[(x2 - xc)^2 + (y2 - yc)^2] - Abs[r])^2 +
    (Sqrt[(x3 - xc)^2 + (y3 - yc)^2] - Abs[r])^2;

poscirc[Pi/8]

Out[62]= {12.5, -4.80046, 11.5424, 0.394159}

circResid2[{12.5, -4.80046, 11.5424}, hits[poscirc[Pi/8]]]



reFit[circle_, l1t_, l2t_, l3t_] := {r, xc, yc, circle[[4]]} /.
    Module[{eqn = circResid2[{r, xc, yc}, hits[circle, l1t, l2t, l3t]]},
    FindMinimum[eqn,
		{r, {1,1.01}*circle[[1]]},
		{xc, {1,1.01}*circle[[2]]},
		{yc, {1,1.01}*circle[[3]]}
	       ][[2]]
	  ];

reFit[poscirc[Pi/8],
      DiagonalMatrix[{1,1}],
      DiagonalMatrix[{1,1}],
      DiagonalMatrix[{1,1}]]

Out[92]= {12.5, -4.80046, 11.5424, 0.394159}

stretchY = DiagonalMatrix[{1,1.2}]

Out[15]= {{1, 0}, {0, 1.2}}

layer[l_] := Table[l*{Cos[t],Sin[t]}, {t, 0, 2*Pi, 2*Pi/100}];

seeItAll[acirc_, bcirc_, l1t_, l2t_, l3t_] :=
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
	, DisplayFunction -> $DisplayFunction
	];
seeItAll[poscirc[Pi*3/8], negcirc[Pi*3/8], stretchY, stretchY, stretchY];

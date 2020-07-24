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

trial = Range[0, 2.3, 0.1];

(* randomPair[phi_] := Module[{c = 0.04/0.2*Sqrt[(1 + mytrial^2)/5^2], *)
(* 			    x0 = Random[NormalDistribution[-0.000060, 0.00065]], *)
(* 			    y0 = Random[NormalDistribution[0.000896, 0.00075]]}, *)
(* 			   {Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]}, *)
(* 				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}], *)
(* 			    Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]}, *)
(* 				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}]} *)
(* 			  ] *)

nobeamspot[{one_, two_}] := {{one[[1]], one[[2]] - -0.000060, one[[3]] - 0.000896, one[[4]]},
			     {two[[1]], two[[2]] - -0.000060, two[[3]] - 0.000896, two[[4]]}}

randomPair[phi_] := Module[{c = Random[Real, {0.04, 0.1}],
			    x0 = Random[NormalDistribution[-0.000060, 0.00065]],
			    y0 = Random[NormalDistribution[0.000896, 0.00075]]},
			   {Module[{r = (1/2/c), myphi = phi - cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}],
			    Module[{r = (-1/2/c), myphi = phi + Pi + cross/2*Sin[phi]},
				   {r, x0 - r*Sin[myphi], y0 + r*Cos[myphi], myphi}]}
			  ]

d0alone[{r1_,xc1_,yc1_,phi1_}, {r2_,xc2_,yc2_,phi2_}] := (Sqrt[(xc1 - -0.000060)^2 + (yc1 - 0.000896)^2] - r1);

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

d0aloneAve[phi_, n_, l1t_, l2t_, l3t_] := {Mean[#], StandardDeviation[#]/Sqrt[n]}&[
    Table[d0alone @@ (reFit[#, l1t, l2t, l3t]& /@ randomPair[phi]), {n}]];

d0missAve[phi_, n_, l1t_, l2t_, l3t_] := {Mean[#], StandardDeviation[#]/Sqrt[n]}&[
    Table[d0miss @@ (reFit[#, l1t, l2t, l3t]& /@ randomPair[phi]), {n}]];

curvmissAve[phi_, n_, l1t_, l2t_, l3t_] := {Mean[#], StandardDeviation[#]/Sqrt[n]}&[
    Table[curvmiss @@ (reFit[#, l1t, l2t, l3t]& /@ randomPair[phi]), {n}]];

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

{innerTrans, midTrans, outerTrans} = makeTransforms[0, 0.00015, -0.00015, 0., 0.00015, -0.00015, 0, 0.00015, -0.00015];

mytrial = trial[[1]];

mytrial = trial[[Length[trial]]];

varytheta = Table[{mytrial = trial[[i]], sinterm[20, 20, {innerTrans, midTrans, outerTrans}]*Pi}, {i, 1, Length[trial]}]

varytheta // InputForm

{{0, -0.0029502240642628854}, {0.1, -0.002946864524009965}, 
 {0.2, -0.002948849071208672}, {0.30000000000000004, -0.0029469984756447243}, 
 {0.4, -0.0029489599036705396}, {0.5, -0.0029439926527691693}, 
 {0.6000000000000001, -0.0029484114104563522}, 
 {0.7000000000000001, -0.0029464752239650306}, {0.8, -0.002947681384932129}, 
 {0.9, -0.0029446036679548673}, {1., -0.002947534561712386}, 
 {1.1, -0.0029443499402927475}, {1.2000000000000002, -0.0029380605466129022}, 
 {1.3, -0.002938491078856976}, {1.4000000000000001, -0.0029363676673966987}, 
 {1.5, -0.0029370233291520927}, {1.6, -0.002929299105911688}, 
 {1.7000000000000002, -0.0029276118376960355}, {1.8, -0.002928052812766678}, 
 {1.9000000000000001, -0.0029276044840949884}, {2., -0.0029227705631834655}, 
 {2.1, -0.0029156442075961}, {2.2, -0.0029119090771078182}, 
 {2.3, -0.002910842204456331}}


Show[
    ListPlot[varytheta
	     , Frame -> True
	     , PlotRange -> {{0, 2.3}, {-0.00300, -0.00290}}
	     , Axes -> False
	     , FrameLabel -> {"cot(theta) of bhabha",
			      "sin(2x) component of d0miss VS phi (m)",
			      "Toy MC stereo section squished 300 microns", ""}
	     , DisplayFunction -> Identity
	    ]
    , Graphics[{
	Line[{{0,-0.00298},{2.3,-0.00298}}]
	, {Line[{{Sqrt[5^2*#^2-1],-0.00298},{Sqrt[5^2*#^2-1],-0.002979}}]
	   , Text[ToString[#], {Sqrt[5^2*#^2-1],-0.002985}]}&
	/@ {0.201, 0.21, 0.22, 0.24, 0.26, 0.28, 0.30, 0.35, 0.40, 0.45}
	, Text["curvature of bhabha (1/m)", {2.3/2, -0.00299}]
	     }]
    , ImageSize -> 8*72
    , DisplayFunction -> (Display["sf4_domiss_lacks_theta_dependence.eps", #, "EPS"]&)
    ];

whattoplot = {{#[[1]], #[[2,1]]}, ErrorBar[#[[2,2]]]}& /@
		 Table[{phi, d0aloneAve[phi, 10000, innerTrans, midTrans, outerTrans]},
		       {phi, 0, 2*Pi, 2*Pi/20}];

whattoplot // InputForm

{{{0, -0.000025273096085194702}, ErrorBar[{-7.437178913932292*^-6, 
    7.437178913932292*^-6}]}, {{Pi/10, -0.00010658258925498166}, 
  ErrorBar[{-7.4100489562634945*^-6, 7.4100489562634945*^-6}]}, 
 {{Pi/5, -0.00014209467667407756}, ErrorBar[{-7.07429156411488*^-6, 
    7.07429156411488*^-6}]}, {{(3*Pi)/10, -0.0001287547722601002}, 
  ErrorBar[{-6.855103922881675*^-6, 6.855103922881675*^-6}]}, 
 {{(2*Pi)/5, -0.00007432688483044805}, 
  ErrorBar[{-6.670438065959511*^-6, 6.670438065959511*^-6}]}, 
 {{Pi/2, 0.000016819516439373494}, ErrorBar[{-6.569049496871907*^-6, 
    6.569049496871907*^-6}]}, {{(3*Pi)/5, 0.00011218203158808064}, 
  ErrorBar[{-6.6634449319292215*^-6, 6.6634449319292215*^-6}]}, 
 {{(7*Pi)/10, 0.00015321178770416664}, 
  ErrorBar[{-6.884570331616564*^-6, 6.884570331616564*^-6}]}, 
 {{(4*Pi)/5, 0.0001388080569514579}, 
  ErrorBar[{-7.216671583931964*^-6, 7.216671583931964*^-6}]}, 
 {{(9*Pi)/10, 0.00007264322944926951}, 
  ErrorBar[{-7.4185837564572804*^-6, 7.4185837564572804*^-6}]}, 
 {{Pi, -0.000021982667689518465}, ErrorBar[{-7.4755652173073395*^-6, 
    7.4755652173073395*^-6}]}, {{(11*Pi)/10, -0.00008369451671125834}, 
  ErrorBar[{-7.430496495976786*^-6, 7.430496495976786*^-6}]}, 
 {{(6*Pi)/5, -0.00014555632306476144}, 
  ErrorBar[{-7.0919192798961185*^-6, 7.0919192798961185*^-6}]}, 
 {{(13*Pi)/10, -0.00013485539039777432}, 
  ErrorBar[{-6.827069458177495*^-6, 6.827069458177495*^-6}]}, 
 {{(7*Pi)/5, -0.0000631234635159764}, 
  ErrorBar[{-6.5399435487462604*^-6, 6.5399435487462604*^-6}]}, 
 {{(3*Pi)/2, 0.000016624800263630755}, 
  ErrorBar[{-6.494957512589996*^-6, 6.494957512589996*^-6}]}, 
 {{(8*Pi)/5, 0.00010532925523443772}, 
  ErrorBar[{-6.642336749926015*^-6, 6.642336749926015*^-6}]}, 
 {{(17*Pi)/10, 0.00015244917270691342}, 
  ErrorBar[{-6.869782242533584*^-6, 6.869782242533584*^-6}]}, 
 {{(9*Pi)/5, 0.00013558280902623894}, 
  ErrorBar[{-7.153325899099261*^-6, 7.153325899099261*^-6}]}, 
 {{(19*Pi)/10, 0.00006804225171489548}, 
  ErrorBar[{-7.4088358195283035*^-6, 7.4088358195283035*^-6}]}, 
 {{2*Pi, -6.674714550713023*^-6}, ErrorBar[{-7.49235565344252*^-6, 
    7.49235565344252*^-6}]}}








MultipleListPlot[whattoplot
		 , Axes -> False
		 , Frame -> True
		 , Axes -> True
		 , AxesOrigin -> {0,0}
		 , PlotRange -> {{0, 2*Pi}, {-0.0003, 0.0003}}
		 , FrameLabel -> {"phi0", "d0 (not d0miss) in meters",
				  "300-micron squished DR expectations for D0", None
				 }
		 , ImageSize -> 8*72
		 , DisplayFunction -> (Display["sf4_twocycle_in_d0.eps", #, "EPS"]&)
		];



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

(* Not just yet...
sag = {2.28*^-05, 2.33*^-05, 3.23*^-05, 3.28*^-05, 4.45*^-05, 4.51*^-05,
       5.48*^-05, 5.54*^-05, 6.44*^-05, 6.5*^-05, 7.82*^-05, 7.89*^-05,
       8.81*^-05, 8.88*^-05, 0.0001073, 0.0001079, 0.000112, 0.0001137,
       0.0001154, 0.0001171, 0.0001187, 0.0001204, 0.0001221,
       0.0001238, 0.0001255, 0.0001272, 0.0001289, 0.0001306,
       0.0001322, 0.0001339, 0.0001356, 0.0001373, 0.000138, 0.0001388,
       0.0001395, 0.0001403, 0.0001411, 0.0001419, 0.0001427,
       0.0001435, 0.0001443, 0.0001451, 0.0001459, 0.0001467,
       0.0001475, 0.0001483, 0.0001485}; *)

(* These are only valid for stereo layers, but that's all I'm simulating anyway. *)
zend[rad_] := (1.16-1.085)*rad/(0.79-0.37) + 1.085 - 0.0660714;
eastPhiWire[layer_, wire_] := phi0east[[layer]] + wire*(2*Pi)/numberWires[[layer]];
westPhiWire[layer_, wire_] := phi0west[[layer]] + wire*(2*Pi)/numberWires[[layer]];
eastPoint[layer_, wire_] := (radius[[layer]]*{Cos[eastPhiWire[layer, wire]], Sin[eastPhiWire[layer, wire]], 0} +
			     {0, 0, -zend[radius[[layer]]]});
westPoint[layer_, wire_] := (radius[[layer]]*{Cos[westPhiWire[layer, wire]], Sin[westPhiWire[layer, wire]], 0} +
			     {0, 0, zend[radius[[layer]]]});
transform = {{_Integer | _Real, _Integer | _Real, _Integer | _Real},
	     {_Integer | _Real, _Integer | _Real, _Integer | _Real},
	     {_Integer | _Real, _Integer | _Real, _Integer | _Real},
	     {_Integer | _Real, _Integer | _Real, _Integer | _Real}};
wireLine[layer_, wire_, eastTrans:transform, westTrans:transform] :=
    Module[{eastMatrix = eastTrans[[{1,2,3}]], eastCenter = eastTrans[[4]],
	    westMatrix = westTrans[[{1,2,3}]], westCenter = westTrans[[4]]},
	   {(eastMatrix . (eastPoint[layer, wire]-eastCenter))+eastCenter, (westMatrix . (westPoint[layer, wire]-westCenter))+westCenter}];
wireLine[layer_, wire_] :=
    wireLine[layer, wire, Join[IdentityMatrix[3], {{0,0,0}}], Join[IdentityMatrix[3], {{0,0,0}}]];

(* This is how I determined the form of pca[]
{(x1-x2)*t + x2, (y1-y2)*t + y2, (z1-z2)*t + z2} /.
Solve[D[((x1-x2)*t + x2 - a)^2 + ((y1-y2)*t + y2 - b)^2 + ((z1-z2)*t + z2 - c)^2, t] == 0, t][[1]]
    // Simplify // InputForm
*)
pcaInternal = Compile[{a, b, c, x1, y1, z1, x2, y2, z2},
		      {x2 - ((x1 - x2)*((x1 - x2)*(-a + x2) + (y1 - y2)*(-b + y2) + (z1 - z2)*(-c + z2)))/
		       ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2), 
		       y2 - ((y1 - y2)*((x1 - x2)*(-a + x2) + (y1 - y2)*(-b + y2) + (z1 - z2)*(-c + z2)))/
		       ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2), 
		       z2 - ((z1 - z2)*((x1 - x2)*(-a + x2) + (y1 - y2)*(-b + y2) + (z1 - z2)*(-c + z2)))/
		       ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)}];
pca[{a_,b_,c_}, {{x1_,y1_,z1_}, {x2_, y2_, z2_}}] :=
    pcaInternal[a, b, c, x1, y1, z1, x2, y2, z2];
vca[{a_,b_,c_}, {{x1_,y1_,z1_}, {x2_, y2_, z2_}}] :=
    pca[{a,b,c}, {{x1,y1,z1},{x2,y2,z2}}] - {a,b,c};
dca[{a_,b_,c_}, {{x1_,y1_,z1_}, {x2_, y2_, z2_}}] :=
    Sqrt[#[[1]]^2 + #[[2]]^2 + #[[3]]^2]&[vca[{a,b,c}, {{x1,y1,z1},{x2,y2,z2}}]];

helixPointInternal = Compile[{sp, curv, phi0, d0, cotTh, z0},
			     Module[{rho = 2 * curv,
				     u0 = Cos[phi0],
				     v0 = Sin[phi0],
				     x0 = -d0 * Sin[phi0],
				     y0 =  d0 * Cos[phi0]},
				    {x0 + (u0/rho) * Sin[rho*sp] - (v0/rho) * (1-Cos[rho*sp]),
				     y0 + (v0/rho) * Sin[rho*sp] + (u0/rho) * (1-Cos[rho*sp]),
				     z0 + cotTh * sp}]];
helixPoint[sp_, {curv_, phi0_, d0_, cotTh_, z0_}] :=
    helixPointInternal[sp, curv, phi0, d0, cotTh, z0];

(* D[dca[helixPoint[sp, {curv, phi0, d0, cotTh, z0}], {{x1, y1, z1}, {x2, y2, z2}}], sp] // InputForm *)
helixDcaDerivativeInternal =
    Compile[{sp, curv, phi0, d0, cotTh, z0, x1, y1, z1, x2, y2, z2},
	    (2*(-(Cos[phi0]*Cos[2*curv*sp]) + Sin[phi0]*Sin[2*curv*sp] - 
	    ((x1 - x2)*(-(cotTh*(z1 - z2)) + (y1 - y2)*(-(Cos[2*curv*sp]*Sin[phi0]) - 
	    Cos[phi0]*Sin[2*curv*sp]) + (x1 - x2)*(-(Cos[phi0]*Cos[2*curv*sp]) + 
	    Sin[phi0]*Sin[2*curv*sp])))/((x1 - x2)^2 + (y1 - y2)^2 + 
	    (z1 - z2)^2))*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv) - 
	    ((x1 - x2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)) + 
	    2*(-(Cos[2*curv*sp]*Sin[phi0]) - Cos[phi0]*Sin[2*curv*sp] - 
	    ((y1 - y2)*(-(cotTh*(z1 - z2)) + (y1 - y2)*(-(Cos[2*curv*sp]*Sin[phi0]) - 
	    Cos[phi0]*Sin[2*curv*sp]) + (x1 - x2)*(-(Cos[phi0]*Cos[2*curv*sp]) + 
	    Sin[phi0]*Sin[2*curv*sp])))/((x1 - x2)^2 + (y1 - y2)^2 + 
	    (z1 - z2)^2))*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv) - 
	    ((y1 - y2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)) + 
	    2*(-cotTh - ((z1 - z2)*(-(cotTh*(z1 - z2)) + 
	    (y1 - y2)*(-(Cos[2*curv*sp]*Sin[phi0]) - Cos[phi0]*Sin[2*curv*sp]) + 
	    (x1 - x2)*(-(Cos[phi0]*Cos[2*curv*sp]) + Sin[phi0]*Sin[2*curv*sp])))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2))*(-(cotTh*sp) - z0 + z2 - 
	    ((z1 - z2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2)))/
	    (2*Sqrt[(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/(2*curv) - 
	    (Cos[phi0]*Sin[2*curv*sp])/(2*curv) - 
	    ((x1 - x2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2))^2 + 
	    (y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/(2*curv) - 
	    (Sin[phi0]*Sin[2*curv*sp])/(2*curv) - 
	    ((y1 - y2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2))^2 + 
	    (-(cotTh*sp) - z0 + z2 - ((z1 - z2)*((z1 - z2)*(-(cotTh*sp) - z0 + z2) + 
	    (x1 - x2)*(x2 + d0*Sin[phi0] + ((1 - Cos[2*curv*sp])*Sin[phi0])/
	    (2*curv) - (Cos[phi0]*Sin[2*curv*sp])/(2*curv)) + 
	    (y1 - y2)*(y2 - d0*Cos[phi0] - (Cos[phi0]*(1 - Cos[2*curv*sp]))/
	    (2*curv) - (Sin[phi0]*Sin[2*curv*sp])/(2*curv))))/
	    ((x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2))^2])];
helixDcaDerivative[sp_, {curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    helixDcaDerivativeInternal[sp, curv, phi0, d0, cotTh, z0, x1, y1, z1, x2, y2, z2];

bisectEpsilon = 1*^-9;
bisect[f_, {low_, high_}, {flow_, fhigh_}] :=
    Module[{mid, fmid},
	   mid = -flow * (high - low)/(fhigh - flow) + low;
	   fmid = N[f[mid]];
	   If[fmid > bisectEpsilon, bisect[f, {low, mid}, {flow, fmid}],
	      If[fmid < -bisectEpsilon, bisect[f, {mid, high}, {fmid, fhigh}],
		 mid]]];
bisect[f_, {low_, high_}] := bisect[f, {low, high}, {f[low], f[high]}];

minimizeHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    Module[{sp = bisect[helixDcaDerivative[#, {curv, phi0, d0, cotTh, z0},
						      {{x1,y1,z1},{x2,y2,z2}}]&, {0, Pi/2/curv}]},
	   {sp, pca[helixPoint[sp, {curv, phi0, d0, cotTh, z0}], {{x1,y1,z1}, {x2,y2,z2}}]}];
spHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    minimizeHelixWire[{curv, phi0, d0, cotTh, z0}, {{x1,y1,z1},{x2,y2,z2}}][[1]];
pcaHelixOnWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    minimizeHelixWire[{curv, phi0, d0, cotTh, z0}, {{x1,y1,z1},{x2,y2,z2}}][[2]];
pcaHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    helixPoint[minimizeHelixWire[{curv, phi0, d0, cotTh, z0}, {{x1,y1,z1},{x2,y2,z2}}][[1]],
	       {curv, phi0, d0, cotTh, z0}];
vcaHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    Module[{sp, pca},
	   {sp, pca} = minimizeHelixWire[{curv, phi0, d0, cotTh, z0}, {{x1,y1,z1},{x2,y2,z2}}];
	   pca - helixPoint[sp, {curv, phi0, d0, cotTh, z0}]];
dcaHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, {{x1_,y1_,z1_}, {x2_,y2_,z2_}}] :=
    Sqrt[#[[1]]^2 + #[[2]]^2 + #[[3]]^2]&[vcaHelixWire[{curv, phi0, d0, cotTh, z0}, {{x1,y1,z1},{x2,y2,z2}}]];

(* This is to be sure that it works.
Clear[showHelixWire];
showHelixWire[{curv_, phi0_, d0_, cotTh_, z0_}, layer_, wire_, {vx_,vy_,vz_}] :=
    Show[Graphics3D[{
	Line[Table[helixPoint[sp, {curv, phi0, d0, cotTh, z0}], {sp, 0, Pi/2/curv, Pi/2/curv/100}]],
	Table[Line[wireLine[layer, w]], {w, 1, numberWires[[layer]]}],
	RGBColor[1,0,0],
	PointSize[0.02],
	Point[pcaHelixWire[{curv, phi0, d0, cotTh, z0}, wireLine[layer, wire]]],
	Point[pcaHelixOnWire[{curv, phi0, d0, cotTh, z0}, wireLine[layer, wire]]]
		    }], ViewPoint -> {vx, vy, vz}];
showHelixWire[{0.2, 0., 0., 0., 0.}, 47, 0, {0, 0, 5}];
showHelixWire[{0.45, 0., 0., 2., 0.}, 47, 0, {0, 0, 5}];
showHelixWire[{1., 0., 0., 0., 0.}, 47, 87, {0, 0, 5}]; *)

generateDcaWire[{curv_, phi0_, d0_, cotTh_, z0_}, layer_, wire_] :=
    dcaHelixWire[{curv, phi0, d0, cotTh, z0}, wireLine[layer, wire]];
generateManyDcaLayer[{curv_, phi0_, d0_, cotTh_, z0_}, layer_] :=
    Table[{wire, generateDcaWire[{curv, phi0, d0, cotTh, z0}, layer, wire]},
	  {wire, 1, numberWires[[layer]]}];
generateManyDcaLayer[{curv_, phi0_, d0_, cotTh_, z0_}, layer_, guessWire_] :=
    {#, generateDcaWire[{curv, phi0, d0, cotTh, z0}, layer, #]}& /@
    ((Mod[#, numberWires[[layer]]] + 1)& /@ Range[guessWire-5, guessWire+5]);  (* Only look within +/- 5 wires *)
generateDcaLayer[{curv_, phi0_, d0_, cotTh_, z0_}, layer_] :=
    Module[{possibilities = generateManyDcaLayer[{curv, phi0, d0, cotTh, z0}, layer],
	    minDca},
	   minDca = Min[#[[2]]& /@ possibilities];
	   Select[possibilities, (#[[2]] == minDca)&][[1]]];  (* I could be general here, but I won't. *)
generateDcaLayer[{curv_, phi0_, d0_, cotTh_, z0_}, layer_, guessWire_] :=
    Module[{possibilities = generateManyDcaLayer[{curv, phi0, d0, cotTh, z0}, layer, guessWire],
	    minDca},
	   minDca = Min[#[[2]]& /@ possibilities];
	   Select[possibilities, (#[[2]] == minDca)&][[1]]];  (* I could be general here, but I won't. *)
generateHits[{curv_, phi0_, d0_, cotTh_, z0_}, {layerStart_, layerStop_}] :=
    Module[{hits, wire, dca, layer},
	   {wire, dca} = generateDcaLayer[{curv, phi0, d0, cotTh, z0}, layerStart];
	   hits = {{layerStart, wire, dca}};
	   For[layer = layerStart+1, layer <= layerStop, layer++,
	       ({wire, dca} = generateDcaLayer[{curv, phi0, d0, cotTh, z0}, layer, wire];
		hits = Join[hits, {{layer, wire, dca}}])];
           hits
	  ];
generateHits[{curv_, phi0_, d0_, cotTh_, z0_}] :=
    generateHits[{curv, phi0, d0, cotTh, z0}, {17, 47}];
hitlist = {{_Integer, _Integer, _Integer | _Real}..};
explicithitlist = {{{{_Integer | _Real, _Integer | _Real, _Integer | _Real},
		     {_Integer | _Real, _Integer | _Real, _Integer | _Real}},
		    _Integer | _Real}..};

chiSquare[{curv_, phi0_, d0_, cotTh_, z0_}, hits:explicithitlist] :=
    Plus @@ ((dcaHelixWire[{curv, phi0, d0, cotTh, z0}, #[[1]]] - #[[2]])^2& /@ hits);
chiSquare[{curv_, phi0_, d0_, cotTh_, z0_}, hits:hitlist, eastTrans:transform, westTrans:transform] :=
    chiSquare[{curv, phi0, d0, cotTh, z0}, {wireLine[#[[1]], #[[2]], eastTrans, westTrans], #[[3]]}& /@ hits];
chiSquare[{curv_, phi0_, d0_, cotTh_, z0_}, hits:hitlist] :=
    chiSquare[{curv, phi0, d0, cotTh, z0}, hits, Join[IdentityMatrix[3], {{0,0,0}}], Join[IdentityMatrix[3], {{0,0,0}}]];

(* Too bad...

Clear[mini1D];
mini1DEpsilon = 1*^-3;
mini1D[f_, start_?VectorQ, step_?VectorQ] :=
    Module[{fm = f[start - step],
	    fz = f[start],
	    fp = f[start + step],
	    min, end},
	   min = N[(1/2)*((fp - fm)/(2*fz - fp - fm))];
	   end = start + min*step;
	   If[Abs[min] > 1,
	      mini1D[f, end, step],
	      If[Sqrt[Plus @@ (#^2& /@ step)] > mini1DEpsilon,
		 mini1D[f, end, min*step],
		 end]
	     ]
	  ] /; Length[start] == Length[step];

Clear[miniTrack];
miniTrackEpsilon = 1*^-3;
miniTrackParamSteps = {0.01, 0.01, 0.00005, 0.05, 0.001};
miniTrack[{curv_, phi0_, d0_, cotTh_, z0_}, {cstep_, pstep_, dstep_, tstep_, zstep_},
	  hits:hitlist, eastTrans:transform, westTrans:transform] :=
    Module[{explicitHits = {wireLine[#[[1]], #[[2]], eastTrans, westTrans], #[[3]]}& /@ hits,
	    firstStep, secondStep, curvStep},
	   firstStep = {curv, 0, 0, 0, 0} +
	   {0, 1, 0, 0, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, pstep, 0, 0, 0}] +
	   {0, 0, 1, 0, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, dstep, 0, 0}] +
	   {0, 0, 0, 1, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, 0, tstep, 0}] +
	   {0, 0, 0, 0, 1} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, 0, 0, zstep}];
	   Print["firstStep = ", firstStep];
	   secondStep = mini1D[chiSquare[#, explicitHits]&, firstStep, {curv, phi0, d0, cotTh, z0} - firstStep];
	   Print["secondStep = ", secondStep];
	   curvStep = mini1D[chiSquare[#, explicitHits]&, secondStep, {cstep, 0, 0, 0, 0}];
	   Print["curvStep = ", curvStep];
	   Print["full step size is ", Sqrt[Plus @@ (#^2& /@ ((curvStep - firstStep) / miniTrackParamSteps))]];
	   If[Sqrt[Plus @@ (#^2& /@ ((curvStep - firstStep) / miniTrackParamSteps))] > miniTrackEpsilon,
	      miniTrack[curvStep, firstStep - curvStep, hits, eastTrans, westTrans],
	      curvStep]];
miniTrack[{curv_, phi0_, d0_, cotTh_, z0_}, hits:hitlist, eastTrans:transform, westTrans:transform] :=
    miniTrack[{curv, phi0, d0, cotTh, z0}, miniTrackParamSteps, hits, eastTrans, westTrans];

Clear[miniTrack];
miniTrackEpsilon = 1*^-12;
miniTrackParamSteps = {0.01, 0.01, 0.00005, 0.05, 0.001};
miniTrack[{curv_, phi0_, d0_, cotTh_, z0_}, {cstep_, pstep_, dstep_, tstep_, zstep_},
	  hits:hitlist, eastTrans:transform, westTrans:transform] :=
    Module[{explicitHits = {wireLine[#[[1]], #[[2]], eastTrans, westTrans], #[[3]]}& /@ hits,
	    startChi, endChi, basisJump, directedJump},
	   startChi = chiSquare[{curv, phi0, d0, cotTh, z0}, explicitHits];
	   basisJump = (
	       {1, 0, 0, 0, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {cstep, 0, 0, 0, 0}] +
	       {0, 1, 0, 0, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, pstep, 0, 0, 0}] +
	       {0, 0, 1, 0, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, dstep, 0, 0}] +
	       {0, 0, 0, 1, 0} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, 0, tstep, 0}] +
	       {0, 0, 0, 0, 1} * mini1D[chiSquare[#, explicitHits]&, {curv, phi0, d0, cotTh, z0}, {0, 0, 0, 0, zstep}]);
	   Print["basisJump = ", basisJump];
	   directedJump = mini1D[chiSquare[#, explicitHits]&, basisJump, {curv, phi0, d0, cotTh, z0} - basisJump];
	   Print["directedJump = ", directedJump];
	   endChi = chiSquare[directedJump, explicitHits];
	   Print["startChi - endChi = ", InputForm[startChi - endChi]];
	   If[Abs[startChi - endChi] > miniTrackEpsilon,
	      miniTrack[directedJump, {curv, phi0, d0, cotTh, z0} - directedJump, hits, eastTrans, westTrans],
	      directedJump]];
miniTrack[{curv_, phi0_, d0_, cotTh_, z0_}, hits:hitlist, eastTrans:transform, westTrans:transform] :=
    miniTrack[{curv, phi0, d0, cotTh, z0}, miniTrackParamSteps, hits, eastTrans, westTrans];

miniTrack[{1.001, 0.001, 0.000005, 0.005, 0.0001}, sampleHits, IdentityMatrix[3], IdentityMatrix[3]]
*)

reFit[{curv_, phi0_, d0_, cotTh_, z0_}, hits:hitlist, eastTrans:transform, westTrans:transform] :=
    Module[{explicitHits = {wireLine[#[[1]], #[[2]], eastTrans, westTrans], #[[3]]}& /@ hits},
	   {c, p, d, t, z} /. FindMinimum[chiSquare[{c, p, d, t, z}, explicitHits],
					  {c, curv, curv + 0.000002},
					  {p, phi0, phi0 + 0.00005},
					  {d, d0, d0 + 0.00005},
					  {t, cotTh, cotTh + 0.04},
					  {z, z0, z0 + 0.02}
					  , AccuracyGoal -> 14
					  , MaxIterations -> 100
					 ][[2]]];




(* Timing estimations (no significant changes since z was fixed...
lnx193> lsl lookfor_strong_thetadependence.m
-rw-r--r--    1 mccann   cleo        38565 May  7 17:44 lookfor_strong_thetadependence.m
lnx193> lsl lookfor_varyPhi0d1mm.mx 
-rw-r--r--    1 mccann   cleo         5896 May  7 18:55 lookfor_varyPhi0d1mm.mx
lnx193> lsl lookfor_varyPhi1d1mm.mx 
-rw-r--r--    1 mccann   cleo         3612 May  7 19:35 lookfor_varyPhi1d1mm.mx
lnx193> lsl lookfor_varyPhi2d1mm.mx 
-rw-r--r--    1 mccann   cleo         3612 May  7 20:09 lookfor_varyPhi2d1mm.mx
lnx193> lsl lookfor_varyPhi0dm1mm.mx 
-rw-r--r--    1 mccann   cleo         5896 May  7 21:21 lookfor_varyPhi0dm1mm.mx
lnx193> lsl lookfor_varyPhi1dm1mm.mx 
-rw-r--r--    1 mccann   cleo         3612 May  7 21:56 lookfor_varyPhi1dm1mm.mx
lnx193> lsl lookfor_varyPhi2dm1mm.mx 
-rw-r--r--    1 mccann   cleo         3612 May  7 22:31 lookfor_varyPhi2dm1mm.mx
*)

Print["Loaded all definitions, ready to do some number-crunching..."];

(* (\* First study: classic squish *\) *)

(* classicSquish = Join[DiagonalMatrix[{1 + 150*^-6/0.80, 1 - 150*^-6/0.80, 1}], {{0,0,0}}]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* varyPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_varyPhi0.mx", varyPhi0]; *)
(* Save["lookfor_varyPhi0.m", varyPhi0]; *)
(* Print["wrote lookfor_varyPhi0.mx..."]; *)
(* varyPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_varyPhi1.mx", varyPhi1]; *)
(* Save["lookfor_varyPhi1.m", varyPhi1]; *)
(* Print["wrote lookfor_varyPhi1.mx..."]; *)
(* varyPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_varyPhi2.mx", varyPhi2]; *)
(* Save["lookfor_varyPhi2.m", varyPhi2]; *)
(* Print["wrote lookfor_varyPhi2.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* varyPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_varyPhi0d1mm.mx", varyPhi0d1mm]; *)
(* Save["lookfor_varyPhi0d1mm.m", varyPhi0d1mm]; *)
(* Print["wrote lookfor_varyPhi0d1mm.mx..."]; *)
(* varyPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_varyPhi1d1mm.mx", varyPhi1d1mm]; *)
(* Save["lookfor_varyPhi1d1mm.m", varyPhi1d1mm]; *)
(* Print["wrote lookfor_varyPhi1d1mm.mx..."]; *)
(* varyPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_varyPhi2d1mm.mx", varyPhi2d1mm]; *)
(* Save["lookfor_varyPhi2d1mm.m", varyPhi2d1mm]; *)
(* Print["wrote lookfor_varyPhi2d1mm.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* varyPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_varyPhi0dm1mm.mx", varyPhi0dm1mm]; *)
(* Save["lookfor_varyPhi0dm1mm.m", varyPhi0dm1mm]; *)
(* Print["wrote lookfor_varyPhi0dm1mm.mx..."]; *)
(* varyPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_varyPhi1dm1mm.mx", varyPhi1dm1mm]; *)
(* Save["lookfor_varyPhi1dm1mm.m", varyPhi1dm1mm]; *)
(* Print["wrote lookfor_varyPhi1dm1mm.mx..."]; *)
(* varyPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], classicSquish, classicSquish], *)
(* 		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_varyPhi2dm1mm.mx", varyPhi2dm1mm]; *)
(* Save["lookfor_varyPhi2dm1mm.m", varyPhi2dm1mm]; *)
(* Print["wrote lookfor_varyPhi2dm1mm.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[varyPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[varyPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[varyPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[varyPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[varyPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[varyPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[varyPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[varyPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[varyPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_classicSquish.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[varyPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[varyPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_classicSquish_z0.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[varyPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_classicSquish_z02.eps", #, "EPS"]&)]]; *)


(* First.fifth study: opposing squish *)

eastSquish = Join[DiagonalMatrix[{1 + 150*^-6/0.80, 1 - 150*^-6/0.80, 1}], {{0,0,0}}];
westSquish = Join[DiagonalMatrix[{1 - 150*^-6/0.80, 1 + 150*^-6/0.80, 1}], {{0,0,0}}];

bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.};
opposingSquishVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}];
DumpSave["lookfor_opposingSquishVaryPhi0.mx", opposingSquishVaryPhi0];
Save["lookfor_opposingSquishVaryPhi0.m", opposingSquishVaryPhi0];
Print["wrote lookfor_opposingSquishVaryPhi0.mx..."];
opposingSquishVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}];
DumpSave["lookfor_opposingSquishVaryPhi1.mx", opposingSquishVaryPhi1];
Save["lookfor_opposingSquishVaryPhi1.m", opposingSquishVaryPhi1];
Print["wrote lookfor_opposingSquishVaryPhi1.mx..."];
opposingSquishVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}];
DumpSave["lookfor_opposingSquishVaryPhi2.mx", opposingSquishVaryPhi2];
Save["lookfor_opposingSquishVaryPhi2.m", opposingSquishVaryPhi2];
Print["wrote lookfor_opposingSquishVaryPhi2.mx..."];
bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.};
opposingSquishVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}];
DumpSave["lookfor_opposingSquishVaryPhi0d1mm.mx", opposingSquishVaryPhi0d1mm];
Save["lookfor_opposingSquishVaryPhi0d1mm.m", opposingSquishVaryPhi0d1mm];
Print["wrote lookfor_opposingSquishVaryPhi0d1mm.mx..."];
opposingSquishVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}];
DumpSave["lookfor_opposingSquishVaryPhi1d1mm.mx", opposingSquishVaryPhi1d1mm];
Save["lookfor_opposingSquishVaryPhi1d1mm.m", opposingSquishVaryPhi1d1mm];
Print["wrote lookfor_opposingSquishVaryPhi1d1mm.mx..."];
opposingSquishVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}];
DumpSave["lookfor_opposingSquishVaryPhi2d1mm.mx", opposingSquishVaryPhi2d1mm];
Save["lookfor_opposingSquishVaryPhi2d1mm.m", opposingSquishVaryPhi2d1mm];
Print["wrote lookfor_opposingSquishVaryPhi2d1mm.mx..."];
bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.};
opposingSquishVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}];
DumpSave["lookfor_opposingSquishVaryPhi0dm1mm.mx", opposingSquishVaryPhi0dm1mm];
Save["lookfor_opposingSquishVaryPhi0dm1mm.m", opposingSquishVaryPhi0dm1mm];
Print["wrote lookfor_opposingSquishVaryPhi0dm1mm.mx..."];
opposingSquishVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}];
DumpSave["lookfor_opposingSquishVaryPhi1dm1mm.mx", opposingSquishVaryPhi1dm1mm];
Save["lookfor_opposingSquishVaryPhi1dm1mm.m", opposingSquishVaryPhi1dm1mm];
Print["wrote lookfor_opposingSquishVaryPhi1dm1mm.mx..."];
opposingSquishVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], eastSquish, westSquish],
		      {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}];
DumpSave["lookfor_opposingSquishVaryPhi2dm1mm.mx", opposingSquishVaryPhi2dm1mm];
Save["lookfor_opposingSquishVaryPhi2dm1mm.m", opposingSquishVaryPhi2dm1mm];
Print["wrote lookfor_opposingSquishVaryPhi2dm1mm.mx..."];

p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[opposingSquishVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[opposingSquishVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[opposingSquishVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[opposingSquishVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[opposingSquishVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[opposingSquishVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[opposingSquishVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[opposingSquishVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[opposingSquishVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-20, 20}}, DisplayFunction -> Identity];
Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron opposing squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_opposingSquish.eps", #, "EPS"]&)];

pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[opposingSquishVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[opposingSquishVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-700, 700}}, DisplayFunction -> Identity];
Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron opposing squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_opposingSquish_z0.eps", #, "EPS"]&)];

ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity];
ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity];
ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[opposingSquishVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-200, 200}}, DisplayFunction -> Identity];
Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 300 micron opposing squish", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_opposingSquish_z02.eps", #, "EPS"]&)]];

















(* (\* Second study: shearTogether *\) *)

(* tiltEast = {{1,  0,                   0}, *)
(* 	    {0,  0.9999999847691291,  0.0001745329243133368}, *)
(* 	    {0, -0.0001745329243133368, 0.9999999847691291}, *)
(* 	    {0, 0, -1.25}}; *)
(* tiltWest = {{1,  0,                   0}, *)
(* 	    {0,  0.9999999847691291,  0.0001745329243133368}, *)
(* 	    {0, -0.0001745329243133368, 0.9999999847691291}, *)
(* 	    {0, 0, 1.25}}; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* shearTogetherVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			       {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi0_small.mx", shearTogetherVaryPhi0]; *)
(* Save["lookfor_shearTogetherVaryPhi0_small.m", shearTogetherVaryPhi0]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi0_small.mx..."]; *)
(* shearTogetherVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			       {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi1_small.mx", shearTogetherVaryPhi1]; *)
(* Save["lookfor_shearTogetherVaryPhi1_small.m", shearTogetherVaryPhi1]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi1_small.mx..."]; *)
(* shearTogetherVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			       {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi2_small.mx", shearTogetherVaryPhi2]; *)
(* Save["lookfor_shearTogetherVaryPhi2_small.m", shearTogetherVaryPhi2]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi2_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* shearTogetherVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				   {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi0d1mm_small.mx", shearTogetherVaryPhi0d1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi0d1mm_small.m", shearTogetherVaryPhi0d1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi0d1mm_small.mx..."]; *)
(* shearTogetherVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				   {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi1d1mm_small.mx", shearTogetherVaryPhi1d1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi1d1mm_small.m", shearTogetherVaryPhi1d1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi1d1mm_small.mx..."]; *)
(* shearTogetherVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				   {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi2d1mm_small.mx", shearTogetherVaryPhi2d1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi2d1mm_small.m", shearTogetherVaryPhi2d1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi2d1mm_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* shearTogetherVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi0dm1mm_small.mx", shearTogetherVaryPhi0dm1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi0dm1mm_small.m", shearTogetherVaryPhi0dm1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi0dm1mm_small.mx..."]; *)
(* shearTogetherVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi1dm1mm_small.mx", shearTogetherVaryPhi1dm1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi1dm1mm_small.m", shearTogetherVaryPhi1dm1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi1dm1mm_small.mx..."]; *)
(* shearTogetherVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearTogetherVaryPhi2dm1mm_small.mx", shearTogetherVaryPhi2dm1mm]; *)
(* Save["lookfor_shearTogetherVaryPhi2dm1mm_small.m", shearTogetherVaryPhi2dm1mm]; *)
(* Print["wrote lookfor_shearTogetherVaryPhi2dm1mm_small.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearTogetherVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearTogetherVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearTogetherVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearTogetherVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearTogetherVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearTogetherVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearTogetherVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearTogetherVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearTogetherVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-together", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearTogether2_small.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearTogetherVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearTogetherVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-together", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearTogether_z0_small.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearTogetherVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-together", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearTogether_z02_small.eps", #, "EPS"]&)]]; *)











(* (\* Third study: shearApart *\) *)

(* tiltEast = {{1,  0,                   0}, *)
(* 	    {0,  0.9999999847691291,  0.0001745329243133368}, *)
(* 	    {0, -0.0001745329243133368, 0.9999999847691291}, *)
(* 	    {0, 0, -1.25}}; *)
(* tiltWest = {{1,  0,                    0}, *)
(* 	    {0,  0.9999999847691291,  -0.0001745329243133368}, *)
(* 	    {0,  0.0001745329243133368,  0.9999999847691291}, *)
(* 	    {0, 0, 1.25}}; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* shearApartVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi0_small.mx", shearApartVaryPhi0]; *)
(* Save["lookfor_shearApartVaryPhi0_small.m", shearApartVaryPhi0]; *)
(* Print["wrote lookfor_shearApartVaryPhi0_small.mx..."]; *)
(* shearApartVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi1_small.mx", shearApartVaryPhi1]; *)
(* Save["lookfor_shearApartVaryPhi1_small.m", shearApartVaryPhi1]; *)
(* Print["wrote lookfor_shearApartVaryPhi1_small.mx..."]; *)
(* shearApartVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi2_small.mx", shearApartVaryPhi2]; *)
(* Save["lookfor_shearApartVaryPhi2_small.m", shearApartVaryPhi2]; *)
(* Print["wrote lookfor_shearApartVaryPhi2_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* shearApartVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi0d1mm_small.mx", shearApartVaryPhi0d1mm]; *)
(* Save["lookfor_shearApartVaryPhi0d1mm_small.m", shearApartVaryPhi0d1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi0d1mm_small.mx..."]; *)
(* shearApartVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi1d1mm_small.mx", shearApartVaryPhi1d1mm]; *)
(* Save["lookfor_shearApartVaryPhi1d1mm_small.m", shearApartVaryPhi1d1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi1d1mm_small.mx..."]; *)
(* shearApartVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi2d1mm_small.mx", shearApartVaryPhi2d1mm]; *)
(* Save["lookfor_shearApartVaryPhi2d1mm_small.m", shearApartVaryPhi2d1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi2d1mm_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* shearApartVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi0dm1mm_small.mx", shearApartVaryPhi0dm1mm]; *)
(* Save["lookfor_shearApartVaryPhi0dm1mm_small.m", shearApartVaryPhi0dm1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi0dm1mm_small.mx..."]; *)
(* shearApartVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi1dm1mm_small.mx", shearApartVaryPhi1dm1mm]; *)
(* Save["lookfor_shearApartVaryPhi1dm1mm_small.m", shearApartVaryPhi1dm1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi1dm1mm_small.mx..."]; *)
(* shearApartVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearApartVaryPhi2dm1mm_small.mx", shearApartVaryPhi2dm1mm]; *)
(* Save["lookfor_shearApartVaryPhi2dm1mm_small.m", shearApartVaryPhi2dm1mm]; *)
(* Print["wrote lookfor_shearApartVaryPhi2dm1mm_small.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearApart_small.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearApart_z0_small.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearApart_z02_small.eps", #, "EPS"]&)]]; *)


















(* (\* Fourth study: rotateApart *\) *)

(* rotEast = {{ 0.9999999847691291,  0.0001745329243133368, 0}, *)
(* 	   {-0.0001745329243133368, 0.9999999847691291,  0}, *)
(* 	   { 0,                   0,                   1}, *)
(* 	   {0, 0, 0}}; *)
(* rotWest = {{ 0.9999999847691291,  -0.0001745329243133368, 0}, *)
(* 	   { 0.0001745329243133368,  0.9999999847691291,  0}, *)
(* 	   { 0,                    0,                   1}, *)
(* 	   {0, 0, 0}}; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* rotateApartVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi0_small.mx", rotateApartVaryPhi0]; *)
(* Save["lookfor_rotateApartVaryPhi0_small.m", rotateApartVaryPhi0]; *)
(* Print["wrote lookfor_rotateApartVaryPhi0_small.mx..."]; *)
(* rotateApartVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi1_small.mx", rotateApartVaryPhi1]; *)
(* Save["lookfor_rotateApartVaryPhi1_small.m", rotateApartVaryPhi1]; *)
(* Print["wrote lookfor_rotateApartVaryPhi1_small.mx..."]; *)
(* rotateApartVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi2_small.mx", rotateApartVaryPhi2]; *)
(* Save["lookfor_rotateApartVaryPhi2_small.m", rotateApartVaryPhi2]; *)
(* Print["wrote lookfor_rotateApartVaryPhi2_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* rotateApartVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi0d1mm_small.mx", rotateApartVaryPhi0d1mm]; *)
(* Save["lookfor_rotateApartVaryPhi0d1mm_small.m", rotateApartVaryPhi0d1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi0d1mm_small.mx..."]; *)
(* rotateApartVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi1d1mm_small.mx", rotateApartVaryPhi1d1mm]; *)
(* Save["lookfor_rotateApartVaryPhi1d1mm_small.m", rotateApartVaryPhi1d1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi1d1mm_small.mx..."]; *)
(* rotateApartVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi2d1mm_small.mx", rotateApartVaryPhi2d1mm]; *)
(* Save["lookfor_rotateApartVaryPhi2d1mm_small.m", rotateApartVaryPhi2d1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi2d1mm_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* rotateApartVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi0dm1mm_small.mx", rotateApartVaryPhi0dm1mm]; *)
(* Save["lookfor_rotateApartVaryPhi0dm1mm_small.m", rotateApartVaryPhi0dm1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi0dm1mm_small.mx..."]; *)
(* rotateApartVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi1dm1mm_small.mx", rotateApartVaryPhi1dm1mm]; *)
(* Save["lookfor_rotateApartVaryPhi1dm1mm_small.m", rotateApartVaryPhi1dm1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi1dm1mm_small.mx..."]; *)
(* rotateApartVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], rotEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_rotateApartVaryPhi2dm1mm_small.mx", rotateApartVaryPhi2dm1mm]; *)
(* Save["lookfor_rotateApartVaryPhi2dm1mm_small.m", rotateApartVaryPhi2dm1mm]; *)
(* Print["wrote lookfor_rotateApartVaryPhi2dm1mm_small.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[rotateApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[rotateApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[rotateApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[rotateApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[rotateApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[rotateApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[rotateApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[rotateApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[rotateApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-10, 10}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree rotate-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_rotateApart_small.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[rotateApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[rotateApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree rotate-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_rotateApart_z0_small.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[rotateApartVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree rotate-apart", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_rotateApart_z02_small.eps", #, "EPS"]&)]]; *)















(* (\* Fifth study: shearFunky *\) *)

(* tiltEast = {{1,  0,                   0}, *)
(* 	    {0,  0.9999999847691291,  0.0001745329243133368}, *)
(* 	    {0, -0.0001745329243133368, 0.9999999847691291}, *)
(* 	    {0, 0, -1.25}}; *)
(* tiltWest = {{ 0.9999999847691291,  0,  0.0001745329243133368}, *)
(* 	    { 0,                   1,  0}, *)
(* 	    {-0.0001745329243133368, 0,  0.9999999847691291}, *)
(* 	    {0, 0, 1.25}}; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* shearFunkyVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi0_small.mx", shearFunkyVaryPhi0]; *)
(* Save["lookfor_shearFunkyVaryPhi0_small.m", shearFunkyVaryPhi0]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi0_small.mx..."]; *)
(* shearFunkyVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi1_small.mx", shearFunkyVaryPhi1]; *)
(* Save["lookfor_shearFunkyVaryPhi1_small.m", shearFunkyVaryPhi1]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi1_small.mx..."]; *)
(* shearFunkyVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 			    {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi2_small.mx", shearFunkyVaryPhi2]; *)
(* Save["lookfor_shearFunkyVaryPhi2_small.m", shearFunkyVaryPhi2]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi2_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* shearFunkyVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi0d1mm_small.mx", shearFunkyVaryPhi0d1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi0d1mm_small.m", shearFunkyVaryPhi0d1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi0d1mm_small.mx..."]; *)
(* shearFunkyVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi1d1mm_small.mx", shearFunkyVaryPhi1d1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi1d1mm_small.m", shearFunkyVaryPhi1d1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi1d1mm_small.mx..."]; *)
(* shearFunkyVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				{phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi2d1mm_small.mx", shearFunkyVaryPhi2d1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi2d1mm_small.m", shearFunkyVaryPhi2d1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi2d1mm_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* shearFunkyVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi0dm1mm_small.mx", shearFunkyVaryPhi0dm1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi0dm1mm_small.m", shearFunkyVaryPhi0dm1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi0dm1mm_small.mx..."]; *)
(* shearFunkyVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi1dm1mm_small.mx", shearFunkyVaryPhi1dm1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi1dm1mm_small.m", shearFunkyVaryPhi1dm1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi1dm1mm_small.mx..."]; *)
(* shearFunkyVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, tiltWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearFunkyVaryPhi2dm1mm_small.mx", shearFunkyVaryPhi2dm1mm]; *)
(* Save["lookfor_shearFunkyVaryPhi2dm1mm_small.m", shearFunkyVaryPhi2dm1mm]; *)
(* Print["wrote lookfor_shearFunkyVaryPhi2dm1mm_small.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearFunkyVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearFunkyVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearFunkyVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearFunkyVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearFunkyVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearFunkyVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearFunkyVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearFunkyVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearFunkyVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-two-ways", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearFunky_small.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearFunkyVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearFunkyVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-two-ways", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearFunky_z0_small.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearFunkyVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-two-ways", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearFunky_z02_small.eps", #, "EPS"]&)]]; *)


















(* (\* Sixth study: shearRotate *\) *)

(* tiltEast = {{1,  0,                   0}, *)
(* 	    {0,  0.9999999847691291,  0.0001745329243133368}, *)
(* 	    {0, -0.0001745329243133368, 0.9999999847691291}, *)
(* 	    {0, 0, -1.25}}; *)
(* rotWest = {{ 0.9999999847691291,  -0.0001745329243133368, 0}, *)
(* 	   { 0.0001745329243133368,  0.9999999847691291,  0}, *)
(* 	   { 0,                    0,                   1}, *)
(* 	   {0, 0, 0}}; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0., cotTh, 0.}; *)
(* shearRotateVaryPhi0 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi0_small.mx", shearRotateVaryPhi0]; *)
(* Save["lookfor_shearRotateVaryPhi0_small.m", shearRotateVaryPhi0]; *)
(* Print["wrote lookfor_shearRotateVaryPhi0_small.mx..."]; *)
(* shearRotateVaryPhi1 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi1_small.mx", shearRotateVaryPhi1]; *)
(* Save["lookfor_shearRotateVaryPhi1_small.m", shearRotateVaryPhi1]; *)
(* Print["wrote lookfor_shearRotateVaryPhi1_small.mx..."]; *)
(* shearRotateVaryPhi2 = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 			     {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi2_small.mx", shearRotateVaryPhi2]; *)
(* Save["lookfor_shearRotateVaryPhi2_small.m", shearRotateVaryPhi2]; *)
(* Print["wrote lookfor_shearRotateVaryPhi2_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, 0.001, cotTh, 0.}; *)
(* shearRotateVaryPhi0d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi0d1mm_small.mx", shearRotateVaryPhi0d1mm]; *)
(* Save["lookfor_shearRotateVaryPhi0d1mm_small.m", shearRotateVaryPhi0d1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi0d1mm_small.mx..."]; *)
(* shearRotateVaryPhi1d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi1d1mm_small.mx", shearRotateVaryPhi1d1mm]; *)
(* Save["lookfor_shearRotateVaryPhi1d1mm_small.m", shearRotateVaryPhi1d1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi1d1mm_small.mx..."]; *)
(* shearRotateVaryPhi2d1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				 {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi2d1mm_small.mx", shearRotateVaryPhi2d1mm]; *)
(* Save["lookfor_shearRotateVaryPhi2d1mm_small.m", shearRotateVaryPhi2d1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi2d1mm_small.mx..."]; *)
(* bhabhaTrack[phi0_, cotTh_] := {0.04*Sqrt[1+cotTh^2], phi0, -0.001, cotTh, 0.}; *)
(* shearRotateVaryPhi0dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 0., 0.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi0dm1mm_small.mx", shearRotateVaryPhi0dm1mm]; *)
(* Save["lookfor_shearRotateVaryPhi0dm1mm_small.m", shearRotateVaryPhi0dm1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi0dm1mm_small.mx..."]; *)
(* shearRotateVaryPhi1dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 1., 1.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi1dm1mm_small.mx", shearRotateVaryPhi1dm1mm]; *)
(* Save["lookfor_shearRotateVaryPhi1dm1mm_small.m", shearRotateVaryPhi1dm1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi1dm1mm_small.mx..."]; *)
(* shearRotateVaryPhi2dm1mm = Table[reFit[bhabhaTrack[phi0, cotTh], generateHits[bhabhaTrack[phi0, cotTh]], tiltEast, rotWest], *)
(* 				  {phi0, 0., 2.*Pi, Pi/10.}, {cotTh, 2., 2.}]; *)
(* DumpSave["lookfor_shearRotateVaryPhi2dm1mm_small.mx", shearRotateVaryPhi2dm1mm]; *)
(* Save["lookfor_shearRotateVaryPhi2dm1mm_small.m", shearRotateVaryPhi2dm1mm]; *)
(* Print["wrote lookfor_shearRotateVaryPhi2dm1mm_small.mx..."]; *)

(* p1 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearRotateVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p2 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearRotateVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p3 = ListPlot[{#[[1,2]], (#[[1,3]]-0.001)*1*^6}& /@ Select[shearRotateVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p4 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearRotateVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p5 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearRotateVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p6 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearRotateVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p7 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearRotateVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p8 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearRotateVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* p9 = ListPlot[{#[[1,2]], (#[[1,3]]+0.001)*1*^6}& /@ Select[shearRotateVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta d0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-50, 50}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{p1, p2, p3}, {p4, p5, p6}, {p7, p8, p9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-and-rotate", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearRotate_small.eps", #, "EPS"]&)]; *)

(* pp1 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp2 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp3 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp4 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp5 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp6 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp7 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 0, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp8 = ListPlot[{#[[1,2]], (#[[1,5]])*1*^6}& /@ Select[shearRotateVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 1, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* pp9 = ListPlot[{#[[1,2]], (#[[1,3]])*1*^6}& /@ Select[shearRotateVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&], Frame -> True, FrameLabel -> {"phi0", "delta z0 (microns)", "cotTh = 2, nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1, PlotRange -> {{0, 2*Pi}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{pp1, pp2, pp3}, {pp4, pp5, pp6}, {pp7, pp8, pp9}}], PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-and-rotate", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearRotate_z0_small.eps", #, "EPS"]&)]; *)

(* ppp1 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi0d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi1d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi2d1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* ppp2 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi0, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi1, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi2, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = 0mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* ppp3 = MultipleListPlot[{{{0, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi0dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{1, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi1dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)], {{2, Mean[#]}, ErrorBar[StandardDeviation[#]/Sqrt[Length[#]]]}&[(#[[1,5]]& /@ Select[shearRotateVaryPhi2dm1mm, MatchQ[#, {{_Real, _Real, _Real, _Real, _Real}}]&]*1*^6)]}, Frame -> True, FrameLabel -> {"cotTheta", "delta z0 (microns)", "nominal d0 = -1mm", None}, AxesOrigin -> {0,0}, AspectRatio -> 1/4, PlotRange -> {{-0.1, 2.1}, {-100, 100}}, DisplayFunction -> Identity]; *)
(* Show[GraphicsArray[{{ppp1},{ppp2},{ppp3}}, PlotLabel -> "Tracks and hits generated with no distortions, refit with 0.01 degree shear-and-rotate", ImageSize -> 9*72, DisplayFunction -> (Display["lookfor_shearRotate_z02_small.eps", #, "EPS"]&)]]; *)

(* Exit *)





(* Now read it all back in again *)

(* << lookforVaryPhi0.mx; *)
(* << lookforVaryPhi1.mx; *)
(* << lookforVaryPhi2.mx; *)
(* << lookforVaryPhi0d1mm.mx; *)
(* << lookforVaryPhi1d1mm.mx; *)
(* << lookforVaryPhi2d1mm.mx; *)
(* << lookforVaryPhi0dm1mm.mx; *)
(* << lookforVaryPhi1dm1mm.mx; *)
(* << lookforVaryPhi2dm1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi0.mx; *)
(* << lookfor_shearTogetherVaryPhi1.mx; *)
(* << lookfor_shearTogetherVaryPhi2.mx; *)
(* << lookfor_shearTogetherVaryPhi0d1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi1d1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi2d1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi0dm1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi1dm1mm.mx; *)
(* << lookfor_shearTogetherVaryPhi2dm1mm.mx; *)
(* << lookfor_shearApartVaryPhi0.mx; *)
(* << lookfor_shearApartVaryPhi1.mx; *)
(* << lookfor_shearApartVaryPhi2.mx; *)
(* << lookfor_shearApartVaryPhi0d1mm.mx; *)
(* << lookfor_shearApartVaryPhi1d1mm.mx; *)
(* << lookfor_shearApartVaryPhi2d1mm.mx; *)
(* << lookfor_shearApartVaryPhi0dm1mm.mx; *)
(* << lookfor_shearApartVaryPhi1dm1mm.mx; *)
(* << lookfor_shearApartVaryPhi2dm1mm.mx; *)
(* << lookfor_rotateApartVaryPhi0.mx; *)
(* << lookfor_rotateApartVaryPhi1.mx; *)
(* << lookfor_rotateApartVaryPhi2.mx; *)
(* << lookfor_rotateApartVaryPhi0d1mm.mx; *)
(* << lookfor_rotateApartVaryPhi1d1mm.mx; *)
(* << lookfor_rotateApartVaryPhi2d1mm.mx; *)
(* << lookfor_rotateApartVaryPhi0dm1mm.mx; *)
(* << lookfor_rotateApartVaryPhi1dm1mm.mx; *)
(* << lookfor_rotateApartVaryPhi2dm1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi0.mx; *)
(* << lookfor_shearFunkyVaryPhi1.mx; *)
(* << lookfor_shearFunkyVaryPhi2.mx; *)
(* << lookfor_shearFunkyVaryPhi0d1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi1d1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi2d1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi0dm1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi1dm1mm.mx; *)
(* << lookfor_shearFunkyVaryPhi2dm1mm.mx; *)
(* << lookfor_shearRotateVaryPhi0.mx; *)
(* << lookfor_shearRotateVaryPhi1.mx; *)
(* << lookfor_shearRotateVaryPhi2.mx; *)
(* << lookfor_shearRotateVaryPhi0d1mm.mx; *)
(* << lookfor_shearRotateVaryPhi1d1mm.mx; *)
(* << lookfor_shearRotateVaryPhi2d1mm.mx; *)
(* << lookfor_shearRotateVaryPhi0dm1mm.mx; *)
(* << lookfor_shearRotateVaryPhi1dm1mm.mx; *)
(* << lookfor_shearRotateVaryPhi2dm1mm.mx; *)









(* varyPhi0 *)
(* varyPhi1 *)
(* varyPhi2 *)
(* varyPhi0d1mm *)
(* varyPhi1d1mm *)
(* varyPhi2d1mm *)
(* varyPhi0dm1mm *)
(* varyPhi1dm1mm *)
(* varyPhi2dm1mm *)
(* shearTogetherVaryPhi0 *)
(* shearTogetherVaryPhi1 *)
(* shearTogetherVaryPhi2 *)
(* shearTogetherVaryPhi0d1mm *)
(* shearTogetherVaryPhi1d1mm *)
(* shearTogetherVaryPhi2d1mm *)
(* shearTogetherVaryPhi0dm1mm *)
(* shearTogetherVaryPhi1dm1mm *)
(* shearTogetherVaryPhi2dm1mm *)
(* shearApartVaryPhi0 *)
(* shearApartVaryPhi1 *)
(* shearApartVaryPhi2 *)
(* shearApartVaryPhi0d1mm *)
(* shearApartVaryPhi1d1mm *)
(* shearApartVaryPhi2d1mm *)
(* shearApartVaryPhi0dm1mm *)
(* shearApartVaryPhi1dm1mm *)
(* shearApartVaryPhi2dm1mm *)
(* rotateApartVaryPhi0 *)
(* rotateApartVaryPhi1 *)
(* rotateApartVaryPhi2 *)
(* rotateApartVaryPhi0d1mm *)
(* rotateApartVaryPhi1d1mm *)
(* rotateApartVaryPhi2d1mm *)
(* rotateApartVaryPhi0dm1mm *)
(* rotateApartVaryPhi1dm1mm *)
(* rotateApartVaryPhi2dm1mm *)
(* shearFunkyVaryPhi0 *)
(* shearFunkyVaryPhi1 *)
(* shearFunkyVaryPhi2 *)
(* shearFunkyVaryPhi0d1mm *)
(* shearFunkyVaryPhi1d1mm *)
(* shearFunkyVaryPhi2d1mm *)
(* shearFunkyVaryPhi0dm1mm *)
(* shearFunkyVaryPhi1dm1mm *)
(* shearFunkyVaryPhi2dm1mm *)
(* shearRotateVaryPhi0 *)
(* shearRotateVaryPhi1 *)
(* shearRotateVaryPhi2 *)
(* shearRotateVaryPhi0d1mm *)
(* shearRotateVaryPhi1d1mm *)
(* shearRotateVaryPhi2d1mm *)
(* shearRotateVaryPhi0dm1mm *)
(* shearRotateVaryPhi1dm1mm *)
(* shearRotateVaryPhi2dm1mm *)









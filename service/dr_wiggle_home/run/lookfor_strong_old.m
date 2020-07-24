(* **************************** OLD ********************************* *)

(* This came from Tuesday night *)
                                             -7                                                       -8
Out[155]= {{{9.96029, -0.00712653, 7.82456 10  , 60.132, -0.0258885}}, {{9.9586, 0.304293, -9.73938 10  , 20.9804, 0.00174181}}, 
 
                                    -7                                                       -6
>    {{9.95338, 0.629142, 6.29514 10  , 21.1382, -0.303279}}, {{9.94685, 0.947174, 4.76028 10  , -4.65541, 0.0556774}}, 
 
                                  -6                                                      -6
>    {{9.9415, 1.26041, 5.43579 10  , -3.29708, 0.0413052}}, {{9.93942, 1.57255, 7.7107 10  , -126.899, 17.6685}}, 
 
                                    -6                                                      -6
>    {{9.94074, 1.88403, -2.04132 10  , -46.7095, 6.85371}}, {{9.94587, 2.19597, -5.27425 10  , -63.8451, 9.85868}}, 
 
                                    -6
>    {{9.95228, 2.50966, -6.83911 10  , 0.418218, 0.621428}}, {{9.95739, 2.82463, -0.0000114828, 2.55641, -0.0567499}}, 
 
                                                                                          -6
>    {{9.95821, 3.14076, -0.000025305, 308.665, -43.9514}}, {{9.95938, 3.45687, 5.16965 10  , 22.7294, -3.19216}}, 
 
>    {{9.95582, 3.77202, 0.0000173601, 3.60545, -0.0217428}}, {{9.94971, 4.0878, 0.0000299879, 41.0344, -5.61966}}, 
 
>    {{9.94291, 4.40137, 0.0000180476, 1.8967, -0.0716788}}, {{10.0845, 4.71011, 0.00144213, -17.9187, 2.60333}}, 
 
                                  -7                      -7                                 -7
>    {{9.94118, 5.02919, 9.5331 10  , 1.99937, -4.38593 10  }}, {{9.94651, 5.34267, 3.9517 10  , -7.31019, -0.0181137}}, 
 
                                   -6                                                       -6
>    {{9.95312, 5.64754, 1.13991 10  , 28.4565, 0.00607567}}, {{9.95863, 5.95302, 6.27474 10  , -0.315013, 0.991374}}, 
 
                                    -6
>    {{9.96029, 6.27598, -1.07524 10  , 60.6301, -0.0263808}}}

ListPlot[{#[[1,2]], #[[1,4]]}& /@ Out[155]];














(* ************************** TESTS ********************************* *)

(* This was used to calibrate the reFit function *)

sampleHits = generateHits[{0.45, Pi/4., 0., 2., 0.}]

Out[274]= {{17, 26, 0.000697637}, {18, 27, 0.00410133}, {19, 26, 0.00449117}, {20, 27, 0.00051617}, {21, 33, 0.00322076}, {22, 34, 0.0000966347}, 
 
>    {23, 34, 0.00315989}, {24, 35, 0.00664058}, {25, 35, 0.00433571}, {26, 35, 0.004712}, {27, 35, 0.000468963}, {28, 36, 0.00403459}, {29, 42, 0.0174021}, 
 
>    {30, 44, 0.00232713}, {31, 44, 0.000120761}, {32, 45, 0.0026701}, {33, 44, 0.000596451}, {34, 45, 0.00303588}, {35, 45, 0.00682015}, 
 
>    {36, 45, 0.00333181}, {37, 51, 0.0423062}, {38, 55, 0.00280929}, {39, 55, 0.00117951}, {40, 56, 0.000499438}, {41, 54, 0.00277819}, 
 
>    {42, 55, 0.000110335}, {43, 55, 0.003095}, {44, 56, 0.00616528}, {45, 62, 0.0528643}, {46, 67, 0.00208838}, {47, 67, 0.00131673}}

sampleTransform = Join[DiagonalMatrix[{1 + 150*^-6/0.80, 1 - 150*^-6/0.80, 1}], {{0,0,0}}]

Out[282]= {{1.00019, 0, 0}, {0, 0.999813, 0}, {0, 0, 1}, {0, 0, 0}}

noTransform = Join[IdentityMatrix[3], {{0,0,0}}]

Out[271]= {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}}

Module[{startTime, result, endTime},
       startTime = TimeUsed[];
       result = reFit[{0.45, Pi/4., 0., 2., 0.}, sampleHits, sampleTransform, sampleTransform];
       endTime = TimeUsed[];
       {endTime - startTime, result}]

Out[291]= {65.73, {0.449998, 0.78535, -0.000053049, 2.00057, -0.00028652}}  (* okay, 11 *)  (* I'm going to go for the whole AccuracyGoal -> 14, 1.5-day program *)

Out[289]= {38.2, {0.449997, 0.785351, -0.0000529684, 2.00026, -0.000121506}}  (* and now 9 *)

Out[287]= {38.2, {0.449997, 0.785351, -0.0000529684, 2.00026, -0.000121506}}  (* Let's try AccuracyGoal of 8 *)

                                                 -6                     -8
Out[285]= {13.38, {0.449842, 0.78539, -6.70058 10  , 2.00033, 6.66495 10  }}  (* now back to an AccuracyGoal of 6 *)

Out[283]= {65.69, {0.449998, 0.78535, -0.000053049, 2.00057, -0.00028652}}  (* no, it has more to do with the solution being far from the starting point *)

Out[277]= {11.08, {0.45, 0.785398, 0., 2., 0.}}  (* same with the old AccuracyGoal again... I see: it was just a whole lot harder with little stereo angle *)

Out[275]= {11.07, {0.45, 0.785398, 0., 2., 0.}}  (* with AccuracyGoal reduced from 14 to 6 (and a corrected wireLine[] *)

Out[136]= {57.32, {0.450002, 0.78535, -0.0000537966, 2.0394, -0.0241712}}  (* with a set of steps that came from the size of the shift *)

Out[132]= {49.88, {0.450002, 0.78535, -0.0000537961, 2.0394, -0.0241721}}  (* with a guessed set of steps *)

(* change the default behaviour *)
wireLine[layer_, wire_] :=
    wireLine[layer, wire, sampleTransform, sampleTransform];

generateHits[Out[136][[2]]]

Out[139]= {{17, 27, 0.00341877}, {18, 28, 0.000656902}, {19, 28, 0.00503319}, {20, 28, 0.00442367}, {21, 32, 0.00297207}, {22, 32, 0.00630547}, 
 
>    {23, 32, 0.00253168}, {24, 33, 0.00150582}, {25, 36, 0.0052518}, {26, 37, 0.00182984}, {27, 37, 0.00176073}, {28, 38, 0.00556742}, {29, 41, 0.00415577}, 
 
>    {30, 42, 0.00107274}, {31, 42, 0.00217941}, {32, 43, 0.00558525}, {33, 46, 0.00457351}, {34, 47, 0.00180335}, {35, 47, 0.00108046}, 
 
>    {36, 48, 0.00408363}, {37, 52, 0.00402737}, {38, 53, 0.00656276}, {39, 52, 0.00430982}, {40, 53, 0.00183509}, {41, 57, 0.00161794}, 
 
>    {42, 58, 0.000476591}, {43, 58, 0.00263377}, {44, 59, 0.00482208}, {45, 63, 0.00122327}, {46, 64, 0.00295277}, {47, 64, 0.0046952}}

(* Be sure that we're not actually closer to a different set of wires... *)
({#[[1]], #[[2]]}& /@ Out[139])  ==  ({#[[1]], #[[2]]}& /@ Out[124])

Out[141]= True  (* great! *)

(* reset the default behaviour *)
wireLine[layer_, wire_] :=
    wireLine[layer, wire, Join[IdentityMatrix[3], {{0,0,0}}], Join[IdentityMatrix[3], {{0,0,0}}]];

(* These were all computed before I applied any transformations. *)

                             -14                            -7                 -7
Out[116]= {157.24, {2.8042 10   , {c -> 1., p -> -6.67516 10  , d -> 2.25676 10  , t -> -0.0000505299, z -> -0.000079738}}}  (* AccuracyGoal -> 14 *)
(* This demonstrates that the fitter can be trusted to +/- 0.00005 in
   cotTheta, +/- 0.00008 in z0, and perfectly well enough in curv,
   phi0, and d0. *)

                             -8
Out[115]= {32.89, {1.27924 10  , {c -> 0.99911, p -> 0.000895061, d -> -0.000307657, t -> 0.0264876, z -> -0.0358762}}}  (* PrecisionGoal -> 12 *)

                             -8
Out[114]= {33.02, {1.27924 10  , {c -> 0.99911, p -> 0.000895061, d -> -0.000307657, t -> 0.0264876, z -> -0.0358762}}}  (* PrecisionGoal -> 4 *)

                             -14                            -7                 -7
Out[113]= {156.73, {2.8042 10   , {c -> 1., p -> -6.67516 10  , d -> 2.25676 10  , t -> -0.0000505299, z -> -0.000079738}}}  (* AccuracyGoal -> 12 *)

                             -8
Out[112]= {17.36, {8.43354 10  , {c -> 0.999015, p -> 0.000923536, d -> -0.000196617, t -> 0.140818, z -> -0.0231887}}}  (* AccuracyGoal -> 4 *)

                             -8
Out[110]= {32.77, {1.27924 10  , {c -> 0.99911, p -> 0.000895061, d -> -0.000307657, t -> 0.0264876, z -> -0.0358762}}}  (* default *)


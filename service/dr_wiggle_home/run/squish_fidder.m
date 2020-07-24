ArcSin[-0.00763]

Out[1]= -0.00763007

d0 = Sqrt[((Cot[phi]*x0 + y0)/(Tan[phi]+Cot[phi]) - x0)^2 +
	  ((Cot[phi]*x0 + y0)*Sin[phi]^2 - y0)^2]

                                                2 2
Out[27]= Sqrt[(-y0 + (y0 + x0 Cot[phi]) Sin[phi] )  + 
 
              y0 + x0 Cot[phi]   2
>     (-x0 + -------------------) ]
             Cot[phi] + Tan[phi]

Plot[
    {d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f},
     d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f+0.00763007*Sin[f]}},
    {f,0,2*Pi}
    , PlotRange -> {{0,2*Pi},{0,0.001}}
    , PlotStyle -> {RGBColor[1,0,0],RGBColor[0,0,1]}
    , DisplayFunction -> (Display["tmp1.eps", #, "EPS"]&)
    ];

Plot[
    (d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f}) - 
    (d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f+0.00763007*Sin[f]}),
    {f,0,2*Pi}
    , PlotRange -> {{0,2*Pi},{-0.00001,0.00001}}
    , DisplayFunction -> (Display["tmp2.eps", #, "EPS"]&)
    ];

Plot[
    (If[#<0,-#,#]&)[(d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f}) - 
    (d0 /. {x0 -> -0.00006, y0 -> 0.0008959, phi -> f+0.00763007*Sin[f]})],
    {f,0,2*Pi}
    , PlotRange -> {{0,2*Pi},{-0.00001,0.00001}}
    , DisplayFunction -> (Display["tmp3.eps", #, "EPS"]&)
    ];


Exit

[Mathematica finished.]

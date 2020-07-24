Off[General::spell];
$DisplayFunction =
    ( Display[ "tmp.gif", #, "GIF" ];
      Run[ "kill -9 `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];
      # )&;

xconstraint = ( (xprime-x)/yprime == (x0-x)/y0 )

        -x + xprime    -x + x0
Out[2]= ----------- == -------
          yprime         y0

zconstraint = ( (zprime-z)/yprime == (z0-z)/y0 )

        -z + zprime    -z + z0
Out[3]= ----------- == -------
          yprime         y0

bowconstraint = ( ( yprime + (l^2 - 4 s^2)/(8s) )^2 + zprime^2 ==
		  ( (l^2 + 4 s^2)/(8s) )^2 )

          2      2                           2      2 2
         l  - 4 s           2         2    (l  + 4 s )
Out[4]= (--------- + yprime)  + zprime  == ------------
            8 s                                   2
                                              64 s

zprimesol = Solve[ zconstraint, zprime ]

                      -(y0 z) + yprime z - yprime z0
Out[6]= {{zprime -> -(------------------------------)}}
                                    y0

yprimesols = Solve[ ( bowconstraint /. zprimesol ), yprime ]

Out[8]= {{yprime ->

         2   2      2   2           2
>      (l  y0  - 4 s  y0  - 8 s y0 z  + 8 s y0 z z0 -

                   2   2       2   2           2               2
>         Sqrt[(-(l  y0 ) + 4 s  y0  + 8 s y0 z  - 8 s y0 z z0)  -

                2     2         2  2          2        2                    2
>           4 (l  s y0  - 4 s y0  z ) (-4 s y0  - 4 s z  + 8 s z z0 - 4 s z0 )

                            2        2                    2
>           ]) / (2 (-4 s y0  - 4 s z  + 8 s z z0 - 4 s z0 ))},

>    {yprime ->

         2   2      2   2           2
>      (l  y0  - 4 s  y0  - 8 s y0 z  + 8 s y0 z z0 +

                   2   2       2   2           2               2
>         Sqrt[(-(l  y0 ) + 4 s  y0  + 8 s y0 z  - 8 s y0 z z0)  -

                2     2         2  2          2        2                    2
>           4 (l  s y0  - 4 s y0  z ) (-4 s y0  - 4 s z  + 8 s z z0 - 4 s z0 )

                            2        2                    2
>           ]) / (2 (-4 s y0  - 4 s z  + 8 s z z0 - 4 s z0 ))}}

TableForm[
    Table[ Prepend[ ( ( yprime /. # )& /@ yprimesols ) /.
		    { l -> 0.15, s -> 0.000100, x -> 0,
		      x0 -> 0, y0 -> 1., z0 -> z }, z ],
	   { z, 0., 0.075, 0.005 } ],
    TableHeadings -> { None, {"z and z0", "yprime (sol1)", "yprime (sol2)"} }
	 ]

Out[20]//TableForm=    z and z0   yprime (sol1)   yprime (sol2)
                       0.         0.0001          -56.25

                       0.005      0.0000995556    -56.25

                       0.01       0.0000982222    -56.25

                       0.015      0.000096        -56.25

                       0.02       0.0000928889    -56.25

                       0.025      0.0000888889    -56.25

                       0.03       0.000084        -56.25

                       0.035      0.0000782223    -56.25

                       0.04       0.0000715556    -56.25

                       0.045      0.000064        -56.25

                       0.05       0.0000555556    -56.25

                       0.055      0.0000462223    -56.2499

                       0.06       0.000036        -56.2499

                       0.065      0.0000248889    -56.2499

                       0.07       0.0000128889    -56.2499

                       0.075      0.              -56.2499

So the real solution to yprime is:

**************************************************************************
*									 *
* (l^2 y0^2 - 4 s^2 y0^2 - 8 s y0 z^2 + 8 s y0 z z0 - 			 *
* 									 *
*   Sqrt[(-(l^2 y0^2) + 4 s^2 y0^2 + 8 s y0 z^2 - 8 s y0 z z0)^2 - 	 *
* 									 *
*     4 (l^2 s y0^2 - 4 s y0^2 z^2) (-4 s y0^2 - 4 s z^2 + 8 s z z0 - 	 *
* 									 *
*       4 s z0^2)])/(2 (-4 s y0^2 - 4 s z^2 + 8 s z z0 - 4 s z0^2))	 *
*									 *
**************************************************************************

And the solution to zprime is:

**************************************************************************
* 									 *
* (Sqrt[y0^2 ((l^2 y0 - 4 s (s y0 + 2 z (z - z0)))^2 + 			 *
* 									 *
*        16 s^2 (l^2 - 4 z^2) (y0^2 + (z - z0)^2))] (-z + z0) + 	 *
* 									 *
*    y0^2 (l^2 (z - z0) + 4 s (2 y0 z + s (-z + z0))))/			 *
* 									 *
*   (8 s y0 (y0^2 + (z - z0)^2))					 *
*									 *
**************************************************************************

The solution to xprime is:

**************************************************************************
* 									 *
* (l^2 (x - x0) y0^2 + (-x + x0) 					 *
* 									 *
*     Sqrt[y0^2 ((l^2 y0 - 4 s (s y0 + 2 z (z - z0)))^2 + 		 *
* 									 *
*        16 s^2 (l^2 - 4 z^2) (y0^2 + (z - z0)^2))] + 			 *
* 									 *
*    4 s y0 (s (-x + x0) y0 + 2 (x y0^2 + x0 z^2 -			 *
* 									 *
*                                (x + x0) z z0 + x z0^2))) /		 *
* 									 *
*   (8 s y0 (y0^2 + (z - z0)^2))					 *
*									 *
**************************************************************************

yprime = (l^2 y0^2 - 4 s^2 y0^2 - 8 s y0 z^2 + 8 s y0 z z0 -
	  Sqrt[(-(l^2 y0^2) + 4 s^2 y0^2 + 8 s y0 z^2 - 8 s y0 z z0)^2 -
	       4 (l^2 s y0^2 - 4 s y0^2 z^2)
	       (-4 s y0^2 - 4 s z^2 + 8 s z z0 - 4 s z0^2)]) /
    (2 (-4 s y0^2 - 4 s z^2 + 8 s z z0 - 4 s z0^2));

zprime = (Sqrt[y0^2 ((l^2 y0 - 4 s (s y0 + 2 z (z - z0)))^2 +
		     16 s^2 (l^2 - 4 z^2) (y0^2 + (z - z0)^2))] (-z + z0) +
	  y0^2 (l^2 (z - z0) + 4 s (2 y0 z + s (-z + z0)))) /
    (8 s y0 (y0^2 + (z - z0)^2));

xprime = (l^2 (x - x0) y0^2 + (-x + x0)
	  Sqrt[y0^2 ((l^2 y0 - 4 s (s y0 + 2 z (z - z0)))^2 +
		     16 s^2 (l^2 - 4 z^2) (y0^2 + (z - z0)^2))] +
	  4 s y0 (s (-x + x0) y0 + 2 (x y0^2 + x0 z^2
				      - (x + x0) z z0 + x z0^2))) /
    (8 s y0 (y0^2 + (z - z0)^2));

{ zlow = -0.075, zhigh = 0.075, zstep = ( zhigh - zlow ) / 6,
  xlow = -0.025, xhigh = 0.025, xstep = ( xhigh - xlow ) / 3 };

Here is a graphical check that all residuals go to zero when s -> 0:

Plot[ { (x - xprime) /. { l -> 0.15, x -> 0.05, z -> 0.15/4.,
			  x0 -> 0, y0 -> 0.1, z0 -> 0. },
	(z - zprime) /. { l -> 0.15, x -> 0.05, z -> -0.15/4.,
			  x0 -> 0, y0 -> 0.1, z0 -> 0. } },
      {s, -0.000100, 0.000100},
      PlotStyle -> { RGBColor[1,0,0], RGBColor[0,0,1] } ]

These are the rphi residuals:

TableForm[
    Table[ (x - xprime) /. { l -> 0.15, s -> 0.000100,
			     x0 -> 0, y0 -> 0.1, z0 -> 0 },
	   {z, zlow, zhigh, zstep}, {x, xlow, xhigh, xstep} ]
	 ]

Out[72]//TableForm= 
 
>              -17             -17              -17             -17
    -2.77556 10      4.68375 10      -4.68375 10      2.77556 10

                                -6             -6
    -0.0000139013    -4.63375 10     4.63375 10       0.0000139013

                                -6             -6
    -0.0000222272    -7.40905 10     7.40905 10       0.0000222272

                                -6             -6
    -0.000025        -8.33333 10     8.33333 10       0.000025

                                -6             -6
    -0.0000222272    -7.40905 10     7.40905 10       0.0000222272

                                -6             -6
    -0.0000139013    -4.63375 10     4.63375 10       0.0000139013

               -17             -17              -17             -17
    -2.77556 10      4.68375 10      -4.68375 10      2.77556 1

These are the z residuals:

TableForm[
    Table[ (z - zprime) /. { l -> 0.15, s -> 0.000100,
			     x0 -> 0, y0 -> 0.1, z0 -> 0 },
	   {z, zlow, zhigh, zstep}, {x, xlow, xhigh, xstep} ]
	 ]

Out[73]//TableForm= 
 
>             -15              -15              -15              -15
    1.83187 10       1.83187 10       1.83187 10       1.83187 10

    -0.0000278025    -0.0000278025    -0.0000278025    -0.0000278025

    -0.0000222272    -0.0000222272    -0.0000222272    -0.0000222272

    0.               0.               0.               0.

    0.0000222272     0.0000222272     0.0000222272     0.0000222272

    0.0000278025     0.0000278025     0.0000278025     0.0000278025

               -15              -15              -15              -15
    -1.83187 10      -1.83187 10      -1.83187 10      -1.83187 10

This is definitely not what I see. But had I seen a linear rphi
dependance, I would have straightened it using y and phix.

(* **************************************************************** *)

rot = ( { { 1,         0,          0 },
	  { 0, Cos[phix], -Sin[phix] },
	  { 0, Sin[phix],  Cos[phix] } }
	.
	{ { Cos[phiy], 0, -Sin[phiy] },
	  {         0, 1,          0 },
	  { Sin[phiy], 0,  Cos[phiy] } }
	.
	{ { Cos[phiz], -Sin[phiz], 0 },
	  { Sin[phiz],  Cos[phiz], 0 },
	  {         0,          0, 1 } } );

trans = { transx, transy, transz };

track = { x0, y0, z0 } + A * { (x-x0), -y0, (z-z0) };

strAtInt = Solve[ ( rot . track + trans ) == { xpp, 0, zpp }, { A, x, z } ];

curvedInt = { xprime, yprime, zprime } /. strAtInt[[1]];

{ xppp, yppp, zppp } = rot . curvedInt + trans;

xresidual = xppp - xpp;
zresidual = zppp - zpp;

DumpSave[ "residuals_from_pp.mx", { xresidual, zresidual } ];

<< "residuals_from_pp.mx"

{ zlow = -0.075, zhigh = 0.075, zstep = ( zhigh - zlow ) / 6,
  xlow = -0.025, xhigh = 0.025, xstep = ( xhigh - xlow ) / 3 };

(* change the phix and they split, change the y and they turn together *)
xresPosZ = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.001000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000001,
      zpp -> 0.15/4 };
xresNegZ = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.001000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000001,
      zpp -> -0.15/4 };
Plot[ { xresPosZ, xresNegZ }, { xpp, xlow, xhigh },
      PlotRange -> { -0.000030, 0.000030 },
      PlotStyle -> { RGBColor[1,0,0], RGBColor[0,0,1] } ]

xresVsZ = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000010, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000001,
      xpp -> 0.01 };
Plot[ xresVsZ, { zpp, zlow, zhigh },
      PlotRange -> { -0.000030, 0.000030 },
      PlotStyle -> { RGBColor[1,0,0] } ]

(* these cover more *)

xresVsXZ = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000001 };
Plot3D[ xresVsXZ, { xpp, xlow, xhigh }, { zpp, zlow, zhigh },
	PlotRange -> { -0.000030, 0.000030 } ]

zresVsXZ = zresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.0000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000001 };
Plot3D[ zresVsXZ, { xpp, xlow, xhigh }, { zpp, zlow, zhigh },
	PlotRange -> { -0.000100, 0.000100 } ]

Exit
[Mathematica finished.]

(* ***************************************************************** *)

The no misalignments, just bowing:

xresVsXZ = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100 };
Plot3D[ xresVsXZ, { xpp, xlow, xhigh }, { zpp, zlow, zhigh },
	PlotRange -> { -0.000030, 0.000030 } ]

xresVsX1 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> 0 };
xresVsX2 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> 0.2 * zhigh };
xresVsX3 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> 0.4 * zhigh };
xresVsX4 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> 0.6 * zhigh };
xresVsX5 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> 0.8 * zhigh };
xresVsX6 = xresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> zhigh };
Plot[ { xresVsX1, xresVsX2, xresVsX3, xresVsX4, xresVsX5, xresVsX6 },
      { xpp, xlow, xhigh },
      PlotRange -> { -0.000030, 0.000030 } ]

zresVsX = zresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.0000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      zpp -> zhigh / 2 };
Plot[ zresVsX, { xpp, xlow, xhigh },
      PlotRange -> { -0.000100, 0.000100 } ]

zresVsZ = zresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.0000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100,
      xpp -> 0 };
Plot[ zresVsZ, { zpp, zlow, zhigh },
      PlotRange -> { -0.000100, 0.000100 } ]

zresVsXZ = zresidual /.
    { x0 -> 0, y0 -> 0.1, z0 -> 0,
      phix -> 0.000000, phiy -> 0.0000001, phiz -> 0.000000,
      transx -> 0.000000, transy -> 0.000000, transz -> 0.000000,
      l -> 0.15, s -> 0.000100 };
Plot3D[ zresVsXZ, { xpp, xlow, xhigh }, { zpp, zlow, zhigh },
	PlotRange -> { -0.000100, 0.000100 } ]

Exit
[Mathematica finished.]

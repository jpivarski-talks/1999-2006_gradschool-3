Off[General::spell];
$DisplayFunction = Display[ "tmp.gif", #, "GIF" ]&;

rot =
    { {          1,          0,          0 },
      {          0,  Cos[phix], -Sin[phix] },
      {          0,  Sin[phix],  Cos[phix] } }
    .
    { {  Cos[phiy],          0, -Sin[phiy] },
      {          0,          1,          0 },
      {  Sin[phiy],          0,  Cos[phiy] } }
    .
    { {  Cos[phiz], -Sin[phiz],          0 },
      {  Sin[phiz],  Cos[phiz],          0 },
      {          0,          0,          1 } };

realToCodeMatrix =
    { { rot[[1,1]], rot[[1,2]], rot[[1,3]], delx },
      { rot[[2,1]], rot[[2,2]], rot[[2,3]], dely },
      { rot[[3,1]], rot[[3,2]], rot[[3,3]], delz },
      {          0,          0,          0,    1 } };

codeToRealMatrix = Simplify[ Inverse[ realToCodeMatrix ] ];

realToCode[ { xr_, yr_, zr_ } ] :=
    Drop[ realToCodeMatrix . { xr, yr, zr, 1 }, -1 ];
codeToReal[ { xc_, yc_, zc_ } ] :=
    Drop[ codeToRealMatrix . { xc, yc, zc, 1 }, -1 ];

(* tools for plotting *)

PlotRphiVsX[ expr_ ] := 
    Plot[ { expr /. zmc -> -0.075,
	    expr /. zmc -> -0.050,
	    expr /. zmc -> -0.025,
	    expr /. zmc -> 0,
	    expr /. zmc -> 0.025,
	    expr /. zmc -> 0.050,
	    expr /. zmc -> 0.075,
	    0
	  }
	  , { xmc, -0.0125, 0.0125 }
	  , PlotRange -> { -0.000030, 0.000030 }
	  , Axes -> False
	  , Frame -> True
	  , AspectRatio -> 1
	  , FrameLabel -> { "x", "rphi resid",
			    "red = posZ, blue = negZ", None }
	  , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1], RGBColor[0,0,1],
			   RGBColor[0,0,0],
			   RGBColor[1,0,0], RGBColor[1,0,0], RGBColor[1,0,0],
			   { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
	];

PlotRphiVsZ[ expr_ ] := 
    Plot[ { expr /. xmc -> -0.0123,
	    expr /. xmc -> -0.0082,
	    expr /. xmc -> -0.0041,
	    expr /. xmc -> 0,
	    expr /. xmc -> 0.0041,
	    expr /. xmc -> 0.0082,
	    expr /. xmc -> 0.0123,
	    0
	  }
	  , { zmc, -0.075, 0.075 }
	  , PlotRange -> { -0.000030, 0.000030 }
	  , Axes -> False
	  , Frame -> True
	  , AspectRatio -> 1
	  , FrameLabel -> { "z", "rphi resid",
			    "red = posX, blue = negX", None }
	  , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1], RGBColor[0,0,1],
			   RGBColor[0,0,0],
			   RGBColor[1,0,0], RGBColor[1,0,0], RGBColor[1,0,0],
			   { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
	];

PlotZResVsX[ expr_ ] := 
    Plot[ { expr /. zmc -> -0.075,
	    expr /. zmc -> -0.050,
	    expr /. zmc -> -0.025,
	    expr /. zmc -> 0,
	    expr /. zmc -> 0.025,
	    expr /. zmc -> 0.050,
	    expr /. zmc -> 0.075,
	    0
	  }
	  , { xmc, -0.0125, 0.0125 }
	  , PlotRange -> { -0.001000, 0.001000 }
	  , Axes -> False
	  , Frame -> True
	  , AspectRatio -> 1
	  , FrameLabel -> { "x", "z resid",
			    "red = posZ, blue = negZ", None }
	  , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1], RGBColor[0,0,1],
			   RGBColor[0,0,0],
			   RGBColor[1,0,0], RGBColor[1,0,0], RGBColor[1,0,0],
			   { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
	];

PlotZResVsZ[ expr_ ] := 
    Plot[ { expr /. xmc -> -0.0123,
	    expr /. xmc -> -0.0082,
	    expr /. xmc -> -0.0041,
	    expr /. xmc -> 0,
	    expr /. xmc -> 0.0041,
	    expr /. xmc -> 0.0082,
	    expr /. xmc -> 0.0123,
	    0
	  }
	  , { zmc, -0.075, 0.075 }
	  , PlotRange -> { -0.001000, 0.001000 }
	  , Axes -> False
	  , Frame -> True
	  , AspectRatio -> 1
	  , FrameLabel -> { "z", "z resid",
			    "red = posX, blue = negX", None }
	  , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1], RGBColor[0,0,1],
			   RGBColor[0,0,0],
			   RGBColor[1,0,0], RGBColor[1,0,0], RGBColor[1,0,0],
			   { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
	];

(* Intersections with the real ladder, given fake ladder intersection *)

straightIntersection = 
    Solve[ { { xmr, ymr, zmr } == codeToReal[ { xmc, 0, zmc } ],
	     ( { xsr, 0, zsr } == { x0r, y0r, z0r } +
	       A * { xmr - x0r, ymr - y0r, zmr - z0r } )
	   }, { xsr, zsr, A } ][[1]];

xStraightResid = ( xmc - xsr ) /. straightIntersection;
zStraightResid = ( zmc - zsr ) /. straightIntersection;

(* My error last time with insensitive plots came from doing an extra
   transformation back to the code world frame. The residual is
   between the misaligned ladder in code world and the straight ladder
   in the real world! *)

PlotRphiVsX[ xStraightResid
	     /. { x0r -> 0, y0r -> 0.10, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0.001 } ]

PlotRphiVsX[ xStraightResid
	     /. { x0r -> 0, y0r -> 0.10, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0 } ]

PlotRphiVsZ[ xStraightResid
	     /. { x0r -> 0, y0r -> 0.10, z0r -> 0,
		  delx -> 0.000010, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0.000100, phiz -> 0 } ]

PlotZResVsZ[ zStraightResid
	     /. { x0r -> 0, y0r -> 0.10, z0r -> 0,
		  delx -> 0, dely -> 0.000010, delz -> 0.000050,
		  phix -> 0, phiy -> 0, phiz -> 0 } ]

                     (* All plots look good! *)

(* Intersections with a curved ladder, given fake ladder intersection *)

(* The following takes a long time to calculate (as you can well imagine) *)
bowedIntersection =
    Solve[ { { xmr, ymr, zmr } == codeToReal[ { xmc, 0, zmc } ],
	     ( { xbr, ybr, zbr } == { x0r, y0r, z0r } +
	       A * { xmr - x0r, ymr - y0r, zmr - z0r } ),
	     ( ( ybr + (w^2-4*s^2)/(8*s) )^2 + xbr^2 ==
	       ( (w^2+4*s^2)/(8*s) )^2 )
	   }, { xbr, ybr, zbr, A } ][[2]];

xBowedResid = ( xmc - xbr ) /. bowedIntersection;
zBowedResid = ( zmc - zbr ) /. bowedIntersection;

DumpSave[ "xz_bowed_resids.mx", { xBowedResid, zBowedResid } ];

<< "xz_bowed_resids.mx";

$DisplayFunction = Display[ "xres_v_x_bow0.gif", #, "GIF" ]&;
PlotRphiVsX[ xBowedResid
	     /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0,
		  w -> 0.025, s -> -0.000120 } ]

$DisplayFunction = Display[ "xres_v_x_bow0_layer4.gif", #, "GIF" ]&;
PlotRphiVsX[ xBowedResid
	     /. { x0r -> 0, y0r -> 0.102, z0r -> 0,   (* layer 4 *)
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0,
		  w -> 0.025, s -> -0.000120 } ]

$DisplayFunction = Display[ "xres_v_z_bow0.gif", #, "GIF" ]&;
PlotRphiVsZ[ xBowedResid
	     /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0,
		  w -> 0.025, s -> -0.000120 } ]

$DisplayFunction = Display[ "zres_v_x_bow0.gif", #, "GIF" ]&;
PlotZResVsX[ zBowedResid
	     /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0,
		  w -> 0.025, s -> -0.000120 } ]

$DisplayFunction = Display[ "zres_v_z_bow0.gif", #, "GIF" ]&;
PlotZResVsZ[ zBowedResid
	     /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
		  delx -> 0, dely -> 0, delz -> 0,
		  phix -> 0, phiy -> 0, phiz -> 0,
		  w -> 0.025, s -> -0.000120 } ]

$DisplayFunction = Display[ "zres_v_both_bow0.gif", #, "GIF" ]&;
tmp = zBowedResid /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
		       delx -> 0, dely -> 0, delz -> 0,
		       phix -> 0, phiy -> 0, phiz -> 0,
		       w -> 0.025, s -> -0.000120 };
Plot3D[ tmp
	, { xmc, -0.0125, 0.0125 }, { zmc, -0.075, 0.075 }
	, AxesLabel -> { "x", "z", "zres" }
	, ImageSize -> 100 * 7
      ]

(* Now for the dependance on misalignment *)

(* xres vs x vary y *)

tmp = xBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,   (* layer 4 *)
	 delx -> 0, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 zmc -> 0
       };
Plot[ { tmp /. dely -> -0.000100,
	tmp /. dely -> -0.000050,
	tmp /. dely -> -0,
	tmp /. dely -> 0.000050,
	tmp /. dely -> 0.000100,
	0
      }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.000030, 0.000030 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "rphi resid",
			"vary y (blue -100 -50 0 50 100 red)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* xres vs x vary phix *)

tmp = xBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,   (* layer 4 *)
	 delx -> 0, dely -> 0, delz -> 0,
	 phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 zmc -> -0.075/2
       };
Plot[ { tmp /. phix -> -0.001000,
	tmp /. phix -> -0.000500,
	tmp /. phix -> -0,
	tmp /. phix -> 0.000500,
	tmp /. phix -> 0.001000,
	0
      }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.000030, 0.000030 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "rphi resid",
			"vary phix (blue -1000 -500 0 500 1000 red) (neg Z)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* xres vs x vary phiz *)

tmp = xBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,   (* layer 4 *)
	 delx -> 0, dely -> 0, delz -> 0,
	 phix -> 0, phiy -> 0,
	 w -> 0.025, s -> -0.000120,
	 zmc -> -0.075/2
       };
Plot[ { tmp /. phiz -> -0.005000,
	tmp /. phiz -> -0.002500,
	tmp /. phiz -> -0,
	tmp /. phiz -> 0.002500,
	tmp /. phiz -> 0.005000,
	0
      }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.000030, 0.000030 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , ImageSize -> 100 * 4
      , FrameLabel -> { "x", "rphi resid",
			"vary phiz (blue -5000 -2500 0 2500 5000 red) (neg Z)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* xres vs z vary phiy *)

tmp = xBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
	 delx -> 0, dely -> 0, delz -> 0,
	 phix -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 xmc -> 0.0125/2
       };
Plot[ { tmp /. phiy -> -0.000100,
	tmp /. phiy -> -0.000050,
	tmp /. phiy -> -0,
	tmp /. phiy -> 0.000050,
	tmp /. phiy -> 0.000100,
	0
      }
      , { zmc, -0.075, 0.075 }
      , PlotRange -> { -0.000030, 0.000030 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "z", "rphi resid",
			"vary phiy (blue -100 -50 0 50 100 red) (pos X)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* xres vs z vary x *)

tmp = xBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,   (* layer 4 *)
	 dely -> 0, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 xmc -> -0.0125/2
       };
Plot[ { tmp /. delx -> -0.000010,
	tmp /. delx -> -0.000005,
	tmp /. delx -> -0,
	tmp /. delx -> 0.000005,
	tmp /. delx -> 0.000010,
	0
      }
      , { zmc, -0.075, 0.075 }
      , PlotRange -> { -0.000030, 0.000030 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "z", "rphi resid",
			"vary x (blue -10 -5 0 5 10 red) (neg X)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* zres vs x vary y *)

tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
	 delx -> 0, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 zmc -> 0.075/2
       };
Plot[ { tmp /. dely -> -0.000100,
	tmp /. dely -> -0.000050,
	tmp /. dely -> -0,
	tmp /. dely -> 0.000050,
	tmp /. dely -> 0.000100,
	0
      }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",
			"vary y (blue -100 -50 0 50 100 red) (pos Z)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

(* zres vs z vary y *)

tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,
	 delx -> 0, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> -0.000120,
	 xmc -> 0
       };
Plot[ { tmp /. dely -> -0.000100,
	tmp /. dely -> -0.000050,
	tmp /. dely -> -0,
	tmp /. dely -> 0.000050,
	tmp /. dely -> 0.000100,
	0
      }
      , { zmc, -0.075, 0.075 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "z", "z resid",
			"vary y (blue -100 -50 0 50 100 red)",
			None }
      , PlotStyle -> { RGBColor[0,0,1], RGBColor[0,0,1],
		       RGBColor[0,0,0],
		       RGBColor[1,0,0], RGBColor[1,0,0],
		       { RGBColor[0,0,0], Dashing[{0.02,0.02}] } }
    ];

Exit
[Mathematica finished.]

(* ***************************************************************************** *)

$DisplayFunction =
    ( Display[ "tmp.gif", #, "GIF" ];
       Run[ "kill -HUP `ps -fu mccann | grep psrender | grep -v grep | awk '{print $2}'`" ];
      # )&;

$DisplayFunction = Display[ "tracking_meeting/layer1_right4.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.04375
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -4.375", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_right3.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.03125
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.03125", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_right2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.01875
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.01875", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_right1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.00625
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.00625", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_left1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.00625
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.00625", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_left2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.01875
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.01875", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_left3.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.03125
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.03125", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer1_left4.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.024, z0r -> 0,    (* layer 1 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.04375
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 4.375", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

(* layer 2 *)

$DisplayFunction = Display[ "tracking_meeting/layer2_right2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.0367, z0r -> 0,    (* layer 2 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.075
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.075", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer2_right1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.0367, z0r -> 0,    (* layer 2 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.025
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.025", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer2_left1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.0367, z0r -> 0,    (* layer 2 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.025
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.025", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer2_left2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0.007, y0r -> 0.0367, z0r -> 0,    (* layer 2 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.075
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.075", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

(* layer 3 *)

$DisplayFunction = Display[ "tracking_meeting/layer3_right2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.07, z0r -> 0,    (* layer 3 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.10
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 0.6
      , FrameLabel -> { "x", "z resid",	"z = -0.10", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer3_right1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.07, z0r -> 0,    (* layer 3 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.05
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 0.6
      , FrameLabel -> { "x", "z resid",	"z = -0.05", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer3_middle.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.07, z0r -> 0,    (* layer 3 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 0.6
      , FrameLabel -> { "x", "z resid",	"z = 0", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer3_left1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.07, z0r -> 0,    (* layer 3 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.05
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 0.6
      , FrameLabel -> { "x", "z resid",	"z = 0.05", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer3_left2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.07, z0r -> 0,    (* layer 3 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.10
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 0.6
      , FrameLabel -> { "x", "z resid",	"z = 0.10", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

(* layer 4 *)

$DisplayFunction = Display[ "tracking_meeting/layer4_left3.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.125
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.125", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer4_left2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.075
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.075", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer4_left1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> 0.025
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = 0.025", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer4_right1.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.025
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.025", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer4_right2.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.075
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.075", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

$DisplayFunction = Display[ "tracking_meeting/layer4_right3.eps", #, "EPS" ]&;
tmp = zBowedResid
    /. { x0r -> 0, y0r -> 0.102, z0r -> 0,    (* layer 4 *)
	 delx -> 0, dely -> -0.000150, delz -> 0,
	 phix -> 0, phiy -> 0, phiz -> 0,
	 w -> 0.025, s -> 0.000150,
	 zmc -> -0.125
       };
Plot[ { tmp, 0 }
      , { xmc, -0.0125, 0.0125 }
      , PlotRange -> { -0.001000, 0.001000 }
      , Axes -> False
      , Frame -> True
      , AspectRatio -> 1
      , FrameLabel -> { "x", "z resid",	"z = -0.125", None }
      , PlotStyle -> { Dashing[{}], Dashing[{0.03,0.03}] }
    ];

Exit
[Mathematica finished.]

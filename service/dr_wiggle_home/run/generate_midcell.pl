#!/usr/local/bin/perl

$n = 1;
foreach $runpair (
		  [126207, 126260],
		  [126262, 126282],
		  [126283, 126302],
		  [126304, 126317],
		  [126318, 126347],
		  [126349, 126364],
		  [126367, 126382],
		  [126386, 126429],
		  [126431, 126449],
		  [126451, 126465],
		  [126467, 126480],
		  [126483, 126503],
		  [126505, 126517],
		  [126520, 126536],
		  [126538, 126555],
		  [126556, 126573],
		  [126575, 126615],
		  [126618, 126639],
		  [126640, 126653],
		  [126656, 126670],
		  [126673, 126689],
		  [126690, 126706],
		  [126707, 126721],
		  [126722, 126736],
		  [126737, 126755],
		  [126756, 126779],
		  [126780, 126822],
		  [126823, 126837],
		  [126838, 126856],
		  [126858, 126874],
		  [126875, 126893],
		  [126894, 126914],
		  [126915, 126934],
		  [126935, 126952],
		  [126956, 126999],
		  [127000, 127014],
		  [127016, 127032],
		  [127033, 127059],
		  [127060, 127072],
		  [127073, 127105],
		  [127107, 127120],
		  [127123, 127136],
		  [127138, 127176],
		  [127180, 127195],
		  [127196, 127214],
		  [127215, 127230],
		  [127231, 127247],
		  [127249, 127264],
		  [127266, 127279],
		  [127280, 127295],
		  [127296, 127315],
		  [127317, 127330],
		  [127378, 127423],
		  [127424, 127461],
		  [127462, 127479],
		  [127480, 127501],
		  [127502, 127513],
		  [127516, 127534],
		  [127535, 127560],
		  ) {
    ($runstart, $runend) = @{ $runpair };
    $on = sprintf("%02d", $n);

    open( RUN, "> generate_midcell$on.run" );
    print RUN "#!/usr/local/bin/tcsh

cd \$HOME/dr_wiggle/run
. /nfs/cleo3/scripts/cleo3defs
c3rel Nov04_02_P2

USER_N=\"$on\"
export USER_N
USER_RUNSTART=\"$runstart\"
export USER_RUNSTART
USER_RUNEND=\"$runend\"
export USER_RUNEND

suez -f generate_midcell.tcl > \$USER_TMP/generate_midcell$on.log
";
    close RUN;

    $n++;
}

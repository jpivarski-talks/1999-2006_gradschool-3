#!/usr/local/bin/perl

foreach $type ( "seedtrack" ) { # "standard", "midcell", "stereoonly", "stereoonly_midcell" ) {
    foreach $n ( 1..30 ) { # 18 ) {
	$on = sprintf("%02d", $n);
	
	open( RUN, "> test_$type$on.run" );
	print RUN "#\$ -l calibration
#\$ -o /cdat/tem/mccann/test_$type$on.out
#\$ -S /bin/bash
#\$ -j y

cd /home/mccann/dr_wiggle/run
. /nfs/cleo3/scripts/cleo3defs
c3rel Jul13_02_P2

USER_FILE=\"$type$on\"
export USER_FILE

suez -f test.tcl > /cdat/tem/mccann/test_$type$on.log
";
        close RUN;
    }
}

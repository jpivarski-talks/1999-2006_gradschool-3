#!/usr/local/bin/perl

foreach $run (200700, 200709, 200712, 200716, 200717, 200720, 200723,
	      200727, 200729, 200743, 200744, 200745, 200853, 200854,
	      200981, 200983, 200986, 200993, 201001, 201003, 201018,
	      201027, 201028, 201031, 201041, 201042, 201043, 201074,
	      201075, 201076, 201077, 201079, 201087, 201089, 201091,
	      201095, 201096, 201109, 201111, 201113, 201116, 201117,
	      201119, 201120, 201133, 201136, 201168, 201214, 201215,
	      201219, 201228, 201236, 201321, 201324) {

    open(RUN, "> collect_tracks_$run.run");
    print RUN "#\$ -l calibration
#\$ -o /cdat/tem/mccann/dr_tracks_$run.log
#\$ -S /bin/bash
#\$ -j y

cd /home/mccann/wcalign/run
. /nfs/cleo3/scripts/cleo3defs
c3rel Oct15_03_MC

export USER_RUN=$run

suez -f collect_tracks.tcl
";
    close(RUN);
}

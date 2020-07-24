#!/usr/local/bin/perl

foreach $run (200552, 200554, 200556, 200557, 200558, 200708, 200709,
	      200712, 200713, 200714, 200716, 200717, 200720, 200723,
	      200725, 200729, 200731, 200732, 200739, 200741, 200742,
	      200743, 200744, 200745, 200746, 200747, 200797, 200801,
	      200802, 200805, 200806, 200814, 200815, 200817, 200818,
	      200819, 200820, 200824, 200832, 200833, 200834, 200835,
	      200836, 200841, 200842, 200845, 200846, 200847, 200850,
	      200851, 200853, 200854) {

    open(RUN, "> zdalign_$run.run");
    print RUN "#$ -l calibration
#$ -o /cdat/tem/mccann/zdalign_$run.log
#$ -S /bin/bash
#$ -j y

cd /home/mccann/zdalign/run
. /nfs/cleo3/scripts/cleo3defs
c3rel Oct15_03_MC

export USER_RUN=$run
suez -f zdalign.tcl
";
    close(RUN);
}

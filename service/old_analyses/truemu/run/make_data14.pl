#!/usr/local/bin/perl

open( SUBMIT, ">> submit.run" );

@jobs = ( 119493, 119508, 119519, 119530, 119540, 119550, 119564, 119574, 119588, 119599, 119618, 119630, 119644, 119659, 119673, 119691, 119703, 119723, 119738, 119755, 119768, 119786, 119802, 119818, 119832, 119849, 119863, 119876, 119891, 119910, 119922 );

for ( $i = 0;  $i < $#jobs;  $i++ )
{
    $run = $jobs[$i];
    $nextrun = ( $jobs[$i+1] - 1 );

    print "Making script for run $run...\n";
    open( SCRIPT, "> mtm$run.run" );
    print SCRIPT "#!/usr/local/bin/tcsh

cd /home/mccann/run/MakeTrueMu
. /nfs/cleo3/scripts/cleo3defs
c3rel cleo3_Pass2_May29_2001

OSAGENT_PORT=31416
export OSAGENT_PORT
C3_CONST=/nfs/c3con/ccon3/constants/Codi
export C3_CONST
echo Using OSAGENT_PORT \$OSAGENT_PORT and constants server \$C3_CONST > \$USER_DAF/truemu/logs/mtm$run.log
which suez >> \$USER_DAF/truemu/logs/mtm$run.log

suez -q >> \$USER_DAF/truemu/logs/mtm$run.log << EOF

default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file \\\$env(USER_DAF)/truemu/plots/mtm$run.rzn
hbook init

run_file \\\$env(C3_SCRIPTS)/getConstants.tcl
run_file \\\$env(C3_SCRIPTS)/Ccfc.tcl

# Load Raw data (for CC hits)
prod sel RawDataProd

# Load the magnetic field
prod sel MagFieldProd

# Set up NavTrack
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd
prod sel SeedTrackToFit

# Get event properties object to quickly find mupairs
prod sel EventPropertiesProd

proc sel \\\$env(USER_BUILD)/shlib/MakeTrueMu

module sel DBModule
database in /nfs/cleo3/database/data14/db beginrun startrun event endrun
source activate db beginrun startrun event endrun
database read_collection pass2 mupair
database run $run $nextrun

report level DEBUG
go 100
report level INFO
go
exit

EOF
";
    close( SCRIPT );

    print SUBMIT "echo \"submitting mtm$run.run...\"\nqsub -V -l calibration -cell rcf mtm$run.run\nsleep 60\n";
}
close( SUBMIT );
print "done!\n";

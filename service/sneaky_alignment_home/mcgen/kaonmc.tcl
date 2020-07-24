exception continueEventLoop on

run_file $env(C3_SCRIPTS)/cleog_command.tcl
cleog gen BBbarGeneratorProd 5000 out ../pds/cleog8a.pds dataset 8 -post {proc sel ../build_OSF1/shlib/RequireKaon.so}

run_file $env(C3_SCRIPTS)/mcpass2_command.tcl
mcpass2 file ../pds/cleog8a.pds out ../pds/mcpass28a.pds

# Wed Jan 29 15:52:27 EST 2003, Valentine Kouznetsov
# Example of NEW DB API testing
# Require:
#   ConstantsModule_g (debug version, otherwise change below to ConstantsModule)
#   TestConstants_g processor, otherwise write your own.
#
# turn on debugging flag
report level debug

# add some producers
prod sel DBRunHeaderProd
prod sel DBEventHeaderProd

# Load constant into suez
if { ! [info exists getPhase2Constants_tcl_loaded] } {
    # keep script from multiply loading
    set getPhase2Constants_tcl_loaded 1

    if [ catch {
	
	global env  # make env available in nested calls

        # to make it run on OSF I need to redefine MICO environment here
        set env(MICONSDFILE) /home/hubert/mico/MicoIOR
        
        # database and metafiles -------
	module sel CorbaModule
	corba init
	module sel ConstantsModule_g

        # according to SunOS a new bootPath is /nfs/c3con/ccon3/test1/db/Constants
        set env(C3_CONST) /nfs/c3con/ccon3/test1/db/Constants

        # Get constants from the DB
	constants in $env(C3_CONST) standard CLEO3default streams svalignment basegeometry

    } resultString ] { # ------------------- tcl try-catch block
	puts stdout "loading failed for constants database:"
	puts stdout "resultString:"
	puts stderr "$resultString"

        # throw exception so if this script is included by another script, the
        #  parent script can see that there was an exception
	error "Phase2 constants loading problem: $resultString"
    } else {
	puts stdout "Constants database setup successfully"
    }
}

# include own Processer/Producers
proc sel shlib/TestConstants_g.so

# create an empty source which will cover different run ranges
emptysource many vk run 121630 20 beginrun

# print out available sources
source ls


#! /usr/local/bin/tcsh 

setenv PASS2_SCRIPTS /home/pass2/Pass2Production/pass2/Pass2ReproScripts/DATA20/pass2scripts
setenv OSAGENT_ADDR solcon.lns.cornell.edu
setenv OUTPUT_COLLECTION pass2
setenv DATABASE /nfs/objy/data20/db
setenv SUB_COLLECTION pass2
setenv RUN_NUMBER $1
setenv OB_BASEPATH /home/pass2/Pass2Production/pass2/Pass2ReproScripts/DATA20
setenv WORKINGDIR /home/pass2/Pass2Production/pass2/Pass2ReproScripts/DATA20
. /nfs/cleo3/Offline/scripts/cleo3defs
c3rel Jul13_02_P2
export C3_SHLIB=$C3_LIB/shlib/${OS_NAME}/${CXX}

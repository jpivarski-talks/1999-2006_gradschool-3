*
* $Id: tfxfit.F,v 1.23 2002/05/23 14:22:39 dpp Exp $
*
* $Log: tfxfit.F,v $
* Revision 1.23  2002/05/23 14:22:39  dpp
*      -> use the radius limit when selecting hits
*
* Revision 1.22  2001/11/20 17:56:49  dpp
*   -> use new variable SCALNW for wide road selection of non group layers
*
* Revision 1.21  2001/11/19 23:50:56  dpp
*      -> replace use of logical in common with argument
*      -> delete unused include file
*      -> add new control parameter include file
*      -> now using extrapolation error for stereo layers in LF
*      -> increase number of sigmas for adding stereo LF hits
*      -> smaller window for LF silicon in STANDARD
*         because Z0 constrained SZ fit has smaller errors in error matrix
*     **> argument to use wider road for stereo layers in standard LF
*         when called from boot_si_z_hits
*     **> use extrapolation error for stereo layers in LF
*
* Revision 1.20  2001/09/12 22:02:52  dpp
*      -> deweight hits added in STANDARD that have large entrance angle
*      -> format statement
*
* Revision 1.19  2001/05/03 19:21:47  dpp
* fix bug introduced May 1, it had killed all Z hits
*
* Revision 1.18  2001/05/01 17:26:16  dpp
*      -> diagnostic info
*      -> F77 in first loop
*      -> fixed error in local ambiguity TFXFIT
*         error: status=0 hit caused susequent hits in layer to be ignored
*      -> use  efficiency dependent scaling  LOC AMB TFXFIT
*
* Revision 1.17  2001/04/19 17:21:11  dpp
*     -> diagnostic info
*      -> test logical before applying efficiency dependent scaling in STANDARD
*
* Revision 1.16  2001/03/30 00:05:07  dpp
*      -> replace SIGLCF and SIGLLF (which had both been =2) with HEFSCL
*      -> calculate total efficiency of the layers
*      -> slide the scaling of resolution used in finding STANDARD road
*
* Revision 1.15  2000/11/15 16:58:18  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.14  2000/10/24 22:44:13  dpp
*      -> set program indicator with parameter
*      -> change program indicator for reduced drift distance hits
*
* Revision 1.13  2000/10/02 20:43:30  dpp
*      -> diagnostics
*      -> change to chain number found in STATCF(1)
*      -> create link list for SAMEAS groups with more than 2 entries
*      -> various modifications to allow double hits without double entries
*      -> set PROGRAM_INDICATOR
*      -> z test on hits in standard
*      -> change energy loss curvature threshold from 1.1 to 1.5
*      -> require valid hit status
*      -> deweight reduced drift distance hits
*
* Revision 1.12  2000/08/28 17:41:34  dpp
*      -> fixed error in logic in determining best group created
*         when logic was changed from "go to" to "setting logical"
*
* Revision 1.11  2000/08/18 18:50:26  dpp
*      -> remove kabg argument in call to TFEXER
*      -> change scattering road calculation to use DOIT scattering definitions
*      -> use a more sensible scattering amount
*      -> error handling for PARXTR,INVERS; argument for TFEXER
*      -> kink for enery loss
*
* Revision 1.10  2000/05/26 16:48:59  dpp
*      -> expanded tracer info
*      -> change some cuts from (fraction of cell size) to (absolute)
*
* Revision 1.9  2000/05/17 19:47:21  dpp
*      -> comments
*
* Revision 1.8  2000/04/26 14:59:54  dpp
*      -> remove argument FITTYP,get fit type from SAV_FIT_TYP set in ROADHITS
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use SAV_FIT_TYP as index rather than branching with TFXS
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> use call to RESIDUALS instead of RESICF, also replace z res calc
*      -> remove some adf4 cleo2 specific silicon treatment
*      -> replace old bit string diagnostics with calls to trace_tfxfit
*
* Revision 1.7  2000/01/06 22:07:34  dpp
* use SIGMA_LAYER instead of obsolete SIGFTF
*
* Revision 1.6  1999/12/09 21:14:31  dpp
* change call to hit_to_fit to use parameter directive
* zero the third argument to HIT_TO_FIT(H2F_NEW_ENTRY...
*
* Revision 1.5  1999/09/12 22:28:58  dpp
* remove argument from CFRFT4; calls HIT_TO_FIT;
* use SZ_FITTER instead of LFLFIT
*
* Revision 1.4  1999/05/28 03:04:56  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.3  1999/04/27 20:16:56  dpp
* extensive F77 changes, remove PT associated hit stuff
*
* Revision 1.2  1998/07/09 00:48:00  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson 
      SUBROUTINE TFXFIT(WIDE,IERR)
C.......................................................................
C.
C. TFXFIT - Pick up extra hits close to track and refit
C.
C. COMMON    : ?
C. CALLS     : FIT_WHATEVER, TFEXER
C. CALLED    : ?
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 22-Jul-87
C.
C.
C. Modification Log.
C. 12-Feb-97...Change the way layer 3 outer r-phi SV hits are handled.
C. 08-July-96..extrapolation of hits into silicon using error matrix
C.           ..allow multiple hits at corners
C. 5-Mar-96....use TFEXER for track extrapolation error
C. 06-Feb-96...allow gaps in strings
C. 24-Jan-96...calls CFFT4D and uses kink to calculate residuals
C.          ...change argument list to include kink, kink radius and FITTYP
C.               (FITTYP later removed;fit type is detemined by ROADHITS call)
C.          ...change argument in call to TFPHTR to include kink
C. 4-Jan-96....use of kinks in fit
C. 18-Dec-95...changes to TFXFIT hit selection, updates of TFXFIT graphics
C. 25-Sep-95...first version compatible with LF hit finding
C. 02-Mar-95...flag changes so that DUET display would work.
C. 24-Mar-94...utilize PT live wire map, PTLIV1/2
C. 25-Oct-93...move parameters, group definitions and cuts to TFXFITCD
C.             and make all the definitions and cuts double value (cf and lf)
C. 11-Oct-93...merge in the changes from Anders to use with silicon
C. 30-Aug-93...again revise time residual sum in correlation,good very bad fits
C. 27-Aug-93...fixed index error in loop to find group/layer number
C.          ...fixed error involving premature cut on BSNHMN
C.          ...removed extra CFCFIT after PT hit deletion when MSCATF.GT.1
C.          ...set SAMEAS for LUSETF hits that have zero drift distance
C.          ...revise time residual sum in correlation,good for zero time hits
C. 23-Aug-93...do not fill LUSETF layers in standard TFXFIT if LCFXLA=.F.
C.             this was done for constants
C. 16-June-93..narrow road for standard tfxfit
C.             put cutting values in parameter statements
C.             different beginning pass numbers for VD/PT and DR
C. 25-May-93...implement local ambiguity in DR, as well as PT,VD
C.  1-May-93...use logicals to turn on local ambiguity and hit deletion
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C=======================================================================
C TFXFIT gets extra hits (if any) from /TFHITS/, lying close to the
C track candidate # ITRK. All points are fitted, NFIT and the arrays
C with it are updated.
C=======================================================================

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/tfxfitcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/runev.inc"

#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C PARAMETERS
C THESE ARE IN SEQUENCE, TFXFITCD

C..NGRUP..... NUMBER OF AMBIGUITY GROUPS
C..MGLAYR.... MAXIMUM NUMBER OF LAYERS IN A GROUP
C..MGSTOR.... MAXIMUM NUMBER OF HITS IN A LAYER
C..TOTLGR.... PRODUCT GROUP*LAYER
C..TOTHGR.... PRODUCT GROUP*LAYER*HIT
C..NFTMIN.... MINIMUM OF NFIT TO ALLOW CALL TO CFCFIT, OTHERWISE RETURN ERROR

C GROUP DEFINITION AND CUTS
C THESE ARE IN SEQUENCE, TFXFITCD

C..LLAYGR.... FIRST LOCAL TFXFIT LAYER, XLYR,  IN A GROUP
C..NLAYGR.... NUMBER OF TF LAYERS IN A GROUP
C..PASS1G..A. FIRST PASS TO ALLOW USE OF THIS GROUP
C..PASSAG..A. PASS NUMBER TO BEGIN ALWAYS ALLOWING USE OF THIS GROUP
C..SKIPPS.... SKIP PASS; ok to skip this pass if previous had IGNORD=.FALSE.
C..MPASSA..A. MAXIMUM NUMBER OF PASSES THROUGH LOCAL AMBIGUITY
C..MPASSS..S. MAXIMUM NUMBER OF PASSES THROUGH STANDARD TFXFIT
C..MPASSL..A. MAXIMUM NUMBER OF PASSES selecting other groups
C             with residual difference relative to best
C..HEFSCL..S. HIGH EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..ULEFSC.... LOGICAL: USE LOW EFFICIENCY SCALING, otherwise default to HEFSCL 
C..LEFSCL..S. LOW EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..LCMPEF..S. LOW EFF SCALE USED BELOW THIS LOW COMPLIMENTARY EFFICIENCY
C..HCMPEF..S. HIGH EFF SCALE USED ABOVE THIS HIGH COMPLIMENTARY EFFICIENCY
C..STANRD..S. STANDARD tfxfit narrow absolute ROAD, fraction of CHISQ cut
C..ILAMRD..A. INITIAL LOCAL AMBIGUITY ROAD, absolute
C..LDRFRD..A. LARGE DRIFTDISTANCE ROAD,  cut, fraction of a full cell
C..DEWRDR..A. DEWEIGHT REDUCE DRIFT DISTANCE
C..ISHFRD..A. ISOLATED HIT ROAD when no previous hit(was LUSETF=.FALSE.) 
C             two hits are isolated if separated by this absolute distance
C..ISHTRD..A. ISOLATED HIT ROAD,as ISHFRD but previous hit,(was LUSETF=.TRUE.) 
C..SAMHRD..A. SAME HIT ROAD,  within which two hits are essentially identical
C..STZERD..S. STANDARD TFXFIT Z at Endplate absolute ROAD
C..SMLDIF..S. SMALL ABS RESIDUAL DIF;below which is confusing which hit to take
C..LRGDIF..S. LARGE RESIDUAL ABS DIF;above which, do not assign confusing hits
C..MAXGAP..A. MAXIMUM SINGLE GAP allowed in a string
C..TOTGAP..A. MAXIMUM TOTAL of all GAPS allowed in a string 
C..CHRDHE..A. CORRELATED HITS ROAD, HIGH PRODUCT EFFICIENCY, loc amb, absolute
C..CHRDLE..A. CORRELATED HITS ROAD, LOW PRODUCT EFFICIENCY, loc amb, absolute
C..HPRDEF..A. HIGH PRODUCT EFFICIENCY, above which use CHRDHE
C..LPRDEF..A. LOW PRODUCT EFFICIENCY, below which use CHRDLE
C..BSRSRD..A. BEST STRING RESIDUAL ROAD (for each pass)
C..BSNHMN..A. BEST STRING, NUMBER of HITS MINIMUM, now a variable
C..SRDFL0..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 0
C..SRDFL1..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 1
C..OSRDRD..A. OTHER STRING RESIDUAL DIFFERENCE ROAD
C..IGSSLN..A. IGNORED STRING SIGNIFICANT LENGTH; to set IGNORE flag
C..WSRDRD..A. WITHIN STRING RESIDUAL DIFFERENCE ROAD; resid relative to average
C..MULTIH..A. MULTIPLE HITS; add more than one hit per layer only in LOC AMB

C GROUP STATUS
C THESE ARE IN SEQUENCE, TFXFITCD

C..NHITGR ... NUMBER OF HITS IN THIS GROUP/LAYER
C..ILLYRG.... SAVED DUET LOGICAL LAYER OF THIS GROUP/LAYER
C..FTLUSE.... IH NUMBER USED IN FIT FOR LUSETF LAYER IN THIS GROUP/LAYER
C..SRCUSE.... PROGRAM_INDICATOR FOR HIT USED IN FIR FOR LUSETF LAYER
C..IHITGR.... SAVED IH (NOT IADRHT(IH)) FOR ELEMENT IN GROUP/LAYER/ELEMENT
C..LFESGR.... LAYER FOR FIRST ELEMENT OF STRING FOR GROUP/LAYER/ELEMENT
C..EFESGR.... ELEMENT FOR FIRST ELEMENT OF STRING FOR GROUP/LAYER/ELEMENT
C..NXLRGR.... NEXT LAYER FOR SAME GROUP/SAME LAYER/THIS ELEMEN
C..NXHTGR.... NEXT ELEMENT(IN NEXT LAYER) FOR SAME GROUP/SAME LAYER/THIS ELEMEN
C..LNHTGR.... NUMBER OF ELEMENTS(LENGTH) FOR THIS STARTING GROUP/FIRST LAYER/EL
C..NGAPGR.... NUMBER OF MISSING LAYERS FOR THIS STARTING GROUP/FIRST LAYER/EL
C..MISHTG.... SIGNED RESIDUAL FOR THIS ELEMENT
C..MISSTG.... SIGNED RESIDUAL FOR THIS STRING OF ELEMENTS
C..REDUCD.... 1     REDUCED DRIFT DISTANCE FOR THIS ELEMENT
C..SAMEAS.... POINTS TO OTHERH IN SAME GROUP,LAYER THAT'S ESSENTIALLY IDENTICAL
C..ISOHGR.... ISOLATION: BITS ARE INDEPENDENT, THIS IS RESET AT EACH ITERATION
C        .... 1     ISOLATED  TO THE - SIDE
C        .... 2     ISOLATED  TO THE + SIDE
C        .... 3     BOTH
C        .... 0     NEITHER
C        .... 4     IF IT IS OPEN TO USE AS EARLY PART OF STRINGS
C        .... 8     IF IT IS OPEN TO USE AS LATER PART OF STRINGS
C .. 16, SET, NOT USED, IF OK TO ADD TO FIT LIST, BASED ON ERROR WITHIN GROUP
C        .... 32    NOT LUSETF LAYER ,OR AT LEAST NOT A WRONG_LUSETF_LAYER_HIT
C..LYRUSE.... INTERNAL USE FLAG FOR LAYER, NOT RESET BETWEEN ITERATIONS
C        .... 0     NOT USED..YET; AVAILABLE FOR HIT SELECTION
C        .....1     found filled at entry
C        .... 2     DO NOT USE LAYER; HIT HAS BAD RESOLUTION COMPARED TO GROUP
C        .... 3     DO NOT USE LAYER; GROUP HAS A WRONG_LUSETF_LAYER_HIT
C        .... 4     FILLED BY REGULAR TFXFIT
C        .... 5     FILLED BY LOCAL AMBIGUITY TFXFIT
C..HITUSE.... INTERNAL USE FLAG FOR HIT, NOT RESET BETWEEN ITERATIONS
C        .... 0     NOT USED..YET; AVAILABLE FOR HIT SELECTION
C        .....1     found filled at entry, or same wire found fille at entry
C        .... 2     DO NOT USE HIT; HIT HAS BAD RESOLUTION COMPARED TO GROUP
C        .... 3     DO NOT USE HIT; GROUP HAS A WRONG_LUSETF_LAYER_HIT
C        .... 4     FILLED BY REGULAR TFXFIT
C        .... 5     FILLED BY LOCAL AMBIGUITY TFXFIT
C..GRPSTA.... GROUP STATUS,  "BEST" OR "KEEP" OR "JUNK"
C        .... -1    GROUP IS JUNK
C        .... 0     UNKNOWN
C        .... 1     GROUP NOT USED
C        .... 2     GROUP ADDED TO HIT LIST
C        .... 3     THIS IS THE SO CALLED BEST GROUP
C..HITSTA.... HIT STATUS
C        .... 0     NOT ADDED FOR UNKNOWN REASON
C        .... 1     NOT ADDED because it is blocked by LYRUSE
C        .... 2     NOT ADDED BECAUSE THE RESIDUAL IS BAD
C        .... 3     NOT ADDED BECAUSE GROUP CONTAINS A WRONG_LUSETF_LAYER_HIT
C        .... 4     IN FIT ALREADY, BUT IT IS A WRONG_LUSETF_LAYER_HIT
C        .... 5     IN FIT ALREADY, AND IT IS RIGHT LUSETF LAYER HIT
C        .... 6     ADDED

#if defined(CLEO_TFXFDIAG)
      INTEGER EVTNUM,RUNNUM,NCALL
      INTEGER EVTOLD/-1/
      INTEGER RUNOLD/-1/
      REAL    RDUM
      INTEGER SEDNUM
      INTEGER SEDOLD/-1/
      INTEGER JBYT
      EXTERNAL JBYT
#endif
C Argument
C..WIDE...... CONTROL OF USE OF WIDER RAOD IN STANDARD
      INTEGER WIDE
C..IERR...... RETURNED ERROR
      INTEGER IERR

C..N_RETURN.. NUMBER OF ENTRIES FOR HIT, RETURNED FROM TRACER ROUTINE
C..IU........ UNIT NUMBER TO WRITE DIAGNOSTIC
C..IPASS..... =1,2,..,MPASSA    PASS # THROUGH LOCAL AMBIGUITY
C             >MPASSA    GET CLOSEST HITS
C..ILLYR..... LOGICAL LAYER NUMBER
C..IFIT...... FIT INDEX
C..SRCSAV.... SAVED program indicator WHEN HIT ON LUSETF LAYER IS IN THE FIT
C..BETTER.... =1 IF THIS HIT IS A BETTER CHOICE FOR FTLUSE
C..IH........ HIT INDEX
C..ADFTTST... ADRESS OF FIT TESTED, IABS(IADFIT())
C..IHSV...... SAVED IH HIT INDEX FOR BEST HIT
C..IHSV2..... SAVED IH FOR THE SECOND BEST HIT IN STANDARD TFXFIT
C..ISCAT......SCATTERING CENTER NUMBER
C..YSCAT2.... MULTIPLE SCATTERING CONTRIBUTION TO DHIT ERROR, SQUARED
C..YSCAT......SQRT(YSCAT2) FOR DIAGNOSTICS
C..SIGMA_USE..RESOLUTION OF LAYER TO USE FOR ROAD CALCULATIONS
C..SIGEXT.... SIGMA OF EXTRAPOLATION RESOLUTION INTO THE SILICON
C..CMPLMTEFF. COMPLIMENTARY EFFICIENCY
C..PRODEFF....PRODUCT EFFICIENCY
C..SCLSIG.... SCALING OF SIGMA TO FIND ROAD 
C..RESSIG.... SIGMA OF THE SPATIAL RESOLUTION FUNCTION
C..RESMAX.... MAXIMUM ALLOWED ERROR TO ACCEPT A HIT, DECREASED AS HITS FOUND 
C..RESMAX_NOL RESMAX BEFORE IMPOSING LIMIT
C..RESLIM.... LIMIT TO RESMAX, TO KILL HITS THAT WILL BLOW UP CHISQ
C..RESMX2.... MISS DISTANCE (SIGNED) FOR THE SECOND BEST HIT IN STANDARD TFXFIT
C..RESUSE.... RESIDIUAL CUT TO ALLIGN HITS IN ADJACENT LAYERS
C..RESSIN.... SIGNED RESIDUAL OF A SINGLE HIT
C..RES....... ABS RERSIDUAL
C..RESDIF.... DIFFERENCE OF TWO RESIDUALS
C..ARESDF.... ABS(RESDIF)
C..ARESDFLIM. LIMIT OF ARESDF BASED ON EFFICIENCY OF HYBRID
C..ISHRD..... ISOLATED HIT ROAD USED FOR THIS KIND OF HIT
C..DIFMIN.... MINIMUM FOUND AREDIF
C..IGTEST.... TEST GROUP NUMBER FOR FINDING GROUP/LAYER OF XLYR
C..LAYDIF.... DIFFERNCE IN LAYER NUMBER FOR EVALUATING TEST GROUP NUMBER
C..IGROUP.... PRESENT GROUP NUMBER
C..SETGROUP.. =.TRUE. IF THE GROUP NUMBER FOR A HIT IS TO BE SET
C..IGLAYR.... LAYER NUMBER WITHING THE PRESENT GROUP
C..XLYR...... LOCAL PHYSICAL LAYER
C..TFLYR..... TF LAYER; TFLYR=XLYR+L0TFXF(TFXS)
C..IHCULA.... HIT NUMBER IN THE CURRENT LAYER
C..SUMPUL.... SUM OF THE MISHTG FOR THE TWO SIGNS OF AMBIGUITY
C..PULLIM.... LIMIT ON SUMPUL TO REMOVE EXTRA LONG DRIFT TIMES
C..EXTRPl.... EXTRA PULL, SUMPUL-PULLIM
C..SIGNEP.... SIGN TO ADD IN EXTRPL
C..CELSIZ.... CELLTF*RTF
C..MAXGAPUSE..MAXGAP USED
C..TRYGAP.... GAP SIZE INDEX
C..LGLAYR.... MINIMUM TESTED ILGAYR
C..LSTLYR.... LAST LAYER, ie before the last advance
C..OTHERL.... OTHER LAYER NUMBER, (LAYER NUMBER MINUS ONE FOR MATCHING)
C..OTHERH.... OTHER HIT NUMBER
C..OTHRHS.... ORIGINAL OTHER HIT NUMBER, SAVED
C..CURHT_1ST. CURRENT HIT SAMEAS LOOP FIRST HIT
C..CURHT_LST. CURRENT HIT SAMEAS LOOP LAST HIT
C..CURHT_TST. CURRENT HIT SAMEAS LOOP TESTED HIT
C..CURHT_NXT. CURRENT HIT SAMEAS LOOP NEXT HIT
C..OTHHT_1ST. OTHER HIT SAMEAS LOOP FIRST HIT
C..OTHHT_LST. OTHER HIT SAMEAS LOOP LAST HIT
C..OTHHT_TST. OTHER HIT SAMEAS LOOP TESTED HIT
C..PROCSAME.. PROCESSING SAMEAS HIT
C..HTCOMP.... HIT WITHIN LAYER USED FOR COMPARISON FOR DETERMINING
C             IF SAMEAS HIT IS VALID/UNIQUE
C..TSTSAM.... TEST TWO HITS FOR BEING VITUALLY THE SAME
C..MERGSAMEAS MERGE THE SAMEAS LOOPS FOR THE TWO HITS FOUND TO BE THE SAME
C..ISOLAT.... THE TWO HITS ARE (OR PRESUMED)ISOLATED
C 
C..HARDLINE.. BE HARD LINE; KILL HIT TO BOTH SIDES
C..KILLUSEP.. MASK TO KILL USE OF HIT TO + SIDE
C..KILLUSEM.. MASK TO KILL USE OF HIT TO - SIDE
C..MAXHIT.... MAXIMUM RANGE FOR OTHER HIT
C..MAXLNH.... LENGTH OF MAXIMUM LENGTH STRING, FOUND IN 2ND LOOP
C..LAYERLREQ. =.TRUE. IF layer LOOP REQUIREMENTS ARE still MET
C..HITLREQ... =.TRUE. IF HIT LOOP REQUIREMENTS ARE still MET
C..GROUPREQ.. =.TRUE. IF GROUP REQUIREMENTS (FOR LOOP) ARE MET
C..OLOOPHITOK =.TRUE. IF THE OUTER LOOP HIT IS OK TO PROCEED
C..ILOOPHITOK =.TRUE. IF THE INNER LOOP HIT IS OK TO PROCEED

C..DIFMIN_L.. MINIMUM FOUND AREDIF, WITHIN A LAYER SET
C..BESTHM_L...BEST HIT MATCH FOR HIT TO HIT, WITHIN A LAYER SET
C..BESTHO_L.. BEST HIT MATCH(OTHER LAYER) FOR HIT TO HIT, WITHIN A LAYER SET 
C..BESTLM_L.. BEST LAYER MATCH FOR HIT TO HIT MATCH, WITHIN A LAYER SET
C..BESTLO_L.. BEST LAYER MATCH(OTHER LAYER) FOR HIT TO HIT, WITHIN A LAYER SET

C..NBESTM.... NUMBER OF BEST MATCHES PREVIOUSLY FOUND
C..BESTHM.... BEST HIT MATCH FOR HIT TO HIT AND STRING TO TRACK
C..BESTHO.... BEST HIT MATCH (OTHER LAYER) FOR HIT TO HIT
C..BESTLM.... BEST LAYER MATCH, FOR HIT TO HIT MATCH
C..BESTLO.... BEST LAYER MATCH (OTHER LAYER) FOR HIT TO HIT

C..SETBESTGR. =.TRUE. IF THE BESTGR IS TO BE SET
C..LENGTHDIF. DIFFERENCE IN LENGTH: TESTED GROUP MINUS CURRENT BEST
C..MISSTGDIF. DIFFERENCE IN MISSTG: TESTED GROUP MINUS CURRENT BEST
C..BESTGR.... GROUP FOR BEST LONG MATCH TO EXTRAPOLATED TRACK
C..BESTLA.... BEGINNING LAYER FOR BEST MATCH
C..BSTLNG.... LENGTH OF BEST MATCH
C..BSTRES.... RESOLUTION (SIGNED) OF THE BEST MATCH
C..           USE RESMAX FOR ABS OF RESOLUTION OF BEST MATCH
C..OUTLA..... OUTER LAYER NUMBER OF CURENT INSPECTED STRING
C..BOUTLA.... OUTER LAYER OF BEST MATCH
C..MISSTM.... MAXIMUMUM AVERAGE MISHTG
C..DOBEST.... FLAG TO DO BEST STRING FIRST, THEN LOOP OVER OTHERS
C..ADDGROUP.. =.TRUE. IF aHITS FROM THIS GROUP ARE TO BE ADDED  TO TRACK
C..IGNORD.... =.TRUE. IF A STRING OF LENGTH 3 OR MORE WAS IGNORED IN  MATCHING
C..           OR, IN STANDARD TFXFIT, NOTE THAT AMBIGUOUS HIT WAS SKIPPED
C..ADDHIT.... =.TRUE. IF HIT IS TO BE ADDED
C..ADDSAMEAS. =.TRUE. IF THE ESSENTIALLY IDENTICAL HITS ARE TO BE ADDED ALSO
C..NFITS..... NFIT SAVED, TO COMPARE AFTER AN ITERATION
C..NOWFIT.... =.TRUE. IF A NEW FIT IS REQUIRED
C..ERR_TFEXER ERROR VALUE RETURNED FROM TFEXER

      INTEGER N_RETURN,IP1,IP2,IP3
      INTEGER IU,IPASS,IGO
      INTEGER IFIT,SRCSAV,ADFTTST,ILLYR,IH,IHSV,IHSV2,BETTER
      INTEGER IGTEST,LAYDIF
      LOGICAL SETGROUP
      LOGICAL LAYERLREQ,HITLREQ,GROUPREQ
      LOGICAL OLOOPHITOK,ILOOPHITOK
      INTEGER IGROUP,IGLAYR,XLYR,TFLYR,IHCULA
      INTEGER MAXGAPUSE,TRYGAP,LGLAYR
      INTEGER LSTLYR,OTHERL,OTHERH,OTHRHS,MAXHIT
      INTEGER CURHT_1ST,CURHT_LST,CURHT_TST,CURHT_NXT
      INTEGER OTHHT_1ST,OTHHT_LST,OTHHT_TST
      LOGICAL PROCSAME,TSTSAM,MERGSAMEAS,ISOLAT,HARDLINE
      INTEGER HTCOMP
      INTEGER KILLUSEP,KILLUSEM
      INTEGER MAXLNH
      INTEGER BESTHM_L,BESTHO_L,BESTLM_L,BESTLO_L
      INTEGER NBESTM,BESTHM,BESTHO,BESTLM,BESTLO
      LOGICAL SETBESTGR
      INTEGER LENGTHDIF
      REAL    MISSTGDIF
      INTEGER BESTGR,BESTLA,BSTLNG,OUTLA,BOUTLA
      INTEGER DOBEST,NFITS
      LOGICAL ADDGROUP
      LOGICAL IGNORD,ADDHIT,ADDSAMEAS,NOWFIT
      INTEGER ISCAT
      REAL    YSCAT2,YSCAT,SIGMA_USE,SIGEXT
      REAL    CMPLMTEFF,PRODEFF,SCLSIG,RESSIG,RESMAX,RES
      REAL    RESMAX_NOL,RESLIM,RESMX2
      REAL    RESUSE,RESSIN,RESDIF,ARESDF,ARESDFLIM,ISHRD
      REAL    DIFMIN_L,DIFMIN,CHIDUM
      REAL    SUMPUL,PULLIM,EXTRPL,SIGNEP,CELSIZ
      REAL    BSTRES,MISSTM
      INTEGER ERR_TFEXER

C FUNCTION DECLARATIONS
      REAL Z2PI
      EXTERNAL Z2PI

* ----------Executable code starts here---------------------

C***********************************************************************
C STARTUP
C***********************************************************************
      PROGRAM_INDICATOR=P_I_TFXFIT
      IERR=0

C make an entry for each SILICON hit in the hit list

#if defined(CLEO_TFXFDIAG)
      CALL TRACE_TFXFIT('CLEAR',0,0,0,0,0)
      DO 14 ILLYR=1,NLYRHT
         DO 13 IH=LHITLR(ILLYR),MHITLR(ILLYR)
            TFLYR=IPLRLR(ILLYR)
            IF(
     1           (IDVCTF(TFLYR).EQ.ISVX1).OR.
     2           (IDVCTF(TFLYR).EQ.ISVZ1))THEN
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2              SV_HYBRID(IADRHT(IH)),
     2              SV_HYBRID_LAYER(SV_HYBRID(IADRHT(IH))),
     2              'SILICON HYBRID, LAYER=')
C                    123456789012345678901234567890
            ENDIF
 13      CONTINUE
 14   CONTINUE
#endif

C now use the FIT_TYP argument of ROADHITS to determine fit type in TFXFIT

      IF    (SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         TFXS=1
      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         TFXS=2
      ELSE
         GO TO 899
      ENDIF

C find the total silicon efficiency
C identify the silicon layers that are in L.A. groups

      DO 29 IGTEST=1,NGRUP
         EFFTOT(IGTEST)=0.
         IF(NLAYGR(IGTEST,TFXS).GE.1)THEN
            DO 27 IGLAYR=1,NLAYGR(IGTEST,TFXS)
               XLYR=LLAYGR(IGTEST,TFXS)+IGLAYR-1
               TFLYR= XLYR + L0TFXF(TFXS)
               IF(
     1              (IDVCTF(TFLYR).EQ.ISVX1).OR.
     2              (IDVCTF(TFLYR).EQ.ISVZ1))THEN
                  EFFTOT(IGTEST)=EFFTOT(IGTEST)+EFFILR(TFLYR)


#if defined(CLEO_TFXFDIAG)
         PRINT 1016,IGTEST,IGLAYR,XLYR,TFLYR,
     2                 EFFILR(TFLYR),EFFTOT(IGTEST)
 1016    FORMAT(' TFXFIT:calc eff group, IGTEST=',I2,
     2        '  IGLYR=',I3,'  XLYR=',I3,'  TFLYR=',I3,
     3        ' EFFILR=',F9.3,'  EFFTOT=',F9.6)
#endif


               ENDIF
 27         CONTINUE
         ENDIF
 29   CONTINUE

C zero out ILLYRG

      CALL VZERO(ILLYRG,TOTLGR)

C set HITUSE and LYRUSE for hits already found in the fit list 

      CALL VZERO(LYRUSE,MLTFX)
      CALL VZERO(HITUSE,MHTFX)
      IF(NFIT.GT.0)THEN
         DO 57 IFIT=1,NFIT
            TFLYR= IPLFIT(IFIT)
            XLYR = TFLYR - L0TFXF(TFXS)
            LYRUSE(XLYR) = 1
            ILLYR=ILLRLR(TFLYR)
            IF(ILLYR.NE.0)THEN
               ADFTTST=IABS(IADFIT(IFIT))
               DO 55 IH = LHITLR(ILLYR), MHITLR(ILLYR)
                  IF(IABS(IADRHT(IH)).EQ.ADFTTST)THEN
                     HITUSE(IH)=1
                  ENDIF
 55            CONTINUE
            ENDIF
 57      CONTINUE
      ENDIF

C***********************************************************************
C DIAGNOSTICS
C***********************************************************************

#if defined(CLEO_TFXFDIAG)
      CALL VZERO(NHITGR,TOTLGR)
      CALL VZERO(IHITGR,TOTHGR)
      RUNNUM = current_run
      EVTNUM = current_event
      SEDNUM=JBYT(STATCF(1),10,8)
      RUNOLD=RUNNUM
      EVTOLD=EVTNUM
      SEDOLD=SEDNUM
#if defined(CLEO_XTSUBS)
      XTS_TFXFIT_ON=1
      CALL XTS_TRAK('CF',0,'TFXFIT:entry')
      IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif

C***********************************************************************
C ENERGY LOSS CORRECTION
C***********************************************************************

#if defined(CLEO_TFXFDIAG)
         PRINT 1011,KNK6CF,KABG2_CLEAR
 1011    FORMAT(' TFXFIT: may include energy loss, KNK6CF=',I4,
     2        '  (clear=',I2,' )')
#endif

      IF(
     1     ((KNK6CF.EQ.0).OR.(KNK6CF.EQ.KABG2_CLEAR)).AND.
     2     (ABS(KAPCF).GT. 1.5))THEN

C define the scatter, except for the amount of material
         KNK6CF=KABG2_ELOSS
         FTK6CF=KABG2_FIX
         RKN6CF=RAD_SCAT(1)
C setup to refit with PARXTR but with no material
         SKN6CF=0.
         CALL KABG2(KNK6CF,
     1        FTK6CF,RKN6CF,SKN6CF,0,
     2        KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF,  0.,
     3        KAPNOW, ALPNOW, BETNOW, GAMNOW, XINOW)
         CALL FIT_WHATEVER
#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'TFXFIT:after PARXTR')
      IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif
C setup to estimate track parameters before loss: use negative material 
         SKN6CF=-R_LEN_SCAT(1)
         CALL KABG2(KNK6CF,
     1        FTK6CF,RKN6CF,SKN6CF,0,
     2        KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF,  0.,
     3        KAPNOW, ALPNOW, BETNOW, GAMNOW, XINOW)
C estimate track parameters before loss
         CALL UCOPY(KAPCF,KAPNOW,NTKPAR)
         CALL KABG2(KABG2_CALC,
     1        0,0,0,1,
     2        KAPNOW, ALPNOW, BETNOW, GAMNOW, XINOW, PHI6NOW,
     3        KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF)
C setup with correct sign material
         SKN6CF=R_LEN_SCAT(1)
         CALL KABG2(KNK6CF,
     1        FTK6CF,RKN6CF,SKN6CF,0,
     2        KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF,  0.,
     3        KAPNOW, ALPNOW, BETNOW, GAMNOW, XINOW)

#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
C XTS_TRAK calls RESIDUALS for printout of residuals, this will set PHI6CF
         CALL XTS_TRAK('CF',0,'TFXFIT:after energy gain set-up')
         IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif

         CALL FIT_WHATEVER

#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'TFXFIT:after energy gain')
      IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif
                     
      ENDIF

C***********************************************************************
C START OF ITERATION, IGNORD IS ZEROED FOR STANDARD TFXFIT, NOT FOR AMBIGUITY
C***********************************************************************
C********************************
C****** TFXFIT COMMON CODE ******
C********************************

      IPASS=1
      IF(.NOT.LCFXLA)IPASS=MPASSA(TFXS)+1

115   NFITS = NFIT
      CALL VZERO(NHITGR,TOTLGR)
      CALL VZERO(IHITGR,TOTHGR)

C*****************************
C****** STANDARD TFXFIT ******
C*****************************

      IF(IPASS.GT.MPASSA(TFXS))THEN
         IGNORD=.FALSE.
      ENDIF


C********************************
C****** TFXFIT COMMON CODE ******
C********************************

#if defined(CLEO_TFXFDIAG)
      CALL TRACE_TFXFIT('CLEAR',0,0,0,0,0)
      CALL VZERO(GRPSTA,TOTHGR)
      CALL VZERO(HITSTA,TOTHGR)
#endif

C loop over the layers with unused hits
C cut on resolution is very wide in order to accept adjacent hits
C  for isolation test

      DO 199 ILLYR = 1, NLYRHT

         TFLYR= IPLRLR(ILLYR)
         XLYR = TFLYR - L0TFXF(TFXS)
         LAYERLREQ=.TRUE.

         IF(LAYERLREQ)THEN


C ----------------layer excluded by fast_trak
            IF(RTF(TFLYR).GT.RPRLIM)THEN
               LAYERLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
               IP3=RPRLIM*1000
               DO 117 IH=LHITLR(ILLYR),MHITLR(ILLYR)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                 IP3,0,
     3                 'layer excluded, radius > (mm) ')
C                       123456789012345678901234567890
 117           CONTINUE
#endif
               
            ENDIF

         ENDIF
         IF(LAYERLREQ)THEN


C ----------------blocking by layer
            IF(LYRUSE(XLYR).GT.1)THEN
               LAYERLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
               DO 119 IH=LHITLR(ILLYR),MHITLR(ILLYR)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                 LYRUSE(XLYR),0,
     3                 'hits in layer blocked; LYRUSE=')
C                       123456789012345678901234567890
 119           CONTINUE
#endif
               
            ENDIF

         ENDIF
         IF(LAYERLREQ)THEN

C identify group number and internal layer number
C group number also needed for standard TFXFIT

C Mar-29-2001 but group number is set =0 when not in the IPASS where
C group is active, thus, igroup is always =0 in standard even though 
C it is tested. This is also the case in old CLEO2 code. 
C now use GROUPREQ instead of testing IGROUP 

            IGROUP=0
            DO 121 IGTEST=1,NGRUP
               IF(NLAYGR(IGTEST,TFXS).LT.2)THEN
                  SETGROUP=.FALSE.
               ELSE
                  LAYDIF=XLYR-LLAYGR(IGTEST,TFXS)
                  IF(LAYDIF.LT.0)THEN
                     SETGROUP=.FALSE.
                  ELSEIF(LAYDIF.GE.NLAYGR(IGTEST,TFXS))THEN
                     SETGROUP=.FALSE.
                  ELSE
                     IF(IGROUP.EQ.0)THEN
                        SETGROUP=.TRUE.
                     ELSE
                        IF(
     1                       LLAYGR(IGTEST,TFXS).GE.
     2                       LLAYGR(IGROUP,TFXS))THEN
                           SETGROUP=.FALSE.
                        ELSE
                           SETGROUP=.TRUE.
                        ENDIF
                     ENDIF
                  ENDIF
               ENDIF
               IF(SETGROUP)THEN
                  IGROUP=IGTEST
                  IGLAYR=LAYDIF+1
               ENDIF
 121        CONTINUE

            IF(IGROUP.NE.0)THEN
               GROUPREQ=(  (NLAYGR(IGROUP,TFXS).GE.2)
     2              .AND. ((IPASS.EQ.PASS1G(IGROUP,TFXS))
     3              .OR.  (IPASS.GE.PASSAG(IGROUP,TFXS))))
            ELSE
               GROUPREQ=.FALSE.
            ENDIF

#if defined(CLEO_TFXFDIAG)
            PRINT 1015,IPASS,ILLYR,TFLYR,IGROUP,GROUPREQ
 1015       FORMAT(' TFXFIT:setting group, IPASS=',I2,
     2           '  ILLYR=',I3,'  TFLYR=',I3,
     3           '  IGROUP=',I3,'  GROUPREQ=',L1)
#endif
         ENDIF

C*****************************
C****** STANDARD TFXFIT ******
C*****************************

         IF(IPASS.GT.MPASSA(TFXS))THEN
            IF(LAYERLREQ)THEN

C set up resmax for standard TFXFIT
C also save RESMAX for display

C ----------------blocking by layer
               IF(LYRUSE(XLYR).GT.0)THEN
                  LAYERLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  DO 127 IH=LHITLR(ILLYR),MHITLR(ILLYR)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                    LYRUSE(XLYR),0,
     3                    'hits in layer blocked; LYRUSE=')
C                          123456789012345678901234567890
 127              CONTINUE
#endif
               ENDIF
               
            ENDIF
            IF(LAYERLREQ)THEN

               SIGMA_USE=SIGMA_LAYER(TFLYR,HITTYP_HIT,SAV_FIT_TYP)

C ADF the following is for extrapolation error
               SIGEXT=0.0
               CALL TFEXER(TFLYR,SIGEXT,ERR_TFEXER)

C error due to scattering; use new scatter #2
C                      NO, use new scatter #1
               ISCAT=1
               IF(ACTIV_SCAT(ISCAT))THEN
                  YSCAT2 =   R_LEN_SCAT(ISCAT) *
     1                 ((RAD_SCAT(ISCAT)-RTF(TFLYR))
     2                 *SCTGEV*EETF/P2TF)**2
               ELSE
                  YSCAT2=0.
               ENDIF
               RESSIG= SQRT(SIGMA_USE**2 + YSCAT2)

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               IF(IGROUP.EQ.0)THEN
                 IF(WIDE.EQ.TFXF_WIDE_STAND)THEN
                   SCLSIG=SCALNW(XLYR,TFXS)
                 ELSE
                   SCLSIG=SCALEN(XLYR,TFXS)
                 ENDIF
               ELSEIF(ULEFSC(IGROUP,TFXS))THEN
                  CMPLMTEFF=EFFTOT(IGROUP)-EFFILR(TFLYR)
                  IF(    CMPLMTEFF.GE.HCMPEF(IGROUP,TFXS))THEN
                     SCLSIG=HEFSCL(IGROUP,TFXS)
                  ELSEIF(CMPLMTEFF.LE.LCMPEF(IGROUP,TFXS))THEN
                     SCLSIG=LEFSCL(IGROUP,TFXS)
                  ELSE
                     SCLSIG=
     1                    LEFSCL(IGROUP,TFXS)
     2                    +(HEFSCL(IGROUP,TFXS)-LEFSCL(IGROUP,TFXS))
     3                    *(CMPLMTEFF          -LCMPEF(IGROUP,TFXS))
     4                    /(HCMPEF(IGROUP,TFXS)-LCMPEF(IGROUP,TFXS))
                  ENDIF
               ELSE
                  SCLSIG=HEFSCL(IGROUP,TFXS)
               ENDIF
               RESMAX= SCLSIG*SQRT(RESSIG**2+SIGEXT**2)
               
               IF(TFXS.EQ.1)THEN
                  IF(
     1                 (IDVCTF(TFLYR).EQ.ISVX1).OR.
     2                 (IDVCTF(TFLYR).EQ.ISVZ1))THEN
                     RESLIM=RESMAX
                  ELSE
                     RESLIM = SIGMA_USE
     1                    *SQRT(NFIT*CH2SCL(NFIT-3)*CHICUT)
     2                    *STANRD(TFXS)
                  ENDIF
               ELSEIF(TFXS.EQ.2)THEN
                  RESLIM = RESMAX
               ENDIF

#if defined(CLEO_TFXFDIAG)

               YSCAT=SQRT(YSCAT2)
               PRINT 1013
 1013          FORMAT(' TFXFIT:st:',
     2              'layr defltSIG   nRL  scatter expctSIG',
     3              ' extrpolt scale   RESMAX   RESLIM')
               PRINT 1014,TFLYR,SIGMA_USE,R_LEN_SCAT(ISCAT),
     2              YSCAT,RESSIG,
     3              SIGEXT,SCLSIG,RESMAX,RESLIM
 1014          FORMAT(' TFXFIT:st:',I4,F9.6,F6.3,F9.6,F9.6,
     2              F9.6,F6.2,F9.6,F9.6)  
#endif

               RESMAX_NOL=RESMAX
               IF(LCFXLA)THEN
                  IF(RESMAX.GT.RESLIM)RESMAX=RESLIM
               ENDIF
               RESMX2=RESMAX

            ENDIF
         ENDIF

C************************************
C****** LOCAL AMBIGUITY TFXFIT ******
C************************************

         IF(IPASS.LE.MPASSA(TFXS))THEN
            IF(LAYERLREQ)THEN

C set up for local ambiguity TFXFIT
C note: illyrg is only set for ambiguity layers with hits in TFHITSCD

               IF(.NOT.GROUPREQ)THEN
                  LAYERLREQ=.FALSE.
#if defined(CLEO_TFXFDIAG)
                  DO 135 IH=LHITLR(ILLYR),MHITLR(ILLYR)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                    'hits in layer blocked;GROUPREQ')
C                          123456789012345678901234567890
 135              CONTINUE
#endif
               ENDIF

            ENDIF
            IF(LAYERLREQ)THEN
               
               CELSIZ=CELLTF(TFLYR)*RTF(TFLYR)
               RESMAX=ILAMRD(IGROUP,TFXS)
               ILLYRG(IGROUP,IGLAYR)=ILLYR
         
C start to identify status of LUSETF layers, which hit is used

               FTLUSE(IGROUP,IGLAYR)=-1
               IF(LUSETF(TFLYR))FTLUSE(IGROUP,IGLAYR)=0
               SRCUSE(IGROUP,IGLAYR)=0

            ENDIF
         ENDIF


C********************************
C****** TFXFIT COMMON CODE ******
C********************************

         IF(LAYERLREQ)THEN

C loop through the hits in this layer

      IHSV= 0
      IHSV2=0
      DO 179 IH=LHITLR(ILLYR),MHITLR(ILLYR)
         HITLREQ=.TRUE.

         IF(HITLREQ)THEN

#if defined(CLEO_TFXFDIAG)
            IF(
     1           (IDVCTF(TFLYR).EQ.ISVX1).OR.
     2           (IDVCTF(TFLYR).EQ.ISVZ1))THEN

               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2              SV_HYBRID(IADRHT(IH)),
     2              SV_HYBRID_LAYER(SV_HYBRID(IADRHT(IH))),
     2              'SILICON HYBRID number, layer=')
C                    123456789012345678901234567890

               IP2=SV_HYBRID_EFF(SV_HYBRID(IADRHT(IH)),TFXS)*100
               IP3=EFFILR(TFLYR)*100
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2              'SILICON EFF: hybrid, layer =')
C                    123456789012345678901234567890

            ENDIF

            CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IADRHT(IH),0,
     2           'start hit in 1st loop; IADRHT=')
C                 123456789012345678901234567890
#endif

            IF    (TFXS.EQ.1)THEN
               IF(STRFHT(IH).LE.0)THEN
                  HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,STRFHT(IH),0,
     2                 'hit has no RF status, STRFHT=')
#endif
               ENDIF

            ELSEIF(TFXS.EQ.2)THEN
               IF(STSZHT(IH).LE.0)THEN
               HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,STSZHT(IH),0,
     2                 'hit has no SZ status, STSZHT=')
#endif
               ENDIF
            ENDIF
         ENDIF

C*****************************
C****** STANDARD TFXFIT ******
C*****************************

         IF(IPASS.GT.MPASSA(TFXS))THEN
            IF(HITLREQ)THEN

#if defined(CLEO_TFXFDIAG)
               IP2=SIGMA_USE*1000000
               IP3=RESSIG*1000000
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2              'SIGMA_USE,RESSIG=    ')

               IP2=SIGEXT*1000000
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,0,
     2              'extrapolation error= ')    

               IF(IGROUP.EQ.0)THEN
                  IP3=SCLSIG*100
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP3,0,
     2                 '(NOT IN GROUP) scaling(*100)=')    
               ELSE
                  IP3=SCLSIG*100
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP3,0,
     2                 '(IN GROUP) scaling(*100)=')    
                  IF(ULEFSC(IGROUP,TFXS))THEN
                     IP2=EFFILR(TFLYR)*100
                     IP3=CMPLMTEFF*100
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2                    'eff(LAY),eff(compl)(*100)=')    
                  ELSE
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                    'not using eff-dep scaling')    
C                          123456789012345678901234567890
                  ENDIF
               ENDIF
               
               IP2=RESMAX_NOL*1000000
               IP3=RESLIM*1000000
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2              'RESMAX(prelim),limit=')
#endif
            ENDIF
         ENDIF


C********************************
C****** TFXFIT COMMON CODE ******
C********************************
         IF(HITLREQ)THEN

#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2           'hit not blocked')
#endif

            CALL RESIDUALS('HIT',0,IH,RESSIN)
            RES=ABS(RESSIN)
         ENDIF


C*****************************
C****** STANDARD TFXFIT ******
C*****************************

         IF(IPASS.GT.MPASSA(TFXS))THEN
            IF(HITLREQ)THEN
               
#if defined(CLEO_TFXFDIAG)
               IP1=RESSIN*1000000
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,0,
     2              'using STANDARD tfxfit')
#endif

C note: LYRUSE is checked earlier
C add hits on .NOT.LUSETF layers
C may add in hits not already found on LUSETF layers
C but only if the layer is in a local amb group
C  and only if local ambiguity groups are used (LCFXLA=.T.)

               IF(LUSETF(TFLYR))THEN
#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                 'hit is in LUSETF layer')
#endif
                  
                  IF(.NOT.LCFXLA)THEN
                     HITLREQ=.FALSE.
                  
#if defined(CLEO_TFXFDIAG)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                    'but.. LOC AMB is NOT enabled')
#endif
                  ELSE

#if defined(CLEO_TFXFDIAG)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                    'in standard; LOC AMB enabled')
#endif

                  ENDIF

C This is effectively the old code which, as of some unknown time long ago,
C defeted ability to add these hits.
C This is only a problem in LUSETF layers which makes it not_a_problem
C in CLEO3 since LUSETF layers are the layers with hits.
                  IF(HITLREQ)THEN
                     IF(.NOT.GROUPREQ)THEN

C-----------------------------------------------------------------------
C This is effectively the new code (the new use of IGROUP)
C which would allow testing LA layers that are not blocked.
C                    IF(IGROUP.EQ.0)THEN
C-----------------------------------------------------------------------

                        HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                       'layer is in a LOC AMB group')
#endif
                        
                     ENDIF
                  ENDIF

C do not add a hit to an LUSETF layer that alraedy has one

                  IF(HITLREQ)THEN
                     IF(FTLUSE(IGROUP,IGLAYR).GT.0)THEN
                        HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                       'already have a hit in layer')
#endif
                     ELSE

#if defined(CLEO_TFXFDIAG)
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                       'but there is no hit in layer')
#endif
                     ENDIF
                  ENDIF
                  
               ELSE

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                 'hit is NOT in LUSETF layer')
#endif

C end of test if this is LUSETF layer
               ENDIF

            ENDIF
            IF(HITLREQ)THEN
    
               IF(ABS(ZHIT(IH)).LT. 1.200)THEN

                  IF(RES.LT.ABS(RESMAX))THEN

#if defined(CLEO_TFXFDIAG)
C pass residual cut
                     IP1=RES*1000000
                     IP2=RESMAX*1000000
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,IP2,
     2                    'hit chosen BEST in layer')
                     IF(IHSV.NE.0)THEN
                        CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2                       'hit displaced to 2nd best')
                     ENDIF
#endif
                     
                     RESMX2=RESMAX
                     IHSV2=  IHSV
                     RESMAX= RESSIN
                     IHSV=   IH
                     
                  ELSEIF(RES.LT.ABS(RESMX2))THEN

#if defined(CLEO_TFXFDIAG)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                    'hit chosen 2nd best in layer')
                     IF(IHSV2.NE.0)THEN
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                       'hit displaced from 2nd best')
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                       'hit displaced from 2nd best')
                     ENDIF
#endif

                     RESMX2 = RESSIN
                     IHSV2  = IH
                  ELSE

#if defined(CLEO_TFXFDIAG)
                     IP1=RESMAX*1000000
                     IP2=RESMX2*1000000
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,IP2,
     2                    'hit not chosen')
#endif
                     
                  ENDIF
               ELSE
#if defined(CLEO_TFXFDIAG)
                  IP1=ABS(ZHIT(IH))*1000
                  IP2=1.200*1000
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,IP2,
     2                 'hit not chosen, BEYOND Z LIMIT')
#endif
               ENDIF
            ENDIF
         ENDIF

C************************************
C****** LOCAL AMBIGUITY TFXFIT ******
C************************************

         IF(IPASS.LE.MPASSA(TFXS))THEN
            IF(HITLREQ)THEN

#if defined(CLEO_TFXFDIAG)
               IP1=RESSIN*1000000
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,0,
     2              'start LOCAL AMBIGUITY tfxfit')
#endif

C check ambiguity group;  check if LUSETF layer hit used in fit
C  FTLUSE(IGROUP,IGLAYR) will match IHITGR(IGROUP,IGLAYR,IHCULA)

C CLEO3 does not have "LUSETF" layers
C the following line is CLEO2 only and replaces the line in the loop
C            IF(FTLUSE(IGROUP,IGLAYR).EQ.0)THEN
C but now, with multiple hits at entry, there is no test on FTLUSE

C test if hit is in the fit, still in a big loop over hit: IH
C layer agreement required because silicon etc may 
C have overlaping use of IADFIT

               IF(NFIT.GE.1)THEN
                  IFIT=0
                  SRCSAV=0
 151              IF((SRCSAV.EQ.0).AND.(IFIT.LT.NFIT))THEN
                     IFIT=IFIT+1
                     IF((IPLFIT(IFIT).EQ.TFLYR).AND.
     2                    (IADFIT(IFIT).EQ.IADRHT(IH)))THEN
                        SRCSAV=SRCFIT(IFIT)
                     ENDIF
                     GO TO 151
                  ENDIF


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                  IF(SRCSAV.NE.0)THEN
                     BETTER=0
                     
#if defined(CLEO_TFXFDIAG)
                     CALL TRACE_TFXFIT('LOG',IH,N_RETURN,SRCSAV,0,
     2                    'hit is already in fit, source=')
C                          123456789012345678901234567890
#endif

C prefer FAST_TRAK hits first
                     IF((BETTER.EQ.0).AND.(SRCSAV.EQ.11).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).NE.11))BETTER=1
                     IF((BETTER.EQ.0).AND.(SRCSAV.NE.11).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).EQ.11))BETTER=-1
C prefer TFXFIT hits second
                     IF((BETTER.EQ.0).AND.(SRCSAV.EQ.21).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).NE.21))BETTER=1.
                     IF((BETTER.EQ.0).AND.(SRCSAV.NE.21).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).EQ.21))BETTER=-1
C prefer SCAN_Z_ROADS hits third
                     IF((BETTER.EQ.0).AND.(SRCSAV.EQ.31).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).NE.31))BETTER=1
                     IF((BETTER.EQ.0).AND.(SRCSAV.NE.31).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).EQ.31))BETTER=-1
C prefer BOOT_SI_Z_HITS hits forth
                     IF((BETTER.EQ.0).AND.(SRCSAV.EQ.41).AND.
     2                    (SRCUSE(IGROUP,IGLAYR).NE.41))BETTER=1

                     IF(BETTER.EQ.1)THEN

#if defined(CLEO_TFXFDIAG)
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                       FTLUSE(IGROUP,IGLAYR),0,
     3                       'set FTLUSE, replacing prev=')
C                             123456789012345678901234567890
#endif

                        FTLUSE(IGROUP,IGLAYR)=IH
                        SRCUSE(IGROUP,IGLAYR)=SRCSAV
                     ENDIF
                  ENDIF
               ENDIF
            
C put into ambiguity group

               IF(RES.GT.RESMAX)THEN
                  HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  IP1=RES*1000000
                  IP2=RESMAX*1000000
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP1,IP2,
     2                 'reject hit based on residual')
#endif
               ENDIF

            ENDIF
            IF(HITLREQ)THEN
         
               IF(NHITGR(IGROUP,IGLAYR).GE.MGSTOR)THEN
                  HITLREQ=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',IH,N_RETURN,0,0,
     2                 'lose hit: storage saturated')
#endif
               ENDIF

            ENDIF
            IF(HITLREQ)THEN

               NHITGR(IGROUP,IGLAYR)=NHITGR(IGROUP,IGLAYR)+1
               IHITGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=IH
               LFESGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=IGLAYR
               EFESGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=
     2              NHITGR(IGROUP,IGLAYR)
               NXLRGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=0
               NXHTGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=0
               LNHTGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=1
               NGAPGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=0
               MISHTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=RESSIN
               MISSTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=RESSIN
               REDUCD(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=0
               SAMEAS(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=0
               ISOHGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=63

#if defined(CLEO_TFXFDIAG)
               CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IGROUP,IGLAYR,
     2              'attach hit to local amb group')
#endif

C remainder of loop is only for multiple hits in the layer

               IF(NHITGR(IGROUP,IGLAYR).GE.2)THEN

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C check if the drift distance is large compared to the cell size

                  IF( 
     1                 IADRHT(IHITGR
     2                 (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)).EQ.
     3                 -IADRHT(IH))THEN
                     SUMPUL=
     1                    ABS(MISHTG
     2                    (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)
     3                    -RESSIN)  /2.
                     PULLIM=LDRFRD(IGROUP,TFXS)*CELSIZ
                     EXTRPL=SUMPUL-PULLIM
                     IF(EXTRPL.GE.0.)THEN
                        SIGNEP=1.
                        IF(IADRHT(IH).LT.0)SIGNEP=-1.
                        MISHTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=
     2                       MISHTG
     3                       (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))
     4                       -SIGNEP*EXTRPL
                        MISSTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=
     2                       MISHTG
     3                       (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))
                        REDUCD(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=1

                        MISHTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)=
     2                       MISHTG
     3                       (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)
     4                       +SIGNEP*EXTRPL
                        MISSTG(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)=
     2                       MISHTG
     3                       (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)
                        REDUCD(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)=1
                  
#if defined(CLEO_TFXFDIAG)
                        IP2=SUMPUL*1000000
                        IP3=PULLIM*1000000
                        CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2                       'reduce large drift distance')
                        CALL TRACE_TFXFIT('LOG',
     2                       IHITGR
     2                       (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)-1)
     3                       ,N_RETURN,IP2,IP3,
     4                       'reduce large drift distance')
#endif
                     ENDIF
                  ENDIF

C check against other hits in this layer for isolation
C and check if hits are virtually the same as other hits
C unisolated hits are not allowed in strings so if a smaller
C fraction of a cell is used for FTLUSE hits, they are more able
C to be placed in string.
C virtually the same hits are used to force the correct FTLUSE hit
C into the string.
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                  MAXHIT=NHITGR(IGROUP,IGLAYR)-1
                  DO 169 OTHERH=1,MAXHIT
                     TSTSAM=.FALSE.
                     ISOLAT=.TRUE.
                     RESDIF=RESSIN-MISHTG(IGROUP,IGLAYR,OTHERH)
                     ARESDF=ABS(RESDIF)
                     
                     IF(FTLUSE(IGROUP,IGLAYR).GE.0)THEN
                        ISHRD=ISHTRD(IGROUP,TFXS)
                     ELSE
                        ISHRD=ISHFRD(IGROUP,TFXS)
                     ENDIF
                     
                     IF(IABS(IADRHT(IHITGR(IGROUP,IGLAYR,OTHERH))).EQ.
     1                    IABS(IADRHT(IH)))THEN
                        
C hits are from the same wire, just with different sign of drift distance.
C therefore, do not test them for isolation.
C when the drift distance is identically zero, the string selection, by
C virtue of using ".GT." instead of ".GE." will always choose the second
C entry (positive drift distance) and the string will not be
C artificially broken.
C when on an FTLUSE layer, that has a hit in the fit,
C must force to use the hit in the fit by setting the SAMEAS pointer
C ( of course, when the hits are this close, each hit is used in
C a track solution from link-tree)

                        IF(ARESDF.LT. SAMHRD(IGROUP,TFXS))THEN
                           TSTSAM=.TRUE.
                        ELSE

#if defined(CLEO_TFXFDIAG)
                           IP1=SAMHRD(IGROUP,TFXS)*1000000
                           IP2=ARESDF*1000000
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                          IP2,IP1,
     3                          'same wire failed same hit road')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IP2,IP1,
     4                          'same wire failed same hit road')
#endif
                        ENDIF
                        
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C hits come from different wires
C isolated if outside of some fraction of a cell width

                     ELSE
                        IF(ARESDF.LE. ISHRD)THEN
                           ISOLAT=.FALSE.
                        ENDIF
               
C same hit if within 120 micron; treat as isolated, 

                        IF(.NOT.ISOLAT)THEN
                           IF(ARESDF.LT. SAMHRD(IGROUP,TFXS))THEN
                              TSTSAM=.TRUE.
                           ENDIF
                        ENDIF
                     ENDIF
               
C hits are virtually the same
                     IF(TSTSAM)THEN
                        ISOLAT=.TRUE.
                        
                        CURHT_1ST=NHITGR(IGROUP,IGLAYR)
                        CURHT_LST=CURHT_1ST
                        OTHHT_1ST=OTHERH
                        OTHHT_LST=OTHHT_1ST
                        MERGSAMEAS=.TRUE.

C ADF 4/30/96 This is to deal with the loop problem, 6->8->7->8->7 etc.
C	It is imperfect for dealing with silicon corner hits because
C	it could allow a nearby noise to grab the sameas instead of
C	a nearby corner hit in the other wafer.  This is inefficient
C	and needs more thought, but is probably not a big deal
C	(says me.)

C                  IF((SAMEAS(IGROUP,IGLAYR,CURHT_LST).NE.0)
C     2              .OR.
C     3               (SAMEAS(IGROUP,IGLAYR,OTHHT_1ST).NE.0)
C     4              )THEN
C                     MERGSAMEAS=.FALSE.
C                  ENDIF

C DPP 9/11/00 Replace the 4/30/96 ADF logic. Find the beginning and end
C       of the "same as" loops defined by the current hit the "other" hit
C       which has newly been found to be the same as the current hit.
C       If the "other" hit is not currently found in the "current" "same as"
C       loop, merge the loops.

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                        IF(SAMEAS(IGROUP,IGLAYR,CURHT_1ST).NE.0)THEN
                           CURHT_TST=SAMEAS(IGROUP,IGLAYR,CURHT_1ST)
 161                       IF(MERGSAMEAS)THEN
                              IF(CURHT_TST.EQ.OTHERH)MERGSAMEAS=.FALSE.
                              CURHT_LST=CURHT_TST
                              CURHT_TST=SAMEAS(IGROUP,IGLAYR,CURHT_TST)
                              IF(CURHT_TST.NE.CURHT_1ST)GO TO 161
                           ENDIF
                        ENDIF
                        IF(MERGSAMEAS)THEN
                           IF(SAMEAS(IGROUP,IGLAYR,OTHHT_1ST).NE.0)THEN
                              OTHHT_TST=SAMEAS(IGROUP,IGLAYR,OTHHT_1ST)
 162                          OTHHT_LST=OTHHT_TST
                              OTHHT_TST=SAMEAS(IGROUP,IGLAYR,OTHHT_TST)
                              IF(OTHHT_TST.NE.OTHHT_1ST)GO TO 162
                           ENDIF
                        ENDIF

                        IF(MERGSAMEAS)THEN
                           SAMEAS(IGROUP,IGLAYR,CURHT_LST)=OTHHT_1ST
                           SAMEAS(IGROUP,IGLAYR,OTHHT_LST)=CURHT_1ST

#if defined(CLEO_TFXFDIAG)
                           IP2=ARESDF*1000000
                           CALL TRACE_TFXFIT('LOG',
     2                          IH,N_RETURN,
     3                          IHITGR(IGROUP,IGLAYR,OTHERH),IP2,
     4                          'hit is "same as" another')
                           
                           CURHT_TST=CURHT_1ST
 163                       CURHT_NXT=SAMEAS(IGROUP,IGLAYR,CURHT_TST)
                           IF(CURHT_NXT.NE.CURHT_1ST)THEN
                              CALL TRACE_TFXFIT('LOG',
     2                             IH,N_RETURN,
     3                             IHITGR(IGROUP,IGLAYR,CURHT_TST),
     3                             IHITGR(IGROUP,IGLAYR,CURHT_NXT),
     4                             'SAMEAS link list from/to..')
C                                   123456789012345678901234567890
                              CURHT_TST=CURHT_NXT
                              GO TO 163
                           ENDIF

                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IH,IP2,
     4                          'hit is "same as" another')
#endif
                        ELSE

#if defined(CLEO_TFXFDIAG)
                           IP2=ARESDF*1000000
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),IP2,
     3                          '"same as" blocked by previous')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IH,IP2,
     4                          '"same as" blocked by previous')
#endif
                        ENDIF
                     ENDIF


C hit is not isolated

                     IF(.NOT.ISOLAT)THEN
                        HARDLINE=.TRUE.
                        IF(HARDLINE)THEN
                           KILLUSEP=60
                           KILLUSEM=60
                        ELSE
                           KILLUSEP=61
                           KILLUSEM=62
                        ENDIF
                        IF(RESDIF.GE.0)THEN
                  
C RESSIN > MISHTG(igroup,iglayr,OTHERH)
C current hit is not isolated to the (-) side
C earlier hit is not isolated to the (+) side

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                           ISOHGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=
     2                          IAND(
     3                          ISOHGR
     3                          (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)),
     4                          KILLUSEM)
                           ISOHGR(IGROUP,IGLAYR,OTHERH)=               
     2                          IAND(
     3                          ISOHGR
     3                          (IGROUP,IGLAYR,OTHERH),
     4                          KILLUSEP)
                           
#if defined(CLEO_TFXFDIAG)
                           IP1=SAMHRD(IGROUP,TFXS)*1000000
                           IP2=ARESDF*1000000
                           IP3=ISHRD*1000000
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2                          'hit not isolated on (-) side ')
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP1,
     2                          'hits failed same hit road    ')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IP2,IP3,
     4                          'hit not isolated on (+) side ')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IP2,IP1,
     4                          'hits failed same hit road    ')
#endif

C RESSIN < MISHTG(igroup,iglayr,OTHERH)
C CURRENT HIT IS NOT ISOLATED TO THE (+) SIDE
C EARLIER HIT IS NOT ISOLATED TO THE (-) SIDE

                        ELSE
                           ISOHGR(IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR))=
     2                          IAND(
     3                          ISOHGR
     3                          (IGROUP,IGLAYR,NHITGR(IGROUP,IGLAYR)),
     4                          KILLUSEP)
                           ISOHGR(IGROUP,IGLAYR,OTHERH)=
     2                          IAND(
     3                          ISOHGR
     3                          (IGROUP,IGLAYR,OTHERH),
     4                          KILLUSEM)
                  
#if defined(CLEO_TFXFDIAG)
                           IP1=SAMHRD(IGROUP,TFXS)*1000000
                           IP2=ARESDF*1000000
                           IP3=ISHRD*1000000
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP3,
     2                          'hit not isolated on (+) side ')
                           CALL TRACE_TFXFIT('LOG',IH,N_RETURN,IP2,IP1,
     2                          'hits failed same hit road    ')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IP2,IP3,
     4                          'hit not isolated on (-) side ')
                           CALL TRACE_TFXFIT('LOG',
     2                          IHITGR(IGROUP,IGLAYR,OTHERH),N_RETURN,
     3                          IP2,IP1,
     4                          'hits failed same hit road    ')
#endif
                     
                        ENDIF
                     ENDIF

C end of loop over other hits in layer
 169              CONTINUE
C end of requirement that there are at least 2 hits
               ENDIF
C end of inner HITLREQ test
            ENDIF
C end of outer HITLREQ test
         ENDIF


C********************************
C****** TFXFIT COMMON CODE ******
C********************************
C end of loop over hits
 179  CONTINUE


C*****************************
C****** STANDARD TFXFIT ******
C*****************************
      IF(IPASS.GT.MPASSA(TFXS))THEN

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C LOAD UP THE CLOSEST HIT FOR STANDARD TFXFIT
C BUT FIRST CHECK IF THERE ARE TWO CHOISES WITHIN LIMITS
C   GIVING WILDLY DIFFERENT RESULT

         IF(IHSV.EQ.0)THEN
            ADDHIT=.FALSE.
           
C This is effectively the old code which, as of some unknown time long ago,
C defeted ability to test if there are ambiguous choises.
C This is a real problem.
         ELSEIF(.NOT.GROUPREQ)THEN

C This is effectively the new code (the new use of IGROUP)
C which would allow testing for ambiguous choises.
C         ELSEIF(IGROUP.EQ.0)THEN

            ADDHIT=.TRUE.
#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2           'ADD hit; not in amb group')
#endif

         ELSEIF(IHSV2.EQ.0)THEN
            ADDHIT=.TRUE.
#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2           'ADD hit; only hit available')
#endif

         ELSEIF(ABS(RESMX2)-ABS(RESMAX).GT.SMLDIF(XLYR,TFXS))THEN
            ADDHIT=.TRUE.
#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2           'ADD hit; unambiguous choice')
#endif

         ELSEIF(ABS(RESMX2-RESMAX)     .LT.LRGDIF(XLYR,TFXS))THEN
            ADDHIT=.TRUE.
#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2           'ADD hit; ambiguous, but close')
C                 123456789012345678901234567890
#endif

         ELSE
            ADDHIT=.FALSE.
            IGNORD=.TRUE.
#if defined(CLEO_TFXFDIAG)
            CALL TRACE_TFXFIT('LOG',IHSV,N_RETURN,0,0,
     2           '(standard) ignore hit        ')
#endif

         ENDIF

C ADD THE HIT
         IF(ADDHIT)THEN
            CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHSV,0)
            IF((ABS(KAPCF)*RTF(TFLYR)).GT.0.9)THEN
               CALL HIT_TO_FIT(H2F_DEWEIGHT,
     2              DEWCRL(XLYR,TFXS),NFIT)
               CALL HIT_TO_FIT(H2F_CHANGE_P_I,
     2              P_I_TFXFIT_REDUCE,NFIT)
            ENDIF
            LYRUSE(XLYR)=4
            HITUSE(IHSV)=4
         ENDIF
      ENDIF

C********************************
C****** TFXFIT COMMON CODE ******
C********************************
C END OF LOOP OVER LAYERS,  STANDARD TFXFIT IS READY TO FIT

      ENDIF
 199  CONTINUE

C************************************
C****** LOCAL AMBIGUITY TFXFIT ******
C************************************
      IF(IPASS.GT.MPASSA(TFXS))GO TO 511

C check against other hits in previous layers
C RESUSE is set in this section to 30% of half cell size; about 1.05mm for ptl
C always allow LUSETF layers in pattern recognition
C second loop through groups

      MAXLNH=1
      DO 289 IGROUP=1,NGRUP
       GROUPREQ=(  (NLAYGR(IGROUP,TFXS).GE.2)
     2      .AND. ((IPASS.EQ.PASS1G(IGROUP,TFXS))
     3       .OR.  (IPASS.GE.PASSAG(IGROUP,TFXS))))
       IF(GROUPREQ)THEN
          NBESTM=0

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C loop over the gap size, this gives priority to lower gap
         MAXGAPUSE=MAXGAP(IGROUP,TFXS)
         IF(MAXGAPUSE.GT.NLAYGR(IGROUP,TFXS)-2)
     2      MAXGAPUSE=NLAYGR(IGROUP,TFXS)-2
         DO 285 TRYGAP=0,MAXGAPUSE
          LGLAYR=2+TRYGAP

C continue the outer and inner loops until all the possible matches are made

 211      BESTHM=0

C loop over (outer) layers in the group
C demand hits in this layer and the previous layer 

          DO 267 IGLAYR=LGLAYR,NLAYGR(IGROUP,TFXS)
           IF(NHITGR(IGROUP,IGLAYR).GE.1)THEN
            OTHERL=IGLAYR-1-TRYGAP
            IF(NHITGR(IGROUP,OTHERL).GE.1)THEN
             XLYR=LLAYGR(IGROUP,TFXS)+IGLAYR-1
             TFLYR= XLYR + L0TFXF(TFXS)
             CELSIZ=CELLTF(TFLYR)*RTF(TFLYR)
             RESUSE=CHRDHE(IGROUP,TFXS)
             BESTHM_L=0


C Try to attach this hit to an earlier hit.
C Get the very best of all combinations; execute it.
C Try again and again until there are no choises.

C Loop over hits in this (outer) layer; 
C demand hit open to use as later part of string
             DO 261 IHCULA=1,NHITGR(IGROUP,IGLAYR)
C If this is the wrong hit of an FTLUSE layer and it is virtually the
C same as the correct hit, do not look at it so only the correct FTLUSE
C hit will be considered.
C now loop over the SAMEAS link list to test each element

              OTHHT_1ST=SAMEAS(IGROUP,IGLAYR,IHCULA)
              IF(OTHHT_1ST.EQ.0)THEN
                 OLOOPHITOK=.TRUE.
              ELSE
                 OLOOPHITOK=.TRUE.
                 OTHHT_TST=OTHHT_1ST
 225             CONTINUE
                 IF(OLOOPHITOK.AND.(OTHHT_TST.NE.IHCULA))THEN
                    OLOOPHITOK=(FTLUSE(IGROUP,IGLAYR).NE.
     1                   IHITGR(IGROUP,IGLAYR,OTHHT_TST))
                    OTHHT_TST=SAMEAS(IGROUP,IGLAYR,OTHHT_TST)
                    GO TO 225
                 ENDIF
              ENDIF
              IF(OLOOPHITOK)THEN

C If either the virtually same hit is not the hit used in the fit
C or this is not an FTLUSE layer, then....
C if the virtually same hit is already part of a string it is preferred.
C In this case do not consider the hit.
C Condition is now simply tested with ISOHGR =8 bit which is turned off
C when a virtually same hit is added to the fit
C This stops artificials stops/starts of strings when two hits are close.

C Require that a hit (with drift sign to the - side) is ISOLATED on the -side. 
C Require that a hit (with drift sign to the + side) is ISOLATED on the +side.
C Require that all hits are open to use as the LATER part of a string.
               IF(IADRHT(IHITGR(IGROUP,IGLAYR,IHCULA)).LE.0)THEN
                OLOOPHITOK=(IAND(ISOHGR(IGROUP,IGLAYR,IHCULA),
     2                            9) .EQ. 9 )
               ELSE
                OLOOPHITOK=(IAND(ISOHGR(IGROUP,IGLAYR,IHCULA),
     2                           10) .EQ.10 )
               ENDIF
               IF(OLOOPHITOK)THEN

#if defined(CLEO_TFXFDIAG)
                  IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                  CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,NBESTM,TRYGAP,
     2                 'pass outer loop, NBESTM, gap=')
C                       123456789012345678901234567890
#endif

C Require that the number of existing gaps in the outer hit plus
C the current TRYGAP is not above the maximum allowed

                IF( NGAPGR(IGROUP,IGLAYR,IHCULA)+TRYGAP 
     2                            .LE.TOTGAP(IGROUP,TFXS) )THEN

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C Loop over other(INNER) layer;demand hit open to use as EARLY part of string
                 DO 257 OTHERH=1,NHITGR(IGROUP,OTHERL)
C If this is the wrong hit of an FTLUSE layer and it is virtually the
C same as the correct hit, do not look at it so only the correct FTLUSE
C hit will be considered.
C now loop over the SAMEAS link list to test each element

                  OTHHT_1ST=SAMEAS(IGROUP,OTHERL,OTHERH)
                  IF(OTHHT_1ST.EQ.0)THEN
                     ILOOPHITOK=.TRUE.
                  ELSE
                     ILOOPHITOK=.TRUE.
                     OTHHT_TST=OTHHT_1ST
 227                 CONTINUE
                     IF(ILOOPHITOK.AND.(OTHHT_TST.NE.OTHERH))THEN
                        ILOOPHITOK=(FTLUSE(IGROUP,OTHERL).NE.
     1                       IHITGR(IGROUP,OTHERL,OTHHT_TST))
                        OTHHT_TST=SAMEAS(IGROUP,OTHERL,OTHHT_TST)
                        GO TO 227
                     ENDIF
                  ENDIF
                  IF(ILOOPHITOK)THEN

C If either the virtually same hit is not the hit used in the fit
C or this is not an FTLUSE layer, then....
C if the virtually same hit is already part of a string it is preferred.
C In this case do not consider the hit.
C Condition is now simply tested with ISOHGR =4 bit which is turned off
C when a virtually same hit is added to the fit
C This stops artificials stops/starts of strings when two hits are close.

C Require a previous hit (with drift sign - ) is ISOLATED on the -side. 
C Require a previous hit (with drift sign + ) is ISOLATED on the +side.
C Require that all hits are open to use as the EARLY part of a string.

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                   IF(IADRHT(IHITGR(IGROUP,OTHERL,OTHERH)).LE.0)THEN
                    ILOOPHITOK=(IAND(ISOHGR(IGROUP,OTHERL,OTHERH),
     3                                5) .EQ. 5 )
                   ELSE
                    ILOOPHITOK=(IAND(ISOHGR(IGROUP,OTHERL,OTHERH),
     3                                6) .EQ. 6 )
                   ENDIF
                   IF(ILOOPHITOK)THEN

C Require that the number of existing gaps in the inner hit plus
C the number of existing gaps in the outer hit plus
C the current TRYGAP is not above the maximum allowed

                    IF( NGAPGR(IGROUP,LFESGR(IGROUP,OTHERL,OTHERH),
     3                                EFESGR(IGROUP,OTHERL,OTHERH))
     4                 +NGAPGR(IGROUP,IGLAYR,IHCULA)
     5                 +TRYGAP 
     4                 .LE.TOTGAP(IGROUP,TFXS) )THEN

C calculate the difference and save the best
C there is currently no bias toward opposite drift sign

C choose match, ordering by best within the layer

cccccccc             ARESDF=ABS(MISHTG(IGROUP,IGLAYR,IHCULA)
cccccccc     1                 -MISHTG(IGROUP,OTHERL,OTHERH))
  
C choose match, ordering by best within the group
C (it is probably redundant to look at the first element of the outer
C  string when it has been demanded to be "open")
                     ARESDF=ABS(MISSTG(IGROUP,
     3                                 LFESGR(IGROUP,IGLAYR,IHCULA),
     4                                 EFESGR(IGROUP,IGLAYR,IHCULA))
     5                         -MISSTG(IGROUP,
     6                                 LFESGR(IGROUP,OTHERL,OTHERH),
     7                                 EFESGR(IGROUP,OTHERL,OTHERH)))

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                     IF(ULEFSC(IGROUP,TFXS))THEN
                        PRODEFF=
     1                       SV_HYBRID_EFF(SV_HYBRID(IADRHT(
     2                       IHITGR(IGROUP,IGLAYR,IHCULA))),TFXS)*
     1                       SV_HYBRID_EFF(SV_HYBRID(IADRHT(
     2                       IHITGR(IGROUP,OTHERL,OTHERH))),TFXS)
                        
                        IF(    PRODEFF.GE.HPRDEF(IGROUP,TFXS))THEN
                           ARESDFLIM=CHRDHE(IGROUP,TFXS)
                        ELSEIF(PRODEFF.LE.LPRDEF(IGROUP,TFXS))THEN
                           ARESDFLIM=CHRDLE(IGROUP,TFXS)
                        ELSE
                           ARESDFLIM=
     1                          CHRDLE(IGROUP,TFXS)
     2                          +(CHRDHE(IGROUP,TFXS)
     2                          -CHRDLE(IGROUP,TFXS))
     3                          *(PRODEFF            
     3                          -CHRDLE(IGROUP,TFXS))
     4                          /(HPRDEF(IGROUP,TFXS)
     4                          -LPRDEF(IGROUP,TFXS))
                        ENDIF
                     ELSE
                        ARESDFLIM=CHRDHE(IGROUP,TFXS)
                     ENDIF

C this line overides (or would overide) the above calculation !!
C                     ARESDFLIM=CHRDHE(IGROUP,TFXS)

#if defined(CLEO_TFXFDIAG)
                     IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                     IP2=PRODEFF*100
                     IP3=ARESDFLIM*1000000
                     CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2                    'eff_product, res_dif_limit=')
C                          123456789012345678901234567890
#endif

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

   
#if defined(CLEO_TFXFDIAG)
                     IF(
     3                    (BESTHM_L.EQ.0).OR.
     4                    (ARESDF.LE.DIFMIN_L))THEN
                      DIFMIN_L=ARESDF
                      BESTHM_L=IHCULA
                      BESTHO_L=OTHERH
                      BESTLM_L=IGLAYR
                      BESTLO_L=OTHERL
                     ENDIF
#endif

                     IF(
     1                    (ARESDF.LE.RESUSE).AND.
     2                    (ARESDF.LE.ARESDFLIM).AND.
     3                    ((BESTHM.EQ.0).OR.
     4                    (ARESDF.LE.DIFMIN)))THEN
                      DIFMIN=ARESDF
                      BESTHM=IHCULA
                      BESTHO=OTHERH
                      BESTLM=IGLAYR
                      BESTLO=OTHERL
                     ENDIF

C end of condition on total number of gaps
                    ENDIF
C end of condition that inner hit is OK (second)
#if defined(CLEO_TFXFDIAG)
                   ELSE
                      IP1=IHITGR(IGROUP,OTHERL,OTHERH)
                      IP2=IHITGR(IGROUP,IGLAYR,IHCULA)
                      CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,0,
     2                     'fail inner loop ISOLATE/OPEN')
C                           123456789012345678901234567890
#endif
                   ENDIF
C end of condition that inner hit is OK (first)
#if defined(CLEO_TFXFDIAG)
                  ELSE
                     IP1=IHITGR(IGROUP,OTHERL,OTHERH)
                     IP2=IHITGR(IGROUP,IGLAYR,IHCULA)
                     CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,0,
     2                    'fail inner loop FTLUSE/SAMEAS')
C                          123456789012345678901234567890
#endif
                  ENDIF
C end of inner hit loop
 257             CONTINUE
C end of condition on number of gap in the outer hit
                ENDIF
C end of condition that outer hit is OK (second)
#if defined(CLEO_TFXFDIAG)
               ELSE
                  IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                  CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,,NBESTM,TRYGAP,
     2                 'failOUTloopISOLATE/OPEN, NB,G=')
C                       123456789012345678901234567890
#endif
               ENDIF
C end of condition that outer hit is OK (first)
#if defined(CLEO_TFXFDIAG)
              ELSE
                 IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                 CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,0,0,
     2                'fail OUTer loop FTLUSE/SAMEAS')
C                      123456789012345678901234567890
#endif
              ENDIF
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

#if defined(CLEO_TFXFDIAG)
              IF(BESTHM_L.NE.0)THEN
                 IP1=IHITGR(IGROUP,BESTLM_L,BESTHM_L)
                 IP2=IHITGR(IGROUP,BESTLO_L,BESTHO_L)
                 CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,BESTLO_L,
     2                'bestmatch_layer,w/hit,loc_layr')
C                      123456789012345678901234567890
                 IP2=DIFMIN_L*1000000
                 IP3=RESUSE*1000000
                 CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2                'bestmatch_layer, res_dif, cut')
C                      123456789012345678901234567890
              ENDIF
#endif

C end of outer hit loop
 261         CONTINUE


C end of condition that inner hit loop layer has hits
            ENDIF
C end of condition that outer hit loop layer has hits
           ENDIF
C end of loop over layer
 267      CONTINUE

C now install the best correlation; 
C increase gap and eventually end of layer if there is none

          IF(BESTHM.NE.0)THEN
             NBESTM=NBESTM+1

#if defined(CLEO_TFXFDIAG)
             IP1=IHITGR(IGROUP,BESTLM,BESTHM)
             IP2=IHITGR(IGROUP,BESTLO,BESTHO)
             IP3=DIFMIN*1000000
             CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2            'bestmatch_GROUP..w/hit,diff')
C                  123456789012345678901234567890
#endif

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C create the link

             NXLRGR(IGROUP,BESTLO,BESTHO)=BESTLM
             NXHTGR(IGROUP,BESTLO,BESTHO)=BESTHM

C calculate the average miss

             MISSTG(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     2                     EFESGR(IGROUP,BESTLO,BESTHO)) =
     3      (MISSTG(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     4                     EFESGR(IGROUP,BESTLO,BESTHO)) *
     5       LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     6                     EFESGR(IGROUP,BESTLO,BESTHO)) + 
     7       MISSTG(IGROUP,BESTLM,BESTHM)                *
     8       LNHTGR(IGROUP,BESTLM,BESTHM))               /
     9      (LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     1                     EFESGR(IGROUP,BESTLO,BESTHO)) + 
     2       LNHTGR(IGROUP,BESTLM,BESTHM))

C adjust length and gap of the early string to include later string
C and update current value of maximum string length

             LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     2                     EFESGR(IGROUP,BESTLO,BESTHO)) =
     3       LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     4                     EFESGR(IGROUP,BESTLO,BESTHO)) +
     5       LNHTGR(IGROUP,BESTLM,BESTHM)
             IF(LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     2                        EFESGR(IGROUP,BESTLO,BESTHO)).GT.MAXLNH)
     3          MAXLNH=
     4          LNHTGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     5                        EFESGR(IGROUP,BESTLO,BESTHO))
             NGAPGR(   IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     2                  EFESGR(IGROUP,BESTLO,BESTHO)) =
     3       NGAPGR(IGROUP,LFESGR(IGROUP,BESTLO,BESTHO),
     4                     EFESGR(IGROUP,BESTLO,BESTHO)) +
     5       NGAPGR(IGROUP,BESTLM,BESTHM)

C set the first element pointers for the first element of the later string
C if the later string is not trivial,
C loop over the later string to set the 
             OTHERL=BESTLM
             OTHERH=BESTHM
 273         CONTINUE
             IF(OTHERH.NE.0)THEN
              LFESGR(IGROUP,OTHERL,OTHERH)=LFESGR(IGROUP,BESTLO,BESTHO)
              EFESGR(IGROUP,OTHERL,OTHERH)=EFESGR(IGROUP,BESTLO,BESTHO)
              LSTLYR=OTHERL
              OTHERL=NXLRGR(IGROUP,LSTLYR,OTHERH)
              OTHERH=NXHTGR(IGROUP,LSTLYR,OTHERH)
              GO TO 273
             ENDIF

C reset LNHTGR for the first element of the later string

             LNHTGR(IGROUP,BESTLM,BESTHM)=1

C .not. the =8 bit; this outer element is not open for late part of string

             ISOHGR(IGROUP,BESTLM,BESTHM)=
     2            IAND(
     3            ISOHGR(IGROUP,BESTLM,BESTHM),
     4            55 )

C .not. the =4 bit; this inner element is not open for early part of string

             ISOHGR(IGROUP,BESTLO,BESTHO)=
     2            IAND(
     3            ISOHGR(IGROUP,BESTLO,BESTHO),
     4            59 )

C .not. both the =4 and the =8 bits of any virtually same hit of
C either inner or outer element to eliminate artificial stops/starts
C now loop over the SAMEAS link list to modify each element

             OTHHT_1ST=SAMEAS(IGROUP,BESTLO,BESTHO)
             IF(OTHHT_1ST.NE.0)THEN
                 OTHHT_TST=OTHHT_1ST
 275             CONTINUE
                 IF(OTHHT_TST.NE.BESTHO)THEN
                    ISOHGR(IGROUP,BESTLO,OTHHT_TST)=
     2                   IAND(ISOHGR(IGROUP,BESTLO,OTHHT_TST),51)
                    OTHHT_TST=SAMEAS(IGROUP,BESTLO,OTHHT_TST)
                    GO TO 275
                 ENDIF
              ENDIF

             OTHHT_1ST=SAMEAS(IGROUP,BESTLM,BESTHM)
             IF(OTHHT_1ST.NE.0)THEN
                 OTHHT_TST=OTHHT_1ST
 277             CONTINUE
                 IF(OTHHT_TST.NE.BESTHM)THEN
                    ISOHGR(IGROUP,BESTLM,OTHHT_TST)=
     2                   IAND(ISOHGR(IGROUP,BESTLM,OTHHT_TST),51)
                    OTHHT_TST=SAMEAS(IGROUP,BESTLM,OTHHT_TST)
                    GO TO 277
                 ENDIF
              ENDIF

C fresh start search for a best correlation
             GO TO 211
C end of condition that a best match was found
          ENDIF
C end loop over gap size
 285     CONTINUE

C end of loop over group
       ENDIF
 289  CONTINUE

C find the best string of hits
C require a minimum of BSNHMN hits in string,
C and a maximum miss distance of 0.0045 for the first pass (IPASS=1:MPASSL),
C then lower to 0.0008 because allignment is 0.0006 in allignment
C to the best string (in the forth loop)
C also, check if the wrong hit in an LUSETF layer is in string
C third loop through the groups

      MISSTM=BSRSRD(IPASS,TFXS)

      BESTGR=0
      DO 389 IGROUP=1,NGRUP
         GROUPREQ=(  (NLAYGR(IGROUP,TFXS).GE.2)
     2        .AND. ((IPASS.EQ.PASS1G(IGROUP,TFXS))
     3         .OR.  (IPASS.GE.PASSAG(IGROUP,TFXS))))
         IF(GROUPREQ)THEN
            DO 387 IGLAYR=1,NLAYGR(IGROUP,TFXS)
               IF(NHITGR(IGROUP,IGLAYR).GE.1)THEN
                  DO 385 IHCULA=1,NHITGR(IGROUP,IGLAYR)

C the rest of the third loop is for strings only; not single hits

                     IF(LNHTGR(IGROUP,IGLAYR,IHCULA).GE.2)THEN

C check if this string includes the wrong hit of an LUSETF layer
C if it does, turn off =32 bit to not use this group in fitting

                        OTHERL=IGLAYR
                        OTHERH=IHCULA

 331                    CONTINUE
                        IF(OTHERH.NE.0)THEN

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C take an opportunity to check for impossibilities in the data structure

                           IF(  (OTHERL.LE.NLAYGR(IGROUP,TFXS))
     2                     .AND.(IHITGR(IGROUP,OTHERL,OTHERH).GT.0)
     3                     .AND.(LFESGR(IGROUP,OTHERL,OTHERH).EQ.IGLAYR)
     4                     .AND.(EFESGR(IGROUP,OTHERL,OTHERH).EQ.IHCULA)
     5                       )THEN

                              IF(   (FTLUSE(IGROUP,OTHERL).GT.0)
     2                         .AND.(FTLUSE(IGROUP,OTHERL).NE.
     3                               IHITGR(IGROUP,OTHERL,OTHERH)))THEN
                                 ISOHGR(IGROUP,IGLAYR,IHCULA)=
     2                           IAND(
     3                           ISOHGR(IGROUP,IGLAYR,IHCULA),
     4                           31 ) 
                              ENDIF
                              
                              LSTLYR=OTHERL
                              OTHERL=NXLRGR(IGROUP,LSTLYR,OTHERH)
                              OTHERH=NXHTGR(IGROUP,LSTLYR,OTHERH)
                              GO TO 331

                           ELSE
C print error messages for errors in the data structure
                              WRITE(CHMESS,2001)IGROUP
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2002)IGLAYR
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2003)IHCULA
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2004)OTHERL
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2005)OTHERH
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2006)NLAYGR(IGROUP,TFXS)
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2007)
     2                              IHITGR(IGROUP,OTHERL,OTHERH)
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2008)
     2                              LFESGR(IGROUP,OTHERL,OTHERH)
                              CALL MESLOG('TFXFIT',1,MSERRO)
                              WRITE(CHMESS,2009)
     2                              EFESGR(IGROUP,OTHERL,OTHERH)
                              CALL MESLOG('TFXFIT',1,MSERRO)
 2001 FORMAT('*SOMETHING IS TERRIBLY WRONG! * IGROUP=',I10)
 2002 FORMAT('******************************* IGLAYR=',I10)
 2003 FORMAT('******************************* IHCULA=',I10)
 2004 FORMAT('******************************* OTHERL=',I10)
 2005 FORMAT('******************************* OTHERH=',I10)
 2006 FORMAT('****************** nlaygr(igroup,tfxs)=',I10,
     1       ' should be .ge.otherl')
2007  FORMAT('********* ihitgr(igroup,otherl,otherh)=',I10,
     1       ' should be .ne. 0')
2008  FORMAT('**********lfesgr(igroup,otherl,otherh)=',I10,
     1       ' should be .eq. iglayr')
2009  FORMAT('**********efesgr(igroup,otherl,otherh)=',I10,
     1       ' should be .eq. ihcula')

#if defined(CLEO_TFXFDIAG)
#if !defined(CLEO_XTSUBS)
                              READ 1002,IGO
 1002                         FORMAT(A1)
#endif
#if defined(CLEO_XTSUBS)
                              CALL XTS_RFV_INPUT
#endif
#endif

                           ENDIF
                        ENDIF

C now find the best string of hits
C absolute cuts...........
C require 2 hits in the string
C require that it not include the wrong hit of an LUSETF layer
C require that the miss distance is better than .0045 (first) or .0008

C allow a string that includes a bad LUSETF hit to be the best
C then the hits in that string will be killed.  if this is the only string
C and it were not allowed to be the best, the hits would not be killed
C ie,leave out.. IF(IAND( 32 ,ISOHGR(IGROUP,IGLAYR,IHCULA)).NE. 32 )..FALSE

C relative cuts.........
C granted: this stuff is still order dependent
C        : if there are 3 strings that do not allign,
C        : some of this taken out by requiring not string 2 shorter than MAXLNH

                        IF   (LNHTGR(IGROUP,IGLAYR,IHCULA).LT.
     2                        BSNHMN(IGROUP,TFXS)    )THEN
                           SETBESTGR=.FALSE.
#if defined(CLEO_TFXFDIAG)
                           IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                           IP2=LNHTGR(IGROUP,IGLAYR,IHCULA)
                           IP3=BSNHMN(IGROUP,TFXS)
                           CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2                          'NOT "best" group... length,cut')
C                                123456789012345678901234567890
#endif
                        ELSEIF(ABS(MISSTG(IGROUP,IGLAYR,IHCULA)).GT.
     2                             MISSTM)THEN
                           SETBESTGR=.FALSE.
#if defined(CLEO_TFXFDIAG)
                           IP1=IHITGR(IGROUP,IGLAYR,IHCULA)
                           IP2=MISSTG(IGROUP,IGLAYR,IHCULA)*1000000
                           IP3=MISSTM*1000000
                           CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2                          'NOT "best" group..ave miss,cut')
C                                123456789012345678901234567890
#endif
                        ELSE
                           OUTLA=LLAYGR(IGROUP,TFXS)
     2                          +LNHTGR(IGROUP,IGLAYR,IHCULA)-1

                           IF(BESTGR.EQ.0)THEN
                              SETBESTGR=.TRUE.
                           ELSE
                              LENGTHDIF=LNHTGR(IGROUP,IGLAYR,IHCULA)
     2                                 -BSTLNG
                              MISSTGDIF=ABS(
     2                                  MISSTG(IGROUP,IGLAYR,IHCULA))
     2                                 -RESMAX

C length better by 2, always accept it

                              IF(LENGTHDIF.GE. 2)THEN
                                 SETBESTGR=.TRUE.

C length better by 1, allow resolution worse by .0008

                              ELSEIF(LENGTHDIF.GE. 1)THEN
                                 IF(MISSTGDIF.LT. SRDFL1(TFXS))THEN
                                    SETBESTGR=.TRUE.
                                 ELSE
                                    SETBESTGR=.FALSE.
                                 ENDIF

C length is the same, decide on resolution if different by 0.0003
C  or if resolution is also similar, use the outer
 
                              ELSEIF(LENGTHDIF.GE. 0)THEN
                                 IF(   MISSTGDIF.LT.-SRDFL0(TFXS))THEN
                                    SETBESTGR=.TRUE.
                                 ELSEIF(MISSTGDIF.GT.SRDFL0(TFXS))THEN
                                    SETBESTGR=.FALSE.
                                 ELSE
                                    IF((OUTLA-BOUTLA).GT.0)THEN
                                       SETBESTGR=.TRUE.
                                    ELSE
                                       SETBESTGR=.FALSE.
                                    ENDIF
                                 ENDIF

C length is worse by 1, use if resolution is better by .0008

                              ELSEIF(LENGTHDIF.GE.-1)THEN
                                 IF(   MISSTGDIF.GT.-SRDFL1(TFXS))THEN
                                    SETBESTGR=.FALSE.
                                 ELSEIF((LNHTGR(IGROUP,IGLAYR,IHCULA)
     2                                  -MAXLNH).LE.-2)THEN
                                    SETBESTGR=.FALSE.
                                 ELSE
                                    SETBESTGR=.TRUE.
                                 ENDIF

C length is worse by 2, never accept it

                              ELSEIF(LENGTHDIF.LE.-2)THEN
                                 SETBESTGR=.FALSE.
                                 
                              ENDIF
                           ENDIF
                        ENDIF
                        IF(SETBESTGR)THEN
                           BESTGR=IGROUP
                           BESTLA=IGLAYR
                           BESTHM=IHCULA
                           RESMAX=ABS(MISSTG(IGROUP,IGLAYR,IHCULA))
                           BSTRES=    MISSTG(IGROUP,IGLAYR,IHCULA)
                           BSTLNG=    LNHTGR(IGROUP,IGLAYR,IHCULA)
                           BOUTLA=OUTLA
                        ENDIF
                     ENDIF
 385              CONTINUE
               ENDIF
 387        CONTINUE
         ENDIF
 389  CONTINUE

C find other strings consistant with the best string
C add hits to DUET arrays, even for best string
C require strings within .0006 of the best
C fourth loop through the groups

      IF(BESTGR.GT.0)THEN
       DOBEST=1
 411   CONTINUE
       DO 489 IGROUP=1,NGRUP
        GROUPREQ=(  (NLAYGR(IGROUP,TFXS).GE.2)
     2        .AND. ((IPASS.EQ.PASS1G(IGROUP,TFXS))
     3         .OR.  (IPASS.GE.PASSAG(IGROUP,TFXS))))
        IF(GROUPREQ)THEN
         DO 487 IGLAYR=1,NLAYGR(IGROUP,TFXS)
          IF(NHITGR(IGROUP,IGLAYR).GT.0)THEN
           DO 485 IHCULA=1,NHITGR(IGROUP,IGLAYR)
            IF(LNHTGR(IGROUP,IGLAYR,IHCULA).GE.2)THEN
             ADDGROUP=.FALSE.
             IF(   (IGROUP.EQ.BESTGR)
     2        .AND.(IGLAYR.EQ.BESTLA)
     3        .AND.(IHCULA.EQ.BESTHM))THEN

              IF(DOBEST.EQ.1)THEN
               ADDGROUP=.TRUE.

#if defined(CLEO_TFXFDIAG)
               GRPSTA(IGROUP,IGLAYR,IHCULA)=3
#endif
              ENDIF

C accept groups, other than the best group, in a road
C now use road relative to "best" only for IPASS</=MPASSL
C use simple road about zero, of size MISSTM, for later passes
             ELSE
              IF(DOBEST.EQ.0)THEN
               ARESDF=ABS(MISSTG(IGROUP,IGLAYR,IHCULA)-BSTRES)
               IF(IPASS.LE.MPASSL(TFXS))THEN
                IF(ARESDF.LT.OSRDRD(IGROUP,TFXS))
     2                                           ADDGROUP=.TRUE.
               ELSE
                IF(ABS(MISSTG(IGROUP,IGLAYR,IHCULA)).LE. MISSTM)
     2                                           ADDGROUP=.TRUE.
               ENDIF

               IF(ADDGROUP)THEN

#if defined(CLEO_TFXFDIAG)
                GRPSTA(IGROUP,IGLAYR,IHCULA)=2
#endif
               ELSE
                              
#if defined(CLEO_TFXFDIAG)
                GRPSTA(IGROUP,IGLAYR,IHCULA)=1
#endif
                IF(.NOT.IGNORD)THEN
                 IF(LNHTGR(IGROUP,IGLAYR,IHCULA).GE.IGSSLN(TFXS))
     2                                           IGNORD=.TRUE.
                ENDIF
               ENDIF

C end the DOBEST.EQ.0 test
              ENDIF
C end the best group test
             ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C add hits
C loop over the layers/hits in the group

             IF(ADDGROUP)THEN
              OTHERL=IGLAYR
              OTHERH=IHCULA

C branch back point for new layer
 452          CONTINUE
              IF(OTHERH.NE.0)THEN
               OTHRHS=OTHERH
               PROCSAME=.FALSE.
               XLYR=LLAYGR(IGROUP,TFXS)+OTHERL-1
               TFLYR=XLYR+L0TFXF(TFXS)

C branch back point for SAMEAS hits
 453           CONTINUE

C add hits from LUSETF layers if there are none already (allow FTLUSE=0)
               IF(.NOT.PROCSAME)THEN

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',
     2                 IHITGR(IGROUP,OTHERL,OTHERH),
     3                 N_RETURN,0,0,
     4                 'processing pricipal hit')
C                       123456789012345678901234567890
#endif

                IF(FTLUSE(IGROUP,OTHERL).GT.0)THEN

C layer contains a hit in an LUSETF layer
                 IF(FTLUSE(IGROUP,OTHERL).EQ.
     2               IHITGR(IGROUP,OTHERL,OTHERH))THEN

C layer contains the right hit in an LUSETF layer
                  ADDHIT=.FALSE.
                  ADDSAMEAS=.TRUE.
                  HTCOMP=OTHERH

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',
     2                 IHITGR(IGROUP,OTHERL,OTHERH),
     3                 N_RETURN,0,0,
     4                 'right hit in LUSETF layer')
                  HITSTA(IGROUP,OTHERL,OTHERH)=5
#endif

C layer contains the wrong hit in an LUSETF layer
                 ELSE
                  ADDHIT=.FALSE.
                  ADDSAMEAS=.FALSE.

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',
     2                 IHITGR(IGROUP,OTHERL,OTHERH),
     3                 N_RETURN,0,0,
     4                 'WRONG hit in LUSETF layer')
                  HITSTA(IGROUP,OTHERL,OTHERH)=4
#endif
C end the test if the previous hit in the LUSETF layer is the correct one
                 ENDIF

C layer does not contain any hit from an LUSETF layer
                ELSE

C ----------------blocking by layer----------------------------
                 IF(LYRUSE(XLYR).GE.1)THEN

                  ADDHIT=.FALSE.
                  ADDSAMEAS=.FALSE.
#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',
     2                 IHITGR(IGROUP,OTHERL,OTHERH),
     3                 N_RETURN,LYRUSE(XLYR),0,
     4                 'not hit-LUSETF-layer, "BLOCK"')
                  HITSTA(IGROUP,OTHERL,OTHERH)=1
#endif
                   
C hit is not blocked
                 ELSE
                  ADDHIT=.TRUE.
                  ADDSAMEAS=.TRUE.
                  HTCOMP=OTHERH

#if defined(CLEO_TFXFDIAG)
                  CALL TRACE_TFXFIT('LOG',
     2                 IHITGR(IGROUP,OTHERL,OTHERH),
     3                 N_RETURN,LYRUSE(XLYR),0,
     4                 'not hit-LUSETF-layer, no block')
#endif
                 ENDIF

C end of test if FTLUSE layer
                ENDIF

C process sameas hit
               ELSE
                ADDHIT=.TRUE.
                ADDSAMEAS=.TRUE.

#if defined(CLEO_TFXFDIAG)
                CALL TRACE_TFXFIT('LOG',
     2               IHITGR(IGROUP,OTHERL,OTHERH),
     3               N_RETURN,
     4               IHITGR(IGROUP,OTHERL,HTCOMP),0,
     5               'processing SAMEAS hit, COMP=')
C                     123456789012345678901234567890
#endif

C end of test of procressing SAMEAS hit
               ENDIF

C further tests
               IF(ADDHIT)THEN

C check the =32 bit to
C check if this ___GROUP___ includes the wrong hit of an LUSETF layer
C if this is the case, flag the layer used internally:
C will not even be picked up in the STANDARD TFXFIT
                IF(IAND( 
     2                 ISOHGR(IGROUP,IGLAYR,IHCULA),
     3                 32).NE. 32 )THEN
                 LYRUSE(XLYR)=3
                 HITUSE(IHITGR(IGROUP,OTHERL,OTHERH))=3
                 ADDHIT=.FALSE.
                 ADDSAMEAS=.FALSE.

#if defined(CLEO_TFXFDIAG)
                 CALL TRACE_TFXFIT('LOG',
     2                IHITGR(IGROUP,OTHERL,OTHERH),
     3                N_RETURN,0,0,
     4                'group inc wrong hit LUSETF lay')
                 GRPSTA(IGROUP,IGLAYR,IHCULA)=-1
                 HITSTA(IGROUP,OTHERL,OTHERH)=3
#endif


C check residual of ___HIT___ against the average residual of string
C do not add hit and kill the =16 bit if it is bad resolution
C flag layer used internally,
C will not even be picked up in STANDARD TFXFIT
C !!! but what if ->MISHTG(IGROUP,OTHERL,OTHERH)<- is close to ->MISSTM<-
C !!! NOTHING IS BEING DONE AT THIS TIME

                ELSEIF(ABS(MISHTG(IGROUP,OTHERL,OTHERH)
     3                    -MISSTG(IGROUP,IGLAYR,IHCULA))
     4                     .GE. WSRDRD(IGROUP,TFXS))THEN
                 ISOHGR(IGROUP,OTHERL,OTHERH)=
     2                  IAND(
     3                  ISOHGR(IGROUP,OTHERL,OTHERH),
     4                  47 )
                 LYRUSE(XLYR)=2
                 HITUSE(IHITGR(IGROUP,OTHERL,OTHERH))=2

                 ADDHIT=.FALSE.
                 IF(.NOT.PROCSAME)ADDSAMEAS=.FALSE.

#if defined(CLEO_TFXFDIAG)
                 IP1=IHITGR(IGROUP,OTHERL,OTHERH)
                 IP2=(MISHTG(IGROUP,OTHERL,OTHERH)
     2               -MISSTG(IGROUP,IGLAYR,IHCULA))*1000000
                 IP3=WSRDRD(IGROUP,TFXS)*1000000
                 CALL TRACE_TFXFIT('LOG',IP1,N_RETURN,IP2,IP3,
     2                'remove hit........rel miss,cut')
                 HITSTA(IGROUP,OTHERL,OTHERH)=2
#endif
                ENDIF
C end of further tests
               ENDIF


C add the hit
               IF(
     1              (ADDHIT).AND.
     2              (HITUSE(IHITGR(IGROUP,OTHERL,OTHERH)).EQ.0))THEN

                LYRUSE(XLYR) = 5
                HITUSE(IHITGR(IGROUP,OTHERL,OTHERH))=5

                CALL HIT_TO_FIT(H2F_NEW_ENTRY,
     2               IHITGR(IGROUP,OTHERL,OTHERH),0)

                IF(REDUCD(IGROUP,OTHERL,OTHERH).NE.0)THEN
                   CALL HIT_TO_FIT(H2F_DEWEIGHT,
     2                  DEWRDR(IGROUP,TFXS),NFIT)
                   CALL HIT_TO_FIT(H2F_CHANGE_P_I,
     2                  P_I_TFXFIT_REDUCE,NFIT)
                ENDIF

#if defined(CLEO_TFXFDIAG)
                CALL TRACE_TFXFIT('LOG',
     2               IHITGR(IGROUP,OTHERL,OTHERH),
     3               N_RETURN,0,0,
     4               'ADD hit')
                HITSTA(IGROUP,OTHERL,OTHERH)=6
#endif

C end the "add hit"
               ENDIF

C try another hit in same layer that is essentially identical
               IF(ADDSAMEAS.AND.MULTIH(IGROUP,TFXS))THEN
 461            OTHERH=SAMEAS(IGROUP,OTHERL,OTHERH)
                IF((OTHERH.NE.0).AND.(OTHERH.NE.OTHRHS))THEN

C compare to all used hits in SAMEAS link list
C    as long as the device dependent test has not failed and
C    that the hit is not the candidate sameas hit and
C    that the hit is in the fit list
C then make device dependent tests
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                 PROCSAME=.TRUE.
                 OTHHT_1ST=HTCOMP
                 OTHHT_TST=OTHHT_1ST
 463             CONTINUE
                 IF(
     1                PROCSAME.AND.
     2                (OTHHT_TST.NE.OTHERH).AND.
     3                (
     4                (HITUSE(IHITGR(IGROUP,OTHERL,OTHHT_TST)).EQ.1).OR.
     5                (HITUSE(IHITGR(IGROUP,OTHERL,OTHHT_TST)).EQ.4).OR.
     6                (HITUSE(IHITGR(IGROUP,OTHERL,OTHHT_TST)).EQ.5)
     7                )
     8                )THEN

C 5/2/96 ADF Only accept as corner hits if they come from different wafers.
C 4/30/96 ADF	"I can't believe I'm using a goto."

                  IF(
     1                   (IDVCTF(TFLYR).EQ.ISVX1).OR.
     2                   (IDVCTF(TFLYR).EQ.ISVZ1))THEN
                   IF(
     1                    SV_HYBRID(IADRHT(
     2                    IHITGR(IGROUP,OTHERL,OTHHT_TST))).EQ.
     2                    SV_HYBRID(IADRHT(
     3                    IHITGR(IGROUP,OTHERL,OTHERH)))
     4                    )PROCSAME=.FALSE.

C drift chamber, require different wires
C this takes care of comparing to wires that are filled in the current
C entry of TFXFIT while the HITUSE test at "add hit" takes care of
C any overlap with hits outside the current SAMEAS link list

                  ELSEIF(IDVCTF(TFLYR).EQ.IDRFT)THEN
                   IF(
     1                    IABS(IADRHT(
     2                    IHITGR(IGROUP,OTHERL,OTHHT_TST))).EQ.
     3                    IABS(IADRHT(
     4                    IHITGR(IGROUP,OTHERL,OTHERH)))
     5                    )PROCSAME=.FALSE.

C other devices, no test
                  ELSE
C end of device selection
                  ENDIF
C end of device independent test hit selection
                 ENDIF

C end of TEST loop over SAMEAS link list
                 OTHHT_TST=SAMEAS(IGROUP,OTHERL,OTHHT_TST)
                 IF(OTHHT_TST.NE.OTHHT_1ST)GO TO 463

C process hit or get the next one
                 IF(PROCSAME)GO TO 453
                 GO TO 461

C end of test that SAMEAS link list is not exhausted
                ENDIF
C end the ADDSAMEAS test
               ENDIF

C go to the next layer
               LSTLYR=OTHERL
               OTHERL=NXLRGR(IGROUP,LSTLYR,OTHRHS)
               OTHERH=NXHTGR(IGROUP,LSTLYR,OTHRHS)
               GO TO 452
                                 
C end the OTHERH (for the primary hit) existance test
              ENDIF
C end the ADDGROUP test
             ENDIF
C end of test that hit is not trivial
            ENDIF
C end the loop over hit in layer
 485       CONTINUE
C end of test that there are hits in layer
          ENDIF
C end the loop over layer
 487     CONTINUE


C end of group requirements
        ENDIF
C end the loop over group
 489   CONTINUE
         
       IF(DOBEST.EQ.1)THEN
          DOBEST=0
          GO TO 411
       ENDIF

C end of the BESTGR test
      ENDIF

511   CONTINUE


C***********************************************************************
C PREPARE TO FIT, OR LEAVE IF NOTHING ADDED
C***********************************************************************

C********************************
C****** TFXFIT COMMON CODE ******
C********************************

C GET NEW FIT IF SOMETHING ADDED
C ADF Screw with this part of the code to get extra diagnostics
      IF(NFIT.NE.NFITS)THEN
         NOWFIT=.TRUE.
      ELSE
         NOWFIT=.FALSE.
         
#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
         IF(IPASS.GT.MPASSA(TFXS))THEN
            CALL XTS_TRAK('CF',0,'TFXFIT:STANDARD fails')
         ELSE
            CALL XTS_TRAK('CF',0,'TFXFIT:AMBIGUITY fails')
         ENDIF
         IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif

      ENDIF
         
C***********************************************************************
C FIT
C***********************************************************************

      IF(NOWFIT)THEN
         IF(NFIT.GE.NFTMIN)THEN
            CALL FIT_WHATEVER
            
#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
            IF(IPASS.GT.MPASSA(TFXS))THEN
               CALL XTS_TRAK('CF',0,'TFXFIT:after STANDARD refit')
            ELSE
               CALL XTS_TRAK('CF',0,'TFXFIT:after AMBIGUITY refit')
            ENDIF
            IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif
         ELSE
            IERR=1
            GO TO 899
         ENDIF
      ENDIF


C***********************************************************************
C PREPARE TO MAKE ANOTHER PASS
C***********************************************************************

C*****************************
C****** STANDARD TFXFIT ******
C*****************************
      IF(IPASS.GT.MPASSA(TFXS))THEN

         IF(NOWFIT)THEN
C NO NEW PASS IF NOTHING ADDED, OR NOTHING IGNORED
C TEST FOR NOTHING ADDED WAS DONE BEFORE FIT
            IF(.NOT.IGNORD)GO TO 589
C ABSOLUTE LIMIT
            IF(IPASS.GE.MPASSA(TFXS)+MPASSS(TFXS))GO TO 589
            IPASS=IPASS+1
            GO TO 115
         ENDIF
      ENDIF

C************************************
C****** LOCAL AMBIGUITY TFXFIT ******
C************************************
      IF(IPASS.LE.MPASSA(TFXS))THEN

C IGNORD WILL BE .FALSE. IF THERE WAS NOTHING FOUND
 583     IPASS=IPASS+1
         IF(IPASS.GT.MPASSA(TFXS))GO TO 115
         IF(SKIPPS(IPASS,TFXS).EQ.0)GO TO 115
         IF(IGNORD)GO TO 115
         GO TO 583
      ENDIF
C********************************
C****** TFXFIT COMMON CODE ******
C********************************
589   CONTINUE

#if defined(CLEO_TFXFDIAG)
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'TFXFIT:exit')
      IF(XTS_ABORT.EQ.1)GO TO 899
#endif
#endif

899   CONTINUE

#if defined(CLEO_XTSUBS)
      XTS_TFXFIT_ON=0
#endif

      RETURN
      END

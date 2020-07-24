* $Id: fast_trak_del_bad_and_add.F,v 1.13 2001/11/19 22:53:57 dpp Exp $
*
* $Log: fast_trak_del_bad_and_add.F,v $
* Revision 1.13  2001/11/19 22:53:57  dpp
*      -> use status bit encapsulation
*      -> call COMPRESS_FIT directly
*
* Revision 1.12  2001/09/14 21:15:39  dpp
*      -> block adding double hits
*      -> use HIT_TO_FIT to remove fit elements
*
* Revision 1.11  2001/09/14 20:56:24  dpp
* some of the repairs
*
* Revision 1.10  2001/09/12 21:55:07  dpp
*      -> comments
*      -> variable name change
*      -> change "DEL_MODE" to use parameter directives
*      -> expand use of program indicator
*
* Revision 1.9  2000/12/19 23:44:03  dpp
*      -> new status bits
*      -> actively insert zeros in status words for compatibility with recall
*
* Revision 1.8  2000/10/24 22:37:26  dpp
*      -> format
*
* Revision 1.7  2000/10/02 20:57:37  dpp
*      -> new argument, controls mode to replace any hit with better one
*      -> require valid hit status
*
* Revision 1.6  2000/08/18 18:45:17  dpp
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.5  2000/04/26 14:37:21  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> use call to RESIDUALS instead of RESICF
*
* Revision 1.4  1999/12/09 21:08:31  dpp
* change call to hit_to_fit to use parameter directive
* zero the third argument to HIT_TO_FIT(H2F_NEW_ENTRY...
* call HIT_TO_FIT(..DEWEIGHT..) instead of applying deweight locally
*
* Revision 1.3  1999/11/29 22:14:03  dpp
*  set UPTODATE_HIT=.FALSE. when dropping hits
*
* Revision 1.2  1999/11/23 23:25:15  dpp
* set UPTODATE_HIT=.FALSE. when adding hits
*
* Revision 1.1  1999/11/03 22:43:34  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_DEL_BAD_AND_ADD(DEL_MODE,ROAD_MODE)
C.......................................................................
C.
C. FAST_TRAK_DEL_BAD_AND_ADD - delete hits with bad residules
C.                             and replace with hits with good residuals
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split off of FAST_TRAK 27-Oct-99
C. LAST MOD  : 
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C argument
C-----------------------------------------------------------------------

* DEL_MODE.............delete mode: either FT_DEL_BAD
*                                       or FT_NO_DELETE
                       INTEGER DEL_MODE

* ROAD_MODE............road mode: either FT_USE_CELL_FRAC
*                                     or FT_USE_SIGMAS 
                       INTEGER ROAD_MODE

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

* INDX_HIT.............counter of hit number
                       INTEGER INDX_HIT
* LAYR_COUNTR..........serial layer counter
                       INTEGER LAYR_COUNTR
* IFIT.................index for fit list
                       INTEGER IFIT

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

* IHITSV...............save the hit number that is to be fitted
                       INTEGER IHITSV
* IHITRET..............hit number returned
                       INTEGER IHITRET
* TF_LAYR..............layer number found
                       INTEGER TF_LAYR

C-----------------------------------------------------------------------
C other
C-----------------------------------------------------------------------

* MESLOC...............message giving location in program
                       CHARACTER*12 MESLOC

* COMMNT...............comment
                       CHARACTER*10 COMMNT

C-----------------------------------------------------------------------
C LOCAL VARIABLES FOR THE HIT_DROPPING/ADDING_OTHER  SECTION
C-----------------------------------------------------------------------

* DROP_SOME............=.TRUE. if anything is dropped
                       LOGICAL DROP_SOME
* NDROP_REFIT...........number ( out of NFIT) of problem hits dropped
*                      (NDROP_REFIT is updated at the time of the refit)
*                      (NFIT is not reduced until the compress)
                       INTEGER NDROP_REFIT
* NDROP_NOT_REFIT......number of hits marked to drop since the 
*                      last call to the fitter
                       INTEGER NDROP_NOT_REFIT
* DROP_MORE............=.TRUE. if continue test to drop more hits
                       LOGICAL DROP_MORE
* REFIT_NOW............=.TRUE. if the refit is to be done now
*                      otherwise, wait until dropping more hits
                       LOGICAL REFIT_NOW 
* ENOUGH_HITS..........=.TRUE. if enough hits remain to continue searching
                       LOGICAL ENOUGH_HITS 
* NHITS_IN_VIEW........number of hits remaining in the view
                       INTEGER NHITS_IN_VIEW
* INDX_MAX_PUL.........element number with maximum PULFIT
                       INTEGER INDX_MAX_PUL
* VAL_MAX_PUL..........value of the maximum PULFIT
                       REAL VAL_MAX_PUL
* LYR_MAX_PUL..........layer number for the maximum PULFIT
                       INTEGER LYR_MAX_PUL 
* TESTPULL.............pull or weighted pull of tested fit element
                       REAL TESTPULL
* FIT_IN_VIEW..........=.TRUE. IF FITTED hit is valid for current view: rf,sz
                       LOGICAL FIT_IN_VIEW 
* HIT_IN_VIEW..........=.TRUE. if hit is valid for current view: rf,sz
                       LOGICAL HIT_IN_VIEW 
* FUL_CEL_SIZ..........full width cell size; RTF*CELLTF
                       REAL FUL_CEL_SIZ
* RES_LIMIT_1..........residual limit to drop hits
                       REAL RES_LIMIT_1
* RES_LIMIT_2..........residual limit to continue testing to drop more hits
                       REAL RES_LIMIT_2
* MTEMP................temporary variable for displaying cut limit
                       INTEGER MTEMP
* NADDED...............number of hits added for later fit
                       INTEGER NADDED
* RESI_RAW.............the residual; difference of hit position - fit
                       REAL RESI_RAW
* RESI_USE.............residual used for comparing, possibly devided by sigma 
                       REAL RESI_USE
* RESMAX...............maximum  RESI_USE
                       REAL RESMAX
* ADD_REPLACEMENT......add the found replacement hit to the fit
                       LOGICAL ADD_REPLACEMENT

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      PROGRAM_INDICATOR=P_I_F_T_DEL_BAD_AND_ADD
      DROP_SOME=.FALSE.
      
C-----------------------------------------------------------------------
C  remove hits with large residuals
C
C  NOTE: There must be a fresh r-phi fit because hit-to-fit does not
C        update the residuals stored in PULFIT; they may still be left
C        from a previous s-z fit.
C-----------------------------------------------------------------------

      IF(DEL_MODE.EQ.FT_DEL_BAD)THEN
         NDROP_REFIT=0
         NDROP_NOT_REFIT=0
         DROP_MORE=.TRUE.
 511     CONTINUE

         IF(DROP_MORE)THEN
            DROP_MORE=.FALSE.
            REFIT_NOW=.FALSE.

#if defined(CLEO_FTDIAG)
            MTEMP=MAX0(MNHTCF,NHIT_MIN_TRAK)
            PRINT 1500,NFIT,NDROP_REFIT,NDROP_NOT_REFIT,MTEMP
#endif

            ENOUGH_HITS=(NFIT-NDROP_REFIT-NDROP_NOT_REFIT).GT.
     2           MAX0(MNHTCF,NHIT_MIN_TRAK)
            IF(ENOUGH_HITS)THEN

               NHITS_IN_VIEW=0
               VAL_MAX_PUL=0.
               DO 529 IFIT=1,NFIT

                  FIT_IN_VIEW=.FALSE.
                  IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                     IF(STRFFT(IFIT).NE.0)FIT_IN_VIEW=.TRUE.
                  ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
                     IF(STSZFT(IFIT).NE.0)FIT_IN_VIEW=.TRUE.
                  ENDIF
                  IF(FIT_IN_VIEW)THEN

                     NHITS_IN_VIEW=NHITS_IN_VIEW+1
                     TF_LAYR=IPLFIT(IFIT)
                     IF(ROAD_MODE.EQ.FT_USE_CELL_FRAC)THEN
                        TESTPULL=ABS(PULFIT(IFIT))
                     ELSEIF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                        TESTPULL=ABS(PULFIT(IFIT))/SIGMA_LAYER
     2                       (TF_LAYR,SAV_HIT_TYP,SAV_FIT_TYP)
                     ENDIF
                     IF(TESTPULL.GT.VAL_MAX_PUL)THEN
                        INDX_MAX_PUL=IFIT
                        VAL_MAX_PUL=TESTPULL
                        LYR_MAX_PUL=TF_LAYR
                     ENDIF
                  ENDIF
 529           CONTINUE
               

C end of first condition that track still has not shrunk below the minimum
            ENDIF
            ENOUGH_HITS=ENOUGH_HITS.AND.(NHITS_IN_VIEW.GE.5)
            IF(ENOUGH_HITS)THEN

               FUL_CEL_SIZ=RTF(LYR_MAX_PUL)*CELLTF(LYR_MAX_PUL)

               IF(ROAD_MODE.EQ.FT_USE_CELL_FRAC)THEN
                  RES_LIMIT_1=CELCT1*FUL_CEL_SIZ
               ELSEIF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                  RES_LIMIT_1=20.
               ELSE

#if defined(CLEO_FTDIAG)
                  PRINT 1702,ROAD_MODE
 1702             FORMAT(' FAST_TRAK_DEL_BAD_AND_ADD:',
     2                 ' INVALID ROAD_MODE=',I6)
#if defined(CLEO_XTSUBS)
                  CALL XTS_RFV_INPUT
#endif
#endif

               ENDIF


#if defined(CLEO_FTDIAG)
               PRINT 1501,
     1              LYR_MAX_PUL,
     2              INDX_MAX_PUL,IADFIT(INDX_MAX_PUL),
     3              NFIT,NDROP_REFIT,NDROP_NOT_REFIT,
     4              HITBSC,HITASC,
     5              VAL_MAX_PUL,RES_LIMIT_1
#endif

               IF(VAL_MAX_PUL.GE.RES_LIMIT_1)THEN
                  NDROP_NOT_REFIT=NDROP_NOT_REFIT+1
                  DROP_SOME=.TRUE.
                  CALL HIT_TO_FIT(H2F_REMOVE_ENTRY,
     2                 IHITRET,INDX_MAX_PUL)
C set uptodate=false; otherwise HITTRK is used to recreate hit
C need a routine that does the opposite of HIT_2_FIT 
                  UPTODATE_HIT=.FALSE.
                  IF(RTF(LYR_MAX_PUL).LT.RKN6CF)THEN
                     HITBSC=HITBSC-1
                  ELSE
                     HITASC=HITASC-1
                  ENDIF

C-----------------------------------------------------------------------
C  it can drop more mild problem hits up to an amount: MAX_DROP
C  it can drop even more for larger residuals
C-----------------------------------------------------------------------
            
#if defined(CLEO_FTDIAG)
                  PRINT 1502,
     1                 LYR_MAX_PUL,VAL_MAX_PUL,
     2                 NDROP_REFIT,NDROP_NOT_REFIT,MAX_DROP
#endif

                  IF((NDROP_REFIT+NDROP_NOT_REFIT).LT.MAX_DROP)
     2                 DROP_MORE=.TRUE.

                  IF(ROAD_MODE.EQ.FT_USE_CELL_FRAC)THEN
                     RES_LIMIT_2=CELCT2*FUL_CEL_SIZ
                  ELSEIF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                     RES_LIMIT_2=100.
                  ENDIF
   
#if defined(CLEO_FTDIAG)
                  PRINT 1503,
     1                 LYR_MAX_PUL,VAL_MAX_PUL,RES_LIMIT_2
#endif

C-----------------------------------------------------------------------
C  always refit after dropping mild problem hits
C  for larger residuals might want to drop a bunch at once; 
C    refit after some number (like 3) or for shrt tracks (like length=10)  
C-----------------------------------------------------------------------

                  IF(VAL_MAX_PUL.LT.RES_LIMIT_2)THEN
                     REFIT_NOW=.TRUE.
                  ELSE

                     DROP_MORE=.TRUE.
                     IF(NDROP_NOT_REFIT.GE.DROP_REQ_REFIT)THEN
                        REFIT_NOW=.TRUE.
                     ELSEIF((NFIT-(NDROP_REFIT+NDROP_NOT_REFIT))
     2                       .LE.SHORT_REQ_REFIT)THEN
                        REFIT_NOW=.TRUE.
                     ELSE
                        REFIT_NOW=.FALSE.
                     ENDIF

                  ENDIF

C end of condition that hit is bad enough to drop
               ENDIF

C end of second condition that track still has not shrunk below the minimum
            ENDIF
         
C-----------------------------------------------------------------------
C refit the track when ready
C-----------------------------------------------------------------------
            IF(REFIT_NOW.AND.(NDROP_NOT_REFIT.GT.0))THEN
               NDROP_REFIT=NDROP_REFIT+NDROP_NOT_REFIT
               NDROP_NOT_REFIT=0
         
               IF(.NOT.((KNK6CF.EQ.0).OR.(KNK6CF.EQ.KABG2_CLEAR)))THEN
                  IF(
     1                 (HITBSC.LT.         4).OR.
     2                 (HITASC.LT.         4))THEN
                     KNK6CF=KABG2_CLEAR
                     CALL STATUSBITS(kSTfillcf,kstFTcwKNKdbaa,0)
                  ELSE
                     CALL STATUSBITS(kSTfillcf,kstFTcwKNKdbaa,1)
                  ENDIF
               ELSE
                  CALL STATUSBITS(kSTfillcf,kstFTcwKNKdbaa,0)
               ENDIF

#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
               CALL XTS_TRAK('CF',0,
     2              'FAST_TRAK drop, will refit')
#endif

               CALL FIT_WHATEVER

#if defined(CLEO_FTDIAG)
               PRINT 1003
               MESLOC='DROPHIT FIT '
               PRINT 1504,MESLOC,
     2              KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF,
     3              LYR_MAX_PUL
#if defined(CLEO_XTSUBS)
               CALL XTS_TRAK('CF',0,
     2              'FAST_TRAK, drop hit fit')
#endif
#endif
            ENDIF
            GO TO 511

C end of condition DROP_MORE
         ENDIF

C end of condition DEL_MODE
      ELSEIF(DEL_MODE.EQ.FT_NO_DELETE)THEN
      ELSE
#if defined(CLEO_FTDIAG)
         PRINT 1701,DEL_MODE
 1701    FORMAT(' FAST_TRAK_DEL_BAD_AND_ADD: INVALID DEL_MODE=',I6)
#if defined(CLEO_XTSUBS)
         CALL XTS_RFV_INPUT
#endif
#endif
      ENDIF

C-----------------------------------------------------------------------
C  are there better hits than the ones that were dropped?
C
C  this section to replace any hit with another with better residual
C-----------------------------------------------------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      NADDED=0
      IF((NDROP_REFIT.GT.0).OR.(DEL_MODE.EQ.FT_NO_DELETE))THEN
         DO 669 IFIT = 1, NFIT
            IF((IADFIT(IFIT).EQ.0).OR.(DEL_MODE.EQ.FT_NO_DELETE))THEN

               FIT_IN_VIEW=.FALSE.
               IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                  IF(STRFFT(IFIT).NE.0)FIT_IN_VIEW=.TRUE.
               ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
                  IF(STSZFT(IFIT).NE.0)FIT_IN_VIEW=.TRUE.
               ENDIF
               IF(FIT_IN_VIEW)THEN

                  TF_LAYR=IPLFIT(IFIT)
                  LAYR_COUNTR=ILLRLR(TF_LAYR)
                  FUL_CEL_SIZ=RTF(TF_LAYR)*CELLTF(TF_LAYR)
                  
                  IF(DEL_MODE.EQ.FT_DEL_BAD)THEN
                     IF(ROAD_MODE.EQ.FT_USE_CELL_FRAC)THEN
                        RESMAX=CELCT1*FUL_CEL_SIZ
                     ELSEIF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                        RESMAX=20.
                     ENDIF
                  ELSE
                     RESMAX=ABS(PULFIT(IFIT))
                     IF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                        RESMAX=RESMAX/SIGMA_LAYER
     2                       (TF_LAYR,SAV_HIT_TYP,SAV_FIT_TYP)
                     ENDIF
                  ENDIF

#if defined(CLEO_FTDIAG)
                  PRINT 1601,IFIT,TF_LAYR,LAYR_COUNTR,RESMAX,
     2                 IADFIT(IFIT)
#endif

                  IF(LAYR_COUNTR.NE.0)THEN
                     IHITSV  = 0
                     DO 665 INDX_HIT=
     2                    LHITLR(LAYR_COUNTR),MHITLR(LAYR_COUNTR)
                        
                        HIT_IN_VIEW=.FALSE.
                        IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                           IF(STRFHT(INDX_HIT).NE.0)HIT_IN_VIEW=.TRUE.
                        ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
                           IF(STSZHT(INDX_HIT).NE.0)HIT_IN_VIEW=.TRUE.
                        ENDIF
                        IF(HIT_IN_VIEW)THEN
                           
                           CALL RESIDUALS('HIT',0,INDX_HIT,RESI_RAW)
                           IF(ROAD_MODE.EQ.FT_USE_CELL_FRAC)THEN
                              RESI_USE=RESI_RAW
                           ELSEIF(ROAD_MODE.EQ.FT_USE_SIGMAS)THEN
                              RESI_USE=RESI_RAW/SIGMA_LAYER
     2                             (TF_LAYR,SAV_HIT_TYP,SAV_FIT_TYP)
                           ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                           
                           IF(HITTRK(INDX_HIT).NE.0)THEN
#if defined(CLEO_FTDIAG)
                              COMMNT='IN FIT    '
#endif
                              
                           ELSEIF(
     1                             (IABS(IADRHT(INDX_HIT)).NE.
     1                              IABS(IADFIT(IFIT))).AND.
     2                             (OTHRHT(INDX_HIT).NE.0).AND.
     3                             (HITTRK(OTHRHT(INDX_HIT)).NE.0)
     4                             )THEN
#if defined(CLEO_FTDIAG)
                              COMMNT='other side'
#endif
                           ELSE
                              IF(ABS(RESI_USE).LT.RESMAX)THEN
                                 RESMAX=ABS(RESI_USE)
                                 IHITSV=INDX_HIT
#if defined(CLEO_FTDIAG)
                                 COMMNT='better !  '
#endif
                              ELSE
#if defined(CLEO_FTDIAG)
                                 COMMNT='          '
#endif
                              ENDIF
                           ENDIF

#if defined(CLEO_FTDIAG)
                              PRINT 1602,INDX_HIT,IADRHT(INDX_HIT),
     2                             TF_LAYR,LAYR_COUNTR,
     3                             RESI_USE,RESMAX,IHITSV,COMMNT
#endif

                        ENDIF
 665                 CONTINUE


                     IF(IHITSV.NE.0)THEN

                        IF(DEL_MODE.EQ.FT_DEL_BAD)THEN
                           ADD_REPLACEMENT=.TRUE.

                        ELSEIF(IADRHT(IHITSV).NE.IADFIT(IFIT))THEN
#if defined(CLEO_FTDIAG)
                           PRINT 1603,IFIT,IADFIT(IFIT),PULFIT(IFIT)
#endif
                           INDX_MAX_PUL=IFIT
                           DROP_SOME=.TRUE.
                           CALL HIT_TO_FIT(H2F_REMOVE_ENTRY,
     2                          IHITRET,INDX_MAX_PUL)
                           ADD_REPLACEMENT=.TRUE.
                        ELSE
#if defined(CLEO_FTDIAG)
                           PRINT 1607,IFIT,IADFIT(IFIT)
#endif
                           ADD_REPLACEMENT=.FALSE.
                        ENDIF

                        IF(ADD_REPLACEMENT)THEN
                           NADDED=NADDED+1

#if defined(CLEO_FTDIAG)
                           PRINT 1605,IFIT,IADRHT(IHITSV),RESMAX,
     2                          IHITSV,DEWEIGHT_CLOSE_ENUF,NADDED
#endif

                           CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITSV,0)
                           CALL HIT_TO_FIT(H2F_DEWEIGHT,
     2                          DEWEIGHT_CLOSE_ENUF,NFIT)
                           UPTODATE_HIT=.FALSE.
                           
                        ENDIF
                     ELSE
#if defined(CLEO_FTDIAG)
                        PRINT 1608,IFIT,IADFIT(IFIT)
#endif
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
 669     CONTINUE
      ENDIF

      IF(NADDED.GT.0)THEN

#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,
     2        'FAST_TRAK add, will refit')
#endif

         CALL FIT_WHATEVER

#if defined(CLEO_FTDIAG)
         PRINT 1003
         MESLOC=' ADDHIT FIT '
         PRINT 1609,MESLOC,
     2        KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF,
     3        NADDED
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK('CF',0,
     2        'FAST_TRAK add hit fit')
#endif
#endif

      ENDIF

      IF(DROP_SOME)THEN
         CALL COMPRESS_FIT
      ENDIF

#if defined(CLEO_FTDIAG)
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1500 FORMAT(' FAST_T:',
     2     'start drop,',
     3     ' require NFIT-NDROP_REFIT-NDROP_NOT_REFIT=',
     4     '(',I3,'-',I3,'-',I3,') >',I2)  
 1501 FORMAT(' FAST_T:',
     1     ' LAYER=',I3,
     2     ' : IADFIT(',I3,')=',I5,
     3     ' : NFIT,Nd,Kd=',I4,I3,I3,
     4     ' : hits be/af=',2I4,
     5     ' : compare PULL=',F11.6,' to LIMIT=', F11.6,' to DROP')
 1502 FORMAT(' FAST_T:',
     1     ' LAYER=',I3,
     2     ' DROP HIT PULL=',F11.6,
     3     ' NDROP_REFIT=',I3,' NDROP_NOT_REFIT=',I3,
     4     ' MAX_DROP=',I3)
 1503 FORMAT(' FAST_T:',
     1     ' LAYER=',I3,
     2     ' COMPARE PULL=',F11.6,
     3     ' TO LIMIT=',F11.6,' ...to drop-a-lot')
 1504 FORMAT(' FAST_T:[',A12,']',
     2     ' KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3     ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4     ' CH=',F9.3,' LAYER',I3)
 1601 FORMAT(' FAST_T:',
     2     ' ADDHIT:new layer   IFIT=',I4,
     3     ' TF_LAYR=',I3,' tfhits_layer',I3,
     4     11X,' MAXNOW=',F11.6,' IADFIT=',I5 )
 1602 FORMAT(' FAST_T:',
     2     ' ADDHIT:    IADRHT(',I3,')=',I5,
     3     ' TF_LAYR=',I3,' tfhits layer',I3,
     4     ' RESI=',F11.6,' MAXNOW=',F11.6,
     5     '   save=',I5,X,A10)
 1603 FORMAT(' FAST_T:',
     2     ' DROP TO REPLACE:   IFIT=',I4,' IADFIT=',I5,' PULL=',F11.6,
     3     ' (unnormalized)')
 1605 FORMAT(' FAST_T:',
     2     ' ADDHIT: adding     IFIT=',I4,' IADRHT=',I5,' RESI=',F11.6,
     3     '  IHITSV=',I4,'   deweight=',F8.2,'  NADDED=',I4)
 1607 FORMAT(' FAST_T:',
     2     ' no replacement for IFIT=',I4,' IADFIT=',I5,
     3     ' (SAME HIT found, but with better residual)')
 1608 FORMAT(' FAST_T:',
     2     ' no replacement for IFIT=',I4,' IADFIT=',I5,
     3     ' NO  hit has a better residual)')
 1609 FORMAT(' FAST_T:[',A12,']',
     2     ' KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3     ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4     ' CH=',F9.3,' NADDED=',I3)

#endif

      RETURN
      END







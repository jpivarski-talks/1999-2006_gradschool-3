* $Id: fast_trak_close_fit_store.F,v 1.13 2003/03/05 17:10:39 dpp Exp $
*
* $Log: fast_trak_close_fit_store.F,v $
* Revision 1.13  2003/03/05 17:10:39  dpp
*      -> changed all variable names in cdscrtcd to have common root
*      -> variable z match requirement in close hit
*
* Revision 1.12  2002/09/05 19:38:07  dpp
*      -> diagnostic formating
*
* Revision 1.11  2002/05/23 14:17:04  dpp
*      -> use the radius limit when selecting hits
*
* Revision 1.10  2001/11/19 22:53:30  dpp
*      -> use status bit encapsulation
*      -> add new control parameter include file
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*      -> use new argument for TFPOPF,TFPUSH
*
* Revision 1.9  2001/09/12 21:56:54  dpp
*      -> expand use of program indicator
*
* Revision 1.8  2000/12/19 23:43:32  dpp
*      -> limit of Z outside endplate
*      -> new status bits
*      -> actively insert zeros in status words for compatibility with recall
*
* Revision 1.7  2000/10/02 20:59:20  dpp
*      -> require valid hit status
*
* Revision 1.6  2000/08/18 18:45:40  dpp
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.5  2000/04/26 14:37:56  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> use call to RESIDUALS instead of RESICF
*
* Revision 1.4  2000/01/06 22:10:55  dpp
* fix error handing
*
* Revision 1.3  1999/12/09 21:09:33  dpp
* change call to hit_to_fit to use parameter directive
* zero the third argument to HIT_TO_FIT(H2F_NEW_ENTRY....
* call HIT_TO_FIT(..DEWEIGHT..) instead of applying deweight locally
*
* Revision 1.2  1999/11/23 23:24:38  dpp
* allow cut in entrance angle
*
* Revision 1.1  1999/11/03 22:43:53  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_CLOSE_FIT_STORE
C.......................................................................
C.
C. F_T_CLOSE_FIT_STORE - add the close hits not found with loc amb groups
C.                       fit
C.                       store in the stack
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   :  split off of FAST_TRACK 21-Oct-99
C. LAST MOD  : 
C.
C. Modification Log.
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
C indices
C-----------------------------------------------------------------------

C INDX_HIT.............counter of hit number
                       INTEGER INDX_HIT
C LAYR_COUNTR..........serial layer counter
                       INTEGER LAYR_COUNTR
C STAK_LOC.............stack location index returned from TFPOPF
                       INTEGER STAK_LOC

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C NFIT_PREV............number of hits, previous
                       INTEGER NFIT_PREV
C TF_LAYR..............layer number found
                       INTEGER TF_LAYR
C DELMAX_C.............maximum "DEL" (close)
                       REAL DELMAX_C
C IHITSV...............save the hit number that is to be fitted
                       INTEGER IHITSV
C DEL..................difference of hit position - fit
                       REAL DEL
C Z_HIT_RESID..........{point - fitted track} in Z used in FITTYP_CF mode
                       REAL Z_HIT_RESID 
C MTEMP................temporary variable for displaying cut limit
                       INTEGER MTEMP
C MESLOC...............message giving location in program
                       CHARACTER*12 MESLOC
C NO_TAILS.............=.TRUE. tails to be eliminated beyond TF_LAYR_MIN/MAX
                       LOGICAL NO_TAILS
C DEWEIGHT_CLOSE.......local ambiguity weight over close hit weight
C                      used for the current track
                       REAL DEWEIGHT_CLOSE
C ROAD_CLOSE_HIT.......cut on allowed deviation from fit for close found hits
C                      used for the current track
                       REAL ROAD_CLOSE_HIT
C ROAD_Z_CLOSE_HIT.....cut on allowed deviation from Z fit 
C                      for close found hits, for the FITTYP_CF mode
C                      used for the current track
                       REAL ROAD_Z_CLOSE_HIT
C TIME_LIM_CLOSE_HIT...cut on observed time
C                      used for the current track
                       REAL TIME_LIM_CLOSE_HIT
C NEW_BEST_HIT.........=.TRUE. for a new best hit based on residuals
                       LOGICAL NEW_BEST_HIT
C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

                       REAL CFIX
                       COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

      PROGRAM_INDICATOR=P_I_F_T_CLOSE_FIT_STORE

C-----------------------------------------------------------------------
C  have filled what is possible using local ambiguity resolution
C  now choose sign of drift distance for remainder using
C  "closest hit to track"
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C  limit large error and low time if there are enough hits from
C    local ambiguity
C  take anything if there are NOT enough hits from local ambiguity
C-----------------------------------------------------------------------

      NFIT_PREV=NFIT

      IF(NFIT_PREV.GT.MAX0(MNHTCF,NHIT_MIN_TRAK))THEN
         DEWEIGHT_CLOSE=DEWEIGHT_CLOSE_ENUF
         ROAD_CLOSE_HIT=ROAD_CLOSE_ENUF
         ROAD_Z_CLOSE_HIT=ROAD_Z_CLOSE_ENUF
         TIME_LIM_CLOSE_HIT=TIME_LIM_CLOSE_ENUF
      ELSE
         DEWEIGHT_CLOSE=1.
         ROAD_CLOSE_HIT=1.E9
         ROAD_Z_CLOSE_HIT=1.E9
         TIME_LIM_CLOSE_HIT=-1.
      ENDIF

      NO_TAILS=NFIT_PREV.GE.N_NOTAIL

C-----------------------------------------------------------------------
C  loop over LAYERS
C  check if the layer is filled from local ambiguity resolution
C  set maximum best from cut defined above
C-----------------------------------------------------------------------


      DO 240 LAYR_COUNTR=1,NLYRHT
      IF(FLAG_LYR_USED(LAYR_COUNTR).NE.0)GO TO 240

      TF_LAYR=IPLRLR(LAYR_COUNTR)

      IF(NO_TAILS)THEN
         IF(TF_LAYR.LT.TF_LAYR_MIN)GO TO 240
c         IF(TF_LAYR.GT.TF_LAYR_MAX)GO TO 240
         IF(ABS(RTF(TF_LAYR)*KAPCF).GT. SIN_ENTR_CLOSE)GO TO 240
      ENDIF

      IF(RTF(TF_LAYR).GT.RADIUS_LIMIT)GO TO 240


      DELMAX_C=ROAD_CLOSE_HIT
      IHITSV=0

C-----------------------------------------------------------------------
C  loop over hit WIRES in the layer
C  choose "best" based on position of wire to the track
C  then choose sign of drift distance based on orientation of track to wire
C  remove low time hits
C  remove hits with no status
C  remove hits beyond endplate
C-----------------------------------------------------------------------

      DO 235 INDX_HIT=LHITLR(LAYR_COUNTR),MHITLR(LAYR_COUNTR)
         IF(ABS(DRFHIT(INDX_HIT)).LT.TIME_LIM_CLOSE_HIT)THEN

#if defined(CLEO_FTDIAG)
            PRINT 1203,TF_LAYR,INDX_HIT,IADRHT(INDX_HIT),
     2           DRFHIT(INDX_HIT)
 1203       FORMAT(' FAST_T: BAD HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2           ' drift=',F9.6)
#endif
            
         ELSEIF(STRFHT(INDX_HIT).LE.0)THEN

#if defined(CLEO_FTDIAG)
            PRINT 1205,TF_LAYR,INDX_HIT,IADRHT(INDX_HIT),
     2           STRFHT(INDX_HIT)
 1205       FORMAT(' FAST_T: BAD HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2           ' STRFHT=',I4)
#endif

         ELSEIF(
     1           ZHIT(INDX_HIT).GT.
     1           (ZENDTF(TF_LAYR)+Z_OVER_LENGTH_FT) )THEN

#if defined(CLEO_FTDIAG)
            PRINT 1207,TF_LAYR,INDX_HIT,IADRHT(INDX_HIT),
     2           ZHIT(INDX_HIT)
 1207       FORMAT(' FAST_T: BAD HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2           ' ZHIT=',F7.3)
#endif

C-----------------------------------------------------------------------
C  distance from the fitted track to the wire: {FITTED TRACK - POINT}
C-----------------------------------------------------------------------
         ELSE
            CALL RESIDUALS('HIT',0,INDX_HIT,DEL)
            NEW_BEST_HIT=(ABS(DEL).LT.DELMAX_C)

            IF(
     1           (NEW_BEST_HIT).AND.
     2           (SAV_FIT_TYP.EQ.FITTYP_CF))THEN

              CALL RESIDUALS('HIT',FITTYP_LF,INDX_HIT,Z_HIT_RESID)
              NEW_BEST_HIT=NEW_BEST_HIT.AND.
     2             (ABS(Z_HIT_RESID).LT.ROAD_Z_CLOSE_HIT)

            ENDIF
            IF(NEW_BEST_HIT)THEN
            
               DELMAX_C=ABS(DEL)
               IHITSV=INDX_HIT

#if defined(CLEO_FTDIAG)
               PRINT 1202,TF_LAYR,INDX_HIT,IADRHT(INDX_HIT),
     2              DEL
 1202          FORMAT(' FAST_T:good HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2              '  DEL=',F9.6)
#endif

            ELSE

#if defined(CLEO_FTDIAG)
               PRINT 1204,TF_LAYR,INDX_HIT,IADRHT(INDX_HIT),DEL
 1204          FORMAT(' FAST_T: bad HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2              '  DEL=',F9.6)
#endif
            ENDIF
C-----------------------------------------------------------------------
C  end of loop over hit wires in layer
C-----------------------------------------------------------------------
         ENDIF
235   CONTINUE

      IF(IHITSV.NE.0)THEN

#if defined(CLEO_FTDIAG)
         DEL=DELMAX_C*ISIGN(1,IADRHT(IHITSV))
         PRINT 1206,TF_LAYR,IHITSV,IADRHT(IHITSV),DEL
 1206    FORMAT(' FAST_T: save HIT LAYER',I3,' HIT',I4,' ADR=',I5,
     2        '  DEL=',F9.6)
#endif
         FLAG_LYR_USED(LAYR_COUNTR)=1
         
         IF(RTF(TF_LAYR).LT.RKN6CF)THEN
            HITBSC=HITBSC+1
         ELSE
            HITASC=HITASC+1
         ENDIF
         
         CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITSV,0)
         CALL HIT_TO_FIT(H2F_DEWEIGHT,DEWEIGHT_CLOSE,NFIT)
      ENDIF

C-----------------------------------------------------------------------
C  end of loop over layers
C-----------------------------------------------------------------------
240   CONTINUE


#if defined(CLEO_FTDIAG)
      MTEMP=MAX0(MNHTCF,NHIT_MIN_TRAK)
      PRINT 1003
      PRINT 1209,NFIT,MTEMP
 1209 FORMAT(' FAST_T: DID OTHER CLOSE HITS, NFIT=',I3,'  NEED',I4)
#endif
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'F_T_close_fit_store CLOSE HITS')
#endif

C-----------------------------------------------------------------------
C  test number of layers in fit before fit
C  turn off the kink if there are not enough hits on each side
C-----------------------------------------------------------------------

      IF(NFIT.GE.MAX0(MNHTCF,NHIT_MIN_TRAK))THEN

C problem, KNK6CF is not reset for each solution

         IF(.NOT.((KNK6CF.EQ.0).OR.(KNK6CF.EQ.KABG2_CLEAR)))THEN
            IF(
     1           (HITBSC.LT.         4).OR.
     2           (HITASC.LT.         4))THEN
               KNK6CF=KABG2_CLEAR
               CALL STATUSBITS(kSTfillcf,kstFTcwKNKaddc,0)
            ELSE
               CALL STATUSBITS(kSTfillcf,kstFTcwKNKaddc,1)
            ENDIF
         ELSE
            CALL STATUSBITS(kSTfillcf,kstFTcwKNKaddc,0)
         ENDIF
         
C-----------------------------------------------------------------------
C  FIT
C-----------------------------------------------------------------------

         CALL FIT_WHATEVER

#if defined(CLEO_FTDIAG)
         PRINT 1003
         MESLOC='   TIME FIT '
         PRINT 1015,MESLOC,
     2              KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK('CF',0,'F_T_close_fit_store AFTR CLOSE')
#endif
#endif

C-----------------------------------------------------------------------
C  turn on the kink and refit if conditions allow
C-----------------------------------------------------------------------

cccccccccccccccccccccc temp ccccccccccccccccccccccccccc
         LKN6TF=.TRUE.
         LKNPRC=.FALSE.
ccccccccccccccccccccccccccccccccccccccccccccccccccccccc
         
#if defined(CLEO_FTDIAG)
         PRINT 1003
         PRINT 1016,KAPCF,NFIT,LKN6TF,LKNPRC,HITBSC,HITASC
#endif

C change in output
         IF(        LKN6TF
     2        .AND. LKNPRC
     3        .AND. (NFIT.GE.          10)
     4        .AND. (HITBSC.GE.         4)
     5        .AND. (HITASC.GE.         4)
     6        .AND. (ABS(KAPCF).LE.   1.5)
     7        .AND. (ABS(KAPCF).GT.   0.1)
     8      )THEN

C kink in kappa:KNK6CF=KABG2_CURV,  kink in  r-phi:KNK6CF=KABG2_PHI
            KNK6CF=KABG2_PHI
            FTK6CF=KABG2_FLOAT
            SKN6CF=R_LEN_SCAT(2)

            CALL FIT_WHATEVER
            CALL STATUSBITS(kSTfillcf,kstFTtaddKNKhf,1)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='4FIT w/KINK '
            PRINT 1015,MESLOC,
     2                 KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif

C-----------------------------------------------------------------------
C CLEO3: if kink is less than 500 micron shift  at 1 meter distance
C CLEO2:                  was 500 micron at .1 meter
C then skip it, it is insignificant
C-----------------------------------------------------------------------

            IF(ABS(PHI6CF).LT.0.0005)THEN
               KNK6CF=KABG2_CLEAR
               CALL FIT_WHATEVER
               CALL STATUSBITS(kSTfillcf,kstFTcwKNKhf,0)

#if defined(CLEO_FTDIAG)
               PRINT 1003
               MESLOC='3F w/o kink '
               PRINT 1015,MESLOC,
     2                    KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif
            ELSE
               CALL STATUSBITS(kSTfillcf,kstFTcwKNKhf,1)
            ENDIF
         ELSE
            CALL STATUSBITS(kSTfillcf,kstFTtaddKNKhf,0)
         ENDIF
         
#if defined(CLEO_FTDIAG)
         PRINT 1003
         MESLOC='FINAL RF FIT'
         PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK('CF',0,'_close_fit_store  FINAL RF FIT')
#endif
#endif

C-----------------------------------------------------------------------
C  store result in stack
C   there can be multiple entries from different loc amb representations
C   and entries will be ordered and popped in FAST_TRAK
C   this is properly preceded by a CF fit
C-----------------------------------------------------------------------

         CALL TFPUSH(STAK_SECONDARY,STAK_LOC)
         
#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,'_close_fit_store  END SOLUTION')
#endif

C-----------------------------------------------------------------------
C  close hit procedure return
C-----------------------------------------------------------------------

      ELSE
         ERROR_CODE=4
      ENDIF

      RETURN

 1003 FORMAT(
     1' --------------------------------------------------------------')
 1015 FORMAT(' FAST_TRAK_CLOSE_FIT_STORE:',
     2 '[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4 ' CH=',F9.3)
 1016 FORMAT(' FAST_T: test of scatter; require'/
     1       '         KAPCF =',F6.3,   '( .1<k<1.5)',
     2              '  NFIT  =',I4,  2X,'( ?>/=10  )'/
     3       '         LKN6TF=',L1,  5X,'( ?=T     )',
     4              '  LKNPRC=',L1,  5X,'( ?=T     )'/
     5       '         HITBSC=',I4,  2X,'( ?>/=4   )',
     6              '  HITASC=',I4,  2X,'( ?>/=4   )')

      END








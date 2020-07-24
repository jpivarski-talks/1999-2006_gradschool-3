*
* $Id: roadhits_coord.F,v 1.3 2003/03/05 18:00:56 dpp Exp $
*
* $Log: roadhits_coord.F,v $
* Revision 1.3  2003/03/05 18:00:56  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:53:50  dpp
*      -> delete unused include file
*      -> add new control parameter include file
*
* Revision 1.1  2000/11/15 16:56:59  dpp
*      ->  NEW name, replaces CDHITS_COORD
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
*
* Revision 1.9  2000/10/24 22:46:38  dpp
*     -> test/set UPTODATE_LYR flag and recalulate SLYRLR
*
* Revision 1.8  2000/10/02 20:44:06  dpp
*      -> fill ZHIT for r-phi silicon
*
* Revision 1.7  2000/04/26 15:00:20  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> require IPLFIT (along with IADFIT) match because
*         silicon has an independent set of hit addresses
*
* Revision 1.6  1999/12/09 21:12:34  dpp
* fill STRFHT and STSZHT with HIT_TYP rather than LVL_TYP
* change call to hit_to_fit to use parameter directive
* change call to hit_to_fit to use "update"
*
* Revision 1.5  1999/11/03 22:21:43  dpp
* eliminate recalculation of TFHITS layer structure
* use UPTODATE_HIT to skip un-needed code
*
* Revision 1.4  1999/09/13 15:07:10  dpp
* fixed fortran error
*
* Revision 1.3  1999/09/12 22:27:35  dpp
* zeros STRFHT and STSZHT on error, does not delete hit
*
* Revision 1.2  1999/08/03 21:55:01  dpp
* XTS call, hit buffer saturation
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE ROADHITS_COORD
C.......................................................................
C.
C. ROADHITS_COORD - Calculate x,y and s,z coordinates of the hits list
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : ROADHITS, WIRES_TO_HITS
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.
C. CREATED   : 23-Dec-97
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
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

      INTEGER ILYRHT
      INTEGER ILYR,IHIT,IFIT,FITFND,IADR
      INTEGER ABS_ADR,IAMB
      REAL YSAG
      INTEGER IERROR
      INTEGER STATRF,STATSZ
      REAL XHT,YHT,FHT,SHT,ZHT
      REAL XTMP,YTMP,FTMP
      REAL COSW,SINW,ZLAYER
      REAL ARCFUN


* ----------Executable code starts here---------------------

      IF(NHIT.GT.0) THEN

C this routine can no longer change the hit list so there
C is no need to rebuild the layer structure
C loop over the hits all layers

         DO 69 IHIT=1,NHIT
            
            YSAG= 0.0

            IADR= IADRHT(IHIT)
            ILYR= IPLHIT(IHIT)
            ILYRHT=ILLRLR(ILYR)

            ABS_ADR= IADR
            IAMB= 1
            IF(ABS_ADR.LT.0)THEN
               ABS_ADR= -ABS_ADR
               IAMB=-1
            ENDIF

C fill x,y values for the hit
C if the hit does not have x,y information, fill with projected

            IF(LUSE_LAYER(ILYR,SAV_LVL_TYP,FITTYP_CF))THEN
               CALL CFXYHT(IADR,ILYR,YSAG,XHT,YHT,IERROR)

               IF(IERROR.EQ.0)THEN
                  STATRF=SAV_HIT_TYP
               ELSE
                  STATRF=0
               ENDIF

            ELSE
               CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2                     RSQTF(ILYR),+1,XHT,YHT,FHT,IERROR)
               STATRF=0
            ENDIF

C make sure the arc length info is up to date
C and fill ZLAYER with default in preparation for z parameter finding 

            IF(.NOT.UPTODATE_LYR(ILYRHT))THEN
               CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2                     RSQTF(ILYR),+1,XTMP,YTMP,FTMP,IERROR)
               SLYRLR(ILYRHT)=ARCFUN(KAPCF,
     2              ALPCF,BETCF,XTMP,YTMP,XICF)
               UPTODATE_LYR(ILYRHT)=.TRUE.
            ENDIF

            IF(UPTODATE_LYR(ILYRHT))THEN
               SHT=SLYRLR(ILYRHT)
            ENDIF

            ZLAYER=Y0LF+TANDLF*SHT
            
C fill s,z values for the hit
C if the hit does not have s,z information, fill with projected

            IF(LUSE_LAYER(ILYR,SAV_LVL_TYP,FITTYP_LF))THEN
               CALL LFXYHT(IADR,ILYR,YSAG,SHT,ZHT,IERROR)
               ZLAYER=Y0LF+TANDLF*SHT

               IF(IERROR.EQ.0)THEN
                  STATSZ=SAV_HIT_TYP
               ELSE
                  STATSZ=0
               ENDIF

            ELSE
               IF(IDVCTF(ILYR).EQ.ISVZ1) THEN
                  ZHT=SV_Z(ABS_ADR)
               ELSE
                  ZHT=ZLAYER
               ENDIF
               STATSZ=0
            ENDIF
            
C correct for stereo angle
            IF(ABS(TANSTF(ILYR)).GT.0.005)THEN
               COSW=XHT/RTF(ILYR)
               SINW=YHT/RTF(ILYR)
               XHT=XHT+ZLAYER*TANSTF(ILYR)*SINW
               YHT=YHT-ZLAYER*TANSTF(ILYR)*COSW
            ENDIF

C load the hit even if there is an error
C IADRHT, IPLHIT filled in calling routine
C DRFHIT may not be filled in all calling routines

            XHIT(IHIT)   = XHT
            YHIT(IHIT)   = YHT
            SHIT(IHIT)   = SHT
            ZHIT(IHIT)   = ZHT
            DRFHIT(IHIT) = EvntHit_Drift(IABS(IADR))*IAMB
            STRFHT(IHIT) = STATRF
            STSZHT(IHIT) = STATSZ


C note: Pointers may be corrupted because they are not preserved in the stack.
C       Have tried a pointer from IADRHT to IFIT but this is also
C       useless after going through the stack.
C       If not going through the stack (and assuming it was not
C       zeroed) could use HITTRK to point to the FIT. 

C NOW: use a flag in /TFHITS/ to specify if the pointers are still up-to-date.
C      UPTODATE_HIT is set false at the beginning of FAST_TRAK, and
C      when the LVL_TYP or HIT_TYP (but not the FIT_TYP) have changed, and
C      in TFPOPF, and
C      always in ROADHITS

            IF(UPTODATE_HIT)THEN
               FITFND=HITTRK(IHIT)
            ELSE
               FITFND=0
               IF(NFIT.NE.0)THEN
                  DO 59 IFIT=1,NFIT
                     IF(
     1                    (IADFIT(IFIT).EQ.IADRHT(IHIT)).AND.
     2                    (IPLFIT(IFIT).EQ.IPLHIT(IHIT)))THEN
                        FITFND=IFIT
                        GO TO 61
                     ENDIF
 59               CONTINUE
 61               CONTINUE
               ENDIF
            ENDIF

C            IF(FITFND.NE.0)THEN
C               IFIT=FITFND
C               PRINT 9901,UPTODATE_HIT,
C     2                    IHIT,IADRHT(IHIT),HITTRK(IHIT),
C     3                    IFIT,IADFIT(IFIT),TRKHIT(IFIT)
C            ELSE
C               PRINT 9902,UPTODATE_HIT,
C     2                    IHIT,IADRHT(IHIT),HITTRK(IHIT)
C            ENDIF
C 9901       FORMAT(' ROADHITS_COORD: up2d=',L1,
C     2             ' hit:',I3,' IADRHT=',I4,' HITTRK=',I3,
C     3             ' fit:',I3,' IADFIT=',I4,' TRKHIT=',I3)
C 9902       FORMAT(' ROADHITS_COORD: up2d=',L1,
C     2             ' hit:',I3,' IADRHT=',I4,' HITTRK=',I3)


C update the FIT list

            IF(FITFND.NE.0)THEN
               CALL HIT_TO_FIT(H2F_UPDATE_ENTRY,IHIT,FITFND)
            ELSE
               HITTRK(IHIT) = 0
            ENDIF
            
 69      CONTINUE
      ENDIF
      
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'ROADHITS_COORD')
#endif

      RETURN
      END



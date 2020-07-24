* $Id: residuals.F,v 1.4 2001/11/19 23:45:58 dpp Exp $
*
* $Log: residuals.F,v $
* Revision 1.4  2001/11/19 23:45:58  dpp
*      -> add new control parameter include file
*
* Revision 1.3  2000/10/02 21:10:43  dpp
*       -> remove reduntant test of fit type
*
* Revision 1.2  2000/08/18 19:03:11  dpp
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.1  2000/04/26 15:04:35  dpp
*      -> NEW
*      -> calculate residuals for r-phi or s-z
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE RESIDUALS(HIT_ARRAY,VIEW,IHIT,RESI)
C.......................................................................
C.
C. RESIDUALS  - find the residual using KAPCF etc
C.             residual is in r-phi or s-z depending on SAV_FIT_TYP
C.             SAV_FIT_TYP can be superceded if VIEW is non-zero
C.
C. COMMON    : ?
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 28-Mar-2000
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
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"

      INTEGER VIEW
      CHARACTER*3 HIT_ARRAY
      INTEGER IHIT
      REAL RESI
      INTEGER FIT_TYP

      REAL XHITL,YHITL,RRH
      REAL SHITL,ZHITL
      INTEGER ILYR,ADRTST

C---------------------------------------------------------------------
C  Executable code starts here
C---------------------------------------------------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C---------------------------------------------------------------------
C check validity of input HITTYP
C---------------------------------------------------------------------

      IF    (HIT_ARRAY.EQ.'HIT')THEN
         XHITL=XHIT(IHIT)
         YHITL=YHIT(IHIT)
         SHITL=SHIT(IHIT)
         ZHITL=ZHIT(IHIT)
         ILYR=IPLHIT(IHIT)
         ADRTST=IADRHT(IHIT)
      ELSEIF(HIT_ARRAY.EQ.'FIT')THEN
         XHITL=XFIT(IHIT)
         YHITL=YFIT(IHIT)
         SHITL=SFIT(IHIT)
         ZHITL=ZFIT(IHIT)
         ILYR=IPLFIT(IHIT)
         ADRTST=IADFIT(IHIT)
      ELSE
         PRINT 9902,HIT_ARRAY
 9902    FORMAT(' RESIDU: invalid HIT_TYP=',A3,' quitting')
         GO TO 990
      ENDIF

C---------------------------------------------------------------------
C check validity of input FITTYP
C---------------------------------------------------------------------

      FIT_TYP=VIEW
      IF(
     2     (FIT_TYP.NE.FITTYP_CF).AND.
     3     (FIT_TYP.NE.FITTYP_LF))THEN
         FIT_TYP=SAV_FIT_TYP
      ENDIF

C---------------------------------------------------------------------
C calculate r-phi residual
C---------------------------------------------------------------------

      IF    (FIT_TYP.EQ.FITTYP_CF)THEN

         IF(ADRTST.NE.0)THEN
            RRH=XHITL**2 + YHITL**2

            IF(
     1           (KNK6CF.NE.0).AND.(KNK6CF.NE.KABG2_CLEAR).AND.
     2           (RTF(ILYR).GT.RKN6CF))THEN
               RESI= KAPAFT6*RRH
     2              +ALPAFT6*XHITL+BETAFT6*YHITL+GAMAFT6
            ELSE
               RESI= KAPCF  *RRH 
     2              +ALPCF*  XHITL+BETCF*  YHITL+GAMCF
            ENDIF

C { a note on the signs...                               }
C {   for CUDR=0 AND DADR=0, ALPCF=SINFI and BETCF=-COSFI}
C {   then for FI=0deg, RESI=-YHIT; before flipping sign }
C {   we want "measured" - "fitting parameters"          }
C { also note that, before Mar 28,2000, the sign was only}
C {   flipped for the "before scatter" case.             }

            RESI=-RESI

         ELSE
            RESI=0.
         ENDIF

C---------------------------------------------------------------------
C calculate s-z residual
C---------------------------------------------------------------------

      ELSEIF(FIT_TYP.EQ.FITTYP_LF)THEN
            RESI= ZHITL - (Y0LF + SHITL*TANDLF)
      ENDIF




 990  CONTINUE
      RETURN
      END









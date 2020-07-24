*
* $Id: cfexfn.F,v 1.7 2000/11/15 16:52:14 dpp Exp $
*
* $Log: cfexfn.F,v $
* Revision 1.7  2000/11/15 16:52:14  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.6  2000/10/02 21:14:00  dpp
*      -> clear out use of STATCF(2)
*      -> use stak number parameter
*
* Revision 1.5  2000/08/18 19:06:55  dpp
*     -> change argument in call to CFLAST to RKN6CF
*
* Revision 1.4  2000/04/26 14:11:01  dpp
*  -> remove argument from TFXFIT
*
* Revision 1.3  1999/09/12 22:30:55  dpp
* remove argument in call to TFXFIT; remove chisq argument
*
* Revision 1.2  1999/08/03 21:53:29  dpp
* calls to stack routines
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/02/14 21:10:44  zfiles
* move call to CFHITS to come before call to TFXFIT, fix arguments of TFXFIT
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :D. Peterson
      SUBROUTINE CFEXFN(ITAG)

C FiNish off the EXtended CF track

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/fast_trak.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"

      INTEGER ITAG
      INTEGER IFIT,ILYR


* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
C CDCORH called in DU3DTR after CFHITS (which calls ROADHITS)
C     CALL CDCORH(CFXYHT)

      IF(LEXTFX)THEN
          CALL VFILL(LUSETF,KLYRTF, .FALSE.)
          DO 15 IFIT=1,NFIT
          ILYR=IPLFIT(IFIT)
          LUSETF(ILYR)=.TRUE.
15        CONTINUE

          CALL TFXFIT
      ENDIF

      NHITCF = NFIT
      NHITTF = NHITCF
      CHITF  = CHICF
      PHI7CF = 0
      RKN7CF = 9.0 + .01*ITAG

#if defined(CLEO_SFENDP)
      CALL CFLAST( KAPCF, ALPCF, BETCF, GAMCF,
     2             PHI6CF, RKN6CF )
#endif

C      CALL SBYT(ITAG,STATCF(2),23,2)
      CALL TFPUSH(SECONDARY_STAK)

      RETURN
      END











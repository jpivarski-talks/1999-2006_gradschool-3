*
* $Id: cflast.F,v 1.3 2000/08/18 19:05:39 dpp Exp $
*
* $Log: cflast.F,v $
* Revision 1.3  2000/08/18 19:05:39  dpp
*      -> change scattering road calculation to use DOIT scattering definitions
*      -> new arguments for KABG2
*
* Revision 1.2  1998/07/09 00:25:09  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:21  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/01/30 15:08:49  zfiles
* include kink in r-phi pattern recognition, requires change in argument
*
* Revision 1.2  1995/04/20 22:38:28  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:42  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  13.54.00  by  Rob Kutschke
*CMZ :  6.00/02 28/07/93  10.03.29  by  Dan Peterson
*CMZ :  3.01/01 28/03/90  17.22.00  by  Dan Peterson
*CMZ :          28/07/88  14.47.20  by  D. Peterson
*-- Author : D. Peterson
      SUBROUTINE CFLAST( KAPPA, ALPHA, BETA, GAMMA, KINK, RKN )
C.......................................................................
C.
C. CFLAST - Find the last reasonable layer contributing to this track
C.
C. COMMON    :
C. CALLS     : nothin'
C. CALLED    : ?
C. AUTHOR    : D. Peterson, modeled after CFXFIT
C.
C. VERSION   : 1.00
C. CREATED   : 3-May-1988
C. LAST MOD  : 1-Aug-88
C.
C.
C. Modification Log.
C. 23-Mar-90, compatable with CLEOII.
C. 1-Aug-88, added calculation of ILRESC(1), exit of VD
C.......................................................................
C=======================================================================
C CFLAST finds the last reasonable hit in /TFHITS/ for
C track candidate # ITRK.
C The intent is to use this information as the track escape point.
C
C  Arguments:
C      ITRK                       Track candidate number
C      KAPPA,ALPHA,BETA,GAMMA     Track parameters after the fit
C      KINK, RKN                  kink and radius
C=======================================================================
C
C#######################################################################
C NOTE! CFLAST uses a mixture of TF and CD geometry varaiables.
C In particular, it use LCANVD and MCANVD.
C This is fine as long as the VD layer numbers are equal in CD and TF.
C TFGEOMPA does not separate VD from the IV/PTL, so there is no
C    valid LLYRVD.
C This routine is so garbled up, one should probably abandon making
C   it general.
C#######################################################################

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/escappar.inc"

C== Argument declarations

      REAL    KAPPA, ALPHA, BETA, GAMMA, KINK, RKN

C== Local Variables
      REAL KAA, ALA, BEA, GAA, XIA, XIB
      INTEGER IHLOOS,ILOC1,ILOC2,ILOC3,ILOC4,ILOC0
      INTEGER NL1STA,ILYR,IHIT,ILLYR,IH,MPSTAK
      INTEGER LOOP
      REAL YSCAT2,RESMAX,RES
      INTEGER NLHVT1,NLHVT2,NLHVT3,MLPAS1,MLPAS2,MLPAS3
      INTEGER NL1STP
      INTEGER NLHVVD,MLHVVD,ILHVVD
      INTEGER ISAVLH,ISAVVD
      DIMENSION ISAVLH(NOUTA2)
      DIMENSION ISAVVD(NCANVD)
      REAL PKAPRD
      INTEGER ILYRVD,IADRQ1,IADRQ2,IADRQ3,IADRQ4,NSAVQV

#if defined(CLEO_CFLADIAG)
      CHARACTER*18 TYPOUT
      REAL TEMPTC
#endif

#include "doit/duseq/duexed.inc"

      CALL KABG2(KABG2_CALC,
     1     0,0,0,KABG2_XI_RECALC,
     2     KAPPA,ALPHA,BETA,GAMMA,XIB,KINK,
     3     KAA,  ALA,  BEA, GAA,  XIA)

      ILRESC(1)=0
      ILRESC(2)=0
      DO 10 ILOC0=1,NOUTA2
10    ISAVLH(ILOC0)=0
      NLHVT1=0
      NLHVT2=0
      NLHVT3=0
      NL1STP=0
      ILOC0=0
      NLHVVD=0

C== loop over all CF layers

      DO 80 ILYR=LLYRCF,MLYRCF

C== define the locations in ISAVLH

      ILOC0=ILOC0+1
      IF(ILOC0.GT.NOUTA2)ILOC0=ILOC0-NOUTA2
      ILOC1=ILOC0-NOUTA1
      IF(ILOC1.LE.0)ILOC1=ILOC1+NOUTA2
      ILOC2=ILOC0-NOUTA2
      IF(ILOC2.LE.0)ILOC2=ILOC2+NOUTA2
      ILOC3=ILOC0-NOUTA3
      IF(ILOC3.LE.0)ILOC3=ILOC3+NOUTA2

C== test to see if there is anything at all in this layer

      IHIT=0
      DO 20 ILLYR=1,NLYRHT
      IF(IPLRLR(ILLYR).EQ.ILYR)GO TO 25
20    CONTINUE

C== unfound layer

      ILLYR=0

#if defined(CLEO_CFLADIAG)
      TYPOUT='NO HIT IN LAYER   '
#endif

      GO TO 50

C== found layer

25    CONTINUE

      IF(ACTIV_SCAT(2))THEN
         YSCAT2 =   R_LEN_SCAT(2) *
     1        ((RAD_SCAT(2)-RTF(ILYR))*SCTGEV*EETF/P2TF)**2
      ELSE
         YSCAT2=0.
      ENDIF

      RESMAX=SQRT(RESTST**2+YSCAT2)

C== Manual loop thru hits in this layer
C              DO 30 IH = LHITLR(ILLYR), MHITLR(ILLYR)

      IH=LHITLR(ILLYR)
28    CONTINUE

      IF(RTF(ILYR).GT.RKN)THEN
            RES= ABS( KAA  *(XHIT(IH)**2 + YHIT(IH)**2)
     2              + ALA  * XHIT(IH) + BEA  *YHIT(IH) + GAA  ) 
      ELSE
            RES= ABS( KAPPA*(XHIT(IH)**2 + YHIT(IH)**2)
     2              + ALPHA* XHIT(IH) + BETA *YHIT(IH) + GAMMA)
      ENDIF

      IF(RES.LE.RESMAX)GO TO 40
      IH=IH+1
      IF(IH.LE.MHITLR(ILLYR))GO TO 28

30    CONTINUE

C== did NOT find reasonable hit

#if defined(CLEO_CFLADIAG)
      TYPOUT='HITS ARE OFF TRACK'
#endif

      GO TO 50

C== FOUND reasonable hit

40    IHIT=1

#if defined(CLEO_CFLADIAG)
      TYPOUT='FOUND GOOD HIT !!!'
#endif

C== now check criteria

50    NLHVT1=NLHVT1+IHIT-ISAVLH(ILOC1)
      NLHVT2=NLHVT2+IHIT-ISAVLH(ILOC2)
      NLHVT3=NLHVT3+IHIT-ISAVLH(ILOC3)
      MLPAS1=0
      MLPAS2=0
      MLPAS3=0
      IF(NLHVT1.GE.NLNED1)MLPAS1=ILYR
      IF(NLHVT2.GE.NLNED2)MLPAS2=ILYR
      IF(NLHVT3.GE.NLNED3)MLPAS3=ILYR

#if defined(CLEO_CFLADIAG)
      PRINT 2001,ILYR,TYPOUT,MLPAS1,MLPAS2,MLPAS3,NL1STP
2001  FORMAT(' CFLAST: LAYER_',I2,'_',A18,
     1       '  P=',I2,I3,I3,' NL1STP=',I3)
#endif

      IF(NL1STP.NE.0)GO TO 55

C= check to see if this is the first layer to pass all 3 criteria

      IF(MLPAS1*MLPAS2*MLPAS3.EQ.0)GO TO 72
      NL1STP=ILYR

#if defined(CLEO_CFLADIAG)
      PRINT 2002,ILYR
2002  FORMAT(' CFLAST: FIRST LAYER TO SATISFY GOOD START=',I3)
#endif

      GO TO 70

C== now test if the search should be stopped

55    IF(MLPAS1.EQ.ILYR)GO TO 70
      IF(MLPAS2.EQ.ILYR)GO TO 70

C== have failed tests #s 1&2,  this is a significant break

#if defined(CLEO_CFLADIAG)
      PRINT 2003,ILYR,ILRESC(2)
2003  FORMAT(' CFLAST: SGNIFICANT BREAK IDENTIFIED LAYER',I3,
     1  ' ILRESC(2)=',I3)
#endif

      GO TO 101

C== save last significant hit

70    IF(MLPAS3.NE.ILYR)GO TO 72
      IF(IHIT.EQ.0)GO TO 72
      ILRESC(2)=ILYR

C== prepare to loop back

72    IF(ILYR.LT.LCANVD)GO TO 80
      IF(ILYR.GT.MCANVD)GO TO 80

C== save VD info for vd section

      ILYRVD=ILYR-LCANVD+1
      ISAVVD(ILYRVD)=IHIT
      IF(IHIT.EQ.0)GO TO 80
      NLHVVD=NLHVVD+1

C== end of loop over CF layers

80    ISAVLH(ILOC0)=IHIT

#if defined(CLEO_CFLADIAG)
      PRINT 2004,ILRESC(2)
2004  FORMAT(' CFLAST: FINISH LOOP OVER LAYERS, ILRESC(2)=',I3)
#endif

C== successfully looped through all layers
C== or  never found a significant track start... ILRESC=0
C== OR found significant break and successfully filled ILRESC(2) above

101   CONTINUE

#if defined(CLEO_CFLADIAG)
      PRINT 2005,ILRESC(2)
2005  FORMAT(
     1 ' CFLAST: FINISHED ILRESC(2) SECTION, CHECKING ILRESC(2)=',I3)
#endif

      IF(ILRESC(2).EQ.0)GO TO 195
      PKAPRD=ABS(KAPPA)*RTF(ILRESC(2))

C NO LONGER SKIP FILLING ILRESC(1), TO FIND Q-DIV
C WILL HAVE TO CHECK AGAINST LLYRE1 AND MLYRE1 IN SFENDP
C==criterion is to maximize (NUMBER BEFORE BREAK)+(MISSING AFTER BREAK)
C==start with break at ILYR=0, score is total missing
C==  best score

      MLHVVD=NCANVD-NLHVVD

C== current score

      ILHVVD=MLHVVD
      DO 110 ILYR=1,NCANVD
      ILHVVD=ILHVVD-1+2*ISAVVD(ILYR)

C==choose the higher layer number with largest score

      IF(ILHVVD.LT.MLHVVD)GO TO 108
      MLHVVD=ILHVVD
      ILRESC(1)=ILYR-1+LCANVD
108   CONTINUE

#if defined(CLEO_CFLADIAG)
      PRINT 2006,ILYR,ILHVVD,MLHVVD,ILRESC(1)
2006  FORMAT(' CFLAST: LAYER_',I2,' ILHVVD/MLHVVD=',I2,I3,
     1  '  ILRESC(1)=',I2)
#endif

110   CONTINUE

C end of section to fill ILRESC(1)

C==either skipped or finished filling ILRESC(1)

195   CONTINUE

#if defined(CLEO_CFLADIAG)
      PRINT 2007,ILRESC
2007  FORMAT(' CFLAST: ILRESC=',2I6)
#endif

      RETURN
      END


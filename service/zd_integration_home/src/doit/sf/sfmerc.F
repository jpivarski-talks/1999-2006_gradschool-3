*
* $Id: sfmerc.F,v 1.4 2003/03/05 18:34:39 dpp Exp $
*
* $Log: sfmerc.F,v $
* Revision 1.4  2003/03/05 18:34:39  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.3  2002/09/05 18:49:10  dpp
*   -> remove local ICHAIN
*
* Revision 1.2  2001/11/20 17:53:53  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.4  1997/02/25 21:53:50  dpp
* use SVX layers in addition to CD layers
*
* Revision 1.3  1996/02/14 21:04:24  zfiles
* remove call to SFLWF; use SFLSFA etc
*
* Revision 1.2  1995/04/20 22:39:54  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:47  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/14 01/03/94  19.39.14  by  Rob Kutschke
*-- Author : D. Peterson
      SUBROUTINE SFMERC (IFWADR, NFITV )
C.......................................................................
C.
C. SFMERC - compare the fit hit list to the input list
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Only by SFFTRK
C. AUTHOR    :  D. Peterson
C.
C. VERSION   : 2.00
C. CREATED   : 12-Nov-91
C. LAST MOD  : 12-Nov-91
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C== Argument declarations

* IFWADR - Addresses of fit wires  input
* NFITV  - Number of fit wires     input

      INTEGER IFWADR(*), NFITV

#include "doit/duseq/tfindpar.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/duseq/tfctlcde.inc"
#include "seq/clutil/mesrep.inc"

C== Local variables

C LSTCHN...LAST CHAIN NUMBER
C LOST.....NUMBER LOST ON THIS CHAIN
C MLOST....MAXIMUM LOST ON ANY ON CHAIN
C MLSTCH...CHAIN NUMBER WITH MAXIMUM LOST
C IFITV
C LYRFCD...LAYER FOUND CD
C LYRFTF...LAYER FOUND TF
C LYRWSF.....LAYER WANT SF
C LYRWCD.....LAYER WANT CD
C LYRWTF.....LAYER WANT TF
      INTEGER LSTCHN,LOST,MLOST,MLSTCH
      INTEGER IFITV ,LYRFCD,LYRFTF
      INTEGER LYRWSF,LYRWCD,LYRWTF
      INTEGER IWIRE,IWADR,IWADRN
      INTEGER I,IWIR
      INTEGER IOS

#if defined(CLEO_SFFTDIAG)
      INTEGER IGO
#endif

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

#if defined(CLEO_SFFTDIAG)
      PRINT 5001,NFITV
5001  FORMAT(' SFMERC:  NFITV=',I3)
#endif


C loop over the chains that were passed

      LSTCHN=0
      MLOST=0
      LOST=0
      IF(NCHANM.LE.0)RETURN
      DO 39 I=1,NCHANM
      ICHAIN=JCHAIN(I)

#if defined(CLEO_SFFTDIAG)
      PRINT 5002,I,ICHAIN
5002  FORMAT(' SFMERC: SEQ CHAIN#',I2,' CHAIN#',I3)
#endif

C loop over hits in the chain

      IWIR=1
      IWADR=LINCHN(ICHAIN)
11    IF(IWADR.EQ.0)GO TO 39
C only check is the INUCHN number points to a different chain
C this is the indication that it is a merge
      IF(INUCHN(IWADR).EQ.ICHAIN)GO TO 27
C get layer and wire number
      LYRWSF=SFLSFA(IWADR)
      IWIRE=SFWSFA(IWADR)
C do not use east VD
      IF(LYRWSF.EQ.0)GO TO 27
      IF(IWIRE.GE.NWRSF(LYRWSF))GO TO 27
C check only CD wires
      LYRWCD=CDLASF(LYRWSF)
      IF(LYRWCD.LT.0)GO TO 27
C convert to TF layer number
      LYRWTF=ILCDTF(LYRWCD)

#if defined(CLEO_SFFTDIAG)
      PRINT 5003,IWIR,IWADR,LYRWSF,IWIRE
5003  FORMAT(' SFMERC:********** SEQ HIT',I3,'  ADR',I5,
     1' LYRWSF=',I3,' WIRE#',I3)
#endif

C tally the number lost for current chain
      IF(INUCHN(IWADR).EQ.LSTCHN)GO TO 13
      LOST=0
      LSTCHN=INUCHN(IWADR)
13    CONTINUE
C SEARCH IFWADR FOR THIS LAYER
      DO 19 IFITV=1,NFITV
      LYRFCD=EvntHit_CDlayr(IABS(IFWADR(IFITV)))
      LYRFTF= ILCDTF(LYRFCD)

#if defined(CLEO_SFFTDIAG)
C      PRINT 5004,IFITV,IFWADR(IFITV),LYRFCD,LYRFTF,LYRWSF
5004  FORMAT(' SFMERC: IFWADR(',I2,')=',I6,
     1          ' ILCD=',I3,' ILTF=',I2,' WANT',I3)
#endif

      IF(LYRFTF.NE.LYRWTF)GO TO 19

#if defined(CLEO_SFFTDIAG)
      PRINT 5005,LYRWSF,LSTCHN
5005  FORMAT(' SFMERC: FOUND LAYER#',I5,'  FOR CHAIN NUMBER',I4)
#endif

      GO TO 27
19    CONTINUE
      LOST=LOST+1
      IF(LOST.LE.MLOST)GO TO 21
      MLOST=LOST
      MLSTCH=LSTCHN
21    CONTINUE

#if defined(CLEO_SFFTDIAG)
      PRINT 5006,LYRWSF,LSTCHN,LOST,MLOST
5006  FORMAT(' SFMERC: LOST  LAYER#',I5,'  FOR CHAIN NUMBER',I4,
     1       '    LOST=',I3,' MLOST=',I3)
#endif

27    IWIR=IWIR+1
      IF(IWADR.EQ.MINCHN(ICHAIN))GO TO 39
      IWADRN=ISTFOR(IWADR)
      IF(IWADRN.GT.IWADR)GO TO 35
      IF(IWADRN.EQ.0)GO TO 35


      WRITE(CHMESS,9901,IOSTAT=IOS)NRUN,NEVENT,ICHAIN,
     1  LINCHN(ICHAIN),MINCHN(ICHAIN),LENCHN(ICHAIN),IWADR,IWADRN
9901  FORMAT(' EV=',I6,'/',I6,' CH',I3,' L=',I5,' M=',I5,
     1                        ' L=',I3,' A=',I5,' AN=',I5)
      CALL MESLOG(' SFMERC',1,MSERRO)

#if defined(CLEO_SFFTDIAG)
      PRINT 5011
      PRINT 5012
      PRINT 5013
      PRINT 5014
      PRINT 5015
5011  FORMAT(' SFMERC:    SSSS TTTTT U   U  CCC  K   K')
5012  FORMAT(' SFMERC:   S       T   U   U C   C K  K ')
5013  FORMAT(' SFMERC:    SSS    T   U   U C     KKK  ')
5014  FORMAT(' SFMERC:       S   T   U   U C   C K  K')
5015  FORMAT(' SFMERC:   SSSS    T    UUU   CCC  K   K')
      READ 5016,IGO
5016  FORMAT(A1)
#endif
      GO TO 45
35    IWADR=IWADRN

C end of loop over hits in the chain
      GO TO 11



39    CONTINUE
      IF(MLOST.EQ.0)GO TO 49

#if defined(CLEO_SFFTDIAG)
      PRINT 5007,MLSTCH,MLOST,MLOST,LENCHN(MLSTCH),
     1                       MLOST,LENCHN(MLSTCH),MLOST
5007  FORMAT(' SFMERC: CONSTITUENT CHAIN#',I3,' LOST ',I3,' LAYERS'/
     1       '         WILL KEEP IF 4* ',I5,' <  'I3/
     1       '         WILL KILL IF 2* ',I5,' >/='I3/
     2       '         WILL KILL IF    ',I5,' >/= 8')
#endif

C TEST CRITERIA

      IF(4*MLOST.LT.LENCHN(MLSTCH))GO TO 49
      IF(2*MLOST.GE.LENCHN(MLSTCH))GO TO 45
      IF(  MLOST.GE. 8)            GO TO 45
      GO TO 49

C FAILURE

45    NFITV=0

C SUCCESS

49    RETURN
      END



















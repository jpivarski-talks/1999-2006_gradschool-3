* Urheim - declare JDV and JDVIN as character variables.
*
* $Id: sfover.F,v 1.6 2003/03/05 17:38:08 dpp Exp $
*
* $Log: sfover.F,v $
* Revision 1.6  2003/03/05 17:38:08  dpp
*      -> remove zfiles stuff and use of SFNOZFILES switch
*      -> changed all variable names in cdscrtcd to have common root
*      -> use EvntHit_OpnHit to indicate usable hits
*
* Revision 1.5  2002/06/05 03:04:03  atems
* minor fixes to compile under linux
*
* Revision 1.4  1999/05/28 03:04:47  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.3  1998/07/09 00:38:49  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.2  1998/05/08 14:13:58  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.2  1995/06/25 01:18:47  zfiles
* Mods from Rob (and Jon)
* doit/duseq/cdtrakcd.inc
* doit/duseq/cdtrakdo.inc
* Add new variable PATLCD.
* doit/duseq/tfxfitcd.inc
* Change PTLIVx to LOGICAL from INTEGER
*
* Revision 1.1  1995/04/20  22:39:10  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :          17/08/93  10.06.14  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.31  by  Dan Peterson
*CMZ :  2.00/00 09/05/89  11.12.32  by  Dan Peterson
*-- Author :
C
C
      SUBROUTINE SFOVER
#if !defined(Linux)
      IMPLICIT NONE
C allows user to enter in CD data by hand

#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "cl3seq/cdgm3/cdraw1in.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfextra.inc"
#include "cl3seq/zfiles/usbank.inc"

      INTEGER ILYRCD,IH,IWIR,I
      INTEGER ILASTH(KLYRCD)
      INTEGER ITEMP,IGO
      INTEGER IDV,JDVS(3)
      CHARACTER*2 JDV(3),JDVIN
      INTEGER MAX,ICD,IWIRW
      REAL PHI,PI,TWOPI
      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=2.*PI)

C note: these address offsets are entered in code; not using zfiles
      DATA JDV,JDVS/'PT','VD','DR',0,384,1984/

C NHWCD is defined in cl3seq/cdgm3/cdrawsin
      PRINT 3001,NHWCD
3001  FORMAT(' SFOVER: READY TO OVERRIDE;NHWCD=',I5,
     1 '  <CR> FOR YES, "-1" FOR NO')
      READ 3002,IGO
3002  FORMAT(I)
      IF(IGO.EQ.-1)RETURN
      DO 303 IH=1,MWIRCD
303   IZU(PZU(IPMPCD)+IH)=0
      DO 305 ILYRCD=1,NLYRCD
305   IZU(PZU(IPFHCD)+ILYRCD)=0
      PRINT 3006
3006  FORMAT(' SFOVER: RAW DATA IN DESTROYED')
      IH=0
324   IDV=3
325   PRINT 3003,JDV(IDV)
3003  FORMAT(' SFOVER:',
     2  ' INPUT IS LOCAL DEVICE ADDRESS AND DEVICE NUMBER'/
     3  ' GI''ME  ADDRESS,DEVICE',
     4  ' END WITH -1; CURRENT DEVICE IS ',A2)
      IF(IH.GE.NHWCD)GO TO 389
      READ 3004,ITEMP,JDVIN
3004  FORMAT(I,A2)
      IF(ITEMP.EQ.-1)GO TO 390
      IF(JDVIN.EQ.' ')GO TO 331
      DO 327 I=1,3
      IDV=I
      IF(JDVIN.EQ.JDV(IDV))GO TO 331
327   CONTINUE
      PRINT 3011,ITEMP,JDVIN
3011  FORMAT(' SFOVER: BAD INPUT, "',I10,'" ,  "',A2,'"')
      GO TO 324
331   IF(ITEMP.LT.1)GO TO 325
      ITEMP=ITEMP+JDVS(IDV)
      IF(ITEMP.LT.1)GO TO 325
      IF(ITEMP.GT.MWIRCD)GO TO 325
C
C GET CD LAYER NUMBER
      MAX=0
      DO 353 ICD=1,NLYRCD
      IF(INDXCD(ICD).GT.ITEMP)GO TO 353
      IF(INDXCD(ICD).LT.MAX)GO TO 353
      MAX=INDXCD(ICD)
      ILYRCD=ICD
353   CONTINUE
C GET CD WIRE NUMBER
      IWIR=ITEMP-MAX
      IWIRW=IWIR
C CHECK WIRE NUMBER
      IF(ILYRCD.LT.LCANVD)GO TO 355
      IF(ILYRCD.GT.MCANVD)GO TO 355
C THIS LINE IS FOR VD, EAST IS WIRE# NWIRCD TO 2*NWIRCD-1
      IF(IWIR.GE.2*NWIRCD(ILYRCD))GO TO 359
      IF(IWIR.GE.NWIRCD(ILYRCD))IWIRW=IWIR-NWIRCD(ILYRCD)
      GO TO 357
C THIS LINE IS FOR OTHER THAN THE VD
355   IF(IWIR.GE.NWIRCD(ILYRCD))GO TO 359
357   PHI=PHIFCD(ILYRCD)+TWOPI/NWIRCD(ILYRCD)*IWIR
      GO TO 383
359   PRINT 3007,ITEMP
3007  FORMAT(' SFOVER: ADDRESS=',I10,' IS INVALID CD HIT')
      GO TO 325

C PUT INTO the data structure

383   IH=IH+1

C*****For non-zfiles use (A. Lyon 5/21/99)
      if (EvntHit_1stLyr(ilyrcd) .ne. 0 ) goto 385
      EvntHit_1stLyr(ilyrcd) = ih
      goto 387
 385  EvntHit_NxtLyr(ilasth(ilyrcd)) = ih
 387  EvntHit_NxtLyr(ih) = 0
      ilasth(ilyrcd) = ih
      EvntHit_MapDet(itemp) = ih
      EvntHit_OpnHit(ih) = 7
      EvntHit_laWire(ih) = iwir
      EvntHit_rawTIM(ih) = 1.0

      GO TO 325
389   PRINT 3012,NHWCD
3012  FORMAT(' SFOVER: THAT IS ALL THAT IS ALLOWED, NHWCD=',I5)
#endif(Linux)
390   RETURN
      END







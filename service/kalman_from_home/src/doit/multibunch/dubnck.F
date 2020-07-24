*
* $Id: dubnck.F,v 1.2 1999/11/29 21:43:52 lyon Exp $
*
* $Log: dubnck.F,v $
* Revision 1.2  1999/11/29 21:43:52  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:36  clib
* Developmental version of DUET.
*
* Revision 1.4  1997/04/15 04:31:12  clib
* dubnch -> cdbnc100.
*
* Revision 1.3  1995/11/30 23:49:48  zfiles
* Minor updates from Peter Kim
*
* Revision 1.2  1995/11/27 21:48:48  zfiles
* TFMAX was not typed
*
* Revision 1.1  1995/11/27 19:40:07  nk
* evaluate bunch hypothesis
*
*
C----------------------------------------------------------------
      SUBROUTINE DUBNCK(IBNIN,IBNOUT)
C
C   Determine the bunch again using  bnchcom.inc & cdtrakcd.inc
C
C   calls : PRJCYL  POS  (both in TRIO)
C
C                                                 11/13/95  PCK
C----------------------------------------------------------------
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdtrakcd.inc"
*
#include "doit/utilseq/bnchcom.inc"

#include "doit/utilseq/cdbnc100.inc"

C-- real function
      REAL POS
      EXTERNAL POS
C                     
      INTEGER IBNIN,IBNOUT

      REAL RESSUM,RESAVG,HITAVG,CDBN
      REAL CU,FI,DA,CT,Z0,PHI,PHIS,ZPOS,ZS,PATHC,PHIX
      REAL RADTOF,TWOPI
      REAL TFSUM

      INTEGER ITRK,ITF
      INTEGER IFLGT(30),NTRKG,HITSUM
      INTEGER IERRC,ICTOFB,IR1,IR2
      INTEGER IFTOFB(64),NTFUS
      INTEGER IBUNCD,IBUNT2

C                              Radius for TOF
      RADTOF = 1.00
      TWOPI  = 6.283185308

      IBUNCD = 0
      IBUNT2 = 0

      CALL VZERO(IFLGT,30)

      NTRKG = 0
      HITSUM = 0
      RESSUM = 0.0
      CDBN = -100.

      IF(NTRKCD.LE.0) GOTO 900

      DO 100 ITRK=1,NTRKCD
        IF(KINCD(ITRK).NE.0)        GOTO 100
        IF(ABS(Z0CD(ITRK)).GT.0.05) GOTO 100
        IF(ABS(D0CD(ITRK)).GT.0.01) GOTO 100
        IF(RESICD(ITRK).GT.0.001)   GOTO 100
          IFLGT(ITRK) = 1
          NTRKG = NTRKG + 1
          HITSUM = HITSUM+NHTADR(ITRK)+NHTSDR(ITRK)+NHTLVD(ITRK) 
          RESSUM = RESSUM+RESICD(ITRK)
 100  CONTINUE


      IF(NTRKG.GE.1) THEN
        RESAVG = RESSUM/FLOAT(NTRKG)
        HITAVG = FLOAT(HITSUM)/FLOAT(NTRKG)
        CDBN = RESAVG*(70.0/0.00075) - HITAVG
        CDBN = CDBN/70.0

        IF(CDBN.LT.0.0) THEN 
          IBUNCD = IBNIN
        ELSE
          IF(IBNIN.EQ.1) IBUNCD = 2
          IF(IBNIN.EQ.2) IBUNCD = 1
        ENDIF
      ENDIF

C---  Associate barrel TOF hits in TFBN with CD tracks

      CALL VZERO(IFTOFB,64)

      DO 200 ITRK = 1, NTRKCD
        IF(IFLGT(ITRK).EQ.0) GOTO 200

        CU = CURCD(ITRK)
        FI = PHI0CD(ITRK)
        DA = D0CD(ITRK)
        CT = TANDCD(ITRK)
        Z0 = Z0CD(ITRK)

        CALL PRJCYL(CU,FI,DA,CT,Z0,RADTOF,IERRC,
     *                              PHI,PHIS,ZPOS,ZS,PATHC)    
        IF(IERRC.NE.0) GOTO 200

        PHIX = POS(PHI)
        ICTOFB = 64.*PHIX/TWOPI+1
C                                              Alternative counting
CCC     CALL TBLOC(ID,PHI,ZPOS,PHIMIN,PHIMAX)
CCC     ICTOFB = ID * 0.5
        
        IR1 = ICTOFB + 1
        IR2 = ICTOFB - 1
        IF(IR1.GT.64) IR1 = IR1 - 64
        IF(IR2.LT. 1) IR2 = IR2 + 64
                                        
        IFTOFB(ICTOFB) = 1  
        IFTOFB(IR1)    = 1
        IFTOFB(IR2)    = 1

 200  CONTINUE

C                              Loop over barrel TOF hits
      TFSUM = 0.0
      NTFUS = 0
      DO 300 ITF=1,NHTTFB
        IF(IFTOFB(TFBNBC(ITF)).EQ.0) GOTO 300
          TFSUM = TFSUM + TFBNBP(ITF)
          NTFUS = NTFUS + 1
 300  CONTINUE

      IF(NTFUS.GE.2) THEN
        IF(TFSUM.LT.0.0)  IBUNT2 = 1       
        IF(TFSUM.GT.0.0)  IBUNT2 = 2
      ENDIF

 900  CONTINUE

C 
C                                  
      IBNOUT = IBNIN

C TFBN not sure, run DUET twice
      IF(ABS(P1OP2).LT.1.0.AND.ABS(TFSUM).LT.1.0) THEN
        IBNOUT = 0 
        GOTO 999        
      ENDIF

      IF(ABS(CDBN).LT.0.2) THEN
        IBNOUT = 0
        IF(IBUNT2.GT.0.AND.IBUNT2.EQ.IBUNCD) 
     *                                  IBNOUT = IBUNT2
      ENDIF


999   CONTINUE

      IBNDU(IBNIN)  = IBUNCD
      NTGDBN(IBNIN) = NTRKG
      DUEVAL(IBNIN) = CDBN
      
      IBNTFD(IBNIN) = IBUNT2
      TFBDLR(IBNIN) = TFSUM

      IBNDUF = IBUNCD

      RETURN
      END



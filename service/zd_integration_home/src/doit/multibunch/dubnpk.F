*
* $Id: dubnpk.F,v 1.2 1999/11/29 21:43:52 lyon Exp $
*
* $Log: dubnpk.F,v $
* Revision 1.2  1999/11/29 21:43:52  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1998/02/24 18:58:29  dpp
* not added 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
C-----------------------------------------------------------------
      SUBROUTINE DUBNPK(IBUNFX)
C
C   Compare the TFBN/DUET results between 2 bunch hypothesis and 
C   make the final decision on the bunch selection.
C
* Replace dubnch with tfbnch and cdbnc100.
* Minor updates from Peter Kim
* "pick" best bunch on output of DUBNCK
C
C                                                  11/13/95  PCK
C-----------------------------------------------------------------

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/utilseq/bnchcom.inc"
#include "doit/utilseq/tfbnch.inc"
#include "doit/utilseq/cdbnc100.inc"

      INTEGER IBUNFX

      REAL    RRF

C--- DUET comparison

      IF(IBNDU(1).EQ.1) THEN
        RRF = DUEVAL(1) - DUEVAL(2)
      ELSE
        RRF = DUEVAL(2) - DUEVAL(1)
      ENDIF

      IF(RRF.EQ.0.0) IBNDUF = 0

      IF(RRF.GT.0.0) IBNDUF = 2
      IF(RRF.LT.0.0) IBNDUF = 1

      DUEVLF = RRF

C--- Time to decide the bunch number

      IBUNFX = 1

C No TFBN info, choose DUET
      IF(P1OP2.EQ.0.0) THEN
        IF(IBNDUF.NE.0) IBUNFX = IBNDUF
        GOTO 200
      ENDIF

C No DUET info, choose TFBN
      IF(RRF.EQ.0.0) THEN
        IF(IBNTF.NE.0) IBUNFX = IBNTF
        GOTO 200
      ENDIF
     
C If both are avaialble, use TFBN matched with better DUET
      IF(TFBDLR(IBNDUF).EQ.0.0) THEN
        IF(IBNTF.NE.0) IBUNFX = IBNTF
        GOTO 200
      ENDIF

      IF(TFBDLR(IBNDUF).LT.0.0) IBUNFX = 1
      IF(TFBDLR(IBNDUF).GT.0.0) IBUNFX = 2

 200  CONTINUE

      IF(ABS(TFBNLR).LT.2.0.AND.ABS(TFBDLR(1)).LT.2.0.AND.
     *                          ABS(TFBDLR(2)).LT.2.0.AND.
     *       NTGDBN(IBNDUF).GE.2.AND.ABS(RRF).GE.0.2)
     *                                      IBUNFX = IBNDUF


      RETURN
      END

*
* $Id: trace_tfxfit.F,v 1.2 2000/05/26 16:48:40 dpp Exp $
*
* $Log: trace_tfxfit.F,v $
* Revision 1.2  2000/05/26 16:48:40  dpp
*     -> expanded tracer info
*
* Revision 1.1  2000/04/26 14:59:13  dpp
*      -> NEW
*      -> store character trace messages for TFXFIT
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :dpp
      SUBROUTINE TRACE_TFXFIT(OPTION,INPUT_HIT,N_USED,NUM1,NUM2,
     2     MESSAGE)
      IMPLICIT NONE
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/duseq/tfxfitcd.inc"

      CHARACTER*(*) OPTION
      INTEGER INPUT_HIT
      INTEGER N_USED
      INTEGER NUM1,NUM2
      CHARACTER*(*)MESSAGE

      CHARACTER*30 MESS_TRUNCATE
      CHARACTER*30 BLANKS
      DATA BLANKS/'                              '/
C                  123456789012345678901234567890
      INTEGER LEN_INPUT,LEN_RESID

C=========================================================================


      IF(OPTION.EQ.'CLEAR')THEN
         HIT_DISPLA=0
         CALL VZERO(N_TRACER,MHTFX)

      ELSEIF(OPTION.EQ.'QUIET')THEN
         HIT_DISPLA=0

      ELSEIF(OPTION.EQ.'LOG')THEN
         LEN_INPUT=LEN(MESSAGE)

         IF(LEN_INPUT.LE.0)THEN
            MESS_TRUNCATE=BLANKS
         ELSEIF(LEN_INPUT.LT.30)THEN
            LEN_RESID=30-LEN_INPUT
            MESS_TRUNCATE=MESSAGE(1:LEN_INPUT)//BLANKS(1:LEN_RESID)
         ELSE
            MESS_TRUNCATE=MESSAGE(1:30)
         ENDIF

         IF(N_TRACER(INPUT_HIT).GE. M_TRACER_LINES)THEN
            N_TRACER(INPUT_HIT)=M_TRACER_LINES

         ELSE
            N_TRACER(INPUT_HIT)=N_TRACER(INPUT_HIT)+1
            TRACER_NUM(1,N_TRACER(INPUT_HIT),INPUT_HIT)=NUM1
            TRACER_NUM(2,N_TRACER(INPUT_HIT),INPUT_HIT)=NUM2
            TRACER(N_TRACER(INPUT_HIT),INPUT_HIT)=MESS_TRUNCATE
         ENDIF

         N_USED=N_TRACER(INPUT_HIT)
      ENDIF

      RETURN
      END







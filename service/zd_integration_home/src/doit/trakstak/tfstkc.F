*
* $Id: tfstkc.F,v 1.4 2002/09/05 19:26:29 dpp Exp $
*
* $Log: tfstkc.F,v $
* Revision 1.4  2002/09/05 19:26:29  dpp
*      -> add code for new "wire fit" and "loc amb" stacks
*      -> change name of the TMP2 stack to LIM_RAD_O
*      -> add names for substak
*
* Revision 1.3  2002/05/23 14:30:17  dpp
*      -> fill new sub-stak limits
*      -> add diagnostic
*
* Revision 1.2  1999/08/03 21:59:55  dpp
* add index
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:43  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:56  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/36 21/04/93  16.07.54  by  Dan Peterson
*-- Author :
      SUBROUTINE TFSTKC(SUBSTAK)
C.......................................................................
C.
C. TFPUSH - clears the stack by zeroing NSTKTF
C.
C. COMMON    :
C.
C. Calls     :
C. Called    :
C. Author    : D. Peterson
C.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/controlpar.inc"
#include "seq/clutil/mesrep.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfstkc.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/09/05 19:26:29 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKC' )
      INTEGER SUBSTAK
      INTEGER ISTAK,BASE
      
      
      LOGICAL SATURATED
      INTEGER IOS

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------




      IF(SUBSTAK.EQ.0)THEN

#if defined(CLEO_DPPDIAG)
         PRINT 1001,SUBSTAK
 1001    FORMAT(' TFSTKC:',
     1        ' SUBSTAK=',I2,' clear all staks')
#endif

C zero the number of track representations in each sub-stak
C and set up the stack structure using parameters in controlpar.inc
         CALL VZERO(NSTKTF,NSTKTYP)
         CALL VZERO(GSTKLOCL,NSTKTYP)
         CALL VZERO(GSTKLOCM,NSTKTYP)
         
         SATURATED=.FALSE.
         BASE=0

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_PRIMARY
            LENSSTK(ISTAK)=LEN_STAK_PRIM
            NAMSSTK(ISTAK)=NAM_STAK_PRIM

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_SECONDARY
            LENSSTK(ISTAK)=LEN_STAK_SECO
            NAMSSTK(ISTAK)=NAM_STAK_SECO

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_TMP1
            LENSSTK(ISTAK)=LEN_STAK_TMP1
            NAMSSTK(ISTAK)=NAM_STAK_TMP1

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_LIM_RAD_O
            LENSSTK(ISTAK)=LEN_STAK_L_R_O
            NAMSSTK(ISTAK)=NAM_STAK_L_R_O

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_FT_WF_OUT
            LENSSTK(ISTAK)=LEN_STAK_WF_O
            NAMSSTK(ISTAK)=NAM_STAK_WF_O

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(.NOT.SATURATED)THEN
            ISTAK=STAK_FT_LA_OUT
            LENSSTK(ISTAK)=LEN_STAK_LA_O
            NAMSSTK(ISTAK)=NAM_STAK_LA_O

            GSTKLOCL(ISTAK)=BASE+1
            GSTKLOCM(ISTAK)=GSTKLOCL(ISTAK)+LENSSTK(ISTAK)-1
            BASE=GSTKLOCM(ISTAK)
            IF(GSTKLOCM(ISTAK).GT.NSTKDM)THEN
               SATURATED=.TRUE.
            ENDIF
         ENDIF

         IF(SATURATED)THEN
            WRITE(CHMESS,1101,IOSTAT=IOS)
 1101       FORMAT(' not enough room in stak for defined sub-stacks')
            CALL MESLOG('TFSTKC',1,MSERRO)
         ENDIF

      ELSE

#if defined(CLEO_DPPDIAG)
         PRINT 1002,SUBSTAK,NAMSSTK(SUBSTAK)
 1002    FORMAT(' TFSTKC:',
     1        ' SUBSTAK=',I2,3X,A15)
#endif

         NSTKTF(SUBSTAK) = 0
      ENDIF

      RETURN
      END





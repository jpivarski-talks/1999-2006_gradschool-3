*
* $Id: doit_layer_order.F,v 1.5 2002/09/05 19:12:20 dpp Exp $

*
* $Log: doit_layer_order.F,v $
* Revision 1.5  2002/09/05 19:12:20  dpp
*      -> merge in all of old tfgmi0
*
* Revision 1.4  1999/08/03 21:56:16  dpp
* fix printout
*
* Revision 1.3  1998/07/09 00:49:52  lyon
* removed gratuitous PRINT
*
* Revision 1.2  1998/07/09 00:48:35  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :

      SUBROUTINE DOIT_LAYER_ORDER
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"

C--> Do we want to use the new finding weights?
      LOGICAL NEWFWT


      INTEGER I
      INTEGER NLYRQD


C--> CHARACTER CODES FOR DEVICE TYPES
      DEVNAM(IPROP) = CRPROP
      DEVNAM(IDRFT) = CRDRFT
      DEVNAM(IVDET) = CRVDET
      DEVNAM(IVDCT) = CRVDCT
      DEVNAM(IDRCT) = CRDRCT
      DEVNAM(IIVDT) = CRIVDT
      DEVNAM(IPTLA) = CRPTLA
      DEVNAM(IPTLC) = CRPTLC
      DEVNAM(ISVX1) = CRSVX1
      DEVNAM(ISVZ1) = CRSVZ1
      DEVNAM(IZDA ) = CRZDA
      DEVNAM(ITPC1) = CRTPC1

C--> Loop over all layers, assigning appropriate data according to device
C     type; Order is SVX IV/PT, VD anodes, DR axial layers,
C     SVZ, VD cathodes, DR inner cathode, DR stereo layers, DR outer cathode.

C number of TF layers, now built here instead of copied from 
C      the cdgeom values NCANCD and NCATCD
      NLYRTF=0
      NCANTF=0
      NCATTF=0
      NLYRCF=0
      NLYRLF=0
      NLYRQD=0
     
C-->  definition: VD is treated as sum of VD plus PT(or IV)
      NLYRVD=0
      LLYRVD=1

C--> IXLRTF -- TF layer number given device and local layer number

      CALL VZERO(IXLRTF,KLYRTF*MXDVCD)
      
      IF (IDTOCD(ISVX1).GT.0) THEN
        DO 10 I=1,NSSVX1
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          LLYRVD=LLYRVD+1
          IXLRTF(I,ISVX1) = NLYRTF
 10     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(ISVX1),IDTOCD(ISVX1),NLYRTF
 3001   FORMAT(' DOIT_LAYER_ORDER:  found device ',A4,' (number',I4,
     1       ')  accumulated NLYRTF=',I4,' r-phi')
#endif
      ENDIF
      
      IF (IDTOCD(IPROP).GT.0) THEN
        DO 11 I=1,NCANIZ
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          LLYRVD=LLYRVD+1
          IXLRTF(I,IPROP) = NLYRTF
 11     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(IPROP),IDTOCD(IPROP),NLYRTF
#endif
      ENDIF
      
      IF (IDTOCD(IIVDT).GT.0) THEN
        DO 12,I=1,NCANIV
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          NLYRVD=NLYRVD+1
          IXLRTF(I,IIVDT) = NLYRTF
 12     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(IIVDT),IDTOCD(IIVDT),NLYRTF
#endif
      ENDIF
      
      IF (IDTOCD(IPTLA).GT.0) THEN
        DO 13,I=1,NCANPT
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          NLYRVD=NLYRVD+1
          IXLRTF(I,IPTLA) = NLYRTF
 13     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(IPTLA),IDTOCD(IPTLA),NLYRTF
#endif
      ENDIF
      
      
      IF (IDTOCD(IVDET).GT.0) THEN
        DO 30,I=1,NCANVD
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          NLYRVD=NLYRVD+1
          NLYRQD=NLYRQD+1
          IXLRTF(I,IVDET) = NLYRTF
 30     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(IVDET),IDTOCD(IVDET),NLYRTF
#endif
      ENDIF
      
      IF (IDTOCD(ITPC1).GT.0) THEN
        DO 41,I=1,NRDTPC
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          IXLRTF(I,ITPC1) = NLYRTF
 41     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(ITPC1),IDTOCD(ITPC1),NLYRTF
#endif
      ENDIF
      
C--> Pick up DR AXIAL anode layers
      IF (IDTOCD(IDRFT).GT.0) THEN
        DO 45,I=1,NCAXDR
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          IXLRTF(I,IDRFT) = NLYRTF
 45     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3001,DEVNAM(IDRFT),IDTOCD(IDRFT),NLYRTF
#endif
      ENDIF
      
      
      IF (IDTOCD(ISVZ1).GT.0) THEN
        DO 49,I=1,NSSVX1
          NLYRTF=NLYRTF+1
          NLYRLF=NLYRLF+1
          IXLRTF(I,ISVZ1) = NLYRTF
 49     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(ISVZ1),IDTOCD(ISVZ1),NLYRTF
 3003   FORMAT(' DOIT_LAYER_ORDER:  found device ',A4,' (number',I4,
     1       ')  accumulated NLYRTF=',I4,' s-Z')
#endif
      ENDIF
      
      
      IF (IDTOCD(IVDCT).GT.0) THEN
        DO 50,I=1,NCATVD
          NLYRTF=NLYRTF+1
          NLYRLF=NLYRLF+1
          NCATTF=NCATTF+1
          IXLRTF(I,IVDCT) = NLYRTF
 50     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(IVDCT),IDTOCD(IVDCT),NLYRTF
#endif
      ENDIF
      
C--> Pick up DR inner cathode
      IF ((IDTOCD(IDRCT).GT.0).and.(NCTIDR.GT.0)) THEN
        NLYRTF=NLYRTF+1
        NLYRLF=NLYRLF+1
        NCATTF=NCATTF+1
        IXLRTF(1,IDRCT) = NLYRTF
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(IDRCT),IDTOCD(IDRCT),NLYRTF
#endif
      ENDIF
      
      
C--> Pick up the ZD staereo layers
      IF (IDTOCD(IZDA).GT.0) THEN
        DO 55,I=1,NCANZD
          NLYRTF=NLYRTF+1
          NLYRCF=NLYRCF+1
          NCANTF=NCANTF+1
          NLYRVD=NLYRVD+1
          IXLRTF(I,IZDA) = NLYRTF
 55     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(IZDA),IDTOCD(IZDA),NLYRTF
#endif
      ENDIF
      
      
C--> Pick up DR STEREO anode layers
      IF (IDTOCD(IDRFT).GT.0) THEN
        DO 60,I=1,NCSTDR
          NLYRTF=NLYRTF+1
          NLYRLF=NLYRLF+1
          NCANTF=NCANTF+1
          IXLRTF(NCAXDR+I,IDRFT) = NLYRTF
 60     CONTINUE
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(IDRFT),IDTOCD(IDRFT),NLYRTF
#endif
      ENDIF
      
C--> Pick up DR outer cathode
      IF (IDTOCD(IDRCT).GT.0) THEN
        NLYRTF=NLYRTF+1
        NLYRLF=NLYRLF+1
        NCATTF=NCATTF+1
        IXLRTF(1+NCTIDR,IDRCT) = NLYRTF
#if defined (CLEO_DPPDIAG)
        PRINT 3003,DEVNAM(IDRCT),IDTOCD(IDRCT),NLYRTF
#endif
      ENDIF
      
C VD layers
      MLYRVD = LLYRVD -1 + NLYRVD
C DR axial layers
      LCAXDR = MLYRVD + 1
      MCAXDR = MLYRVD + NCAXDR
C double number layers for loop in and out
      NLR2TF = 2*(NLYRTF + NLYRQD)
C CF layers
      LLYRCF = 1
      MLYRCF = LLYRCF+NLYRCF-1
C LF layers
      LLYRLF = MLYRCF+1
      MLYRLF = LLYRLF+NLYRLF-1
      
#if defined (CLEO_DPPDIAG)
      PRINT 4001,NCANTF,NCATTF,NLYRTF,NLR2TF
 4001 FORMAT(' DOIT_LAYER_ORDER:'/
     1     '            NCANTF,NCATTF,NLYRTF,NLR2TF=',4I6)
      PRINT 4002,LLYRLF,MLYRLF,LLYRVD,MLYRVD
 4002 FORMAT(
     1     '            LLYRLF,MLYRLF,LLYRVD,MLYRVD=',4I6)
      PRINT 4003,LCAXDR,MCAXDR
 4003 FORMAT('            LCAXDR,MCAXDR       =',2I6)
#endif
      RETURN
      END
      







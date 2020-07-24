*
* $Id: doit_layer_order.F,v 1.4 1999/08/03 21:56:16 dpp Exp $
*
* $Log: doit_layer_order.F,v $
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


      INTEGER I,ILYR

C--> CHARACTER CODES FOR DEVICE TYPES
      DEVNAM(IPROP) = 'PROP'
      DEVNAM(IDRFT) = 'DRFT'
      DEVNAM(IVDET) = 'VDET'
      DEVNAM(IVDCT) = 'VDCATH'
      DEVNAM(IDRCT) = 'DRCATH'
      DEVNAM(IIVDT) = 'IVDT'
      DEVNAM(IPTLA) = 'PTLA'
      DEVNAM(IPTLC) = 'PTLC'
      DEVNAM(ISVX1) = 'SVX1'
      DEVNAM(ISVZ1) = 'SVZ1'


C--> Loop over all layers, assigning appropriate data according to device
C     type; Order is SVX IV/PT, VD anodes, DR axial layers,
C     SVZ, VD cathodes, DR inner cathode, DR stereo layers, DR outer cathode.

      ILYR = 0

C--> IXLRTF -- TF layer number given device and local layer number

      IF (IDTOCD(ISVX1).GT.0) THEN
          DO 9,I=1,NSSVX1
             ILYR = ILYR+1
             IXLRTF(I,ISVX1) = ILYR
9         CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3001,DEVNAM(ISVX1),IDTOCD(ISVX1),ILYR
3001      FORMAT(' DOIT_LAYER_ORDER:  found device ',A8,' (number',I4,
     1           ')  accumulated ILYR=',I4,' r-phi')
#endif
      ENDIF

      IF (IDTOCD(IIVDT).GT.0) THEN
          DO 10,I=1,NCANIV
             ILYR = ILYR+1
             IXLRTF(I,IIVDT) = ILYR
10        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IIVDT),IDTOCD(IIVDT),ILYR
3002      FORMAT(' DOIT_LAYER_ORDER:  found device ',A8,' (number',I4,
     1           ')  accumulated ILYR=',I4)
#endif
      ENDIF

      IF (IDTOCD(IPTLA).GT.0) THEN
          DO 20,I=1,NCANPT
             ILYR = ILYR+1
             IXLRTF(I,IPTLA) = ILYR
20        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IPTLA),IDTOCD(IPTLA),ILYR
#endif
      ENDIF
      IF (IDTOCD(IVDET).GT.0) THEN
          DO 30,I=1,NCANVD
             ILYR = ILYR+1
             IXLRTF(I,IVDET) = ILYR
30        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IVDET),IDTOCD(IVDET),ILYR
#endif
      ENDIF

      IF (IDTOCD(IDRFT).GT.0) THEN
          DO 40,I=1,NCAXDR
             ILYR = ILYR+1
             IXLRTF(I,IDRFT) = ILYR
40        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3001,DEVNAM(IDRFT),IDTOCD(IDRFT),ILYR
#endif
      ENDIF


      IF (IDTOCD(ISVZ1).GT.0) THEN
          DO 49,I=1,NSSVX1
             ILYR = ILYR+1
             IXLRTF(I,ISVZ1) = ILYR
49         CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3003,DEVNAM(ISVZ1),IDTOCD(ISVZ1),ILYR
3003      FORMAT(' DOIT_LAYER_ORDER:  found device ',A8,' (number',I4,
     1           ')  accumulated ILYR=',I4,' S-Z')
#endif
      ENDIF

      IF (IDTOCD(IVDCT).GT.0) THEN
          DO 50,I=1,NCATVD
             ILYR = ILYR+1
             IXLRTF(I,IVDCT) = ILYR
50        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IVDCT),IDTOCD(IVDCT),ILYR
#endif
      ENDIF

C--> Pick up DR inner cathode
      IF ((IDTOCD(IDRCT).GT.0).and.(NCTIDR.GT.0)) THEN
          ILYR = ILYR+1
          IXLRTF(1,IDRCT) = ILYR
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IDRCT),IDTOCD(IDRCT),ILYR
#endif
      ENDIF

      IF (IDTOCD(IDRFT).GT.0) THEN
          DO 60,I=1,NCSTDR
             ILYR = ILYR+1
             IXLRTF(NCAXDR+I,IDRFT) = ILYR
60        CONTINUE
#if defined (CLEO_DPPDIAG)
          PRINT 3003,DEVNAM(IDRFT),IDTOCD(IDRFT),ILYR
#endif
      ENDIF

C--> Pick up DR outer cathode
      IF (IDTOCD(IDRCT).GT.0) THEN
          ILYR = ILYR+1
          IXLRTF(1+NCTIDR,IDRCT) = ILYR
#if defined (CLEO_DPPDIAG)
          PRINT 3002,DEVNAM(IDRCT),IDTOCD(IDRCT),ILYR
#endif
      ENDIF


      RETURN
      END





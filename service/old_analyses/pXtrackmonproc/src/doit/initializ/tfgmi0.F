*
* $Id: tfgmi0.F,v 1.3 2000/12/19 23:34:22 dpp Exp $
*
* $Log: tfgmi0.F,v $
* Revision 1.3  2000/12/19 23:34:22  dpp
*      -> remove all references to LCSTDR,MCSTDR,MCTICD,LCTOCD
*
* Revision 1.2  1998/07/09 00:50:10  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.3  1995/09/25 19:42:19  zfiles
* (better) checking of device declaration before adding layers
*
* Revision 1.2  1995/09/06  00:04:32  zfiles
* Removed print statements.
*
* Revision 1.1.1.1  1994/10/08  01:00:55  zfiles
* first version of duet in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/26 17/05/94  13.44.20  by  Anders Ryd
*CMZ :  6.00/24 08/05/94  23.37.53  by  Anders Ryd
*CMZ :  5.54/07 04/11/91  12.47.27  by  Steve Schaffner
*CMZ :  5.52/00 11/01/90  22.40.19  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.22.53  by  Steve Schaffner
*-- Author :    Steve Schaffner   12/06/89
      SUBROUTINE TFGMI0
C.......................................................................
C.
C. TFGMI0 - Sets up useful geometry definitions.
C.
C.
C.
C. Called    : DUETBJ
C. Author    : Steve Schaffner  12/06/89  14.35.41
C.
C. These definitions are based on compile-time geometry selected; most
C. were done in PARAMETER statements in CLEOI version.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "seq/clutil/mesrep.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFGMI0' )
      INTEGER IOS

* ----------Executable code starts here---------------------

      WRITE(CHMESS,3001,IOSTAT=IOS) NSSVX1
3001  FORMAT(' NSSVX1=',I6)
      CALL MESLOG(CRNAME,1,MSINFO)
      WRITE(CHMESS,3002,IOSTAT=IOS) NCANIZ
3002  FORMAT(' NCANIZ=',I6)
      CALL MESLOG(CRNAME,1,MSINFO)

C VD layers

C--> A definition: VD is treated as sum of VD plus PT(or IV)
                              NLYRVD = 0
      IF (IDTOCD(IVDET).GT.0) NLYRVD = NLYRVD + NCANVD
      IF (IDTOCD(IPTLA).GT.0) NLYRVD = NLYRVD + NCANPT
      IF (IDTOCD(IIVDT).GT.0) NLYRVD = NLYRVD + NCANIV

                              LLYRVD = 1
      IF (IDTOCD(IPROP).GT.0) LLYRVD = LLYRVD + NCANIZ
      IF (IDTOCD(ISVX1).GT.0) LLYRVD = LLYRVD + NSSVX1
                              MLYRVD = LLYRVD -1 + NLYRVD


C DR axial layers

      LCAXDR = MLYRVD + 1
      MCAXDR = MLYRVD + NCAXDR



C TF layers
C--> Copy geometry values from CD

      NCANTF = NCANCD
      NCATTF = NCATCD
                              NLYRTF = NCANTF + NCATTF
      IF (IDTOCD(ISVX1).GT.0) NLYRTF=NLYRTF+2*NSSVX1
      NLR2TF = 2*(NLYRTF + NQDVCD)

C CF layers
C--> still not good, but should work (A. RYD)

                              NLYRCF = NCANIZ + NCAXDR + NLYRVD
      IF (IDTOCD(ISVX1).GT.0) NLYRCF = NLYRCF + NSSVX1
      LLYRCF = 1
      MLYRCF = LLYRCF+NLYRCF-1

C LF layers
C--> still not good, but should work (A. RYD)

                              NLYRLF = NCATCD+NCSTDR
      IF (IDTOCD(ISVZ1).GT.0) NLYRLF = NLYRLF + NSSVX1
      LLYRLF = MLYRCF+1
      MLYRLF = LLYRLF+NLYRLF-1
      NFETLF = NLYRLF*(NLYRLF+1)/2


#if defined (CLEO_DPPDIAG)
      PRINT 4001,NCANTF,NCATTF,NLYRTF,NLR2TF
4001  FORMAT(' IN TFGMI0: NCANTF,NCATTF,NLYRTF,NLR2TF=',4I6)
      PRINT 4002,LLYRLF,MLYRLF,LLYRVD,MLYRVD
4002  FORMAT('            LLYRLF,MLYRLF,LLYRVD,MLYRVD=',4I6)
      PRINT 4003,LCAXDR,MCAXDR
4003  FORMAT('            LCAXDR,MCAXDR       =',2I6)
      PRINT 4004,NFETLF
4004  FORMAT('            NFETLF=',I6)
#endif

      RETURN
      END










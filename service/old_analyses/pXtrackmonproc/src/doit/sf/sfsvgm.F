*
* $Id: sfsvgm.F,v 1.2 2001/11/20 17:54:26 dpp Exp $
*
* $Log: sfsvgm.F,v $
* Revision 1.2  2001/11/20 17:54:26  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1  1997/02/25 22:04:23  dpp
* first version: define SVX geometry as far as SF cares
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :  DAN PETERSON
      SUBROUTINE SFSVGM(SHOWCL)
C.......................................................................
C.
C. SFSVGM - define the SVX geometry as used in SF
C.
C. COMMON    : /STEPCn/ and others
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 
C. LAST MOD  : 
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "seq/clutil/mesrep.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"

C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL

      CHARACTER*(*) SHOWCL
      INTEGER LENSC ,IOS

      INTEGER INDXTM,MHITTM,LAYRTM

      REAL PI,TWOPI
      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=2.*PI)

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      LENSC=LEN(SHOWCL)
      WRITE(CHMESS,1101,IOSTAT=IOS)SHOWCL(1:LENSC)
1101  FORMAT(' entry: called from',A20)
      CALL MESLOG('SFSVGM',1,MSSUCC)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C number of layers
      SFSVNL=3

C device
      DO 19 LAYRTM=1,SFSVNL
19    SFSVDV(LAYRTM)=ISVXSP

C radius
      SFSVRD(1)=.0235
      SFSVRD(2)=.0325
      SFSVRD(3)=.0475

C number of cells per layer
      SFSVCL(1)=24
      SFSVCL(2)=32
      SFSVCL(3)=48

C phi zero (offsets were taken from a cleoxd plot; using a protractor)
      SFSVF0(1)=.555 -1.0 * TWOPI/SFSVCL(1)
      SFSVF0(2)=.522 -1.5 * TWOPI/SFSVCL(2)
      SFSVF0(3)=.513 +2.5 * TWOPI/SFSVCL(3)

C pointer from SV layer to SF layer
C     SFLASV(svx_layer) is filled in subroutine SFDEFN


C index in SV map of first cell in SV layer
      INDXTM=0
      DO 79 LAYRTM=1,SFSVNL
      
      IF(INDXTM.GE.SFSVMC)THEN
          MHITTM=SFSVMC
          WRITE(CHMESS,1102,IOSTAT=IOS)INDXTM,MHITTM
1102      FORMAT(' *** index excedes SFSVMC:',I7,I7)
          CALL MESLOG('SFSVGM',1,MSERRO)
      ENDIF

C west
      SFSVIN(LAYRTM,1)=INDXTM+1
      INDXTM=INDXTM+SFSVCL(LAYRTM)
C east
      SFSVIN(LAYRTM,2)=INDXTM+1
      INDXTM=INDXTM+SFSVCL(LAYRTM)
79    CONTINUE

      MHITTM=SFSVMC
      WRITE(CHMESS,1103,IOSTAT=IOS)INDXTM,MHITTM
1103  FORMAT(' have used ',I7,' locations, out of SFSVMC=',I7)
      CALL MESLOG('SFSVGM',1,MSSUCC)

      RETURN
      END












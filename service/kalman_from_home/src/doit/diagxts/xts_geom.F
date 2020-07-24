*
* $Id: xts_geom.F,v 1.8 2000/10/24 22:32:17 dpp Exp $
*
* $Log: xts_geom.F,v $
* Revision 1.8  2000/10/24 22:32:17  dpp
*      -> fix bug in boundaries
*
* Revision 1.7  2000/10/02 20:55:27  dpp
*      -> boundaries for silicon
*
* Revision 1.6  2000/04/26 14:52:31  dpp
*      -> put in border of silicon
*
* Revision 1.5  2000/02/12 20:24:23  dpp
*      -> define superlayer border radii for display
*
* Revision 1.4  1999/01/29 15:34:39  dpp
* force update
*
* Revision 1.3  1998/07/09 00:33:21  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.2  1998/05/08 14:13:48  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.5  1997/02/25 21:37:33  dpp
* fill XTS geom with CD layers and SF layers that are not CD layers
*
* Revision 1.4  1996/02/14 20:33:23  zfiles
* moved initialization of SFLACD to SFDEFN
*
* Revision 1.3  1995/09/25 19:20:25  zfiles
* compatibility with missing PT
*
* Revision 1.2  1995/04/20  22:38:54  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 10/05/94  17.48.42  by  Anders Ryd
*CMZ :  6.00/14 01/03/94  17.00.57  by  Rob Kutschke
*CMZ :  3.01/01 23/03/90  12.38.39  by  Dan Peterson
*-- Author :dpp


      SUBROUTINE XTS_GEOM(CALLER)
      IMPLICIT NONE
#include "doit/duseq/tfindpar.inc"
C#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "cl3seq/cdgm3/cdraw1in.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"
#include "doit/sfseq/sffix.inc"
#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      CHARACTER*(*) CALLER

      REAL PI,TWOPI,PIO2,PIO3,PIO6

      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=PI*2.)
      PARAMETER (PIO2=PI/2., PIO3=PI/3., PIO6=PI/6. )

      REAL PHI,ANG
      INTEGER I,LYRCD,LYRSF,IWIR,J,I2,ICKLIM
      INTEGER IMIN,IMAX,ARROW
      INTEGER IDEVICE,FIRSTLAYER,LASTLAYER,PATH
      CHARACTER*10 LABEL
      REAL CELLSIZE
      INTEGER ITEMP,IWIRS
      INTEGER ICHECK(MCD)
      REAL XP,YP,CPHI,SPHI,XPC,XPS,YPC,YPS
      INTEGER READO2
      REAL RDISP,SVXSEP
      INTEGER MCDTMP

#if defined(CLEO_XTSDEBUG)
      PRINT 1004,CALLER
1004  FORMAT(' XTS_GEOM: ENTER: CALLED FROM ',A20)
#endif



C boundries of ALL the detectors
      NBOUNDS=0
      CALL VZERO(RBOUNDS,MBOUNDS)

C boundries of the CD detectors
      IF(MDEVCD.GT.0)THEN
         DO 27 IDEVICE=1,MDEVCD
            IF    (IDFRCD(IDEVICE).EQ.IPTLA)THEN
               PATH=1
               LABEL='PTL,       '
            ELSEIF(IDFRCD(IDEVICE).EQ.IVDET)THEN
               PATH=1
               LABEL='VD,        '
            ELSEIF(IDFRCD(IDEVICE).EQ.IDRFT)THEN
               PATH=1
               LABEL='DR2,       '
            ELSEIF(IDFRCD(IDEVICE).EQ.IDRCT)THEN
               PATH=2
               LABEL='DR2cathode,'
            ELSEIF(IDFRCD(IDEVICE).EQ.IVDCT)THEN
               PATH=2
               LABEL='VD cathode,'
            ELSEIF(IDFRCD(IDEVICE).EQ.ISVX1)THEN
               PATH=2
               LABEL='SVX r-phi, '
            ELSEIF(IDFRCD(IDEVICE).EQ.ISVZ1)THEN
               PATH=2
               LABEL='SVX s-z,   '
            ELSE
               PATH=0
            ENDIF
            
            IF(PATH.NE.0)THEN
#if defined(CLEO_XTSDEBUG)
               PRINT 9904,LABEL,IDEVICE
 9904          FORMAT(' XTS_GEOM: found ',A11,' device number=',I4)
#endif
               IF(PATH.EQ.1)THEN
                  IF(NBOUNDS.LE.MBOUNDS-2)THEN
                     NBOUNDS=NBOUNDS+2
                     
                     FIRSTLAYER=IDVLCD(1,IDEVICE)
                     CELLSIZE=RCD(FIRSTLAYER)*CELLCD(FIRSTLAYER)/2.
                     RBOUNDS(NBOUNDS-1)=RCD(FIRSTLAYER)-CELLSIZE*1.1
                     TYP_BOUND(NBOUNDS-1)=1
                     
                     LASTLAYER= IDVLCD(2,IDEVICE)
                     CELLSIZE=RCD(LASTLAYER )*CELLCD(LASTLAYER )/2.
                     RBOUNDS(NBOUNDS  )=RCD(LASTLAYER )+CELLSIZE*1.1
                     TYP_BOUND(NBOUNDS  )=1
                  ENDIF
               ENDIF
               IF(IDFRCD(IDEVICE).EQ.IDRFT)THEN
                  IF(NBOUNDS.LE.MBOUNDS-12)THEN
                     NBOUNDS=NBOUNDS+12
                     RBOUNDS  (NBOUNDS-11)=.18139
                     TYP_BOUND(NBOUNDS-11)=2
                     RBOUNDS  (NBOUNDS-10)=.23747
                     TYP_BOUND(NBOUNDS-10)=2
                     RBOUNDS  (NBOUNDS- 9)=.29338
                     TYP_BOUND(NBOUNDS- 9)=2
                     RBOUNDS  (NBOUNDS- 8)=.34900
                     TYP_BOUND(NBOUNDS- 8)=2
                     RBOUNDS  (NBOUNDS- 7)=.36295
                     TYP_BOUND(NBOUNDS- 7)=2
                     RBOUNDS  (NBOUNDS- 6)=.41909
                     TYP_BOUND(NBOUNDS- 6)=2
                     RBOUNDS  (NBOUNDS- 5)=.47517
                     TYP_BOUND(NBOUNDS- 5)=2
                     RBOUNDS  (NBOUNDS- 4)=.53121
                     TYP_BOUND(NBOUNDS- 4)=2
                     RBOUNDS  (NBOUNDS- 3)=.58722
                     TYP_BOUND(NBOUNDS- 3)=2
                     RBOUNDS  (NBOUNDS- 2)=.64320
                     TYP_BOUND(NBOUNDS- 2)=2
                     RBOUNDS  (NBOUNDS- 1)=.69916
                     TYP_BOUND(NBOUNDS- 1)=2
                     RBOUNDS  (NBOUNDS   )=.75510
                     TYP_BOUND(NBOUNDS   )=2
                  ENDIF
               ENDIF

               IF(IDFRCD(IDEVICE).EQ.ISVX1)THEN
                  IF(NBOUNDS.LE.MBOUNDS-2)THEN
                     NBOUNDS=NBOUNDS+2
                     RBOUNDS  (NBOUNDS- 1)=.020
                     TYP_BOUND(NBOUNDS- 1)=2
                     RBOUNDS  (NBOUNDS   )=.105
                     TYP_BOUND(NBOUNDS   )=2
                  ENDIF
               ENDIF
               
            ENDIF
 27   CONTINUE
      ENDIF
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C boundries of the SV spacifier bit  detector
      IF(SFSVNL.LE.0)GO TO 39
      IF(NBOUNDS.GT.MBOUNDS-2)GO TO 39
      NBOUNDS=NBOUNDS+2
      FIRSTLAYER=1
      CELLSIZE=.005
      RBOUNDS(NBOUNDS-1)=SFSVRD(FIRSTLAYER)-CELLSIZE*1.1
      LASTLAYER= SFSVNL
      CELLSIZE=.005
      RBOUNDS(NBOUNDS  )=SFSVRD(LASTLAYER )+CELLSIZE*1.1
39    CONTINUE


C coordinates of each element in the XTS geometry

      CALL VZERO(XCENTRS ,MCD)
      CALL VZERO(YCENTRS ,MCD)
      CALL VZERO(SIZHIT  ,MCD)
      CALL VZERO(ICHECK  ,MCD)
      ICKLIM=0
      NCDXTS=0

C coordinates of each element in the CD geometry
      DO 69 LYRCD=LCANCD,MCANCD
      INXSCD(LYRCD)=NCDXTS+1
      IWIRS=NWIRCD(LYRCD)-1

      DO 67 IWIR=0,IWIRS
      I=INXSCD(LYRCD)+IWIR
      PHI=TWOPI*IWIR/NWIRCD(LYRCD)+PHIFCD(LYRCD)
     1                             +FFIXSF(LYRCD)
      CPHI=COS(PHI)
      SPHI=SIN(PHI)
      XCENTRS(I)=RCD(LYRCD)*COS(PHI)
      YCENTRS(I)=RCD(LYRCD)*SIN(PHI)
      SIZHIT(I)= RCD(LYRCD)*CELLCD(LYRCD)*.5
      ICHECK(I)=ICHECK(I)+1
      IF(I.GT.ICKLIM)ICKLIM=I

      IF(IDVCCD(LYRCD).EQ.IPTLA)GO TO 51
      IF(IDVCCD(LYRCD).EQ.IVDET)GO TO 53
      IF(IDVCCD(LYRCD).EQ.IDRFT)GO TO 56
      GO TO 67

C PT
51    XCORNR(1,I)=SIZHIT(I)*2.
      YCORNR(1,I)=9.
      GO TO 67
C VD
53    ANG=PHI+PIO6
      DO 54 J=1,6
      XCORNR(J,I)=XCENTRS(I)+SIZHIT(I)*COS(ANG)
      YCORNR(J,I)=YCENTRS(I)+SIZHIT(I)*SIN(ANG)
54    ANG=ANG+PIO3

C VD EAST
      I2=I+NWIRCD(LYRCD)
      XCENTRS(I2)=XCENTRS(I)
      YCENTRS(I2)=YCENTRS(I)
      SIZHIT(I2)=SIZHIT(I)
      ICHECK(I2)=ICHECK(I2)+1
      IF(I2.GT.ICKLIM)ICKLIM=I2

      DO 55 J=1,6
      XCORNR(J,I2)=XCORNR(J,I)
      YCORNR(J,I2)=YCORNR(J,I)
55    CONTINUE
      GO TO 67

C DR
56    CONTINUE
      XP=.0065
      YP=0.95*SIZHIT(I)
      XPC=XP*CPHI
      XPS=XP*SPHI
      YPC=YP*CPHI
      YPS=YP*SPHI
      XCORNR(1,I)=XCENTRS(I)+XPC-YPS
      YCORNR(1,I)=YCENTRS(I)+XPS+YPC
      XCORNR(2,I)=XCENTRS(I)-XPC-YPS
      YCORNR(2,I)=YCENTRS(I)-XPS+YPC
      XCORNR(3,I)=XCENTRS(I)-XPC+YPS
      YCORNR(3,I)=YCENTRS(I)-XPS-YPC
      XCORNR(4,I)=XCENTRS(I)+XPC+YPS
      YCORNR(4,I)=YCENTRS(I)+XPS-YPC
      YCORNR(5,I)=9.

67    CONTINUE
      NCDXTS=NCDXTS+IWIRS+1
        IF(IDVCCD(LYRCD).EQ.IVDET)NCDXTS=NCDXTS+IWIRS+1
      IF(NCDXTS.GT.MCD)GO TO 301
      PRINT 9901,LYRCD,INXSCD(LYRCD),ICKLIM
9901  FORMAT(' INXSCD(cdlayer:',I3,')=',I6,'  ICKLIM=',I6)
69    CONTINUE

C coordinates of each element in the SF geometry BUT NOT in the CD geometry
      DO 89 LYRSF=1,NLAYSF
      LYRCD=CDLASF(LYRSF)
      IF(LYRCD.GT.0)THEN
            INXSSF(LYRSF)=INXSCD(LYRCD)
      ELSE
            INXSSF(LYRSF)=NCDXTS+1

C now create the new layer in XTS for the non-CD layer
            IWIRS=SFSVCL(-LYRCD)-1

            DO 87 IWIR=0,IWIRS

            CELLSIZE=TWOPI/SFSVCL(-LYRCD)
            PHI=CELLSIZE*IWIR+SFSVF0(-LYRCD)
            CPHI=COS(PHI)
            SPHI=SIN(PHI)

            I=INXSSF(LYRSF)+IWIR
            SIZHIT(I)= SFSVRD(-LYRCD)*CELLSIZE*.5
            ICHECK(I)=ICHECK(I)+1
            IF(I.GT.ICKLIM)ICKLIM=I

            IF(SFSVDV(-LYRCD).EQ.ISVXSP)GO TO 71
            GO TO 87

C SVX SPARCIFIER BITS
71          SVXSEP=.002
            XP=.0025
            YP=0.95*SIZHIT(I)
            XPC=XP*CPHI
            XPS=XP*SPHI
            YPC=YP*CPHI
            YPS=YP*SPHI

            RDISP=SFSVRD(-LYRCD)-SVXSEP
            READO2=1
72          XCENTRS(I)=RDISP*COS(PHI)
            YCENTRS(I)=RDISP*SIN(PHI)
            XCORNR(1,I)=XCENTRS(I)+XPC-YPS
            YCORNR(1,I)=YCENTRS(I)+XPS+YPC
            XCORNR(2,I)=XCENTRS(I)-XPC-YPS
            YCORNR(2,I)=YCENTRS(I)-XPS+YPC
            XCORNR(3,I)=XCENTRS(I)-XPC+YPS
            YCORNR(3,I)=YCENTRS(I)-XPS-YPC
            XCORNR(4,I)=XCENTRS(I)+XPC+YPS
            YCORNR(4,I)=YCENTRS(I)+XPS-YPC
            YCORNR(5,I)=9.
            IF(READO2.EQ.2)GO TO 87

C SVX SPARCIFIER BITS ---  EAST
            I2=I+SFSVCL(-LYRCD)
            SIZHIT(I2)=SIZHIT(I)
            ICHECK(I2)=ICHECK(I2)+1
            IF(I2.GT.ICKLIM)ICKLIM=I2
            I=I2

            RDISP=SFSVRD(-LYRCD)+SVXSEP
            READO2=2
            GO TO 72

87          CONTINUE
            NCDXTS=NCDXTS+IWIRS+1
              IF(SFSVDV(-LYRCD).EQ.ISVXSP)NCDXTS=NCDXTS+IWIRS+1
            IF(NCDXTS.GT.MCD)GO TO 301
      PRINT 9902,LYRSF,INXSSF(LYRSF),ICKLIM
9902  FORMAT(' INXSSF(sflayer:',I3,')=',I6,'  ICKLIM=',I6)
      ENDIF
89    CONTINUE


C CHECK THAT IT IS RIGHT

      DO 201 I=1,ICKLIM
      IF(ICHECK(I).EQ.1)GO TO 201
      PRINT 1001,I,ICHECK(I)
1001  FORMAT(' XTS_GEOM: <<<<ERROR>>>> ICHECK(',I5,')=',I10)
201   CONTINUE
      GO TO  401

301   CONTINUE
      MCDTMP=MCD
      PRINT 1005,NCDXTS,MCDTMP
1005  FORMAT(' XTS_GEOM: <<<<ERROR>>>> used',I6,' outof',I6,' EXIT')
      GO TO 501

401   CONTINUE
#if defined(CLEO_XTSDEBUG)
      MCDTMP=MCD
      PRINT 1002,NCDXTS,MCDTMP
1002  FORMAT(' XTS_GEOM: used',I6,' outof',I6,
     2       ' COMPLETED WITH CHECK, EXIT')
#endif

501   RETURN
      END









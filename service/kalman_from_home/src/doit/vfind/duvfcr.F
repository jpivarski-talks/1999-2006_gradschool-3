*
* $Id: duvfcr.F,v 1.2 2001/11/20 00:16:54 dpp Exp $
*
* $Log: duvfcr.F,v $
* Revision 1.2  2001/11/20 00:16:54  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:50  zfiles
* first version of doit in CVS
*
*
*--------------------------------------------------------
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 07/01/92  12.55.00  by  Steve Schaffner
*CMZ :          10/10/91  16.36.09  by  Jon Urheim
* minor mod to facillitate compilation on ibm rs6000.
*CMZ :  5.54/04 23/09/91  11.41.59  by  Steve Schaffner
*-- Author :    Hitoshi Yamamoto   07/08/91
       SUBROUTINE DUVFCR(LDBG,I,J,T,NVS,XYZV)
*
* finds crossing points of two tracks I,J in x-y plane and
* returns the vertex candidates if z-view also is consistent.

#if defined(CLEO_TYPCHK)
       IMPLICIT NONE
#endif
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/anltmp.inc"

*--------- arguments ------------------------------------------------
*     I,J .... index of tracks in the track bank T (not the standard
*              track id.
*     T .......track parameter bank
*     NVS ....number of vertex candidates found (up to 2)
*     XYZV(3,2)...vertex candidates

       INTEGER I,J,NVS
       REAL XYZV(3,2)

       REAL T(MTRKDR,12)

       INTEGER XC,YC,RC,FC,CC,ZC,PC,HC,DAC,F0C,DZC,DDC
       PARAMETER (XC=1,YC=2,RC=3,FC=4,CC=5,ZC=6,PC=7
     +           ,HC=8,DAC=9,F0C=10,DZC=11,DDC=12)

*--------- misc local stuff
       INTEGER NX,IV,LDBG
       REAL D,RI,RJ,DMAX,DMIN,PI,CDFI,CDFJ,DELFI,DELFJ
       REAL FI12,FI(2),FJ(2)
       REAL XVI,YVI,ZVI,XVJ,YVJ,ZVJ,XAV,YAV,ZAV,DZI,DZJ
       REAL DCUT,ZCUT,RMAX,ZMAX
       REAL DLF,DLA,DLFCUT,DLACUT

*---- data and save
       SAVE DCUT,RMAX,ZMAX,PI
       DATA DCUT/.003/,RMAX/0.2/,ZMAX/0.4/
       DATA DLFCUT/0.03/,DLACUT/0.003/
       DATA PI/3.14159265/

*===================================================
* ----------Executable code starts here---------------------

       NVS = 0

       D = SQRT((T(I,XC)-T(J,XC))**2 + (T(I,YC)-T(J,YC))**2)
       RI = ABS(T(I,RC))
       RJ = ABS(T(J,RC))

       DMAX = RI+RJ
       DMIN = ABS(RI-RJ)

       IF(LDBG.GE.2) THEN
         PRINT 1100,I,J,RI,RJ,D,DMAX,DMIN
 1100    FORMAT(/' I,J=',2I3,' RI,J=',2F8.4,' D,MAX,MIN=',3F8.4)
       ENDIF

*----- First, check if the trks are back-to-back and close at
*      the PCA. If so, take the average PCA as vertex.

       DLF = T(I,F0C)-T(J,F0C)
       DLF = AMOD(DLF+4.*PI,2.*PI)-PI

       IF(ABS(DLF).LT.DLFCUT) THEN
         DLA = T(I,DAC)+T(J,DAC)
         CALL HF1(920,DLA,1.)
         IF(ABS(DLA).LT.DLACUT) THEN
           FI(1) = 0.
           FJ(1) = 0.
           NX = 1
           GOTO 100
         ENDIF
       ENDIF

       IF(D.GE.DMAX+DCUT .OR. D.LE.DMIN-DCUT) THEN
*-----  2 circles clearly don't meet. Quit.

         GOTO 1

       ENDIF

         IF(T(I,YC)-T(J,YC).EQ.0. .AND.
     +      T(I,XC)-T(J,XC).EQ.0.) THEN
           GOTO 1
         ELSE
           FI12 = ATAN2(T(I,YC)-T(J,YC),T(I,XC)-T(J,XC))
         ENDIF

         IF(D.GE.DMAX-DCUT .OR. D.LE.DMIN+DCUT) THEN
*--------  did not quite cross, but close. This includes gamma
*          conversions.

           GOTO 1

         ELSE
*--------  they actually crossed - at two points
*                 ----- 0<DELF<pi -----
           CDFI = (D**2 + RI**2 - RJ**2)/(2.*D*RI)
           CDFJ = (D**2 + RJ**2 - RI**2)/(2.*D*RJ)
           IF (ABS(CDFI).GT.1.0) CDFI = SIGN(1.,CDFI)
           IF (ABS(CDFJ).GT.1.0) CDFJ = SIGN(1.,CDFJ)
           DELFI = ACOS(CDFI)
           DELFJ = ACOS(CDFJ)
           FI(1) = FI12+PI+DELFI-T(I,FC)
           FJ(1) = FI12-DELFJ   -T(J,FC)
           FI(2) = FI12+PI-DELFI-T(I,FC)
           FJ(2) = FI12+DELFJ   -T(J,FC)
           NX = 2

         ENDIF

*------ now check the z-view for consistency. limit to track segments
*       +- pi around PCA.

 100     DO 10 IV = 1,NX
           FI(IV) = AMOD(FI(IV)+5.*PI,2.*PI)-PI
           FJ(IV) = AMOD(FJ(IV)+5.*PI,2.*PI)-PI
           XVI = T(I,XC)+T(I,RC)*COS(FI(IV)+T(I,FC))
           XVJ = T(J,XC)+T(J,RC)*COS(FJ(IV)+T(J,FC))
           YVI = T(I,YC)+T(I,RC)*SIN(FI(IV)+T(I,FC))
           YVJ = T(J,YC)+T(J,RC)*SIN(FJ(IV)+T(J,FC))
           ZVI = T(I,ZC)+T(I,CC)*T(I,RC)*FI(IV)*SIGN(1.,T(I,PC))
           ZVJ = T(J,ZC)+T(J,CC)*T(J,RC)*FJ(IV)*SIGN(1.,T(J,PC))

           IF(LDBG.GE.2) THEN
             PRINT 1000,IV
     +        ,T(I,PC),FI(IV),XVI,YVI,ZVI,T(I,HC),T(I,DZC)
     +        ,T(J,PC),FJ(IV),XVJ,YVJ,ZVJ,T(J,HC),T(J,DZC)
 1000        FORMAT(' IV=',I2
     +  ,' P=',F7.3,' FI=',F7.3,' V=',3F7.3,' H=',F5.0,' DZ=',F7.3
     +/7X,'P=',F7.3,' FJ=',F7.3,' V=',3F7.3,' H=',F5.0,' DZ=',F7.3)
           ENDIF

*------ store them if zcut is OK. Actually there is no need to
*       take average over I,J for x,y for most cases; but so what?
*       Take a weighted average for Z position.

           DZI = AMAX1(0.001,T(I,DZC))
           DZJ = AMAX1(0.001,T(J,DZC))
           ZCUT = 20.*SQRT(DZI**2 + DZJ**2)

           IF(ABS(ZVI-ZVJ).LT.ZCUT) THEN
             XAV = 0.5*(XVI+XVJ)
             YAV = 0.5*(YVI+YVJ)
             ZAV = (ZVI/DZI**2+ZVJ/DZJ**2)/(1./DZI**2+1./DZJ**2)

             IF(ABS(ZAV).LT.ZMAX .AND.
     +          XAV**2+YAV**2.LT.RMAX**2) THEN
               NVS = NVS+1
               XYZV(1,NVS) = XAV
               XYZV(2,NVS) = YAV
               XYZV(3,NVS) = ZAV
             ENDIF
           ENDIF

 10      CONTINUE

       IF(LDBG.GE.1) THEN
         PRINT *,' <PMFIND> I,J=',I,J,' NVS=',NVS
         IF(NVS.GT.0) THEN
           DO 25 IV=1,NVS
             PRINT 1030,IV,(XYZV(NX,IV),NX=1,3)
 1030        FORMAT(I3,' XYZ-V=',3F8.4)
 25        CONTINUE
         ENDIF
       ENDIF

 1     RETURN
       END

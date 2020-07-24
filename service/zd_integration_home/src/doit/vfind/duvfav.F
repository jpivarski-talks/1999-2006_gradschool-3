*
* $Id: duvfav.F,v 1.2 2001/11/20 00:16:28 dpp Exp $
*
* $Log: duvfav.F,v $
* Revision 1.2  2001/11/20 00:16:28  dpp
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
*CMZ :  5.54/07 23/09/91  11.41.59  by  Steve Schaffner
*-- Author :

*CMZ :          11/09/91  18.32.08  by  Steve Schaffner
*-- Author :

*CMZ :          11/09/91  18.07.50  by  Steve Schaffner
*-- Author :    Hitoshi Yamamoto   07/08/91
       SUBROUTINE DUVFAV(NPTK,IPTK,T,PMVTX,ERVTX)
*
*  Crude primary vertex finder. Takes a simple average of PCA of
*  given list of tracks. For Z, a weighted average is taken.
*
* arguments: NPTK,IPTK(MTRKDR) ... track list
*            T(MTRKDR,12)     .... track parameter bank
*            PMVTX(3) ............ primary vertex
*            ERVTX(3) ............ error in primary vertex (crude est)
*

#if defined(CLEO_TYPCHK)
       IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/anltmp.inc"

       INTEGER NPTK,IPTK(MTRKDR)
       REAL T(MTRKDR,12),PMVTX(3),ERVTX(3)
*
* misc local stuff.
       REAL TEMX,TEMY,TEMZ,WTZ,WTZTOT,ERA,ERZ
       INTEGER I,J

       INTEGER XC,YC,RC,FC,CC,ZC,PC,HC,DAC,F0C,DZC,DDC
       PARAMETER (XC=1,YC=2,RC=3,FC=4,CC=5,ZC=6,PC=7
     +           ,HC=8,DAC=9,F0C=10,DZC=11,DDC=12)

* ----------Executable code starts here---------------------

       TEMX = 0.
       TEMY = 0.
       TEMZ = 0.
       ERA = 0.
       ERZ = 0.
       WTZTOT  = 0.
       DO 20 I = 1,NPTK
         J = IPTK(I)
         TEMX = TEMX + T(J,XC)+T(J,RC)*COS(T(J,FC))
         TEMY = TEMY + T(J,YC)+T(J,RC)*SIN(T(J,FC))
         WTZ  = 1./AMAX1(.001,T(J,DZC))**2
         TEMZ = TEMZ + WTZ*T(J,ZC)
         WTZTOT = WTZTOT + WTZ

         ERA = ERA + T(J,DDC)**2
         ERZ = ERZ + T(J,DZC)**2
 20    CONTINUE

       PMVTX(1) = TEMX/FLOAT(NPTK)
       PMVTX(2) = TEMY/FLOAT(NPTK)
       PMVTX(3) = TEMZ/WTZTOT

       ERVTX(1) = SQRT(ERA)/FLOAT(NPTK)
       ERVTX(2) = ERVTX(1)
       ERVTX(3) = SQRT(ERZ)/FLOAT(NPTK)

       RETURN
       END

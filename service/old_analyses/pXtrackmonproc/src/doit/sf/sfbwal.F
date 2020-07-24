*
* $Id: sfbwal.F,v 1.3 1999/11/19 22:52:13 dpp Exp $
*
* $Log: sfbwal.F,v $
* Revision 1.3  1999/11/19 22:52:13  dpp
* install output for cleo3 startup running
*
* Revision 1.2  1998/07/09 00:55:11  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.7  1997/02/25 21:44:55  dpp
* use SVX layer in addition to CD layers
*
* Revision 1.6  1996/04/03 16:10:05  dpp
* cut tuning, do cosmic test on same sign pairs, argument indicates trial
*
* Revision 1.5  1996/03/28 20:30:06  dpp
* tuning; and implement requirements of hits in the VD
*
* Revision 1.4  1996/03/20 17:10:31  zfiles
* changed various cuts, and added dedicated cosmic killer
*
* Revision 1.3  1996/03/14 01:44:38  zfiles
* added compilation switch on print statement
*
* Revision 1.2  1996/03/06 22:03:57  zfiles
* major revisions to SFBWAL (first real working version); vertex calculation
* SFNEXT compatible with SFBWAL
*
* Revision 1.1  1996/02/29 17:35:19  zfiles
* determine event quality for level3
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON
      SUBROUTINE SFBWAL(GBU)
C......................................................................
C.
C. SFBWAL - use the SFIND routines to identify beam wall events
C.
C. COMMON    : 
C. CALLS     : SFIND, SFNEXT
C. CALLED    : 
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 19-Feb-96
C. LAST MOD  :
C.
C. Modification Log.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE

#endif
      SAVE
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfbwal_access.inc"
#include "doit/sfseq/sfsvcom.inc"
#include "seq/clutil/mesrep.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

      CHARACTER*6  CRNAME
      PARAMETER    (CRNAME='SFBWAL')

      REAL PI,TWOPI,HAFPI,QARPI,EIGPI,TH4PI,TH8PI,FT8PI,ROOT2
      PARAMETER (PI=3.14159265)
      PARAMETER (TWOPI=2.*PI)
      PARAMETER (HAFPI=PI/2. , QARPI=PI/4., EIGPI=PI/8.)
      PARAMETER (TH4PI=QARPI*3.)
      PARAMETER (TH8PI=EIGPI*3.,FT8PI=EIGPI*15.)
      PARAMETER (ROOT2=1.4142136)


C.. PICK .....SF chain number from
C.. L1 .......beginning layer
C.. L2 .......ending layer
C.. W1 .......wire number in beginning layer
C.. W2 .......wire number in ending layer
C.. CDLAY1 ...CD layer of beginning layer
C.. CDLAY2 ...CD layer of ending layer
C.. RADU1.....radius of beginning layer
C.. RADU2.....radius of ending layer
C.. LENCSF ...chain length
C.. OCTANT ...octant number of ANG 
C.. ANG ......angle of chain in beginning layer
C.. ANGLO1 ...local angle within an octant
C.. ANGLOn ...higher powers of ANGLO1
C.. CANG .....cosine of angle
C.. SANG .....sin    of angle
C.. CANGT ....temp cosine of angle
C.. SANGT ....temp    sin of angle
C.. TT .......transformation from an octant to global cordinates
C.. TAN1 .....properly calibrated tangent of BEGINNING track angle
C.. TAN2 .....properly calibrated tangent of ENDING track angle
C.. ARGT .....temporary argument for determining SQRT
C.. ARG1 .....argument for determining SQRT
C.. ARGn .....higher powers of ARG1
C.. SQRM .....sqrt multiplier
C.. COS1 .....COS of beginning track angle
C.. SIN1 .....SIN of beginning track angle
C.. COS2 .....COS of ending track angle
C.. SIN2 .....SIN of ending track angle
C.. IADR .....SF address
C.. LYR ......SF layer number
C.. NHITPT ...number of hits in the PT device
C.. NHITVD ...number of hits in the VD device
C.. NHITDR ...number of hits in the DR device
C.. NHT1DR ...number of single hits in the DR device
C.. IY .......return from DSF_RFV_INPUT
C.. KILLA ....stop asking flag

      INTEGER PICK,L1,L2,W1,W2,CDLAY1,CDLAY2
      REAL RADU1,RADU2
      INTEGER LENCSF
      INTEGER OCTANT
      REAL ANG,ANGLO1,ANGLO2,ANGLO3,ANGLO4
      REAL CANG,SANG,CANGT,SANGT,TT(4,0:7)
      REAL TAN1,TAN2,ARGT,ARG1,ARG2,ARG3,SQRM
      REAL COS1,SIN1,COS2,SIN2
      INTEGER IADR,LYR,NHITPT,NHITVD,NHITDR,NHT1DR
      INTEGER IY,KILLA

C.. GBU ......1=Good:beam-beam event, 0=Bad:not enough info, -1=Ugly:beam wall

      INTEGER GBU

C.. J ........occasional use index

      INTEGER J,JMAX


C these now in sfbwal_access
C.. NTSFNX ...sequential track number in order from SFNEXT
C.. NTSFNO....other sequential track number, for vertex
C.. MSFNSF ...map from SFnext sequntial trak to SF chain number
C.. CUDRSF ...curvature
C.. DADRSF ...impact parameter
C.. DADRCO ...correction to the inpact parameter from curvature
C.. F0DRSF ...PHI 0
C.. F0DRCO ...correction to the PHI 0  from curvature
C.. Z0DRSF ...Z impact parameter
C.. CTDRSF ...tan of z angle
C.. ANGSTR ...starting angle; of chain in beginning layer
C.. XSTART ...x starting postion of chain, global cordinates, for SFNEXT track
C.. YSTART ...y starting postion of chain, global cordinates, for SFNEXT track
C.. DXDSGL ...dx/(unit length) in global cordinates for each SFNEXT track
C.. DYDSGL ...dy/(unit length) in global cordinates for each SFNEXT track


C.. NTRIAL ...number of trials to classify "good" events

C.. MISDCT ...miss distance cut to define "good" chains
C.. LENGCT ...total length cut to define "good" chains
C.. LNVDCT ...length, or number of hits, in the VD to define "good" chains
C.. CURVCT ...curvature cut to define "good" chains
C.. NTCHCT ...number, total, of "good" chains to pass the trial
C.. NTMXCT ...number, maximum "good" chains allowed to pass trial

C.. NHITCT ...number of hits in "good" chains to pass a trial
C.. NHMXCT ...number hits (max) in the longest "good" chain to pass a trial
C.. NVDHCT ...number of VD hits in "good" chains to pass a trial
C.. NVDMCT ...number VD hits (max) in the longest "good" chain to pass trial
C..        note: trial is not passed with ZERO chains 
C..              or if NTCHCT,NHITCT,NHMXCT,NVDHCT&NVDMCT are all =0

C..        note: "BWALL" tests are only done AFTER passing the "good" tests
C.. STRNKL ...strength of kill: 0=no kill, 1=can kill trial, 2=can kill event
C.. MISDKL ...miss distance cut to define "bad" chains that can kill a trial 
C.. RITANG ...definition of cos(right angle);test if vertex can be calculated
C.. VRTXKL ...vertex radius for defining a bad vertex
C.. BAK2BK ...definition of cos(back to back);test of cosmic
C             also require opposite sign curvature
C.. COSMKL ...average miss distance of cosmic candidate to define bad track

C.. LENGKL ...length cut to define "BWALL" chains that can kill a trial
C.. CURVKL ...curvature cut to define "BWAL" chains that can kill a trial
C.. NTCHKL ...number, total, of "BWALL" chains to kill a trial  
C.. NDCHKL ...number, differential ["good"-"BWALL"], to not kill a trial
C          ...applies only if number of "BWALL" >0 

C.. NGOOD ....number of good tracks found in each trial
C.. ISGOOD ...flag that a good track is still good is each trial 
C.. NHITG ....number of hits in good chains
C.. NHMAX ....number of hits (max) in the longest good chain
C.. NHTGVD ...number of VD hits in good chains
C.. NHMXVD ...number of VD hits (max) in chain with most VD hits
C.. NBAD .....number of good tracks later marked bad in each trial

C.. MAPTRK ...map from good track within a trial to sequential SFNEXT track 

C.. GBUTRI ...good/bad/ugly for the trial
C..        ...1=Good:beam-beam trial, 0=Bad:not enough info,
C..        ...-1=Ugly:beam wall
C.. NGOODT ...number of good trials
C.. NBADT ....number of bad trials
C.. NUGLYT ...number of ugly trials


      INTEGER MTRIAL
      PARAMETER (MTRIAL=5)

      INTEGER       NTRIAL,ITRIAL

      REAL          MISDCT(MTRIAL)
      INTEGER       LENGCT(MTRIAL)
      INTEGER       LNVDCT(MTRIAL)
      REAL          CURVCT(MTRIAL)
      INTEGER       NTCHCT(MTRIAL)
      INTEGER       NTMXCT(MTRIAL)

      INTEGER       NHITCT(MTRIAL)
      INTEGER       NHMXCT(MTRIAL)
      INTEGER       NVDHCT(MTRIAL)
      INTEGER       NVDMCT(MTRIAL)

      INTEGER       STRNKL(MTRIAL)
      REAL          MISDKL(MTRIAL)
      REAL          RITANG(MTRIAL)
      REAL          VRTXKL(MTRIAL)
      REAL          BAK2BK(MTRIAL)
      REAL          COSMKL(MTRIAL)
      INTEGER       LENGKL(MTRIAL)
      REAL          CURVKL(MTRIAL)
      INTEGER       NTCHKL(MTRIAL)
      INTEGER       NDCHKL(MTRIAL)


      INTEGER       NGOOD (MTRIAL),NDIFF
      INTEGER       ISGOOD(MTSFNX,MTRIAL)
      INTEGER       NHITG (MTRIAL)
      INTEGER       NHMAX (MTRIAL)
      INTEGER       NHTGVD(MTRIAL)
      INTEGER       NHMXVD(MTRIAL)
      INTEGER       NBAD  (MTRIAL)

      INTEGER       MAPTRK(MTSFNX,MTRIAL)

      INTEGER       GBUTRI(MTRIAL)
      INTEGER       NGOODT,NBADT,NUGLYT

      
C.. NVRTEX ...number of vertex found
C.. IVRTEX ...vertex index
C.. TYPCUR ...current value of the vertex type
C.. TYPVTX ...type vertex, 1=vertex of two tracks at right angles
C                          2=distance of closest approach of back2back pair
C.. MAPVTX ...map from two seq SFNEXT track numbers to vertex number n1<n2
C.. MAPSFN ...map from vertex number to seqential SFNEXT track number

C.. DOTPRD ...dot product of two tracks considered for vertex
C.. DOTPRA ...ABS(dot product of two tracks considered for vertex)
C.. XSTR1 ....XSTART of the 1st track in the vertex finding
C.. XSTR2 ....XSTART of the 2nd track in the vertex finding
C.. YSTR1 ....YSTART of the 1st track in the vertex finding
C.. YSTR2 ....YSTART of the 2nd track in the vertex finding
C.. DXDS1 ....DXDSGL of the 1st track in the vertex finding
C.. DXDS2 ....DXDSGL of the 2nd track in the vertex finding
C.. DYDS1 ....DYDSGL of the 1st track in the vertex finding
C.. DYDS2 ....DYDSGL of the 2nd track in the vertex finding
C.. UU .......unit lengths from the start on the logical first track
C.. VV .......unit lengths from the start on the logical second track
C.. DET ......determinant for vertex finding
C.. COLM1 ....1st element of column vertor for vertex finding
C.. COLM2 ....2nd element of column vertor for vertex finding
C.. VRTXR2 ...radius squared of vertex
C.. VRTXR ....temporary radius
C.. APROC ....dist of closest approach of cosmic
C.. APROCA ...ABS(dist of closest approach of cosmic)
C.. DIFAPR ...actually, the sum of the DADRs of the two tracks
C.. VERTEX ...X location of a vertex
C.. VERTEY ...Y location of a vertex
C.. VRTXTR ...best vertex, RADIUS SQUARED, if any,
C..        ...for each initial good track in each trial
C.. CLOSTR ...best close approach, if any,
C..           for each initial good track in each trial

      INTEGER MVRTEX
      PARAMETER (MVRTEX=100)

      INTEGER       NVRTEX,IVRTEX,TYPCUR
      INTEGER       TYPVTX(MVRTEX)
      INTEGER       MAPVTX(MTSFNX,MTSFNX)
      INTEGER       MAPSFN(2,MVRTEX)
      REAL          DOTPRD,DOTPRA
      REAL          XSTR1,XSTR2,YSTR1,YSTR2,DXDS1,DXDS2,DYDS1,DYDS2
      REAL          UU,VV,DET,COLM1,COLM2,VRTXR2,VRTXR
      REAL          APROC,APROCA,DIFAPR
      REAL          VERTEX(MVRTEX)
      REAL          VERTEY(MVRTEX)
      REAL          VRTXTR(MTSFNX,MTRIAL)
      REAL          CLOSTR(MTSFNX,MTRIAL)

      DATA TT/
     +  1.     , 0.     , 0.     , 1.     ,
     1  0.70711,-0.70711, 0.70711, 0.70711,
     2  0.     ,-1.     , 1.     , 0.     ,
     3 -0.70711,-0.70711, 0.70711,-0.70711,
     4 -1.     , 0.     , 0.     ,-1.     ,
     5 -0.70711, 0.70711,-0.70711,-0.70711,
     6  0.     , 1.     ,-1.     , 0.     ,
     7  0.70711, 0.70711,-0.70711, 0.70711/

C NOV-1999 CLEOIII: NTRIAL =0 TO SKIP EVENT EVALUATION 
      DATA NTRIAL/0/

C     DATA NTRIAL/4/

C  1   2+ TRACK WITH BEAM WALL KILLER
C  2   =2 TRACK WITH COSMIC KILLER
C  3   3+ TRACK LOOSER
C  4   4+ TRACK LOOSER



      DATA (MISDCT(j),j=1,4)/ .025,  .025,  .050,  .050 /
      DATA (LENGCT(j),j=1,4)/   20,    19,    20,    20 / 
      DATA (LNVDCT(j),j=1,4)/    2,     2,     2,     0 / 
      DATA (CURVCT(j),j=1,4)/ 1.25,   .90,   .90,   .90 /
      DATA (NTCHCT(j),j=1,4)/    2,     2,     3,     4 /
      DATA (NTMXCT(j),j=1,4)/   99,     2,    99,    99 /
      DATA (NHITCT(j),j=1,4)/    0,    59,     0,     0 /
      DATA (NHMXCT(j),j=1,4)/   25,     0,    25,     0 / 
      DATA (NVDHCT(j),j=1,4)/    0,     0,    12,     0 /
      DATA (NVDMCT(j),j=1,4)/    4,     4,     0,     0 /

      DATA (STRNKL(j),j=1,4)/    2,     2,     1,     2 /
      DATA (MISDKL(j),j=1,4)/.0125,  .025, .0160, .0180 /
      DATA (RITANG(j),j=1,4)/  .75,   -1.,   -1.,   -1. /
      DATA (VRTXKL(j),j=1,4)/.0132,    .9,    .9,    .9 /
      DATA (BAK2BK(j),j=1,4)/  -2.,  -.98,   -2.,   -2. /
      DATA (COSMKL(j),j=1,4)/   .9, .0070,    .9,    .9 /
      DATA (LENGKL(j),j=1,4)/   20,    19,    20,    20 /
      DATA (CURVKL(j),j=1,4)/ 1.25,   .90,   .90,   .90 /
      DATA (NTCHKL(j),j=1,4)/  999,   999,   999,     4 /
      DATA (NDCHKL(j),j=1,4)/    1,     1,     2,     0 /



C********************************************************************
C executable code starts here
C********************************************************************

      GBU=0
      NVRTEX=0
      NGOODT=0
      NBADT=0
      NUGLYT=0

      DO 15 ITRIAL=1,NTRIAL
            NGOOD (ITRIAL)=0
            NHITG (ITRIAL)=0
            NHMAX (ITRIAL)=0
            NHTGVD(ITRIAL)=0
            NHMXVD(ITRIAL)=0
            NBAD  (ITRIAL)=0
15          GBUTRI(ITRIAL)=0

      CALL SFIND

      NTSFNX=0
20    CALL SFNEXT
      IF(NCHANM.EQ.1)THEN
            NTSFNX=NTSFNX+1

                IF(NTSFNX.GE.2)THEN
                    JMAX=NTSFNX-1
                    DO 21 NTSFNO=1,JMAX
21                  MAPVTX(NTSFNO,NTSFNX)=0
                ENDIF

                IF(NTRIAL.GT.0)THEN
                   DO 22 ITRIAL=1,NTRIAL
                      VRTXTR(NTSFNX,ITRIAL)=9.
                      CLOSTR(NTSFNX,ITRIAL)=9.
 22                CONTINUE
                ENDIF

            PICK=JCHAIN(1)
            MSFNSF(NTSFNX)=PICK

            LENCSF=LENCHN(PICK)
            L1=SFLSFA(LINCHN(PICK))
            L2=SFLSFA(MINCHN(PICK))
            W1=SFWSFA(LINCHN(PICK))
            CDLAY1=CDLASF(L1)
            CDLAY2=CDLASF(L2)

                IF(CDLAY1.LT.0)THEN
                   ANG=SFSVF0(-CDLAY1)+TWOPI/SFSVCL(-CDLAY1)*W1
                   RADU1=SFSVRD(-CDLAY1)
                ELSE
                   ANG=PHIFCD(CDLAY1)+CELLCD(CDLAY1)*W1
                   RADU1=RCD(CDLAY1)
                ENDIF
                IF(CDLAY2.LT.0)THEN
                   RADU2=SFSVRD(-CDLAY2)
                ELSE
                   RADU2=RCD(CDLAY2)
                ENDIF

                F0DRSF(NTSFNX)=ANG
            
26            IF(ANG.GE.-EIGPI)GO TO 27
                  ANG=ANG+TWOPI
                  GO TO 26            
27             IF(ANG.LT. FT8PI)GO TO 28
                  ANG=ANG-TWOPI
                  GO TO 27            
28             CONTINUE

            OCTANT=(ANG+EIGPI)/QARPI
               IF(OCTANT.LT.0)OCTANT=0
               IF(OCTANT.GT.7)OCTANT=7
            ANGLO1=ANG-QARPI*OCTANT
            ANGLO2=ANGLO1*ANGLO1
            ANGLO3=ANGLO2*ANGLO1
            ANGLO4=ANGLO3*ANGLO1
            CANGT= 1.-ANGLO2/2. +ANGLO4/24.
            SANGT=    ANGLO1    -ANGLO3/6.
            CANG=  TT(1,OCTANT)*CANGT + TT(2,OCTANT)*SANGT
            SANG=  TT(3,OCTANT)*CANGT + TT(4,OCTANT)*SANGT
            ANGSTR(NTSFNX)=ANG+EIGPI
            XSTART(NTSFNX)=RADU1*CANG
            YSTART(NTSFNX)=RADU1*SANG

            TAN1=(1.*LVLCHN(1,PICK)/MULST)*(TWOPI/CELSSF)*100.
               ARGT=TAN1**2
                  IF    (ARGT.LT.0.5)THEN
                     SQRM=1.
                     ARG1=ARGT
                  ELSEIF(ARGT.LT.2.0)THEN
                     SQRM=ROOT2
                     ARG1=(ARGT-1.)/2.
                  ELSE
                     SQRM=2.
                     ARG1=(ARGT-3.)/4.
                  ENDIF
               ARG2=ARG1*ARG1
               ARG3=ARG2*ARG1
            COS1=1./(SQRM*(1.+ARG1/2.-ARG2/8.+ARG3/16.))
            SIN1=COS1*TAN1
            TAN2=(1.*LVLCHN(2,PICK)/MULST)*(TWOPI/CELSSF)*100.
               ARGT=TAN2**2
                  IF    (ARGT.LT.0.5)THEN
                     SQRM=1.
                     ARG1=ARGT
                  ELSEIF(ARGT.LT.2.0)THEN
                     SQRM=ROOT2
                     ARG1=(ARGT-1.)/2.
                  ELSE
                     SQRM=2.
                     ARG1=(ARGT-3.)/4.
                  ENDIF
               ARG2=ARG1*ARG1
               ARG3=ARG2*ARG1
            COS2=1./(SQRM*(1.+ARG1/2.-ARG2/8.+ARG3/16.))
            SIN2=COS2*TAN2

            DXDSGL(NTSFNX)=COS1*CANG-SIN1*SANG
            DYDSGL(NTSFNX)=COS1*SANG+SIN1*CANG

            DADRSF(NTSFNX)=-TAN1 *RADU1
C DADRSF is linearly corrected to the midpoint between origin and start
            DADRCO=-TAN2 *RADU1*(-.5*RADU1)/(RADU2-RADU1)
            DADRSF(NTSFNX)=DADRSF(NTSFNX)+DADRCO
            CUDRSF(NTSFNX)= SIN2 /RADU2
C phi = angle of first hit for now            
C           F0DRSF(NTSFNX) is filled above
            F0DRCO=-.5*RADU1*(2.*CUDRSF(NTSFNX))
            F0DRSF(NTSFNX)=F0DRSF(NTSFNX)+F0DRCO
#if defined(CLEO_SFZFIT)
            Z0DRSF(NTSFNX)=ZR0CHN(PICK)/(1.*MULST)
            CTDRSF(NTSFNX)=ZDRCHN(PICK)/(1.*MULST)
#else
            Z0DRSF(NTSFNX)=0.
            CTDRSF(NTSFNX)=0.
#endif


C calculate the number of hits in the sub-elements
C this code is coppied from SFNEXT, but not run there for beam wall

            NHITPT=0
            NHITVD=0
            NHITDR=0
            NHT1DR=0
            IADR=LINCHN(PICK)
63          IF(IADR.EQ.0)GO TO 67
                IF(INUCHN(IADR).GT.0)THEN
                    LYR=SFLSFA(IADR)
                    IF(LYR.LE.MLSFPT)THEN
                        NHITPT=NHITPT+1
                    ELSEIF(LYR.LE.MLSFVD)THEN
                        NHITVD=NHITVD+1
                    ELSEIF(LYR.LE.MLSFDR)THEN
                        NHITDR=NHITDR+1
                        IF(DOUBPT(IADR).EQ.0)NHT1DR=NHT1DR+1
                    ENDIF
                ENDIF
                IF(IADR.EQ.MINCHN(PICK))GO TO 67
                      IADR=ISTFOR(IADR)
                      GO TO 63
67          NHITVD=NHITVD+NHITPT

#if defined(CLEO_SFDIAG)
      PRINT 4401,LENCSF,NHITVD,
     2           LVLCHN(1,PICK),LVLCHN(2,PICK),
     3           XSTART(NTSFNX),YSTART(NTSFNX),
     4           TAN1,SIN1,COS1,TAN2,SIN2,COS2,
     6           DXDSGL(NTSFNX),DYDSGL(NTSFNX),
     7           CUDRSF(NTSFNX),DADRSF(NTSFNX),DADRCO,
     8           ANGSTR(NTSFNX),F0DRSF(NTSFNX),F0DRCO,
     9           Z0DRSF(NTSFNX),CTDRSF(NTSFNX)
4401  FORMAT(' SFBWAL: LENCSF=',I5,'      LHITVD=',I5,
     2          '      LVLSF1=',I5,'      LVLSF2=',I5/
     3       '         XSTART=',F9.4,'  YSTART=',F9.4/
     4       '           TAN1=',F9.5,'    SIN1=',F9.5,'    COS1=',F9.5/
     5       '           TAN2=',F9.5,'    SIN2=',F9.5,'    COS2=',F9.5/
     6       '          dx/ds=',F9.5,'   dy/ds=',F9.5/
     7       '           CUDR=',F9.4,'    DADR=',F9.4,'    CORR=',F9.4/
     8       '         angstr=',F9.4,'   PHIDF=',F9.4,'    CORR=',F9.4/
     9       '           Z0DR=',F9.4,'    CTDR=',F9.4)
      CALL DSF_RFV_INPUT(IY)
#endif

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234

C*****< process chain number= PICK = JCHAIN(1) >*************************


C loop over the trials
            IF(NTRIAL.GT.0)THEN
               DO 290 ITRIAL=1,NTRIAL
C no further tests required if trial is already ugly
                  IF(GBUTRI(ITRIAL).LT.0)GO TO 290

C test the trial pass variables,
                  IF(LENCSF             .LT.LENGCT(ITRIAL))GO TO 290
                  IF(NHITVD             .LT.LNVDCT(ITRIAL))GO TO 290
                  IF(ABS(DADRSF(NTSFNX)).GT.MISDCT(ITRIAL))GO TO 290
                  IF(ABS(CUDRSF(NTSFNX)).GT.CURVCT(ITRIAL))GO TO 290

C track is basically good
                  NGOOD (ITRIAL)=NGOOD (ITRIAL)+1
                  NHITG (ITRIAL)=NHITG (ITRIAL)+LENCSF
                  NHTGVD(ITRIAL)=NHTGVD(ITRIAL)+NHITVD
                  IF(LENCSF.GT.NHMAX (ITRIAL))NHMAX (ITRIAL)=LENCSF
                  IF(NHITVD.GT.NHMXVD(ITRIAL))NHMXVD(ITRIAL)=NHITVD

C test if there is room to store this track as a good track for vertex
                  IF(NGOOD(ITRIAL).LE.MTSFNX)THEN
                     ISGOOD(NGOOD(ITRIAL),ITRIAL)=2
                     MAPTRK(NGOOD(ITRIAL),ITRIAL)=NTSFNX
                     
C calculate the vertex
C test if a vertex is required in this test
                     IF((RITANG(ITRIAL).GT. 0.).OR.
     2                  (BAK2BK(ITRIAL).GT.-1.))THEN
C test if there are previous tracks
                        IF(NGOOD(ITRIAL).GE.2)THEN
                           JMAX=NGOOD(ITRIAL)-1
                           DO 223 J=1,JMAX
                              NTSFNO=MAPTRK(J,ITRIAL)
                              IVRTEX=MAPVTX(NTSFNO,NTSFNX)

C test if vertex needs to be calculated
                              IF(IVRTEX.EQ.0)THEN
                                 DXDS1=DXDSGL(NTSFNO)
                                 DYDS1=DYDSGL(NTSFNO)
                                 DXDS2=DXDSGL(NTSFNX)
                                 DYDS2=DYDSGL(NTSFNX)
                                 DOTPRD=DXDS1*DXDS2+DYDS1*DYDS2
                                 DOTPRA=DOTPRD
                                 IF(DOTPRA.LT.0.)DOTPRA=-DOTPRA

C test if tracks is within phi range to calculate the vertex 
                                 IF(DOTPRA.LE.RITANG(ITRIAL))THEN
C test if there is room for a new vertex
                                    IF(NVRTEX.LT.MVRTEX)THEN
                                       NVRTEX=NVRTEX+1
                                       IVRTEX=NVRTEX
                                       TYPCUR=1
                                       XSTR1=XSTART(NTSFNO)
                                       YSTR1=YSTART(NTSFNO)
                                       XSTR2=XSTART(NTSFNX)
                                       YSTR2=YSTART(NTSFNX)
                                       DET=-DXDS1*DYDS2+DYDS1*DXDS2
                                       COLM1=XSTR2-XSTR1
                                       COLM2=YSTR2-YSTR1
                                       UU=(-DYDS2*COLM1+DXDS2*COLM2)/DET
                                       VERTEX(IVRTEX)=XSTR1+UU*DXDS1
                                       VERTEY(IVRTEX)=YSTR1+UU*DYDS1
                                       VRTXR2=VERTEX(IVRTEX)**2
     2                                       +VERTEY(IVRTEX)**2
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234
#if defined(CLEO_SFDIAG)
                                       VRTXR=SQRT(VRTXR2)
                                       PRINT 4402,NTSFNO,NTSFNX,
     2                                 VERTEX(IVRTEX),VERTEY(IVRTEX),
     3                                 VRTXR
 4402  FORMAT(' SFBWAL: have calculated new vertex',
     2       ' of SFNEXT sequential tracks',I3,' and',I3,' at'/
     3       '                    X=',F9.4,'  Y=',F9.4,'  R=',F9.4)
#endif
C end of test if there is room for a new vertex
                                    ENDIF

C failed test if tracks is within phi range to calculate the vertex 
C test if tracks is within phi range to calculate closest approach 
C                     ELSEIF((CUDRSF(NTSFNO)*CUDRSF(NTSFNX).LT.0.)
C     2                .AND.(DOTPRD.LT.BAK2BK(ITRIAL)))THEN
                                 ELSEIF((DOTPRD.LT.BAK2BK(ITRIAL)))THEN
C test if there is room for a new vertex
                                    IF(NVRTEX.LT.MVRTEX)THEN
                                       NVRTEX=NVRTEX+1
                                       IVRTEX=NVRTEX
                                       TYPCUR=2
                                       APROC= (DADRSF(NTSFNO)
     2                                        -DADRSF(NTSFNX))/2.
                                       DIFAPR=(DADRSF(NTSFNO)
     2                                       +DADRSF(NTSFNX))
                                       APROCA=APROC
                                       IF(APROC.LT.0.)THEN
                                          APROCA=-APROCA
                                          DIFAPR=-DIFAPR
                                       ENDIF
                                       VERTEX(IVRTEX)=APROCA
                                       VERTEY(IVRTEX)=DIFAPR
#if defined(CLEO_SFDIAG)
      PRINT 4408,NTSFNO,NTSFNX,VERTEX(IVRTEX),VERTEY(IVRTEX)
4408  FORMAT(' SFBWAL: calculated cosmic approach',
     2       ' of SFNEXT sequential tracks',I3,' and',I3,' at'/
     3       '             approach=',F9.5,'  diff=',F9.5)
#endif
C end of test if there is room for a new vertex
                                    ENDIF

C failed test if tracks is within phi range to calculate closest approach 
                                 ELSE
c this would block calculation later                        IVRTEX=-1
                                    TYPCUR=0
#if defined(CLEO_SFDIAG)
                                    PRINT 4404,NTSFNO,NTSFNX,DOTPRD
4404  FORMAT(' SFBWAL:   can not calculate vertex'/
     2       '                or closest approach',
     3       ' of SFNEXT sequential tracks',I3,' and',I3,' ;',
     4       ' DOTPRD=',F9.4)
#endif

C end of test if tracks is within phi range to calculate the vertex 
                                 ENDIF
                                 MAPVTX(NTSFNO,NTSFNX)=IVRTEX
                                 IF(IVRTEX.GT.0)THEN
                                    TYPVTX(IVRTEX)=TYPCUR
                                    MAPSFN(1,IVRTEX)=NTSFNO
                                    MAPSFN(2,IVRTEX)=NTSFNX
                                 ENDIF

C test if vertex does not need to be calculated because it already is
                              ELSEIF(IVRTEX.GT.0)THEN
C test if this is a vertex of two right angle tracks
                                 IF(TYPVTX(IVRTEX).EQ.1)THEN
                                    VRTXR2=VERTEX(IVRTEX)**2+
     2                              VERTEY(IVRTEX)**2
#if defined(CLEO_SFDIAG)
                                    VRTXR=SQRT(VRTXR2)
                                    PRINT 4403,NTSFNO,NTSFNX,
     2                              VERTEX(IVRTEX),VERTEY(IVRTEX),
     3                              VRTXR
4403  FORMAT(' SFBWAL: previous calculated vertex',
     2       ' of SFNEXT sequential tracks',I3,' and',I3,' at'/
     3       '                    X=',F9.4,'  Y=',F9.4,'  R=',F9.4)
#endif
C test is this is an approch of two back to back tracks
                                 ELSEIF(TYPVTX(IVRTEX).EQ.2)THEN
                                    APROCA=VERTEX(IVRTEX)
                                    DIFAPR=VERTEY(IVRTEX)
#if defined(CLEO_SFDIAG)
      PRINT 4409,NTSFNO,NTSFNX,VERTEX(IVRTEX),VERTEY(IVRTEX)
4409  FORMAT(' SFBWAL: prevus calculated approach',
     2       ' of SFNEXT sequential tracks',I3,' and',I3,' at'/
     3       '             approach=',F9.5,'  diff=',F9.5)
#endif
                                 ENDIF
C previously found that vertex can not be calculated from these tracks
                              ELSE
C (IVRTEX.LT.0)
      PRINT 4405,NTSFNO,NTSFNX
4405  FORMAT(' SFBWAL:    old uncalculable vertex',
     2       ' of SFNEXT sequential tracks',I3,' and',I3)
C end of test if vertex needs to be calculated
                              ENDIF

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234
C test if the new vertex is a new low for either of the tracks
                              IF(IVRTEX.GT.0)THEN
C test of the kind of vertex
                                 IF(TYPVTX(IVRTEX).EQ.1)THEN
#if defined(CLEO_SFDIAG)
                                    PRINT 4406,VRTXR2,J,ITRIAL,
     2                              VRTXTR(J,ITRIAL)
4406  FORMAT(' SFBWAL: VRTXR2=',F9.7,' <?VRTXTR(',I4,',',I4,')=',F9.7)
#endif
                                    IF(VRTXR2.LT.
     2                                 VRTXTR(J,ITRIAL)            )THEN
                                       VRTXTR(J,            ITRIAL)=
     2                                 VRTXR2
#if defined(CLEO_SFDIAG)
      PRINT 4407,J,ITRIAL
4407  FORMAT(' SFBWAL: REPLACED VRTXTR(',I4,',',I4,')')
#endif
                                    ENDIF
#if defined(CLEO_SFDIAG)
      PRINT 4406,VRTXR2,NGOOD(ITRIAL),ITRIAL,
     2           VRTXTR(NGOOD(ITRIAL),ITRIAL)
#endif
                                    IF(VRTXR2.LT.
     2                                 VRTXTR(NGOOD(ITRIAL),ITRIAL))THEN
                                       VRTXTR(NGOOD(ITRIAL),ITRIAL)=
     2                                 VRTXR2
#if defined(CLEO_SFDIAG)
      PRINT 4407,NGOOD(ITRIAL),ITRIAL
#endif
                                    ENDIF
C test of the kind of vertex
                                 ELSEIF(TYPVTX(IVRTEX).EQ.2)THEN
#if defined(CLEO_SFDIAG)
      PRINT 4410,APROCA,J,ITRIAL,
     2           CLOSTR(J,ITRIAL)
4410  FORMAT(' SFBWAL: aproch=',F9.7,' <?CLOSTR(',I4,',',I4,')=',F9.7)
#endif
                                    IF(APROCA.LT.
     2                                 CLOSTR(J,ITRIAL)            )THEN
                                       CLOSTR(J,            ITRIAL)=
     2                                 APROCA
#if defined(CLEO_SFDIAG)
      PRINT 4411,J,ITRIAL
4411  FORMAT(' SFBWAL: REPLACED CLOSTR(',I4,',',I4,')')
#endif
                                    ENDIF
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234
#if defined(CLEO_SFDIAG)
                                    PRINT 4410,APROCA,NGOOD(ITRIAL),
     2                              ITRIAL,CLOSTR(NGOOD(ITRIAL),ITRIAL)
#endif
                                    IF(APROCA.LT.
     2                                 CLOSTR(NGOOD(ITRIAL),ITRIAL))THEN
                                       CLOSTR(NGOOD(ITRIAL),ITRIAL)=
     2                                 APROCA
#if defined(CLEO_SFDIAG)
                                       PRINT 4411,NGOOD(ITRIAL),ITRIAL
#endif
                                    ENDIF 
C end of test of the kind of vertex
                                 ENDIF
C end of test if the new vertex is a new low for either of the tracks
                              ENDIF

C end of loop over previous tracks
 223                       CONTINUE
C end of test if there are previous tracks
                        ENDIF
C end of test if a vertex is required in this test
                     ENDIF
C end of test if there is room to store this track as a good track for vertex
                  ENDIF

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234
C test the trial kill variables
                  IF(STRNKL(ITRIAL)     .EQ.0                 )GO TO 290
                  IF(LENCSF             .LT.LENGKL(ITRIAL)    )GO TO 290
                  IF(ABS(CUDRSF(NTSFNX)).GT.CURVKL(ITRIAL)    )GO TO 290
                  IF(ABS(DADRSF(NTSFNX)).LT.MISDKL(ITRIAL)    )THEN
                     ISGOOD(NGOOD(ITRIAL),ITRIAL)=1
                  ELSE
                     NBAD  (ITRIAL)=NBAD  (ITRIAL)+1
                     ISGOOD(NGOOD(ITRIAL),ITRIAL)=0
                  ENDIF
C end of loop over trials
 290           CONTINUE
C break point if there are no trials
            ENDIF


C*****< end of process of chain >************************************
            GO TO 20
         ENDIF

C new loop over the trials and the tracks within the trials to count the
C number of bad vertices
C loop over the trials
         IF(NTRIAL.GT.0)THEN
            DO 390 ITRIAL=1,NTRIAL
C this count is not required if there are no good tracks
               IF(NGOOD(ITRIAL).LE.0                  )GO TO 390
C this count not required if the kill has no strength
               IF(STRNKL(ITRIAL).LE.0                 )GO TO 390   

C this count is not required if there is no vertex cut in this trial
               IF(RITANG(ITRIAL).GT.0.                )THEN
C loop over the tracks
                  DO 323 J=1,NGOOD(ITRIAL)
C do not test tracks that are already bad from DADR
C  these are already counted in the NBAD count
                     IF(ISGOOD(J,ITRIAL).NE.1         )GO TO 323
C do not test tracks without a found vertex
                     IF(VRTXTR(J,ITRIAL).GT.8.        )GO TO 323
C apply the vertex test
                     IF(VRTXTR(J,ITRIAL).LT.
     2                  VRTXKL(ITRIAL)*VRTXKL(ITRIAL) )GO TO 323
                     NBAD(ITRIAL)=NBAD(ITRIAL)+1
                     ISGOOD(J,ITRIAL)=0
C end of loop over tracks
 323              CONTINUE
               ENDIF

C this count is not required if there is no vertex cut in this trial
               IF(BAK2BK(ITRIAL).GT.-1.               )THEN
C loop over the tracks
                  DO 325 J=1,NGOOD(ITRIAL)
C do not test tracks that are already bad from DADR or vertex
C  these are already counted in the NBAD count
                     IF(ISGOOD(J,ITRIAL).NE.1         )GO TO 325
C do not test tracks without a found approach
                     IF(CLOSTR(J,ITRIAL).GT.8.        )GO TO 325
C apply the vertex test
                     IF(CLOSTR(J,ITRIAL).LT.
     2                  COSMKL(ITRIAL) )GO TO 325
                     NBAD(ITRIAL)=NBAD(ITRIAL)+1
                     ISGOOD(J,ITRIAL)=0
C end of loop over tracks
 325              CONTINUE
               ENDIF

C end of loop over trials
 390        CONTINUE
         ENDIF


C complete testing of the trials

C event is good if there is at least one good trial
C       and maybe even some ugly trials
C       but no ugly trial with strength=2
C event is ugly if there are any strength=2 ugly trials,
C event is ugly if there are no good trials but some ugly trials
C event is bad is there are neither good nor ugly trials


C loop over the trials
         IF(NTRIAL.GT.0)THEN
            DO 690 ITRIAL=1,NTRIAL
C no further tests on this trial required if this trial has kill strength=1
C and if GBU>0 (a passed trial has been found)
               IF((GBU.GT.0).AND.(STRNKL(ITRIAL).LT.2))GO TO 690   

C test the trial pass variables,

#if defined(CLEO_SFDIAG)
      PRINT 5503,ITRIAL,NGOOD (ITRIAL),LENGCT(ITRIAL),LNVDCT(ITRIAL),
     2                  CURVKL(ITRIAL),MISDCT(ITRIAL)
5503  FORMAT(' SFBWAL: trial:',I2,', has',I3,'  [good] chains with',
     2       ' length>/=',I3,' VDhits>/=',I3/
     3   27X,' curv<',F7.4,' and misdis<',F6.3)
#endif


C no further tests on this trial required goodness cuts not defined
               IF((NTCHCT(ITRIAL).EQ.0).AND.
     2            (NHITCT(ITRIAL).EQ.0).AND.
     3            (NHMXCT(ITRIAL).EQ.0).AND.
     4            (NVDHCT(ITRIAL).EQ.0).AND.
     5            (NVDMCT(ITRIAL).EQ.0)             )GO TO 690
C trial is bad if there are no good tracks
               IF(NGOOD (ITRIAL).LE.0               )GO TO 660 

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 1234
#if defined(CLEO_SFDIAG)
      PRINT 5504,ITRIAL,NGOOD (ITRIAL),NTCHCT(ITRIAL),NTMXCT(ITRIAL),
     2                  NHITG (ITRIAL),NHITCT(ITRIAL),
     3                  NHMAX (ITRIAL),NHMXCT(ITRIAL),
     2                  NHTGVD(ITRIAL),NVDHCT(ITRIAL),
     4                  NHMXVD(ITRIAL),NVDMCT(ITRIAL)
5504  FORMAT(' SFBWAL: trial:',I2,
     2       ' requires tracks,',I2,'/',I2,'(</=',I2,');',
     3       ' hits on tracks,',I4,'/',I3,'; longest track,',I2,'/',I2/
     3   48X,' VDhits ontrack,',I4,'/',I3,'; most VD hits ,',I2,'/',I2)
#endif


               IF(NGOOD (ITRIAL).LT.NTCHCT(ITRIAL)  )GO TO 660 
               IF(NGOOD (ITRIAL).GT.NTMXCT(ITRIAL)  )GO TO 660 
               IF(NHITG (ITRIAL).LT.NHITCT(ITRIAL)  )GO TO 660 
               IF(NHMAX (ITRIAL).LT.NHMXCT(ITRIAL)  )GO TO 660 
               IF(NHTGVD(ITRIAL).LT.NVDHCT(ITRIAL)  )GO TO 660 
               IF(NHMXVD(ITRIAL).LT.NVDMCT(ITRIAL)  )GO TO 660 


#if defined(CLEO_SFDIAG)
      PRINT 5505,ITRIAL
5505  FORMAT(' SFBWAL: trial:',I2,' [good] parts satisfied')
#endif


               GBUTRI(ITRIAL)=1

 630           IF(STRNKL(ITRIAL).EQ.0               )GO TO 670


#if defined(CLEO_SFDIAG)
      PRINT 5506,ITRIAL,NBAD  (ITRIAL),LENGKL(ITRIAL),CURVKL(ITRIAL),
     2                                 MISDKL(ITRIAL),VRTXKL(ITRIAL),
     3                                 COSMKL(ITRIAL)
5506  FORMAT(' SFBWAL: trial:',I2,', has',I3,' [BWALL] chains with',
     2       ' length>/=',I3,' curv<',F6.3/
     3   27X,' and misdis>/=',F7.4,
     4       ' or vertex>/=',F7.4,' or approach>/=',F7.4)
#endif


               IF(NBAD  (ITRIAL).LE.0               )GO TO 670


#if defined(CLEO_SFDIAG)
      PRINT 5501,ITRIAL,NBAD  (ITRIAL),NTCHKL(ITRIAL)
5501  FORMAT(' SFBWAL: trial:',I2,' requires [BWALL] chains =',I2,
     2       ' <',I4)
#endif

               IF(NBAD  (ITRIAL).GE.NTCHKL(ITRIAL)  )GO TO 650

               NDIFF=NGOOD(ITRIAL)-NBAD(ITRIAL)

#if defined(CLEO_SFDIAG)
      PRINT 5507,ITRIAL,NGOOD (ITRIAL),NBAD  (ITRIAL),
     2                  NDIFF,NDCHKL(ITRIAL)
5507    FORMAT(' SFBWAL: trial:',I2,
     2         ' requires [good]-[BWALL]=(',I2,'-',I4,')=',I2,
     3         ' >/=',I4)
#endif

               IF(NDIFF         .LT.NDCHKL(ITRIAL)  )GO TO 650
               GO TO 670

C trial is ugly
650            GBUTRI(ITRIAL)=-1
               NUGLYT=NUGLYT+1
               IF    (STRNKL(ITRIAL).EQ.1)THEN
                  IF(GBU.EQ.0)GBU=-ITRIAL

#if defined(CLEO_SFDIAG)
      PRINT 5508,ITRIAL
5508  FORMAT(' SFBWAL: trial:',I2,' UGLY') 
#endif


               ELSEIF(STRNKL(ITRIAL).EQ.2)THEN
                  GBU=-ITRIAL

#if defined(CLEO_SFDIAG)
      PRINT 5509,ITRIAL
5509  FORMAT(' SFBWAL: trial:',I2,' UGLY; event is UGLY')
#endif

                   GO TO 899

               ENDIF
               GO TO 690

C trial is bad
 660           NBADT=NBADT+1
               GO TO 690
C trial is good
 670           NGOODT=NGOODT+1
               IF(GBU.LE.0)GBU=ITRIAL

C end of loop over trials
 690        CONTINUE
         ENDIF


C------------------------------------------------------------------
C the end
C------------------------------------------------------------------

 899     CONTINUE

#if defined(CLEO_SFDIAG)
      PRINT 7701,NGOODT,NBADT,NUGLYT,GBU
7701  FORMAT(' SFBWAL: NGOODT,NBADT,NUGLYT,GBU=',3I4,I20)
#endif

      RETURN
      END



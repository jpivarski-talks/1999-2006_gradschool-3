*
* $Id: xts_tfxf.F,v 1.19 2003/03/05 17:27:13 dpp Exp $
*
* $Log: xts_tfxf.F,v $
* Revision 1.19  2003/03/05 17:27:13  dpp
*      -> diagnostic format
*      -> protect against number of hits > M_XTS_HITS
*
* Revision 1.18  2002/09/05 19:10:03  dpp
*      -> some F77
*      -> change color, use closed triangles
*
* Revision 1.17  2002/05/23 14:34:27  dpp
*      -> make displayed points bigger
*
* Revision 1.16  2001/11/19 23:11:49  dpp
*     -> remove unneeded declarations
*      -> delete unused include file
*
* Revision 1.15  2000/12/19 23:54:29  dpp
*      -> add vertical line at zero
*
* Revision 1.14  2000/10/24 22:32:39  dpp
*      -> scale character size to plot size
*      -> remove print
*
* Revision 1.13  2000/10/02 20:54:40  dpp
*      -> remove axis locations of TFXF plot; put in common
*
* Revision 1.12  2000/08/28 17:40:43  dpp
*      -> format to display larger residuals
*
* Revision 1.11  2000/08/18 18:39:49  dpp
*      -> new arguments for KABG2
*
* Revision 1.10  2000/05/26 16:52:34  dpp
*      -> expanded tracer info
*
* Revision 1.9  2000/04/26 14:50:17  dpp
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> remove using z hit horizontal scale, unless called from TFXFIT
*      -> use call to RESIDUALS instead of RESICF, also replace z res calc
*      -> move some local variables into sfxtsc.inc
*      -> remove "info" argument for XTS_TFXF
*
* Revision 1.8  2000/02/12 20:23:58  dpp
*      -> display superlayer borders
*
* Revision 1.7  1999/10/08 13:41:37  lyon
* Fixed SunOS directives and fixed CALLs to functions
*
* Revision 1.6  1999/08/03 21:52:35  dpp
* remove annoying iadr() message
*
* Revision 1.5  1999/04/27 20:12:49  dpp
* reorder ETYPE
*
* Revision 1.4  1999/01/29 15:34:43  dpp
* force update
*
* Revision 1.3  1999/01/11 02:46:51  lyon
* SunOS does not allow CALLing functions
*
* Revision 1.2  1998/07/09 00:36:48  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/02/06 00:06:36  zfiles
* allow gaps in strings
*
* Revision 1.3  1996/01/25 15:08:40  zfiles
* use kink to calculate residual
*
* Revision 1.2  1995/12/18 19:45:15  nk
* changes to TFXFIT hit selection, updates of TFXFIT graphics
*
* Revision 1.1  1995/09/25 19:23:58  zfiles
* first version
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :dpp
      SUBROUTINE XTS_TFXF(CALLER)
      IMPLICIT NONE
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfxfitcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
#include "doit/sfseq/xts_cntl.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"


      CHARACTER*(*) CALLER

      REAL ARCFUN
      INTEGER SET_BACKING_C
      INTEGER SET_FOREGROUND_C

      INTEGER TFXSL

      INTEGER IDUM,TEMP

      REAL XADIF,XNDIVX
      INTEGER EXPXAD
      INTEGER NDIVX,NSMLX

      REAL POINSZ
      REAL YADIF
      INTEGER NDIVY,NSMLY


      INTEGER IFIT,ILLYR,IHIT,ILYR,ILYRL,IHYBR,LCOUNT,ITRACE
      INTEGER IHSIGN,IHABS
      REAL RESSIN

      REAL RESHLO,RESHHI,RESLLO,RESLHI

      INTEGER NRESL
      INTEGER IGROUP,IGLAYR,IHCULA,ODDEVN,JCLG
      INTEGER LSTLYR,OTHERL,OTHERH,OTHRHS
      INTEGER IPLACE,L1ST
      REAL SIGNO,X1ST,Y1ST,XLAST,YLAST

      REAL        XPLACE(NGRUP,MGLAYR,MGSTOR,7)
      REAL        YPLACE(NGRUP,MGLAYR,MGSTOR,7)
      REAL        MOVEXX(7,2)
      INTEGER     ODDEVS(NGRUP,MGLAYR,MGSTOR)
      REAL        XMAX  (NGRUP,MGLAYR,MGSTOR)
      INTEGER     YJUSTP(NGRUP,MGLAYR,MGSTOR,7)
      INTEGER     XJUST,YJUST,MOVEYJ      


      REAL        XX(2),YY(2)
      REAL        RAD1ST
      CHARACTER*4  COM1
      CHARACTER*1  COM2
      CHARACTER*6  STRING
      CHARACTER*46 STRNGL

      INTEGER     JLIST,SPREAD,LENT,IERR,MICRON
      REAL        YH_USE,YH_TRACE,YH_AXIS

      INTEGER     PWANT,PNOW

#if defined(CLEO_ULTRIX)
      INTEGER     CLWANT,CLNOW,CLHIT,CLFHIT,CLG(3)
#else
C*****Alpha and SunOS
      INTEGER*8   CLWANT,CLNOW,CLHIT,CLFHIT,CLG(3)
#endif

      INTEGER HITDMI(31),IBIT

      REAL TINY
      DATA TINY/.0000001/

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C=======================================================================
C announcement
C=======================================================================

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,ETYPE,CALLER
 1001 FORMAT(' XTS_TFXF: enter,'
     2     '  ETYPE=',I2,'  called from ',A20)
#endif
      
C=======================================================================
C mode
C=======================================================================

      IF(XTS_TFXFIT_ON.EQ.1)THEN
        IF(.NOT.((TFXS.EQ.1).OR.(TFXS.EQ.2)))THEN
          PRINT 1002,TFXS
 1002     FORMAT(' XTS_TFXF: argument error, TFXS=',I3)
          GO TO 901
        ELSE
          TFXSL=TFXS
        ENDIF
      ELSE
        TFXSL=1
      ENDIF
      
C=======================================================================
C define the axis
C=======================================================================

      ZOOMU=ZOOMU_TFXF(TFXSL)
      
C range of plot WIDTH is RESMIN to RESMAX
      RESMIN=ZOOMXY(3,ZOOMU)
      RESDIF=ZOOMXY(1,ZOOMU)
      RESMAX=RESMIN+RESDIF
      
C range of plot HEIGHT is RADMIN TO RADMAX
      RADMIN=ZOOMXY(4,ZOOMU)
      RADDIF=ZOOMXY(2,ZOOMU)
      RADMAX=RADMIN+RADDIF
      
C x limits  of the X axis
      XAPOSL=RESMIN+.1*RESDIF
      XAPOSR=RESMIN+.9*RESDIF
      
C increments of X axis AND small divisions of X axis
      XADIF=RESDIF/10.
C all of this to just rationalize the increments of X axis
      EXPXAD=0
      
 23   CONTINUE
      IF(XADIF.GE.10.)THEN
        EXPXAD=EXPXAD+1
        XADIF=XADIF/10.
        GO TO 23   
      ENDIF
      
 25   CONTINUE
      IF(XADIF.LT. 1.)THEN
        EXPXAD=EXPXAD-1
        XADIF=XADIF*10.
        GO TO 25   
      ENDIF
      
      IF    (XADIF.LT. 1.5)THEN
        XADIF= 1.
        NSMLX=10
      ELSEIF(XADIF.LT. 3.5)THEN
        XADIF= 2.
        NSMLX=10
      ELSEIF(XADIF.LT. 7.0)THEN
        XADIF= 5.
        NSMLX=5
      ELSE
        XADIF=10.
        NSMLX=10
      ENDIF
      IF(EXPXAD.NE.0)XADIF=XADIF*10.**EXPXAD
      
C number of divisions of X axis
      XNDIVX=(XAPOSR-XAPOSL)/XADIF+.5
      NDIVX= XNDIVX
      
C y position of the X axis
      XAPOSB=RADMIN+.0833*RADDIF
      XAPOST=RADMIN+.9167*RADDIF
      
C x position of the Y axis
      YAPOSL=RESMIN+.0800*RESDIF
      YAPOSR=RESMIN+.9200*RESDIF
      
C y position of the Y axis
      YAPOSB=RADMIN+.0833*RADDIF
      IF(ABS(YAPOSB).LT.0.01)YAPOSB=0.
      YAPOST=RADMIN+.9167*RADDIF
C increments of Y axis AND small divisions of Y axis
      YADIF=(YAPOST-YAPOSB)/10.
      NDIVY=10
      NSMLY=10
      
C=======================================================================
C define the bracket locations
C=======================================================================

      MOVEXX(1,1)=-.010*RESDIF
      MOVEXX(2,1)=-.050*RESDIF
      MOVEXX(3,1)=-.050*RESDIF
      MOVEXX(4,1)=-.060*RESDIF
      MOVEXX(5,1)= .070*RESDIF
      MOVEXX(6,1)=-.190*RESDIF
      MOVEXX(7,1)= .070*RESDIF
      MOVEXX(1,2)= .010*RESDIF
      MOVEXX(2,2)= .050*RESDIF
      MOVEXX(3,2)= .050*RESDIF
      MOVEXX(4,2)= .050*RESDIF
      MOVEXX(5,2)= .070*RESDIF
      MOVEXX(6,2)= .035*RESDIF
      MOVEXX(7,2)= .070*RESDIF
      
C character height in mm and character relative position
      YH_USE=       2.1/200.*MAXSIZ
      YH_TRACE=     2.5/200.*MAXSIZ
      XJUST      =  0
      MOVEYJ     =  -3
      
C point size
      POINSZ=1. *1.2/RADDIF
      IF(POINSZ.GT.2.)POINSZ=2.
      
C make point bigger
c      POINSZ=POINSZ*1.5
      POINSZ=POINSZ*2.
      
      
      IDUM=SET_BACKING_C(0)
      CALL ERASE_C
      IDUM=SET_FOREGROUND_C(CLBORD)
      YH_AXIS=1.8/200.*MAXSIZ
      CALL RESCALE_C(RESDIF, RADDIF, RESMIN, RADMIN)
      CALL AXIS_C(1,
     2     XAPOSL,XAPOSB,XADIF,- NDIVX,NSMLX,-4,'RESIDUAL',YH_AXIS)
      CALL AXIS_C(1,
     2     XAPOSL,XAPOST,XADIF,  NDIVX,NSMLX, 0,       ' ',YH_AXIS)
      CALL AXIS_C(0,
     2     YAPOSL,YAPOSB,YADIF,- NDIVY,NSMLY,-3,  'RADIUS',YH_AXIS)
      CALL AXIS_C(0,
     2     YAPOSR,YAPOSB,YADIF,  NDIVY,NSMLY, 0,       ' ',YH_AXIS)
      
      IDUM=SET_FOREGROUND_C(CLSUBB)
      DO 117 IDUM=1,NBOUNDS
        IF(
     1       (TYP_BOUND(IDUM).EQ.1).OR.
     2       (TYP_BOUND(IDUM).EQ.2))THEN
          XX(1)=RESMIN
          XX(2)=RESMAX
          YY(1)=RBOUNDS(IDUM)
          YY(2)=RBOUNDS(IDUM)
          CALL LINES_C(0,XX,YY,2)
        ENDIF
 117  CONTINUE
      IF(
     1     (RESMIN.LT.0.).AND.
     2     (RESMAX.GT.0.))THEN
        XX(1)=0.
        XX(2)=0.
        YY(1)=YAPOSB
        YY(2)=YAPOST
        CALL LINES_C(0,XX,YY,2)
      ENDIF
      
      
c      CLHIT =CLNORM
c      CLFHIT=CLCHAN
      
      CLHIT =CLDUCH
      CLFHIT=CLDUMB
      
      CLG(1)  =CLDUMB
      CLG(2)  =CLDUCH
      CLG(3)  =CLUSED
      
      
C=======================================================================
C display the hits in the hit list
C=======================================================================

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      
      CLNOW=0
C      CALL SET_FOREGROUND_C(CLNOW)
      PNOW=0

C call KABG2; prepare to call RESIDUALS in loop over hits
      
      CALL KABG2(KABG2_CALC,
     1     0,0,0,KABG2_XI_RECALC,
     2     KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF, PHI6CF,
     3     KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6)
      
      
      PRINT 1015,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      PRINT 1013,KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6
 1013 FORMAT(
     2     ' XTS_TFXF:[after kink  ] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3     ' G=',F9.6,' X=',F6.3,' F6=',F9.6)
 1015 FORMAT(
     2     ' XTS_TFXF:[AFTER KABG2 ] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3     ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4     ' CH=',F9.3)
      
C=======================================================================
C change loop structure allowed with new TFHITCD variables (ie IPLHIT)
C=======================================================================

      IF(ETYPE.EQ.9)THEN
        N_HIT_TFXF=NFIT
      ELSE
        N_HIT_TFXF=NHIT
      ENDIF
      IF(N_HIT_TFXF.LE.0)GO TO 901
      
      ILYRL=0
      DO 179 IHIT=1,N_HIT_TFXF
        
C plot info from tftrakcd

        IF(ETYPE.EQ.9)THEN
          CALL RESIDUALS('FIT',0,IHIT,RESSIN)
          IHSIGN=IADFIT(IHIT)
          IHABS=IABS(IHSIGN)
          ILYR=IABS(IPLFIT(IHIT))
          CLWANT=CLFHIT
          IF(IPLFIT(IHIT).LT.0)CLWANT=CLHIT
          
C plot info from tfhitscd 
          
        ELSE

          CALL RESIDUALS('HIT',0,IHIT,RESSIN)
          IHSIGN=IADRHT(IHIT)
          IHABS=IABS(IHSIGN)
          ILYR=IPLHIT(IHIT)
          
C test if hit is in the fit list and change color
          
          CLWANT=CLHIT
          IF(NFIT.GE.1)THEN
            DO 153 IFIT=1,NFIT
              IF(IPLFIT(IFIT).NE.ILYR)GO TO 153
              IF(IADFIT(IFIT).NE.IADRHT(IHIT))GO TO 153
              CLWANT=CLFHIT
              GO TO 155
 153        CONTINUE


          ENDIF
 155      CONTINUE
        ENDIF
        
        IF(ILYR.NE.ILYRL)LCOUNT=0
        LCOUNT=LCOUNT+1
        
        IF(CLWANT.NE.CLNOW)THEN
          CLNOW=CLWANT
          IDUM=SET_FOREGROUND_C(CLNOW)
        ENDIF
        
C=======================================================================
C test which kind of point to draw
C=======================================================================
        
C silicon, use Z of hit, r-phi hits have center of hybrid
        IF(
     1       (IDVCTF(ILYR).EQ.ISVX1).OR.
     1       (IDVCTF(ILYR).EQ.ISVZ1))THEN
          IHYBR=SV_HYBRID(IHABS)
C           IF(SV_Z(IHABS).GT.0.)THEN
          IF((
     1         SVBOUNDARY_Z(IHYBR,1)+
     2         SVBOUNDARY_Z(IHYBR,2)
     3         ).GT.0.)THEN
            PWANT=1
          ELSE
            PWANT=2
          ENDIF
          
C drift chamber, positive drift distance, points up
        ELSE
          IF(IHSIGN.GT.0)THEN
            PWANT=1
          ELSE
            PWANT=2
           ENDIF
         ENDIF
         
         
         IF(PWANT.NE.PNOW)THEN
           PNOW=PWANT
           IF    (PNOW.EQ.1)THEN
c               CALL SET_SYMBOL_C(7,POINSZ)
             CALL SET_SYMBOL_C(8,POINSZ)
           ELSEIF(PNOW.EQ.2)THEN
c               CALL SET_SYMBOL_C(9,POINSZ)
             CALL SET_SYMBOL_C(10,POINSZ)
           ENDIF
         ENDIF
         
C=======================================================================
C plot the hit
C=======================================================================
         
         IF(IHIT.LE.M_XTS_HITS)THEN
           
           RESSAV(IHIT)=RESSIN
           
           IF(
     1          (IDVCTF(ILYR).EQ.ISVX1).OR.
     1          (IDVCTF(ILYR).EQ.ISVZ1))THEN
           RESSAV(IHIT)=RESSIN
             RADSAV(IHIT)=SQRT(XHIT(IHIT)**2+YHIT(IHIT)**2)
           ELSE
           RESSAV(IHIT)=RESSIN
             RADSAV(IHIT)=RTF(ILYR)
           ENDIF
           
           
           CALL POINTS_C(RESSAV(IHIT),RADSAV(IHIT),1)
           
C record the layer to compare to the next hit
           
           ILYRL=ILYR
           
C end of loop over hits
           
         ELSE
           
           TEMP=M_XTS_HITS
           PRINT 2001,IHIT,TEMP
 2001      FORMAT(' XTS_TFXF: TROUBLE, IHIT > M_XTS_HITS',I10,I10)
           
           
         ENDIF
         
 179   CONTINUE
       
       IF(XTS_TFXFIT_ON.NE.1)GO TO 699
       IF(NGRUP.LE.0)GO TO 699
       
C=======================================================================
C pick ODDEVN so that brackets do not overlap 
C find the largest #2 position, ODDEVN; the side of the display
C=======================================================================
       
       JCLG=1
CCC      JCLG=JCLG+1
CCC      IF(JCLG.EQ.3)JCLG=1
       CLWANT=CLG(JCLG)
       IF(CLWANT.NE.CLNOW)THEN
         CLNOW=CLWANT
         IDUM=SET_FOREGROUND_C(CLNOW)
       ENDIF
       
C=======================================================================
C loop over the groups 
C look through the local abiguity groups
C the DUET hit list above is connected to the ambiguity groups
C  through IHITGR(group,layer_in_group,hit_in_layer)=IHIT 
C=======================================================================
       

       ODDEVN=1
       
       DO 589 IGROUP=1,NGRUP
         IF(NLAYGR(IGROUP,TFXSL).LT.2)GO TO 589
         
C first loop over layers in the group
C determine the limits of residuals within this group
         NRESL=0
         RESLLO= 99999.
         RESLHI=-99999.
         DO 457 IGLAYR=1,NLAYGR(IGROUP,TFXSL)
           IF(NHITGR(IGROUP,IGLAYR).LE.0)GO TO 457
           
           RESHLO= 99999.
           RESHHI=-99999.
C loop over the hits in ths layer
           DO 455 IHCULA=1,NHITGR(IGROUP,IGLAYR)
             IF(LNHTGR(IGROUP,IGLAYR,IHCULA).LT.2)GO TO 455
C loop over associated hits
             OTHERL=IGLAYR
             OTHERH=IHCULA
 442         OTHRHS=OTHERH
 443         CONTINUE
             
             IHIT=IHITGR(IGROUP,OTHERL,OTHERH)
             IF(RESSAV(IHIT).LT.RESHLO)RESHLO=RESSAV(IHIT)
             IF(RESSAV(IHIT).GT.RESHHI)RESHHI=RESSAV(IHIT)
             
C try another hit in the same layer that is essentially identical
             IF(.NOT.MULTIH(IGROUP,TFXSL))GO TO 452
             OTHERH=SAMEAS(IGROUP,OTHERL,OTHERH)
             IF(OTHERH.EQ.0     )GO TO 452
             IF(OTHERH.EQ.OTHRHS)GO TO 452
             GO TO 443
C go to the next layer
 452         LSTLYR=OTHERL
             OTHERL=NXLRGR(IGROUP,LSTLYR,OTHRHS)
             OTHERH=NXHTGR(IGROUP,LSTLYR,OTHRHS)
             IF(OTHERH.NE.0)GO TO 442
 455       CONTINUE
           NRESL=NRESL+1
           IF(RESHLO.LT.RESLLO)RESLLO=RESHLO
           IF(RESHHI.GT.RESLHI)RESLHI=RESHHI
 457     CONTINUE
         
         
C second loop over layers in the group
C determine ODDEVN by comparing RESHLO and RESHHI of the hit-string
C with the limits of the group
         DO 497 IGLAYR=1,NLAYGR(IGROUP,TFXSL)
           IF(NHITGR(IGROUP,IGLAYR).LE.0)GO TO 497
           
           RESHLO= 99999.
           RESHHI=-99999.
C loop over the hits in the layer
           DO 495 IHCULA=1,NHITGR(IGROUP,IGLAYR)
             IF(LNHTGR(IGROUP,IGLAYR,IHCULA).LT.2)GO TO 495
C loop over associated hits
C to determine RESHLO and RESHHI of the hit-string
             OTHERL=IGLAYR
             OTHERH=IHCULA
 462         OTHRHS=OTHERH
 463         CONTINUE
             
             IHIT=IHITGR(IGROUP,OTHERL,OTHERH)
             IF(RESSAV(IHIT).LT.RESHLO)RESHLO=RESSAV(IHIT)
             IF(RESSAV(IHIT).GT.RESHHI)RESHHI=RESSAV(IHIT)
             
C try another hit in the same layer that is essentially identical
             IF(.NOT.MULTIH(IGROUP,TFXSL))GO TO 472
             OTHERH=SAMEAS(IGROUP,OTHERL,OTHERH)
             IF(OTHERH.EQ.0     )GO TO 472
             IF(OTHERH.EQ.OTHRHS)GO TO 472
             GO TO 463
C go to the next layer
 472         LSTLYR=OTHERL
             OTHERL=NXLRGR(IGROUP,LSTLYR,OTHRHS)
             OTHERH=NXHTGR(IGROUP,LSTLYR,OTHRHS)
             IF(OTHERH.NE.0)GO TO 462
 475         CONTINUE
             
C determine and store ODDEVN based on the value of RESHLO and RESHHI 
             
             IF(NRESL.LE.1)THEN
               ODDEVN=3-ODDEVN
             ELSE
               IF    ((ABS(RESHLO-RESLLO).LT.TINY).AND.
     2              (ABS(RESHHI-RESLHI).GT.TINY))THEN
                 ODDEVN=1
               ELSEIF((ABS(RESHLO-RESLLO).GT.TINY).AND.
     2                (ABS(RESHHI-RESLHI).LT.TINY))THEN
                 ODDEVN=2
               ELSE
                 ODDEVN=3-ODDEVN
               ENDIF
              ENDIF
              
              ODDEVS(IGROUP,IGLAYR,IHCULA)=ODDEVN
              SIGNO=              -1.
              IF(ODDEVN.EQ.2)SIGNO=1.
              XMAX(IGROUP,IGLAYR,IHCULA)=0.
              
C loop over associated hits
C calculate the first order #2 positions
              OTHERL=IGLAYR
              OTHERH=IHCULA
              JLIST=0
 482          OTHRHS=OTHERH
              SPREAD=0
 483          JLIST=JLIST+1
              
              IHIT=IHITGR(IGROUP,OTHERL,OTHERH)
              IF(RESSAV(IHIT).LT.RESHLO)RESHLO=RESSAV(IHIT)
              IF(RESSAV(IHIT).GT.RESHHI)RESHHI=RESSAV(IHIT)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
              IHIT=IHITGR(IGROUP,OTHERL,OTHERH)
              IF(JLIST.EQ.1)RAD1ST=RADSAV(IHIT)
              
              XPLACE(IGROUP,OTHERL,OTHERH,1)
     1             =RESSAV(IHIT)+MOVEXX(1,ODDEVN)
              YPLACE(IGROUP,OTHERL,OTHERH,1)
     1             =RADSAV(IHIT) +SPREAD*.0035
              YJUSTP(IGROUP,OTHERL,OTHERH,1)=(JLIST-1)*MOVEYJ
              
              IPLACE=2
              XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
     1             =XPLACE(IGROUP,OTHERL,OTHERH,IPLACE-1)
     2             +MOVEXX(IPLACE,ODDEVN)
              YPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
     1             =YPLACE(IGROUP,OTHERL,OTHERH,IPLACE-1)
              YJUSTP(IGROUP,OTHERL,OTHERH,IPLACE)=
     1             YJUSTP(IGROUP,OTHERL,OTHERH,IPLACE-1)

              IF    (XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)*SIGNO
     2             .GT. XMAX  (IGROUP,IGLAYR,IHCULA)       *SIGNO)
     3             XMAX  (IGROUP,IGLAYR,IHCULA)
     4             =   XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
              
C try another hit in the same layer that is essentially identical
              IF(.NOT.MULTIH(IGROUP,TFXSL))GO TO 492
              OTHERH=SAMEAS(IGROUP,OTHERL,OTHERH)
              IF(OTHERH.EQ.0     )GO TO 492
              IF(OTHERH.EQ.OTHRHS)GO TO 492
              SPREAD=SPREAD+1
              GO TO 483
C go to the next layer
 492          LSTLYR=OTHERL
              OTHERL=NXLRGR(IGROUP,LSTLYR,OTHRHS)
              OTHERH=NXHTGR(IGROUP,LSTLYR,OTHRHS)
              IF(OTHERH.NE.0)GO TO 482
 495        CONTINUE
            
 497      CONTINUE
          
C loop over layers in the group
C reallign the #2 positions and add on the later positions
          DO 587 IGLAYR=1,NLAYGR(IGROUP,TFXSL)
            IF(NHITGR(IGROUP,IGLAYR).LE.0)GO TO 587
C loop over the hits in ths layer
            DO 585 IHCULA=1,NHITGR(IGROUP,IGLAYR)
              IF(LNHTGR(IGROUP,IGLAYR,IHCULA).LT.2)GO TO 585
              
              ODDEVN=ODDEVS(IGROUP,IGLAYR,IHCULA)
              
C loop over associated hits
              L1ST=1
              OTHERL=IGLAYR
              OTHERH=IHCULA
 562          OTHRHS=OTHERH
 563          CONTINUE
              
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
              IPLACE=2
              XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)=
     1             XMAX(IGROUP,IGLAYR,IHCULA)
              XLAST=   XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
              YLAST=   YPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
              
              IF(L1ST.EQ.1)THEN
                X1ST=XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
                Y1ST=YPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
              ENDIF
              
              DO 575 IPLACE=3,7
                XPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
     1               =XPLACE(IGROUP,OTHERL,OTHERH,IPLACE-1)
     2               +MOVEXX(IPLACE,ODDEVN)
                YPLACE(IGROUP,OTHERL,OTHERH,IPLACE)
     1               =YPLACE(IGROUP,OTHERL,OTHERH,IPLACE-1)
                YJUSTP(IGROUP,OTHERL,OTHERH,IPLACE)=
     1               YJUSTP(IGROUP,OTHERL,OTHERH,IPLACE-1)
 575          CONTINUE
              
              XX(1)=     XPLACE(IGROUP,OTHERL,OTHERH,1)
              XX(2)=     XPLACE(IGROUP,OTHERL,OTHERH,2)
              YY(1)=     YPLACE(IGROUP,OTHERL,OTHERH,1)
              YY(2)=     YPLACE(IGROUP,OTHERL,OTHERH,2)
              IF(L1ST.EQ.1)THEN
                XX(2)= XPLACE(IGROUP,OTHERL,OTHERH,3)
              ENDIF
              
              CALL LINES_C(0,XX,YY,2)
              
C the hits are positioned with the current track parameters but the
C residuals are those from the pattern recognition, before the refit
              MICRON=1000000.*MISHTG(IGROUP,OTHERL,OTHERH)+.5
 5001         FORMAT(I6)
              WRITE(STRING,5001,IOSTAT=IERR)MICRON
              LENT=LEN(STRING)
              CALL VEC_STRING_C(XPLACE(IGROUP,IGLAYR,IHCULA,4),
     2             YPLACE(IGROUP,IGLAYR,IHCULA,4),
     3             XJUST, YJUSTP(IGROUP,OTHERL,OTHERH,4),
     5             YH_USE,STRING(1:LENT),0.)
              
C..HITSTA.... HIT STATUS
C        .... 0     NOT ADDED FOR UNKNOWN REASON
C        .... 1     NOT ADDED because it is blocked by LYRUSE
C        .... 2     NOT ADDED BECAUSE THE RESIDUAL IS BAD
C        .... 3     NOT ADDED BECAUSE GROUP CONTAINS A WRONG_LUSETF_LAYER_HIT
C        .... 4     IN FIT ALREADY, BUT IT IS A WRONG_LUSETF_LAYER_HIT
C        .... 5     IN FIT ALREADY, AND IT IS RIGHT LUSETF LAYER HIT
C        .... 6     ADDED
              IF(GRPSTA(IGROUP,IGLAYR,IHCULA).EQ. 1)THEN
                COM2='.'
              ELSE
                IF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 1)THEN
                  COM2='b'
                ELSEIF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 2)THEN
                  COM2='-'
                ELSEIF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 3)THEN
                  COM2='='
                ELSEIF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 4)THEN
                  COM2='f'
                ELSEIF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 5)THEN
                  COM2='F'
                ELSEIF(HITSTA(IGROUP,OTHERL,OTHERH).EQ. 6)THEN
            COM2='X'
          ELSE
            COM2='?' 
          ENDIF
        ENDIF
        
 5002   FORMAT(A1)
        WRITE(STRING,5002,IOSTAT=IERR)COM2
        LENT=LEN(STRING)
        CALL VEC_STRING_C(XPLACE(IGROUP,IGLAYR,IHCULA,5),
     2       YPLACE(IGROUP,IGLAYR,IHCULA,5),
     3       XJUST, YJUSTP(IGROUP,OTHERL,OTHERH,5),
     5       YH_USE,STRING(1:LENT),0.)
        
        
C do these displays only for the first hit in the string
        
        IF(L1ST.NE.1)GO TO 581
        L1ST=0
        
        MICRON=1000000.*MISSTG(IGROUP,OTHERL,OTHERH)+.5
 5004   FORMAT(I6)
        WRITE(STRING,5004,IOSTAT=IERR)MICRON
        LENT=LEN(STRING)
        CALL VEC_STRING_C(XPLACE(IGROUP,IGLAYR,IHCULA,6),
     2       YPLACE(IGROUP,IGLAYR,IHCULA,6),
     3       XJUST, YJUSTP(IGROUP,OTHERL,OTHERH,6),
     5       YH_USE,STRING(1:LENT),0.)
        
        
C GRPSTA .... FLAG TO INDICATE THAT GROUP IS "BEST" OR "KEEP" OR "JUNK"
C        .... -1    GROUP IS JUNK
C        .... 0     UNKNOWN
C        .... 1     GROUP NOT USED
C        .... 2     GROUP ADDED TO HIT LIST
C        .... 3     THIS IS THE SO CALLED BEST GROUP
        IF    (GRPSTA(IGROUP,OTHERL,OTHERH).EQ.-1)THEN
          COM1='JUNK'
        ELSEIF(GRPSTA(IGROUP,OTHERL,OTHERH).EQ. 1)THEN
          COM1='BAD '
        ELSEIF(GRPSTA(IGROUP,OTHERL,OTHERH).EQ. 2)THEN
          COM1='KEEP'
        ELSEIF(GRPSTA(IGROUP,OTHERL,OTHERH).EQ. 3)THEN
          COM1='BEST'
        ELSE
          COM1='?   '
        ENDIF
        
 5003   FORMAT(A4)
        WRITE(STRING,5003,IOSTAT=IERR)COM1
        LENT=LEN(STRING)
        CALL VEC_STRING_C(XPLACE(IGROUP,IGLAYR,IHCULA,7),
     2       YPLACE(IGROUP,IGLAYR,IHCULA,7),
     3       XJUST, YJUSTP(IGROUP,OTHERL,OTHERH,7),
     5       YH_USE,STRING(1:LENT),0.)
 581    CONTINUE
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        
C try another hit in the same layer that is essentially identical
        IF(.NOT.MULTIH(IGROUP,TFXSL))GO TO 582
        OTHERH=SAMEAS(IGROUP,OTHERL,OTHERH)
        IF(OTHERH.EQ.0     )GO TO 582
        IF(OTHERH.EQ.OTHRHS)GO TO 582
        GO TO 563
        
C go to the next layer
 582    LSTLYR=OTHERL
        OTHERL=NXLRGR(IGROUP,LSTLYR,OTHRHS)
        OTHERH=NXHTGR(IGROUP,LSTLYR,OTHRHS)
        IF(OTHERH.NE.0)GO TO 562
        
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        XX(1)=     X1ST
        XX(2)=     XLAST
        YY(1)=     Y1ST
        YY(2)=     YLAST
        CALL LINES_C(0,XX,YY,2)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
        
C end of loop over hits in layer
 585  CONTINUE
C end of loop over layers in group
 587  CONTINUE
C end of loop over groups
 589  CONTINUE
      
      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C display the tracer contents
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      
      CALL VEC_STRING_C(
     2     RESMIN+RESDIF*.2,
     3     RADMIN+RADDIF*.98,
     3     -1,1,2.0,
     4     'CLICK ON A HIT FOR INFO',0.)

      
      IF(HIT_DISPLA.NE.0)THEN
        
        JCLG=1
        CLWANT=CLG(JCLG)
        IF(CLWANT.NE.CLNOW)THEN
          CLNOW=CLWANT
          IDUM=SET_FOREGROUND_C(CLNOW)
        ENDIF
        YJUST=-2
        
        WRITE(STRNGL,2012,IOSTAT=IERR)HIT_DISPLA
 2012   FORMAT('history for road hit ',I4)
        LENT=LEN(STRNGL)
        YJUST=YJUST+3
        CALL VEC_STRING_C(
     1       RESMIN+RESDIF*.50,
     2       RADMIN+RADDIF*.98,
     3       -1,YJUST,YH_TRACE,
     4       STRNGL(1:LENT),0.)
        
        
        
        IF(N_TRACER(HIT_DISPLA).NE.0)THEN
          DO 695 ITRACE=1,N_TRACER(HIT_DISPLA)
            
            WRITE(STRNGL,2013,IOSTAT=IERR)
     1           TRACER(ITRACE,HIT_DISPLA),
     2           TRACER_NUM(1,ITRACE,HIT_DISPLA),
     3           TRACER_NUM(2,ITRACE,HIT_DISPLA)
 2013       FORMAT(A30,X,I6,X,I6)
            LENT=LEN(STRNGL)
            
            YJUST=YJUST+3
            CALL VEC_STRING_C(
     1           RESMIN+RESDIF*.50,
     2           RADMIN+RADDIF*.98,
     3           -1,YJUST,YH_TRACE,
     4           STRNGL(1:LENT),0.)
 695      CONTINUE
        ELSE
          YJUST=YJUST+3
          CALL VEC_STRING_C(
     1         RESMIN+RESDIF*.50,
     2         RADMIN+RADDIF*.98,
     3         -1,YJUST,YH_TRACE,
     4         'hit has no log',0.)
        ENDIF
      ENDIF
      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C break point when the TFXFIT info is not displayed
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      
 699  CONTINUE
      
      CALL UPDATE_C
 901  CONTINUE
      
#if defined(CLEO_XTSDEBUG)
      PRINT 4001
 4001 FORMAT(' XTS_TFXF: EXIT')
      RETURN
#endif
      
      END






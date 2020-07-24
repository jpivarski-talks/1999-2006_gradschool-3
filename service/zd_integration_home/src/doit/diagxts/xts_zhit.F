*
* $Id: xts_zhit.F,v 1.14 2003/03/05 17:30:36 dpp Exp $
*
* $Log: xts_zhit.F,v $
* Revision 1.14  2003/03/05 17:30:36  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.13  2002/09/05 19:09:24  dpp
*      -> use SIGMA_LAYER instead of SIGHTF
*      -> bolder lines for hits
*      -> scale the plot to the detector size
*      -> change color
*
* Revision 1.12  2001/11/19 23:12:28  dpp
*     -> remove use of ILRESC to draw chamber exit; put in chamber length
*
* Revision 1.11  2000/08/18 18:39:26  dpp
*      -> skyline plot of cathodes
*      -> remove excessive print statement
*
* Revision 1.10  2000/05/26 16:49:58  dpp
*      -> remove full listing of cathode hits
*
* Revision 1.9  2000/04/26 14:48:08  dpp
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*      -> remove tftrkscd.inc references
*
* Revision 1.8  1999/11/04 17:22:50  dpp
* fix cathode include file directory
*
* Revision 1.7  1999/11/03 22:36:02  dpp
* print info for the new cathode storage
*
* Revision 1.6  1999/10/08 13:41:38  lyon
* Fixed SunOS directives and fixed CALLs to functions
*
* Revision 1.5  1999/04/27 20:12:15  dpp
* reorder ETYPE; change scale and color, F77
*
* Revision 1.4  1999/01/29 15:34:45  dpp
* force update
*
* Revision 1.3  1999/01/11 02:46:52  lyon
* SunOS does not allow CALLing functions
*
* Revision 1.2  1998/07/09 00:37:25  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/08/08 16:08:28  dpp
* process qdiv hits already in the fit list
*
* Revision 1.3  1995/09/25 19:28:02  zfiles
* use SIGMA rather than 1/sqrt(weight)
* put box around silicon hits
*
* Revision 1.2  1995/04/20  22:39:02  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:01:00  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.25.55  by  Rob Kutschke
*CMZ :  6.00/01 29/01/92  09.57.25  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_ZHIT(CALLER)
      IMPLICIT NONE
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
#include "doit/sfseq/xts_cntl.inc"

#include "CathodeFortranAccess/cathclus.inc"
#include "CathodeFortranAccess/clustopad.inc"
#include "CathodeFortranAccess/cathpads.inc"
#include "CathodeFortranAccess/padtoclusll.inc"


      CHARACTER*(*)CALLER
      INTEGER ILLYR,ILYR,IHIT
      INTEGER IDUM

      REAL XSIZE,YSIZE,XORG,YORG
      REAL SCLSIZX,SCLSIZY,SCLORGX,SCLORGY
      REAL RSQESC,XESCAP,YESCAP,FESCAP,SESC,RESI
      INTEGER IER,ITRY,IFITTR,IADR
      INTEGER GRAB(KLR2TF)
      REAL XX(4),YY(4),USEINT,USESLP,ERRB
      INTEGER OLDWIDTH,NEWWIDTH

      REAL PMPI
      REAL ARCFUN
      INTEGER SET_BACKING_C
      INTEGER SET_FOREGROUND_C
      INTEGER SET_LINE_WIDTH_C

#if defined(CLEO_ULTRIX)
      INTEGER   CLWANT,CLNOW,CLHIT,CLFHIT,CLFEXT,CLLINE,CLESCP
#else
C*****Alpha and SunOS
      INTEGER*8 CLWANT,CLNOW,CLHIT,CLFHIT,CLFEXT,CLLINE,CLESCP
#endif

      REAL X_AT_CATH,Y_AT_CATH,F_AT_CATH,F_DIFF


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C -------------- executable starts here---------------------------------

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,CALLER
1001  FORMAT(' XTS_ZHIT: ENTER:  CALLED FROM ',A20)
#endif

      IF(
     2     (ETYPE.EQ.1).OR.
     3     (ETYPE.EQ.2).OR.
     4     (ETYPE.EQ.3))THEN
         USEINT=Y0LF
         USESLP=TANDLF
      ENDIF

      IF(NFIT.GT.0)THEN
        DO 21 IFITTR=1,NFIT
          GRAB(IFITTR)=0
 21     CONTINUE
      ENDIF
      
      ZOOMU=ZOOMU_SZ
      XSIZE=ZOOMXY(1,ZOOMU)
      YSIZE=ZOOMXY(2,ZOOMU)
      XORG=ZOOMXY(3,ZOOMU)
      YORG=ZOOMXY(4,ZOOMU)

      IDUM=SET_BACKING_C(0)
      CALL ERASE_C


      CALL RESCALE_C(XSIZE,YSIZE,XORG,YORG)
      SCLORGX=XORG+XSIZE*.2/1.4
      SCLSIZX=XSIZE*1./1.4
      SCLORGY=YORG+YSIZE*.2/2.8
      SCLSIZY=YSIZE*2.0/2.8
      IDUM=SET_FOREGROUND_C(CLBORD)


      CALL AXIS_C(1, 0., SCLORGY,.1*SCLSIZX,10,5,3,'PATH LENGTH',2.)
      CALL AXIS_C(1, 0., 0.0    ,.1*SCLSIZX,10,5,0,          ' ',2.)
      CALL AXIS_C(1, 0.,-SCLORGY,.1*SCLSIZX,10,5,0,          ' ',2.)
      CALL AXIS_C(0, 0., SCLORGY,.1*SCLSIZY,12,5,3,'Z POSITION ',2.)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

c      CLHIT =CLNORM
c      CLFHIT=CLCHAN
      CLHIT =CLDUCH
      CLFHIT=CLDUMB

      CLFEXT=CLDUMB
      CLLINE=CLDUCH

c      CLESCP=CLUSED
      CLESCP=CLNORM

      CLNOW=CLHIT
      idum = SET_FOREGROUND_C(CLNOW)

C DRAW THE HITS

      CALL SET_SYMBOL_C(5,2.)
      oldwidth=set_line_width_c(4)

      DO 59 IlLYR=1,NLYRHT
        ILYR=IPLRLR(ILLYR)
        DO 49 IHIT=LHITLR(ILLYR),MHITLR(ILLYR)

          
C test if hit is in the fit list and change the color
          
          CLWANT=CLHIT
          IF(NFIT.GE.1)THEN
            IFITTR=0
 41         IFITTR=IFITTR+1
            IF(
     1           (IFITTR.LE.NFIT).AND.
     2           (CLWANT.NE.CLFHIT))THEN

              IF(
     1             (IPLFIT(IFITTR).EQ.ILYR).AND.
     2             (IADFIT(IFITTR).EQ.IADRHT(IHIT)))THEN
                GRAB(IFITTR)=1
                CLWANT=CLFHIT
              ENDIF
              GO TO 41

            ENDIF
          ENDIF
        
          IF(CLWANT.NE.CLNOW)THEN
            CLNOW=CLWANT
            idum = SET_FOREGROUND_C(CLNOW)
          ENDIF
          
          
C DRAW A LARGE SYMBOL FOR SOME POINTS, IE CATHODES
          IF((IDVCTF(ILYR).EQ.IVDCT).OR.
     2         (IDVCTF(ILYR).EQ.IDRCT).OR.
     2         (IDVCTF(ILYR).EQ.ISVZ1)    )
     4         CALL POINTS_C(SHIT(IHIT),ZHIT(IHIT),1)
          
C DRAW THE HORIZONTAL LINE
          
          XX(1)=SHIT(IHIT)-.004
          XX(2)=SHIT(IHIT)+.004
          YY(1)=ZHIT(IHIT)
          YY(2)=YY(1)
          CALL LINES_C(0,XX,YY,2)
          
          
C DRAW THE VERTICAL LINE; ERROR BAR
          
          XX(1)=SHIT(IHIT)
          XX(2)=XX(1)
          ERRB= SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)
          YY(1)=ZHIT(IHIT)-ERRB
          YY(2)=ZHIT(IHIT)+ERRB
          CALL LINES_C(0,XX,YY,2)
          
 49     CONTINUE
 59   CONTINUE
      newwidth=set_line_width_c(oldwidth)



C*******************************************************
C draw the cathodes directly from the new arrays 
C*******************************************************

      DO 69 ILYR=1,NLYRTF
        IF((IDVCTF(ILYR).EQ.IVDCT).OR.
     2       (IDVCTF(ILYR).EQ.IDRCT))THEN
          
          CLWANT=CLESCP
          IF(CLNOW.NE.CLWANT)THEN
            CLNOW=CLWANT
            idum = SET_FOREGROUND_C(CLNOW)
          ENDIF
          
          XX(1)=.9
          YY(1)=-CELZTF(ILYR)*NWRZTF(ILYR)/2.
          XX(2)=.9
          YY(2)= CELZTF(ILYR)*NWRZTF(ILYR)/2.
          CALL LINES_C(0,XX,YY,2)
          
          CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2         RSQTF(ILYR),+1,
     3         X_AT_CATH,Y_AT_CATH,F_AT_CATH,IER)
          
      PRINT 3001,N_PAD_CATH,F_AT_CATH,CELLTF(ILYR)
 3001 FORMAT(' XTS_ZHIT: N_PAD_CATH=',I5,
     2     '  F_AT_CATH=',F9.4,'  CELLTF=',F9.4)

          IF(N_PAD_CATH.Gt.0)THEN
            DO 67 IHIT=1,N_PAD_CATH
              F_DIFF=PMPI(PADPHI(IHIT)-F_AT_CATH)

              IF(ABS(F_DIFF).LT.CELLTF(ILYR)*.50)THEN
                XX(1)=.9
                XX(2)=.9+.1*PADCH(IHIT)/1000.
                XX(3)=XX(2)
                XX(4)=XX(1)
                YY(1)=PADZ(IHIT)-CELZTF(ILYR)/2.
                YY(2)=YY(1)
                YY(3)=PADZ(IHIT)+CELZTF(ILYR)/2.
                YY(4)=YY(3)
                CALL LINES_C(5,XX,YY,4)
              ENDIF
 67         CONTINUE
          ENDIF
        ENDIF
 69   CONTINUE

C*******************************************************



C DRAW THE FITTED HITS NOT IN HIT LIST, IE THE QDIV HITS
C ASSUME THAT QDIV HITS ARE NO LONGER IN STACK
C AND IGNORE THEM IF THEY ARE

      CALL SET_SYMBOL_C(1,2.5)
      DO 71 IFITTR=1,NFIT
         ILYR=IPLFIT(IFITTR)

         IF((ILYR.GT.QDIVAD).OR.(ILYR.GT.KLYRTF))THEN
C process charge division
C ignore charge division ihts in the stack
         ELSE

C recreate the charge division hits
            CLWANT=CLHIT
            IADR = IABS(IADFIT(IFITTR))
            IF(  (GRAB(IFITTR).EQ.0)
     2      .AND. LQDVTF(ILYR)
     3      .AND.(EvntHit_NxtCls(IADR).GT.0))THEN
               XX(1)=SFIT(IFITTR)
               YY(1)=EvntHit_Z(IADR)
               RESI=EvntHit_Z(IADR)-(USEINT + SFIT(IFITTR)*USESLP)
               IF((WGTQTF(ILYR)*RESI**2).LT.16)CLWANT=CLFEXT
               IF(CLNOW.NE.CLWANT)THEN
                  CLNOW=CLWANT
                  idum = SET_FOREGROUND_C(CLNOW)
               ENDIF
               CALL POINTS_C(XX(1),YY(1),1)
            ENDIF   
         ENDIF   
 71   CONTINUE
         
C DRAW THE FITTED LINE

      CLWANT=CLLINE
      IF(CLNOW.NE.CLWANT)THEN
         CLNOW=CLWANT
         idum = SET_FOREGROUND_C(CLNOW)
      ENDIF

      XX(1)=0.
      YY(1)=USEINT
      XX(2)=1.
      YY(2)=USEINT+USESLP
      IF((ABS(YY(2)).GT.1.2).AND.(USESLP.NE.0.))THEN
         YY(2)=1.2
         IF(USESLP.LT.0.)YY(2)=-1.2
         XX(2)=(YY(2)-USEINT)/USESLP
      ENDIF
      CALL LINES_C(0,XX,YY,2)
      PRINT 3005,USEINT,USESLP
3005  FORMAT(' XTS_ZHIT: Z INTERCEPT=',F8.4,' CT=',F8.4)

C ENTER IN THE ENDPLATE ESCAPE

      CALL SET_SYMBOL_C(3,2.)
      CLWANT=CLESCP
      IF(CLNOW.NE.CLWANT)THEN
         CLNOW=CLWANT
         idum = SET_FOREGROUND_C(CLNOW)
      ENDIF

      ILYR=0
      IER=0
 81   ILYR=ILYR+1
      IF((ILYR.LE.NLYRTF).AND.(IER.EQ.0))THEN
         IF(
     1       (IDVCTF(ILYR).EQ.IDRFT).OR.
     2       (IDVCTF(ILYR).EQ.IZDA).OR.
     3       (IDVCTF(ILYR).EQ.ITPC1))THEN
            RSQESC=RENDTF(ILYR)**2
            ITRY=1
 82         CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQESC,+1,
     1           XESCAP,YESCAP,FESCAP,IER)
            IF((IER.NE.0).AND.(ITRY.EQ.1))THEN
               RSQESC=(RENDTF(ILYR)-.014)**2
               ITRY=2
               GO TO 82
            ENDIF
            IF(IER.EQ.0)THEN
               SESC=ARCFUN(KAPCF,ALPCF,BETCF,XESCAP,YESCAP,XICF)
               CALL POINTS_C(SESC, ZENDTF(ILYR),1)
               CALL POINTS_C(SESC,-ZENDTF(ILYR),1)
            ENDIF
         ENDIF
         GO TO 81
      ENDIF
      CALL UPDATE_C

#if defined(CLEO_XTSDEBUG)
      PRINT 4001
4001  FORMAT(' XTS_ZHIT: EXIT')
      RETURN
#endif

      END

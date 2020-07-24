*
* $Id: xts_zhit.F,v 1.11 2000/08/18 18:39:26 dpp Exp $
*
* $Log: xts_zhit.F,v $
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
* use SIGHTF rather than 1/sqrt(weight)
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
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
C#include "doit/duseq/tftrkscd.inc"
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

      REAL RSQESC,XESCAP,YESCAP,FESCAP,SESC,RESI
      INTEGER IER,ITRY,IFITTR,IADR
      INTEGER GRAB(KLR2TF)
      REAL XX(4),YY(4),USEINT,USESLP,ERRB

      REAL PMPI
      REAL ARCFUN
      INTEGER SET_BACKING_C
      INTEGER SET_FOREGROUND_C

#if defined(CLEO_ULTRIX)
      INTEGER   CLWANT,CLNOW,CLHIT,CLFHIT,CLFEXT,CLLINE,CLESCP
#else
C*****Alpha and SunOS
      INTEGER*8 CLWANT,CLNOW,CLHIT,CLFHIT,CLFEXT,CLLINE,CLESCP
#endif

      INTEGER CATHODE_TF
      PARAMETER (CATHODE_TF=56)
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

      IF(NFIT.LE.0)GO TO 23
      DO 21 IFITTR=1,NFIT
21    GRAB(IFITTR)=0


23    IDUM=SET_BACKING_C(0)
      CALL ERASE_C
      CALL RESCALE_C(1.4,2.8,-.2,-1.4)
      IDUM=SET_FOREGROUND_C(CLBORD)
      CALL AXIS_C(1, 0.,-1.2,.1,10,5,2,'PATH LENGTH',2.)
      CALL AXIS_C(1, 0., 0.0,.1,10,5,0,          ' ',2.)
      CALL AXIS_C(1, 0., 1.2,.1,10,5,0,          ' ',2.)
      CALL AXIS_C(0, 0.,-1.2,.2,12,5,3,'Z POSITION ',2.)

      CLHIT =CLNORM
      CLFHIT=CLCHAN
      CLFEXT=CLDUMB
      CLLINE=CLDUCH
      CLESCP=CLUSED

      CLNOW=CLHIT
      idum = SET_FOREGROUND_C(CLNOW)

C DRAW THE HITS

      CALL SET_SYMBOL_C(5,2.)
      DO 59 IlLYR=1,NLYRHT
      ILYR=IPLRLR(ILLYR)
      DO 49 IHIT=LHITLR(ILLYR),MHITLR(ILLYR)

      CLWANT=CLHIT

C test if hit is in the fit list and change the color

      IF(NFIT.GE.1)THEN
         DO 43 IFITTR=1,NFIT
         IF(IPLFIT(IFITTR).NE.ILYR)GO TO 43
         IF(IADFIT(IFITTR).NE.IADRHT(IHIT))GO TO 43
         GRAB(IFITTR)=1
         CLWANT=CLFHIT
         GO TO 45
43    CONTINUE
      ENDIF

45    IF(CLWANT.NE.CLNOW)THEN
         CLNOW=CLWANT
         idum = SET_FOREGROUND_C(CLNOW)
      ENDIF


C DRAW A LARGE SYMBOL FOR SOME POINTS, IE CATHODES
      IF((IDVCTF(ILYR).EQ.IVDCT).OR.
     2   (IDVCTF(ILYR).EQ.IDRCT).OR.
     2   (IDVCTF(ILYR).EQ.ISVZ1)    )
     4CALL POINTS_C(SHIT(IHIT),ZHIT(IHIT),1)

C DRAW THE HORIZONTAL LINE

      XX(1)=SHIT(IHIT)-.004
      XX(2)=SHIT(IHIT)+.004
      YY(1)=ZHIT(IHIT)
      YY(2)=YY(1)
      CALL LINES_C(0,XX,YY,2)

C DRAW THE VERTICAL LINE; ERROR BAR

      XX(1)=SHIT(IHIT)
      XX(2)=XX(1)
C OR COULD USE SIGHTF; ALSO IN TFGEOMCD
C      ERRB=1./SQRT(WGTFTF(ILYR))
      ERRB=SIGHTF(ILYR)
      YY(1)=ZHIT(IHIT)-ERRB
      YY(2)=ZHIT(IHIT)+ERRB
      CALL LINES_C(0,XX,YY,2)

49    CONTINUE
59    CONTINUE



C*******************************************************
C draw the cathodes directly from the new arrays 
C*******************************************************
      IF(CATHODE_TF.NE.0)THEN
         CLWANT=CLESCP
         IF(CLNOW.NE.CLWANT)THEN
            CLNOW=CLWANT
            idum = SET_FOREGROUND_C(CLNOW)
         ENDIF

         XX(1)=.9
         YY(1)=-CELZTF(CATHODE_TF)*NWRZTF(CATHODE_TF)/2.
         XX(2)=.9
         YY(2)= CELZTF(CATHODE_TF)*NWRZTF(CATHODE_TF)/2.
         CALL LINES_C(0,XX,YY,2)
         
         CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2        RSQTF(CATHODE_TF),+1,
     3        X_AT_CATH,Y_AT_CATH,F_AT_CATH,IER)
         
         IF(N_PAD_CATH.Gt.0)THEN
            DO 69 IHIT=1,N_PAD_CATH
               F_DIFF=PMPI(PADPHI(IHIT)-F_AT_CATH)
               IF(ABS(F_DIFF).LT.CELLTF(CATHODE_TF)*.50)THEN
                  XX(1)=.9
                  XX(2)=.9+.1*PADCH(IHIT)/1000.
                  XX(3)=XX(2)
                  XX(4)=XX(1)
                  YY(1)=PADZ(IHIT)-CELZTF(CATHODE_TF)/2.
                  YY(2)=YY(1)
                  YY(3)=PADZ(IHIT)+CELZTF(CATHODE_TF)/2.
                  YY(4)=YY(3)
                  CALL LINES_C(5,XX,YY,4)
               ENDIF
 69         CONTINUE
         ENDIF
      ENDIF
      

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
     3      .AND.(INXHCD(IADR).GT.0))THEN
               XX(1)=SFIT(IFITTR)
               YY(1)=ZWCD(IADR)
               RESI=ZWCD(IADR)-(USEINT + SFIT(IFITTR)*USESLP)
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
      IF(ABS(YY(2)).LT.1.)GO TO 73
      IF(USESLP.EQ.0.)GO TO 73
      YY(2)=1.
      IF(USESLP.LT.0.)YY(2)=-1.
      XX(2)=(YY(2)-USEINT)/USESLP
73    CALL LINES_C(0,XX,YY,2)
      PRINT 3005,USEINT,USESLP
3005  FORMAT(' XTS_ZHIT: Z INTERCEPT=',F8.4,' CT=',F8.4)

C ENTER IN THE ENDPLATE ESCAPE

      IF(ILRESC(2).EQ.0)GO TO 89
      PRINT 3001,ILRESC(2),RENDTF(ILRESC(2))
3001  FORMAT(' XTS_ZHIT:2:  RENTF(',I3,')=',F10.3)
      RSQESC=RENDTF(ILRESC(2))**2
      ITRY=1
81    CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQESC,+1,
     1            XESCAP,YESCAP,FESCAP,IER)
      IF(IER.EQ.0)GO TO 82
      IF(ITRY.EQ.2)GO TO 89
      RSQESC=RENDTF(ILRESC(2)-.014)**2
      ITRY=2
      GO TO 81
82    SESC=ARCFUN(KAPCF,ALPCF,BETCF,XESCAP,YESCAP,XICF)
      CLWANT=CLESCP
      IF(CLNOW.NE.CLWANT)THEN
         CLNOW=CLWANT
         idum = SET_FOREGROUND_C(CLNOW)
      ENDIF
      CALL SET_SYMBOL_C(3,4.)
      CALL POINTS_C(SESC, ZENDTF(ILRESC(2)),1)
      CALL POINTS_C(SESC,-ZENDTF(ILRESC(2)),1)
      IF(ILRESC(1).EQ.0)GO TO 89
      PRINT 3002,ILRESC(1),RENDTF(ILRESC(1))
3002  FORMAT(' XTS_ZHIT:1:  RENTF(',I3,')=',F10.3)
      RSQESC=RENDTF(ILRESC(1))**2
      CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQESC,+1,
     1            XESCAP,YESCAP,FESCAP,IER)
      IF(IER.EQ.0)GO TO 87
      GO TO 89
87    SESC=ARCFUN(KAPCF,ALPCF,BETCF,XESCAP,YESCAP,XICF)
      CALL POINTS_C(SESC, ZENDTF(ILRESC(1)),1)
      CALL POINTS_C(SESC,-ZENDTF(ILRESC(1)),1)
89    CONTINUE

      CALL UPDATE_C

#if defined(CLEO_XTSDEBUG)
      PRINT 4001
4001  FORMAT(' XTS_ZHIT: EXIT')
      RETURN
#endif

      END























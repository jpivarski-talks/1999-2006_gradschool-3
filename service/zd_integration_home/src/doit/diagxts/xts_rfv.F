
* $Id: xts_rfv.F,v 1.15 2003/03/05 17:25:12 dpp Exp $
*
* $Log: xts_rfv.F,v $
* Revision 1.15  2003/03/05 17:25:12  dpp
*      -> define info option for 'next hit'
*
* Revision 1.14  2000/12/19 23:50:13  dpp
*      -> new answer: other2
*
* Revision 1.13  2000/10/02 20:53:33  dpp
*     -> remove extranious C in first line
*
* Revision 1.12  2000/08/18 18:40:26  dpp
*      -> implement new choice: multi-hit
*
* Revision 1.11  2000/04/26 14:52:08  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> add call to xts_select_hit_tfxf if click in field
*      -> remove "info" argument for XTS_TFXF
*
* Revision 1.10  2000/01/24 19:54:00  dpp
*      -> allow integer input to be 5 character
*
* Revision 1.9  1999/11/29 21:43:37  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.8  1999/10/12 15:11:26  dpp
* define usse on button "next pass"
*
* Revision 1.7  1999/08/03 21:52:14  dpp
* remove annoying ICHOICE message
*
* Revision 1.6  1999/05/28 03:04:45  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1.1.1  1998/07/26 19:45:39  lyon
* DoitProcessor (Wrapped DOIT)
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.4  1995/12/18 19:45:05  nk
* changes to TFXFIT hit selection, updates of TFXFIT graphics
*
* Revision 1.3  1995/09/25 19:22:14  zfiles
* compatibility with TFXF display
*
* Revision 1.2  1995/04/20  22:38:59  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/14 01/03/94  17.18.34  by  Rob Kutschke
*CMZ :  6.00/07 31/08/93  14.18.07  by  Rob Kutschke
*CMZ :  6.00/01 03/06/93  18.05.19  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author : D. Peterson
      SUBROUTINE XTS_RFVIEW
      IMPLICIT NONE
      SAVE
C +SEQ,CDGEOMPA. INCLUDED IN TFINDPAR
#include "doit/duseq/tfindpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "cl3seq/cdgm3/cdraw1in.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
#include "doit/sfseq/xts_cntl.inc"
#include "doit/duseq/cdscrtcd.inc"

C THESE ADDED WHEN CONVERTED TO XTS
      INTEGER GET_USRTXT_C
      INTEGER ICHOICE,TYPCHO
      REAL XCHO,YCHO

      INTEGER SEN_HOLD(5)

      INTEGER NINFO
      INTEGER KILLA
      INTEGER INITOK/0/

      INTEGER K,L,LEN,GETOPN
      INTEGER STOPIT
      CHARACTER*6 GETSTR
      CHARACTER*1 GETONE,TESTN(11)
      DATA TESTN/'0','1','2','3','4','5','6','7','8','9',' '/

C*****LABCHO is integer*8 but ichoice is integer*4
      INTEGER*8 dchoice
      INTEGER*8 offset
      PARAMETER (OFFSET = 4294967296)
C*****use the offset: dchoice = ichoice + offset

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C EXECUTABLE CODE STARTS HERE
C-----------------------------------------------------------------------

      ENTRFV=ENTRFV_RFVIEW
      CALL XTS_ZOOM_RFV('XTS_RFVIEW')
      XTS_ABORT=0

C-----------------------------------------------------------------------
C KILLA= 0 plot and wait for acknowledge
C KILLA= 1 plot but do not wait for input
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C initialization for first call
C XTS_RFVIEW_INIT calls XTS_PREP
C-----------------------------------------------------------------------

      IF(INITOK.EQ.0)THEN
         INITOK=1
         KILLA=0
         CALL XTS_RFVIEW_INIT('XTS_RFVIEW')
         
C setup for later calls
      ELSE
         CALL XTS_PREP('XTS_RFVIEW')
      ENDIF

C-----------------------------------------------------------------------
C XTS_TFXFIT_ON is used for switching the TFXFIT group display
C-----------------------------------------------------------------------
      XTS_TFXFIT_ON=0

C-----------------------------------------------------------------------
C first plot the event
C-----------------------------------------------------------------------
      GO TO 101

C-----------------------------------------------------------------------
C post the menu and wait for  choice
C-----------------------------------------------------------------------

40    IF(KILLA.EQ.1)GO TO 989
      PRINT 1091
1091  FORMAT(' XTS_RFVIEW:',
     1     ' posting menu, type "K"ill to eliminate breakpoint')
      GO TO 42

      ENTRY XTS_SCREEN
      ENTRFV=ENTRFV_SCREEN
      GO TO 41

      ENTRY XTS_SCREEN_D
      ENTRFV=ENTRFV_SCREEN_D

C start point XTS_SCREEN and XTS_SCREEN_D entries; these reset KILLA switch
C start point after call to XTS_TFXF; this resets KILLA switch
41    KILLA=0

42    ICHOICE=1

C when a point in the field is clicked, NEXT_EVENT_C with TYPCHO=1 
C  (this in the button number)
C if it is left TYPCHO=1,
C   NEXT_EVENT_C returns (before any further inputs) with TYPCHO=-1
C if TYPCHO is set =0,   
C   NEXT_EVENT_C returns (before any further inputs) with TYPCHO=-1
C if TYPCHO is set =-1,   
C   NEXT_EVENT_C returns (before any further inputs) with TYPCHO=-1
C if ICHOICE=0 entering NEXT_EVENT_C , still
C  returns (before any further inputs) with TYPCHO=-1
C if a call to CALL STRING_C before the next NEXT_EVENT_CC, still
C  returns (before any further inputs) with TYPCHO=-1
C it really appears that 
C   NEXT_EVENT_C has some internal flag that needs to be flushed
C even when NEXT_EVENT_C is call somewhere else and intentionally
C given a field click, it flushes out that input here 

50    CONTINUE

#if defined(CLEO_XTSDEBUG)
      PRINT 1010,ICHOICE,TYPCHO
 1010 FORMAT(' XTS_RFVIEW:',
     1     ' will call NEXT_EVENT_C, ICHOICE,TYPCHO=',2I10)  
#endif

      CALL NEXT_EVENT_C(ICHOICE,TYPCHO,XCHO,YCHO)

#if defined(CLEO_XTSDEBUG)
      PRINT 1011,ICHOICE,TYPCHO,XCHO,YCHO
1011  FORMAT(' XTS_RFVIEW:',
     1     '    NEXT_EVENT_C returned ICHOICE,TYPCHO=',2I10,
     2     ' {x,y}=',2F12.6)
#endif

C process the NEXT_EVENT_C selection

      IF(TYPCHO.EQ.0)THEN
         PRINT 1012
 1012    FORMAT(' XTS_RFVIEW: MAIN WINDOW EXPOSED')
         GO TO 50

      ELSEIF(TYPCHO.EQ.1)THEN
         IF(ICHOICE.EQ.0)THEN

            IF(
     1           (ENTRFV.EQ.ENTRFV_TFXFIT).AND.
     2           (XTS_TFXFIT_ON.NE.0))THEN
               GO TO 533
            ELSE
               GO TO 40
            ENDIF

         ELSE
            DCHOICE = ICHOICE + OFFSET
            IF (DCHOICE.EQ.LABCHO(LAREDRAW    ))GO TO 105
            IF (DCHOICE.EQ.LABCHO(LANEW_DRAW  ))GO TO 107
            IF (DCHOICE.EQ.LABCHO(LAZOOM      ))GO TO 211
            IF (DCHOICE.EQ.LABCHO(LAFULLSIZE  ))GO TO 221
            IF (DCHOICE.EQ.LABCHO(LAFORWARD   ))GO TO 231
            IF (DCHOICE.EQ.LABCHO(LABACKUP    ))GO TO 241
            IF (DCHOICE.EQ.LABCHO(LAID_TDC_ADC))GO TO 301
            IF (DCHOICE.EQ.LABCHO(LAID_MULTIH ))GO TO 308
            IF (DCHOICE.EQ.LABCHO(LAIC_CCC    ))GO TO 302
            IF (DCHOICE.EQ.LABCHO(LADISTANCE  ))GO TO 304
            IF (DCHOICE.EQ.LABCHO(LAWIR_DUET  ))GO TO 306
            IF (DCHOICE.EQ.LABCHO(LANEXT_HIT  ))GO TO 309
            IF (DCHOICE.EQ.LABCHO(LAINUCHN    ))GO TO 303
            IF (DCHOICE.EQ.LABCHO(LAISTFOR    ))GO TO 305
            IF (DCHOICE.EQ.LABCHO(LACOLOR     ))GO TO 307
            IF (DCHOICE.EQ.LABCHO(LADSFDCH    ))GO TO 321
            IF (DCHOICE.EQ.LABCHO(LADSFDMP    ))GO TO 351
            IF (DCHOICE.EQ.LABCHO(LANEXT_EVENT))GO TO 931
            IF (DCHOICE.EQ.LABCHO(LANEXT_PASS ))GO TO 932
            IF (DCHOICE.EQ.LABCHO(LAYES       ))GO TO 411
            IF (DCHOICE.EQ.LABCHO(LANO        ))GO TO 412
            IF (DCHOICE.EQ.LABCHO(LAALWAYS    ))GO TO 413
            IF (DCHOICE.EQ.LABCHO(LAKILL      ))GO TO 414
            IF (DCHOICE.EQ.LABCHO(LAOTHER     ))GO TO 415
            IF (DCHOICE.EQ.LABCHO(LAOTHER2    ))GO TO 416
            IF (DCHOICE.EQ.LABCHO(LAMORE      ))GO TO 417
            IF (DCHOICE.EQ.LABCHO(LACR        ))GO TO 418
            IF (DCHOICE.EQ.LABCHO(LAZHIT      ))GO TO 511
            IF (DCHOICE.EQ.LABCHO(LARESI      ))GO TO 531
            IF (DCHOICE.EQ.LABCHO(LALOOP_1    ))GO TO 535
            IF (DCHOICE.EQ.LABCHO(LALOOP_2    ))GO TO 537
            GO TO 40
         ENDIF
      ELSE
         GO TO 50
      ENDIF

C draw a new picture with (pre SFIND) colors
101   CONTINUE
      CALL XTS_ZOOM_UNZOOM_SUPPRESS('XTS_RFVIEW')
      CALL XTS_NEW_PIXMAP('XTS_RFVIEW')

      IF(INITOK.GE.2)GO TO 40
      INITOK=2
C this is a recursive call since SFCRATE call XTS_RFVIEW_INPUT
      CALL XTS_RFVIEW_INIT_MORE('XTS_RFVIEW')
      GO TO 40

C draw a new picture from the PIXMAP and draw the tracks
105   CALL XTS_RFVIEW2_TRAKS('XTS_RFVIEW')
      GO TO 40

C draw a new picture and create new PIXMAP and draw the tracks
107   CALL XTS_NEW_PIXMAP_TRAKS('XTS_RFVIEW')
      GO TO 40

C zoom
211   CALL XTS_ZOOM('XTS_RFVIEW')
      GO TO 40
221   CALL XTS_ZOOM_UNZOOM('XTS_RFVIEW')
      GO TO 40
231   CALL XTS_ZOOM_FORWARD('XTS_RFVIEW')
      GO TO 40
241   CALL XTS_ZOOM_BACKWARD('XTS_RFVIEW')
      GO TO 40

C info
301   NINFO=1
      GO TO 311
302   NINFO=2
      GO TO 311
303   NINFO=3
      GO TO 311
304   NINFO=4
      GO TO 311
305   NINFO=5
      GO TO 311
306   NINFO=6
      GO TO 311
307   NINFO=7
      GO TO 311
308   NINFO=8
      GO TO 311
309   NINFO=9
311   CALL XTS_RFVIEW2_TRAKS('XTS_RFVIEW')
      CALL XTS_HITS(NINFO,'XTS_RFVIEW')
      GO TO 40

C run DSFDCH
321   CONTINUE
#if defined(CLEO_SFDIAG)
      DO 325 K=1,MCHAIN
      IF(SHOWCH(K).NE.3)GO TO 325
      PRINT 1031,K
1031  FORMAT(' XTS_RFVIEW:FOUND DISPLAYED CHAIN NUMER',I3,
     1       ' WILL CALL DSFDCH')
      CALL DSFDCH(K+0,-1)
325   CONTINUE
#endif

#if !defined(CLEO_SFDIAG)
      PRINT 1032
1032  FORMAT(' XTS_RFVIEW: ----->   DSFDCH NOT ALLOWED')
#endif

      GO TO 40

C run DSFDMP
C lock out recursive call to DSFDMP
351   CONTINUE
      IF(ENTRFV.EQ.ENTRFV_SCREEN_D)THEN
         PRINT 3001
 3001    FORMAT(' WILL NOT CALL DSFDMP FROM ENTRY FROM DSFDMP')
         GO TO 989
      ENDIF
      CALL DSFDMP
      GO TO 40

C answer questions from other programs
C bypass resetting KILLA
      ENTRY XTS_RFV_INPUT
401   ENTRFV=ENTRFV_INPUT
      IF(INITOK.EQ.0)GO TO 430
      PRINT 4002
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
4002  FORMAT('      .....XTS_RFV_INPUT: request input from the MENU')
4003  FORMAT('      .....XTS_RFV_INPUT: request input from TERMINAL')
4005  FORMAT('      .....XTS_RFV_INPUT:                echo input..',A4)
      GO TO 42

411   XTS_ANSWER_IY='Y'
      GO TO 421
412   XTS_ANSWER_IY='N'
      GO TO 421
413   XTS_ANSWER_IY='A'
      GO TO 421
414   XTS_ANSWER_IY='K'
      GO TO 421
415   XTS_ANSWER_IY='O'
      GO TO 421
 416  XTS_ANSWER_IY='2'
      GO TO 421
 417  XTS_ANSWER_IY='M'
      GO TO 421
 418  XTS_ANSWER_IY='null'

421   CONTINUE
      PRINT 4005,XTS_ANSWER_IY

      IF(ENTRFV.EQ.ENTRFV_ZHIT)GO TO 521
      IF(ENTRFV.EQ.ENTRFV_TFXFIT)GO TO 541
      IF(ENTRFV.NE.ENTRFV_RFVIEW)GO TO 989
      IF(XTS_ANSWER_IY.EQ.'K')KILLA=1
      GO TO 989

C this requests input from the terminal if the XTSUBS screen is not open
430   PRINT 4003
      READ 4004,XTS_ANSWER_IY
4004  FORMAT(A1)
      IF(XTS_ANSWER_IY .EQ. 'Y')GO TO 421
      IF(XTS_ANSWER_IY .EQ. 'N')GO TO 421
      IF(XTS_ANSWER_IY .EQ. 'A')GO TO 421
      IF(XTS_ANSWER_IY .EQ. 'K')GO TO 421
      IF(XTS_ANSWER_IY .EQ. 'O')GO TO 421
      IF(XTS_ANSWER_IY .EQ. 'M')GO TO 421
      IF(XTS_ANSWER_IY .EQ. ' ')GO TO 423
      GO TO 430
423   XTS_ANSWER_IY = 'null'
      GO TO 421

C get an integer for another program
      ENTRY XTS_RFV_GETINT
      ENTRFV=ENTRFV_GETINT
450   CONTINUE
      LEN=GET_USRTXT_C('>integer<','      ',GETSTR)
      XTS_ANSWER_IY=0
      IF(LEN.LT.1)GO TO 471
      GETOPN=0
      DO 467 L=1,LEN
      GETONE=GETSTR(L:L)
      DO 463 K=1,10
      IF(GETONE.NE.TESTN(K))GO TO 463
      GETOPN=1
      XTS_ANSWER_IY=XTS_ANSWER_IY *10  +   K-1
      GO TO 467
463   CONTINUE
      IF(GETONE.NE.TESTN(11))GO TO 465
      IF(GETOPN.EQ.0)GO TO 467
      GO TO 471
465   PRINT 1021,L,GETONE,XTS_ANSWER_IY,LEN
1021  FORMAT(' XTS_RFV_GETINT: ERROR AT CHARACTER#',I3,
     1  ' FOUND..',A1,'..AFTER',I10,' LEN=',I1)
      GO TO 450
467   CONTINUE
471   CONTINUE
      GO TO 989

C print or plot information on the z hits in TFHITSCD
511   ENTRFV=ENTRFV_ZHIT
      CALL XTS_ZHIT('XTS_RFVIEW')
      CALL XTS_SENSITIZE(2,LACR,1,0)
      CALL XTS_SENSITIZE(1,0,0,SEN_HOLD(1))
      GO TO 41
521   ZOOMU=ZOOMU_RPHI
      CALL XTS_ZOOM_REDRAW('XTS_RFVIEW')
      CALL XTS_SENSITIZE(0,0,0,SEN_HOLD(1))
      GO TO 401

C print or plot the "TFXFIT" display
531   ENTRFV=ENTRFV_TFXFIT
      CALL XTS_ZOOM_TFXF('XTS_RFVIEW')
      CALL XTS_TFXF('XTS_RFVIEW')
      CALL XTS_SENSITIZE(2,LACR,1,0)
      CALL XTS_SENSITIZE(14,LAZOOM_DRAW,1,0)
C      CALL XTS_SENSITIZE(4,LALOOP_1,1,0)
C      CALL XTS_SENSITIZE(4,LALOOP_2,1,0)
      CALL XTS_SENSITIZE(1,0,0,SEN_HOLD(2))
      GO TO 41

C request a hit and replot the "TFXFIT" display
 533  CALL XTS_ZOOM_TFXF('XTS_RFVIEW')
      CALL XTS_SELECT_HIT_TFXF
      CALL XTS_TFXF('XTS_RFVIEW')
      GO TO 41

C spare
 535  ENTRFV=ENTRFV_LOOP1
      GO TO 41

C spare
 537  ENTRFV=ENTRFV_LOOP2
      GO TO 41

541   CALL XTS_ZOOM_RFV('XTS_RFVIEW')
      CALL XTS_ZOOM_REDRAW('XTS_RFVIEW')
      CALL XTS_SENSITIZE(0,0,0,SEN_HOLD(2))
      GO TO 401

C go to next event without further processing
 931  XTS_ABORT=1
      XTS_ANSWER_IY='null'
      GO TO 989

C go to next pass
 932  XTS_ABORT=2
      CALL XTS_TRAK_INTERRUPT_THRESH('9')
      XTS_ANSWER_IY='null'
      GO TO 989


989   CONTINUE

      RETURN
      END

















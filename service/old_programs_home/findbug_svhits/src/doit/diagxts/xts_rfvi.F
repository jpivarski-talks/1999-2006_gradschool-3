*
* $Id: xts_rfvi.F,v 1.9 2000/12/19 23:49:45 dpp Exp $
*
* $Log: xts_rfvi.F,v $
* Revision 1.9  2000/12/19 23:49:45  dpp
*      -> plot size
*      -> new answer: other2
*
* Revision 1.8  2000/10/24 22:33:04  dpp
*      -> plot size
*
* Revision 1.7  2000/08/18 18:40:10  dpp
*      -> implement new choice: multi-hit
*
* Revision 1.6  2000/05/26 16:52:16  dpp
*      -> change some color codes
*
* Revision 1.5  2000/02/12 20:23:31  dpp
*      -> define new color for superlayer borders
*
* Revision 1.4  1999/10/12 15:10:30  dpp
* create button: "next pass"
*
* Revision 1.3  1999/01/29 15:34:42  dpp
* force update
*
* Revision 1.2  1998/05/08 14:13:50  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.4  1995/12/18 19:45:10  nk
* changes to TFXFIT hit selection, updates of TFXFIT graphics
*
* Revision 1.3  1995/09/25 19:23:10  zfiles
* definition of TFXF display
*
* Revision 1.2  1995/04/20  22:39:00  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 17/03/94  20.56.22  by  Dan Peterson
*CMZ :  6.00/07 31/08/93  14.19.57  by  Rob Kutschke
*CMZ :  6.00/01 04/03/93  17.02.19  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_RFVIEW_INIT(CALLER)
      IMPLICIT NONE
      SAVE
#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      INTEGER ADD_CHOICE_C
      INTEGER START_PULLDOWN_MENU_C
      INTEGER DEFINE_RGB_COLOR_C

      INTEGER IRED,IGRN,IBLU
      INTEGER CLRMAX/65534/
      CHARACTER*(*) CALLER
      INTEGER SEN_HOLD
      INTEGER IDUM

C-------executable code starts here-------------------------------------

#if defined(CLEO_XTSDEBUG)
      PRINT 1004,CALLER
1004  FORMAT(' XTS_RFVIEW_INIT: ENTER: CALLED FROM ',A20)
#endif

C initialize CDDRAW geometry as requested
      CALL XTS_GEOM('XTS_RFVIEW_INIT')

C SET LEFTSF=1 TO TELL XTS_PREP THAT IT IS POSSIBLY COMING FROM DUET

      LEFTSF=1

C GET DISPLAY NODE
C      PRINT 1001
1001  FORMAT(' XTS_RFVIEW_INIT: Display node: ',$)
C      READ 1002, DSPNOD
C1002  FORMAT(A)
C      CALL STR$UPCASE(DSPNOD,DSPNOD)
C      PRINT 1003
1003  FORMAT(' XTS_RFVIEW_INIT: NO! force local display node')

      DISPLAY='local'
      BANNER='YO MAMA'
c 12/1/00      MAXSIZ=270.
c 12/1/00      MAXSIZ=310.
      MAXSIZ=295.

C XTS_PREP  CALLS PLTOPN_C

      CALL XTS_PREP('XTS_RFVIEW_INIT')

C CREATE THE CHOICES STRUCTURE

#if defined(CLEO_XTSDEBUG)
      PRINT 1009
1009  FORMAT(' XTS_RFVIEW_INIT: will create choice structure')
#endif
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      CALL XTS_DEF_CHOIC(0,' ',IDUM,0,0)
      CALL XTS_DEF_CHOIC(1,'ZOOM_DRAW' ,LAZOOM_DRAW ,0,0,1,1)
      CALL XTS_DEF_CHOIC(1,'ZOOM'      ,LAZOOM      ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'FORWARD'   ,LAFORWARD   ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'BACKUP'    ,LABACKUP    ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'FULLSIZE'  ,LAFULLSIZE  ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'REDRAW'    ,LAREDRAW    ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'NEW DRAW'  ,LANEW_DRAW  ,1,LAZOOM_DRAW,0,1)
      CALL XTS_DEF_CHOIC(1,'INFO'      ,LAINFO      ,0,0,1,1)
      CALL XTS_DEF_CHOIC(1,'ID/TDC/ADC',LAID_TDC_ADC,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'multhit'   ,LAID_MULTIH ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'IC/CCC'    ,LAIC_CCC    ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'DISTANCE'  ,LADISTANCE  ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'WIR/DUET'  ,LAWIR_DUET  ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'INUCHN'    ,LAINUCHN    ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'ISTFOR'    ,LAISTFOR    ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'DSFDCH'    ,LADSFDCH    ,1,LAINFO     ,0,1) 
c      CALL XTS_DEF_CHOIC(1,'DSFDMP'    ,LADSFDMP    ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'COLOR?'    ,LACOLOR     ,1,LAINFO     ,0,1) 
      CALL XTS_DEF_CHOIC(1,'LOOP_1'    ,LALOOP_1    ,1,LAINFO     ,0,0) 
      CALL XTS_DEF_CHOIC(1,'LOOP_2'    ,LALOOP_2    ,1,LAINFO     ,0,0) 
      CALL XTS_DEF_CHOIC(1,'SKIP'      ,LASKIP      ,0,0,1,1)
      CALL XTS_DEF_CHOIC(1,'NEXT EVENT',LANEXT_EVENT,1,LASKIP     ,0,1)
      CALL XTS_DEF_CHOIC(1,'NEXT PASS' ,LANEXT_PASS ,1,LASKIP     ,0,1)
      CALL XTS_DEF_CHOIC(1,'ANSWER'    ,LAANSWER    ,0,0,1,1)
      CALL XTS_DEF_CHOIC(1,'YES'       ,LAYES       ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'NO'        ,LANO        ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'ALWAYS'    ,LAALWAYS    ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'KILL'      ,LAKILL      ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'MORE'      ,LAMORE      ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'OTHER'     ,LAOTHER     ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'OTHER2'    ,LAOTHER2    ,1,LAANSWER   ,0,1) 
      CALL XTS_DEF_CHOIC(1,'<CR>'      ,LACR        ,0,0,0,1)
      CALL XTS_DEF_CHOIC(1,'ZHIT'      ,LAZHIT      ,0,0,0,0)
      CALL XTS_DEF_CHOIC(1,'RESI'      ,LARESI      ,0,0,0,0)
      CALL XTS_SENSITIZE(1,0,0,SEN_HOLD)

C DEFINE THE COLORS

#if defined(CLEO_XTSDEBUG)
      PRINT 1008
1008  FORMAT(' XTS_RFVIEW_INIT: will define the colors')
#endif

C CLZERO...BACKGROUND OF THE PICTURE
        IRED=0.00*CLRMAX
        IGRN=0.00*CLRMAX
        IBLU=0.22*CLRMAX
                  CLZERO=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
#if defined(CLEO_XTSDEBUG)
                  print 1101,clzero
 1101             format(' XTS_RFVIEW_INIT: clzero=',I20)
#endif
C CLBORD...CHAMBER BORDERS
        IRED=0.99*CLRMAX
        IGRN=0.57*CLRMAX
        IBLU=0.00*CLRMAX
                  CLBORD=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
#if defined(CLEO_XTSDEBUG)
                  print 1102,clbord
 1102             format(' XTS_RFVIEW_INIT: clbord=',I20)
#endif
C CLSUBB...CHAMBER SUB-BORDERS
        IRED=0.49*CLRMAX
        IGRN=0.28*CLRMAX
        IBLU=0.00*CLRMAX
                  CLSUBB=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLLET1...THIS IS WHITE FOR THE DARK COLORED HITS
        IRED=0.99*CLRMAX
        IGRN=0.99*CLRMAX
        IBLU=0.99*CLRMAX
                  CLLET1=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLLET2...THIS IS BLACK FOR THE HITS ON CHAINS
        IRED=0.00*CLRMAX
        IGRN=0.00*CLRMAX
        IBLU=0.00*CLRMAX
                  CLLET2=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLNORM...NORMAL GOOD HITS THAT ARE AVAILABLE FOR NEW CHAINS
        IRED=0.00*CLRMAX
C        IGRN=0.72*CLRMAX
        IGRN=0.65*CLRMAX
        IBLU=0.00*CLRMAX
                  CLNORM=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLSTRO...STEREO
        IRED=0.00*CLRMAX
C        IGRN=0.70*CLRMAX
        IGRN=0.62*CLRMAX
        IBLU=0.20*CLRMAX
                  CLSTRO=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLCHAN...NORMAL HITS THAT ARE IN THE CURRENT DISPLAYED CHAIN
        IRED=0.99*CLRMAX
        IGRN=0.99*CLRMAX
        IBLU=0.20*CLRMAX
                  CLCHAN=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLUSED...USED IN PREVIOUS CHAINS, NOT AVAILABLE
        IRED=0.94*CLRMAX
        IGRN=0.28*CLRMAX
        IBLU=0.94*CLRMAX
                  CLUSED=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLDUSE...ALREADY USED BY DUET
        IRED=0.69*CLRMAX
        IGRN=0.00*CLRMAX
        IBLU=0.64*CLRMAX
                  CLDUSE=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLDUCH...ALREADY USED BY DUET BUT ASSIGNED TO THE CURRENT CHAIN
        IRED=0.85*CLRMAX
        IGRN=0.70*CLRMAX
        IBLU=0.08*CLRMAX
                  CLDUCH=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLDADJ...ADJECENT TO A HIT USED BY DUET
        IRED=0.59*CLRMAX
        IGRN=0.80*CLRMAX
        IBLU=0.16*CLRMAX
                  CLDADJ=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLHITM...HIGH TIME HITS
        IRED=0.20*CLRMAX
        IGRN=0.00*CLRMAX
        IBLU=0.40*CLRMAX
                  CLHITM=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLHADC...HIGH ADC (MEANING LOW CHARGE)
        IRED=0.00*CLRMAX
        IGRN=0.20*CLRMAX
        IBLU=0.40*CLRMAX
                  CLHADC=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)
C CLDUMB..................WHITE
        IRED=0.99*CLRMAX
        IGRN=0.99*CLRMAX
        IBLU=0.99*CLRMAX
                  CLDUMB=DEFINE_RGB_COLOR_C(IRED,IGRN,IBLU)

      CALL XTS_BORDR('XTS_RFVIEW_INIT')

#if defined(CLEO_XTSDEBUG)
      PRINT 1006
1006  FORMAT(' XTS_RFVIEW_INIT: EXIT')
#endif

      RETURN

C THIS STUFF IS DONE AFTER THE MENU IS POSTED

      ENTRY XTS_RFVIEW_INIT_MORE(CALLER)
#if defined(CLEO_SFDIAG)
C SFCRATE GET THE CELL-ID TO CRATE INFO
#if defined(CLEO_XTSDEBUG)
      PRINT 1014,CALLER
1014  FORMAT(' XTS_RFVIEW_INIT_MORE: ENTER: CALLED FROM ',A20)
#endif
      PRINT 1015
1015  FORMAT(' XTS_RFVIEW_INIT_MORE: will call SFCRATE')
      CALL SFCRATE
#endif

#if defined(CLEO_XTSDEBUG)
      PRINT 1016
1016  FORMAT(' XTS_RFVIEW_INIT_MORE: EXIT')
#endif

      RETURN
      END
















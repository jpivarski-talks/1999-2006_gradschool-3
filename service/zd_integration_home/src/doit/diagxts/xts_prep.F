*
* $Id: xts_prep.F,v 1.5 2002/09/05 19:08:33 dpp Exp $
*
* $Log: xts_prep.F,v $
* Revision 1.5  2002/09/05 19:08:33  dpp
*      -> change size of plots scaled to detector size
*      -> diagnostics
*      -> ZOOMU for sz plot,for now so sz plot size can be defined in xts_prep
*
* Revision 1.4  2000/10/02 20:54:25  dpp
*      -> vertical scale of TFXF plots
*
* Revision 1.3  2000/05/04 21:31:26  dpp
* remove tfindpar.inc
*
* Revision 1.2  1999/01/29 15:34:41  dpp
* force update
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.3  1995/12/18 19:44:59  nk
* changes to TFXFIT hit selection, updates of TFXFIT graphics
*
* Revision 1.2  1995/09/25 19:21:14  zfiles
* compatibility with zooming TFXF display
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.17.05  by  Rob Kutschke
*CMZ :  6.00/01 29/01/92  09.49.41  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.39  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_PREP(CALLER)
C PREPARE XTS FOR SF; BUT IT IS UNKNOWN HOW GKS FROM
C   DUET WILL INTERACT WITH MOHANS XTS
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

C note: WKID,TITLE, ARE PASSED THROUGH COMMON
C NOTE:  WKID   MUST BE SET   =0   IN XTS_RFVI


      INTEGER I
      REAL XSIZE,YSIZE,XORG,YORG,RADSIZ
      CHARACTER*20 CALLER
      REAL    TMPSIZ
      INTEGER ITMSIZ

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,WKID,CALLER
1001  FORMAT(' XTS_PREP: ENTER: WKID='I2,
     1       '  CALLED FROM...',A20)
#endif

      IF(WKID.EQ.0)THEN

        CALL XTS_TRAN(XSIZE,YSIZE,XORG,YORG,RADSIZ,'XTS_PREP')

#if defined(CLEO_XTSDEBUG)
        PRINT 2002,DISPLAY,BANNER,MAXSIZ,XSIZE,YSIZE,XORG,YORG
 2002   FORMAT(' XTS_PREP: CALLING PLTOPN_C: DISPLAY=',A20/
     1       '                             BANNER =',A20/
     2       '           MAXSIZ=',F7.2,' XSIZE=',F7.3,
     3       ' YSIZE=',F7.3/
     4       '           XORG=',F7.3,' YORG=',F7.3)
#endif

        CALL PLTOPN_C('local',BANNER,MAXSIZ,XSIZE,YSIZE,XORG,YORG)
C       CALL PLTOPN_C(DISPLAY,BANNER,MAXSIZ,XSIZE,YSIZE,XORG,YORG)

        WKID=1
        DO 125 I=1,1000
          ZOOMIT(1,I)=0
          ZOOMIT(2,I)=0
          ZOOMIT(3,I)=0
 125    CONTINUE
          
        ZOOMU_RPHI=1
        ZOOMU_TFXF(1)=2
        ZOOMU_TFXF(2)=3
        ZOOMU_SZ=4
        
C 2,4 was 1.2,-.1
        ZOOMU=ZOOMU_TFXF(1)
        ZOOMIT(1,ZOOMU)=ZOOMU
        ZOOMXY(1,ZOOMU)= .05
        ZOOMXY(2,ZOOMU)= 1.25*RADSIZ
        ZOOMXY(3,ZOOMU)=-.025
        ZOOMXY(4,ZOOMU)=-0.1*RADSIZ
        
        ZOOMU=ZOOMU_TFXF(2)
        ZOOMIT(1,ZOOMU)=ZOOMU
        ZOOMXY(1,ZOOMU)= .20
        ZOOMXY(2,ZOOMU)=1.25*RADSIZ
        ZOOMXY(3,ZOOMU)=-.10
        ZOOMXY(4,ZOOMU)=-0.1*RADSIZ
        
        ZOOMU=ZOOMU_RPHI
        ZOOMIT(1,ZOOMU)=ZOOMU
        ZOOMXY(1,ZOOMU)=XSIZE
        ZOOMXY(2,ZOOMU)=YSIZE
        ZOOMXY(3,ZOOMU)=XORG
        ZOOMXY(4,ZOOMU)=YORG

        ZOOMU=ZOOMU_SZ
        ZOOMIT(1,ZOOMU)=ZOOMU
        ITMSIZ=(RADSIZ*10.)+1.99999
        TMPSIZ=0.1*ITMSIZ
        ZOOMXY(1,ZOOMU)= 1.4*TMPSIZ
        ZOOMXY(2,ZOOMU)= 2.8*TMPSIZ
        ZOOMXY(3,ZOOMU)=-0.2*TMPSIZ
        ZOOMXY(4,ZOOMU)=-1.4*TMPSIZ

c YSIZE

c YORG
        
        
      ELSE
#if defined(CLEO_XTSDEBUG)
        PRINT 3001,WKID
 3001   FORMAT(' XTS_PREP:',
     1       ' PLOTTING APPEARS TO BE PREVIOUSLY OPEN, WKID=',I3)
#endif
      ENDIF
      
      LEFTSF=0
      CALL XTS_TRK0
      
#if defined(CLEO_XTSDEBUG)
      PRINT 2011,LEFTSF
 2011 FORMAT(' XTS_PREP: EXIT:  LEFTSF=',I3)
#endif
      
      RETURN
      END
      












*
* $Id: sfinit.F,v 1.6 2001/11/20 00:05:20 dpp Exp $
*
* $Log: sfinit.F,v $
* Revision 1.6  2001/11/20 00:05:20  dpp
*      -> use meslog for all print statements
*
* Revision 1.5  1999/12/09 21:25:41  dpp
*  change some variable names
* add ZNDSSF; length of layer in sf format, for z escape
*
* Revision 1.4  1999/11/22 21:00:16  dpp
* allow 'STARTUP' as valid input
*
* Revision 1.3  1998/07/09 01:54:57  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/07/09 01:01:17  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.7  1997/02/25 21:53:00  dpp
* add "which" input argument and add code to handle SFSV layers
*
* Revision 1.6  1996/02/29 17:30:04  zfiles
* changes for level3: remove calc of time and ph cuts; not in SFTCUT
*
* Revision 1.5  1996/02/14 21:04:22  zfiles
* remove call to SFLWF; use SFLSFA etc
*
* Revision 1.4  1995/11/27 17:02:42  nk
* removed use of SVD94 compilation time switch
*
* Revision 1.3  1995/04/20 22:39:52  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.2  1994/12/06  17:38:18  zfiles
* Remove improper syntax in write statements for AIX from J. Urheim
*
* Revision 1.1.1.1  1994/10/08  01:00:43  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 12/05/94  11.05.58  by  Anders Ryd
*CMZ :  6.00/18 17/03/94  14.56.58  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.29  by  Dan Peterson
*-- Author :  DAN PETERSON
      SUBROUTINE SFINIT(SHOWCL,WHICHI)
C.......................................................................
C.
C. SFINIT - Initialization for SFIND
C.
C. COMMON    : /STEPCn/ and others
C. CALLS     :
C. CALLED    : DUET2,SFBRUN
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 27-Jul-88
C. LAST MOD  : 27-Jul-88
C.
C. Modification Log.
C.  8-July-98  Retrofitted for SF_DATA_ACCESS
C. 30-March-93 separated chain finding cuts into SFCUTS
C. 26-July-89 fill LAYSSF form LAYFIL
C. 21-June-89 set up super layers
C. 03-May-89  changed to run from CDGEOM
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "doit/sfseq/sffix.inc"
#include "seq/clutil/mesrep.inc"

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

      REAL PI,TWOPI
      PARAMETER (PI=3.14159265, TWOPI=6.28318530)

C ITMP......TEMPORARY INDEX, WHICH CAN ALLOW DELETING A LAYER FROM SF
C ISF.......SF INDEX
C ICD.......CD INDEX
C ISSF......SSF INDEX
C IWIR......WIRE INDEX
C I3,I4.....INDEX
C I3HLD,I4HLD...INDEX HOLD
C INSFTM....TEMP VALUE TO CALCULATE INSFSF
C MHITTM....TEMP VALUE TO PRINT MHITSF
C NCELLS....LIMIT OF THE NUMBER OF CELLS IN A LAYER
C DOEAST....LOGICAL to define when east addresses are to be created
C FI0.......PHI OF FIRST WIRE IN LAYER
C FIOTHR....PHI OF FIRST WIRE IN OTHER LAYER
C RADIUS....RADIUS OF LAYER
C ZLENGT....LENGTH IN "Z" OF A LAYER
C STEREO....STEREO DIFFERENCE OF LAYER
C NSUBLR....NUMBER OF SUB LAYERS
C NTEMP1....TEMPORARY STORAGE OF NUMBER FOUND
C DIDUSE....FLAG SHOWING WHEN A WIRE HAS BEEN ASSIGNED TO SSF LAYER
C ADRTMP....ADDRESS IN MAPSSF
C ADRHLD....ADDRESS OF CURRENT CLOSE WIRE IN SF
C NWIRSM....TOTAL NUMBER OF WIRES IN SSF LAYER
C FDIFF.....PHI DIFFERENCE FROM A WIRE FROM FIRST WIRE
C FDIFM.....MINIMUM OF FDIFF
C RSUM......SUM OF RADII OF SF LAYERS IN SSF LAYER
C ZESUM.....SUM OF ZEND  OF SF LAYERS IN SSF LAYER
C DIFSUM....SUM OF DIFCD OF SF LAYERS IN SSF LAYER
C CWIDTH....RESTRICTED CELL WIDTH, IN METERS
C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL
C WHICHI....INPUT WHICH SET OF CUTS TO USE
C LENSW.....LENTH OF CHARACTER STRING = WHICH
C WHICHP....WHICH SET OF CUTS PASSED TO SFCUTS
C LENSWP....LENTH OF CHARACTER STRING = WHICHP
C NLEGAL....NUMBER OF LEGAL VALUES OF WHICHI
C ILEGAL....INDEX 
C WHICHL....LEGAL VALUES OF INPUT WHICH
      INTEGER ITMP,ISF,ICD,ISSF,IWIR,I3,I4,I3HLD,I4HLD
      INTEGER INSFTM,MHITTM,NCELLS
      LOGICAL DOEAST
      INTEGER NSUBLR
      INTEGER DIDUSE(400,5)
      INTEGER ADRTMP,ADRHLD
      INTEGER NWIRSM
      REAL FI0,RADIUS,ZLENGT,STEREO
      REAL FIOTHR,FDIFF,FDIFM,RSUM,ZESUM,DIFSUM,CWIDTH
      CHARACTER*(*) SHOWCL,WHICHI
      CHARACTER*8 WHICHP
      INTEGER LENSC,LENSW,LENSWP,IOS

      INTEGER MLEGAL,DEFALT,ILEGAL
      PARAMETER (MLEGAL=5,DEFALT=1)
      CHARACTER*8 WHICHT,PAD,WHICHL(MLEGAL)
      DATA PAD   /'        '/
      DATA WHICHL/'SHORT','CLEAN','TIMING','SFBWAL','STARTUP'/




C************** executable code starts here**************************

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC ACKNOWLEDGE                                                CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C this changes WHICH=(nonvalid input or ' ') to the default
C and stores the new value in WHICHP to be passed to other routines
      WHICHP=WHICHL(DEFALT)
      WHICHT=WHICHI//PAD
      LENSW=LEN(WHICHT)

      ILEGAL=0
 1    ILEGAL=ILEGAL+1
      IF(ILEGAL.LE.MLEGAL)THEN
         LENSWP=LEN(WHICHL(ILEGAL))
         WRITE(CHMESS,9901,IOSTAT=IOS)
     2        ILEGAL,WHICHL(ILEGAL),LENSWP,WHICHL(ILEGAL)(1:LENSWP),
     2        WHICHT,LENSW, WHICHT(1:LENSW )
 9901    FORMAT(' try ',I2,' "',A8,'" len=',I2,' trunc="',A8,'"',
     2        ' input "',A8,'" len=',I2,' trunc="',A8,'"')

         CALL MESLOG('SFINIT',1,MSNOTI)
         IF(WHICHT.EQ.WHICHL(ILEGAL))THEN
            WHICHP=WHICHT
         ELSE
            GO TO 1
         ENDIF
      ENDIF

      LENSC=LEN(SHOWCL)
      IF(LENSC.GT.10)LENSC=10
      LENSW=LEN(WHICHI)
      IF(LENSW.GT.8)LENSW=8
      LENSWP=LEN(WHICHP)
      IF(LENSWP.GT.8)LENSWP=8

      WRITE(CHMESS,1101,IOSTAT=IOS)SHOWCL(1:LENSC),WHICHI(1:LENSW),
     2                             WHICHP(1:LENSWP)
1101  FORMAT(' entry: called from ',A10, ' specify"',A8,'"',
     2                                 '   pass "',A8,'"')
      CALL MESLOG('SFINIT',1,MSNOTI)
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC SFDEFN defines the SF and SSF layers                       CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      CALL SFDEFN('SFINIT',WHICHP)

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC phi fix for CD layers next to the cathodes                 CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      DO 5 ICD=1,KLYRCD
5     FFIXSF(ICD)=0.

#if !defined(CLEO_CLEO3)
C CORRECTIONS AT 1.5 TESLA
      IF(IVDET.NE.0)THEN
        FFIXSF(ILDVCD(1,IVDET))= .0008/RCD(ILDVCD(1,IVDET))
        FFIXSF(ILDVCD(2,IVDET))=-.0012/RCD(ILDVCD(2,IVDET))
      ENDIF
      IF(IDRFT.NE.0)THEN
        FFIXSF(ILDVCD(1,IDRFT))= .0015/RCD(ILDVCD(1,IDRFT))
        FFIXSF(ILDVCD(2,IDRFT))=-.0015/RCD(ILDVCD(2,IDRFT))
      ENDIF
#endif

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC do standard SF layers first                                CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      WRITE(CHMESS,1104,IOSTAT=IOS)SHOWCL
1104  FORMAT(' starting standard SF layers')
      CALL MESLOG('SFINIT',1,MSINFO)

      INSFTM=0
      DO 79 ISF=1,NLAYSF

C cell ID offset for SF layers in SF and CD cell numbering systems
      IF(INSFTM.GE.MHITSF)THEN
          MHITTM=MHITSF
          WRITE(CHMESS,1102,IOSTAT=IOS)INSFTM,MHITTM
1102      FORMAT(' *** index excedes MHITSF:',I7,I7)
          CALL MESLOG('SFINIT',1,MSERRO)
      ENDIF

C set up the pointer to the first address of the SF layer
C and the pointers from the SF address to the SF layer and wire(ref0)
      INSFSF(ISF)=INSFTM+1

C get the number of cells in the layer for CD and non-CD layers
      ICD=CDLASF(ISF)
      IF(ICD.LT.0)THEN
            NCELLS=SFSVCL(-ICD)
            DOEAST=.TRUE.
      ELSE
            NCELLS=NWIRCD(ICD)
            DOEAST=LQDVCD(ICD)
      ENDIF

C fill the layer and wire numbers for SF addresses
      DO 63 IWIR=1,NCELLS
      SFLSFA(INSFTM+IWIR)=ISF
63    SFWSFA(INSFTM+IWIR)=IWIR-1
      INSFTM=INSFTM+NCELLS

C new SF numbering system DOES allow for VD EAST and SILICON EAST
C SFLWF returned wire number+NWIR  for EAST hit
      IF(DOEAST)THEN
          DO 65 IWIR=1,NCELLS
          SFLSFA(INSFTM+IWIR)=ISF
65        SFWSFA(INSFTM+IWIR)=IWIR-1+NCELLS
          INSFTM=INSFTM+NCELLS
      ENDIF

C get index, angle and radius
C  for external data structure (non-SF) for CD and non-CD layers
      IF(ICD.LT.0)THEN
            INCDSF(ISF)=SFSVIN(-ICD,1)
            FI0=SFSVF0(-ICD)
            RADIUS=SFSVRD(-ICD)
      ELSE
            INCDSF(ISF)=INDXCD(ICD)
            FI0=PHIFCD(ICD)+FFIXSF(ICD)
            RADIUS=RCD(ICD)
      ENDIF

C this is the old SFINIT information
C FI0; thats ,like -.262 for DR-II layer 1
71    IF(FI0.LE. PI)GO TO 73
      FI0=FI0-TWOPI
      GO TO 71
73    IF(FI0.GT.-PI)GO TO 75
      FI0=FI0+TWOPI
      GO TO 73
75    CONTINUE

C WIRE OFFSET *1024
      OFFSF(ISF)=FI0/TWOPI*NCELLS*MULST+.5
C RADIUS, CM *1024
      RADSF(ISF)=RADIUS*100.*MULST+.5
C NUMBER OF WIRES IN SF LAYER
      NWRSF(ISF)=NCELLS

C end loop over SF layers
79    CONTINUE

      MHITTM=MHITSF
      WRITE(CHMESS,1103,IOSTAT=IOS)INSFTM,MHITTM
1103  FORMAT(' have used ',I7,' locations, out of MHITSF=',I7)
      CALL MESLOG('SFINIT',1,MSINFO)


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC  DO SSF LAYERS NEXT                                        CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      WRITE(CHMESS,1105,IOSTAT=IOS)SHOWCL
1105  FORMAT(' starting SSF layers')
      CALL MESLOG('SFINIT',1,MSINFO)

      CALL VZERO(LAYSF,MSFSSF*MLAYSF)

C LOOP OVER THE "SUPER SF", IE SSF LAYERS
      LADSSF(1)=1
      DO 179 ISSF=1,NLASSF
      IF(ISSF.EQ.1)GO TO 105
      LADSSF(ISSF)=MADSSF(ISSF-1)+1
105   MADSSF(ISSF)=0
      NSUBLR=0
      NWIRSM=0
      RSUM=0.
      ZESUM=0.
      DIFSUM=0.

C find the contributing REGULAR SF layers
      DO 111 ISF=1,NLAYSF
      IF(LAYSSF(ISF).NE.ISSF)GO TO 111
      IF(NSUBLR.EQ.MSFSSF)GO TO 111
      NSUBLR=NSUBLR+1
      LAYSF(NSUBLR,ISSF)=ISF

      ICD=CDLASF(ISF)
      IF(ICD.LT.0)THEN
            NCELLS=SFSVCL(-ICD)
            FI0=SFSVF0(-ICD)
            RADIUS=SFSVRD(-ICD)
            ZLENGT=9.
            STEREO=0.
      ELSE
            NCELLS=NWIRCD(ICD)
            FI0=PHIFCD(ICD)+FFIXSF(ICD)
            RADIUS=RCD(ICD)
            ZLENGT=ZENDCD(ICD)
c temp, because DIFSCD is not zeroed in CDPABR nor CDVABR, 3/23/95
            if(icd.ge.ILDVCD(1,IDRFT))then
              STEREO=DIFSCD(ICD)
            else
              STEREO=0.
            endif
      ENDIF

      NWIRSM=NWIRSM+NCELLS
      RSUM=RSUM    +RADIUS
      ZESUM=ZESUM  +ZLENGT
      DIFSUM=DIFSUM+STEREO
      
C end loop over SF layers
111   CONTINUE

      IF(NSUBLR.EQ.0)GO TO 179
      DO 113 I3=1,NSUBLR
      ISF=LAYSF(I3,ISSF)
      ICD=CDLASF(ISF)
      IF(ICD.LT.0)THEN
            NCELLS=SFSVCL(-ICD)
      ELSE
            NCELLS=NWIRCD(ICD)
      ENDIF
      DO 113 I4=1,NCELLS
113   DIDUSE(I4,I3)=0

C USE THE FIRST WIRE OF THE FIRST REGULAR SF LAYER AS
C THE FIRST WIRE OF THE SUPER SF LAYER
      ADRTMP=LADSSF(ISSF)
      ISF=LAYSF(1,ISF)
      MAPSSF(ADRTMP)=INSFSF(ISF)
      DIDUSE(1,1)=1
      WIRSSF(INSFSF(ISF))=0

C ORDER THE WIRES BY INCREASING PHI
      ICD=CDLASF(ISF)
      IF(ICD.LT.0)THEN
            FI0=SFSVF0(-ICD)
      ELSE
            FI0=PHIFCD(ICD)+FFIXSF(ICD)
      ENDIF

C COUNT  THE IDENTIFIED SUPER SF WIRES
      DO 129 IWIR=2,NWIRSM
      FDIFM=99.
C LOOP OVER THE CONTRIBUTING SF LAYERS
      DO 123 I3=1,NSUBLR
      ISF=LAYSF(I3,ISSF)

      ICD=CDLASF(ISF)
      IF(ICD.LT.0)THEN
            NCELLS=SFSVCL(-ICD)
            FIOTHR=SFSVF0(-ICD)
      ELSE
            NCELLS=NWIRCD(ICD)
            FIOTHR=PHIFCD(ICD)+FFIXSF(ICD)
      ENDIF

C LOOP OVER THE WIRES IN THE LAYER
      DO 121 I4=1,NCELLS
      IF(DIDUSE(I4,I3).NE.0)GO TO 121
      FDIFF=FIOTHR +TWOPI*(I4-1)/NCELLS-FI0
115   IF(FDIFF.LT.TWOPI)GO TO 117
      FDIFF=FDIFF-TWOPI
      GO TO 115
117   IF(FDIFF.GE.0.)GO TO 119
      FDIFF=FDIFF+TWOPI
      GO TO 117
119   IF(FDIFF.GT.FDIFM)GO TO 121
C THIS IS THE PHI DIFFERENCE AND ADDRESS OF THE CLOSEST HIT
      FDIFM=FDIFF
      ADRHLD=INSFSF(ISF)+(I4-1)
      I3HLD=I3
      I4HLD=I4
121   CONTINUE
123   CONTINUE
      ADRTMP=ADRTMP+1
      MAPSSF(ADRTMP)=ADRHLD
      DIDUSE(I4HLD,I3HLD)=1
      WIRSSF(ADRHLD)=(IWIR-1)
129   CONTINUE
      MADSSF(ISSF)=ADRTMP

C THIS IS THE OLD SFINIT INFORMATION
C FI0; THATS LIKE -.262 FOR DR LAYER 1
131   IF(FI0.LE. PI)GO TO 133
      FI0=FI0-TWOPI
      GO TO 131
133   IF(FI0.GT.-PI)GO TO 135
      FI0=FI0+TWOPI
      GO TO 133
C WIRE OFFSET *1024
135   OFFSSF(ISSF)=FI0/TWOPI*NWIRSM*MULST+.5
C RADIUS, CM *1024
      RADSSF(ISSF)=RSUM/NSUBLR*100.*MULST+.5
C NUMBER OF WIRES IN SSF LAYER
      NWRSSF(ISSF)=NWIRSM
C RADIUS PER NUMBER OF WIRES IN SSF LAYER
      RPWSSF(ISSF)=RSUM/NSUBLR*CELSSF*MULST/NWIRSM
C STEREO ANGLE: SHIFT IN CELLS PER Z (USE LAST LAYER IN GROUP FOR NOW)
      TANSSF(ISSF)=DIFSUM/(2.*ZESUM)*MULST
C Z END, OR EXIT, METERS *1024
      ZNDSSF(ISSF)=ZESUM/NSUBLR*MULST

C end of loop over super layers
179   CONTINUE

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC SFCUTS sets the cutting values used inside of SFCHAN       CCCCCCCC
CCCCCCCC        needs values set in SFDEFN (eg NLASSF..)            CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      CALL SFCUTS('SFINIT',WHICHP)

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC SFDATA to zero the base address in case SFDATA is called   CCCCCCCC
CCCCCCCC        by diagnostic routines when SFCHAN is not           CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      sf_data_input_flag=0
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC EXIT                                                       CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      WRITE(CHMESS,1109,IOSTAT=IOS)NLAYSF,NLASSF
1109  FORMAT(' exit: NLAYSF=',I4,',   NLASSF=',I4)
      CALL MESLOG('SFINIT',1,MSNOTI)

      RETURN
      END













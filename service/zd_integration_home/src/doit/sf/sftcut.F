*
* $Id: sftcut.F,v 1.13 2003/03/05 18:34:22 dpp Exp $
*
* $Log: sftcut.F,v $
* Revision 1.13  2003/03/05 18:34:22  dpp
*      -> define new cut set for "TPC1"
*
* Revision 1.12  2001/11/20 00:05:55  dpp
*      -> use meslog for all print statement
*
* Revision 1.11  2000/10/02 20:50:54  dpp
*      -> create new cut definition "2NDPASS"
*         now leave in the pulse height cut in the 2nd pass
*
* Revision 1.10  2000/08/18 19:11:17  dpp
*      -> change the extra drift distance window to use parameter bit:
*         EXB_SF_DATA_CUTS rather than compile switch: STARTUPTIMECUTS
*
* Revision 1.9  2000/05/26 16:53:30  dpp
*      -> define pulse height cuts for "startup" and "clean"
*
* Revision 1.8  2000/03/01 22:59:39  dpp
*      -> replace compile switch STARTUPTIMECUTS
*         with NEXPT bit: EXB_SF_DATA_CUTS
*
* Revision 1.7  2000/02/18 21:18:38  dpp
*      -> now uses compile switch STARTUPTIMECUTS
*
* Revision 1.6  2000/02/12 20:33:25  dpp
*      -> fill cuts for raw TDC, ADC, time, distance, used in SFIND and SFHITT
*
* Revision 1.5  1999/11/19 22:52:42  dpp
* add time cuts and multiplier
* "startup" option
*
* Revision 1.4  1999/10/12 15:57:48  lyon
* SFCUTS and SFTCUTS messages only come out once
*
* Revision 1.3  1999/05/28 03:05:05  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1998/07/09 01:05:54  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.4  1997/02/25 21:59:03  dpp
* use SVX layers in addition to CD layers
*
* Revision 1.3  1996/04/24 03:28:37  zfiles
* Bug fix.
*
* Revision 1.2  1996/04/23 19:34:24  dpp
* use run number (from CLGHED) to select VD TDC window for SFBWAL
*
* Revision 1.1  1996/02/29 17:33:29  zfiles
* time and pulse height cuts that used to be in SFINIT
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :  DAN PETERSON
      SUBROUTINE SFTCUT(SHOWCL,WHICH)
C.......................................................................
C.
C. SFTCUT - Initialization for SFIND Time and pulse height CUTs 
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 16-Feb-96
C. LAST MOD  : 16-Feb-96
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfctlcde.inc"

#include "doit/sfseq/sfpar.inc"
#include "seq/clutil/mesrep.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"
#include "doit/duseq/runev.inc"

      REAL PI,TWOPI
      PARAMETER (PI=3.14159265, TWOPI=6.28318530)

C ISF.............SF INDEX
C ICD.............CD INDEX
C ISSF............SSF INDEX
C SHOWCL..........SHOW WHERE CALLED FROM
C WHICH...........INPUT WHICH SET OF CUTS TO USE
C LENSC...........LENGTH OF CHARATER STRING =SHOWCL
C L_SF_DATA_CUT...logical, =.true. if SF_DATA_CUT event control bit is set

      INTEGER ISF,ICD,ISSF,ISFSSF
      REAL CWIDTH
      CHARACTER*(*) SHOWCL,WHICH
      INTEGER LENSC,LENSW,IOS
      INTEGER NRUNL
      REAL RDUM
      INTEGER COUNT,COUNTM
      LOGICAL LFIRST
      LOGICAL L_SF_DATA_CUT

      DATA COUNT,COUNTM/0,10/
      DATA LFIRST/.TRUE./

C************** executable code starts here**************************

      CALL EVENT_CONTROL_BITS(EXB_SF_DATA_CUT,L_SF_DATA_CUT)

      NRUNL = current_run

      IF(COUNT.LT.COUNTM)THEN
         COUNT=COUNT+1
      ELSE
         LFIRST=.FALSE.
      ENDIF

      IF ( LFIRST ) THEN
         LENSC=LEN(SHOWCL)
         IF(LENSC.GT.10)LENSC=10
         LENSW=LEN(WHICH)
         IF(LENSW.GT.8)LENSW=8

C         PRINT 1103,SHOWCL(1:LENSC),WHICH(1:LENSW)
C 1103    FORMAT(' SFTCUT: entry: called from',A10,' specify  "',A8,'"')

         WRITE(CHMESS,1101,IOSTAT=IOS)SHOWCL(1:LENSC),WHICH(1:LENSW)
 1101    FORMAT(' entry: called from',A10,' specify  "',A8,'"')
        CALL MESLOG('SFTCUT',1,MSNOTI)
      ENDIF

C-----------------------------------------------------------------------
C loop over SF layers
C-----------------------------------------------------------------------

      DO 79 ISF=1,NLAYSF
      ICD=CDLASF(ISF)

C-----------------------------------------------------------------------
C default cuts
C-----------------------------------------------------------------------

      RAW_TDC_L(ISF)= -9999999
      RAW_TDC_H(ISF)=  9999999

      TIME_PS_L(ISF)= -999999999.
      TIME_PS_H(ISF)=  999999999.

      PULS_HT_L(ISF)= -999999999.
      PULS_HT_H(ISF)=  999999999.


C-----------------------------------------------------------------------
C     "CLEAN"  developed in MC for first SF pass for CLEO-III
C              now used as baseline; default 
C-----------------------------------------------------------------------
      IF(WHICH.EQ.'CLEAN')THEN

         IF(L_SF_DATA_CUT)THEN
            RAW_TDC_L(ISF)=   8000
            RAW_TDC_H(ISF)=  13000
         ENDIF
         PULS_HT_L(ISF)=      1.
         PULS_HT_H(ISF)=  999999999.

C-----------------------------------------------------------------------
C startup of CLEO3 with clean tracks but allowing longer gaps
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'STARTUP')THEN
         RAW_TDC_L(ISF)=   8000
         RAW_TDC_H(ISF)=  13000
         PULS_HT_L(ISF)=      1.
         PULS_HT_H(ISF)=  999999999.

C-----------------------------------------------------------------------
C  2nd pass evolved from SHORT
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'2NDPASS')THEN

         IF(L_SF_DATA_CUT)THEN
            RAW_TDC_L(ISF)=   8000
            RAW_TDC_H(ISF)=  13000
         ENDIF
         PULS_HT_L(ISF)=      1.
         PULS_HT_H(ISF)=  999999999.

C-----------------------------------------------------------------------
C  the traditional set of cuts used in DUET cleanup track finding
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'SHORT')THEN

         IF(L_SF_DATA_CUT)THEN
            RAW_TDC_L(ISF)=   8000
            RAW_TDC_H(ISF)=  13000
         ENDIF

C-----------------------------------------------------------------------
C for use with a TPC detector, first iteration
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'TPC1')THEN


C-----------------------------------------------------------------------
C   for bunch finding
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'TIMING')THEN


C-----------------------------------------------------------------------
C   for level3 beam wall tests
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'SFBWAL')THEN

      ENDIF

C-----------------------------------------------------------------------
C end loop over SF layers
C-----------------------------------------------------------------------
79    CONTINUE

C-----------------------------------------------------------------------
C now loop over SSF layers so drift cut can be used to select SFF hits
C-----------------------------------------------------------------------


      DO 179 ISSF=1,NLASSF
      ISFSSF=0

105   IF(ISFSSF.EQ.MSFSSF)GO TO 179
      ISFSSF=ISFSSF+1
      ISF=LAYSF(ISFSSF,ISSF)
      IF(ISF.EQ.0)GO TO 179

      ICD=CDLASF(ISF)

C 12/20/91 was 1.10 for 1991 compress
C 06/25/92 was 1.14 for tests since 12/20/91
      IF(ICD.GT.0)THEN
            CWIDTH=TWOPI*RCD(ICD)/NWRSSF(ISSF)/2.  *1.1
      ELSE
            CWIDTH=TWOPI*SFSVRD(-ICD)/NWRSSF(ISSF)/2.  *1.1
      ENDIF

C-----------------------------------------------------------------------
C extra drift distance window, in meters
C default
C-----------------------------------------------------------------------

      DRIFT_EXTRA_LO=-99.
      DRIFT_EXTRA_HI= 99.

C-----------------------------------------------------------------------
C     "CLEAN"  developed in MC for first SF pass for CLEO-III
C              now used as baseline; default 
C-----------------------------------------------------------------------
      IF(WHICH.EQ.'CLEAN')THEN
         IF(L_SF_DATA_CUT)THEN
            DRIFT_EXTRA_HI=0.001
         ELSE
            DRIFT_EXTRA_HI=99.
         ENDIF

C-----------------------------------------------------------------------
C startup of CLEO3 with clean tracks but allowing longer gaps
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'STARTUP')THEN
         DRIFT_EXTRA_HI= 99.

C-----------------------------------------------------------------------
C  the traditional set of cuts used in DUET cleanup track finding
C-----------------------------------------------------------------------
      ELSEIF(
     1        (WHICH.EQ.'SHORT').OR.
     2        (WHICH.EQ.'2NDPASS'))THEN
         IF(L_SF_DATA_CUT)THEN
            DRIFT_EXTRA_HI=0.001
         ELSE
            DRIFT_EXTRA_HI=99.
         ENDIF

C-----------------------------------------------------------------------
C   for bunch finding
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'TIMING')THEN

C-----------------------------------------------------------------------
C   for level3 beam wall tests
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'SFBWAL')THEN

      ENDIF

      DRIFT_D_L(ISF)=             DRIFT_EXTRA_LO *1000000.

      IF    (ISF.LE.MLSFSV)THEN
         DRIFT_D_H(ISF)=  999000.
      ELSE
         DRIFT_D_H(ISF)=(CWIDTH + DRIFT_EXTRA_HI)*1000000.
      ENDIF

C end of lopop over layers
      GO TO 105
C end of loop over super layers
179   CONTINUE


      IF (LFIRST) THEN
        LENSC=LEN(SHOWCL)
        WRITE(CHMESS,1109,IOSTAT=IOS)NLAYSF,NLASSF
 1109   FORMAT(' exit: NLAYSF=',I4,',   NLASSF=',I4)
        CALL MESLOG('SFTCUT',1,MSINFO)
      ENDIF

      RETURN
      END







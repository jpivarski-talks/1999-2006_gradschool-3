*
* $Id: sfind.F,v 1.17 2001/08/21 14:10:04 dpp Exp $
*
* $Log: sfind.F,v $
* Revision 1.17  2001/08/21 14:10:04  dpp
*      -> change condition for executing print statement
*
* Revision 1.16  2000/08/18 19:10:23  dpp
*      -> revise diagnostic breakpointswith sfeff
*
* Revision 1.15  2000/05/26 16:53:43  dpp
*     -> use pulse height in happy units instead of raw
*
* Revision 1.14  2000/02/12 20:32:58  dpp
*      -> use NEXPT (the doit running level flag) to select Doit or DoitFast
*      -> fix cuts on raw TDC, ADC, time, distance to values set in SFTCUT
*      -> more F77 modifications
*
* Revision 1.13  2000/01/24 20:00:18  dpp
*      -> cuts on raw TDC for STARTUP and DOITFAST
*         (((these should be put in SFCUTS)))
*      -> fix the reset of ISTFOR etc.
*      -> start F77 modifications
*
* Revision 1.12  1999/11/19 22:55:29  dpp
* oops; take out the printout
*
* Revision 1.11  1999/11/19 22:53:06  dpp
* remove sum cuts for cleo3
*
* Revision 1.10  1999/05/28 03:05:03  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.9  1999/04/27 19:37:48  dpp
* order chains by decreasing number of single hits for secondary operations
*
* Revision 1.8  1999/01/29 19:33:02  lyon
* removed extremely annoying question
*
* Revision 1.7  1999/01/29 14:52:09  dpp
* a few prints
*
* Revision 1.6  1998/07/21 14:19:00  lyon
* Protect against including cdscrtcd twice
*
* Revision 1.5  1998/07/09 13:01:44  lyon
* Added include cdscrtcd.inc
*
* Revision 1.4  1998/07/09 01:49:59  lyon
* Removed extraneous sf includes
*
* Revision 1.3  1998/07/09 01:46:23  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/05/08 14:14:10  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.12  1997/02/25 21:51:21  dpp
* call the new SFSVFM to load in the SVX sparcifier hits
* and create the clone chains for extending into west and east SVX
*
* Revision 1.11  1996/04/15 17:11:31  dpp
* argument list for SFFILL
*
* Revision 1.10  1996/04/03 16:11:45  dpp
* replace inward extension for beamwall
*
* Revision 1.9  1996/03/21 19:28:10  zfiles
* provide direct access to data, without SFDATA buffer, for beam wall use
*
* Revision 1.8  1996/03/20 20:21:26  zfiles
* Fixed typos
*
* Revision 1.7  1996/03/20 17:15:57  zfiles
* various changes for speed: veroing in SFIND, removed SFCDAD
*
* Revision 1.6  1996/02/29 17:28:34  zfiles
* changes for level3: switch to beam wall event finding with CNTLSF
*
* Revision 1.5  1996/01/25 16:35:20  zfiles
* changed a format statement
*
* Revision 1.4  1995/11/27 18:40:20  nk
*  put compilation time switch, SFDIAG, of calls to SFEFFn
*
* Revision 1.3  1995/11/27 17:01:55  nk
* allow analysis of cosmics using SFEFF
*
* Revision 1.2  1995/04/20  22:39:51  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:43  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 10/05/94  17.42.30  by  Anders Ryd
*CMZ :  6.00/20 22/03/94  14.40.22  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.28  by  Dan Peterson
*CMZ :  2.00/00 08/05/89  13.55.50  by  Dan Peterson
*-- Author : DAN PETERSON
      SUBROUTINE SFIND
C......................................................................
C.
C. SFIND - Create the chains and the merged chains
C.
C. COMMON    : /STEPCn/;IADRHW AND DWCD IN CDSCRTCD
C. CALLS     : VZERO  SFCHAN SFMERG DSF_RFVIEW DSF_RFVIEW2 CHVIE1 DSFDM0
C. CALLED    : DUFIND,SFEVNT
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 20-Jul-88
C. LAST MOD  : 26-Sept-88
C.
C. Modification Log.
C. 30-March-93 compatable with loop limits set up in SFCUTS
C. 17-OCT-91  SET INUCHN=-4 adjecent to DUET (had been -3; save as on DUET)
C.            can set back to INUCHN=0 for the second pass at SFCHAN
C. 22-June-89 set INUCHN=-1 for long time hits
C. 10-June-89 install OVERIDE option
C. 22-May-89 VZERO(ISTFOR,4*MHITSF) because INUCHN is 2 arrays;
C. 05-MAY-89 changed to run from ZFILES and CDGMBJ
C.           eliminated the link list { I1STWL, ILSTWL, INXTWL }
C.           use NAXLCD instaed of MLYRCF
C. 26-Sept-88 added layer pointer data structure
C.            changed to specifying initial layer range by parameter
C. 05-Aug-88 changed CALL VZERO(ISTFOR,"3"*MHITSF) to get INUCHN
C.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE

#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if needed)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "seq/clutil/mesrep.inc"

#ifndef CLEO_SFNOZFILES
#include "doit/duseq/cdscrtcd.inc"
#endif

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"


      CHARACTER*6  CRNAME
      PARAMETER    (CRNAME='SFIND')
      INTEGER      OVERIDE,ANALYZ,ACKMERG
      INTEGER      ABORT,IY,ASFEFF,YSFEFF
      INTEGER      ILSTSF,ILAY,ICHAIN,OCHAIN,COUNTC
      INTEGER      NCHANH,IEXTND,UCHAIN,ANOTHR,NXTSAV
      INTEGER      SFCERR,SFCDIR
      INTEGER      LYRSF,LYRCD,IH,IWR0 ,IWRP ,IWRM
      INTEGER                      IADR0,IADRP,IADRM
      LOGICAL      ALTRNT,SO_FAR_SO_GOOD
      INTEGER      IADCD,IHDUET
      INTEGER      TIME,PULSEH
      REAL         TIMER,DIST
      INTEGER      IOS
      INTEGER      NLEN10

#if defined(CLEO_SFDIAG)
      INTEGER INUTMP(MHITSF),DOUTMP(MHITSF),INETMP(MHITSF)
      INTEGER LINTMP(MCHAIN),MINTMP(MCHAIN)
      INTEGER ISFTMP(MHITSF),ISBTMP(MHITSF)
#endif

      LOGICAL LFIRST

      DATA ANALYZ/0/
      DATA OVERIDE/0/
      DATA ASFEFF/0/
      DATA ACKMERG/0/
      DATA LFIRST/.TRUE./


C********************************************************************
C executable code starts here
C********************************************************************

      NCHAIN=0
	
#if defined(CLEO_SFDIAG)
      CALL DSF_ABORT0
      PRINT 9901,CNTLSF
9901  FORMAT(' SFIND: CNTLSF=',I3)
#endif

C------------------------------------------------------------------
C load the SVX sparsifier bits
C SFSVFM sets SFSVVA=1 for valid data; 
C  this is used later to allow SFSV changes to SFIND
C set the readoutside for the silicon
C------------------------------------------------------------------

      IF(LSFSVF)THEN

#if defined(CLEO_SFDIAG)
         PRINT 9902
 9902    FORMAT('        WILL CALL SFSVFM')
#endif

         CALL SFSVFM('SFIND')

#if defined(CLEO_SFDIAG)
         PRINT 9903,SFSVVA
 9903    FORMAT('        RETURN FROM SFSVFM, SFSVVA=',I1)
#endif

         SFSVWE=1

      sf_data_input_sfsvwe=SFSVWE
#define SF_DATA_SET_SFSVWE
#include "doit/sf_data_access/sf_data_access.inc"

      ELSE
         SFSVVA=0
      ENDIF


C------------------------------------------------------------------
C faster reset required for beam wall finder, reset only the used hits
C------------------------------------------------------------------

      IF((CNTLSF.EQ.5).AND.(.NOT.LFIRST).AND.(NLSTSF.LT.MLSTSF))THEN
            IF(NLSTSF.GT.0)THEN
                  DO 23 ILSTSF=1,NLSTSF
                  IADR0=LSTCEL(ILSTSF)
                  ISTFOR(IADR0)=0
                  ISTBAK(IADR0)=0
                  INUCHN(IADR0)=0 
                  DOUBPT(IADR0)=0
#if defined(CLEO_SFDIAG)
                  INUEFF(IADR0)=0
#endif
23                CONTINUE
                  NLSTSF=0
            ENDIF
      ELSE

C------------------------------------------------------------------
C general reset
C------------------------------------------------------------------

            CALL VZERO(ISTFOR,5*MHITSF)
#if defined(CLEO_SFDIAG)
            CALL VZERO(INUEFF,  MHITSF)
#endif
            LFIRST=.FALSE.
            NLSTSF=0
      ENDIF


C------------------------------------------------------------------
C overide the event hits with input from terminal
C------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      CALL DSFDM00
      IF(OVERIDE.NE.-1)THEN

         PRINT 1001
 1001    FORMAT(' SFIND: DO YOU WANT TO OVERIDE ?,',
     2        '"Y"es, --->"N"o, or "K"ill')
         CALL DSF_RFV_INPUT(IY)
         CALL DSF_ABORT(ABORT)
         IF(ABORT.EQ.1)GO TO 599
         IF(IY.EQ.'K')THEN
            OVERIDE=-1
         ELSEIF(IY.EQ.'Y')THEN
            OVERIDE=1
         ELSE
            OVERIDE=0
         ENDIF
      ENDIF
      GO TO 53

 52   CONTINUE
      PRINT 1004
 1004 FORMAT(' DO YOU WANT TO OVERIDE AGAIN?  "Y"es, --->"N"o')
      CALL DSF_RFV_INPUT(IY)
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 599
      IF(IY.EQ.'Y')THEN
         OVERIDE=1
      ELSE
         OVERIDE=0
      ENDIF


 53   CONTINUE
      IF(OVERIDE.EQ.1)THEN
         CALL SFOVER
         CALL VZERO(ISTFOR,5*MHITSF)
         NLSTSF=0
         CALL DSFDM00
         GO TO 120
      ENDIF
#endif

C------------------------------------------------------------------
C ADDRESSING OF INUCHN ETC. IS SF
C ELIMINATE HITS WITH A TIME CUT
C ELIMINATE HITS WITH A PH CUT
C ELIMINATE HITS THAT ARE IN A DUET TRACK
C SFCHAN IGNORES ALL HITS WITH INUCHN(IADR0)<0
C    BOTH AS DESIRED HIT OR ADJACENT
C------------------------------------------------------------------

C------------------------------------------------------------------
C loop over the SF layers
C------------------------------------------------------------------

      DO 79 LYRSF=1,NLAYSF
         LYRCD=CDLASF(LYRSF)
         
C------------------------------------------------------------------
C loop over the hits in the layer
C------------------------------------------------------------------

      sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

61       CONTINUE
         IF(IH.GT.0)THEN
            SO_FAR_SO_GOOD=.TRUE.

C------------------------------------------------------------------
C addresses of the wire and the neighbors of the wire
C remember, SFSVWE=1
C------------------------------------------------------------------

      sf_data_input_hit_adr=IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
      IWR0=sf_data_output_wire

            ALTRNT=IWR0.GT.NWRSF(LYRSF)
            IWRM=IWR0-1
            IWRP=IWR0+1

            IF(.NOT.ALTRNT)THEN
               IF(IWRM.LT.  0           )IWRM=IWRM+NWRSF(LYRSF)
               IF(IWRP.GE.  NWRSF(LYRSF))IWRP=IWRP-NWRSF(LYRSF)
            ELSE
               IF(IWRM.LT.  NWRSF(LYRSF))IWRM=IWRM+NWRSF(LYRSF)
               IF(IWRP.GE.2*NWRSF(LYRSF))IWRP=IWRP-NWRSF(LYRSF)
            ENDIF

            IADR0=INSFSF(LYRSF)+IWR0
            IADRM=INSFSF(LYRSF)+IWRM
            IADRP=INSFSF(LYRSF)+IWRP

C INCDSF points to either the CD of SV geometry, depending on layer
            IADCD=INCDSF(LYRSF)+IWR0
C save hits in the hit list
            IF(NLSTSF.LT.MLSTSF)THEN
               NLSTSF=NLSTSF+1
               LSTCEL(NLSTSF)=IADR0
            ENDIF

C------------------------------------------------------------------
C distance and time cuts
C------------------------------------------------------------------

            IF(LYRCD.GT.0)THEN

C------------------------------------------------------------------
C cut in the raw TDC
C------------------------------------------------------------------
               IF(SO_FAR_SO_GOOD)THEN

      sf_data_input_hit_adr=IH
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TIME=sf_data_output_tdc_raw
      
                  IF( 
     1                 (TIME .LT.RAW_TDC_L(LYRSF)).OR.
     1                 (TIME .GT.RAW_TDC_H(LYRSF)))THEN
                     INUCHN(IADR0)=-1
                     SO_FAR_SO_GOOD=.FALSE.
                  ENDIF
               ENDIF

C------------------------------------------------------------------
C cut in the drift time in picoseconds
C------------------------------------------------------------------
               IF(SO_FAR_SO_GOOD)THEN

      sf_data_input_hit_adr=IH
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
      TIMER=sf_data_output_r_tim_ps

                  IF(
     1                 (TIMER.LT.TIME_PS_L(LYRSF)).OR.
     1                 (TIMER.GT.TIME_PS_H(LYRSF)))THEN
                     INUCHN(IADR0)=-1
                     SO_FAR_SO_GOOD=.FALSE.
                  ENDIF
               ENDIF
                  
C------------------------------------------------------------------
C          cut on the drift distance stored in DUET
C-------------------------------[-----------------------------------
               IF(SO_FAR_SO_GOOD)THEN

                  IHDUET=IABS(IADRHW(IADCD))
                  IF(IHDUET.GT.0)THEN
                     DIST=DWCD(IHDUET)*1000000.
                     IF(
     1                    (DIST .LT.DRIFT_D_L(LYRSF)).OR.
     1                    (DIST .GT.DRIFT_D_H(LYRSF)))THEN
                        INUCHN(IADR0)=-1
                        SO_FAR_SO_GOOD=.FALSE.
                     ENDIF
                  ENDIF
               ENDIF


C------------------------------------------------------------------
C pulse height cut
C------------------------------------------------------------------
               IF(SO_FAR_SO_GOOD)THEN
                  

C      sf_data_input_hit_adr=IH
C#define SF_DATA_RADC
C#include "doit/sf_data_access/sf_data_access.inc"
C      PULSEH=sf_data_output_adc_raw

      sf_data_input_hit_adr=IH
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
      PULSEH=sf_data_output_ph_happy

                  IF(
     1               (PULSEH.LT.PULS_HT_L(LYRSF)).OR.
     1               (PULSEH.GT.PULS_HT_H(LYRSF)))THEN
                     INUCHN(IADR0)=-2
                     SO_FAR_SO_GOOD=.FALSE.
                  ENDIF
               ENDIF
         
C------------------------------------------------------------------
C end of the LYRCD condition
C------------------------------------------------------------------
            ENDIF
C------------------------------------------------------------------
C cut on hits used by previous DUET tracks
C not beam wall
C------------------------------------------------------------------
#if defined(CLEO_SFDUET)
            IF(SO_FAR_SO_GOOD)THEN
               IF(LYRCD.GT.0)THEN
                  IF(CNTLSF.NE.5)THEN 
                     IF(IADRHW(IADCD).LT.0)THEN
                        INUCHN(IADR0)=-3
                        IF(INUCHN(IADRM).EQ.0)INUCHN(IADRM)=-4
                        IF(INUCHN(IADRP).EQ.0)INUCHN(IADRP)=-4
                        SO_FAR_SO_GOOD=.FALSE.
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
#endif

C process next hit

      sf_data_input_hit_adr=IH
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

            GO TO 61

C end of valid hit condition
         ENDIF
C end of loop over layers
 79   CONTINUE
C------------------------------------------------------------------
C display
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
120   CONTINUE
      CALL DSF_RFVIEW('SFIND')
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 599

      CALL DSFDM0
#endif

C------------------------------------------------------------------
C determine if SFIND will be run 
C------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      IF(ANALYZ.EQ.1)THEN
      ELSEIF(ANALYZ.EQ.-1)THEN
         GO TO 599
      ELSE
 81      CONTINUE
#if defined(CLEO_SFDUET)
         PRINT 1021,NRUN,NEVENT
 1021    FORMAT(' SFIND:',
     2        ' DO YOU WANT TO ANALYZE EVENT',I7,'/',I7,' ?'/
     3        '     --> "Y"ES ,  "N"O ,  "A"LWAYS , "K"ILL (NEVER)')
#else
         PRINT 1021
 1021    FORMAT(' SFIND:',
     2        ' DO YOU WANT TO ANALYZE EVENT ?'/
     3        '     --> "Y"ES ,  "N"O ,  "A"LWAYS , "K"ILL (NEVER)')
#endif

         CALL DSF_RFV_INPUT(IY)
         CALL DSF_ABORT(ABORT)
         IF(ABORT.EQ.1)GO TO 599
         IF(IY.EQ.'Y')THEN
         ELSEIF(IY.EQ.'N')THEN
            GO TO 599
         ELSEIF(IY.EQ.'A')THEN
            ANALYZ=1
         ELSEIF(IY.EQ.'K')THEN
            ANALYZ=-1
            GO TO 599
         ELSEIF(IY.EQ.' ')THEN
         ELSEIF(IY.EQ.'null')THEN
         ELSE
            GO TO 81
         ENDIF
      ENDIF
#endif

C------------------------------------------------------------------
C DETERMINE MODE FOR SFIND, IF COSMICS ARE TO BE ANALYZED INSIDE DUET
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      YSFEFF=1
#if defined(CLEO_SFDUET)
      IF(ASFEFF.EQ.1)THEN
         YSFEFF=1
      ELSEIF(ASFEFF.EQ.-1)THEN
         YSFEFF=0
      ELSE
 91      CONTINUE
         PRINT 1022
 1022    FORMAT(' SFIND: do you want to analyze with SFEFF ?'/
     1        '     "Y"ES ,  --> "N"O ,  "A"LWAYS , "K"ILL (NEVER)')
         CALL DSF_RFV_INPUT(IY)
         CALL DSF_ABORT(ABORT)
         IF(ABORT.EQ.1)GO TO 599
         IF(IY.EQ.'Y')THEN
            YSFEFF=1
         ELSEIF(IY.EQ.'N')THEN
            YSFEFF=0
         ELSEIF(IY.EQ.'A')THEN
            ASFEFF=1
            YSFEFF=1
         ELSEIF(IY.EQ.'K')THEN
            ASFEFF=-1
            YSFEFF=0
         ELSEIF(IY.EQ.' ')THEN
            YSFEFF=0
         ELSEIF(IY.EQ.'null')THEN
            YSFEFF=0
         ELSE
            GO TO 91
         ENDIF
      ENDIF
#endif
#endif

C------------------------------------------------------------------
C SFCHAN SFCHAN       SECTION FOR FINDING PRIMARY CHAINS
C SFCHAN SFCHAN       SECTION FOR FINDING PRIMARY CHAINS
C SFCHAN SFCHAN       SECTION FOR FINDING PRIMARY CHAINS
C------------------------------------------------------------------

      SFCDIR=1
      IF(FRSTST.GT.LASTST)SFCDIR=-1
#if defined(CLEO_SFDIAG)
      PRINT 3002,FRSTST,LASTST,SFCDIR
 3002 FORMAT(' SFIND:FRSTST,LASTST,SFCDIR=',3I4)
#endif
      DO 127 ILAY=FRSTST,LASTST,SFCDIR
      CALL SFCHAN(1,ILAY+0,LASTFN,SFCERR)
#if defined(CLEO_SFDIAG)
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 599
#endif
      IF(SFCERR.EQ.0)GO TO 127
      WRITE(CHMESS,3001,IOSTAT=IOS) SFCERR,ILAY
3001  FORMAT(' FATAL ERROR IN SFCHAN, CODE=',I5,'  CHAIN#',I3)
      CALL MESLOG('SFIND',1,MSERRO)

#if defined(CLEO_SFDIAG)
      CALL DSFDCH0
#endif
127   CONTINUE

C------------------------------------------------------------------
C END OF SECTION FOR FINDING PRIMARY CHAINS
C------------------------------------------------------------------
C------------------------------------------------------------------
C FILL THE FLAGS FOR CHAIN USE
C------------------------------------------------------------------
C KEY TO ICHANU....=0  CHAIN IS BAD OR TRIED, DO NOT USE IN ANY WAY
C KEY TO ICHANU....=1  CHAIN IS MARGINAL, CAN BE USED IF PART OF MERGE
C KEY TO ICHANU....=2  CHAIN IS USABLE, BUT CAN NOT BE MERGED AGAIN
C KEY TO ICHANU....=3  CHAIN IS USABLE, CAN BE MERGED TO THE LOW END
C KEY TO ICHANU....=4  CHAIN IS USABLE, CAN BE MERGED TO THE HIGH END
C KEY TO ICHANU....=5  CHAIN IS USABLE AND COMPLETELY MERGEABLE
C KEY TO ICHANU....CLASSES 3 AND 4 NOT IMPLEMENTED, NOW CALLED =2
C------------------------------------------------------------------

      ICHAN1=1
      NCHANH=NCHAIN
      IF(NCHANH.GT.0)THEN
         DO 248 ICHAIN=1,NCHANH
            NXTCHN(ICHAIN)=ICHAIN+1
            KILCHN(1,ICHAIN)=ICHAIN
            KILCHN(2,ICHAIN)=0
            ICHANU(ICHAIN)=5
 248     CONTINUE
         NXTCHN(NCHANH)=0
      ENDIF

C------------------------------------------------------------------
C back up the data structure in temporary arrays for diagnostics
C------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      CALL UCOPY(LINCHN,LINTMP,MCHAIN)
      CALL UCOPY(MINCHN,MINTMP,MCHAIN)
      CALL UCOPY(ISTFOR,ISFTMP,MHITSF)
      CALL UCOPY(ISTBAK,ISBTMP,MHITSF)
      CALL UCOPY(INUCHN,INUTMP,MHITSF)
      CALL UCOPY(DOUBPT,DOUTMP,MHITSF)
      CALL UCOPY(INUEFF,INETMP,MHITSF)
#endif

C------------------------------------------------------------------
C SET THE INUCHN=-4 (adjacent to previous doit tracks) HITS BACK T0 0
C------------------------------------------------------------------
      DO 279 LYRSF=1,NLAYSF
      LYRCD=CDLASF(LYRSF)
      IF(LYRCD.GT.0)THEN

      sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

261      IF(IH.EQ.0)GO TO 279

      sf_data_input_hit_adr=IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
      IWR0=sf_data_output_wire

         IADR0=INSFSF(LYRSF)+IWR0
         IF(INUCHN(IADR0).EQ.-4)INUCHN(IADR0)=0

      sf_data_input_hit_adr=IH
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr
  
         GO TO 261


      ENDIF
279   CONTINUE
C------------------------------------------------------------------
C set up variables required if no chains found
C------------------------------------------------------------------
      NCHANM=0
      NLEN10=0
#if defined(CLEO_SFDIAG)
      CALL DSF_RFVIEW2('SFIND1')
      CALL DSF_ABORT(ABORT)
      IF(ABORT.EQ.1)GO TO 599
#endif
C------------------------------------------------------------------
C SFCHAN SFCHAN       LOOP TO EXTEND PRIMARY CHAINS
C SFCHAN SFCHAN       LOOP TO EXTEND PRIMARY CHAINS
C SFCHAN SFCHAN       LOOP TO EXTEND PRIMARY CHAINS
C------------------------------------------------------------------
#if defined(CLEO_SFDUET)
      CALL DECSRI(ICHAN1,NXTCHN,SNGCHN)

C must order the chains and change the loop to be over nxtchn
      
      IF(NCHANH.GT.0)THEN
         OCHAIN=0
C         DO 348 ICHAIN=1,NCHANH
            ICHAIN=ICHAN1
            COUNTC=0
            GO TO 323
 321        IF(COUNTC.EQ.NCHANH)GO TO 348
            ICHAIN=NXTCHN(ICHAIN)
            IF(ICHAIN.EQ.0)GO TO 348
            IF(CLNCHN(ICHAIN).EQ.2)GO TO 321
 323        COUNTC=COUNTC+1

            DO 328 IEXTND=1,2
               IF(CHNEXT(IEXTND).NE.0)THEN
                  ANOTHR=0   

C duplicate the chain for silicon west and east
                  IF(LCLNSF(IEXTND))THEN
                     IF((SFSVVA.EQ.1).AND.(NCHAIN.LT.MCHAIN))THEN
                        NCHAIN=NCHAIN+1
                        LINCHN(NCHAIN)=LINCHN(ICHAIN)
                        MINCHN(NCHAIN)=MINCHN(ICHAIN)
                        LENCHN(NCHAIN)=LENCHN(ICHAIN)
                        SNGCHN(NCHAIN)=SNGCHN(ICHAIN)
                        LVLCHN(1,NCHAIN)=LVLCHN(1,ICHAIN)
                        LVLCHN(2,NCHAIN)=LVLCHN(2,ICHAIN)
                        ZR0CHN(NCHAIN)=ZR0CHN(ICHAIN)
                        ZDRCHN(NCHAIN)=ZDRCHN(ICHAIN)
                        NXTSAV=NXTCHN(ICHAIN)
                        NXTCHN(ICHAIN)=NCHAIN
                        NXTCHN(NCHAIN)=NXTSAV
                        KILCHN(1,NCHAIN)=NCHAIN
                        KILCHN(2,NCHAIN)=ICHAIN
                        KILCHN(3,NCHAIN)=0
                        ICHANU(NCHAIN)=5
                        HTUCHN(NCHAIN)=HTUCHN(ICHAIN)
                        CLNCHN(NCHAIN)=2
                        KILCHN(2,ICHAIN)=NCHAIN
                        KILCHN(3,ICHAIN)=0
                        ANOTHR=NCHAIN
                     ENDIF
                  ENDIF
                  UCHAIN=ICHAIN

C set the readoutside for the silicon
 325              SFSVWE=1
                  IF(CLNCHN(UCHAIN).EQ.2)SFSVWE=2

      sf_data_input_sfsvwe=SFSVWE
#define SF_DATA_SET_SFSVWE
#include "doit/sf_data_access/sf_data_access.inc"

#if defined(CLEO_SFDIAG)
                  IF(UCHAIN.NE.OCHAIN)THEN
                     CALL DSF_CHVIEW1(UCHAIN+0)
                     CALL DSF_ABORT(ABORT)
                     IF(ABORT.EQ.1)GO TO 599
C                    CALL DSFDCH(UCHAIN+0,0)
C                    CALL DSF_ABORT(ABORT)
C                    IF(ABORT.EQ.1)GO TO 599
C after CHVIEW1, turn off display of current chain
C and delete graphics segment from CHVIEW1 
                     CALL DSF_CHVIEW_SIMPLE(UCHAIN+0,0)
                     CALL DSF_CHVIEW_DELSEG(UCHAIN+0)
                     OCHAIN=UCHAIN
                  ENDIF
#endif
C chain is displayed during SFCHAN, but turned off at end of SFCHAN
                  CALL SFCHAN(2,UCHAIN+0,-CHNEXT(IEXTND)*VSTORF,SFCERR)
#if defined(CLEO_SFDIAG)
                  CALL DSF_ABORT(ABORT)
                  IF(ABORT.EQ.1)GO TO 599
#endif
                  IF(SFCERR.NE.0)THEN
                     WRITE(CHMESS,3001,IOSTAT=IOS) SFCERR,UCHAIN
                     CALL MESLOG('SFIND',1,MSERRO)
                     ICHANU(UCHAIN)=0
#if defined(CLEO_SFDIAG)
                     CALL DSFDCH0
#endif
                  ENDIF
#if defined(CLEO_SFDIAG)
C display chain for DSFDCH, test for errors in data structure
C                 CALL DSF_CHVIEW_SIMPLE(UCHAIN+0,3)
C                 CALL DSFDCH(UCHAIN+0,0)
                  CALL DSF_ABORT(ABORT)
                  IF(ABORT.EQ.1)GO TO 599
#endif
                  IF(ANOTHR.NE.0)THEN
                     UCHAIN=ANOTHR
                     ANOTHR=0
                     GO TO 325
                  ENDIF
               ENDIF
 328        CONTINUE
            IF(LENCHN(ICHAIN).GE.10)NLEN10=NLEN10+1
C------------------------------------------------------------------
C END OF LOOP TO EXTEND PRIMARY CHAINS
C------------------------------------------------------------------
            GO TO 321
 348     CONTINUE
C branch point if there are no chains
      ENDIF
#endif
C------------------------------------------------------------------
C histograming in production run
C------------------------------------------------------------------
#if defined(CLEO_SFDUET)
C PUT IN DUET HISTOGRAMS OF NCHAIN AND NLEN10
      IF((CNTLSF.NE.2).AND.(CNTLSF.NE.5))THEN
            IF(LCOMPR)THEN
C-A Lyon-5-24-99              CALL DGH1(50,FLOAT(NCHAIN))
C-A Lyon-5-24-99              CALL DGH1(51,FLOAT(NLEN10))
            ENDIF
      ENDIF
#endif
      IF(NCHAIN.GT.0)THEN
C------------------------------------------------------------------
C SFFILL SFFILL       SEPARATE LOOP FOR SFFILL
C SFFILL SFFILL       SEPARATE LOOP FOR SFFILL
C SFFILL SFFILL       SEPARATE LOOP FOR SFFILL
C------------------------------------------------------------------

C FIRST ARGUMENT IS =0 TO LOOP OVER THE CHAINS INTERNAL TO SFFILL
C SECOND ARGUMENT =1 TO FILL IN REAL HITS IN INUEFF
C THIRD ARGUMENT =0 TO NOT EXTRAPOLATE TO INNER LAYERS
C SFFILL CAN NOT BE RUN AFTER SFMERG,
C    BECAUSE SFMERG CHANGES ISTFOR AND ISTBAK
         CALL SFFILL(0,1,0)
         IF(CNTLSF.NE.2)THEN

#if defined(CLEO_SFDIAG)
C FIRST ARGUMENT IS =0 TO LOOP OVER THE CHAINS INTERNAL TO SFFILL
C SECOND ARGUMENT =2 TO FILL IN NON HITS IN INUEFF
C THIRD ARGUMENT =0 TO NOT EXTRAPOLATE TO INNER LAYERS
C SFFILL(N,2,M)  IS NEEDED FOR SFEFF
C SFFILL(N,2,M) CAN BE RUN ONLY AFTER ALL OF EXTENDING IS DONE
C    BECAUSE  SFCHAN WILL NOT RECOGNIZE NON-HITS FILLED IN SFFILL
C SFFILL CAN NOT BE RUN AFTER SFMERG,
C    BECAUSE SFMERG CHANGES ISTFOR AND ISTBAK
C IF (.not.CLEO_SFDUET), YSFEFF WILL BE SET =1
C  OTHERWISE, YSFEFF WILL BE DETERMINED FROM QUESTIONS

            IF(YSFEFF.EQ.1)CALL SFFILL(0,2,0)
#endif
C------------------------------------------------------------------
C END OF SECTION; SEPARATE LOOP FOR SFFILL
C------------------------------------------------------------------
#if defined(CLEO_SFDUET)
C------------------------------------------------------------------
C SFMERG SFMERG       SFMERG, CAREFUL! CAN NOT BE RUN AFTER SFFILL(N,2)
C SFMERG SFMERG       SFMERG, CAREFUL! CAN NOT BE RUN AFTER SFFILL(N,2)
C SFMERG SFMERG       SFMERG, CAREFUL! CAN NOT BE RUN AFTER SFFILL(N,2)
C------------------------------------------------------------------
#if !defined(CLEO_SFDIAG)
            CALL SFMERG
#endif

#if defined(CLEO_SFDIAG)
            IF(YSFEFF.EQ.0)THEN
               PRINT 1032
 1032          FORMAT(' SFIND: ',
     2              'will call SFMERG,  ackowledge or "K"ill')

               IF(ACKMERG.NE.-1)THEN
                  CALL DSF_RFV_INPUT(IY)
                  CALL DSF_ABORT(ABORT)
                  IF(ABORT.EQ.1)GO TO 599
                  IF(IY.EQ.'K')ACKMERG=-1
               ENDIF

               CALL SFMERG
               CALL DSF_ABORT(ABORT)
               IF(ABORT.EQ.1)GO TO 599
            ENDIF
#endif
C------------------------------------------------------------------
C END OF SECTION; SFMERG
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
C------------------------------------------------------------------
C SFEFF SFEFF   SECTION TO ANALYZE HIT EFFICIENCY
C SFEFF SFEFF   SECTION TO ANALYZE HIT EFFICIENCY
C SFEFF SFEFF   SECTION TO ANALYZE HIT EFFICIENCY
C------------------------------------------------------------------
            IF(YSFEFF.EQ.1)THEN
               CALL SFEFF1
               CALL SFEFF2
            ENDIF
C------------------------------------------------------------------
C END OF SECTION TO ANALYZE HIT EFFICIENCY
C------------------------------------------------------------------
#endif
C sfdoit is still active
#endif
         ENDIF
      ENDIF
599   CONTINUE

C------------------------------------------------------------------
C overide
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      IF(OVERIDE.EQ.1)GO TO 52
#endif

C------------------------------------------------------------------
C display
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      CALL DSF_ABORT(ABORT)
      IF(ABORT.NE.1)THEN
         CALL DSF_RFVIEW2('SFIND2')
         CALL DSF_ABORT(ABORT)
      ENDIF
      CALL DSF_LEAV(CRNAME)
      GO TO 899
#endif

C------------------------------------------------------------------
C entry to reset conditions
C------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      ENTRY SFIND0
      ANALYZ=0
      OVERIDE=0
      ASFEFF=0
      ACKMERG=0
#endif

C------------------------------------------------------------------
C the end
C------------------------------------------------------------------
899   RETURN
      END

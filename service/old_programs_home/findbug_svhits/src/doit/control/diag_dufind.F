*
* $Id: diag_dufind.F,v 1.18 2001/09/12 21:58:06 dpp Exp $
*
* $Log: diag_dufind.F,v $
* Revision 1.18  2001/09/12 21:58:06  dpp
*      -> added entries
*
* Revision 1.17  2001/08/21 14:10:31  dpp
*      -> use "KILL" to disable acknowledge of track list
*
* Revision 1.16  2001/01/04 16:46:01  dpp
*      -> display D0CF
*
* Revision 1.15  2000/12/19 23:48:44  dpp
*      -> another choice
*
* Revision 1.14  2000/11/15 16:51:29  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.13  2000/10/24 22:38:36  dpp
*      -> print out NFIT
*      -> change a compile flag for after SFIND
*      -> acknowledge at cut
*
* Revision 1.12  2000/10/02 20:56:00  dpp
*      -> remove obsolete comment
*      -> adjust interupt values; make room/define value for BOOT_SI_Z_HITS
*
* Revision 1.11  2000/08/28 17:39:41  dpp
*      -> new key assignments, change setting of INTERRUPT_VALUE
*
* Revision 1.10  2000/08/24 19:03:47  dpp
* -> new DCLEO_DIAG_DUFIND compile switch, dump w/o prompt
*
* Revision 1.9  2000/08/18 18:44:17  dpp
*     -> diagnostics
*
* Revision 1.8  2000/04/26 14:39:25  dpp
*      -> remove obsolete calls to XTS_TRAK_INIT
*      -> use new XTS_TRAK variables: INTERRUPT_VALUE and INTERRUPT_THRESH
*
* Revision 1.7  2000/02/12 20:22:04  dpp
*      -> print out value of NEXPT; now used as the doit running level flag
*
* Revision 1.6  1999/11/17 21:44:20  dpp
* print out number of calls to WIRES_2_HITS
*
* Revision 1.5  1999/10/12 15:17:21  dpp
* change reset of XTS_NEW_TRACK for "NEXT PASS"
*
* Revision 1.4  1999/10/06 12:35:37  dpp
* fix print statement
*
* Revision 1.3  1999/05/28 03:04:43  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1999/04/27 20:10:30  dpp
* set XTS_NEW_TRACK; revive calls to XTS_TRAK_INIT
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE DIAG_DUFIND(KEY,IINPUT,LRETRN)
C.......................................................................
C.
C. DIAG_DUFIND - diagnostic entries for DUFIND
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 06-03-95
C. LAST MOD  : 
C.
C. Modification Log.
C.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdcorrcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfxfitcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tffitctl.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif


      INTEGER ITRK,IFSTAT,NLDROP

      LOGICAL LFOUND,LFIRST_GRAPHICS,LFIRST_HBOOK
      REAL RMSRET,RMS0,RMSLAS,DIRRMS,T0BOT
      REAL TFSAV,TDSAV,TCSAV

      INTEGER JBIT
      EXTERNAL JBIT

      SAVE LFIRST_GRAPHICS,LFIRST_HBOOK
      DATA LFIRST_GRAPHICS,LFIRST_HBOOK/ .TRUE. , .TRUE. /


      REAL ANG,PMPI
      LOGICAL LRETRN
      INTEGER KEY,IINPUT,USEED,AFTRSF
      INTEGER IY
      INTEGER AFTRTRKS/0/

      INTEGER IFIT
      INTEGER TRK_HAS_CATH,CATH_USED(1536)
      INTEGER IWIRE,IWADR,IWADR_USED(KWS1CD)

      REAL CFIX
      COMMON/FIXER/CFIX(250)

      REAL DUM

* ----------Executable code starts here---------------------
C force opening of XTS graphics window
C and call the autorizations of XTS_TRAK
      IF(KEY.EQ.1)THEN
         PRINT 1011,NEXPT
 1011    FORMAT(' DIAG_DUFIND: ENTER: NEXPT=',I4)
         LRETRN=.TRUE.
         LXTSUB=.TRUE.
         IF(LFIRST_GRAPHICS)THEN
            USEED=0
            AFTRSF=0

#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INIT ('CS', 1,   1,'SFINIT')
            CALL XTS_TRAK_INIT ('CF', 1,   1,'SFINIT')
            CALL XTS_TRAK_INIT ('CD', 1,   1,'SFINIT')
#endif
            LFIRST_GRAPHICS=.FALSE.
         ENDIF


C print message at start of  SF
      ELSEIF(KEY.EQ.3)THEN
         LRETRN=.TRUE.
         PRINT 1031
 1031    FORMAT(' DIAG_DUFIND:WILL CALL SFIND')

C print message at after SFIND
      ELSEIF(KEY.EQ.4)THEN
         PRINT 1041,NCHAIN
 1041    FORMAT(' DIAG_DUFIND: BACK FROM SFIND: NCHAIN=',I4)

#if defined(CLEO_SFDIAG)
         IF(AFTRSF.EQ.-1)THEN
            LRETRN=.FALSE.
         ELSEIF(AFTRSF.EQ. 1)THEN
            LRETRN=.TRUE.
         ELSE
            PRINT 1042
            CALL DSF_RFV_INPUT(IY)
#if defined(CLEO_XTSUBS)
            IF(XTS_ABORT.EQ.1)IY='N'
#endif
            IF(IY.EQ.'Y')THEN
               LRETRN=.TRUE.
            ELSEIF(IY.EQ.'A')THEN
               LRETRN=.TRUE.
               AFTRSF= 1
            ELSEIF(IY.EQ.'N')THEN
               LRETRN=.FALSE.
            ELSEIF(IY.EQ.'K')THEN
               LRETRN=.FALSE.
               AFTRSF=-1
            ELSE
               LRETRN=.TRUE.
            ENDIF
         ENDIF
#else
         LRETRN=.TRUE.
#endif
         IF(LRETRN)THEN
            PRINT 1044
         ELSE
            PRINT 1043
         ENDIF

 1042    FORMAT(' DIAG_DUFIND: do you want continue after SFIND ?'/
     2        ' "-->Y"es, "N"o, "A"lways, or  "K"ill')
 1043    FORMAT(' DIAG_DUFIND: DUFIND will return after SFIND')
 1044    FORMAT(' DIAG_DUFIND: DUFIND will continue after SFIND')
         

      ELSEIF(KEY.EQ.6)THEN
         LRETRN=.TRUE.
         PRINT 1061
 1061    FORMAT(' DIAG_DUFIND: will call SFHITS'
     2        '  and reset XTS_TRAK display')
#if defined(CLEO_XTSUBS)
         IF(XTS_ABORT.EQ.0)CALL XTS_TRAK_INTERRUPT_THRESH('0')
         CALL XTS_TRAK_INTERRUPT_VALUE('0')
#endif
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      ELSEIF(KEY.EQ.7)THEN
         LRETRN=.TRUE.
         PRINT 1071
 1071    FORMAT(' DIAG_DUFIND:',
     2        ' will call FAST_TRAK',
     3        ' (set xts_trak INTERRUPT)')
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif

C this is for zooming
         TFXS=1
C this clears the counter in WIRES_2_HITS
         CALL WIRES_2_HITS(USE_ACCESS,0,0)


      ELSEIF(KEY.EQ.8)THEN
         LRETRN=.TRUE.
C this prints the counter in WIRES_2_HITS
         CALL WIRES_2_HITS(USE_ACCESS,1,0)
         PRINT 1081,NFIT
 1081    FORMAT(' DIAG_DUFIND: return from FAST_TRAK',
     2        ', NFIT=',I4)


      ELSEIF(KEY.EQ.9)THEN
         LRETRN=.TRUE.
         PRINT 1091,CHICF,D0CF,NFIT
 1091    FORMAT(' DIAG_DUFIND: possibly cut on',
     1        ' CHICF=',F8.3,'  D0CF=',F9.6,' .....'
     2        ' NFIT=',I4)

#if !defined(CLEO_XTSUBS)
         accept 1092,IY
 1092    format(a1)
#endif

      ELSEIF(KEY.EQ.10)THEN
         LRETRN=.TRUE.
         PRINT 1101,NFIT
 1101    FORMAT(' DIAG_DUFIND:',
     2        ' will call ROADHITS for CF before TFXFIT',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.11)THEN
         LRETRN=.TRUE.
         PRINT 1111
 1111    FORMAT(' DIAG_DUFIND:',
     2        ' after call to ROADHITS')

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 

      ELSEIF(KEY.EQ.12)THEN
         LRETRN=.TRUE.
         PRINT 1121
 1121    FORMAT(' DIAG_DUFIND:',
     2        ' will call FAST_TRAK_RECALL_DZ_WINDO w/ROADHITS input',
     3        ' to test before TFXFIT')
C     3        ' (set xts_trak INTERRUPT)')

      ELSEIF(KEY.EQ.13)THEN
         LRETRN=.TRUE.
         PRINT 1131,NFIT
 1131    FORMAT(' DIAG_DUFIND:',
     2        ' will call TFXFIT for CF',
     3        '(set xts_trak INTERRUPT)',
     3        ', NFIT=',I4)
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('3')
#endif
         
      ELSEIF(KEY.EQ.14)THEN
         LRETRN=.TRUE.
         PRINT 1141,NFIT
 1141    FORMAT(' DIAG_DUFIND: will call CFLAST',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.15)THEN
         LRETRN=.TRUE.
         PRINT 1151,IINPUT,NFIT
 1151    FORMAT(' DIAG_DUFIND: will call CFCHOP',
     2        ' IF(ILRESC(2)=',I3,'.NE.0)',
     3        ', NFIT=',I4)

      ELSEIF(KEY.EQ.16)THEN
         LRETRN=.TRUE.
         PRINT 1161,NFIT
 1161    FORMAT(' DIAG_DUFIND:',
     2        ' will call ROADHITS for LF fit',
     3        ', NFIT=',I4)

      ELSEIF(KEY.EQ.17)THEN
         LRETRN=.TRUE.
         PRINT 1171,NHITLF,IINPUT,NFIT
 1171    FORMAT(' DIAG_DUFIND:',
     2        ' will call REPLACE_Z_HITS',
     3        ' if NHITLF=',I3,' < cut=',I2,
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.18)THEN
         LRETRN=.TRUE.
         PRINT 1181
 1181    FORMAT(' DIAG_DUFIND:',
     2        ' will call FAST_TRAK_RECALL_DZ_WINDO w/ROADHITS input',
     3        ' to test optional "z" windows AFTER TFXFIT'/
     6        '                        ',
     7        ' (may set xts_trak INTERRUPT inside _RECALL_DZ_WINDO)')


      ELSEIF(KEY.EQ.21)THEN
         LRETRN=.TRUE.
         PRINT 1211,NFIT
 1211    FORMAT(' DIAG_DUFIND:',
     2        ' clear primary stak and push in r-phi result',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.22)THEN
         LRETRN=.TRUE.
         PRINT 1221,NFIT
 1221    FORMAT(' DIAG_DUFIND:',
     2        ' REFIT in Z with Z CONSTRAINT',
     3        ' and push that into the stak',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.23)THEN
         LRETRN=.TRUE.
         PRINT 1231,IINPUT,NFIT
 1231    FORMAT(' DIAG_DUFIND: will call TFXFIT for LF',
     2        ' (rep',I3,')',
     2        '(set xts_trak INTERRUPT)',
     3        ', NFIT=',I4)
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('4')
#endif
         
      ELSEIF(KEY.EQ.24)THEN
         LRETRN=.TRUE.
         CFIX(114)=Y0LF
         PRINT 1241,NFIT
 1241    FORMAT(' DIAG_DUFIND: will call TFPUSH after TFXFIT-LF',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.25)THEN
         LRETRN=.TRUE.
         PRINT 1251,NFIT
 1251    FORMAT(' DIAG_DUFIND: order the STAK and call TFPOPF',
     2        ' after calls to TFXFIT',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.28)THEN
         LRETRN=.TRUE.
         PRINT 1281,NFIT
 1281    FORMAT(' DIAG_DUFIND: will call BOOT_SI_Z_HITS',
     2        '(set xts_trak INTERRUPT)'
     3        '(set xts_trak INTERRUPT again at end)',
     3        ', NFIT=',I4)
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('5')
#endif
         
      ELSEIF(KEY.EQ.31)THEN
         LRETRN=.TRUE.
         PRINT 1311,NFIT
 1311    FORMAT(' DIAG_DUFIND: will call CDSFSD',
     2        ' in prparation for CFEXTN',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.32)THEN
         LRETRN=.TRUE.
         PRINT 1321,NFIT
 1321    FORMAT(' DIAG_DUFIND: will call CFEXTN',
     3        ', NFIT=',I4)
         
      ELSEIF(KEY.EQ.33)THEN
         LRETRN=.TRUE.
         PRINT 1331,NFIT
 1331    FORMAT(' DIAG_DUFIND: order the STAK and call TFPOPF',
     2        ' in preparation for TF3FIT',
     3        ', NFIT=',I4)
      CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
         
      ELSEIF(KEY.EQ.35)THEN
         LRETRN=.TRUE.
         PRINT 1351,NFIT
 1351    FORMAT(' DIAG_DUFIND:',
     2        ' will call TF3FIT (or F_T_DEL_BAD_AND_ADD)',
     3        ' (set xts_trak INTERRUPT)',
     3        ', NFIT=',I4)
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('7')
#endif
      CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
         
      ELSEIF(KEY.EQ.36)THEN
         LRETRN=.TRUE.
         PRINT 1361,NFIT
 1361    FORMAT(' DIAG_DUFIND:',
     2        ' will call FAST_TRAK_DEL_BAD_AND_ADD',
     3        ' after CF hits and fit',
     4        ', NFIT=',I4)
      CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')

      ELSEIF(KEY.EQ.37)THEN
         LRETRN=.TRUE.
         PRINT 1371,NFIT
 1371    FORMAT(' DIAG_DUFIND:',
     2        ' will call FAST_TRAK_DEL_BAD_AND_ADD',
     3        ' after LF hits and fit',
     4        ', NFIT=',I4)
      CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')

      ELSEIF(KEY.EQ.38)THEN
         LRETRN=.TRUE.
         CFIX(115)=Y0LF
         PRINT 1381,NFIT
 1381    FORMAT(' DIAG_DUFIND: call TFUSTK and TFPOPF',
     2        ' after TF3FIT (or F_T_DEL_BAD_AND_ADD)',
     3        ', NFIT=',I4)
      CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')

         
      ELSEIF(KEY.EQ.40)THEN
         LRETRN=.TRUE.
         PRINT 1401,NFIT,NHITTF
 1401    FORMAT(' DIAG_DUFIND:',
     2        ' will call CDNEWT',
     3        ' (set xts_trak INTERRUPT)',
     3        ', NFIT=',I4,', NHITTF=',I4)
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('8')
         CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
C refresh the CF hits for the display
         CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
#endif

C-------------------
C ADD A QUICKY PLOT
C-------------------
      ELSEIF(KEY.EQ.71)THEN
         LRETRN=.TRUE.
         IF(LFIRST_HBOOK)THEN
            CALL HBOOK1(151,'CURVA ',100,-1.0,1.0,0.)
            CALL HBOOK1(152,'TANDLF',100,-1.5,1.5,0.)
            CALL HBOOK1(153,'Z0    ',100,-.10,.10,0.)
            CALL HBOOK1(154,'CHICF ',100,0.,50.,0.)
            CALL HBOOK1(155,'CURVA ',100,-1.0,1.0,0.)
            CALL HBOOK1(156,'RES ALL  ',100,-.05,.05,0.)
            CALL HBOOK1(161,'RES -1.05',100,-.05,.05,0.)
            CALL HBOOK1(162,'RES -0.70',100,-.05,.05,0.)
            CALL HBOOK1(163,'RES -0.35',100,-.05,.05,0.)
            CALL HBOOK1(164,'RES  0   ',100,-.05,.05,0.)
            CALL HBOOK1(165,'RES +0.35',100,-.05,.05,0.)
            CALL HBOOK1(166,'RES +0.70',100,-.05,.05,0.)
            CALL HBOOK1(167,'RES +1.05',100,-.05,.05,0.)
            LFIRST_HBOOK=.FALSE.
         ENDIF
            
         IF(
     1        (ABS(TANDLF).LT. 1.225).AND.
     1        (ABS(Y0LF).LT. .03).AND.
     3        (CHICF.LT.15.)
     9        )THEN
            CALL HF1(151,KAPCF,1.)
         ENDIF
         
         IF(
     1        (ABS(KAPCF).GT.  0.04).AND.
     2        (ABS(KAPCF).LT.  0.16).AND.
     1        (ABS(Y0LF).LT. .03).AND.
     2        (CHICF.LT.15.)
     9        )THEN
            CALL HF1(152,TANDLF,1.)
         ENDIF
         
         IF(
     1        (ABS(KAPCF).GT.  0.04).AND.
     2        (ABS(KAPCF).LT.  0.16).AND.
     2        (ABS(TANDLF).LT. 1.225).AND.
     3        (CHICF.LT.15.)
     9        )THEN
            CALL HF1(153,Y0LF,1.)
         ENDIF
         
         
         IF(
     1        (ABS(KAPCF).GT.  0.04).AND.
     2        (ABS(KAPCF).LT.  0.16).AND.
     2        (ABS(TANDLF).LT. 1.225).AND.
     1        (ABS(Y0LF).LT. .03)
     9        )THEN
            CALL HF1(154,CHICF,1.)
         ENDIF
         
         
         IF(
     1        (ABS(KAPCF).GT.  0.04).AND.
     2        (ABS(KAPCF).LT.  0.16).AND.
     2        (ABS(TANDLF).LT. 1.225).AND.
     1        (ABS(Y0LF).LT. .03).AND.
     3        (CHICF.LT.15.)
     9        )THEN
            CALL HF1(155,KAPCF,1.)
            
            DO 711 IFIT=1,NFIT
               IF(IPLFIT(IFIT).EQ.56)THEN
                  CALL HF1(156,PULFIT(IFIT),1.)
                  IF(
     1                 (TANDLF.GE.-1.225).AND.
     2                 (TANDLF.LT.-0.875))THEN
                     CALL HF1(161,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE.-0.875).AND.
     2                    (TANDLF.LT.-0.525))THEN
                     CALL HF1(162,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE.-0.525).AND.
     2                    (TANDLF.LT.-0.175))THEN
                     CALL HF1(163,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE.-0.175).AND.
     2                    (TANDLF.LT. 0.175))THEN
                     CALL HF1(164,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE. 0.175).AND.
     2                    (TANDLF.LT. 0.525))THEN
                     CALL HF1(165,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE. 0.525).AND.
     2                    (TANDLF.LT. 0.875))THEN
                     CALL HF1(166,PULFIT(IFIT),1.)
                  ELSEIF(
     1                    (TANDLF.GE. 0.875).AND.
     2                    (TANDLF.LT. 1.225))THEN
                     CALL HF1(167,PULFIT(IFIT),1.)
                  ENDIF
                  
               ENDIF
 711        CONTINUE
         ENDIF

      ELSEIF(KEY.EQ.72)THEN
         LRETRN=.TRUE.
         CALL VZERO(CATH_USED,1536)
         CALL VZERO(IWADR_USED,KWS1CD)
         PRINT 7701,NTRKCD
 7701    FORMAT(' DOIT:DUFIND:NTRKCD=',I3)
         IF(NTRKCD.GE.1)THEN
            DO 749 ITRK=1,NTRKCD
               IF(NFITCD(ITRK).GE.1)THEN
                  TRK_HAS_CATH=0
                  DO 729 IFIT=1,NFITCD(ITRK)
                     IF(IDVCTF(IABS(IPLRCD(IFIT,ITRK))).EQ.IDRCT)THEN
                        TRK_HAS_CATH=TRK_HAS_CATH+1
                        IF(CATH_USED(IADFCD(IFIT,ITRK)).EQ.0)THEN
                           CATH_USED(IADFCD(IFIT,ITRK))=ITRK
                           IF(TRK_HAS_CATH.EQ.1)THEN
                              PRINT 7711,IADFCD(IFIT,ITRK),ITRK
 7711                         FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                             ' belongs to track',I3)
                           ELSE
                              PRINT 7712,IADFCD(IFIT,ITRK),ITRK
 7712                         FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                             ' belongs to track',I3,
     3                             ' (hopefully!)')
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
                           ENDIF
                        ELSE

                           IF(TRK_HAS_CATH.EQ.1)THEN
                              PRINT 7713,IADFCD(IFIT,ITRK),ITRK,
     2                             CATH_USED(IADFCD(IFIT,ITRK))
 7713                         FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                             ' belongs to track',I3,
     3                             '             (and to track',I3,')')
                           ELSE
                              PRINT 7714,IADFCD(IFIT,ITRK),ITRK,
     2                             CATH_USED(IADFCD(IFIT,ITRK))
 7714                         FORMAT(' DOIT:DUFIND:Cathode hit',I5,
     2                             ' belongs to track',I3,
     3                             ' (hopefully!)(and to track',I3,')')
                           ENDIF
                        ENDIF
                     ELSEIF(
     1                       IDVCTF(IABS(IPLRCD(IFIT,ITRK))).EQ.
     2                       IDRFT)THEN
                        IWIRE=IWIRCD(IABS(IADFCD(IFIT,ITRK)))
                        IWADR=INDXTF(IABS(IPLRCD(IFIT,ITRK)))+IWIRE
                        IF(IWADR_USED(IWADR).EQ.0)THEN
                           IWADR_USED(IWADR)=ITRK
                           PRINT 7721,IPLRCD(IFIT,ITRK),IWIRE,ITRK
 7721                      FORMAT(' DOIT:DUFIND:Anode L',I3,' w',I4,
     2                          ' belongs to track',I3)
                           
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 
                        ELSE
                           PRINT 7723,IPLRCD(IFIT,ITRK),IWIRE,ITRK,
     2                          IWADR_USED(IWADR)
 7723                      FORMAT(' DOIT:DUFIND:Anode L',I3,' w',I4,
     2                          ' belongs to track',I3,
     3                          '             (and to track',I3,')')
                        ENDIF
                     ENDIF
 729              CONTINUE
               ELSE
                  PRINT 7002,ITRK
 7002             FORMAT(' DOIT:DUFIND:',
     2                 'pretty weird, no hits in track',I3)
               ENDIF
 749        CONTINUE
         ENDIF
#if defined(CLEO_SFDIAG)
         IF(AFTRTRKS.EQ.0)THEN
            CALL DSF_RFV_INPUT(IY)
            IF(IY.EQ.'K')THEN
               AFTRTRKS=-1
            ENDIF
         ENDIF
#endif

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C give exit status of DUFIND
      ELSEIF(KEY.EQ.99)THEN
         LRETRN=.TRUE.
C-A Lyon-5-24-99            CALL MAKE
#if defined(CLEO_XTSUBS)
         PRINT 1991,XTS_ABORT,XTS_NEW_TRACK
 1991    FORMAT(' DIAG_DUFIND: DUFIND EXIT, XTS_ABORT=',L1,
     2        ' XTS_NEW_TRACK=',I2)
#endif

C INVALID ENTRY
      ELSE
         PRINT 9999,KEY
 9999    FORMAT(' DIAG_DUFIND: invalid key number=',I10)
      ENDIF   

      RETURN
      END




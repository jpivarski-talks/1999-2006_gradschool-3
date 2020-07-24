*
* $Id: duhbok.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: duhbok.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:22  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:00  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 01/11/91  15.15.32  by  Steve Schaffner
*CMZ :  5.54/03 17/05/91  12.04.16  by  Steve Schaffner
*CMZ :  5.54/00 13/11/90  19.09.23  by  Steve Schaffner
*CMZ :  5.53/07 08/10/90  14.48.02  by  Steve Schaffner
*CMZ :  5.53/06 16/08/90  14.14.54  by  Steve Schaffner
*CMZ :  5.53/04 06/06/90  12.24.51  by  Steve Schaffner
*CMZ :  5.53/01 17/04/90  12.26.21  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.02  by  Steve Schaffner
*CMZ :  5.50/00 13/05/89  12.37.48  by  Steve Schaffner
*CMZ :          28/10/88  00.07.39  by  R. Namjoshi
* Supply all HBOOK arguments for HBOOK4
*CMZ :          24/10/88  13.11.29  by  Dan Riley
* Added "TF LAYER NUKED IN TF3FIT" histograms.
*CMZ :          19/07/88  17.43.42  by  Dan Riley
*-- Author :
      SUBROUTINE DUHBOK
C.......................................................................
C.
C. DUHBOK - Book Duet HBOOK histograms
C.
C. COMMON    : TFCTLI TFCTLR
C. CALLS     : HBOOK1 LENOCC
C. CALLED    : DUHIST
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.02
C. CREATED   : 05-Dec-86
C. LAST MOD  : 19-Jul-88
C.
C.
C. Modification Log.
C.
C. 17-Dec-87 : Add DULFHIST, DUCFHIST histograms.                    dsr
C. 20-Dec-87 : Use LCF/LFHST for CF/LF histograms.                   RMN
C. 19-Jul-88 : Add "LAYER NUKED IN TF3FIT" histograms             dsr
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
C
      CHARACTER*2  LN
      CHARACTER*80 CTIT
      INTEGER I,IE,LENOCC

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"
C
C            CF HISTS
C
C 11,  -5.0,     0.2,   50,
C DIFFERENCE OF DIFCCF
C 12,  -0.25,    0.01,  50,
C CF D0(LINK IN TRACK)
C 17,  -0.25,    0.01,  50,
C CF D0 (ALL LINKS)
C
      IF( LCFHST ) THEN
      CALL HBOOK1( NHDFCF, 'DIFFERENCE OF DIFCCF$',                 50,
     +             -5.0, 5.0, 0.0 )
      CALL HBOOK1( NHDLCF, 'CF D0(LINK IN TRACK)$',                 50,
     +             -0.25, 0.25, 0.0 )
      CALL HBOOK1( NHDACF, 'CF D0 (ALL LINKS)$',                    50,
     +             -0.25, 0.25, 0.0 )
      CALL HBOOK1( NHRSCF, 'CF RESIDUALS$',                         50,
     +             -1250.0, 1250.0, 0.0 )
      CALL HBOOK1( NHSRCF, 'CF RESIDUALS - SCALED WITH DOF$',       50,
     +             -1250.0, 1250.0, 0.0 )
      CALL HBOOK1( NHPRCF, 'CF PROBABILITY OF FIT$',                50,
     +                 0.0,    1.0, 0.0 )
      CALL HBOOK1( NHDTCF, 'CF D0 (TRACK)$',                        50,
     +             -0.05, 0.05, 0.0 )
      ENDIF
C
C     LF     HISTS
C
C 21,  -5.0,     0.2,   50,
C DIFFERENCE OF DIFCLF
C 22,  -0.50,    0.02,  50,
C LF Z0(LINK IN TRACK)
C 27,  -0.50,    0.02,  50,
C LF Z0 (ALL LINKS)
C
      IF( LLFHST ) THEN
      CALL HBOOK1( NHDFLF, 'DIFFERENCE OF DIFCLF$',                 50,
     +             -5.0, 5.0, 0.0 )
      CALL HBOOK1( NHDLLF, 'LF Z0(LINK IN TRACK)$',                 50,
     +             -0.50, 0.50, 0.0 )
      CALL HBOOK1( NHDALF, 'LF Z0 (ALL LINKS)$',                    50,
     +             -0.50, 0.50, 0.0 )
      CALL HBOOK1( NHRSLF, 'LF RESIDUALS$',                         50,
     +             -1250.0, 1250.0, 0.0 )
      CALL HBOOK1( NHSRLF, 'LF RESIDUALS - SCALED WITH DOF$',       50,
     +             -1250.0, 1250.0, 0.0 )
      CALL HBOOK1( NHPRLF, 'LF PROBABILITY OF FIT$',                50,
     +                 0.0,    1.0, 0.0 )
      CALL HBOOK1( NHDTLF, 'LF Z0 (TRACK)$',                        50,
     +                -0.1,    0.1, 0.0 )
      ENDIF
C
C      TF     HISTS
C
      CALL HBOOK1( NHPRTF, 'TF PROB OF 3D FIT$',                    50,
     +                 0.0,    1.0, 0.0 )
      CALL HBOOK1( NHRSTF, 'TF RESID - ALL    ANODE LYRS (SCALED)$',50,
     +             -1000.0, 1000.0, 0.0 )
      CALL HBOOK1( NHRATF, 'TF RESID - AXIAL  ANODE LYRS (SCALED)$',50,
     +             -1000.0, 1000.0, 0.0 )
      CALL HBOOK1( NHRZTF, 'TF RESID - STEREO ANODE LYRS (SCALED)$',50,
     +             -1000.0, 1000.0, 0.0 )
C
      CALL HBOOK1( NHPRPF,
     +    'TF PROB OF 3D FIT, WITH P CUT$',                         50,
     +                 0.0,    1.0, 0.0 )
      CALL HBOOK1( NHRSPF,
     +    'TF RESID - ALL    ANODE LYRS (SCALED), WITH P CUT$',     50,
     +             -1000.0, 1000.0, 0.0 )
      CALL HBOOK1( NHRAPF,
     +    'TF RESID - AXIAL  ANODE LYRS (SCALED), WITH P CUT$',     50,
     +             -1000.0, 1000.0, 0.0 )
      CALL HBOOK1( NHRZPF,
     +    'TF RESID - STEREO ANODE LYRS (SCALED), WITH P CUT$',     50,
     +             -1000.0, 1000.0, 0.0 )
C
      CALL HBOOK1( NHCDFT, 'NUMBER OF CD HITS USED IN FIT$',        69,
     +             0.5, 69.5, 0.0 )
      CALL HBOOK1( NHANFT, 'NUMBER OF CD ANODE HITS USED IN FIT$',  54,
     +             0.5, 54.5, 0.0 )
      IF( LUSECT )
     +CALL HBOOK1( NHCAFT, 'NUMBER OF CD CATHODE HITS USED IN FIT$', 5,
     +             0.5,  5.5, 0.0 )

C
C       MISCELLANEOUS
C
C      CALL HBOOK1( NHK0TF,
C     +    'PI+ PI- (K0) MASS DISTRIBUTION: 2 MEV BINS$',            50,
C     +     0.450, 0.550, 0.0 )
C      CALL HBOOK1( NHK5TF,
C     +    'PI+ PI- (K0) MASS DISTRIBUTION: 5 MEV BINS$',            50,
C     +     0.375, 0.625, 0.0 )
C      CALL HBOOK1( NHL0TF, 'PI  P   (LAMBDA) MASS DISTRIBUTION$',   50,
C     +             1.070, 1.170, 0.0 )
C
      CALL HBOOK1( NHD0TF, 'D0(1)+D0(2) FOR 2-PRONG EVENTS$',       50,
     +             -1250.0, 1250.0, 0.0 )
      CALL HBOOK1( NHDMTF,
     +    'D0(1)+D0(2) FOR 2-PRONG EVENTS WITH .GE. 90% VD HITS$',  50,
     +             -1250.0, 1250.0, 0.0 )
C
C    TEMP
C
C 130,  -10.0,     0.4,    50,
C SIGMA IN CFXFIT FOR SELECTING HITS
C       VERTEX DETECTOR
C
C== PT HISTOGRAMS
C
      IF( LUSEPT ) THEN
      CALL HBOOK1( NHRITF,    'TF RESID - ALL PT LAYERS (SCALED)$', 50,
     +             -1000.0, 1000.0, 0.0 )
      DO 1 I = 1, NCANPT
          CTIT = 'TF RESID (SCALED) - PT LAYER '
          IE   = LENOCC( CTIT ) + 1
          WRITE( LN, 1000 ) I
          CALL HBOOK1( NHRITF+I, CTIT(1:IE)//LN//'$',               50,
     +                 -1000.0, 1000.0, 0.0 )
1     CONTINUE
C
      CALL HBOOK1( NHINFT, 'NUMBER OF PT HITS USED IN FIT$',         12,
     +             0.5,  6.5, 0.0 )
      CALL HBOOK1( NHQIVD, 'PULSE HEIGHT OF PT HITS USE IN FIT$',   50,
     +             0.0, 2000.0, 0.0 )
      CALL HBOOK1( NHIVBF, 'DWCD PT BEFORE FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NHIVAF, 'DWCD PT AFTER  FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NTIMPT, 'PT times', 100, -50000.,150000., 0.0 )
      ENDIF
C
C== IV HISTOGRAMS
C
      IF( LUSEIV ) THEN
      CALL HBOOK1( NHRITF,    'TF RESID - ALL IV LAYERS (SCALED)$', 50,
     +             -1000.0, 1000.0, 0.0 )
      DO 5 I = 1, NCANIV
          CTIT = 'TF RESID (SCALED) - IV LAYER '
          IE   = LENOCC( CTIT ) + 1
          WRITE( LN, 1000 ) I
          CALL HBOOK1( NHRITF+I, CTIT(1:IE)//LN//'$',               50,
     +                 -1000.0, 1000.0, 0.0 )
5     CONTINUE
C
      CALL HBOOK1( NHINFT, 'NUMBER OF IV HITS USED IN FIT$',         8,
     +             0.5,  4.5, 0.0 )
      CALL HBOOK1( NHQIVD, 'PULSE HEIGHT OF IV HITS USE IN FIT$',   50,
     +             0.0, 2000.0, 0.0 )
      CALL HBOOK1( NHIVBF, 'DWCD IV BEFORE FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NHIVAF, 'DWCD IV AFTER  FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      ENDIF
C
C== VD HISTOGRAMS
C

      IF( LUSEVD ) THEN
      CALL HBOOK1( NHRVTF,    'TF RESID - ALL VD LAYERS (SCALED)$', 50,
     +             -1000.0, 1000.0, 0.0 )
C
      DO 10 I = 1, NCANVD
          CTIT = 'TF RESID (SCALED) - VD LAYER '
          IE   = LENOCC( CTIT ) + 1
          WRITE( LN, 1000 ) I
          CALL HBOOK1( NHRVTF+I, CTIT(1:IE)//LN//'$',               50,
     +                 -1000.0, 1000.0, 0.0 )
10    CONTINUE
C
      CALL HBOOK1( NHVNFT, 'NUMBER OF VD HITS USED IN FIT$',        21,
     +             0.0, 10.5, 0.0 )
C
      CALL HBOOK1( NHQ1VD, 'PULSE HEIGHT OF VD HITS USE IN FIT 1$', 50,
     +             0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHQ2VD, 'PULSE HEIGHT OF VD HITS USE IN FIT 2$', 50,
     +             0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHVWBF, 'DWCD VD BEFORE FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NHVWAF, 'DWCD VD AFTER  FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( 160,
     +    'DWCD AS A FRACTION OF MAXIMUM POSSIBLE DRIFT DISTANCE$', 65,
     +                 0.0,    1.3, 0.0 )
      CALL HBOOK1( NTIMVD, 'VD times', 100, -50000., 250000., 0.0 )
C
      ENDIF
C
C          CHARGE DIVISION
C
      IF( LUSEQD ) THEN
      CALL HBOOK1( NHRQTF,'CHARGE DIVISION - VD ALL LAYERS  (MM)$', 50,
     +               -75.0,   75.0, 0.0 )
C
      DO 20 I = 1, NLYRVD
          CTIT = 'CHARGE DIVISION (MM) - VD LAYER '
          IE   = LENOCC( CTIT ) + 1
          WRITE( LN, 1000 ) I
          CALL HBOOK1( NHRQTF+I, CTIT(1:IE)//LN//'$',               50,
     +                 -75.0, 75.0, 0.0 )
20    CONTINUE
C
      CALL HBOOK1( 270, 'DELTA VD HIT, ALL LAYERS:   PROJECTION$',  50,
     +             -2500.0, 2500.0, 0.0 )
      ENDIF
C
C       DR PLOTS
C
      CALL HBOOK1( NHRDTF,    'TF RESID - ALL DR LAYERS (SCALED)$', 50,
     +             -1000.0, 1000.0, 0.0 )
      CALL HBOOK2( NHDRT0, 'Axial abs Resid vs. Drift dist.',
     +      50, -1.0, 1.0,100,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHDRT0+1, 'Stereo abs Resid vs. Drift dist.',
     +      50, -1.0, 1.0,100,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHPTT0, 'PT abs Resid vs. Drift dist.',
     +      50, -1.0, 1.0,100,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHVDT0, 'VD abs Resid vs. Drift dist.',
     +      50, -1.0, 1.0,100,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHCOR3, 'Correlated DR resid (0.3 - 0.5 GeV)',
     +      40, -1000., 1000.,40,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHCOR1, 'Correlated DR resid (p > 1 GeV)',
     +      40, -1000., 1000.,40,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHCOPT, 'Correlated PT resid (0.3 - 0.5 GeV)',
     +      40, -1000., 1000.,40,-1000.0,1000.0,0.0)
      CALL HBOOK2( NHCOVD, 'Correlated VD resid (0.3 - 0.5 GeV)',
     +      40, -1000., 1000.,40,-1000.0,1000.0,0.0)
      DO 30 I = 1, NCANDR
          CTIT = 'TF RESIDUAL - DR LAYER '
          IE   = LENOCC( CTIT ) + 1
          WRITE( LN, 1000 ) I
          CALL HBOOK1( NHRDTF+I, CTIT(1:IE)//LN//'$',               50,
     +                  -500.0,  500.0, 0.0 )
30    CONTINUE
C
      CALL HBOOK1( NHDRFT, 'NUMBER OF DR HITS USED IN FIT$',        52,
     +             0.5, 52.5, 0.0 )
      CALL HBOOK1( NHDAFT, 'NUMBER OF DR AXIAL HITS USED IN FIT$',  40,
     +             0.5, 40.5, 0.0 )
      CALL HBOOK1( NHDSFT, 'NUMBER OF DR STEREO HITS USED IN FIT$', 20,
     +             0.5, 10.5, 0.0 )
      CALL HBOOK1(NHDRBD, 'TF LAYER NUKED IN TF3FIT$', NLYRTF,
     +             0.5, NLYRTF+0.5, 0.0 )
      CALL HBOOK1(NHDRBD+1, 'TF LAYER NUKED IN TF3FIT$', NLYRTF,
     +             0.5, NLYRTF+0.5, 0.0 )
C
      CALL HBOOK1( NHQFDR, 'PULSE HEIGHT ON DR ANODE LAYERS$',      50,
     +                 0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHDWBF, 'DR DWCD BEFORE FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NHDWAF, 'DR DWCD AFTER  FULL CORRECTIONS$',      60,
     +                 0.0,    1.2, 0.0 )
      CALL HBOOK1( NTIMDR, 'DR axial times', 100, -50000.,450000.,0.0)
C
C       ERROR MATRIX
C
C      CALL HBOOK1( NHERCU, 'SIGMA KAPPA FROM TF3FIT$',              50,
C     +             0.0, 0.1, 0.0 )
C      CALL HBOOK1( NHERFI, 'SIGMA PHI FROM TF3FIT$',                50,
C     +             0.0, 0.1, 0.0 )
C      CALL HBOOK1( NHERD0, 'SIGMA D0 FROM TF3FIT$',                 50,
C     +             0.0, 0.01, 0.0 )
C      CALL HBOOK1( NHERTD, 'SIGMA TANDIP FROM TF3FIT$',             50,
C     +             0.0, 0.1, 0.0 )
C      CALL HBOOK1( NHERZ0, 'SIGMA Z0 FROM TF3FIT$',                 50,
C     +             0.0, 0.05, 0.0 )
C      CALL HBOOK1( 520, 'SIGMA PHI6 FROM TF3FIT$',                  50,
C     +             0.0, 0.005, 0.0 )
C
C       VD AND DR CATHODE HISTOGRAMS
C
      IF( LUSECT ) THEN
      CALL HBOOK1( NHCTDS,
     +    'TIME ON EACH VD CATHODE - CORRELATED ANODE (NSEC)$',     50,
     +     -25.0, 100.0, 0.0 )
      CALL HBOOK1( NHCTDP,
     +    'TIME ON PEAK VD CATHODE - CORRELATED ANODE (NSEC)$',     50,
     +     -25.0, 100.0, 0.0 )
      CALL HBOOK1( NHCTQS, 'CHARGE ON VD CATHODE STRIP$',           50,
     +             0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHCTQP, 'CHARGE ON PEAK VD CATHODE STRIP$',      50,
     +             0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHCTQC, 'TOTAL CHARGE ON VD CATHODE CLUSTER$',   50,
     +             0.0, 5000.0, 0.0 )
      CALL HBOOK1( NHCTNS, 'NUM OF VD CATHODE STRIPS IN CLUSTER$',  50,
     +             0.0, 10.0, 0.0 )
      CALL HBOOK1( NHRCTF,   'VD CATHODE RESIDUAL: ALL LAYERS$',    50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHRCTF+1, 'VD CATHODE RESIDUAL: LAYER 1$',       50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHRCTF+2, 'VD CATHODE RESIDUAL: LAYER 2$',       50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHCNFT, 'NUM OF VD CATHODE HITS USED IN FIT$',   12,
     +             0.5, 3.5, 0.0 )
*
* DR cathodes
*
      CALL HBOOK1( NHDCDS,
     +    'TIME ON EACH DR CATHODE - CORRELATED ANODE (NSEC)$',     50,
     +     -25.0, 100.0, 0.0 )
      CALL HBOOK1( NHDCDP,
     +    'TIME ON PEAK DR CATHODE - CORRELATED ANODE (NSEC)$',     50,
     +     -25.0, 100.0, 0.0 )
      CALL HBOOK1( NHDCQS, 'CHARGE ON DR CATHODE STRIP$',           50,
     +             0.0, 25000.0, 0.0 )
      CALL HBOOK1( NHDCQP, 'CHARGE ON PEAK DR CATHODE STRIP$',      50,
     +             0.0, 25000.0, 0.0 )
      CALL HBOOK1( NHDCQC, 'TOTAL CHARGE ON DR CATHODE CLUSTER$',   50,
     +             0.0, 25000.0, 0.0 )
      CALL HBOOK1( NHDCNS, 'NUM OF DR CATHODE STRIPS IN CLUSTER$',  50,
     +             0.0, 10.0, 0.0 )
      CALL HBOOK1( NHRDCF,   'DR CATHODE RESIDUAL: ALL LAYERS$',    50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHRDCF+1, 'DR CATHODE RESIDUAL: LAYER 1$',       50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHRDCF+2, 'DR CATHODE RESIDUAL: LAYER 2$',       50,
     +             -5000.0, 5000.0, 0.0 )
      CALL HBOOK1( NHDCFT, 'NUM OF DR CATHODE HITS USED IN FIT$',   12,
     +             0.5, 3.5, 0.0 )
      ENDIF
C
C      CALL HBOOK1( 401, 'SUM OF DRFT DISTANCE, LAYERS 1-5$',        50,
C     +              0.25, 0.75, 0.0 )
C      CALL HBOOK1( 402, 'DIF OF DRFT DISTANCE, LAYERS 1-5$',        50,
C     +             -0.25, 0.25, 0.0 )
C      CALL HBOOK1( 403, 'SUM OF DRFT DISTANCE, LAYERS 6-10$',       50,
C     +              0.25, 0.75, 0.0 )
C      CALL HBOOK1( 404, 'DIF OF DRFT DISTANCE, LAYERS 6-10$',       50,
C     +             -0.25, 0.25, 0.0 )
C      CALL HBOOK1( 405, 'SUM OF DRFT DISTANCE, LAYERS 1-10$',       50,
C     +              0.25, 0.75, 0.0 )
C      CALL HBOOK1( 406, 'DIF OF DRFT DISTANCE, LAYERS 1-10$',       50,
C     +             -0.25, 0.25, 0.0 )
C
C      *** MONTE CARLO PLOTS HAVE NUMBER 5NN ***
C
C      CALL HBOOK1( NHMCX2, 'CHI2/DOF FOR 3D FIT$',                  50,
C     +             0.0, 10.0, 0.0 )
C      CALL HBOOK1( NHMCPR, 'CHI2 PROB OF 3D FIT$',                  50,
C     +             0.0,  1.0, 0.0 )
C      CALL HBOOK1( NHMCRS, 'RESIDUAL FROM TF3FIT$',                 50,
C     +             -1000.0, 1000.0, 0.0 )
C      CALL HBOOK1( NHMCCU, 'KAPPA(MEAS) - KAPPA(START)$',           50,
C     +             -0.005, 0.005, 0.0 )
C      CALL HBOOK1( NHMCFI, 'PHI(MEAS) - PHI(START)$',               50,
C     +             -0.0075, 0.0075, 0.0 )
C      CALL HBOOK1( NHMCD0, 'D0(MEAS) - D0(START)$',                 50,
C     +             -0.00125, 0.00125, 0.0 )
C      CALL HBOOK1( NHMCTD, 'TANDIP(MEAS) - TANDIP(START)$',         50,
C     +             -0.02, 0.02, 0.0 )
C      CALL HBOOK1( NHMCZ0, 'Z0(MEAS) - Z0(START)$',                 50,
C     +             -0.0025, 0.0025, 0.0 )
C      CALL HBOOK1( NHMCAP, 'ABS(P)/EBEAM$',                         50,
C     +             0.09, 1.1, 0.0 )
C
C      MUPAIR
C
C      CALL HBOOK1( NHMCPM, 'ABS(P)/EBEAM -- MUPAIR$',               50,
C     +             0.09, 1.1, 0.0 )
C      CALL HBOOK1( NHMCDM, 'D0   --  MUPAIR$',                      50,
C     +             -0.00125, 0.00125, 0.0 )
C      CALL HBOOK1( NHMCSM, 'D0(1) + D0(2)   --  MUPAIR$',           50,
C     +             -0.00125, 0.00125, 0.0 )
C
C      MORE MASSES
C
C      CALL HBOOK1( NHMD0,  'D0 MASS CUTTING ON M(KPIPI) - M(KPI)$', 50,
C     +             1.765, 1.965, 0.0 )
C      CALL HBOOK1( NHMDIF, 'MASS DIFFERENCE, CUTTING ON M(KPI)$',   50,
C     +             0.139, 0.164, 0.0 )
C SCT
C 60,  -1.0,    0.03333,  60,  -1.0,   0.02,   100,  5,
C DRIFT CHAMBER HITS
C STP
1000  FORMAT(I2)
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"

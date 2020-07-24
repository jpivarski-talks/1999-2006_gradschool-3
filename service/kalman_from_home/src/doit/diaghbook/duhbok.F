*
* $Id: duhbok.F,v 1.3 2001/11/20 17:55:20 dpp Exp $
*
* $Log: duhbok.F,v $
* Revision 1.3  2001/11/20 17:55:20  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.2  2001/11/19 23:05:45  dpp
*      -> delete unused include file
*      -> delete unused variables
*
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

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"

      CHARACTER*2  LN
      CHARACTER*80 CTIT
      INTEGER I,IE,LENOCC

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
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

C
C       MISCELLANEOUS
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
1000  FORMAT(I2)
      END

*
* $Id: doitnl.F,v 1.9 2003/03/05 18:10:00 dpp Exp $
*
* $Log: doitnl.F,v $
* Revision 1.9  2003/03/05 18:10:00  dpp
*      -> remove unused variables, qcrlct,qpkmct,qminct,etc. ,sigllf
*          we are down to only 21 variable defined in the namelist,
*           let's keep up the good work
*
* Revision 1.8  2002/09/05 19:21:42  dpp
*      -> remove LWHIST,LCFHST,LLFHST
*
* Revision 1.7  2001/11/20 00:03:27  dpp
*      -> remove unused varaiables
*      -> delete unused include file
*
* Revision 1.6  2000/12/20 00:28:12  dpp
* apparently someone deleted the cvssrc/Klmn3Fort/
*
* Revision 1.5  1999/11/29 21:43:56  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.4  1999/11/03 22:38:22  dpp
* change cathode ph threshold cuts
*
* Revision 1.3  1999/08/03 21:56:48  dpp
* change z cut, YHTCUT, from 1.2 meter to 1 naut
*
* Revision 1.2  1998/07/09 00:54:30  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:36  clib
* Developmental version of DUET.
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE DOITNL
C.......................................................................
C.
C. DOITNL - Set NAMELIST defaults and read
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.05
C. CREATED   : ?
C. LAST MOD  : 25-Oct-88
C.
C. Modification Log.
C.
C. 06-Aug-87 : Update DR2 defaults and print warning if defaults used
C. 20-Dec-87 : Add CF/LF histogram logicals to NAMELIST      R. Namjoshi
C. 25-Oct-88 : Loosed CFIND CHI2's way out for DRII.                 dsr
C. 08-Apr-89 : Add LUSEPT.                                           SFS
C. 08-Sep-89 : Set default pulse height cut to 0 (temp for MC)       SFS
C. 22-Apr-91 : Make reading of namelist optional                     SFS
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C== External function declarations
C
      LOGICAL  INTRAC
      EXTERNAL INTRAC

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tffitctl.inc"
#include "doit/duseq/tfxfitcd.inc"
#include "seq/clutil/mesrep.inc"
C#include "Klmn3Fort/klmnctrl.inc"
#include "doit/duklseq/duklmncd.inc"

      CHARACTER*10 FNML
#if defined(CLEO_IBM)
      CHARACTER*8 CDUMMY
      LOGICAL     LEXIST
#endif
      LOGICAL     LDEF
      INTEGER IERR

      NAMELIST/DUENML/
     +          LCOMPR,LU3DTF,
     +          ERRCAN,
     +          YHTCUT,
     +          SIGLCF,NDELTF,DCH2TF,IMNVTF,
     +          XSCMIN,
     +          LSLEAC, LUSVDE,
     +          LCALIB, DUALTR, LKN6FI, LRMVHT, LKN7CT, LADVDE,
     +          LCFXLI,
     +          ISVDPR,
     +          LKNPRC,
     +          LSMHRA

* ----------Executable code starts here---------------------


      LDEF = .TRUE.

C== Device selection initialization according to geometry setup


      CALL VFILL( LU3DTF, KLYRTF, .TRUE. )

C== Initialize print flags

      LDEBUG = .FALSE.
C                   debug CFCFIT 
      LCOMPR = .TRUE.
C                 ! MAKE DGHIST HISTOS FOR COMPRESS
C
C== Initialize various cuts

      ERRCAN = 1.2
C                 ! SCALE FACTOR TO KILL HITS IN CDCANH
      SIGLCF = 2.0
C                 ! SIG TO PICK UP .NOT. LUSETF HITS

      YHTCUT = 1852.000
C                 ! L USED IN DEFINING ALLOWED HIT LIST IN ST LYRS
C                   7/30/99 now set to 1 nautical mile, dpp

      NDELTF = 3
C                 ! MAX # OF HITS TO DELETE IN TF3FIT

      DCH2TF = 0.05
C                 ! DELTA(CHI2) BETWEEN ITERATIONS IN TF3FIT
      IMNVTF = 2
C                 ! use diagonal cov matrix with post-fit correction

      XSCMIN = 0.00
C                 ! LKNKTF = XSCATF(2).GT.XSCMIN




      LSLEAC = .FALSE.

C 11/14/96 ADF4 Add check for VD E/W swapping at 81206.
      LUSVDE = .FALSE.

      LCALIB = .FALSE.
      DUALTR = .FALSE.
      LKN6FI = .FALSE.
      LRMVHT = .FALSE.
      LKN7CT = .FALSE.
      LADVDE = .FALSE.
      LCFXLI = .TRUE.

      ISVDPR              = -3

      LKNPRC              = .FALSE.


      LSMHRA              = .FALSE.

      IF (LNAME) THEN
C== Read NAMELIST

C== NAMELIST FILE NAME
         FNML   = 'DOIT.NML'

#if defined(CLEO_IBM)
         FNML   = 'DUENML'
         CALL QDDNAM(FNML,LEXIST,CDUMMY,IERR)
         IF(.NOT.LEXIST) GOTO 90
#endif
         IUNTMP = 21
         CALL GETLUN(IUNTMP,'TFUNIN')

         OPEN(UNIT=IUNTMP,FILE=FNML,IOSTAT=IERR,STATUS='OLD')
         IF( IERR.NE.0 ) GOTO 90
C== File was read => defaults were not used
         LDEF = .FALSE.
         READ(UNIT=IUNTMP,NML=DUENML,END=80)

   80    CONTINUE
         CLOSE(UNIT=IUNTMP)
      END IF

   90 CONTINUE



C== Temporary: Nov/95. RKK
C== Disable DGHIST histos if Illinois pat. rec. is used.
      IF ( (ISVDPR .GE. 0 .OR. ISVDPR .EQ.-3 )
     +    .AND. LCOMPR ) THEN
          CHMESS = 
     +    'DG histograms temporarily disabled due to SVD '//
     +    'teething problems'
          IERR = 2
          CALL MESLOG('DOITNL',IERR,MSINFO)
          LCOMPR = .FALSE.
      ENDIF


C== Decide whether we need to open a lineprinter output file
      LOPEN = .FALSE.

C== Initialize logical unit numbers
      CALL TFUNIN

      IF (LDEF .AND. LNAME) THEN
          WRITE(CHMESS,1000,IOSTAT=IERR) FNML
          IERR = 1
          CALL MESLOG('DOITNL',IERR,MSINFO)
          PRINT 1000, FNML
      END IF

      RETURN

1000  FORMAT(' DOITNL: Using defaults, unable to open NAMELIST file ',
     +       A )
      END





*
* $Id: doitnl.F,v 1.6 2000/12/20 00:28:12 dpp Exp $
*
* $Log: doitnl.F,v $
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
C. 09-Dec-87 : Turn off LPLOT if running in batch            R. Namjoshi
C. 20-Dec-87 : Add CF/LF histogram logicals to NAMELIST      R. Namjoshi
C. 17-Oct-88 : Add LPLOT2 for seed track plots.                      dsr
C. 25-Oct-88 : Loosed CFIND CHI2's way out for DRII.                 dsr
C. 08-Apr-89 : Add LUSEPT.                                           SFS
C. 08-Sep-89 : Set default pulse height cut to 0 (temp for MC)       SFS
C. 14-Sep-89 : Add TMINPT and TMAXPT to namelist
C. 22-Apr-91 : Make reading of namelist optional                     SFS
C. 16-Feb-97 : Add SVDPR SVTF-DOIT linking chisquare cuts            MAP
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

#include "doit/duseq/tfctlpar.inc"
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
#include "doit/duklseq/svtfcd.inc"

      CHARACTER*10 FNML
#if defined(CLEO_IBM)
      CHARACTER*8 CDUMMY
      LOGICAL     LEXIST
#endif
      LOGICAL     LDEF
      INTEGER IERR

      NAMELIST/DUENML/ LUSEIZ,LUSEIV,LUSEPT,LUSEVD,LUSEDR,LUSECT,LUSQD1,
     +          LUSSV1,LPRNT1,LPRNT2,LPRNT3,LPRNT4,LPRNT5,LPRNT6,LPRNT7,
     +   LTYPE ,LPRINT,LPRINH,LPLOT ,LPTRKS,LPLNKS,LPETRS,LPHITS,LPLHTS,
     +   LPWLST,LPSTAK,LP3DT ,LWSUM ,LWTIME,LWHIST,LGPADR,LPRERR,
     +          LPROUT,LCOMPR,LU3DTF,LCFHST,LLFHST,LANHST,LPLOT2,LDIVID,
     +          NDRPCF,GMINCF,SGYSCF,DIFSCF,D0MAX1,D0MAX2,ERRCAN,
     +          ERRHCF,CH2CF1,CH2CF2,CH2CFD,CH2CD1,CH2CD2,
     +          NDRPLF,GMINLF,YHTCUT,SGYSLF,DIFSLF,YMXCUT,
     +          Y0MIN ,Y0MIN1,Y0MIN2,Y0MAX ,Y0MAX1,Y0MAX2,
     +          ERRHLF,ERRDLF,CH2LF1,CH2LF2,CH2LFD,Z0BIAS,CHIFUZ,
     +          SIGLCF,SIGLLF,NDELTF,DCH2TF,IMNVTF,
     +          TCRLCT,QPKMCT,QMINCT,TCRLDC,QPKMDC,QMINDC,
     +          XSCMIN,T0BHTF,
     +          ADDSVT, DO5SVT, DO4SVT, CKALLSVT, RANKSVT, 
     +          LNKSVT,
     +          PHMNIZ, PHMNIV, PHMNVD, PHMNDR, PHMNVC, PHMNDC,
     +          PHMNPT, PHMNPC,
     +          TMINIZ, TMINIV, TMINVD, TMINDR, TMINVC, TMINDC,
     +          TMAXIZ, TMAXIV, TMAXVD, TMAXDR, TMAXVC, TMAXDC,
     +          TMINPT, TMAXPT, RCCHOP, RLCHOP, LCFXLA, LCFXPR,
     +          LAB1TF, LKALMN, LVTEST, LSLEAC, LUSVDE,
     +          LCALIB, DUALTR, LKN6FI, LRMVHT, LKN7CT, LADVDE,
     +          LCFXLI,
     +          LTRTAB, ITWRIT, SINGLT, ISVDPR, LUSSCF, KLPART,
     +          LTTSVD, LSVDUF, LTKLOG, LKNPRC, LTSTSD, LEXTFX,
     +          LSMOOT, LRESID, LHIDEL, LRESHT, JSVDPR, LWHITS,
     +          LDUMPG, LSMHRA, LYRKIL

C     +          KLMNCTRL_NOMS,KLMNCTRL_NOELOSS,KLMNCTRL_NOSTRAGGLE,

* ----------Executable code starts here---------------------


      LDEF = .TRUE.
C
C== Device selection initialization according to geometry setup
C
      LUSSV1 = .TRUE.
      LUSEDR = .TRUE.
      LUSEIZ = .FALSE.
C      LUSEIZ = NCANIZ.GT.0
      LUSEIV = (NCANIV.GT.0).AND.(IDTOCD(IIVDT).GT.0)
      LUSEPT = (NCANPT.GT.0).AND.(IDTOCD(IPTLA).GT.0)
      LUSEVD = (NLYRVD.GT.0).AND.(IDTOCD(IVDET).GT.0)
      LUSECT = (NCATCD.GT.0).AND.((IDTOCD(IVDCT).GT.0).
     +      OR.(IDTOCD(IDRCT).GT.0).OR.(IDTOCD(IPTLC).GT.0))

      CALL VFILL( LU3DTF, KLYRTF, .TRUE. )
C
C== Initialize print flags
C
      LDEBUG = .FALSE.
      LPRNT1 = .FALSE.
C                 ! PRINT PICTURES OF LINKS
      LPRNT2 = .FALSE.
C                 ! PRINT PICTURES OF TREES
      LPRNT3 = .FALSE.
C                 ! PRINT THE LINK & TREE CUTS
      LPRNT4 = .FALSE.
      LPRNT5 = .FALSE.
      LPRNT6 = .FALSE.
      LPRNT7 = .FALSE.
      LTYPE  = .FALSE.
C                 ! TYPEOUT PER EVENT
      LPRINT = .FALSE.
C                 ! PRINTOUT IF INPUT & OUTPUT TRACKS
      LPRINH = .FALSE.
C                 ! PRINTOUT OF OUTPUT HITS & ADDRESSES
      LDIVID = .FALSE.
C                 ! PRODUCE SINGLE-WIDTH OUTPUT FILES
      LPROUT = .FALSE.
C                 ! PRINT ROUTINE OUTPUT
      LPTRKS = .FALSE.
C                 ! PRINT THE COMPLETE CHAINS
      LPLNKS = .FALSE.
C                 ! PRINT THE LINK LIST
      LPETRS = .FALSE.
C                 ! PRINT THE ELEMENTARY TREE LIST
      LPHITS = .FALSE.
C                 ! PRINT THE HITS FOR THIS TRACK
      LPLHTS = .FALSE.
C                 ! PLOT THE HITS ON LPT
      LPSTAK = .FALSE.
C                 ! PRINT THE TRACK STACK
      LP3DT  = .FALSE.
C                 ! PRINT THE 3D FITTED TRACK
      LPWLST = .FALSE.
C                 ! PRINT THE COMPLETE HIT WIRE LIST
C
      LWSUM  = .FALSE.
C                 ! PRINT THE DOIT SUMMARY
      LWTIME = .FALSE.
C                 ! PRINT THE DOIT TIMES
      LWHIST = .FALSE.
C                 ! PRINT THE HISTOGRAMS
      LCOMPR = .TRUE.
C                 ! MAKE DGHIST HISTOS FOR COMPRESS
      LCFHST = .FALSE.
      LLFHST = .FALSE.
      LANHST = .FALSE.
      LGPADR = .FALSE.
C                 ! WRITE WIRE ADDRESSES ON OUTPUT TAPE
      LPRERR = .FALSE.
C                 ! WRITE ERROR BUFFER
C
C
C== Initialize various cuts
C
      NDRPCF = 2
C                 ! # OF HITS TO DROP IN 1ST CFIND
      GMINCF = 0.05
C                 ! MINIMUM GAP FOR HITS IN THE SAME CFIND LAYER
      SGYSCF = 3.0
C                 ! SCALE FACTOR FOR SIGMA(LINK) FOR D0 OF LINK
      ERRHCF = 1.0
C                 ! SCALE FACTOR FOR CFIND  RESOLUTION
      ERRCAN = 1.2
C                 ! SCALE FACTOR TO KILL HITS IN CDCANH
      CH2CF1 =  3.0
C                 ! CHI2 CUT OF 1ST CFIND
      CH2CF2 = 25.0
C                 ! CHI2 CUT OF 2ND CFIND
      CH2CD1 =  2.0
C                 ! CHI2 CUT OF 1ST CFDRDG
      CH2CD2 = 10.0
C                 ! CHI2 CUT OF 2ND CFDRDG
      CH2CFD = 20.0
C                 ! CHI2 CUT OF LAST CFIND IN DREDGE
      DIFSCF = 3.0
C                 ! SCALE FACTOR FOR SIGMA(TREE) FOR DIF IN CUR
      IF( LUSEVD ) THEN
          SIGLCF = 6.0
C                 ! SIG TO PICK UP .NOT. LUSETF HITS
      ELSE
          SIGLCF = 2.0
      ENDIF
      D0MAX1 = 0.05
C                 ! MAXIMUM D0 ACCEPTED
      D0MAX2 = 0.10
C                 ! MAXIMUM D0 ACCEPTED FOR CFDRDG TRACKS
C
      ERRHLF = 1.0
C                 ! SCALE FACTOR FOR LFIND  RESOLUTION
      ERRDLF = 1.0
C                 ! SCALE FACTOR FOR TF3FIT RESOLUTION
      NDRPLF = 2
C                 ! # OF HITS TO DROP IN 1ST LFIND
      CH2LF1 = 2.0
C                 ! CHI2 CUT OF 1ST LFIND
      CH2LF2 = 3.5
C                 ! CHI2 CUT OF 2ND LFIND
      CH2LFD = 10.0
C                 ! CHI2 CUT OF LAST LFIND IN DREDGE
      YHTCUT = 1852.000
C                 ! L USED IN DEFINING ALLOWED HIT LIST IN ST LYRS
C                   7/30/99 now set to 1 nautical mile, dpp
      DIFSLF = 3.0
C                 ! SCALE FACTOR FOR SIGMA(TREE) FOR DIF IN TANDIP
      GMINLF = 0.05
C                 ! MINIMUM GAP FOR HITS IN THE SAME LFIND LAYER
      SGYSLF = 3.0
C                 ! SCALE FACTOR FOR SIGMA(LINK) FOR Z0 OF LINK
      SIGLLF = 2.0
C                 ! # OF SIGMA FOR PICKING UP .NOT. LUSETF HITS
      Y0MIN1 = -0.10
C                 ! MINIMUM AND MAXIMUM Y0 FOR SEED   TRACKS
      Y0MAX1 =  0.10
      Y0MIN2 = -0.20
C                 ! MINIMUM AND MAXIMUM Y0 FOR CFDRDG TRACKS
      Y0MAX2 =  0.20

      Z0BIAS = 0.05
C                 ! Z0 ERROR USED IN BIASED CHI2 IN LFLFIT
      CHIFUZ = 2.0
C                 ! WIDTH OF FUZZY CUT IN TFSORT (USED TO TURN ON BIASED CHI2)
      RCCHOP = 10.0
C                 ! Radial dist. beyond "escape point" for which hits accepted
      RLCHOP = 0.05
C                 ! Z dist. beyond "escape point" for which hits accepted
      NDELTF = 3
C                 ! MAX # OF HITS TO DELETE IN TF3FIT
      DCH2TF = 0.05
C                 ! DELTA(CHI2) BETWEEN ITERATIONS IN TF3FIT
      IMNVTF = 2
C                 ! use diagonal cov matrix with post-fit correction
      TCRLCT = 12000000.
C                 ! MAX DELTA T BETWEEN CATHODE AND CORRELATED LYR
      QPKMCT = 60.
C                 ! MIN CHARGE OF PEAK STRIP IN CATHODE CLUSTER
      QMINCT = 20.
C                 ! MIN CHARGE OF EACH STRIP IN CATHODE CLUSTER
      TCRLDC = 12000000.
C                 ! MAX DELTA T BETWEEN CATHODE AND CORRELATED LYR
      QPKMDC = 50.
C                 ! MIN CHARGE OF PEAK STRIP IN CATHODE CLUSTER
      QMINDC = 30.
C                 ! MIN CHARGE OF EACH STRIP IN CATHODE CLUSTER
      YMXCUT = 10.0
C                 ! CUT IN POSITION OF TRACK AT MAX RAD OF TRACK
      XSCMIN = 0.00
C                 ! LKNKTF = XSCATF(2).GT.XSCMIN
      T0BHTF = 0
C                 ! T0 CORRECTION FOR BHABHAS (DIRTY, DIRTY)
C
C== SVTF to DOIT linking control information 
C== (see doit/duklseq/svtfcd.inc)
C== MAP  02/16/97

C     Add SVTF track segments to DOIT CD tracks
      AddSVT   = .TRUE.

C     Use 5-hit SVTF track segments
      Do5SVT   = .TRUE.

C     Use 6-hit SVTF track segments
      Do4SVT   = .TRUE.

C     Check all SVTF track segments
      CkAllSVT = .TRUE.

C     Rank all SVTF track segments
      RankSVT  = .TRUE.

C     Chisquare cuts for use in SVDPR
C     These can be specified in the DOIT.NML file as a comma 
C     separated list.                          
      LnkSVT(1) =  25.
      LnkSVT(2) = 100.
      LnkSVT(3) = 200.

C== Initialize raw hit pulse height and timing cuts
C
C      PHMNIZ = 10.0
C      PHMNIV = 25.0
C      PHMNVD = 25.0
C      PHMNDR = 1000.0
C      PHMNVC = 20.0
C      PHMNDC = 1000.0
C      PHMNPT = 50.0
C      PHMNPC = 20.0

      PHMNIZ = 0.0
      PHMNIV = 0.0
      PHMNVD = 200.0
      PHMNDR = 50.0
      PHMNVC = -10000.0
      PHMNDC = -10000.0
      PHMNPT = 100.0
      PHMNPC = 0.0
*
      CALL VFILL( TMINCD, MXDVCD, -1.0E8 )
      CALL VFILL( TMAXCD, MXDVCD, 1.0E8 )
      TMINPT = -20000.
      TMINVD = -20000.
      TMINDR = -20000.

C
      LCFXLA = .TRUE.
      LCFXPR = .TRUE.
      LAB1TF = .TRUE.

C     The following must be initialized in DOITBJ since they can be
C     modified by a KUIP command.
C     LKALMN = .FALSE.
C     LWHITS = .FALSE.

      LVTEST = .TRUE.
      LSLEAC = .FALSE.
C 11/14/96 ADF4 Add check for VD E/W swapping at 81206.

      IF ( NRUN .LT. NRUNVE ) THEN
         LUSVDE = .FALSE.
      ELSEIF (NRUN .LT. NRNVDS) THEN
         LUSVDE = .TRUE.
      ELSE
         LUSVDE = .FALSE.
      ENDIF
      LCALIB = .FALSE.
      DUALTR = .FALSE.
      LKN6FI = .FALSE.
      LRMVHT = .FALSE.
      LKN7CT = .FALSE.
      LADVDE = .FALSE.
      LCFXLI = .TRUE.

C      KLMNCTRL_NOMS       = .FALSE.
C      KLMNCTRL_NOELOSS    = .FALSE.
C      KLMNCTRL_NOSTRAGGLE = .FALSE.

      LTRTAB              = .FALSE.
      ITWRIT              = 0
      SINGLT              = .FALSE.
      ISVDPR              = -3
      JSVDPR              = -1
      LUSSCF              = .TRUE.
      KLPART              = 0
      LTTSVD              = .FALSE.
      LSVDUF              = .FALSE.
      LTKLOG              = .FALSE.
      LKNPRC              = .FALSE.
      LTSTSD              = .FALSE.
      LEXTFX              = .TRUE.
      LSMOOT              = .TRUE.
      LRESID              = .TRUE.
      LHIDEL              = .TRUE.
      LRESHT              = .FALSE.
      LDUMPG              = .FALSE.
      LSMHRA              = .FALSE.
      LYRKIL              = 0

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

C== Undo crazy device selections
C

      LUSEDR = LUSEDR .AND. (NCAXDR.GT.0) .AND. (NCSTDR.GT.0)
      LUSEIZ = .FALSE.
C      LUSEIZ = LUSEIZ .AND. (NCANIZ.GT.0)
      LUSEIV = LUSEIV .AND. (NCANIV.GT.0) .AND. (IDTOCD(IIVDT).GT.0)
      LUSEPT = LUSEPT .AND. (NCANPT.GT.0) .AND. (IDTOCD(IPTLA).GT.0)
      LUSEVD = LUSEVD .AND. (NLYRVD.GT.0) .AND. (IDTOCD(IVDET).GT.0)
      LUSECT = LUSECT .AND. (NCATCD.GT.0) .AND.((IDTOCD(IVDCT).GT.0)
     +      .OR.(IDTOCD(IDRCT).GT.0).OR.(IDTOCD(IPTLC).GT.0))
      LUSVDE = LUSVDE .AND. LUSEVD


C== Turn off plotting if running in batch or on the IBM

#if defined(CLEO_IBM)
      LPLOT  = .FALSE.
      LPLOT2 = .FALSE.
#endif
#if defined(CLEO_VAX)
      LPLOT  = ( LPLOT  .AND. INTRAC() )
      LPLOT2 = ( LPLOT2 .AND. INTRAC() )
#endif

C== If we're making DGHIST histo's, don't make HBOOK ones
      LWHIST = LWHIST.AND..NOT.LCOMPR

C== Temporary: Nov/95. RKK
C== Disable DGHIST histos if Illinois pat. rec. is used.
      IF ( (ISVDPR .GE. 0 .OR. JSVDPR .GE.0 .OR. ISVDPR .EQ.-3 )
     +    .AND. LCOMPR ) THEN
          CHMESS = 
     +    'DG histograms temporarily disabled due to SVD '//
     +    'teething problems'
          IERR = 2
          CALL MESLOG('DOITNL',IERR,MSINFO)
          LCOMPR = .FALSE.
      ENDIF

C== Turn OFF histograms if LWHIST is .FALSE.

      LCFHST = LCFHST.AND.LWHIST
      LLFHST = LLFHST.AND.LWHIST
      LANHST = LANHST.AND.LWHIST

C== Decide whether we need to open a lineprinter output file
      LOPEN = LPWLST.OR.LPRINT.OR.
     +      LP3DT.OR.LPRERR.OR.LPETRS.OR.LPHITS.OR.LPLNKS.OR.
     +      LPSTAK.OR.LPTRKS.OR.LWHIST.OR.
     +      LPLHTS.OR.LWSUM.OR.LWTIME.OR.LPRNT1.OR.LPRNT2
     +      .OR.LPRNT3.OR.LPRNT4.OR.LPRNT5.OR.LPROUT
C== Initialize logical unit numbers
      CALL TFUNIN

      IF (LDEF .AND. LNAME) THEN
          WRITE(CHMESS,1000,IOSTAT=IERR) FNML
          IERR = 1
          CALL MESLOG('DOITNL',IERR,MSINFO)
          IF (LPROUT) WRITE(IUNLPT,1000) FNML
      END IF
      IF (LPROUT .AND. LNAME) WRITE(UNIT=IUNLPT,NML=DUENML)

      RETURN

1000  FORMAT(' DOITNL: Using defaults, unable to open NAMELIST file ',
     +       A )
      END



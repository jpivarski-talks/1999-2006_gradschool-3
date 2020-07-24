*
* $Id: doit_init_sigma.F,v 1.14 2003/03/05 18:06:27 dpp Exp $
*
* $Log: doit_init_sigma.F,v $
* Revision 1.14  2003/03/05 18:06:27  dpp
*      -> remove unused variables, qcrlct,qpkmct,qminct,etc.
*      -> add new clustering controls
*      -> LCLSTR_LAYER, false for cathode; these are clustered elsewhere
*                       true for TPC
*      -> add flag for multi-hit electronics
*
* Revision 1.13  2002/09/05 19:12:39  dpp
*      -> remove unused sigmas, weights, search widths
*      -> add TPC1
*      -> add DRIFTDIR to distinguish between drift chamber and TPC
*
* Revision 1.12  2001/11/20 00:00:10  dpp
*     -> remove direct access of the event control parameter
*      -> delete unused include file
*      -> add new control parameter include file
*      -> remove print statement
*      -> remove use of errhcf and errhlf
*
* Revision 1.11  2001/09/12 22:04:23  dpp
*      -> change artificial deweight of silicon SZ from .0001 to .001
*         (effective sigma is changed from 3000 microns to 948 microns)
*      ->  structure to change search width for inner 8 axial dr layers
*          (but width set to old standard, =1)
*
* Revision 1.10  2001/05/01 17:27:08  dpp
*      -> change artificial deweight of silicon from .0001 to .01
*         (effective sigma is changed from 1500 microns to 150 microns)
*
* Revision 1.9  2000/12/19 23:32:50  dpp
*      -> add flag for stereo layer
*
* Revision 1.8  2000/11/15 16:59:04  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.7  2000/10/02 20:48:29  dpp
*      -> turn off silicon for calibration
*      -> change artificial deweight of cathode from .0001 to .01
*
* Revision 1.6  2000/07/07 22:07:50  dpp
* change search width for silicon r-phi to 2000 sigmas
* de-weight cathodes, silicon r-phi, silicon z
*
* Revision 1.5  2000/05/26 16:57:01  dpp
*      -> change ROADHITS acceptance road for ISVX1
*
* Revision 1.4  2000/05/17 19:46:06  dpp
*      -> turn on ISVX1 and ISVZ1 devices
*
* Revision 1.3  2000/04/26 15:07:50  dpp
*      -> fix road widths for silicon
*      -> turn off ISVX1 and ISVZ1 devices for the submit
*
* Revision 1.2  1999/11/03 22:47:10  dpp
* turn off clustering flags for silicon
* use QPKMDC instead of the mistyped QPKMCT for the drift chamber cathode
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE DOIT_INIT_SIGMA
C.......................................................................
C.
C. DOIT_INIT_SIGMA - Initialize quantities for trackfinding
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 13-Nov-97
C. 24/05/88  add LSAVTF, LPRFTF. by  D. Riley
C. 25/02/88 DR outer cathode moved to correct radial order by R. Namjoshi
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfconspa.inc"

#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfunitcd.inc"

C NSRWTF..........SEARCH WIDTH WHEN USING THE WIRES ONLY, in SFHITS
C LSAVTF...........TRUE. => THIS LAYER IS USED IN LINKS
C                 This is the default version, used for dredging everything.
C LPRFTF..........This designates the 'preferred' layers for ROADHITS.
C                 Currently all layers except the inner and outer DR
C                 anode layer are preferred.
C L_CALIB.........logical, =.true. if CALIB event control bit is set


      INTEGER ILYR,IDEV
      LOGICAL NEWFWT
      REAL ERRDIZ,SIGTMP,SIGTMPW
      INTEGER TEMP
      LOGICAL L_CALIB 
* ----------Executable code starts here---------------------

      CALL EVENT_CONTROL_BITS(EXB_CALIB,L_CALIB)

C--> New finding weights were first used at the same time as DME
C     was put into the PT.  This flag needs to be revisited.

      NEWFWT = .FALSE.
      IF ( NRUN .GT. NRUNDM ) NEWFWT = .TRUE.

C== Initialize all sigmas and weight

      DO 10 ILYR = 1, NLYRTF
         IDEV = IDVCTF(ILYR)

         IF( IDEV.EQ.IDRFT ) THEN
            IF (ABS(TANSTF(ILYR)).LE. 0.005)THEN


               SIGQTF(ILYR) = 9.
               IF(LQDVTF(ILYR)) WGTQTF(ILYR) = 1.0/SIGQTF(ILYR)**2

               NSRWTF(ILYR) = 1

               LSAVTF(ILYR) = .FALSE.
c               IF (I.LT.12) LSAVTF(ILYR) = .TRUE.
               LPRFTF(ILYR) = .TRUE.
c               IF ((I.EQ.1).OR.(I.EQ.NCAXDR)) LPRFTF(ILYR) = .FALSE.

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.

               SIGTMP                               =110.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               IF(IXSFTF(ILYR).LE.8)THEN
                  SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =1
               ELSE
                  SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =1
               ENDIF
               SIGTMPW             =CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =1
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI
               
            ELSE

C--> No new finding weights for DR stereo layers.

               SIGQTF(ILYR) = 9.
               IF(LQDVTF(ILYR)) WGTQTF(ILYR) = 1.0/SIGQTF(ILYR)**2

               NSRWTF(ILYR) = (ABS(DIFSTF(ILYR))+1.1)/2.0

               LSAVTF(ILYR) = .FALSE.
c               IF(I.LT.12)LSAVTF(ILYR) = .TRUE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.

               SIGTMP=110.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =1

               SIGTMPW=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =1

               SIGTMP=110.00E-6/ABS(TANSTF(ILYR))
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =1

               SIGTMPW=SIGTMP
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =1
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.TRUE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI
            ENDIF


         ELSEIF( IDEV.EQ.ITPC1 ) THEN

               SIGQTF(ILYR) = 9.
               IF(LQDVTF(ILYR)) WGTQTF(ILYR) = 1.0/SIGQTF(ILYR)**2

               NSRWTF(ILYR) = 1

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .TRUE.

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.

               SIGTMP                               =100.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =5

               SIGTMP                               =.001
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =1

               SIGTMPW             =CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =1

               SIGTMPW             =.001
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =1

               LCLSTR_LAYER(ILYR)                   =.TRUE.
               MultiHitLayer(ILYR)                  =.TRUE.

C time difference is in pico-sec
C time difference of               ( 1 E+6 ps           )   (1 microsec )
C with drift velocity of           ( X 50 E-9 meters/ps )   (X 50 mm/microsec)
C gives a distance difference of   ( = 50 E-3 meters    )   ( = 50mm)
               CLS_TC_MAX  (ILYR)                   =1 000 000.
               CLS_Q_P_MIN (ILYR)                   =200.
c               CLS_Q_A_MIN (ILYR)                   =20.
               CLS_Q_A_MIN (ILYR)                   =10.
               CLS_F_E_MIN (ILYR)                   =0.51
               CLS_F_C_MAX (ILYR)                   =0.49


               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_Z

         ELSEIF(IDEV.EQ.IZDA)THEN

               SIGQTF(ILYR) = 9.
               IF(LQDVTF(ILYR)) WGTQTF(ILYR) = 1.0/SIGQTF(ILYR)**2

               NSRWTF(ILYR) = (ABS(DIFSTF(ILYR))+1.1)/2.0 +2

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.

               SIGTMP=140.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =4

               SIGTMPW=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =4

               SIGTMP=140.00E-6/ABS(TANSTF(ILYR))
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =4

               SIGTMPW=SIGTMP
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =4

               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.TRUE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI

         ELSEIF(IDEV.EQ.IVDET)THEN

               SIGQTF(ILYR) = 0.013
               IF(LQDVTF(ILYR)) WGTQTF(ILYR) = 1.0/SIGQTF(ILYR)**2

               NSRWTF(ILYR) = 2

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.

               SIGTMP=140.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =2

               SIGTMPW=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =2
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI


         ELSEIF(IDEV.EQ.IIVDT)THEN

               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 2

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.

               SIGTMP=120.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =2

               SIGTMPW=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =2
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI

         ELSEIF(IDEV.EQ.IPTLA)THEN
C--> Make resolution run-dependent (Ar-ethane vs. DME).
C     Hardwiring this will undoubtedly lead to grief someday.

               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 2

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.

               SIGTMP=120.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =2

               SIGTMPW=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =2
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.TRUE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_PHI

         ELSEIF( IDEV.EQ.IVDCT) THEN

               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 0

               LSAVTF(ILYR) = .TRUE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.
               
               SIGTMP=1700.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =0

               SIGTMPW=CELZTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =0
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               CLS_TC_MAX  (ILYR)                   =12 000 000.
               CLS_Q_A_MIN (ILYR)                   =20.
               CLS_Q_P_MIN (ILYR)                   =60.
               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_NONE


         ELSEIF( IDEV.EQ.IDRCT) THEN

               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 0

               LSAVTF(ILYR) = .TRUE.
               LPRFTF(ILYR) = .TRUE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.
               
               SIGTMP=1800.00E-6
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
     2              *.01
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =0

               SIGTMPW=CELZTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =0
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               CLS_TC_MAX  (ILYR)                   =12 000 000.
               CLS_Q_A_MIN (ILYR)                   =30.
               CLS_Q_P_MIN (ILYR)                   =50.
               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_NONE

         ELSEIF( IDEV.EQ.IPROP ) THEN
               ERRDIZ = CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)

               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 0

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .FALSE.

               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.

               SIGTMP=CELLTF(ILYR)*RTF(ILYR)/(2.0*ROOT3)
               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =0

               SIGTMPW=SIGTMP
               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =0
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_NONE

         ELSEIF( IDEV.EQ.ISVX1 ) THEN
 
               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.
 
               NSRWTF(ILYR) = 100
 
               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .FALSE.

C ISVX1 is turned on!
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.FALSE.

               IF(L_CALIB)THEN
                  LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.FALSE.
                  print 9902
 9902             format('             and I am setting Luse_layer F')
               ELSE
                  LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.TRUE.
               ENDIF

               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.

               SIGTMP                               =15.00E-6
               SIGTMPW                              =3.5*SIGTMP

               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_CF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)=1./SIGTMP**2
     2              *.01
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_CF)   =2000

               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_CF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_CF)   =500
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_NONE


          ELSEIF( IDEV.EQ.ISVZ1 ) THEN

               SIGQTF(ILYR) = 9.
               WGTQTF(ILYR) = 0.

               NSRWTF(ILYR) = 100

               LSAVTF(ILYR) = .FALSE.
               LPRFTF(ILYR) = .FALSE.

C ISVZ1 is turned ON!
               LUHTTF(ILYR) = .TRUE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_CF)=.FALSE.
               LUSE_LAYER  (ILYR,LVLTYP_PAT_REC,FITTYP_LF)=.FALSE.

               IF(L_CALIB)THEN
                  LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.FALSE.
               ELSE
                  LUSE_LAYER  (ILYR,LVLTYP_TFXFIT ,FITTYP_LF)=.TRUE.
               ENDIF

               SIGTMP                               =30.00E-6
               SIGTMPW                              =3.5*SIGTMP

               SIGMA_LAYER (ILYR,HITTYP_HIT ,FITTYP_LF)   =SIGTMP
               WEIGHT_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)=1./SIGTMP**2
     2              *.001
               SEARCH_LAYER(ILYR,HITTYP_HIT ,FITTYP_LF)   =50

               SIGMA_LAYER (ILYR,HITTYP_WIRE,FITTYP_LF)   =SIGTMPW
               WEIGHT_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)=1./SIGTMPW**2
               SEARCH_LAYER(ILYR,HITTYP_WIRE,FITTYP_LF)   =1000
               LCLSTR_LAYER(ILYR)                   =.FALSE.
               MultiHitLayer(ILYR)                  =.FALSE.
               LAMB_D_LAYER(ILYR)                   =.FALSE.
               LSTEREOLAYER(ILYR)                   =.FALSE.
               DRIFTDIR(ILYR)                       =DRIFTDIR_NONE
          ENDIF
10    CONTINUE
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12


      RETURN
      END






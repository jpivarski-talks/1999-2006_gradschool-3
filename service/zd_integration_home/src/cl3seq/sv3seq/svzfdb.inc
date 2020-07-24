*
* $Id: svzfdb.inc,v 1.4 1998/07/29 20:57:18 pg Exp $
*
* $Log: svzfdb.inc,v $
* Revision 1.4  1998/07/29 20:57:18  pg
*  All elements numbered from 1 to N. Changed array's declaration to
* reflect this.
*
* Revision 1.3  1998/05/07 20:37:42  pg
*   Modified include files to better represent Si3 geometry
*
* Revision 1.2  1996/09/30 05:48:12  jjo
* Modify svgeompa.inc to use longer variable names per CLEO3 standards
*        SVNSTR -> NSTRIP_SV3, SVLYRS -> NLAYER_SV3, SVMWFR -> NWAFER_SV3
*        NSVWP? -> NWAF_PHI?_SV3, NSVWZ? -> NWAF_Z?_SV3
*        SV_CHAN_ACTIVE  -> NCHAN_ACTIVE_SV3, SV_CHAN_TOTAL  -> NCHAN_TOTAL_SV3
*
* Revision 1.1.1.1  1995/11/01 21:18:37  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:05  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/57 28/08/94  21.06.41  by  Unknown
*-- Author :    Dan Payne   21/08/94
*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
*                        SVX ZFILES constants
*
*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
*
* Electronic calibration constants
* --------------------------------
*
* SVNPED          /I - Number of items
* IPSVPD(SVNPED)  /I - Ptr-ptr to each item of the Zfiles entity:
*                      IPSVPD(1) - Ptr-ptr to pedestals
*                      IPSVPD(2) - Ptr-ptr to pedestal widths
*                      IPSVPD(3) - Ptr-ptr to slopes (gains)
*                      IPSVPD(4) - Ptr-ptr to channel status words
*
      INTEGER    SVNPED
      PARAMETER (SVNPED=4)

      INTEGER        IPSVPD
      COMMON/SVPEDS/ IPSVPD(SVNPED)
      SAVE  /SVPEDS/
*
*
* Common-mode-noise (CMN) for each CAMEX
* --------------------------------------
*
* SVNCMN              /I - Number of items
* SVCMN(0:SVMCMX-1,I) /R - Mean (M) of CMN distribution for each CAMEX
*                          I=1 - Mean
*                          I=2 - Width
c      INTEGER    SVNCMN
c      PARAMETER (SVNCMN=2)

c      REAL           SVCMN
c      COMMON/SVCMNS/ SVCMN(0:SVMCMX-1,SVNCMN)
c      SAVE  /SVCMNS/
*
*
* Thresholds for sparsification
* -----------------------------
*
* SVNTHR  /I - Number of items
* IPSVTH  /I - Ptr-ptr to thresholds
*
      INTEGER    SVNTHR
      PARAMETER (SVNTHR=1)

      INTEGER       IPSVTH
      COMMON/SVTHR/ IPSVTH
      SAVE  /SVTHR/
*
*
* Logical address (LA) --> Electronic address (EA) map
* ----------------------------------------------------
* For each logical address gives the crate number and channel number
* in the crate
*
* SVNMAP         /I - Number of items
* IPSVMP(SVNMAP) /I - Ptr-ptr to each item of entity
*                     IPSVMP(1) - ptr-ptr to crate
*                     IPSVMP(2) - ptr-ptr to channel in crate
*
c      INTEGER    SVNMAP
c      PARAMETER (SVNMAP=2)

c      INTEGER      IPSVMP
c      COMMON/SVMAP/IPSVMP(SVNMAP)
c      SAVE  /SVMAP/

*
*  SVD_POS      - 0's 
*  SVUNIT_POS  - 0's
*  SVLADDER_POS - X,Y displacements and Z-axis rotation.
*  SVWAFER_POS  - Z-displacement within the ladder.
*  SVYROT       - 180 deg. rotation around the Y-axis (FALSE for East(Z<0)
*                 TRUE for West(Z>0))
*  SVZROT       - 180 deg. rotation around the Z-axis (FALSE for Layers 1,3
*                 TRUE for Layers 3,4)


      DOUBLE PRECISION  SVD_POS, SVUNIT_POS
      DOUBLE PRECISION  SVLADDER_POS,SVWAFER_POS
      LOGICAL           SVROT_Y,SVROT_Z
      COMMON /SVPOS/   SVD_POS(SVNOFF),SVUNIT_POS(NLAYER_SV3,SVNOFF),
     &                 SVLADDER_POS(NLADDER_SV3,SVNOFF),
     &                 SVWAFER_POS(NWAFER_SV3,SVNOFF),
     &                 SVROT_Y(NWAFER_SV3),SVROT_Z(NWAFER_SV3)
      SAVE   /SVPOS/

*  Si3 ALIGNMENT constants:
* ----------------------------------
*
*  SVD_OFF      - 1,2,3 - 3 OFFSETS; 4,5,6 - 3 ROTATIONS
*  SVUNIT_OFF  - same for the UNITS
*  SVLADDER_OFF - same for the ladders
*  SVWAFER_OFF  - same for the wafers.
*

      DOUBLE PRECISION  SVD_OFF, SVUNIT_OFF
      DOUBLE PRECISION  SVLADDER_OFF,SVWAFER_OFF
      COMMON /SVOFF/   SVD_OFF(SVNOFF),SVUNIT_OFF(NLAYER_SV3,SVNOFF),
     &                 SVLADDER_OFF(NLADDER_SV3,SVNOFF),
     &                 SVWAFER_OFF(NWAFER_SV3,SVNOFF)
      SAVE   /SVOFF/


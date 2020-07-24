*
* $Id: dutmp1.inc,v 1.1 1998/07/09 02:36:21 lyon Exp $
*
* $Log: dutmp1.inc,v $
* Revision 1.1  1998/07/09 02:36:21  lyon
* Adding duklseq from DPP
*
* Revision 1.7  1996/04/23 23:21:09  zfiles
* Comments in previous version are wrong!
* True comment:
*      Moved some variables to stack.
*      Delete unused variables.
*
* Revision 1.6  1996/04/23 07:28:06  zfiles
* New namelist variables.  Delete unused variables.  Move some variables to
* stack.
* 
* Revision 1.5  1996/03/12 15:01:24  zfiles
* Add map of new tracks to old tracks
*
* Revision 1.4  1995/11/28 18:42:48  zfiles
* Added NTKSVD
*
* Revision 1.3  1995/09/07  23:41:06  zfiles
* Added Id and Log. Added SAVE statements.
*
*
C
C Some more things that need a proper home.
C
C These quantities are parallel to the quantities in cleofitcom.inc
C
C     Layer number and address in DUSCRTCH of this hit.
      INTEGER LAYSAV, IASAV

C     Arc length from origin to the hit.
      REAL SFSAV

C     Other things that need to be saved in order to restore 
C     TFTRAKCD at the end of the fit.  Probably some of these
C     can be elinated as they are redundant.
      REAL XFSAV, YFSAV, DFSAV, WFSAV

C     Normalized residuals.
      REAL NRES

C These are quantities which do not yet have a home.

C     MAPO2N - map from cdtrakcd track numbers to zebra track numbers.
C     MAPN2O - map from zebra track numbers to cdtrakcd track numbers.
C     MAPCRL - map for renumbering of joined curlers.
      INTEGER MAPO2N, MAPN2O, MAPCRL

C     Number of tracks in the map for this event.
C     ( = NTRKCD immediately after DUFIND )
      INTEGER NTKMAP

      COMMON /DUTMPI/ LAYSAV(KLYRTF), IASAV(KLYRTF),
     +                MAPO2N(MTRAK2), MAPCRL(2,MTRAK2), NTKMAP,
     +                MAPN2O(MTRAK2)
      SAVE   /DUTMPI/

      COMMON /DUTMPR/ SFSAV(KLYRTF), NRES(KLYRTF),
     +                XFSAV(KLYRTF), YFSAV(KLYRTF),
     +                DFSAV(KLYRTF), WFSAV(KLYRTF)
      SAVE   /DUTMPR/



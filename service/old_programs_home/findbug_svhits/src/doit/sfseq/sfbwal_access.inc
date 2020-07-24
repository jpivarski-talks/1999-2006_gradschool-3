*
* $Id: sfbwal_access.inc,v 1.1 1999/11/19 22:50:29 dpp Exp $
*
* $Log: sfbwal_access.inc,v $
* Revision 1.1  1999/11/19 22:50:29  dpp
* new
*
* ------------------------------------------
*
* sfbwal_access.inc
*
#if !defined(CLEO_NOCOMDOC)
* SFBWAL_ACCESS DOCUMENTATION

C.* NTSFNX ...sequential track number in order from SFNEXT
C.. NTSFNO....other sequential track number, for vertex
C.. MSFNSF ...map from SFnext sequntial trak to SF chain number
C.* CUDRSF ...curvature
C.* DADRSF ...impact parameter
C.. DADRCO ...correction to the inpact parameter from curvature
C.* F0DRSF ...PHI 0
C.. F0DRCO ...correction to the PHI 0  from curvature
C.* Z0DRSF ...Z impact parameter =0
C.* CTDRSF ...tan of z angle; TANCD
C.. ANGSTR ...starting angle; of chain in beginning layer
C.. XSTART ...x starting postion of chain, global cordinates, for SFNEXT track
C.. YSTART ...y starting postion of chain, global cordinates, for SFNEXT track
C.. DXDSGL ...dx/(unit length) in global cordinates for each SFNEXT track
C.. DYDSGL ...dy/(unit length) in global cordinates for each SFNEXT track
#endif

      INTEGER MTSFNX
      PARAMETER (MTSFNX=MCHAIN)

      INTEGER       NTSFNX,NTSFNO
      INTEGER       MSFNSF
      REAL          CUDRSF
      REAL          DADRSF,DADRCO
      REAL          F0DRSF,F0DRCO
      REAL          Z0DRSF
      REAL          CTDRSF
      REAL          ANGSTR
      REAL          XSTART
      REAL          YSTART
      REAL          DXDSGL
      REAL          DYDSGL

      COMMON/SFBWAL_ACCESS/NTSFNX,NTSFNO,
     2                     MSFNSF(MTSFNX),CUDRSF(MTSFNX),
     3                     DADRSF(MTSFNX),DADRCO,
     4                     F0DRSF(MTSFNX),F0DRCO,
     5                     Z0DRSF(MTSFNX),CTDRSF(MTSFNX),
     6                     ANGSTR(MTSFNX),
     7                     XSTART(MTSFNX),YSTART(MTSFNX),
     8                     DXDSGL(MTSFNX),DYDSGL(MTSFNX)
* ------------------------------------------

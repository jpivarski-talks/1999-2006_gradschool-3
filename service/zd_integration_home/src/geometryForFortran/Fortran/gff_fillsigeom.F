
* $Id: gff_fillsigeom.F,v 1.4 2000/05/03 15:42:59 lyon Exp $
*
* $Log: gff_fillsigeom.F,v $
* Revision 1.4  2000/05/03 15:42:59  lyon
* Added
* #include "cl3seq/cdgm3/cdgeompa.inc"
* #include "doit/duseq/tfgeompa.inc"
* so #include "doit/duseq/svgeometry.inc" would work
*
* Revision 1.3  2000/04/13 17:29:35  pg
* Fxied typos in gff_sigeom.F
*
* Revision 1.2  2000/04/03 14:39:25  pg
* Added additional geometry information for SI
*
* Revision 1.1  2000/03/28 16:02:52  pg
* Added routine gff_fillsigeom.F to make doit Si-aware
*
* Revision 1.1  1999/05/28 02:38:15  lyon
* C++ -> Fortran handlers for the different geometry subsystems
*
* Revision 1.1.1.1  1999/05/04 17:44:13  lyon
* Imported geometryForFortran sources
*
* Revision 1.1  1999/05/03 21:12:53  lyon
* Files needed for fortran geometry communication
*
*
      SUBROUTINE GFF_FILLSIGEOM(hybrid, x0, x511, y0, y511, r0, r511,
     &     phi0, phi511, farz, closez,COSX,COSY)
*
* begin_doc
*
*  Documentation for subroutine gff_fillsirzgeom
*
*  Purpose: Fill a Cathode layer in cdgeomcd.inc
*
*  This function fills the following variables in cdgeomcd.inc:
*     NWIRCD, INDXCD, LQDVCD, LSTLCD, LAXLCD, LCTLCD, RCD, RSQCD, 
*     TANSCD, RENDCD, PHIFCD, CELLCD, ZENDCD, WENDCD, DIFSCD, BSAGCD, 
*     ANGSCD, XX0CD, UU0CD, INDVCD, IDVCCD, IXLRCD, NWRZCD, ZEEFCD,
*     CELZCD, REFFCD, ATOCCD, CTOACD, ICANCD
*
*  Notes:  XX0CD and UU0CD are all set to zero (similar to original geometry).
*          INDVCD is set to zero since nothing in doit or solo uses it.
*          ATOCCD, CTOACD are zero since no cathodes yet
*
*  --------
*
*  Author:   Adam Lyon      Created:  Wed Apr 28 16:10:53 EDT 1999
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/svgeometry.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      integer hybrid
      real    x0,x511,y0,y511,r0,r511,phi0,phi511,farz,closez
      real COSX,COSY
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: gff_fillsigeom.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/05/03 15:42:59 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'GFF_FILLSIRZGEOM' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER i, j
*
* end_var
*
*  Data statements:
*  ----------------
*
*  executable code for routine gff_fillsirzgeom:
*----6----------------------------------------------------------------72
*
      I = HYBRID
      
      SVBOUNDARY_X(I,1) = x0 
      SVBOUNDARY_X(I,2) = x511 

      SVBOUNDARY_Y(I,1) = y0 
      SVBOUNDARY_Y(I,2) = y511 

      SVBOUNDARY_R(I,1) = r0 
      SVBOUNDARY_R(I,2) = r511 

      SVBOUNDARY_PHI(I,1) = phi0 
      SVBOUNDARY_PHI(I,2) = phi511 

      SVBOUNDARY_Z(I,1) = CLOSEZ 
      SVBOUNDARY_Z(I,2) = FARZ

      SVDXDZ(I) = COSX
      SVDYDZ(I) = COSY


      RETURN
      END







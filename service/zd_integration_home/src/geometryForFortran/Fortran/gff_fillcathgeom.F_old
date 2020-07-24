* $Id: gff_fillcathgeom.F,v 1.2 1999/10/31 15:11:23 lyon Exp $
*
* $Log: gff_fillcathgeom.F,v $
* Revision 1.2  1999/10/31 15:11:23  lyon
* improvements for cathodes
*
* Revision 1.1  1999/05/28 02:38:13  lyon
* C++ -> Fortran handlers for the different geometry subsystems
*
* Revision 1.1.1.1  1999/05/04 17:44:13  lyon
* Imported geometryForFortran sources
*
* Revision 1.1  1999/05/03 21:12:53  lyon
* Files needed for fortran geometry communication
*
*
      SUBROUTINE GFF_FILLCATHGEOM(layer, laydev, r, ntot,
     &     nzdiv, width,
     &     zend, rend, index)
*
* begin_doc
*
*  Documentation for subroutine gff_fillcathgeom
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
#include "cl3seq/cdgm3/cdgeomcd.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      integer layer, laydev, n, index, nzdiv, ntot
      real    r, width, zend, rend
      
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: gff_fillcathgeom.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1999/10/31 15:11:23 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'GFF_FILLCATHGEOM' )
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
*  executable code for routine gff_fillcathgeom:
*----6----------------------------------------------------------------72
*
      i = layer
      
      NWIRCD(i) = ntot                   ! # of readouts in the layer
      INDXCD(i) = index                  ! Index of first wire
      
      LQDVCD(i) = .false.                ! never charge division

      LSTLCD(i) = .false.
      LAXLCD(i) = .false.

      LCTLCD(i) = .true.                 ! It's a cathode!

      RCD(i) = r                         ! layer radius
      RSQCD(i) = r**2                    ! layer radius**2
      TANSCD(i) = 0.0                    ! tan(stereo angle)
      RENDCD(i) = rend                   ! radius at end of wire (+z)
      PHIFCD(i) = 0.0                    ! Phi offset of first wire
      CELLCD(i) = width                  ! Phi width of cell
      ZENDCD(i) = zend                   ! Z at end of wire (+z)
      WENDCD(i) = 0                      ! End read out 
      DIFSCD(i) = 0.0                    ! # of cells stereo wire traverses
      BSAGCD(i) = 0.0                    ! Sag of wire in y
      ANGSCD(i) = 0.0                    ! Stereo angle

C*****Make these zero for now
      do j = 1, 3
        XX0CD(j, i) = 0.0                ! 3-D offset of layer
        UU0CD(j, i) = 0.0                ! Directions cosine
      enddo

C*****No z divisions for this layer
      nwrzcd(i) = nzdiv                ! Number of pads down the cathode in z
      zeefcd(i)  = 0.0      
      celzcd(i)  = 0.02
      reffcd(i)  = 0.0

C*****This isn't used by any code
      INDVCD(i) = indxcd(i)
      
C*****The drift chamber is device IDRFT. Its layer numbers map one-to-one
C*****with the global CD layer ordering. Eg. drift anode layer 1 is CD layer 1
      IDVCCD(i) = IDRCT
      IXLRCD(laydev, IDRCT) = i

C*****The layer before is supposed to be the anode layer associated
C*****with this cathode
      ATOCCD(i-1) = i
      CTOACD(i) = i-1

C*****The ICANCD layer corresponds to the CD layer (?? so long as the 
C*****   DR goes first ??)
      ICANCD(i) = 9999999
      
      RETURN
      END




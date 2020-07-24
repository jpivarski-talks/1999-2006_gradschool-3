
* $Id: gff_filldrgeomcd.F,v 1.2 2003/04/24 14:19:35 huanggs Exp $
*
* $Log: gff_filldrgeomcd.F,v $
* Revision 1.2  2003/04/24 14:19:35  huanggs
* add laydev as argument for general case
*
* Revision 1.1  1999/05/28 02:37:41  lyon
* Was gff_fillcdgeomcd.F
*
* Revision 1.1.1.1  1999/05/04 17:44:13  lyon
* Imported geometryForFortran sources
*
* Revision 1.1  1999/05/03 21:12:53  lyon
* Files needed for fortran geometry communication
*
*
      SUBROUTINE GFF_FILLDRGEOMCD(layer, laydev, nw, index, ist, radius,
     &     tanstereo, phi0, cellwidth, zatend, ratend, wend, dif, bsag,
     &     stang)
*
* begin_doc
*
*  Documentation for subroutine gff_filldrgeomcd
*
*  Purpose: Fill a DR anode layer in drgeomcd.inc (called by 
*       ADRSenseWireStore::fillFortranDRGEOMCD )
*
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
      integer layer, laydev, nw, index, ist, wend
      real radius, tanstereo, phi0, cellwidth, zatend, ratend, dif,
     &     bsag, stang
      
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: gff_filldrgeomcd.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/04/24 14:19:35 $')
      PARAMETER(CHRCSA='$Author: huanggs $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'GFF_FILLDRGEOMCD' )
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
*  executable code for routine gff_filldrgeomcd:
*----6----------------------------------------------------------------72
*


      print 1001,layer, laydev, nw, index, ist, radius
 1001 format(' filldrgeomcd enter:layer,laydev,nw,index,ist,radius=',
     2     5I6,F9.4     )


      i = layer
      
      NWIRCD(i) = nw                     ! Number of wires
      INDXCD(i) = index                  ! Index of first wire
      
      LQDVCD(i) = .false.                ! never charge division

      LSTLCD(i) = ist .eq. 1             ! could be stereo
      LAXLCD(i) = .not. lstlcd(i)        ! or maybe axial

C      LSTLCD(i) = .true.
C      LAXLCD(i) = .false.

      LCTLCD(i) = .false.                ! never cathode

      RCD(i) = radius                    ! layer radius
      RSQCD(i) = radius**2               ! layer radius**2
      TANSCD(i) = tanStereo              ! tan(stereo angle)
      RENDCD(i) = ratend                 ! radius at end of wire (+z)
      PHIFCD(i) = phi0                   ! Phi offset of first wire
      CELLCD(i) = cellwidth              ! Phi width of cell
      ZENDCD(i) = zatend                 ! Z at end of wire (+z)
      WENDCD(i) = wend                   ! End read out 
      DIFSCD(i) = dif                    ! # of cells stereo wire traverses
      BSAGCD(i) = bsag                   ! Sag of wire in y
      ANGSCD(i) = stang                  ! Stereo angle

C*****Make these zero for now
      do j = 1, 3
        XX0CD(j, i) = 0.0                ! 3-D offset of layer
        UU0CD(j, i) = 0.0                ! Directions cosine
      enddo

C*****No z divisions for this layer
      nwrzcd(i) = 1        ! Well, a wire is one big z division
      zeefcd(i)  = 0.0      
      celzcd(i)  = 0.0
      reffcd(i)  = 0.0

C*****This isn't used by any code
      INDVCD(i) = indxcd(i)
      
C*****The drift chamber is device IDRFT. Its layer numbers map one-to-one
C*****with the global CD layer ordering. Eg. drift anode layer 1 is CD layer 1
      IDVCCD(i) = IDRFT
*hgs  IXLRCD(i, IDRFT) = i
*hgs: but dpp concludes it is true only when drift anode is the first device,
*hgs: so more generally, a local layer number, laydev, is necessary,
*hgs: and this is also the way used for drift cathode and silicon codes.
      IXLRCD(laydev, IDRFT) = i

C*****Set corresponding anode/cathode layer (just zero for now)
      ATOCCD(i) = 0
      CTOACD(i) = 0

C*****map the anode layers to CD layers
      NCANCD=NCANCD+1
      ICANCD(i) = NCANCD
      
      RETURN
      END




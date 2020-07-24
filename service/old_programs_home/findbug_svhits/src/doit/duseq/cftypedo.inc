*
* $Id: cftypedo.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cftypedo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
* CFTYPECD Documentation
*
* cftypedo.inc
*
*
C Defines (for CFIND) the correspondence between :
C          (a) Type numbers and inner & outer layer numbers
C          (b) Kind(s) of elementary trees and type numbers
C Initialized in CFINIT
*
* NLRUCF - Number of layers used in CFIND
* IPLRCF - Physical layer # for logical layer number
* LETRCF - First elementary tree for tracks with dropped layers
* METRCF - Last  elementary tree for tracks with dropped layers
* IILRCF - Number of inner  logical  layer for given type
* IMLRCF - Number of middle logical  layer for given type
* IOLRCF - Number of outer  logical  layer fot given type
* IRTPCF - Type # of root   link in an elementry tree of given type
* IBTPCF - Type # of branch link in an elementry tree of given type
* IFETCF - Index of first tree with inner physical layer ILYR and
*          outer physical layer JLYR stored as a triangular matrix
* NXETCF - Index of next tree with same inner & outer physical layers
* D0LHCF - Sigma of d0 for link of given type using SIGHCF
* D0LWCF - Sigma of d0 for link of given type using wire only
* DIFHCF - Cur diff cut for tree of given tree using SIGHCF
* DIFWCF - Cur diff cut for tree of given tree using wire
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"

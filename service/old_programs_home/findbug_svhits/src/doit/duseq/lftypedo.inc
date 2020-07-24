*
* $Id: lftypedo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: lftypedo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:08  zfiles
* first version of doit in CVS
*
*
* LFTYPECD Documentation
*
* lftypedo.inc
*
*
* /LFTYPE/ Defines (for LFIND) the correspondence between :
*              (a) type numbers and inner & outer layer numbers
*              (b) kind(s) of elementary trees and type numbers
*
* NLRULF - Number of layers used in LFIND
* IPLRLF - Physical layer # for given logical layer #
* LETRLF - First elementary tree for tracks with given # of dropped lyrs
* METRLF - Last  elementary tree for tracks with given # of dropped lyrs
* IILRLF - # of inner  logical  layer for given type #
* IOLRLF - # of outer  logical  layer for given type #
* IRTPLF - Type # of root   link in an elementry tree of given kind
* IBTPLF - Type # of branch link in an elementry tree of given kind
* IFETLF - Index of first tree with inner physical layer ILYR and
*          outer physical layer JLYR stored as a triangular matrix
* NXETLF - Index of next tree with same inner & outer physical layers
* D0LHLF - Sigma of D0  for link of given type using SIGHLF
* DIFHLF - Cur diff cut for tree of given kind using SIGHLF
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"

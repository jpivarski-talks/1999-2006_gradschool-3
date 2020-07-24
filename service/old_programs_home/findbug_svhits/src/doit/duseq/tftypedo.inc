*
* $Id: tftypedo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftypedo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
* TFTYPECD Documentation
*
* tftypedo.inc
*
C
* /TFTYPE/ Holds pointers to the elementary trees necessary for all
*          tracks with a given # of hits. If all layers have hits,
*          these are just copied from /CFTYPE/ or /LFTYPE/.
*
* MLYRTP(ITYP) - The maximum physical layer for the link type
*                filled in ?FLINK and used in TFNEWT for tracks
*                with the minimum # of hits
*
* LSTRUT(ITYP) - Link type #ITYP can be the last link in a root chain
* LSTBRN(ITYP) - Link type #ITYP can be the last link in a branch chain
*
* LUSRUT(ITYP) - Link type #ITYP has been used as the
*                root   link in an elementary tree
* LUSBRN(ITYP) - Link type #ITYP has been used as the
*                branch link in an elementary tree
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"

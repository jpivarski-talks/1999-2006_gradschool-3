*
* $Id: sffix.inc,v 1.2 2001/11/20 17:51:16 dpp Exp $
*
* $Log: sffix.inc,v $
* Revision 1.2  2001/11/20 17:51:16  dpp
*      -> delete stupid useless nonexistent include file
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:12  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/01 21/07/93  17.15.53  by  Rob Kutschke
*CMZ :          30/03/93  13.10.25  by  Dan Peterson
* ------------------------------------------
* STEP FIND FIXES TO THE GEOMETRY
*
* sffix.inc
*
#if !defined(CLEO_NOCOMDOC)
C /STEPF1/..........FIX TO THE PHI OFFSETS OF THE LAYERS
C FFIXSF...PHI_FIX_SF
#endif
      REAL FFIXSF
      COMMON/STEPF1/FFIXSF(KLYRCD)






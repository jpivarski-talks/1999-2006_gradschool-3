*
* $Id: cdcorrcd.inc,v 1.2 2001/11/19 23:20:56 dpp Exp $
*
* $Log: cdcorrcd.inc,v $
* Revision 1.2  2001/11/19 23:20:56  dpp
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:06  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.38  by  Steve Schaffner
*CMZ :  5.53/03 16/05/90  17.28.00  by  Steve Schaffner
*CMZ :  5.52/00 24/01/90  14.42.04  by  Steve Schaffner
*-- Author :
* CD Corrections sequence
*
* cdcorrcd.inc
*
#if !defined(CLEO_NOCOMDOC)
* VDRFT  - Drift velocities per cylinder
* BETAW  - Beta on sense wires
* CDRFT  - Quadratic correction term
* ASAG   - Gravitational sag correction
* EBC1/4 - Vermutlich E x B corrections
* CLIGHT - Speed of light in m/psec
* VCONV  - Converts m to psec
#endif

      REAL VDRFT, BETAW, CDRFT, ASAG, EBC1, EBC2, EBC3, EBC4
      REAL CLIGHT, VCONV
*
      COMMON /CDCORR/ VDRFT(KLYRTF), BETAW(KLYRTF), CDRFT,
     +                ASAG, EBC1, EBC2, EBC3, EBC4,
     +                CLIGHT, VCONV

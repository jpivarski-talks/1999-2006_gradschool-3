*
* $Id: tftime.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftime.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.41  by  Steve Schaffner
*CMZ :  5.53/04 18/06/90  12.52.37  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.22  by  Steve Schaffner
*-- Author :
* TF CPU time statistics sequence
*
* tftime.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tftimedo.inc"
#endif
C
      INTEGER    NTIMDM
      PARAMETER( NTIMDM = 62 )
*
      INTEGER    ITTOTL     , ITDUE2     , ITDUE3     , ITDUET
      PARAMETER( ITTOTL =  1, ITDUE2 =  2, ITDUE3 =  3, ITDUET =  4 )
      INTEGER    ITDRHW     , ITCDFT
      PARAMETER( ITDRHW =  5, ITCDFT = 6              )
*
      INTEGER    ITTFCR     , IT3FIT     , ITCDHS     , ITCDNT
      PARAMETER( ITTFCR =  7, IT3FIT =  8, ITCDHS =  9, ITCDNT = 10 )
      INTEGER    ITDKFN     , ITPHFN     , ITCDST
      PARAMETER( ITDKFN = 11, ITPHFN = 12, ITCDST = 13              )
*
      INTEGER    ITTFND     , KTIMDG     , KTIMCF     , KTIMLF
      PARAMETER( ITTFND = 14, KTIMDG = 15, KTIMCF = 31, KTIMLF = 47 )
*
      INTEGER    ITFIND     , ITHITS     , ITCORR     , ITGETR
      PARAMETER( ITFIND =  1, ITHITS =  2, ITCORR =  3, ITGETR =  4 )
      INTEGER    ITLINK     , ITTREE     , ITFRST     , ITCLMB
      PARAMETER( ITLINK =  5, ITTREE =  6, ITFRST =  7, ITCLMB =  8 )
      INTEGER    ITNEWT     , ITTFIT     , ITFIT      , ITSORT
      PARAMETER( ITNEWT =  9, ITTFIT = 10, ITFIT  = 11, ITSORT = 12 )
      INTEGER    ITTCUT     , ITRFIT     , ITHIST
      PARAMETER( ITTCUT = 13, ITRFIT = 14, ITHIST = 15              )
*
      INTEGER         KTIMES, NCALLS        , ITIMES
      COMMON /TFTIMI/ KTIMES, NCALLS(NTIMDM), ITIMES(NTIMDM)
*
      CHARACTER*8             SUBTTL
      COMMON /TFTIMA/         SUBTTL(NTIMDM)
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"

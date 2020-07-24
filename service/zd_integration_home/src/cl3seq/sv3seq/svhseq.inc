*
* $Id: svhseq.inc,v 1.1.1.1 1995/11/01 21:18:37 lkg Exp $
*
* $Log: svhseq.inc,v $
* Revision 1.1.1.1  1995/11/01 21:18:37  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:04  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/46 17/12/93  10.55.40  by  Unknown
*CMZ :  1.02/44 22/10/93  14.58.18  by  Unknown
*-- Author :
*
*   MXSVNH - number of elements per SVD hit.
*
      INTEGER MXSVNH
      PARAMETER (MXSVNH = 14)
*
*   SVNMSH - names of hit elements, filled in SVGINI.

      CHARACTER*4     SVNMSH

      COMMON /SVHTCC/ SVNMSH(MXSVNH)
      SAVE   /SVHTCC/
*
*   SVNBSH - number of bits for each hit element, filled in SVGINI.

      INTEGER         SVNBSH

      COMMON /SVHTCI/ SVNBSH(MXSVNH)
      SAVE   /SVHTCI/
*
*   SVORIG - packing offset for SVD hits, filled in SVGINI.
*   SVFACT - packing factor for SVD hits, filled in SVGINI.

      REAL            SVORIG,         SVFACT

      COMMON /SVHTCR/ SVORIG(MXSVNH), SVFACT(MXSVNH)
      SAVE   /SVHTCR/
*
*   JSVXIW - XIN world   hit element number.
*   JSVYIW - YIN world    "     "       "
*   JSVZIW - ZIN world    "     "       "
*   JSVP   - P            "     "       "
*   JSVXIL - XIN local    "     "       "
*   JSVYIL - YIN local    "     "       "
*   JSVZIL - ZIN local    "     "       "
*   JSVXO  - XOUT local   "     "       "
*   JSVYO  - YOUT local   "     "       "
*   JSVZO  - ZOUT local   "     "       "
*   JSVWN  - WAFN (copy #)"     "       "
*   JSVS   - S            "     "       "
*   JSVED  - EDEP         "     "       "
*   JSVTLN - TRACK LENGTH AT CURRENT POINT (sleng in gctrak)

      INTEGER    JSVXIW,     JSVYIW,      JSVZIW,     JSVP
      INTEGER    JSVXIL,     JSVYIL,      JSVZIL
      INTEGER    JSVXO,      JSVYO,       JSVZO
      INTEGER    JSVWN,      JSVS,        JSVED,      JSVTLN

      PARAMETER (JSVXIW = 1, JSVYIW = 2,  JSVZIW = 3, JSVP   = 4)
      PARAMETER (JSVXIL = 5, JSVYIL = 6,  JSVZIL = 7)
      PARAMETER (JSVXO  = 8, JSVYO  = 9,  JSVZO  =10)
      PARAMETER (JSVWN  =11, JSVS   =12,  JSVED  =13, JSVTLN =14)
*
*

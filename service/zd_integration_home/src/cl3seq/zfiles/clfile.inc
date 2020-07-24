*
* $Id: clfile.inc,v 1.1 1998/05/07 20:39:37 pg Exp $
*
* $Log: clfile.inc,v $
* Revision 1.1  1998/05/07 20:39:37  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/43 19/10/93  15.25.55  by  Dan Payne
*CMZ :  1.01/90 04/04/91  12.04.01  by  Mark Battle
*CMZ :  1.01/02 10/05/89  14.00.22  by  B. Heltsley
*CMZ :  2.00/10 05/12/88  20.47.47  by  R. Namjoshi
*-- Author :    R. Namjoshi   05/12/88
* CLEVER/CLEOG file/top directory definitions
*
* CLCDTD - Top Level directory for CD constants
* CLCCTD - Top Level directory for CC constants
* CLTFTD - Top Level directory for TF constants
* CLMUTD - Top Level directory for MU constants
* CLTGTD - Top Level directory for TG constants
* CLCGTD - Top Level directory for CLEOG constants
* CLDETD - Top Level directory for DE/DR constants
* CLSVTD - Top Level directory for SV constants
*
* CLCDZF - ZFILES file name for CD constants
* CLCCZF - ZFILES file name for CC constants
* CLTFZF - ZFILES file name for TF constants
* CLMUZF - ZFILES file name for MU constants
* CLTGZF - ZFILES file name for TG constants
* CLCGZF - ZFILES file name for CLEOG constants
* CLDEZF - ZFILES file name for DE/DR constants
* CLSVZF - ZFILES file name for SV constants
*
      CHARACTER*16    CLCDTD, CLCCTD, CLTFTD, CLMUTD, CLTGTD
      CHARACTER*16    CLCGTD, CLDETD, CLSVTD
      CHARACTER*256   CLCDZF, CLCCZF, CLTFZF, CLMUZF, CLTGZF
      CHARACTER*256   CLCGZF, CLDEZF, CLSVZF
      COMMON /CLFILE/ CLCDTD, CLCCTD, CLTFTD, CLMUTD, CLTGTD,
     +                CLCGTD, CLCDZF, CLCCZF, CLTFZF, CLMUZF,
     +                CLTGZF, CLCGZF, CLDETD, CLDEZF, CLSVTD,
     +                CLSVZF

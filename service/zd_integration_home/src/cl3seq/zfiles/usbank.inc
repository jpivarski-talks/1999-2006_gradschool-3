*
* $Id: usbank.inc,v 1.2 1998/07/29 21:01:43 pg Exp $
*
* $Log: usbank.inc,v $
* Revision 1.2  1998/07/29 21:01:43  pg
*  Bumped up array sizes to match Brian's. Should remove cl3seq/zfiles
* eventually.
*
* Revision 1.1  1998/05/07 20:39:39  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.3  1997/01/23 22:03:52  clib
* Update machine dependent cpp sections to use #if/#elif/#else.
*
* Revision 1.2  1995/11/29 18:12:59  zfiles
* Define larger array if CLEO_HUGM is defined
*
* Revision 1.1.1.1  1994/10/04  20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/48 07/02/94  09.47.21  by  Unknown
*CMZ :  1.01/81 28/01/91  23.36.43  by  CLEO II Librarian
*-- Author :
*
* usbank.inc
*
*
      INTEGER    NUDUMM
#if defined(CLEO_HUGM)
      PARAMETER( NUDUMM = 5500000 )
#elif defined(CLEO_BIGM)
      PARAMETER( NUDUMM = 2500000 )
#else
      PARAMETER( NUDUMM = 1000000 )
#endif
      INTEGER         NUWRDS, USFENC,     LZU

* *RGB1296* Updated to use #if/elif/else structure
#if defined(CLEO_ZFSYMODE)
      INTEGER NZEBRA
C catch if GCBANK is already defined.
      COMMON /GCBANK/ NUWRDS, USFENC(21), LZU(NUDUMM)
      EQUIVALENCE (NZEBRA, NUWRDS)
#else
      COMMON /USBANK/ NUWRDS, USFENC(16), LZU(NUDUMM)
#endif
      INTEGER IZU(NUDUMM)
      REAL     ZU(NUDUMM)
      EQUIVALENCE ( LZU(9), IZU(1), ZU(1) )
*
#include "cl3seq/zfiles/uslink.inc"
*


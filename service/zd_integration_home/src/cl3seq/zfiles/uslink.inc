*
* $Id: uslink.inc,v 1.1 1998/05/07 20:39:40 pg Exp $
*
* $Log: uslink.inc,v $
* Revision 1.1  1998/05/07 20:39:40  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.3  1996/05/08 22:46:16  zfiles
* Double size of NULDUM.  Now at 40000 for CLEO_HUGM and
* 10400 for no CLEO_HUGM.
*
* Revision 1.2  1995/11/29 18:13:01  zfiles
* Define larger array if CLEO_HUGM is defined
*
* Revision 1.1.1.1  1994/10/04  20:58:43  zfiles
* first version of seq in CVS
*
*
*  USER link area for user pointers
*
* uslink.inc
*
*
* NZLDUM - Dummy size of link area
* UP     - User pointers
*
* USPMAX - Size of the user   link area    for USBANK
* USRMIN - Lowest  reference  link pointer for USBANK
* USRMAX - Highest reference  link pointer for USBANK
* USSMIN - Lowest  structural link pointer for USBANK
* USSMAX - Highest structural link pointer for USBANK
*
      INTEGER         USPMAX, USRMIN, USRMAX, USSMIN, USSMAX
      COMMON /USPMCM/ USPMAX, USRMIN, USRMAX, USSMIN, USSMAX
* USLSUP - Supporting link for USGLNK
      INTEGER         USLSUP, USLAST
      COMMON /USSLNK/ USLSUP, USLAST

      INTEGER    NULDUM
#if defined(CLEO_BIGM) || defined(CLEO_HUGM)
      PARAMETER( NULDUM = 40000 )
#else
      PARAMETER( NULDUM = 10400 )
#endif
      INTEGER         PZU
      COMMON /USLINK/ PZU(NULDUM)

      INTEGER         IUSTOR, IUSEVT, IUSCON, IUSSTA
      COMMON /USDIVI/ IUSTOR, IUSEVT, IUSCON, IUSSTA
*

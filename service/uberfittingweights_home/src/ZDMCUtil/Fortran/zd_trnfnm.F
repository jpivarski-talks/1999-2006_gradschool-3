#include "sys/CLEO_machine.h"
*------------------------------------------------------------------------------
* *RGB1196*  Move #if defined(CLEO_UNIX) inside subroutine
* #if defined(CLEO_UNIX)
*------------------------------------------------------------------------------
*CMZ :  1.00/35 14/05/91  14.28.52  by  B. Heltsley
* add declaration for LENOCC and IF=UNIX above
*CMZ :          02/07/90  18.38.16  by  C Ray Ng
*CMZ :  1.00/20 19/12/89  20.54.40  by  C Ray Ng
*-- Author :    C Ray Ng   19/12/89
      SUBROUTINE ZD_TRNFNM( LOGINP, EQUNAM )
*
*     Translate logical name to equvalence physical name
*
*------------------------------------------------------------------------------
* *RGB1196* There are now 3 variants, not 2, changed the comment line 
*           following these added comments and added comments for case 3)
*------------------------------------------------------------------------------
*     There are three variants in the usage of logical name:
*
*     1) single logical name
*        e.g.   CONFILE       = /usr/users/guest/confile.dat
*
*     2) compounded logical name
*        e.g.   CLLIB:confile.dat
*               where CLLIB   = /usr/cllib/bank
*        or     $CLLIB/confile.dat
*
*     A single character ":" is used to distinguish these cases;
*       or prefixed with a "$" and terminated with "/".
*
*     3) non-UNIX call, input is physical name
*
*     Return value:
*     case 1)   return the translated name if it is defined or
*               the logical name itself if it is not defined
*     case 2)   append the rest of the path name to the translated directory
*               or leave the directory blank if it is not defined.
*     case 3)   return the input
*
*------------------------------------------------------------------------------
* *RGB1196* Start UNIX-only section of code.  The lines between the following
*           #if defined(CLEO_UNIX) statement and the subsequent #else are
*           unmodified from the previous version of this subroutine.
#if defined(CLEO_UNIX)
*------------------------------------------------------------------------------
      EXTERNAL LENOCC
      INTEGER  LENOCC
      CHARACTER*(*)
     $          LOGINP,
     $          EQUNAM
*
*     Local declarations:
*
      CHARACTER*200 LOGNAM
      INTEGER   LENLOG, LENEQU, ICOLON, ISLASH

      LOGNAM = LOGINP
      LENEQU = 0
      EQUNAM = ' '
      LENLOG = LENOCC( LOGNAM )
      ICOLON = INDEX( LOGNAM(:LENLOG), ':' )
      ISLASH = INDEX( LOGNAM(:LENLOG), '/' )

      IF( LOGNAM(1:1) .EQ. '$' ) THEN
         IF( ISLASH .EQ. 0 ) THEN
            CALL GETENV( LOGNAM(2:LENLOG), EQUNAM )
            LENEQU = LENOCC( EQUNAM )
         ELSEIF( ISLASH .GT. 2 ) THEN
            CALL GETENV( LOGNAM(2:ISLASH-1), EQUNAM )
            LENEQU = LENOCC( EQUNAM )
            IF( LENEQU .GT. 0 .AND. LENLOG .GT. ISLASH ) THEN
               EQUNAM = EQUNAM(1:LENEQU)//LOGNAM(ISLASH:LENLOG)
            ENDIF
         ENDIF
      ELSE
         IF( ICOLON .EQ. 0 ) THEN
            CALL GETENV( LOGNAM, EQUNAM )
            LENEQU = LENOCC( EQUNAM )
         ELSEIF( ICOLON .GT. 1 ) THEN
            CALL GETENV( LOGNAM(1:ICOLON-1), EQUNAM )
            LENEQU = LENOCC( EQUNAM )
            IF( LENEQU .GT. 0 .AND. LENLOG .GT. ICOLON ) THEN
               EQUNAM = EQUNAM(1:LENEQU)//'/'//LOGNAM(ICOLON+1:LENLOG)
            ENDIF
         ENDIF
      ENDIF

      IF( LENEQU .EQ. 0 ) EQUNAM = LOGNAM
*------------------------------------------------------------------------------
* *RGB1196* Closing the #if defined(CLEO_UNIX) block that started at the 
*           beginning of the subroutine.  The following #else block is
*           new and is intended for all non-UNIX compilation.  It handles
*           case 3) explained in the introductory comments.
*------------------------------------------------------------------------------
#else
      IMPLICIT NONE

* *RGB1196* Declare input, output variable names
      CHARACTER*(*)
     $          LOGINP,
     $          EQUNAM

* *RGB1196* If not on UNIX (case 3)), input does not need translation, so
*           set output = input
        EQUNAM = LOGINP

#endif
*------------------------------------------------------------------------------
* *RGB1196* end of "new code" to handle dummy non-UNIX case.
*------------------------------------------------------------------------------
      RETURN
      END
*------------------------------------------------------------------------------
* *RGB1196* #if block was moved inside the subroutine, remove this #endif
* #endif
*------------------------------------------------------------------------------

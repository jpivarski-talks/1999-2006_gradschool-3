*
* $Id: duklmncd.inc,v 1.2 2001/11/19 23:14:44 dpp Exp $
*
* $Log: duklmncd.inc,v $
* Revision 1.2  2001/11/19 23:14:44  dpp
*     -> remove unused variables
*
* Revision 1.1  1998/07/09 02:36:19  lyon
* Adding duklseq from DPP
*
* Revision 1.6  1996/04/29 16:02:32  zfiles
* Add LDUMPG.
*
* Revision 1.5  1996/04/23 23:19:00  zfiles
* Comments in previous version are wrong!
*
* Revision 1.4  1996/04/23 07:28:05  zfiles
* New namelist variables.  Delete unused variables.  Move some variables to
* stack.
*
* Revision 1.3  1995/09/07 23:41:04  zfiles
* Added Id and Log. Added SAVE statements.
*
*
C
C Control common for the interface between duet and the kalman filter.
C 
C PERFECT - Expect tracks to have no MS and no ELOSS.  In this case we
C           can check CLEOG's drift distance calculation.
C DEBUG   - enable/disable debug printout
C
C
C
C ISVDPR - Controls calls to SVDPR when LTRTAB = .FALSE.
C           0 = call svdpr for each track
C           1 = call svdpr once, at the end of the event
C
C
C
C
C
C LDUMPG - dump the Kalman geometry information to an ascii file
C          which can be read back by user_trans.
C
      LOGICAL PERFECT, DEBUG
      INTEGER ISVDPR

      COMMON /DUKLML/ PERFECT, DEBUG
      COMMON /DUKLMI/ ISVDPR

      SAVE /DUKLML/
      SAVE /DUKLMI/



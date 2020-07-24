*
* $Id: duklmncd.inc,v 1.1 1998/07/09 02:36:19 lyon Exp $
*
* $Log: duklmncd.inc,v $
* Revision 1.1  1998/07/09 02:36:19  lyon
* Adding duklseq from DPP
*
* Revision 1.6  1996/04/29 16:02:32  zfiles
* Add LDUMPG.
*
* Revision 1.5  1996/04/23 23:19:00  zfiles
* Comments in previous version are wrong!
* True comment:
*      New namelist variables: LSMOOT, LRESID, LHIDEL, LRESHT, JSVDPR.
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
C LTRTAB - use Truth table pattern recognition instead of DUET's pattern
C          recognition.
C PERFECT - Expect tracks to have no MS and no ELOSS.  In this case we
C           can check CLEOG's drift distance calculation.
C DEBUG   - enable/disable debug printout
C
C SINGLT  - Are the input events single track MC?
C
C ITWRIT - 0 do not write any ascii files of hits.
C          1 write the truth table hits to a file instead of calling
C            calling the code to fit them.  Use the original format
C            we used a long time ago.
C          2 same but use a new format that exactly mimics the
C            information currently passed to the kalman filter code.
C
C ISVDPR - Controls calls to SVDPR when LTRTAB = .FALSE.
C           0 = call svdpr for each track
C           1 = call svdpr once, at the end of the event
C
C JSVDPR - Controls calls to SVDPR when LTRTAB=.TRUE.
C          -2 - fit wire chamber hits only; report result at the origin.
C          -1 - fit all hits at once and report result at the origin.
C           0 - Fit in two steps:
C               a) fit only DR, VD hits and report the result at a point
C                  just outside the SVD wafers.
C               b) add svd hits and then continue the fit to the origin.
C
C LTTSVD - Control truth table/real pattern recognition separately for
C          SVD.
C          .TRUE.  = Use truth table
C          .FALSE. = Use real pattern recognition.
C
C LUSSCF - apply layer by layer scale factors to resolution functions.
C          ( only for tf3fit )
C
C LSMOOT - Do smoothing, or not.
C 
C LRESID - Compute residuals, or not.
C
C LHIDEL - Do hit deletion, or not.
C
C LRESHT - Include hit in fit, or not, for computation of residual.
C          .TRUE.  - include hit.
C          .FALSE. - exclude hit.
C
C KLPART - particle hypothesis to be used in KLMNFT call to KLMNFT.
C          0     - do all, 
C          1...5 - e, mu, pi, K, p
C
C LDUMPG - dump the Kalman geometry information to an ascii file
C          which can be read back by user_trans.
C
      LOGICAL LTRTAB, PERFECT, DEBUG, SINGLT, LUSSCF, LTTSVD
      LOGICAL LSMOOT, LRESID, LHIDEL, LRESHT, LDUMPG
      INTEGER ITWRIT, ISVDPR, KLPART, JSVDPR

      COMMON /DUKLML/ LTRTAB, PERFECT, DEBUG, SINGLT, LUSSCF, LTTSVD,
     +                LSMOOT, LRESID, LHIDEL, LRESHT, LDUMPG
      COMMON /DUKLMI/ ITWRIT, ISVDPR, KLPART, JSVDPR

      SAVE /DUKLML/
      SAVE /DUKLMI/



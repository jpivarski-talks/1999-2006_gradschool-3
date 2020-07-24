*
* $Id: err_matrix_element.F,v 1.3 2001/11/19 23:42:22 dpp Exp $
*
* $Log: err_matrix_element.F,v $
* Revision 1.3  2001/11/19 23:42:22  dpp
*      -> delete unused include file
*
* Revision 1.2  2000/08/18 18:56:17  dpp
*     -> change print format
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE ERR_MATRIX_ELEMENT(IN_OUT,P3D1,P3D2,VALUE)
C.......................................................................
C.
C. ERR_MATRIX_ELEMENT - fill the error matrix element in tftrakcd
C.
C. COMMON    : ?
C. CALLS     : 
C. CALLED    : ?
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 22-Dec-97
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER IN_OUT,P3D1,P3D2,P3DTM,P3DLO,P3DHI,PERM
      REAL VALUE

* ----------Executable code starts here---------------------
         P3DLO=P3D1
         P3DHI=P3D2
         IF(P3DLO.GT.P3DHI)THEN
            P3DTM=P3DLO
            P3DLO=P3DHI
            P3DHI=P3DTM
         ENDIF
         PERM=P3DHI*(P3DHI-1)/2.+P3DLO
         IF((PERM.GE.1).AND.(PERM.LE.NERMTF))THEN
            IF(IN_OUT.EQ.1)THEN
               ERMFIT(PERM)=VALUE
#if defined(CLEO_DPPDIAG)
               PRINT 1003,PERM,VALUE
 1003          FORMAT(' ERR_MATRIX_ELEMENT:'
     2                ' fill ERMFIT(',I2,')=',F18.10) 
#endif
            ELSEIF(IN_OUT.EQ.2)THEN
               VALUE=ERMFIT(PERM)
#if defined(CLEO_DPPDIAG)
               PRINT 1004,PERM,VALUE
 1004          FORMAT(' ERR_MATRIX_ELEMENT:'
     2                ' retrieve  ERMFIT(',I2,')=',F18.10) 
#endif
            ELSE

#if defined(CLEO_DPPDIAG)
               PRINT 1001,IN_OUT
 1001          FORMAT(' ERR_MATRIX_ELEMENT: garbage IN_OUT=',I7) 
#endif

            ENDIF
         ELSE

#if defined(CLEO_DPPDIAG)
            PRINT 1002,P3D1,P3D2,PERM
 1002       FORMAT(' ERR_MATRIX_ELEMENT: bad parameter:',3I7)
#endif

         ENDIF
      RETURN
      END







*
* $Id: meslog_local.F,v 1.1 2003/03/05 17:15:24 dpp Exp $
*
* $Log: meslog_local.F,v $
* Revision 1.1  2003/03/05 17:15:24  dpp
*      -> NEW ROUTINE
*      -> local version of meslog to avoid messy C++ usage by system meslog
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
#if defined(CLEO_DOITTPC)
      SUBROUTINE MESLOG(SHOWCL,I1,ERRORTYPE)
#else
      SUBROUTINE MESLOG_LOCAL(SHOWCL,I1,ERRORTYPE)
#endif

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "seq/clutil/mesrep.inc"

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12 

C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL

      CHARACTER*(*) SHOWCL,ERRORTYPE
      INTEGER I1

      INTEGER LENSC,LENCH,LENET


* ----------Executable code starts here---------------------


      LENSC=LEN(SHOWCL)
      IF(LENSC.GT.10)LENSC=10
      LENCH=LEN(CHMESS)
      IF(LENCH.GT.100)LENCH=100
      LENET=LEN(ERRORTYPE)
      IF(LENET.GT.10)LENET=10

      PRINT 1103,SHOWCL(1:LENSC),CHMESS
 1103 FORMAT(' meslog_local:',
     2     A10,', lev:','?',', ',A100)

      RETURN
      END









      SUBROUTINE DOIT_BEGIN_RUN_FORTRAN
C-----------------------------------------------------------------------
C-
C-   Purpose and Methods: This little function just allocates memory
C-                        for variables in dvstatus common block.
C-                        
C-                        You'll get link errors without this.
C-
C-                        It doesn't really do anything. 
C-
C-   Inputs  :
C-   Outputs :
C-   Controls:
C-
C-   Created  17-APR-1998   Adam L. Lyon
C-
C-----------------------------------------------------------------------
      IMPLICIT NONE

#include "dvlib/seq/dvstatus.inc"
C-----------------------------------------------------------------------

      irundv = 75001
      ievtdv = 0

  999 RETURN
      END
     

      SUBROUTINE HBOOKINIT( )
C!----------------------------------------------------------------------
C!
C!   Routine  : HBOOKINIT
C!
C!   Purpose  : CLOSE a fortran logical unit
C!
C!   Author   : Simon Patton  15-JUN-1995
C!
C!   Inputs:  LUN    /I ! Logical unit number
C!
C!   Outputs: None
C!
C!   Calls:   None
C!
C!   Description
C!   ===========
C!     This routine exists to reserve the common block PAWC and
C!       to call the HLIMIT to initialize HBOOK.
C?
C?----------------------------------------------------------------------
      IMPLICIT NONE
C
C Declare routine arguments here
C-----------------------------------------------------------------------
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'HBOOKINIT' )
C
C Declare Function that are called
C-----------------------------------------------------------------------
C
C Declare CLEO include files here
C
C My include files here
C-----------------------------------------------------------------------
C
C Declare any parameters
c
      CALL LOAD_HBOOK
c
      RETURN
      END



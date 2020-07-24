      integer function get_size_pawc ( nothing )
c
c  04/27/00  Vladimir Savinov ( VPS )
c
c  This function returns the size of the Fortran 
c  common block PAWC in 4-byte words and, when 
c  called for the first time, verifies that the 
c  correct common block has been dynamically loaded. 
c  This function assumes that HLIMIT has not been 
c  called yet. 
c
c  Notice that the future code generator MUST generate 
c  exactly this function if the user wants to change 
c  the size of memory in the PAWC common block. 
c
      implicit NONE
c
      integer nothing
c
#include "seq/clutil/mesrep.inc"
c
      character*(*) CRNAME
      parameter   ( CRNAME = 'GET_SIZE_PAWC' )
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      logical  GET_INIT_HBOOK
c
      external GET_INIT_HBOOK
c
      logical  INIT_HBOOK
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
c HBOOK_MEMORY - Hbook memory
c HBOOK_NWORDS - Hbook memory size 
c
      integer     HBOOK_NWORDS
      parameter ( HBOOK_NWORDS = 2000000 )
c
      integer         HBOOK_MEMORY
      common / PAWC / HBOOK_MEMORY ( HBOOK_NWORDS )

      integer     MAGIC
      parameter ( MAGIC = 19842041 )

      data HBOOK_MEMORY / HBOOK_NWORDS * MAGIC /
c
      save   / PAWC /
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      logical FIRST
      data    FIRST / .true. /
c
c+++++++++++++++++++++++++++++++++++++++++++++++++++++++
c
      get_size_pawc = 0
c
c  Check if already initialized
c
      INIT_HBOOK = GET_INIT_HBOOK (  )
c
c  Flaw in the SUEZ/other modules/this module logistics
c
      if ( INIT_HBOOK .and. FIRST ) then
c
       CHMESS = 
     +'Logical mistake detected upon the first call:'
       call MESLOG ( CRNAME, 1, MSABOR )
c
       CHMESS = 
     +'HBOOK is ALREADY initialized!'
       call MESLOG ( CRNAME, 1, MSABOR )
c
       CHMESS = 
     +'Change the configuration of your job and try again.'
       call MESLOG ( CRNAME, 1, MSABOR )
c
       CHMESS = 
     +'Exiting the program due to potential memory corruption.'
       call MESLOG ( CRNAME, 1, MSABOR )
c
       stop
c
      endif
c
c  First time actually measure the size of the available memory :)
c
      if ( FIRST ) then
c
c  No longer the first time...
c
       FIRST = .false. 
c
c  Test the memory...
c
cc       CHMESS = 
cc     +'Verifying if the correct copy of common block PAWC was loaded'
cc       call MESLOG ( CRNAME, 1, MSINFO )
c
       call FOR_COM_MEMORY_GUARD ( HBOOK_MEMORY, MAGIC, HBOOK_NWORDS )
c
c  Cleared!
c
       call SET_SIZE_PAWC ( HBOOK_NWORDS )
c
      endif
c
      get_size_pawc = HBOOK_NWORDS
c
      return
      end




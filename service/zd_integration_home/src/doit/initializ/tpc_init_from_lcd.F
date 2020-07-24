*
* $Id: tpc_init_from_lcd.F,v 1.2 2003/05/12 16:09:18 dpp Exp $
*
* $Log: tpc_init_from_lcd.F,v $
* Revision 1.2  2003/05/12 16:09:18  dpp
*      -> use CLEO_DOITTPC to dummy this routine in CLEO production
*
* Revision 1.1  2003/03/05 18:05:49  dpp
*      -> NEW ROUTINE
*      -> extract the geometry infor supplied by LCD
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_INIT_FROM_LCD(
     2     ret_bfield,ret_IR,ret_or,ret_haflen,ret_nlay) 
C.......................................................................
C.
C. TPC_INIT_FROM_LCD - return geometry numbers from LCD
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 29-Oct-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C arguments
      real ret_bfield,ret_IR,ret_or,ret_haflen
      integer ret_nlay 

#if defined(CLEO_DOITTPC)

      integer i

#include "/home/axp/dpp/lcd_simulation/cornell/hep/lcd/io/fortran/lcdgeom.inc"

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      print 1000
 1000 FORMAT(' TPC_INIT_FROM_LCD: enter')

      print 1001,bfield
 1001 format(' TPC_INIT_FROM_LCD:      BFIELD=',F10.4)

      print 1002,(trackergeom(i),i=1,3)
 1002 format(' TPC_INIT_FROM_LCD: TRACKERGEOM=',3F10.4)

      print 1003,trackernlayers
 1003 format(' TPC_INIT_FROM_LCD:     NLAYERS=',I10)

      ret_bfield=bfield
      ret_haflen=trackergeom(3)/100.

      print 1004
 1004 format(' TPC_INIT_FROM_LCD: overide some input LCD values')

      ret_IR=0.5692
      ret_or=1.9000
      ret_nlay=144

      print 1005,ret_bfield,ret_IR,ret_or,ret_haflen,ret_nlay
 1005 format(' TPC_INIT_FROM_LCD:',
     1     '  bfield=',F6.3,
     2     '  innerR=',F6.3,
     3     '  outerR=',F6.3,
     4     '  haflen=',F6.3,
     5     '  nlay  =',I6)

C end of compliation flag CLEO_DOITTPC
#endif

      RETURN
      END







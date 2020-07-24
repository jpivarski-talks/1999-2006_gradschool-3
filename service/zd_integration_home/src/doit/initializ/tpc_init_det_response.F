
* $Id: tpc_init_det_response.F,v 1.1 2003/03/05 18:06:13 dpp Exp $
*
* $Log: tpc_init_det_response.F,v $
* Revision 1.1  2003/03/05 18:06:13  dpp
*      -> NEW ROUTINE
*      -> extract the geometry infor supplied by user
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_INIT_DET_RESPONSE(
     2     ret_nominal_cell_wid,ret_cell_z_width,ret_ncell_devis)
C.......................................................................
C.
C. TPC_INIT_DET_RESPOSE - return tpc detector response defined by user
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

      real ret_nominal_cell_wid,ret_cell_z_width
      integer ret_ncell_devis 

      SAVE

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      print 1000
 1000 FORMAT(' TPC_INIT_DET_RESPONSE: enter')

c      ret_nominal_cell_wid=.002
c      ret_nominal_cell_wid=.005
      ret_nominal_cell_wid=.010
      ret_ncell_devis=8
      ret_cell_z_width=10.

      RETURN
      END





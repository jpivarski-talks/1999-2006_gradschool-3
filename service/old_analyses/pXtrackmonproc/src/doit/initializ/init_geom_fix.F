*
* $Id: init_geom_fix.F,v 1.1 2001/11/19 23:59:26 dpp Exp $
*
* $Log: init_geom_fix.F,v $
* Revision 1.1  2001/11/19 23:59:26  dpp
*      -> NEW ROUTINE
*      -> fix a few geometry problems
*      -> split off from DUFIND
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :
      SUBROUTINE INIT_GEOM_FIX
C.......................................................................
C.
C. INIT_GEOM_FIX - fix a few problems in the CLEOIII geomtry
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : 09-Oct-01
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/svgeometry.inc"

      integer ihyb,idum
      real svwafer

C DUDE! this is necessary until the geometry is fixed
C       this is (only slightly more than) the minimum required for FINDHW 

      nwirtf(56)=1536
      nwrztf(56)=192
      phiftf(56)=3.14159265/8.
      celltf(56)=3.14159265/4.
      celztf(56)=.0116
      rtf(56)=rtf(55)
      rsqtf(56)=rsqtf(55)
C so, the above can be deleted when the geometry is verified



C DUDE! this necesssary because the silicon geomtry is wrong
C SVBOUNDARY_Z(hybrid,2)..... Z coordinate of boundary of wafer
      svwafer=0.053
      do 41 ihyb=1,7
         svboundary_z(   ihyb,1)=  1.5  *svwafer
         svboundary_z(61+ihyb,2)= -1.5  *svwafer
         if(ihyb.le.3)then
            svboundary_z(   ihyb,2)= -0.5  *svwafer
         else
            svboundary_z(   ihyb,2)=  0.5  *svwafer
         endif
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 41   continue
      do 42 ihyb=8,17
         svboundary_z(   ihyb,1)=  2.0  *svwafer
         svboundary_z(61+ihyb,2)= -2.0  *svwafer
         svboundary_z(   ihyb,2)=  0.0
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 42      continue
      do 43 ihyb=18,35
         svboundary_z(   ihyb,1)=  3.5  *svwafer
         svboundary_z(61+ihyb,2)= -3.5  *svwafer
         idum=ihyb/2
         if(2*idum.ne.ihyb)then
            svboundary_z(   ihyb,2)= -0.5  *svwafer
         else
            svboundary_z(   ihyb,2)=  0.5  *svwafer
         endif
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 43      continue
      do 44 ihyb=36,61
         svboundary_z(   ihyb,1)=  5.0  *svwafer
         svboundary_z(61+ihyb,2)= -5.0  *svwafer
         svboundary_z(   ihyb,2)=  0.0
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 44   continue
C so, the above can be deleted when the geometry is verified
      return
      end


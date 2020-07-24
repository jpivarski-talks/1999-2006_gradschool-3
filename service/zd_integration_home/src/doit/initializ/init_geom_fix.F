*
* $Id: init_geom_fix.F,v 1.2 2002/09/24 00:29:12 dpp Exp $
*
* $Log: init_geom_fix.F,v $
* Revision 1.2  2002/09/24 00:29:12  dpp
*      -> print out stuff to check the provided geometry, commented
*      -> remove some of the fixes that were verified
*
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
C. INIT_GEOM_FIX - fix a few problems in the CLEOIII geometry
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

c ok 10-sept-2002      nwirtf(56)=1536
c ok 10-sept-2002      nwrztf(56)=192
c ok 10-sept-2002      rtf(56)=rtf(55)
c ok 10-sept-2002      rsqtf(56)=rsqtf(55)

c wrong 10-sept-2002, found   =0.0
      PRINT 9903,phiftf(56)
 9903 FORMAT(' INIT_GEOM_FIX: phiftf(56)=',F9.7,'  WANT 0.3926881')
      phiftf(56)=3.14159265/8.

c wrong 10-sept-2002, found   =0.01157
      PRINT 9904,celltf(56)
 9904 FORMAT(' INIT_GEOM_FIX: celltf(56)=',F9.7,'  WANT 0.7853982')
      celltf(56)=3.14159265/4.

c wrong 10-sept-2002, found   =0.02
      PRINT 9905,celztf(56)
 9905 FORMAT(' INIT_GEOM_FIX: celztf(56)=',F9.7,'  WANT 0.0116')
      celztf(56)=.0116

      PRINT 9913,phiftf(54)
 9913 FORMAT(' INIT_GEOM_FIX: phiftf(54)=',F9.7,'  WANT 0.3926881')
      PRINT 9914,celltf(54)
 9914 FORMAT(' INIT_GEOM_FIX: celltf(54)=',F9.7,'  WANT 0.7853982')
      PRINT 9915,celztf(54)
 9915 FORMAT(' INIT_GEOM_FIX: celztf(54)=',F9.7,'  WANT 0.0116')


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










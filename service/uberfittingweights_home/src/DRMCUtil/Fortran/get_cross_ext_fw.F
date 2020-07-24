*
* $Id: get_cross_ext_fw.F,v 1.3 2000/11/08 15:10:52 dubrovin Exp $
*
* $Log: get_cross_ext_fw.F,v $
* Revision 1.3  2000/11/08 15:10:52  dubrovin
*  Insert check for track point radius
*
* Revision 1.2  2000/09/29 21:24:34  dubrovin
* Insert check for for repeat collision
*
* Revision 1.1  2000/09/27 20:02:29  dubrovin
* Add new function get_cross_ext_fw.F
*
*
      SUBROUTINE GET_CROSS_EXT_FW(layer,wtrk,bfield,
     >                            cross_fw,wirePars,w_fw,dca_fw)
*
* begin_doc
*
*  Documentation for subroutine get_cross_ext_fw
*
*  Purpose: Check the crossing of track with external field wire,
*           (external f.w. - wire on the boarder of the layer)
*  -------- In case of crossing, find dca and track parameters at dca point. 
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*   layer - integer   : layer #
*   wtrk(10)  - real  : W presentation of input track
*   bfield    - real  : z-component of the magnetic field
*
*  Output Parameters  :
*  -----------------
*   cross_fw - logical: =.true. if there is a crossing with field wire 
*   wirePars(10) -real: founded wire parameters: 
*                     : (1-3) - point; 
*                     : (4-6) - direction;
*                     : (7)   - diameter   <- input. 
*   w_fw(10)    - real: W representation of track parameters at dca
*   dca_fw      - real: distance of closest approach
*
*  Called by          : DRGeantHits/FRortran/cdhits.F 
*  --------
*
*  Call to            : cdhits_extfieldwire_position from /DRGeantHits/Class/CDHitsProxy.cc
*  -------
*
*  Author:   Mikhail Dubrovin      Created:  Mon Sep 25 14:16:32 EDT 2000
*  -------
*
* end_doc
*
*#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
*#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*   input variables:
      integer layer
      real    wtrk(10) 
      real bfield

*   output variables:
      logical cross_fw
      real    wirePars(3)
      real    w_fw(10)
      real    dca_fw
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: get_cross_ext_fw.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/11/08 15:10:52 $')
      PARAMETER(CHRCSA='$Author: dubrovin $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'GET_CROSS_EXT_FW' )
*
*  Local User defined variables:
*  -----------------------------
      real point_wire(3)
      logical success
      real arca

      real R_layer_center, Rtrkp
      integer ilayer

      real R_min(47), R_max(47), R_down, R_up
      real R_min_16,  R_max_16

      real     wtk_drift
      external wtk_drift
*
      INTEGER NWRITE

      real X_old, Y_old, Z_old
      save X_old, Y_old, Z_old

      save R_min,    R_max
      save R_min_16, R_max_16
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
      SAVE NWRITE
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
*
*  executable code for routine get_cross_ext_fw:
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
*----6----------------------------------------------------------------72
*
      IF (NWRITE .LT. 1) THEN
        NWRITE = NWRITE + 1
*===========================
*        CHMESS='This is a DUMMY routine, this message written once'
*        CALL MESLOG(CRNAME,1,MSINFO)
*         CHMESS=' Create array of tolerances to check field wire radius'
*         call MESLOG(CRNAME,1,MSINFO)
*===========================
       do ilayer=1,47

         call cdhits_rcd_down(ilayer, R_down)
         R_min(ilayer) = R_down - 0.01
         R_max(ilayer) = R_down + 0.08
*===========================
*         write(CHMESS,
*     >'('' Layer='',I3,'' R_min='',f10.5,'' R_max='',f10.5)')
*     >    ilayer,         R_min(ilayer),    R_max(ilayer)
*         call MESLOG(CRNAME,1,MSINFO)
*===========================
       enddo

         ilayer = 16
         call cdhits_rcd_up(ilayer, R_up)
         R_min_16 = R_up - 0.01
         R_max_16 = R_up + 0.08
*===========================
*         write(CHMESS,
*     >'('' Layer=16 UP  R_min_16='',f10.5,'' R_max_16='',f10.5)')
*     >                  R_min_16,            R_max_16
*         call MESLOG(CRNAME,1,MSINFO)
*===========================

      ENDIF


       cross_fw = .false.
       success  = .false.

* Radius of the track point
      Rtrkp = sqrt(wtrk(5)**2 + wtrk(6)**2)

*===========================
*      write(CHMESS,
*     >'('' L='',I3,'' Rt='',f10.5,'' Rmi='',f10.5,'' Rma='',f10.5)')
*     >     layer,     Rtrkp,         R_min(layer),   R_max(layer)       
*      CALL MESLOG(CRNAME,1,MSINFO)         
*===========================

      if(layer.eq.16) then 

        if( (Rtrkp.gt.R_min(layer).and. Rtrkp.lt.R_max(layer) )
     > .or. (Rtrkp.gt.R_min_16    .and. Rtrkp.lt.R_max_16))then
          call cdhits_extfieldwire_position( layer, wtrk(5), 
     >                                   wirePars(1), success )
        endif

      elseif(layer.eq.47) then

        if( Rtrkp.gt.R_min(layer).and. Rtrkp.lt.R_max(layer) )then
          call cdhits_extfieldwire_position( layer, wtrk(5), 
     >                                   wirePars(1), success )
        endif

      else

        if( Rtrkp.gt.R_min(layer).and. Rtrkp.lt.R_max(layer) )then
          call cdhits_extfieldwire_position( layer, wtrk(5), 
     >                                   wirePars(1), success )
        endif

        if( Rtrkp.gt.R_min(layer+1).and. Rtrkp.lt.R_max(layer+1) )then
          call cdhits_extfieldwire_position( layer+1, wtrk(5), 
     >                                   wirePars(1), success )
        endif

      endif

      if( success ) then

* Check for double hit at the same place within ~1mm
         if( abs(wirePars(1)-X_old) .le. 0.1 ) then
          if( abs(wirePars(2)-Y_old) .le. 0.1 ) then
           if( abs(wirePars(3)-Z_old) .le. 0.1 ) then

            RETURN
          
           endif
          endif
         endif

*       Find dca using curved track and stereo geometry.
        dca_fw=wtk_drift(wtrk,wirePars(1),wirePars(4),bfield,arca)

        if( abs(dca_fw).lt.wirePars(7) ) then
          cross_fw = .true.  

*===========================
*          write(CHMESS,*) 'Ext:have a mult.sc. Layer=',layer
*          CALL MESLOG(CRNAME,1,MSINFO)         
*          write(CHMESS,*) 'Ext:wtk_drift returned arca=',arca
*          CALL MESLOG(CRNAME,1,MSINFO)         
*===========================

*         Move the track to the wire DCA point
          call wtk_move(wtrk, arca, bfield, w_fw)

          X_old = wirePars(1)
          Y_old = wirePars(2)
          Z_old = wirePars(3)
          
        endif

      endif

      RETURN
      END




      SUBROUTINE ZD_GET_CROSS_EXT_FW(layer,wtrk,bfield,
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
*  Called by          : ZDGeantHits/FRortran/zdhits.F 
*  --------
*
*  Call to            : zdhits_extfieldwire_position from
*  -------              ZDGeantHits/Class/ZDHitsProxy.cc
*
*  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
      PARAMETER(CHRCSS='$RCSfile: zd_get_cross_ext_fw.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:42 $')
      parameter(chrcsa='$Author: huanggs $')
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

      real R_min(6), R_max(6), R_down, R_up

      real     wtk_drift
      external wtk_drift
*
      INTEGER NWRITE

      real X_old, Y_old, Z_old
      save X_old, Y_old, Z_old

      save R_min,    R_max
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
       do ilayer=1,6

         call zdhits_rcd_down(ilayer, R_down)
         R_min(ilayer) = R_down - 0.01
         R_max(ilayer) = R_down + 0.08
*===========================
*         write(CHMESS,
*     >'('' Layer='',I3,'' R_min='',f10.5,'' R_max='',f10.5)')
*     >    ilayer,         R_min(ilayer),    R_max(ilayer)
*         call MESLOG(CRNAME,1,MSINFO)
*===========================
       enddo

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

      if( Rtrkp.gt.R_min(layer).and. Rtrkp.lt.R_max(layer) )then
          call zdhits_extfieldwire_position( layer, wtrk(5), 
     >                                   wirePars(1), success )
      endif

      if( Rtrkp.gt.R_min(layer+1).and. Rtrkp.lt.R_max(layer+1) )then
          call zdhits_extfieldwire_position( layer+1, wtrk(5), 
     >                                   wirePars(1), success )
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
*          write(CHMESS,*) 'Ext:zd_wtk_drift returned arca=',arca
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

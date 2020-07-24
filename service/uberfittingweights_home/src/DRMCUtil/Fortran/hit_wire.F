*
* $Id: hit_wire.F,v 1.4 2003/02/25 15:24:57 cleo3 Exp $
*
* $Log: hit_wire.F,v $
* Revision 1.4  2003/02/25 15:24:57  cleo3
* minor changes needed to compile under g77
*
* Revision 1.3  2001/06/12 15:01:14  dubrovin
* New generators of random numbers
*
* Revision 1.2  2000/09/29 21:22:37  dubrovin
* First version of mult.scatt. on field wires
*
* Revision 1.1  2000/09/27 21:35:10  dubrovin
* Add subroutine hit_wire.F to generate
*
*
      SUBROUTINE HIT_WIRE(wirePars, wtrk, dca) 
*
* begin_doc
*
*  Documentation for subroutine hit_wire
*
*  Purpose: Generate multiple scattering and energy loss in wire,
*  -------- change GEANT track parameters
*
*  Input Parameters :  (Name - Type - Meaning)
*  ----------------
*  wirePars - real  : Parameters of the wire
*  wtrk - real      : w-track parameters at DCA
*  dca - real       : DCA
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*
*  Other routines   : called from DRGeantHits/Fortran/cdhits.F 
*  ---------------
*
*  Author:   Mikhail Dubrovin      Created:  Wed Sep 27 16:51:37 EDT 2000
*  -------
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "geant321/gctrak.inc"
* PI,TWOPI,PIBY2,DEGRAD,RADDEG,CLIGHT,BIG,EMASS
#include "geant321/gconst.inc"  
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      real wirePars(10)
      real wtrk(10)
      real dca
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: hit_wire.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/02/25 15:24:57 $')
      PARAMETER(CHRCSA='$Author: cleo3 $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'HIT_WIRE' )
*
*  Local User defined variables:
*  -----------------------------
*
        integer NWRITE

        real X0_Au,  X0_Al
        real Rwire,  Rcore, dR_Au
        real Rwire2, Rcore2

        save X0_Au,  X0_Al
        save Rwire,  Rcore, dR_Au
        save Rwire2, Rcore2

        real X_Au, X_Al, X_tot, dca2
        real Xtrk, Q, Pt, P, E, Sigma, Coef
        real*8 ranGaus1,ranGaus2
        real*8 rndm(5)
        real*8 RANDOM_MODULE_GAUSS

        real phi, theta, costhet, sinthet, cosphi, sinphi

        real UV(3), UVIN(3), UVINT, UVOUT(3), ROTM(3,3)
        integer i


*      real DIRCOSX
*
* end_var
*
*  Data statements:
*  ----------------
*
      save Coef
      SAVE RCSINI
      SAVE NWRITE
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
*
*  executable code for routine hit_wire:
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
*        CHMESS='This is a DUMMY routine, this message written once'
*        CALL MESLOG(CRNAME,1,MSINFO)

*------------------------------------------------------------------
* Field wires of DR3 are made of 110um AL 5056 wires, gold plated .75um
*------------------------------
*   AL 5056 aloy consist of: 
*   Aluminum         Al(Z=13)  X0=8.9cm 
*   Magnesium     5% Mg(Z=12)  X0 should be close to Al
*   Chrome     0.12% Cr(Z=24)  \ neglect as a smoll fraction
*   Manganese  0.12% Mn(Z=25)  /
*------------------------------
*   Gold             Au(Z=79)  X0=0.33cm (calculated from Dahe formulae
****** So we treated a wire as an Al core plated by Au. ******
*------------------------------
* Field wire parameters:

        X0_Au=0.33        ! cm, radiation length of the goald 
        X0_Al=8.9         ! cm, radiation length of the aluminum
        dR_Au=0.000075    ! cm       = 0.75um
        Rwire=wirePars(7) ! 0.0055cm = 55um, defined in cdhits.F
        Rcore=Rwire-dR_Au ! cm, radius of aluminum core
        Rwire2=Rwire**2
        Rcore2=Rcore**2
        Coef=0.0136 * 0.76 * 1.414213562  ! Coef = 13.6MeV*(1+ln)*sqrt(2)
* sqrt(2) is necessary to go from plane to space angle.
      ENDIF

* Check once more, that track is really crossing the wire

        if( abs(dca).gt.Rwire ) then
          write(CHMESS,*) 
     >         'No crossing with wire ??? -> inconsistant parameters:'
          CALL MESLOG(CRNAME,1,MSWARN)
          write(CHMESS,*) 'dca, Rwire=', dca, Rwire
          CALL MESLOG(CRNAME,1,MSWARN)

         RETURN

        endif

* Define the track pass in Al and Au

        dca2=dca**2
        X_tot = sqrt(Rwire2-dca2)

        if(dca2 .ge. Rcore2) then
          X_Au=X_tot
          X_Al=0.
        else
          X_Al = sqrt(Rcore2-dca2)
          X_Au = X_tot - X_Al
        endif

* Track pass in radiation length is:
*              ~1.24e-3     ~0.45e-3 in the center of the wire
*                     up to ~50.e-3 at the surface of wire!  
        Xtrk = X_Al/X0_Al + X_Au/X0_Au

* Take in to account track tilt angle WRT wire axes.
        if(Pt.ne.0) then
           Xtrk = Xtrk * P/Pt
*       else
* Track is going inside the wire along the axis, inprobable case!
* Wait some time, It will probably appeared again.   
        endif

* Multiple scattering angle:

      Q = abs(wtrk(10))
      Pt= wtrk(8)
      P = wtrk(9)
      E = wtrk(4)

      Sigma = Coef*Q*E / P**2 * sqrt(Xtrk)

*    >        (1. + 0.038*alog(Xtrk)) ~ 0.76
* Logarithmical term gives ~ -24.7% correction for most part
*                      and ~ -20% for wire border,
* at 11% accuracy of this expression in general. 
* To decrease consuming time it was changed to constant -0.24

* O'K, now I have Sigma and will generate multiple scattering angle.
* According to RPP 2000 prescription 98% of events are in the Gaussian core, 
* while the rest 2% ???. 
* Let them (2% events) to be isotropically distributed for the rest 2%.


C      call GRNDM(rndm,3)           ! Random array [0,1]
       call RANDOM_MODULE_FLAT_ARRAY( rndm, 3 )

         phi=TWOPI*rndm(1)           ! TWOPI from gconst.inc
         sinphi = sin(phi)
         cosphi = cos(phi)

      if( rndm(2).le.0.98 ) then
* Angular distribution for 98% fraction of events - Gaussian
c         call GRANOR(ranGaus1,ranGaus2) ! Norm.Random Gaussian distr.
         ranGaus1 = RANDOM_MODULE_GAUSS()

         theta = Sigma * ranGaus1

         costhet = cos(theta)
         sinthet = sin(theta)

      else
* Angular distribution for 2% fraction of events - isotropic

         costhet=2.*rndm(3)-1.
         if(costhet.lt.-1.) costhet=-1.
         if(costhet.gt. 1.) costhet=1.
         sinthet=sqrt(1.-costhet**2)

      endif
* So, we have unit vector of scattering WRT initial direction:

       UV(1)=sinthet*cosphi
       UV(2)=sinthet*sinphi
       UV(3)=costhet

* In order to rotate UV to Lab system we are using rotation matrix
* like at CLEO-II.V, which is a superposition of two Euler angles rotations
* around z and y' axes respectively: Ry'(beta)*Rz(alpha)

* Define unit vector of initial direction UVIN 
* and elements of rotation matrics ROTM

       do i=1,3
          UVIN(i)=wtrk(i)/P
          ROTM(i,3)=UVIN(i)
       enddo
             UVINT=Pt/P

          if(UVINT.ne.0.) then
             ROTM(1,2)=-UVIN(2)/UVINT
             ROTM(2,2)= UVIN(1)/UVINT
             ROTM(3,2)= 0.
             ROTM(1,1)= ROTM(2,2)*UVIN(3)
             ROTM(2,1)=-ROTM(1,2)*UVIN(3)
             ROTM(3,1)=-UVINT
          else
             ROTM(1,1)=1.
             ROTM(2,1)=0.
             ROTM(3,1)=0.
             ROTM(1,2)=0.
             ROTM(2,2)=1.
             ROTM(3,2)=0.
          endif

* To rotate an unit vector to the Lab frame, we have to use inverse rotation
* matrics, so for unitarity matrics it should be R(i,j)^-1=R(j,i)=R(i,j)^T

       do i=1,3
         UVOUT(i)=ROTM(i,1)*UV(1)+ROTM(i,2)*UV(2)+ROTM(i,3)*UV(3)
       enddo   

* New coordinate of the track is DCA point 
* I neglected by wire diameter etc., because 
* 1) it is small, diam=110um -> RMS is smaller
* 2) anyway we use integral expression for mul.sc. angle (not individual acts) 
* 3) I very doubt, that we know wire location with this precision

       VECT(1)=wtrk(5)
       VECT(2)=wtrk(6)
       VECT(3)=wtrk(7)

* One could also try to play here with energy loss, but ...
* except low probable high energy fluctuations, 
* the mean energy loss is <~0.07 MeV/wire. So, ionization energy loss itself 
* does not matter. Perhaps we have to care about hard interactions...
* with production of additional particles... I skip it for the moment...

       VECT(4)=UVOUT(1)*P
       VECT(5)=UVOUT(2)*P
       VECT(6)=UVOUT(3)*P
       VECT(7)=P

*       write(CHMESS,'(''Unit vector of scattering:'',3f10.6)'), UV
*       CALL MESLOG(CRNAME,1,MSINFO)
*       write(CHMESS,'(''Unit vector of P input   :'',3f10.6)'), UVIN
*       CALL MESLOG(CRNAME,1,MSINFO)
*       write(CHMESS,'(''Unit vector of P out     :'',3f10.6)'), UVOUT
*       CALL MESLOG(CRNAME,1,MSINFO)

* To turn track direction on 90 degree for test
*      DIRCOSX=VECT(4)
*      VECT(4)=-VECT(5)
*      VECT(5)=DIRCOSX


      RETURN
      END







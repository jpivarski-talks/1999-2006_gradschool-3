      SUBROUTINE ZDMC_IONDIST(   Layer,
     &                           cosZ,
     &                           sinXangle,
     &                           pathLength,
     &                           Eloss,
     &                           DriftDist,
     &                           IonDist )

*
* begin_doc
*
*  Documentation for subroutine zdmc_iondist
*
*  Purpose: Generate real drift distance due to ion statistics effect
*  --------               in stead of DCA
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*    Layer      - int  - layer number of considered hit 
*    cosZ       - real - cos of track angle to Z direction
*    sinXangle  - real - attack angle in r-phi plane
*    pathLength - real - path length of the hit in the drift cell, m
*    Eloss      - real - Energy loss, GeV/hit pathLength 
*                        for this hit
*    DriftDist  - real - D.C.A. from track(hit) to wire in m
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*    IonDist - real - ion distance
*
*  Other routines: 
*  ---------------
*    called from mc_zd_responses.F
*    call get_ions.F - to get threshold # of ions to trigger the wire  
*                      from constant DB
*
*  Notes: Add additional input parameters WRT CLEO II.V.
*  ------ Probably, not all of them will be used, but, presumably
*         the dependence on these variables should have a sense.
*
*  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
*  -------
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
* for IDRUN
#include "geant321/gcflag.inc"  
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------

      integer Layer
      real cosZ, sinXangle, pathLength, Eloss, DriftDist, IonDist
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: zdmc_iondist.F,v $')
      parameter(chrcsr='$Revision: 1.5 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/07 17:34:21 $')
      parameter(chrcsa='$Author: cleo3 $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'ZDMC_IONDIST' )
*
*  Local User defined variables:
*  -----------------------------
*
        INTEGER NWRITE

        logical Update  !  Flag to update constants for ion statistics
        integer Nthr    !  Threshold # of ions to trigger the wire
        real Delta      !  Half length of the hit part lenght containing
                        !  threshold # of ions 
        real Nions_per_hit ! # of ions per hit

*   variables to generate Poisson distribution table @ first entry

        real rM,rN,mubin,muran, Sum
        parameter (mubin=0.1)
        integer M,N,Mmax,Nmax, MtimesN, muind
        parameter (Mmax=100)
        parameter (Nmax=10, MtimesN=Mmax*Nmax)
        real Poisson(Mmax,Nmax)
        data Poisson/ MtimesN*0. /
        save Poisson
        real Probran

* The P_POISSON function is used to define table 
* for inverse Poisson distribution

        real     P_POISSON
        external P_POISSON

        real*8 RANDOM_MODULE_FLAT
        integer LOCATR

*-----------------------------------------------------------------------
* !!!!!!!!!!!!! VERY IMPORTANT PARAMETER !!! should be ajusted at tuning
*-----------------------------------------------------------------------
* kElos_to_Nion - Conversion coefficient transforms the energy loss, GeV
*    to number of ions per hit in particular gas. Should be tuned.
*    What we know about this coefficient? 
*    * The gas mixture is He 60% - propane C3H8 40%
* >>>>>>>> Radiation length. Data from PDG/RPP 2000
* X0(He   @STP) = 94.32 g/cm^2 / 0.1786*10^-3 g/cm^3 = 5281 m 
* X0(C3H8 @STP) = 45.2  g/cm^2 / 1.879 *10^-3 g/cm^3 = 240.6 m 
* 1/X0(Mixture) = 0.6/X0(He) + 0.4/X0(C3H8) -> X0(Mixture)=563m
* 
* >>>>>>>> Mean energy loss, charge/keV
* According to dpp information we have 32.6 i.e./cm @STP for m.i.p.
* densiti(mixture) = 0.6*0.1786*10^-3 g/cm^3 + 0.4*1.879 *10^-3 g/cm^3
*                  = 0.8588*10^-3 g/cm^3
* <dE/dx> = 2 MeV /(g/cm^2) * 0.8588*10^-3 g/cm^3 = 1.718 keV/cm
* Finally we could conclude dNi.p./dE = 32.6 i.e./cm / 1.718 keV/cm
*                                = 18.98 i.e. /keV ~ 19
* GeV -> keV factor 10^6

        real       kElos_to_Nion
*       parameter (kElos_to_Nion = 19e6) ! at Nthr = 1 cluster
*       parameter (kElos_to_Nion = 60e6) ! V3, V5, V16 at Nthr = 1 cluster
*       parameter (kElos_to_Nion = 40e6) ! V17 at Nthr = 1 cluster
*       parameter (kElos_to_Nion = 35e6) ! Number obtained from Ahren
*       parameter (kElos_to_Nion = 50e6) ! V4
        save       kElos_to_Nion
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
*  executable code for routine zdmc_iondist:
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

                            kElos_to_Nion = 50e6
        if(IDRUN.eq.116044) kElos_to_Nion = 35e6

        write(CHMESS,*),'For run #',IDRUN,' Nions/GeV=',kElos_to_Nion
*       CHMESS='Initialization of ion statistics simulation'
        CALL MESLOG(CRNAME,1,MSINFO)


c        call test_ions
        Update = .false.

        do N=1,Nmax

          Sum=0.
          do M=1,Mmax
            rN=float(N)
            rM=(float(M)-0.5)*mubin 
c           Poisson(M,N)=P_POISSON(rN,rM) ! See P_POISSON @ end of this file
            Sum = Sum + P_POISSON(rN,rM)  ! See P_POISSON @ end of this file
            Poisson(M,N)=Sum              ! In order to eliminate call RNHPRE
          enddo
          
c         Once per each histogram
c          print '('' Poisson :'',10f10.5)',(Poisson(M,N),M=1,10)
c         call RNHPRE(Poisson(1,N),Mmax)
c         print '('' Prepared:'',10f10.5)',(Poisson(M,N),M=1,10)
c I have found that RNHPRE have changed input probability array
c Poisson(1,N) to the normalized integral values

        enddo

c      print '('' Poisson(i,Nthr=1) :'',10f10.5)',(Poisson(M,1),M=1,Mmax)

      ENDIF

*        call get_ions(Update, Layer, Nthr)

        Nions_per_hit = kElos_to_Nion * Eloss

      if( Nions_per_hit.le.0. ) Nions_per_hit=1. 

c      call RNPSSN(AMU, N, ERR) ! it's unfortunately direct Poisson generator

c For simplicity,
c     Delta = 0.5*pathLength * Nthr / Nions_per_hit      

c Actually Delta should be obtained from inverse Poisson distribution.
c Based on Nthr we have to find the random value for <N>,
c where <N> = Nions_per_hit / pathLength * 2*Delta,
c Hence, if <N> = muran

c Check for an array index overflow 
      if(Nthr.gt.Nmax) then
         write(CHMESS,'(''!!! Nthr.gt.Nmax='',2i5)'),Nthr,Nmax
         CALL MESLOG(CRNAME,1,MSINFO)
         Nthr=Nmax
      endif

      if(Nthr.lt.1) then
         write(CHMESS,'(''!!! Nthr.lt.1='',2i5)'),Nthr
         CALL MESLOG(CRNAME,1,MSINFO)
         Nthr=Nmax
      endif

c     call RNHRAN(Poisson(1,Nthr),Mmax,0.,mubin,muran)

      Probran=RANDOM_MODULE_FLAT()*Poisson(Mmax,Nthr)
      muind = LOCATR( Poisson(1,Nthr), Mmax, Probran )    ! find random bin#

      muran = mubin*(float(iabs(muind)) + sngl(RANDOM_MODULE_FLAT()))

c      write(CHMESS,'(''muind, muran='',i5,f10.3)'),muind,muran
c      CALL MESLOG(CRNAME,1,MSINFO)

c Check that obtained random mean is less than available number of
c ions in the hit

      if(muran.lt.Nions_per_hit) then
         Delta = 0.5*pathLength * muran  / Nions_per_hit   
      else
         Delta = 0.5*pathLength ! Actually, this is a case of non-efficiency!
      endif

         IonDist = DriftDist**2 + Delta**2*(1.-cosZ**2)

      if(IonDist.le.0.) then
         IonDist = 0.
      else
         IonDist = sqrt( IonDist )
      endif

      if(DriftDist.lt.0.) IonDist = -IonDist

#if defined(DEBUG_RESPONSE)
      write(CHMESS,*) '********* ZDMC_IONDIST in/out variables'
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''Layer='',i3,'' Nthr='',i3)'),Layer,Nthr
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''cosZ='',f6.3,'' sinXangle='',f6.3)'),
     >                 cosZ,           sinXangle 
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''Eloss='',f12.8,'' DriftDist='',f10.6)'),
     >                 Eloss,            DriftDist
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''ions/hit='',f9.3,'' IonDist='',f10.6)'),
     >                 Nions_per_hit,      IonDist
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''random <N>='',f6.3,'' pathLength='',f10.6)'),
     >                 muran,                pathLength
      CALL MESLOG(CRNAME,1,MSINFO)
      write(CHMESS,'(''    Delta ='',f10.6)'),Delta
      CALL MESLOG(CRNAME,1,MSINFO)
#endif

      RETURN
      END


*============================================================
* Poisson probability, direct calculation
* with mean value rmu for variable rn
* Author: Mikhail Dubrovin

      real function P_POISSON(rn,rmu)

      real rn,rmu
      integer i, N
      double precision P, dmu

      N=int(rn)
      dmu=dble(rmu)

      if(N.lt.0 .or. rmu.lt.0.) then
        print *,' Poisson : wrong input N or mu =',N,rmu
        P_POISSON=0.
        return
      endif

c        print *,' Poisson : input N or mu =',N,rmu,dmu

      P=1d0

      if(N.ne.0) then
        do i=1,N
           P=P*dmu/dble(i)
        enddo
      endif

      P_POISSON=P*dexp( -dble(rmu) ) 

      return

      end

C 
C  $Id: drmc_cdctbr.F,v 1.2 2003/02/25 15:24:56 cleo3 Exp $
C 
C  $Log: drmc_cdctbr.F,v $
C  Revision 1.2  2003/02/25 15:24:56  cleo3
C  minor changes needed to compile under g77
C
C  Revision 1.1  1999/09/20 22:48:08  jjo
C  Import files into cvs
C
C 
      SUBROUTINE DRMC_CDCTBR
C 
C  begin_doc
C 
C   Documentation for subroutine drmc_cdctbr
C 
C   Purpose: Open the histograms needed for cathodes simulation at Begin Run
C   --------
C 
C   Input Parameters:  None
C   ----------------
C 
C   Output Parameters: None
C   -----------------
C 
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C   Based on CLEO-II routine cdctbr.F by V. Boisvert
C   We have 2d histo of Q vs ADC, we make 1d histo of Q for each ADC bin.
C   Used in getchrg in cdrawm
C   We have 2d histo of Q vs Npad, we make 1d histo of Npad for each Q bin.
C   Used in getnp in cdrawm
C 
C   Author:   John J O'Neill      Created:  Thu Aug 12 13:36:54 EDT 1999
C   -------
C 
C   Detailed description:
C   ---------------------
C 
C   Major revisions:
C   ----------------
C      
C 
C  end_doc
C 
      IMPLICIT NONE
 
C  begin_inc
C   Include files :
C   ---------------------
C   Cleo3 Message Reporter
#include "ToolBox/report.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
 
C 
C   Local User defined variables:
C   -----------------------------
C   pawdir_save - paw current working directory:
      character*80 pawdir_save

C   cathname - partial file name of cathode histogram file
C              currently in $Z_BANK.  
C              The size of this must match what's in the constants database
      character*4 cathname

C   cathFileName - full cathode histogram file name
C   oldFileName  - save copy of old file name to see if we've read it in
C   actualCathFileName - with $Z_BANK expanded to full pathname
      character*80 cathFileName, oldFileName, actualCathFileName

C   FileNameLength - number of character the filename is
C   lunCathFile - logical unit number of cathode histogram file 
      integer FileNameLength, lunCathFile

C   icycle - paw/hbook variable used when reading in file 
      integer icycle



C ----------------------------------------------------------------------------------
C Variables copied from cdctbr - need to get defs for these

      INTEGER I,J,K,L
      INTEGER A,B,E,F,G,HIST,DX,DY
      REAL D,A1,HIJ,R1,C,XNENT,HSUM
      EXTERNAL HIJ,HSUM

C DR:
      REAL      DQDR, QMINDR, QMAXDR
      INTEGER   QBINSDR, ADC_MINDR, ADC_MAXDR, ADCBINSDR
      PARAMETER (ADC_MINDR = 3, ADC_MAXDR = 6)      
      PARAMETER (QMINDR = 0., QMAXDR = 50000., QBINSDR = 100)
      PARAMETER (ADCBINSDR = 50)
      PARAMETER (DQDR = (QMAXDR - QMINDR)/QBINSDR)

      INTEGER   SLICES_CD, PREFIX_CD, OFFSET_CD
      PARAMETER (SLICES_CD = 16, PREFIX_CD = 10000, OFFSET_CD = 12000)

* NPAD:
      REAL      DXNP, XMINNP, XMAXNP
      INTEGER   NBINSNP, Q_MINNP, Q_MAXNP, SLICES_NP, QBINSNP
      INTEGER   PREFIX_NP, OFFSET_NP
      PARAMETER (SLICES_NP = 4, PREFIX_NP = 11000, OFFSET_NP = 18000)
      PARAMETER (Q_MINNP = 1, Q_MAXNP = 5)
      PARAMETER (XMINNP = 0., XMAXNP = 10., NBINSNP = 10)
      PARAMETER (QBINSNP = 100)
      PARAMETER (DXNP = (XMAXNP - XMINNP)/NBINSNP)
C ----------------------------------------------------------------------------------

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: drmc_cdctbr.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/25 15:24:56 $')
      parameter(chrcsa='$Author: cleo3 $')
 
      character*(*) crname
      parameter(    crname = 'drmc_cdctbr' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine drmc_cdctbr:
#if WRITE_RCS_INFO
      if(rcsini)then
        rcsini = .false.
        write(6,*)'-------- CVSinfo --------'
        write(6,*)chrcss
        write(6,*)chrcsr//' '//chrcst
        write(6,*)chrcsd//' '//chrcsa
      endif
#endif
C ----6----------------------------------------------------------------72
 
      if (lfirst) then
        chmess='Open histograms for cathodes simulation at Begin Run'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif

C     Save the current paw working directory
      call hcdir(pawdir_save,'R')

C     Get the name of the histo file from constants database
      call drmc_cathname( cathname )
C     We only store 4 characters in constants database 
C     for CLEO-II historical reasons.  
C     TODO: Need to put entire file name into constants database
      cathFileName = '4s'//cathname//'h.rzn'

C     Check to see if we've already read in this histogram
      IF ( cathFileName .NE. oldFileName ) THEN
          oldFileName = cathFileName
C         Add $Z_BANK to file name
          FileNameLength  = INDEX(cathFileName, ' ') - 1
          cathFileName = '$Z_BANK/'//cathFileName(1:FileNameLength)
C         Translate filename - expand $Z_BANK env. variable into full filepath
          call trnfnm ( cathFileName, actualCathFileName )
          FileNameLength  = INDEX(actualCathFileName, ' ') - 1
          chmess = 'Reading cathode histograms from '
     &             //actualCathFileName(1:FileNameLength)
          call report(MS_INFO, crname, chmess)

C         Get logical unit number for cathode histogram file
          call getlun( lunCathFile, crname )

C         Open file - platform dependent          
#if defined(CLEO_RS6000) || defined(CLEO_HPUX)
          open(UNIT=lunCathFile,FORM='UNFORMATTED',
     &         FILE=actualCathFileName,
     &         RECL=4096,ACCESS='DIRECT',STATUS='OLD')
#else
          open(UNIT=lunCathFile,FORM='UNFORMATTED',
     &         FILE=actualCathFileName,
     &         RECL=1024,ACCESS='DIRECT',STATUS='OLD')
#endif

          call hmdir('//PAWC/CATHOD','S')
          call hrfile(lunCathFile,'PAWC/CATHOD',' ')
          ICYCLE=0

C Charge vs ADC plots
C Now do DR

          DO I=ADC_MINDR,ADC_MAXDR
              DO J=1,SLICES_CD
                  HIST = PREFIX_CD+(I*100)+J
                  CALL HRIN(HIST, ICYCLE,0)
C Map between 2dhisto # and 1d histo slice # ... see getchrg.F and getnpd.F
                  A = (INT((HIST-PREFIX_CD)/100))*100
                  B = A*10+OFFSET_CD
                  C = HIST - (PREFIX_CD+A)
                  D = (C+1)/2
                  E = INT(D) * 100
                  F = (D-INT(D))*100
                  G = B+E+F
                  DO K=1,ADCBINSDR
                      CALL HBOOK1(G+K,'R',QBINSDR,QMINDR,QMAXDR,0.)
                      DO L=1,QBINSDR
                          A1=0.
                          IF(L.GT.0 .AND. L.LE.QBINSDR)THEN
                              A1=HIJ(HIST,K,L)
                          ENDIF
                          R1=L*DQDR-DQDR*0.5
                          CALL HFILL(G+K,R1,0.,A1)
                      END DO
                  END DO
                  CALL HDELET(HIST)
              END DO
          END DO


C Charge vs Npad plots
          DO I=Q_MINNP, Q_MAXNP
              DO J=1,SLICES_NP
                  HIST = PREFIX_NP+(I*100)+J
                  CALL HRIN(HIST, ICYCLE,0)
                  A = (INT((HIST-PREFIX_NP)/100))*100
                  B = A*10+OFFSET_NP
                  C = HIST - (11000+A)
                  G = B+C*100
                  DO K=1,QBINSNP
                      CALL HBOOK1(G+K,'R',NBINSNP,XMINNP,XMAXNP,0.)
                      DO L=1,NBINSNP
                          A1=0.
                          IF(L.GT.0 .AND. L.LE.NBINSNP)THEN
                              A1=HIJ(HIST,L,K)
                          ENDIF
                          R1=L*DXNP-DXNP*0.5
                          CALL HFILL(G+K,R1,0.,A1)
                      END DO
                  END DO
                  CALL HDELET(HIST)
              END DO
          END DO
          
          CALL HREND('PAWC/CATHOD')
          CLOSE(lunCathFile)        
      END IF


C     Change back to paw directory  
      CALL HCDIR(pawdir_save,' ')

      return
      end




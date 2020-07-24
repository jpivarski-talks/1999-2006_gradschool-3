*
* $Id: cdwtqd.F,v 1.4 2003/03/05 17:48:06 dpp Exp $
*
* $Log: cdwtqd.F,v $
* Revision 1.4  2003/03/05 17:48:06  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.3  1998/07/09 00:39:50  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.2  1998/02/24 18:24:19  dpp
* restore the pilot.h
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :    Suzanne Jones 20/05/92
        REAL FUNCTION CDWTQD( LAYER, IADR)
C.......................................................................
C.
C. CDWTQD - Returns the weight to be assigned to a given hit in the VD based
C.          on charge division
C.
C. Inputs    : LAYER = cd layer number
C.           : IADR = address of charge division hit
C.
C.           :
C. Outputs   : weight to be used in fit, in inverse squared meters.
C.
C. Author    : Maurice Garcia-Sciveres   17/01/90  12.48.32
C.           : Suzanne Jones             20/05/92
C.
C. Detailed description
C.  Taking the inverse mean squared residual as the weight would only be
C.  correct for a gaussian distribution. Our distribution is sort of gaussian
C.  but with some tails, which means that the m.s. residual may be dependent
C.  on where the tails are cut. Since DUET needs the distribution of residuals
C.  to be Gaussian in order to correctly compute error matrices, we fit the
C.  residual distribution (excluding tails) to a Gaussian, and let the sigma
C.  of the fit determine the weight. This means that DUET will have to cut
C.  on residual distribution tails to come up with a decent error matrix.
C.  The sigma vs. inverse pulse height function is close to a straight line and
C.  hence is stored by a slope and a y-intercept.  A value for inverse pulse
C.  height is supplied and  the value for sigma determined from the equation
C.  of the line for that particular layer.
C.
C. Modification (09/10/95, J. Lee)
C.  1. Use layer numbers compatible with SVX in use 
C.  2. Correct total pulse height for the case when VD east hits
C      are primary.    
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -----------------Argument Declarations--------------------
            INTEGER LAYER, IADR
* -----------------SEQUENCE Declarations--------------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/qdcom.inc"
#include "seq/clutil/mesrep.inc"
* ------------------------- Local declarations -------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: cdwtqd.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/03/05 17:48:06 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'CDWTQD' )
      REAL SLOPES(10),SLPDAT(10)
      REAL YINTCPT(10),YINDAT(10)
      EQUIVALENCE(SLPDAT(1),SLOPES(1))
      EQUIVALENCE(YINDAT(1),YINTCPT(1))
      REAL SIGMA, PLSHGT
      INTEGER IDLAY, IWIR, IHIS
*
      SAVE
*  ------------------------- DATA  initializations ---------
      DATA RCSINI/.TRUE./
C-- Ten slope and y-intercept values for charge division
C-- layers follow.
C--
C--      sigma(inverse pulse height) = slope(layer)(inverse pulse height)+
C--                                            yintcpt(layer)
C--
C-- Values given are in centimeters (will be converted to meters).
C--
C---------------
C--  Assign resolutions by layer: different layers have different resolutions
C--
        DATA SLPDAT/0.64,0.66,0.64,0.79,0.62,0.69,0.65,0.74,0.83,0.50/
        DATA YINDAT/0.88,0.91,0.91,0.92,1.18,0.82,0.86,1.03,1.12,1.43/
C.
* ---------------------- Executable code starts here -------
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif

C-- Code executed every call follows:
C------------------------------------

      IF (IDTOCD(IVDET).EQ.0) GO TO 50

      IDLAY = LAYER - ILCDTF( ILDVCD(1,IVDET) ) + 1
      CDWTQD= 0.0

C-- We need to extract the gain-corrected total charge for the hit since our
C-- weights are given in terms of this quantity.

      IWIR=EvntHit_laWire(IADR)
      IF (IDLAY .LE. 5) THEN
          IHIS=(IDLAY-1)*64+IWIR+1
      ELSE
          IHIS=5*64 + (IDLAY-6)*96 + IWIR +1
      ENDIF
C--   Get pulse height in CLEO units
      PLSHGT=(
     1     GAIN(IHIS)*EvntHit_Charge(IADR)
     2     +EvntHit_Charge(EvntHit_NxtCls(IADR))
     3     )/10

CW2E  For the case when VD east hits are primary (note that Gain=W/E). 
      IF (LUSVDE) THEN 
        PLSHGT=(
     1       EvntHit_Charge(IADR)+
     2       GAIN(IHIS)*EvntHit_Charge(EvntHit_NxtCls(IADR))
     3       )/10
      ENDIF

C.
C--  We really use inverse pulse height...
      PLSHGT = (1/PLSHGT)
C--  Correct for slopes' units:
      PLSHGT = PLSHGT*1000
C-- Calculate sigma in meters and 1.0/sigma**2
      SIGMA = (SLOPES(IDLAY)*PLSHGT + YINTCPT(IDLAY))*1.0E-2
      IF (SIGMA.NE.0.0) CDWTQD = 1.0/SIGMA**2

      GO TO 100

50    WRITE(CHMESS,60)LAYER
60    FORMAT('VD is not used since the layer number is',I4)
      CALL MESLOG('CDWTQD',1,MSERRO)

100   RETURN
      END

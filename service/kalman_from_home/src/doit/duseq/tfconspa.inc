*
* $Id: tfconspa.inc,v 1.2 2001/11/19 23:28:50 dpp Exp $
*
* $Log: tfconspa.inc,v $
* Revision 1.2  2001/11/19 23:28:50  dpp
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:08  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/10/91  11.55.02  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.21  by  Steve Schaffner
*-- Author :
* DUET TF constants
*
* tfconspa.inc
*
#if !defined(CLEO_NOCOMDOC)
* PI     - Pi
* HALFPI - PI / 2.0
* TWOPI  - PI * 2.0
* RADDEG - PI / 180.0
* DEGRAD - 180.0 / PI
*
* ROOT2  - SQRT(2)
* ROOT3  - SQRT(3)
*
* AMTOMU - Meters / Microns
* PCONST - Convert curvature to GeV
* SCTGEV - Multiple scattering coefficient
*
* AMK0   - K0       mass
* AML0   - Lambda   mass
* AMEL   - Electron mass
* AMMU   - Muon     mass
* AMPI   - Pion     mass
* AMKA   - Kaon     mass
* AMPR   - Proton   mass
* AMEL2  - AMEL**2
* AMMU2  - AMMU**2
* AMPI2  - AMPI**2
* AMKA2  - AMKA**2
* AMPR2  - AMPR**2
* AML02  - AML0**2
* AMK02  - AMK0**2
#endif

      REAL       PI
      PARAMETER( PI     =  3.1415926536)
      REAL       HALFPI          ,   TWOPI
      PARAMETER( HALFPI =  PI/2.0,   TWOPI  =  PI*2.0           )
      REAL       RADDEG            , DEGRAD
      PARAMETER( RADDEG =  PI/180.0, DEGRAD =  180.0/PI         )

      REAL       ROOT2             , ROOT3
      PARAMETER( ROOT2  =  1.414214, ROOT3  =  1.732051         )

      REAL       AMTOMU
      PARAMETER( AMTOMU =  1.0E6 )
      REAL       PCONST
      PARAMETER( PCONST =  0.0149896229)
      REAL       SCTGEV
      PARAMETER( SCTGEV =  0.0136)

      REAL       AMK0            ,   AML0
      PARAMETER( AMK0   = 0.49767,   AML0   = 1.1156                  )
      REAL       AMEL             ,  AMMU
      PARAMETER( AMEL   = 0.000511,  AMMU   = 0.10565943              )
      REAL       AMPI              , AMKA
      PARAMETER( AMPI   = 0.1395673, AMKA   = 0.493667                )
      REAL       AMPR
      PARAMETER( AMPR   = 0.9382796                                   )
      REAL       AMEL2           , AMMU2           , AMPI2
      PARAMETER( AMEL2  = AMEL**2, AMMU2  = AMMU**2, AMPI2  = AMPI**2 )
      REAL       AMKA2           , AMPR2           , AML02
      PARAMETER( AMKA2  = AMKA**2, AMPR2  = AMPR**2, AML02  = AML0**2 )
      REAL       AMK02
      PARAMETER( AMK02  = AMK0**2                                     )

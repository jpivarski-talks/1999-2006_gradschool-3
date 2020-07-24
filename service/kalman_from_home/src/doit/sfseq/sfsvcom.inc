*
* $Id: sfsvcom.inc,v 1.2 2001/11/20 17:52:38 dpp Exp $
*
* $Log: sfsvcom.inc,v $
* Revision 1.2  2001/11/20 17:52:38  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:42  clib
* Developmental version of DUET.
*
* Revision 1.1  1997/02/25 22:06:44  dpp
* first version: commons and pars for SF treatment of SVX
*
*
* ------------------------------------------
* COMMON BLOCKS FOR SF SVX sparcifier bit
*
* sfsvcom.inc
*
#if !defined(CLEO_NOCOMDOC)
C SFSVML.....PARAMETER maximum number of layers
C SFSVMC.....PARAMETER maximum number cells
C ISVXSP.....serial number for SVX SParcifier device

C /SFSVCI/   integer
C SFSVNL.....number of layers used
C SFSVCL.....number of cells in each layer
C SFSVIN.....index in SV map of first cell in SV layer (LAYER,WE)
C SFLASV.....SF layer of SV layer number
C SFSVDV.....device number for layer

C /SFSVCR/   real
C SFSVRD.....radius
C SFSVF0.....phi zero

C /SFSVDI/   data section
C  SFSVVA.... = 1 : data valid
C  SFSVWE....control; 1=use west, 2=use east
C  SFSVMP....MAP
C  SFSV1S....first element for SFSV layer in map
C  SFSVNX....next element, same layer, for map element
#endif

      INTEGER SFSVML,SFSVMC
      PARAMETER (SFSVML=4)
      PARAMETER (SFSVMC=210)

      INTEGER ISVXSP
      PARAMETER (ISVXSP=41)

      INTEGER SFSVNL,SFSVCL,SFSVIN,SFLASV,SFSVDV
      REAL    SFSVRD,SFSVF0
      INTEGER SFSVVA,SFSVWE,SFSVMP,SFSV1S,SFSVNX

      COMMON/SFSVCI/SFSVNL,        SFSVCL(SFSVML),SFSVIN(SFSVML,2),
     2              SFLASV(SFSVML),SFSVDV(SFSVML)
      COMMON/SFSVCR/SFSVRD(SFSVML),SFSVF0(SFSVML)
      COMMON/SFSVDI/SFSVVA,        SFSVWE,
     2              SFSVMP(SFSVMC),
     3              SFSV1S(SFSVML),SFSVNX(SFSVMC)










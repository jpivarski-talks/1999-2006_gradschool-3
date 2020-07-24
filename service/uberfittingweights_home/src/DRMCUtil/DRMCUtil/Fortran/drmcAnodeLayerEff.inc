C      Include file drmcAnodeLayerEff
C
C begin_doc
C
C  $Id: drmcAnodeLayerEff.inc,v 1.1 1999/09/20 22:47:17 jjo Exp $
C
C  $Log: drmcAnodeLayerEff.inc,v $
C  Revision 1.1  1999/09/20 22:47:17  jjo
C  Import files into cvs
C
C
C  Documentation for include file drmcAnodeLayerEff
C
C  Purpose: Average DR Anode wire efficiency constants for a given layer
C  --------
C
C  Used in routines:
C  ----------------
C
C  Notes:
C  ------
C
C  Author:   John J O'Neill      Created:  Tue Aug 17 17:18:55 EDT 1999
C  -------
C
C  Major revisions:
C  ----------------
C
C end_doc
C
C
C  Remember NO NESTED INCLUDES (except for parameters)
C
C  COMMON BLOCK NAME should be the same as include file name!
C
C begin_var
C  variables for include file drmcAnodeLayerEff:
C  -----------------------------------
C  KLYRCD - from seq/cdgeom/cdgeompa.inc
C           maximum number of allowed CD layers.

C  EFF_LAYER - Average layer efficiency found by taking the average
C              of all the live channels in the layer.

C  EFF_LAYER(I) = average LIVE wire efficiency for wire I

C  SPEFDG - Species dependent efficiency fudge.

C----6----------------------------------------------------------------72
C   
C  Variables contained in drmcAnodeLayerEff common block
C  --------------------------------------------
 
C  end_var
C 
      REAL EFF_LAYER, SPEFDG
      COMMON/DRMCANODELAYEREFF/EFF_LAYER(KLYRCD), SPEFDG(KLYRCD,5)
C----6----------------------------------------------------------------72

      SAVE /DRMCANODELAYEREFF/




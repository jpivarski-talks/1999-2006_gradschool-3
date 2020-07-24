C      Include file zdmcAnodeLayerEff
C
C begin_doc
C
C  Documentation for include file zdmcAnodeLayerEff
C
C  Purpose: Average ZD Anode wire efficiency constants for a given layer
C  --------
C
C  Used in routines:
C  ----------------
C  seems never been referenced (?)
C
C  Notes:
C  ------
C
C  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
C  variables for include file zdmcAnodeLayerEff:
C  -----------------------------------
C  KLYRCD - from seq/cdgeom/cdgeompa.inc
C           maximum number of allowed CD layers.

C  EFF_LAYER - Average layer efficiency found by taking the average
C              of all the live channels in the layer.

C  EFF_LAYER(I) = average LIVE wire efficiency for wire I

C  SPEFDG - Species dependent efficiency fudge.

C----6----------------------------------------------------------------72
C   
C  Variables contained in zdmcAnodeLayerEff common block
C  --------------------------------------------
 
C  end_var
C 
      REAL EFF_LAYER, SPEFDG
      COMMON/ZDMCANODELAYEREFF/EFF_LAYER(KLYRCD), SPEFDG(KLYRCD,5)
C----6----------------------------------------------------------------72

      SAVE /ZDMCANODELAYEREFF/

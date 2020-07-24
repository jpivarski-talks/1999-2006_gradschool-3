*
*  Bunch finder results from TFBN 
*
*  tfbnch.inc
* 
#if !defined(CLEO_NOCOMDOC)
*
* DUBNCH  Documentation
*
* IBNTF      - Bunch # determined in TFBN
*
* NTRAIN     - Number of trains  
* NBNCHS     - Number of bunches 
* ISHFLG     - Flag to shift or not 
*
* BUNSPC     - Bunch spacing 
*
* TFBNLG(5)  - Log(prob) in TFBN for each bunch hypothesis
* TFBNLR     - Log ratios between the best one and all other bunch
*              hypotheses summed over all TOF hits
* TFBNCF     - Confidence level from TFBNLR
#endif
*
      INTEGER IBNTF, NTRAIN, NBNCHS, ISHFLG

      COMMON /ITBNCM/ 
     +  IBNTF, NTRAIN, NBNCHS, ISHFLG

      REAL TFBNLG, TFBNLR, TFBNCF, BUNSPC

      COMMON /RTBNCM/
     +  TFBNLG(5), TFBNLR, TFBNCF, BUNSPC
* --------------------------------------------------------------------

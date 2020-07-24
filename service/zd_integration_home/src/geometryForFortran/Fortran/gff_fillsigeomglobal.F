

*
*
      SUBROUTINE GFF_FILLSIGEOMGLOBAL()
*
* begin_doc
*
*  Documentation for subroutine gff_fillsigeomglobal
*
*  Purpose: Fill global Si information in  svgeometry.inc (called by 
*       fillFortranSVGeomGeometry)
*
*
*
*
*   This function fills the following variables in svgeometry.inc:
*
*
*  --------
*
*  Author:   Peter Gaidarev      Created:  Sat Apr 1 16:10:53 EDT 2000
*  -------
*
*  Major revisions:
*  ----------------
*     
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
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/svgeometry.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: gff_fillsigeomglobal.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/05/03 15:42:59 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'GFF_FILLSIGEOMGLOBAL' )
*
*  Local User defined variables:
*  -----------------------------
*

      NUM_SV_STRIPS = 511

      SV_PITCH_PHI = 0.000050

      SV_PITCH_Z = 0.000100

      SV_RADIUS(1) = 0.025515
      SV_RADIUS(2) = 0.038007
      SV_RADIUS(3) = 0.070150
      SV_RADIUS(4) = 0.101150


      NUM_HYBRIDS_LAYER(1) = 14
      NUM_HYBRIDS_LAYER(2) = 20
      NUM_HYBRIDS_LAYER(3) = 36
      NUM_HYBRIDS_LAYER(4) = 52

C LAYER 1 HYBRIDS

      SV_HYBRIDS(1,1) = 1
      SV_HYBRIDS(1,2) = 2
      SV_HYBRIDS(1,3) = 3
      SV_HYBRIDS(1,4) = 4
      SV_HYBRIDS(1,5) = 5
      SV_HYBRIDS(1,6) = 6
      SV_HYBRIDS(1,7) = 7
      SV_HYBRIDS(1,8) = 62
      SV_HYBRIDS(1,9) = 63
      SV_HYBRIDS(1,10) = 64
      SV_HYBRIDS(1,11) = 65
      SV_HYBRIDS(1,12) = 66
      SV_HYBRIDS(1,13) = 67
      SV_HYBRIDS(1,14) = 68
      
C LAYER 2 HYBRIDS

      SV_HYBRIDS(2,1) = 8
      SV_HYBRIDS(2,2) = 9
      SV_HYBRIDS(2,3) = 10
      SV_HYBRIDS(2,4) = 11
      SV_HYBRIDS(2,5) = 12
      SV_HYBRIDS(2,6) = 13
      SV_HYBRIDS(2,7) = 14
      SV_HYBRIDS(2,8) = 15
      SV_HYBRIDS(2,9) = 16
      SV_HYBRIDS(2,10) = 17
      SV_HYBRIDS(2,11) = 69
      SV_HYBRIDS(2,12) = 70
      SV_HYBRIDS(2,13) = 71
      SV_HYBRIDS(2,14) = 72
      SV_HYBRIDS(2,15) = 73
      SV_HYBRIDS(2,16) = 74
      SV_HYBRIDS(2,17) = 75
      SV_HYBRIDS(2,18) = 76
      SV_HYBRIDS(2,19) = 77
      SV_HYBRIDS(2,20) = 78

C LAYER 3 HYBRIDS

      SV_HYBRIDS(3,1) = 18
      SV_HYBRIDS(3,2) = 19
      SV_HYBRIDS(3,3) = 20
      SV_HYBRIDS(3,4) = 21
      SV_HYBRIDS(3,5) = 22
      SV_HYBRIDS(3,6) = 23
      SV_HYBRIDS(3,7) = 24
      SV_HYBRIDS(3,8) = 25
      SV_HYBRIDS(3,9) = 26
      SV_HYBRIDS(3,10) = 27
      SV_HYBRIDS(3,11) = 28
      SV_HYBRIDS(3,12) = 29
      SV_HYBRIDS(3,13) = 30
      SV_HYBRIDS(3,14) = 31
      SV_HYBRIDS(3,15) = 32
      SV_HYBRIDS(3,16) = 33
      SV_HYBRIDS(3,17) = 34
      SV_HYBRIDS(3,18) = 35
      SV_HYBRIDS(3,19) = 79
      SV_HYBRIDS(3,20) = 80
      SV_HYBRIDS(3,21) = 81
      SV_HYBRIDS(3,22) = 82
      SV_HYBRIDS(3,23) = 83
      SV_HYBRIDS(3,24) = 84
      SV_HYBRIDS(3,25) = 85
      SV_HYBRIDS(3,26) = 86
      SV_HYBRIDS(3,27) = 87
      SV_HYBRIDS(3,28) = 88
      SV_HYBRIDS(3,29) = 89
      SV_HYBRIDS(3,30) = 90
      SV_HYBRIDS(3,31) = 91
      SV_HYBRIDS(3,32) = 92
      SV_HYBRIDS(3,33) = 93
      SV_HYBRIDS(3,34) = 94
      SV_HYBRIDS(3,35) = 95
      SV_HYBRIDS(3,36) = 96

C LAYER 4 HYBRIDS

      SV_HYBRIDS(4,1) = 36
      SV_HYBRIDS(4,2) = 37
      SV_HYBRIDS(4,3) = 38
      SV_HYBRIDS(4,4) = 39
      SV_HYBRIDS(4,5) = 40
      SV_HYBRIDS(4,6) = 41
      SV_HYBRIDS(4,7) = 42
      SV_HYBRIDS(4,8) = 43
      SV_HYBRIDS(4,9) = 44
      SV_HYBRIDS(4,10) = 45
      SV_HYBRIDS(4,11) = 46
      SV_HYBRIDS(4,12) = 47
      SV_HYBRIDS(4,13) = 48
      SV_HYBRIDS(4,14) = 49
      SV_HYBRIDS(4,15) = 50
      SV_HYBRIDS(4,16) = 51
      SV_HYBRIDS(4,17) = 52
      SV_HYBRIDS(4,18) = 53
      SV_HYBRIDS(4,19) = 54
      SV_HYBRIDS(4,20) = 55
      SV_HYBRIDS(4,21) = 56
      SV_HYBRIDS(4,22) = 57
      SV_HYBRIDS(4,23) = 58
      SV_HYBRIDS(4,24) = 59
      SV_HYBRIDS(4,25) = 60
      SV_HYBRIDS(4,26) = 61
      SV_HYBRIDS(4,27) = 97
      SV_HYBRIDS(4,28) = 98
      SV_HYBRIDS(4,29) = 99
      SV_HYBRIDS(4,30) = 100
      SV_HYBRIDS(4,31) = 101
      SV_HYBRIDS(4,32) = 102
      SV_HYBRIDS(4,33) = 103
      SV_HYBRIDS(4,34) = 104
      SV_HYBRIDS(4,35) = 105
      SV_HYBRIDS(4,36) = 106
      SV_HYBRIDS(4,37) = 107
      SV_HYBRIDS(4,38) = 108
      SV_HYBRIDS(4,39) = 109
      SV_HYBRIDS(4,40) = 110
      SV_HYBRIDS(4,41) = 111
      SV_HYBRIDS(4,42) = 112
      SV_HYBRIDS(4,43) = 113
      SV_HYBRIDS(4,44) = 114
      SV_HYBRIDS(4,45) = 115
      SV_HYBRIDS(4,46) = 116
      SV_HYBRIDS(4,47) = 117
      SV_HYBRIDS(4,48) = 118
      SV_HYBRIDS(4,49) = 119
      SV_HYBRIDS(4,50) = 120
      SV_HYBRIDS(4,51) = 121
      SV_HYBRIDS(4,52) = 122

      RETURN
      END







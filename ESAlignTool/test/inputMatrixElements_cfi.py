import FWCore.ParameterSet.Config as cms

from AlignmentTool.ESAlignTool.esaligntool_cfi import * #DefaultMatrixElement_Iter 
MatrixElementsTmp = DefaultMatrixElement_Iter.clone()

########## Modify the Matrix element for each iterator ################
#  MatrixElementsTmp.Iter1_ESpFdX = cms.double(0.2)
#  MatrixElementsTmp.Iter2_ESpFdX = cms.double(0.1)

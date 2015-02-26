import FWCore.ParameterSet.Config as cms

process = cms.Process("ESAlignmentTool")

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'GR_R_74_V1A::All'  #for RECO data CMSSW_7_4_0_pre6 with condition=auto::run2_data
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'  #https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions


# Default Parameter options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')
options.register('MaxEvents', -1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Run events max"
	)
options.register('OutFilename', 'skimTest.root',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Output File name"
	)
options.register('InputRefitter', False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Input with refit-files"
	)
options.register('InputReducer', False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Input with reduce-files"
	)
options.register('RecHitLabel', 'ecalPreshowerRecHit:EcalRecHitsES',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"ES Reco hits label"
	)
options.register('TrackLabel', 'generalTracks',
	VarParsing.multiplicity.singleton,
	VarParsing.varType.string,
	"Track label"
	)
options.parseArguments()

### Log
#process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger = cms.Service("MessageLogger",
#    destinations = cms.untracked.vstring(
#      'ESAlignmentTool',
#      ),
#    ESAlignmentTool = cms.untracked.PSet(
      #threshold = cms.untracked.string('INFO'),  
#      threshold = cms.untracked.string('DEBUG'),  
#      ), 
    #suppressInfo = cms.untracked.vstring('ESAlignmentTool'),
#    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.MaxEvents) )

### input
from inputFiles_cfi import * #FileNames 
process.source = cms.Source("PoolSource",
    #skipEvents = cms.untracked.uint32(0),
    #firstEvent = cms.untracked.uint32(1),
    #fileNames = cms.untracked.vstring(FileNames_CSA14Test)
    #fileNames = cms.untracked.vstring(FileNames_PionGunTest)
    #fileNames = cms.untracked.vstring(FileNames_Skim721)
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_master/src/SkimTool/ESHitSkimLoose/test/ESRedSkim.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_master/src/SkimTool/ESHitSkimLoose/test/Refitter.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_4_0_pre6_dev/src/SkimTool/ESHitSkimLoose/test/ESRedSkim.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/generateSamples/CMSSW_7_4_0_pre6/src/DoubleElectron-Run2012D/reco_RAW2DIGI_RECO_100.root")
    fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/generateSamples/CMSSW_7_4_0_pre6/src/DoubleElectron-Run2012D/reco_RAW2DIGI_RECO_400.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_0_7_master/src/SkimTool/ESHitSkimLoose/test/ESRedSkimtestDone.root")
    #fileNames = cms.untracked.vstring(FileNames)
)

### output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.OutFilename) 
)

### Check parameters options
if options.InputRefitter == True and options.TrackLabel == 'generalTracks':
	print 'WARNING: Using Refit files, options.TrackLabel = TrackRefitter'
	options.TrackLabel = 'TrackRefitter'
print 'Load lables:'
print ' options.RecHitLabel = '+options.RecHitLabel
print ' options.TrackLabel = '+options.TrackLabel

### Input parameters
process.ESAlignmentTool = cms.EDAnalyzer('SkimTest',
	RecHitLabel = cms.InputTag(options.RecHitLabel),
	TrackLabel = cms.InputTag(options.TrackLabel),
	InputRefitter = cms.bool(options.InputRefitter),
)
process.p = cms.Path(process.ESAlignmentTool)

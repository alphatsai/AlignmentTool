import FWCore.ParameterSet.Config as cms

process = cms.Process("ESAlignmentTool")

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

#process.GlobalTag.globaltag = 'GR_R_74_V1A::All'  #for RECO data CMSSW_7_4_0_pre6 with condition=auto::run2_data
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'  #https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
#process.GlobalTag.globaltag = 'MCRUN2_74_V1::All'  #https://twiki.cern.ch/twiki/bin/viewauth/CMS/RelValGT
process.GlobalTag.globaltag = 'GR_P_V56::All'  #https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions


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
    #fileNames = cms.untracked.vstring(FileNames_PionGunTest)
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/j/jtsai/ESAlignment/CMSSW_7_4_0_pre6_dev/src/SkimTool/ESHitSkimLoose/test/ESSkim.root")
    fileNames = cms.untracked.vstring("root://eoscms//eos/cms/store/data/Run2015A/Jet/RECO/PromptReco-v1/000/247/992/00000/6C94F6E2-2415-E511-BDD0-02163E011938.root")
)

### output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.OutFilename) 
)

### Check parameters options
if   options.TrackLabel != 'generalTracks' and options.TrackLabel != 'TrackRefitter' and options.TrackLabel != 'esGeneralTracks' and options.InputRefitter == False and options.InputRefitter == False:
	print '[NOTE] Using additional track label, options.TrackLabel = '+options.TrackLabel
elif options.InputRefitter == True and options.InputReducer == True:
	print '[ERROR]: Should not make InputRefitter and InputReducer are True!'
elif options.InputRefitter == True and options.TrackLabel == 'esGeneralTracks':
	print '[ERROR]: Should not make InputRefitter is True and TrackLabel=esGeneralTracks '
elif options.InputReducer == True and options.TrackLabel == 'TrackRefitter':
	print '[ERROR]: Should not make InputReducer is True and TrackLabel=TrackRefitter '
elif options.TrackLabel == 'TrackRefitter' or options.InputRefitter == True and options.InputReducer == False:
	print '[NOTE]: Using Refitted files, options.TrackLabel = TrackRefitter'
	options.TrackLabel = 'TrackRefitter'
elif options.TrackLabel == 'esGeneralTracks' or options.InputRefitter == False and options.InputReducer == True:
	print '[NOTE]: Using Reduced files, options.TrackLabel = esGeneralTracks'
	options.TrackLabel = 'esGeneralTracks'  # it was ESTracksReducer

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

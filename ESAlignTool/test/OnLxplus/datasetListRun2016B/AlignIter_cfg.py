import FWCore.ParameterSet.Config as cms

process = cms.Process("ESAlignmentTool")

#process.load('Configuration.StandardSequences.GeometryDB_cff')
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")

###################### Modify following Global tag ################################
######################       This is example       ################################
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v8_CustomTrackerAndECAL_2016B_v0', '')

#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v2', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v4', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Candidate_2015_11_18_10_21_42', '')

# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
#process.GlobalTag.globaltag = 'POSTLS170_V6::All'  
#process.GlobalTag.globaltag = 'POSTLS170_V5::All'  
#process.GlobalTag.globaltag = 'MCRUN2_74_V1::All' 
#process.GlobalTag.globaltag = 'GR_P_V56::All'  
#process.newESAlignment = cms.ESSource("PoolDBESSource",
#           process.CondDBSetup,
#                                       connect = cms.string('sqlite_file:inputDB.db'),
#                                       toGet= cms.VPSet(cms.PSet(record = cms.string("ESAlignmentRcd"),
#                                                       tag = cms.string('ES'))
#                                                           )
#)
#process.es_prefer_GlobalPositionDB = cms.ESPrefer("PoolDBESSource", "newESAlignment")
#process.newTKAlignment = cms.ESSource("PoolDBESSource",
#                                        process.CondDBSetup,
#                                        connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT'),
#                                        timetype = cms.string('runnumber'),
#                                        toGet = cms.VPSet(cms.PSet(
#                                                record = cms.string('TrackerAlignmentRcd'),
#                                                tag = cms.string('TrackerAlignment_GR10_v4_offline')
#                                                ))
#                                        )
#process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "newTKAlignment")

####################################################################################


# Default Parameter options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')
options.register('IterN', 1,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Alignment iteration number"
    )
options.register('MaxEvents', -1,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Run events max"
    )
options.register('OutFilename', 'AlignmentFile.root',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Output File name"
    )
options.register('JSONFilename', '',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Input JSON"
    )
options.register('Debug', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Debug"
    )
options.register('InputRefitter', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Input with refit-files"
    )
options.register('DrawMagField', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Draw Magnic Field"
    )
options.register('PrintPosition', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Print Position"
    )
options.register('PrintMatrix', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Print Matrix"
    )
options.register('CalculateESorigin', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Calulate ES origin from Geometry"
    )
options.register('CalculateESaxes', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Calulate ES Axes from Geometry"
    )
options.register('StoreDetail', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Store detail branches"
    )
options.register('OverwriteRotationM', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Overwrite Rotation Matrix from Geomatry"
    )
options.register('ReSetRfromOutside', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "ReSet Radio from Outside"
    )
options.register('RecHitLabel', 'ecalPreshowerRecHit:EcalRecHitsES',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "ES Reco hits label"
    )
options.register('TrackLabel', 'TrackRefitter',
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
#      threshold = cms.untracked.string('INFO'),  
#      ), 
#    #suppressInfo = cms.untracked.vstring('ESAlignmentTool'),
#    )
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.MaxEvents) )

### input
from inputFiles_cfi import * #FileNames 
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(FileNames)
    #fileNames = cms.untracked.vstring(FileNames_CSA14Test)
    #fileNames = cms.untracked.vstring(FileNames_PionGunTest)
    #fileNames = cms.untracked.vstring("root://eoscms//eos/cms/store/data/Run2015A/Jet/RECO/PromptReco-v1/000/247/992/00000/6C94F6E2-2415-E511-BDD0-02163E011938.root")
    #skipEvents = cms.untracked.uint32(0),
    #firstEvent = cms.untracked.uint32(1),
)

### output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.OutFilename) 
)

### Check parameters options
if options.InputRefitter == False and options.TrackLabel == 'TrackRefitter':
    print 'WARNING: Not using Refit files, options.TrackLabel = generalTracks'
    options.TrackLabel = 'generalTracks' 
print 'Load lables:'
print ' options.RecHitLabel = '+options.RecHitLabel
print ' options.TrackLabel = '+options.TrackLabel

if options.IterN == 1 and options.OverwriteRotationM == False:
    print 'WARNING: First iter, options.OverwriteRotationM = True' 
    options.OverwriteRotationM = True

### Input parameters
from AlignmentTool.ESAlignTool.DefaultESLocation_cfi import *  
from inputMatrixElements_cfi import * #MatrixElementsTmp # Modify inputMatrixElements_cfi.py for Matrix Elements 
process.ESAlignmentTool = cms.EDAnalyzer('ESAlignTool',
    RecHitLabel        = cms.InputTag(options.RecHitLabel),
    TrackLabel         = cms.InputTag(options.TrackLabel),
    IterN              = cms.uint32(options.IterN),
    Debug              = cms.bool(options.Debug),
    InputRefitter      = cms.bool(options.InputRefitter),
    DrawMagField       = cms.bool(options.DrawMagField),
    PrintPosition      = cms.bool(options.PrintPosition),
    PrintMatrix        = cms.bool(options.PrintMatrix),
    CalculateESorigin  = cms.bool(options.CalculateESorigin),
    CalculateESaxes    = cms.bool(options.CalculateESaxes),
    OverwriteRotationM = cms.bool(options.OverwriteRotationM),
    StoreDetail        = cms.bool(options.StoreDetail),
    ReSetRfromOutside  = cms.bool(options.ReSetRfromOutside),
    e_xxlimit          = cms.double(1.),
    e_yylimit          = cms.double(1.),
    e_yxlimit          = cms.double(1.),
    winlimit           = cms.double(3.),
    Selected_idee      = cms.uint32(0),
    Selected_RUNmin    = cms.int32(0),
    Selected_RUNmax    = cms.int32(0),
    DefaultESLocation  = DefaultESLocation.clone(),
    MatrixElements     = MatrixElementsTmp.clone(),
)

### Input JSON
if options.JSONFilename != "":
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename=options.JSONFilename).getVLuminosityBlockRange()
#process.source.lumisToProcess = LumiList.LumiList(filename='Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON.txt').getVLuminosityBlockRange()
#process.source.lumisToProcess = LumiList.LumiList(filename='Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON_v2.txt').getVLuminosityBlockRange()

process.load("RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi") # For ES ALCA RECO dataset
#from RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi import *
#process.esLocalReco = cms.Sequence( ecalPreshowerRecHit )

process.p = cms.Path( process.ecalPreshowerRecHit*process.ESAlignmentTool)
#process.p = cms.Path( process.esLocalReco *process.ESAlignmentTool)

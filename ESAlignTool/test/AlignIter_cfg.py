import FWCore.ParameterSet.Config as cms

process = cms.Process("ESAlignmentTool")

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

###################### Modify following Global tag ################################
######################       This is example       ################################
#	https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
#process.GlobalTag.globaltag = 'GR_R_311_V1::All'
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
#
#process.newGlobalPosition = cms.ESSource("PoolDBESSource",
#                                          process.CondDBSetup,
#                                          connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT'),
#                                          toGet= cms.VPSet(cms.PSet(record = cms.string("GlobalPositionRcd"),
#                                                                     tag = cms.string('GlobalAlignment_TkRotMuonFromLastIovV2_offline'))
#                                                           )
#                                         )
#process.es_prefer_GlobalPositionDB = cms.ESPrefer("PoolDBESSource", "newGlobalPosition")
#process.GlobalTag.globaltag = 'POSTLS170_V6::All'  
process.GlobalTag.globaltag = 'POSTLS170_V5::All'  #https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
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
options.register('InputRefitter', True,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Input with refit-files"
	)
options.register('doRotation', True,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Consider  Rotation in alignment"
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
options.register('CalculateESorigin', False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Calulate ES origin from Geometry"
	)
options.register('CalculateESaxes', False,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Calulate ES Axes from Geometry"
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
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring(
      'ESAlignmentTool',
      ),
    ESAlignmentTool = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),  
      ), 
    #suppressInfo = cms.untracked.vstring('ESAlignmentTool'),
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.MaxEvents) )

### input
from inputFiles_cfi import * #FileNames 
process.source = cms.Source("PoolSource",
    #skipEvents = cms.untracked.uint32(0),
    #firstEvent = cms.untracked.uint32(1),
	#fileNames = cms.untracked.vstring(FileNames)
	#fileNames = cms.untracked.vstring(FileNames_CSA14Test)
	fileNames = cms.untracked.vstring(FileNames_PionGunTest)
)

### Check parameters options
if options.InputRefitter == False and options.TrackLabel == 'TrackRefitter':
	print 'WARNING: Not using Refit files, options.TrackLabel = generalTracks'
	options.TrackLabel = 'generalTracks'	
print 'Load lables:'
print ' options.RecHitLabel = '+options.RecHitLabel
print ' options.TrackLabel = '+options.TrackLabel

if options.IterN == 1 and options.CalculateESorigin == False:
	print 'WARNING: First iter, options.CalculateESorigin = True' 
	options.CalculateESorigin = True 
if options.IterN == 1 and options.CalculateESaxes == False:
	print 'WARNING: First iter, options.CalculateESaxes = True' 
	options.CalculateESaxes = True

### Input parameters
from inputMatrixElements_cfi import * #MatrixElementsTmp # Modify inputMatrixElements_cfi.py for Matrix Elements 
process.ESAlignmentTool = cms.EDAnalyzer('ESAlignTool',
	RecHitLabel = cms.InputTag(options.RecHitLabel),
	TrackLabel = cms.InputTag(options.TrackLabel),
	IterN = cms.uint32(options.IterN),
	InputRefitter = cms.bool(options.InputRefitter),
	doRotation = cms.bool(options.doRotation),
	DrawMagField = cms.bool(options.DrawMagField),
	PrintPosition = cms.bool(options.PrintPosition),
    	PrintMatrix = cms.bool(options.PrintMatrix),
	CalculateESorigin = cms.bool(options.CalculateESorigin),
	CalculateESaxes = cms.bool(options.CalculateESaxes),
    	OverwriteRotationM = cms.bool(options.OverwriteRotationM),
    	ReSetRfromOutside = cms.bool(options.ReSetRfromOutside),
	e_xxlimit = cms.double(1.),
	e_yylimit = cms.double(1.),
	e_yxlimit = cms.double(1.),
	winlimit = cms.double(3.),
    	Selected_idee = cms.uint32(0),
    	Selected_RUNmin = cms.int32(0),
    	Selected_RUNmax = cms.int32(0),
	MatrixElements = MatrixElementsTmp.clone(),
)

process.p = cms.Path(process.ESAlignmentTool)

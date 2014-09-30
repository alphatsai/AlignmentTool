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
process.GlobalTag.globaltag = 'POSTLS170_V7::All'  #https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
####################################################################################


# Default Parameter options
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')
options.register('IterNum', 1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Alignment iteration number"
	)
options.register('MaxEvents', -1,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.int,
	"Run events max"
	)
options.register('InputRefit', True,
	VarParsing.multiplicity.singleton,
	VarParsing.varType.bool,
	"Input with refit-files"
	)
options.register('ConsiderRotation', True,
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
    #suppressInfo = cms.untracked.vstring('ABCD'),
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.MaxEvents) )

### input
from inputFiles_cfi import * #FileNames 
process.source = cms.Source("PoolSource",
    #skipEvents = cms.untracked.uint32(0),
    #firstEvent = cms.untracked.uint32(1),
	fileNames = cms.untracked.vstring(FileNames)
)

### Check parameters options
if options.InputRefit == False and options.TrackLabel == 'TrackRefitter':
	print 'WARNING: Not using Refit files, options.TrackLabel = generalTracks'
	options.TrackLabel = 'generalTracks'	
print 'Load lables:'
print ' options.RecHitLabel = '+options.RecHitLabel
print ' options.TrackLabel = '+options.TrackLabel

if options.IterNum != 1 and options.CalculateESorigin != False:
	print 'WARING: Not first iter, options.CalculateESorigin = False' 
	options.CalculateESorigin = False
if options.IterNum != 1 and options.CalculateESaxes != False:
	print 'WARING: Not first iter, options.CalculateESaxes = False' 
	options.CalculateESaxes = False

### Input parameters
from AlignmentTool.ESAlignTool.esaligntool_cfi import * #DefaultMatrixElement_Iter 
process.ESAlignmentTool = cms.EDAnalyzer('ESAlignTool',
	RecHitLabel = cms.InputTag(options.RecHitLabel),
	TrackLabel = cms.InputTag(options.TrackLabel),
	IterN = cms.uint32(options.IterNum),
	fromRefitter = cms.bool(options.InputRefit),
	withRotation = cms.bool(options.ConsiderRotation),
	DrawMagField = cms.bool(options.DrawMagField),
	PrintPosition = cms.bool(options.PrintPosition),
    	PrintMatrix = cms.bool(options.PrintMatrix),
	Cal_ESorigin_from_Geometry = cms.bool(options.CalculateESorigin),
	Cal_ESaxes_from_Geometry = cms.bool(options.CalculateESaxes),
    	Overwrite_RotationMatrix_fromGeometry = cms.bool(options.OverwriteRotationM),
    	ReSetRfromOutside = cms.bool(options.ReSetRfromOutside),
	e_xxlimit = cms.double(1.),
	e_yylimit = cms.double(1.),
	e_yxlimit = cms.double(1.),
	winlimit = cms.double(3.),
    	Selected_idee = cms.uint32(0),
    	Selected_RUNmin = cms.int32(0),
    	Selected_RUNmax = cms.int32(0),
	MatrixElements = DefaultMatrixElement_Iter.clone(),
)

process.p = cms.Path(process.ESAlignmentTool)

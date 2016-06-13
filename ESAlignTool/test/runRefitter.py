import FWCore.ParameterSet.Config as cms

process = cms.Process("REFIT")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

###################### Modify following Global tag ################################
######################       This is example       ################################
#	https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag( process.GlobalTag, '80X_dataRun2_Prompt_v8_CustomTrackerAndECAL_2016B_v0', '')
#### Add and replace additional tags case
#process.GlobalTag = GlobalTag( process.GlobalTag, '80X_dataRun2_HLTValidation_EB_EE_ESAlignment_week20_2016', '')
#process.GlobalTag.toGet = cms.VPSet(
#        ###### starts customization of tracker part
#         cms.PSet(record = cms.string("TrackerAlignmentRcd"),
#                  tag =  cms.string("TrackerAlignment_MP_Run2016B_v2"),
#                  connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#                  ),
#         cms.PSet(record = cms.string("TrackerAlignmentErrorExtendedRcd"),
#                  tag =  cms.string("TrackerAlignmentExtendedErrors_MP_Run2016B"),
#                  connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#                  ),
#         cms.PSet(record = cms.string("SiPixelTemplateDBObjectRcd"),
#                  tag =  cms.string("SiPixelTemplateDBObject_38T_2016_v1_hltvalidation"),
#                  connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
#                  )
#)
#### Add or change spacial parameters from DB
#process.TrackerAlignment2009 = cms.ESSource("PoolDBESSource",
#                                          process.CondDBSetup,
#                                          connect = cms.string('frontier://PromptProd/CMS_COND_31X_ALIGNMENT'),
#                                          toGet= cms.VPSet(cms.PSet(record = cms.string("TrackerAlignmentRcd"),
#                                                                     tag = cms.string('TrackerAlignment_2009_v1_prompt'))
#                                                           )
#)
#process.es_prefer_TrackerAlignment2009 = cms.ESPrefer("PoolDBESSource", "TrackerAlignment2009")

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.NavigationSchool = ""

#process.TrackRefitter.src = "esGeneralTracks" 
process.TrackRefitter.src = "ecalAlCaESAlignTrackReducer" # Default is generalTracks, changing depend on new collection from producer

################### Input file #############################
from inputFiles_cfi import * 
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(FileNames)
    #fileNames = cms.untracked.vstring(FileNames_PionGunTest)
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

################### Define process #########################
process.out = cms.OutputModule("PoolOutputModule",
     outputCommands = cms.untracked.vstring('keep *'),
     fileName = cms.untracked.string('Refitter.root')
)

process.p = cms.EndPath(process.out)
process.p1 = cms.Path(process.TrackRefitter)
process.schedule = cms.Schedule( process.p1 , process.p)


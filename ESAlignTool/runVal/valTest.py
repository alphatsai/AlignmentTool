import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'FT_R_44_V9::All'


process.newTKAlignment = cms.ESSource("PoolDBESSource",
                                        process.CondDBSetup,
                                        connect = cms.string('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT'),
                                        timetype = cms.string('runnumber'),
                                        toGet = cms.VPSet(cms.PSet(
                                                record = cms.string('TrackerAlignmentRcd'),
                                                tag = cms.string('TrackerAlignment_v10_offline')
                                                ))
                                        )
process.es_prefer_trackerAlignment = cms.ESPrefer("PoolDBESSource", "newTKAlignment")

process.newDeformation = cms.ESSource("PoolDBESSource",
                                          process.CondDBSetup,
                                          connect = cms.string('frontier://FrontierProd/CMS_COND_310X_ALIGN'),
                                          toGet= cms.VPSet(cms.PSet(record = cms.string("TrackerSurfaceDeformationRcd"),
                                                                     tag = cms.string('TrackerSurfaceDeformations_v4_offline'))
                                                           )
                                         )
process.es_prefer_DeformationDB = cms.ESPrefer("PoolDBESSource", "newDeformation")
process.SiStripLorentzAngle = cms.ESSource("PoolDBESSource",
                                          process.CondDBSetup,
                                          connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1025/SiStripLorentzAngleDeco_GR10_v1_offline_BPCorrected.db'),
                                          toGet= cms.VPSet(cms.PSet(record = cms.string("SiStripLorentzAngleRcd"),
                                                                     tag = cms.string('SiStripLorentzAngleDeco_GR10_v1_offline_BPCorrected'),
								   label = cms.untracked.string('deconvolution'))
                                                           )
                                         )
process.es_prefer_SiStripLorentzAngle = cms.ESPrefer("PoolDBESSource", "SiStripLorentzAngle")
process.SiStripShiftAndCrosstalk = cms.ESSource("PoolDBESSource",
                                          process.CondDBSetup,
                                          connect = cms.string('sqlite_file:/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1025/SiStripShiftAndCrosstalk_GR10_v2_offline_BPCorrected.db'),
                                          toGet= cms.VPSet(cms.PSet(record = cms.string("SiStripConfObjectRcd"),
                                                                     tag = cms.string('SiStripShiftAndCrosstalk_GR10_v2_offline_BPCorrected'))
                                                           )
                                         )
process.es_prefer_SiStripShiftAndCrosstalk = cms.ESPrefer("PoolDBESSource", "SiStripShiftAndCrosstalk")




process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    firstEvent = cms.untracked.uint32(1),
	fileNames = cms.untracked.vstring(


'root://eoscms//eos/cms/store/caf/user/jtsai/test0801RAWRECO/refit2/file_ESRefit_P99.root?svcClass=default'


	)

)

process.load("AlignmentTool.ESAlignTool.esaligntool_cfi")
#process.esAlignTool.DrawMagField = cms.bool(True)
#process.esAlignTool.PrintPosition = cms.bool(True)
process.esAlignTool.Overwrite_RotationMatrix_fromGeometry = cms.bool(True)
process.esAlignTool.fromRefitter = cms.bool(True)

process.esAlignTool.Cal_ESorigin_from_Geometry = cms.bool(True)
process.esAlignTool.withRotation = cms.bool(True)
process.esAlignTool.IterN = cms.uint32(1)
process.esAlignTool.e_xxlimit = cms.double(1.)
process.esAlignTool.e_yylimit = cms.double(1.)
process.esAlignTool.e_yxlimit = cms.double(1.)
process.esAlignTool.winlimit = cms.double(3.)



process.p = cms.Path(process.esAlignTool)

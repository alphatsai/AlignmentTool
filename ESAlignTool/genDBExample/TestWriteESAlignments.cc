#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"
#include "Geometry/EcalAlgo/interface/WriteESAlignments.h"

typedef WriteESAlignments WEA ;

class TestWriteESAlignments : public edm::EDAnalyzer
{
public:

  explicit TestWriteESAlignments(const edm::ParameterSet& /*iConfig*/)
    : nEventCalls_(0) {}
  ~TestWriteESAlignments() {}
  
  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

private:
  
  unsigned int nEventCalls_;
};
  
void TestWriteESAlignments::analyze(const edm::Event& /*evt*/, const edm::EventSetup& evtSetup)
{
   if (nEventCalls_ > 0) {
     edm::LogInfo("TestWriteESAlignments") << "Writing to DB to be done only once, "
	       << "set 'untracked PSet maxEvents = {untracked int32 input = 1}'."
	       << "(Your writing should be fine.)";
     return;
   }


   //static const unsigned int nA ( EcalPreshowerGeometry::numberOfAlignments() ) ;

      // ordering of i is: left, right, left, right,...                         
      // starting at ES- rear, then ES- front,                                  
      // then ES+ front, then ES+ rear          

   // Run2015A 0 Tesla dataset: Front z: 303.846, Rear z: 308.306 
   typedef std::vector<double> DVec ;
   double Ar_Alpha[] = {0.0014, 0.0014, 0.0012, 0.0012, -0.0012, -0.0012, -0.0012, -0.0012};
   double Ar_Beta[] = {0.0002, 0.0002, 0.0006, 0.0006, 0.0000, 0.0000, 0.0004, 0.0004};
   double Ar_Gamma[] = {0.0001, 0.0001, 0.0006, 0.0006, 0.0008, 0.0008, 0.0006, 0.0006};
   double Ar_X[] = {-0.225, -0.225, -0.242, -0.242, 0.331, 0.331, 0.351, 0.351};
   double Ar_Y[] = {-0.503, -0.503, -0.534, -0.534, -0.927, -0.927, -0.898, -0.898};
   double Ar_Z[] = {-2.438, -2.438, -2.281, -2.281,  2.134,  2.134,  2.25, 2.25};

   DVec alphaVec   ( Ar_Alpha, Ar_Alpha+8 ) ;
   DVec betaVec    ( Ar_Beta, Ar_Beta+8 ) ;
   DVec gammaVec   ( Ar_Gamma, Ar_Gamma+8 ) ;
   DVec xtranslVec(Ar_X,Ar_X+8);
   DVec ytranslVec(Ar_Y,Ar_Y+8);
   DVec ztranslVec(Ar_Z,Ar_Z+8);

   const WriteESAlignments wea( evtSetup   ,
				alphaVec   ,
				betaVec    ,
				gammaVec   ,
				xtranslVec ,
				ytranslVec ,
				ztranslVec  ) ;
   
   edm::LogInfo("TestWriteESAlignments") << "Done!";
   nEventCalls_++;
}

DEFINE_FWK_MODULE(TestWriteESAlignments);

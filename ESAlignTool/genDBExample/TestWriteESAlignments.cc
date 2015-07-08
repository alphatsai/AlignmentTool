#include <string>
#include <map>
#include <vector>

// Framework
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "Utilities/General/interface/ClassName.h"
#include "DataFormats/DetId/interface/DetId.h"

// Database
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"

#include "Geometry/EcalAlgo/interface/WriteESAlignments.h"
typedef WriteESAlignments WEA ;

class TestWriteESAlignments : public edm::EDAnalyzer
{
public:

  explicit TestWriteESAlignments(const edm::ParameterSet& iConfig)
    :nEventCalls_(0) {}
  ~TestWriteESAlignments() {}
  
//  template<typename T>
//  void writeAlignments(const edm::EventSetup& evtSetup);

  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

private:
  
  unsigned int nEventCalls_;
};
  
void TestWriteESAlignments::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup)
{
   if (nEventCalls_ > 0) {
     std::cout << "Writing to DB to be done only once, "
	       << "set 'untracked PSet maxEvents = {untracked int32 input = 1}'."
	       << "(Your writing should be fine.)" << std::endl;
     return;
   }


   static const unsigned int nA ( EcalPreshowerGeometry::numberOfAlignments() ) ;

      // ordering of i is: left, right, left, right,...                         
      // starting at ES- rear, then ES- front,                                  
      // then ES+ front, then ES+ rear           
   typedef std::vector<double> DVec ;
   //double Ar_Alpha[] = {0.0019, 0.0019, 0.0016, 0.0016, -0.0012, -0.0012, -0.0015, -0.0015};
   //double Ar_Beta[] = {0.0006, 0.0006, 0.0009, 0.0009, 0.0000, 0.0000, -0.0000, -0.0000};
   //double Ar_Gamma[] = {0.00001, 0.00001, 0.00055, 0.00055, 0.00092, 0.00092, 0.00065, 0.00065};
   //double Ar_X[] = {-0.04, -0.04, -0.03, -0.03, 0.52, 0.52, 0.53, 0.53};
   //double Ar_Y[] = {-0.77, -0.77, -0.77, -0.77, -0.98, -0.98, -0.96, -0.96};
   //double Ar_Z[] = {-0.94, -0.94, -0.74, -0.74, 0.61, 0.61, 0.81, 0.81};

   //for test
   //double Ar_Alpha[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Beta[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Gamma[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_X[] = {0.,0.,0.,0.,1.0,1.0,1.0,1.0};
   //double Ar_Y[] = {+1.0,+1.0,+1.0,+1.0,-1.5,-1.5,-1.5,-1.5};
   //double Ar_Z[] = {0.,0.,0.,0.,0.,0.,0.,0.};

   //for ideal
   //double Ar_Alpha[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Beta[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Gamma[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_X[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Y[] = {0.,0.,0.,0.,0.,0.,0.,0.};
   //double Ar_Z[] = {0.,0.,0.,0.,0.,0.,0.,0.};

   //for 44X
   //double Ar_Alpha[] = {0.0019, 0.0019, 0.0022, 0.0022, -0.0010, -0.0010, -0.0016, -0.0016};
   //double Ar_Beta[] = {-0.0002, -0.0002, 0.0005, 0.0005, 0.0003, 0.0003, -0.0005, -0.0005};
   //double Ar_Gamma[] = {-0.00013, -0.00013, 0.00049, 0.00049, 0.00076, 0.00076, 0.00052, 0.00052};
   //double Ar_X[] = {-0.06, -0.06, -0.05, -0.05, 0.52, 0.52, 0.54, 0.54};
   //double Ar_Y[] = {-0.75, -0.75, -0.78, -0.78, -0.97, -0.97, -0.96, -0.96};
   //double Ar_Z[] = {-0.98, -0.98, -0.80, -0.80, 0.62, 0.62, 0.81, 0.81};

   //for Run2012AB
   double Ar_Alpha[] = { 0.0018 , 0.0018,  0.0021 , 0.0021 ,-0.0010 ,-0.0010 ,-0.0015 ,-0.0015};
   double Ar_Beta[]  = {-0.0001 ,-0.0001 , 0.0001 , 0.0001 , 0.0010 , 0.0010 , 0.0008 , 0.0008};
   double Ar_Gamma[] = {-0.00007,-0.00007, 0.00045, 0.00045, 0.00061, 0.00061, 0.00062, 0.00062};
   double Ar_X[] = {-0.09, -0.09, -0.04, -0.04, 0.51, 0.51, 0.51, 0.51};
   double Ar_Y[] = {-0.75, -0.75, -0.78, -0.78,-0.98,-0.98,-0.95,-0.95};
   double Ar_Z[] = {-0.98, -0.98, -0.78, -0.78, 0.51, 0.51, 0.90, 0.90};

   //for Run2012AB more digis
   //double Ar_Alpha[] = { 0.00179 , 0.00179 ,  0.00208 , 0.00208 ,-0.00105 ,-0.00105 ,-0.00149 ,-0.00149 };
   //double Ar_Beta[]  = {-0.00009 ,-0.00009 ,  0.00013 , 0.00013 , 0.00104 , 0.00104 , 0.00082 , 0.00082 };
   //double Ar_Gamma[] = { 0.000187, 0.000187,  0.000452, 0.000452, 0.000609, 0.000609, 0.000615, 0.000615};
   //double Ar_X[] = {-0.092, -0.092, -0.045, -0.045, 0.513, 0.513, 0.508, 0.508};
   //double Ar_Y[] = {-0.754, -0.754, -0.777, -0.777,-0.980,-0.980,-0.953,-0.953};
   //double Ar_Z[] = {-0.984, -0.984, -0.783, -0.783, 0.512, 0.512, 0.896, 0.896};

   //for Run2012AB full digits
   //double Ar_Alpha[] = {0.00178923, 0.00178923, 0.00208174, 0.00208174, -0.00104776, -0.00104776, -0.00148966, -0.00148966};
   //double Ar_Beta[]  = {-8.59965e-05, -8.59965e-05, 0.000127171, 0.000127171, 0.00104075, 0.00104075, 0.000824722, 0.000824722};
   //double Ar_Gamma[] = { 0.000187302, 0.000187302, 0.000452951, 0.000452951, 0.00060889, 0.00060889, 0.000615388, 0.000615388};
   //double Ar_X[] = {-0.0924595, -0.0924595, -0.0445455, -0.0445455, 0.512707, 0.512707, 0.508078, 0.508078};
   //double Ar_Y[] = {-0.754394, -0.754394, -0.776534, -0.776534, -0.979818, -0.979818, -0.952701, -0.952701};
   //double Ar_Z[] = {-0.98409, -0.98409, -0.783444, -0.783444, 0.512402, 0.512402, 0.896227, 0.896227};


   DVec alphaVec(Ar_Alpha,Ar_Alpha+8);
   DVec betaVec(Ar_Beta,Ar_Beta+8);
   DVec gammaVec(Ar_Gamma,Ar_Gamma+8);
   DVec xtranslVec(Ar_X,Ar_X+8);
   DVec ytranslVec(Ar_Y,Ar_Y+8);
   DVec ztranslVec(Ar_Z,Ar_Z+8);

   const WEA::WriteESAlignments wea( evtSetup   ,
				     alphaVec   ,
				     betaVec    ,
				     gammaVec   ,
				     xtranslVec ,
				     ytranslVec ,
				     ztranslVec  ) ;

   std::cout << "done!" << std::endl;
   nEventCalls_++;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestWriteESAlignments);

// analysis code
#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "EventLoopGrid/PrunDriver.h"
#include <EventLoopAlgs/NTupleSvc.h>
#include <EventLoopAlgs/AlgSelect.h>
#include <EventLoop/OutputStream.h>
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "SUSYUpgradeExample/UpgradeAnalysis.h"

// standard library stuff
#include <iostream>

using namespace std;


//===========================
int main( int argc, char* argv[] ) {
  //===========================


  // Take the submit directory from the input if provided:
  string mysample = "bob";
  if( argc > 1 ) mysample = argv[ 1 ];
  string submitDir = "/usera/chpotter/Analysis_Upgrade/SUSYUpgradeExample/outdir_"+mysample;

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
  //const char* inputFilePath = gSystem->ExpandPathName ("/r03/atlas/chpotter/Part3_Project_2018");
  //SH::ScanDir().samplePattern(sample+"*").filePattern("*root*").scan(sh,inputFilePath);
  SH::scanRucio (sh, mysample);


  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setDouble (EL::Job::optMaxEvents, -1);

  // add ntuple output to job
  EL::OutputStream output ("output");
  job.outputAdd (output);
  EL::NTupleSvc *ntuple = new EL::NTupleSvc ("output");
  // configure ntuple object
  job.algsAdd (ntuple);
    
  // Add our analysis to the job:
  UpgradeAnalysis* alg = new UpgradeAnalysis();
  //alg->SampleName = mysample; 
  job.algsAdd( alg );

  // Run the job using the local/direct driver:
  //EL::DirectDriver driver;
  EL::PrunDriver driver;
  driver.options()->setString("nc_outputSampleName", "user.chpotter.%in:name[2]%.%in:name[6]%.19Mar2018");
  driver.options()->setString (EL::Job::optRootVer, "6.04.16");
  driver.options()->setString (EL::Job::optCmtConfig, "x86_64-slc6-gcc49-opt");
  //driver.options()->setString (EL::Job::optSubmitFlags, "--useRootCore --nFilesPerJob=1 --mergeOutput"); // 
  //driver.options()->setString (EL::Job::optSubmitFlags, "--useRootCore --noBuild --noCompile");

  /*
  ifstream my_file("Tarball.tar");
  if(!my_file){ // tarball doesn't exist, make it 
    driver.options()->setString (EL::Job::optSubmitFlags, "--outTarBall=Tarball.tar --useRootCore --noBuild --noCompile");
    //driver.options()->setString (EL::Job::optGridNoSubmit, "True");
    cout << "--- Making Tarball.tar, please rerun command afterwards ---" << endl;
  }
  else{
    driver.options()->setString (EL::Job::optSubmitFlags, "--inTarBall=Tarball.tar --useRootCore --noBuild --noCompile");
  }
  */
  /*
if no tarball 
prun OPTIONS --outTarBall=tarball.tar --noSubmit
if tarball
prun OPTIONS --inTarBall=tarball.tar 


EL::Job::optRootVer, 
EL::Job::optCmtConfig
EL::Job::optGridNoSubmit, 
driver.options()->setString (EL::Job::optSubmitFlags, "-x -y -z");
   */
  
  //driver.submit( job, submitDir );
  driver.submitOnly( job, submitDir );

  return 0;
}

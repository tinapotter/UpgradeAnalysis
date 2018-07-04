// analysis code
#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
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
  if( argc > 0 ) mysample = argv[2];
  string submitDir = "/usera/chpotter/Analysis_Upgrade/SUSYUpgradeExample/outdir_"+mysample;

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
  const char* inputFilePath = gSystem->ExpandPathName ("/r03/atlas/chpotter/Part3_Project_2018/Private_C1C1WW/");
  //SH::ScanDir().samplePattern("user.chpotter."+mysample+".DAOD_TRUTH1.30March2018_EXT0").filePattern("*root*").scan(sh,inputFilePath);
  SH::ScanDir().samplePattern("user.chpotter."+mysample+".DAOD_TRUTH1.11May2018_EXT0").filePattern("*root*").scan(sh,inputFilePath);
  //SH::ScanDir().samplePattern(mysample+"*").filePattern("*root*").scan(sh,inputFilePath);
  //if( mysample.find("C1N2") != std::string::npos || mysample.find("C1C1") != std::string::npos ){
  //  inputFilePath = gSystem->ExpandPathName ("/r03/atlas/chpotter/Part3_Project_2018/DAOD");
  //  SH::ScanDir().filePattern("DAOD_TRUTH1."+mysample+".DAOD.root").scan(sh,inputFilePath);
  //}
  //DAOD_TRUTH1.MC15.999101.MGPy8EG_A14N23LO_C1N2N3_SlepSnu_x0p50_100_1_noFilter.DAOD.root


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
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}

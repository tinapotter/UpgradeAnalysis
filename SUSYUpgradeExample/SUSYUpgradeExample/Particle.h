#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <string>
#include <math.h>

#include "TLorentzVector.h"


class Particle: public TLorentzVector {

 public:


  Particle();
  ~Particle() {};

  
  bool Good;
  float pdgid;
  int nprong;
  float etcone20;
  float ptcone30;
  
  // this is needed to distribute the algorithm to the workers
  ClassDef(Particle, 1);

  
};

#endif

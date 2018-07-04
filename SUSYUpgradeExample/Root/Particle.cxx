#ifndef PARTICLE_CXX
#define PARTICLE_CXX


#include "SUSYUpgradeExample/Particle.h"

Particle::Particle() {
  // This is the default constructor, so we use default settings
  Good = false;
  pdgid=0;
  nprong=0;
  etcone20=0;
  ptcone30=0;
}


#endif

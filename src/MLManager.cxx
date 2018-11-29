// Implementation of MLManager

#include "MLInference/MLManager.h"

using namespace mlinference::base;

MLManager::~MLManager()
{
  for(auto& p : mKernels) {
    if(p) {
      delete p;
    }
  }
}

void MLManager::setFeatures(const std::vector<std::string>& featureNames)
{
  mFeatureNames = featureNames;
}


void MLManager::compute(const Inputs& inputs)
{
  for(const auto& in : inputs) {
    mInputs[in.first] = in.second;
  }
  for(auto& k : mKernels) {
    k->compute();
  }
}

const std::vector<std::unordered_map<std::string, double>>&
MLManager::getPredictions() const
{
  return mPredictions;
}

unsigned int MLManager::nKernels() const
{
  return mNKernels;
}

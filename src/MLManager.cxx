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

void MLManager::setInputs(const std::unordered_map<std::string, double>& inputMap)
{
  mInputs = inputMap;
}

void MLManager::compute()
{
  for(unsigned int i = 0; i < mKernels.size(); i++) {
    mKernels[i]->compute(mInputs, mPredictions[i]);
  }
}

const std::vector<std::unordered_map<std::string, double>>&
MLManager::getPredictions() const
{
  return mPredictions;
}

// Implementation of MLManager

#include "MLInference/Types.h"
#include "MLInference/MLManager.h"

using namespace mlinference;
using namespace mlinference::base;

void MLManager::configure(const std::vector<std::string>& featureNames)
{
  assertState(EState::kPreInit);
  mFeatureNames.resize(featureNames.size());
  for(int i = 0; i < featureNames.size(); i++) {
    mFeatureNames[i] = featureNames[i];
  }
  changeState(EState::kInit);
}

void MLManager::configure(const std::string& JSONPath)
{
  std::vector<std::string> featureNames;
  utilities::JSONToVec(JSONPath, featureNames);
  configure(featureNames);
}

void MLManager::initialize()
{
  assertState(EState::kInit);
  // Initialize the inputs
  for(const auto& fn : mFeatureNames) {
    // Initielize feature values with dummy -1;
    mInputs[fn] = -1.;
  }

  // One prediction container for each MLKernel and initialize
  mPredictions.resize(mKernels.size());
  // One decision container for each MLKernel and initialize together with
  // a decision container
  for(int i = 0; i < mKernels.size(); i++) {
    mPredictions[i].reset(new Predictions());
    mKernels[i]->initialize(&mInputs, mPredictions[i].get());
  }
  changeState(EState::kPostInit);
}

void MLManager::reset()
{
  mKernels.clear();
  mInputs.clear();
  mPredictions.clear();
  mCurrentState = EState::kPreInit;
}

const std::vector<std::string>& MLManager::getFeatureNames() const
{
  return mFeatureNames;
}

Inputs& MLManager::getInputs()
{
  return mInputs;
}

const Inputs& MLManager::getInputs() const
{
  return mInputs;
}

void MLManager::compute()
{
  for(auto& k : mKernels) {
    k->compute();
  }
}

const Predictions& MLManager::getPredictions(unsigned int kernelId) const
{
  if(!hasKernel(kernelId)) {
    std::cerr << "ERROR: Kernel ID " << kernelId << "unknown, exit.\n";
    exit(1);
  }
  return *mPredictions[kernelId];
}

unsigned int MLManager::nKernels() const
{
  return mKernels.size();
}

void MLManager::assertState(EState state) const
{
  if(mCurrentState != state) {
    std::cerr << "ERROR: Wrong state, exit.\n";
    exit(1);
  }
}

void MLManager::changeState(EState state)
{
  mCurrentState = state;
}


bool MLManager::hasKernel(unsigned int kernelId) const
{
  if(kernelId >= mKernels.size()) {
    return false;
  }
  return true;
}

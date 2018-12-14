// Implementation of MLManager

#include "MLInference/Types.h"
#include "MLInference/MLManager.h"

using namespace mlinference;
using namespace mlinference::base;

MLManager::~MLManager()
{
  for(auto& k : mKernels) {
    if(k) {
      delete k;
    }
  }
}

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
  mPredictions.resize(mKernels.size(), Predictions());
  // One decision container for each MLKernel and initialize together with
  // a decision container
  mWorkingPoints.resize(mKernels.size(), Predictions());
  mDecisions.resize(mKernels.size(), Predictions());
  for(int i = 0; i < mKernels.size(); i++) {
    mKernels[i]->initialize(&mInputs, &(mPredictions[i]));
    // Initialize decision map for this MLKernel
    for(auto& p : mPredictions[i]) {
      // Default WP is 0.5. User needs/can specify values later via getting
      mWorkingPoints[i][p.first] = 0.5;
      mDecisions[i][p.first] = 0.;
    }
  }
  changeState(EState::kPostInit);
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

PredictionsVec& MLManager::getWorkingPoints()
{
  return mWorkingPoints;
}

const PredictionsVec& MLManager::getWorkingPoints() const
{
  return mWorkingPoints;
}

void MLManager::compute()
{
  for(auto& k : mKernels) {
    k->compute();
  }
}

const PredictionsVec& MLManager::getPredictions() const
{
  return mPredictions;
}

const PredictionsVec& MLManager::getDecisions()
{
  // Decisions are not computed on the fly together with the predictions but
  // only on request.
  for(unsigned int i = 0; i < mPredictions.size(); i++) {
    for(const auto& p : mPredictions[i]) {
      // Return 1. if prediction is >= working point and 0. otherwise
      mDecisions[i][p.first] = p.second >= mWorkingPoints[i][p.first] ? 1. : 0.;
    }
  }
  return mDecisions;
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

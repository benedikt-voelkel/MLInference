#include "MLInference/XGBoostKernel.h"

//
// Standard constructor
//
using namespace mlinference::xgboost;

MLKernel::MLKernel(unsigned int id, const std::string& modelPath,
                   const std::unordered_map<std::string, std::string>& variableMap)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kXGBoost),
    mmodelPath(modelPath),
    mInputs(nullptr), mPredictions(nullptr)
{
  XGBoosterLoadModel(mHandle, modelPath.c_str());
}

MLKernel::MLKernel(unsigned int id, const std::string& modelPath,
                   const std::string& variableMapJSON)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kXGBoost),
    mmodelPath(modelPath),
    mInputs(nullptr), mPredictions(nullptr)
{
  XGBoosterLoadModel(mHandle, modelPath.c_str());
}


void MLKernel::initialize(Inputs* inputs, Predictions* predictions)
{
  mInputs = inputs;
  mPredictions = predictions;
  // TODO Prepare internal input map

}


//
// Compute predictions and fill output map
//
void MLKernel::compute()
{
  // TODO compute predictions
}

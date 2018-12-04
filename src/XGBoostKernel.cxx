#include "MLInference/LWTNNKernel.h"

//
// Standard constructor
//
using namespace mlinference::xgboost;

MLKernel::MLKernel(unsigned int id, const std::string& modelLibPath,
                   const std::unordered_map<std::string, std::string>& variableMap)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kXGBOOST),
    mModelLibPath(modelLibPath),
    mInputs(nullptr), mPredictions(nullptr)
{
  load();
}

MLKernel::MLKernel(unsigned int id, const std::string& modelLibPath,
                   const std::string& variableMapJSON)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kXGBOOST),
    mModelLibPath(modelLibPath),
    mInputs(nullptr), mPredictions(nullptr)
{
  load();
}

MLKernel::load()
{
  if(TreeliteLoadXGBoostModel(mModelLibPath.c_str(), &mModel) < 0) {
    std::cerr << "Model loading from " << mModelLibPath << " failed.\n";
    exit(1);
  }
}




void MLKernel::initialize(Inputs* inputs, Predictions* predictions)
{
  mInputs = inputs;
  mPredictions = predictions;
  // Prepare internal input map
  for(auto& ic : mConfig.getConfig().inputs) {
    // Set default value -1
    for(auto& variable : ic.variables) {
      mInputMap[ic.name][variable.name] = -1.;
    }
  }
  // Prepare the output map the user will see.
  for(auto& oc : mConfig.getConfig().outputs) {
    for(auto& output : oc.second.labels) {
      mPredictions->operator[](output) = -1.;
    }
  }
}


//
// Compute predictions and fill output map
//
void MLKernel::compute()
{
  // TODO That's a hack for now!!!
  for(auto& ic : mConfig.getConfig().inputs) {
    for(const auto& in : *mInputs) {
      mInputMap[ic.name][in.first] = in.second;
    }
  }
  for(const auto& tmpOut : mGraph.compute(mInputMap)) {
    mPredictions->operator[](tmpOut.first) = tmpOut.second;
  }
}

#include "MLInference/Types.h"
#include "MLInference/LWTNNKernel.h"

//
// Standard constructor
//
using namespace mlinference::lwtnn;

MLKernel::MLKernel(unsigned int id, const std::string& modelJSON,
                   const std::unordered_map<std::string, std::string>& variableMap)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kLWTNN),
    mConfig(modelJSON, variableMap), mGraph(mConfig.getConfig()),
    mInputs(nullptr), mPredictions(nullptr)
{
  mInputMap.clear();
}

MLKernel::MLKernel(unsigned int id, const std::string& modelJSON,
                   const std::string& variableMapJSON)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kLWTNN),
    mConfig(modelJSON, variableMapJSON), mGraph(mConfig.getConfig()),
    mInputs(nullptr), mPredictions(nullptr)
{
  mInputMap.clear();
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

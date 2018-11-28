#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/LWTNNKernel.h"

using namespace mlinference::lwtnn;

//------------------------------------------------------------------------------
// LWTNN Kernel configuration implementation
//------------------------------------------------------------------------------
LWTNNKernelConfig::LWTNNKernelConfig(const std::string& modelFilepath,
                                     const std::unordered_map<std::string, std::string>
                                     featureVarLWTNNVarMap)
  : mModelFilepath(modelFilepath),
    mConfigStream(modelFilepath),
    mConfig(lwt::parse_json_graph(mConfigStream)),
    mVariableMap(featureVarLWTNNVarMap)
{

  // NOTE Only accept one layer of nodes for now
  if(mConfig.inputs.size() > 1) {
    std::cerr << "ERROR: Accept only one layer of input nodes.\n";
    exit(1);
  }
  // Replace (some) LWTNN standard variable names with user defined ones
  if(!mVariableMap.empty()) {
    for(auto& ic : mConfig.inputs) {
      // Prepare the input map for this node
      // Prepare a map wrapper for this node and pass the input map
      // Loop over all variables for one input node
      for(auto& variable : ic.variables) {
        const auto& it = featureVarLWTNNVarMap.find(variable.name);
        if(it == featureVarLWTNNVarMap.end()) {
          continue;
        }
        variable.name = it->second;
      }
    }
  } // end if mVairableMap.empty
}

LWTNNKernelConfig::LWTNNKernelConfig(const LWTNNKernelConfig& rhs)
  : mModelFilepath(rhs.mModelFilepath), mConfigStream(rhs.mModelFilepath),
    mConfig(rhs.mConfig), mVariableMap(rhs.mVariableMap)
{}



//------------------------------------------------------------------------------
// LWTNN Kernel implementation
//------------------------------------------------------------------------------

//
// Standard constructor
//
LWTNNKernel::LWTNNKernel(unsigned int id, const LWTNNKernelConfig& config)
  : mlinference::base::MLKernel(id, EMLType::kNN, EMLBackend::kLWTNN),
    mConfig(config), mGraph(mConfig.getConfig())
{
  mInputMap.clear();
  // Prepare input map
  for(auto& ic : mConfig.getConfig().inputs) {
    // Set default value -1
    for(auto& variable : ic.variables) {
      mInputMap[ic.name][variable.name] = -1.;
    }
  }
}

//
// Compute predictions and fill output map
//
void LWTNNKernel::compute(const std::unordered_map<std::string, double>& inputs,
             std::unordered_map<std::string, double>& outputs)
{
  // TODO That's a hack for now!!!
  for(auto& ic : mConfig.getConfig().inputs) {
    for(const auto& in : inputs) {
      mInputMap[ic.name][in.first] = in.second;
    }
  }
  for(const auto& tmpOut : mGraph.compute(mInputMap)) {
    outputs[tmpOut.first] = tmpOut.second;
  }
}

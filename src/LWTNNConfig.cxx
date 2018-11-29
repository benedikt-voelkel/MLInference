#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/Utilities.h"
#include "MLInference/LWTNNConfig.h"

using namespace mlinference::lwtnn;

KernelConfig::KernelConfig(const std::string& modelFilepath,
                           const std::unordered_map<std::string, std::string>&
                           featureVarLWTNNVarMap)
  : mModelFilepath(modelFilepath),
    mConfigStream(modelFilepath),
    mConfig(lwt::parse_json_graph(mConfigStream)),
    mVariableMap(featureVarLWTNNVarMap)
{
  configure();
}

KernelConfig::KernelConfig(const std::string& modelFilepath,
                           const std::string& variableMapJSON)
  : mModelFilepath(modelFilepath),
    mConfigStream(modelFilepath),
    mConfig(lwt::parse_json_graph(mConfigStream))
{
  mlinference::utilities::JSONToMap(variableMapJSON, mVariableMap);
  configure();
}

void KernelConfig::configure()
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
        const auto& it = mVariableMap.find(variable.name);
        if(it == mVariableMap.end()) {
          continue;
        }
        variable.name = it->second;
      }
    }
  } // end if mVairableMap.empty
}

KernelConfig::KernelConfig(const KernelConfig& rhs)
  : mModelFilepath(rhs.mModelFilepath), mConfigStream(rhs.mModelFilepath),
    mConfig(rhs.mConfig), mVariableMap(rhs.mVariableMap)
{}

#ifndef LWTNN_CONFIG_H_
#define LWTNN_CONFIG_H_

#include <unordered_map>
#include <string>
#include <fstream>

// Include several headers. See the files for more documentation.
// First include the class that does the computation
#include "lwtnn/LightweightGraph.hh"
// Then include the json parsing functions
#include "lwtnn/parse_json.hh"

/// Example implementation for LWTNN wrapper.
/// Generalize this such that different ML algorithms/backends can be used
/// having a common frontend.


namespace mlinference {

  namespace lwtnn {

    /// Storing the configuration fro LWTNN
    class KernelConfig
    {
      public:
        KernelConfig(const std::string& modelFilepath,
                     const std::unordered_map<std::string, std::string>&
                     featureVarLWTNNVarMap);

        KernelConfig(const std::string& modelFilepath,
                     const std::string& variableMapJSON);

        KernelConfig(const KernelConfig& rhs);
        KernelConfig() = delete;
        ~KernelConfig() = default;

        const lwt::GraphConfig& getConfig() const
        {
          return mConfig;
        }

      private:
        /// Configure mConfig from mVariableMap
        void configure();

      private:
        /// Filepath
        std::string mModelFilepath;
        /// The config file stream
        std::ifstream mConfigStream;
        /// Parsed config
        lwt::GraphConfig mConfig;
        /// Feature variable name to LWTNN variable names map
        std::unordered_map<std::string, std::string> mVariableMap;
    };
  } // end namespace lwtnn
} // end namespace mlinference

#endif /* LWTNN_CONFIG_H_ */

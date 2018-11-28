#ifndef LWTNN_KERNEL_H_
#define LWTNN_KERNEL_H_

#include <type_traits> // std::enable_if
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
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

#include "MLInference/MLKernel.h"

namespace mlinference {

  namespace lwtnn {

    /// Storing the configuration fro LWTNN
    class LWTNNKernelConfig
    {
      public:
        LWTNNKernelConfig(const std::string& modelFilepath,
                          const std::unordered_map<std::string, std::string>
                          featureVarLWTNNVarMap);


        LWTNNKernelConfig(const LWTNNKernelConfig& rhs);
        LWTNNKernelConfig() = delete;
        ~LWTNNKernelConfig() = default;

        const lwt::GraphConfig& getConfig() const
        {
          return mConfig;
        }

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


    class LWTNNKernel : public mlinference::base::MLKernel
    {
      public:
        LWTNNKernel(unsigned int id, const LWTNNKernelConfig& config);

        LWTNNKernel(const LWTNNKernel&) = delete;
        LWTNNKernel& operator=(const LWTNNKernel&) = delete;

        ~LWTNNKernel() = default;

        void compute(const std::unordered_map<std::string, double>& inputs,
                     std::unordered_map<std::string, double>& outputs) override final;

      private:
        /// Stored config
        LWTNNKernelConfig mConfig;
        /// The LWTNN graph doing the computation.
        lwt::LightweightGraph mGraph;
        /// The input map.
        std::map<std::string,std::map<std::string,double>> mInputMap;

    };
  } // end namespace lwtnn
} // end namespace mlinference

#endif /* LWTNN_KERNEL_H_ */

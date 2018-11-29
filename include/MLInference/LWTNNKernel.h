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
#include "MLInference/LWTNNConfig.h"

namespace mlinference {

  namespace lwtnn {

    class MLKernel : public mlinference::base::MLKernel
    {
      public:
        MLKernel(unsigned int id, const std::string& modelJSON,
                 const std::unordered_map<std::string, std::string>& variableMap);

        MLKernel(unsigned int id, const std::string& modelJSON,
                 const std::string& variableMapJSON);

        MLKernel(const MLKernel&) = delete;
        MLKernel& operator=(const MLKernel&) = delete;

        ~MLKernel() = default;

        void initialize(Inputs* inputs, Predictions* predictions) override final;
        void compute() override final;

      private:
        /// Stored config
        KernelConfig mConfig;
        /// The LWTNN graph doing the computation.
        lwt::LightweightGraph mGraph;
        /// The input map.
        std::map<std::string,std::map<std::string,double>> mInputMap;
        /// Pointer to global inputs
        Inputs* mInputs;
        /// Pointer to global predictions
        Predictions* mPredictions;

    };
  } // end namespace lwtnn
} // end namespace mlinference

#endif /* LWTNN_KERNEL_H_ */

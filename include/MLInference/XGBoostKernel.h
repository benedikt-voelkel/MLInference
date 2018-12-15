#ifndef XGBOOST_KERNEL_H_
#define XGBOOST_KERNEL_H_

/// Example implementation for LWTNN wrapper.
/// Generalize this such that different ML algorithms/backends can be used
/// having a common frontend.

/// NOTE
/// In principlde all XGBoost, LightGBM and SciKit models could be handled
/// by one class. However, split which makes it simpler if there will be another
/// (meaning better) interface for one of them in the future.

#include "MLInference/Types.h"
#include "MLInference/MLKernel.h"

#include "xgboost/c_api.h"

namespace mlinference {

  namespace xgboost {

    class MLKernel : public mlinference::base::MLKernel
    {
      public:
        MLKernel(unsigned int id, const std::string& modelPath,
                 const std::unordered_map<std::string, std::string>& variableMap);

        MLKernel(unsigned int id, const std::string& modelPath,
                 const std::string& variableMapJSON);

        MLKernel(const MLKernel&) = delete;
        MLKernel& operator=(const MLKernel&) = delete;

        ~MLKernel() = default;

        void initialize(Inputs* inputs, Predictions* predictions) override final;
        void compute() override final;

      private:
        /// Load everything from the library and build the model
        void load();

      private:
        /// Full path to model file
        std::string mmodelPath;
        /// Model handle of treelite
        ModelHandle mModel;
        /// Pointer to global inputs
        Inputs* mInputs;
        /// Pointer to global predictions
        Predictions* mPredictions;
        /// The handle to load a model to
        BoosterHandle mHandle;

    };
  } // end namespace lwtnn
} // end namespace mlinference

#endif /* XGBOOST_KERNEL_H_ */

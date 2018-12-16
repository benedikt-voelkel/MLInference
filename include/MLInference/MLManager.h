#ifndef ML_MANAGER_H_
#define ML_MANAGER_H_

#include <utility> // std::forward
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/MLKernel.h"
#include "MLInference/Utilities.h"

// This class manages instanciation of ML algorithms as well as centralised
// application
// 1) Have to take same inputs?! NOTE
// 1.1) If one takes certain inputs and another algorithm just a subset, can
//      we just pass all and the latter would ignore those not needed?
// 1.2) Input names/ordering of input notes must be the same.
// 2) Don't use names mapping to nodes but just numbers
//    ==> Can use setFeature(i, 42.);
// 2.1) Do we need pre-processing in terms of interpreting or compilation in
//      place?
//      ==> Benchmarking
// 3) Can we have different output topologies?
// 4) This is a singleton class to get access to things from anywhere.


namespace mlinference {

  namespace base {

    enum class EState {kPreInit, kInit, kPostInit};

    class MLManager
    {
      public:
        /// Get reference to static instance
        static MLManager& getInstance()
        {
          static MLManager instance;
          return instance;
        }
        /// Delete copy and assignement
        MLManager(const MLManager&) = delete;
        MLManager& operator=(const MLManager&) = delete;

        ~MLManager();

        /// Create a MLKernel by providing arguments which will be forwarded to
        /// the respective kernel construction
        template <typename T, typename... TArg>
        typename std::enable_if<std::is_base_of<mlinference::base::MLKernel, T>::value, unsigned int>::type
        createKernel(TArg&&... Args)
        {
          assertState(EState::kInit);
          mKernels.push_back(new T(mKernels.size(), std::forward<TArg>(Args)...));
          return mKernels.size() - 1;
        }

        /// Configure, so far only setting feature names
        void configure(const std::vector<std::string>& featureNames);

        /// Configure and set feature names extracting them from JSON
        void configure(const std::string& JSONPath);

        /// Initialize all MLKernels passing input and output container pointers
        void initialize();

        /// Reset the MLManager and all its backends
        void reset();

        /// Get feature names, can be useful if they were set from a JSON
        const std::vector<std::string>& getFeatureNames() const;

        /// Get a reference to inputs to set them directly
        Inputs& getInputs();
        /// Get a reference to inputs read only
        const Inputs& getInputs() const;

        /// Get reference to woking points to set them
        PredictionsVec& getWorkingPoints();
        /// Get reference to woking points read only
        const PredictionsVec& getWorkingPoints() const;

        /// Compute the predictions
        void compute();

        /// Get predictions from all algorithms ordered by ID
        const PredictionsVec& getPredictions() const;

        /// Get the decision based on user defined working points
        /// 0: prediction < WP
        /// 1: prediction >= WP
        /// Default WP = 0.5
        const PredictionsVec& getDecisions();

        /// Get number of kernels
        unsigned int nKernels() const;

      private:
        /// Private default constructor only used by static method getInstance()
        MLManager() = default;

        /// Make sure MLManager is in required state.
        void assertState(EState state) const;

        /// Change the current state
        void changeState(EState state);

      private:
        /// Hold all instanciated ML inferers
        std::vector<MLKernel*> mKernels;
        /// The feature names
        std::vector<std::string> mFeatureNames;
        /// All inputs
        Inputs mInputs;
        /// Summing up all predictions
        PredictionsVec mPredictions;
        /// Working points for all output neurons of all MLKernels
        PredictionsVec mWorkingPoints;
        /// Decisions made based on predictions of working points
        PredictionsVec mDecisions;
        /// The current state
        EState mCurrentState = EState::kPreInit;
    };
  } // end namespace base

} // end namespace cppmlinference

#endif /* ML_MANAGER_H_ */

#ifndef ML_MANAGER_H_
#define ML_MANAGER_H_

#include <utility> // std::forward
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/MLKernel.h"
#include "MLInference/LWTNNKernel.h"

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

    class MLManager
    {
      public:
        /// Get reference to static instance
        static MLManager& getInstance()
        {
          static MLManager instance;
          return instance;
        }
        // Delete copy and assignement
        MLManager(const MLManager&) = delete;
        MLManager& operator=(const MLManager&) = delete;

        ~MLManager();

        /// Create an algorithm by providing a path to a config file and
        /// additional arguments which will be forwarded to the respective
        /// kernel construction
        template <typename... TArg>
        unsigned int createAlgorithm(EMLBackend backend, TArg&&... Args)
        {
          switch(backend) {
            case EMLBackend::kLWTNN: createKernelLWTNN(std::forward<TArg>(Args)...);
                                     mPredictions.push_back(std::unordered_map<std::string,double>());
                                     mNKernels++;
                                     break;
            // Default doesn't make sense but again: testing
            default: std::cerr << "Default: No ML algorithm created\n";
                     break;
          }

        }

        /// Setting all feature names at once
        void setFeatures(const std::vector<std::string>& featureNames);

        /// Set inputs to all registered algorithms
        void setInputs(const std::unordered_map<std::string, double>& inputMap);

        /// Compute the predictions
        void compute();

        /// Get predictions from all algorithms ordered by ID
        const std::vector<std::unordered_map<std::string, double>>&
        getPredictions() const;

      private:
        /// Private default constructor only used by static method getInstance()
        MLManager() = default;

        /// Creating kernel for different ML backends
        /// TODO Should this be outsourced or stay with the MLManager?
        ///      Then could think about allowing for macros creating kernels
        ///      such that new kernels could be used on the fly.
        void createLWTNNKernel(const std::string& modelJSON,
                               const std::unordered_map<std::string, std::string> featureLWTNNMap)
        {
          lwtnn::LWTNNKernelConfig config(modelJSON, featureLWTNNMap);
          mKernels.push_back(new lwtnn::LWTNNKernel(mNKernels, config));
        }

        /// Add a prediction map for a kernel to write its predictions to
        void addPredictionMap(unsigned int kernelId);

      private:
        /// Number of registered kernels
        unsigned int mNKernels = 0;
        /// Hold all instanciated ML inferers
        std::vector<MLKernel*> mKernels;
        /// The feature names
        std::vector<std::string> mFeatureNames;
        /// All inputs
        std::unordered_map<std::string, double> mInputs;
        /// Summing up all predictions
        std::vector<std::unordered_map<std::string,double>> mPredictions;
    };
  } // end namespace base

} // end namespace cppmlinference

#endif /* ML_MANAGER_H_ */

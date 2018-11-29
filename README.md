# C++ ML inference
This repository is work in progress. The goal is a general wrapper to apply trained ML algorithms in a C++ environment. The training is supposed to be completely independent of the inference. So far the only backend supported [LWTNN](https://github.com/lwtnn/lwtnn) which is able to rebuild NNs trained in Keras.

## Requirements
* [CMake](https://cmake.org/) in order to install this project
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (required by LWTNN, see link below for further info)
* [Boost](https://www.boost.org/) (its property-tree is required by LWTNN, see link below for further info)
* [LWTNN](https://github.com/lwtnn/lwtnn)

## Installation
This is installed using CMake according to
```bash
mkdir $BUILD_DIR; cd $BUILD_DIR
cmake [-DCMAKE_INSTALL_PREFIX=$INSTALL_DIR] -DEIGEN_DIR=<top-Eigen-install-dir> \
                                            -DLWTNN_DIR=<top-LWTNN-install-dir> \
                                            [-DBOOST_ROOT=<top-LWTNN-install-dir>]
make install
```
Set `BOOST_ROOT` in case you have `Boost` installed to a custom path. Although you could try an in-source build (not tested) this is not recommended (especially to not mess up the source directory). So let's suggest you have it installed to `$INSTALL_DIR` in the following where after installation there are libraries and headers.

## Usage
As this package deals with inference of a trained ML algorithm, there is no training involved at all. The interface between the training and this package is a configuration file providing the architecture and the parameter values of the trained algorithm.

### Keras via LWTNN
After training a NN in Keras, the architecture needs to be saved in [JSON format](https://keras.io/getting-started/faq/#how-can-i-save-a-keras-model) (see "Saving/loading only a model's architecture") and the weights must be dumped in HDF5 format [HDF5 format](https://keras.io/getting-started/faq/#how-can-i-save-a-keras-model) (see "Saving/loading only a model's weights"). Important: LWTNN is guaranteed to work with [Keras functional models](https://keras.io/getting-started/functional-api-guide/) so either start with that or convert it to a functional model after training via
```python
func_model = seq_model.model
```
 Finally, the architecture and weights need to be combined in one JSON file using
 ```bash
 $INSTALL_DIR/scripts/keras2lwtnn -a <arch.json> -w <weights.h5> [-m <saved_model.json>]
 ```
 The default for the saved model is `model.json`.

### Loading the model
The core is the `MLManager` which is used to create ML kernels providing a unified interface to talk to them at the same time. A basic implementation could look like the following:
```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/MLManager.h"
//...
int main(int argc, char* argv[])
{
  //...
  // Get reference to the global manager
  mlinference::base::MLManager& mgr = mlinference::base::MLManager::getInstance();
  const std::vector<std::string> featureNames = {"feature_1", "feature_2", "feature_N"};
  // configure it
  mgr.configure(featureNames);
  // For LWTNN we need some mapping of internally used feature names "variable_i" to global common feature names (can be ommitted)
  const std::unordered_map<std::string, std::string> featureNamesToLWTNNMap = { {"variable_0": "feature_1"}, {"variable_1": "feature_2"}, {"variable_N-1", "feature_N"} };
  // Add a kernel (so far only LWTNN is available)
  unsigned int kernelID = mgr.createKernel<mlinference::lwtnn::MLKernel>(modelJsonFilepath, featureNamesToLWTNNMap);
  // Initialize the MLManager will lock such that no more MLKernels can be added
  mgr.initialize();
  // Connect to inputs via reference
  mlinference::Inputs& inputs = mgr.getInputsRef();

  //
  // In the following you would loop over your samples deriving a prediction for each one
  //

  // Set features
  inputs["feature_1"] = 5.;
  //...
  inputs["feature_N"] = 42.;
  // compute predictions
  mgr.compute();
  // get the predictions
  const Predictions& predictions = mgr.getPredictions(kernelID);
  // do whatever
  for(const auto& p : predictions) {
    std::cout << "Prediction " << p.first << " is " << p.second << "\n";
  }

  //
  // End looping over samples
  //

  //...

  return 0;
}
```

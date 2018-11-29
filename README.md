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
The core is the `MLManager` which is used to create new ML algorithms based on configuration files. To use this for LWTNN, do
```c++
#include "MLInference/Types.h"
#include "MLInference/MLManager.h"
//...

mlinference::base::MLManager& mgr = mlinference::base::MLManager::getInstance();
mgr.createKernel(mlinference::EMLBackend::kLWTNN, modelJsonFilepath, featureNamesToLWTNN)

```


### Preparing the input
The following things are required:
* input `JSON` file specifying some information on the input `ROOT` file and tree:
```json
{
  "treename": "some_treename",
  "filepath": "some_file.root",
  "copyBranches": ["branch_to_copy_1", "branch_to_copy_2", "branch_to_copy_N" ],
  "branchMap": {
    "variable_0": "feature_branch_name_1",
    "variable_1": "feature_branch_name_2",
    "variable_2": "feature_branch_name_3",
    "variable_3": "feature_branch_name_4",
    "variable_4": "feature_branch_name_5",
    "variable_5": "feature_branch_name_6",
    "variable_6": "feature_branch_name_7",
    "variable_7": "feature_branch_name_8",
    "variable_N": "feature_branch_name_9"
  }
}
```
The treename and filename are obvious. It is also possible to copy some branches to the target tree if needed by specifying the the list at `copyBranches`. If `branchMap` is not specified, the branch names will be assumed to be `variable_i`.

* output `JSON` file specifying some information on the output `ROOT` file and tree
```json
{
  "treename": "some_treename",
  "filepath": "some_file.root",
  "branchMap": {
    "out_0": "prediction_branch_name_1",
    "out_1": "prediction_branch_name_2",
    "out_2": "prediction_branch_name_3",
    "out_N": "prediction_branch_name_N"
  }
}
```
Here the structure is analoguous to the input `JSON`.

* model `JSON` file containing the converted `keras` model which can be digested by `LWTNN`
In order to get this you need the NN's architecture as well as the weights. The former one needs to be available in `JSON` format whereas the second one must be in `HDF5`. Both can be written out after training in `keras` easily, see [here](https://keras.io/getting-started/faq/). Also, please use `Keras`' [functional API](https://keras.io/getting-started/functional-api-guide/) when training or convert the output files accordingly. Assuming the architecture and weights files are present, conversion is done via
```bash
keras2lwtnn -a <architecture_file> -a <weights_file> [-m <model_file>]
```
Via the last parameter the full path of the created model file can be specified. The default is `$(pwd)/model.json`.

### Running
After input preparation run
```bash
applyNN <model_file> <input_json> <output_json>
```
This should give you a `ROOT` file with desired predictions.

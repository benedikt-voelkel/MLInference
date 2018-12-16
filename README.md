# C++ ML inference
This repository is work in progress. The goal is a general wrapper to apply trained ML algorithms in a C++ environment. The training is supposed to be completely independent of the inference. So far the only backend supported [LWTNN](https://github.com/lwtnn/lwtnn) which is able to rebuild NNs trained in Keras.

## Dependencies and requirements

### Build requirements

This package is built via [CMake](https://cmake.org/) with minimum version `3.5.0`. Furthermore, a GCC toolchain with CXX11 support is required.

### Optional dependencies

The core of this package has no dependencies at all. However, in that case it is built without any supported backend and hence cannot be deployed immediately. This mode can be used in case a user wants to implement a backend from scratch using the `MLManager` as the steering instance. Instruction of how to implement a backend can be find [below](## Adding a custom backend).

Currently supported backends built in are [LWTNN](https://github.com/lwtnn/lwtnn) (tested with version v2.7.1) and [XGBoost](https://xgboost.readthedocs.io/en/latest/index.html) (tested with version v0.81). These have themselves a few dependencies which are

* `LWTNN`
  * [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (tested with version 3.3.5)
  * [Boost](https://www.boost.org/) (actually only its property-tree is required, tested with version v1.68.0)

* `XGBoost` The easiest way is to go for `git clone --recursive https://github.com/dmlc/xgboost` which recursively clones the dependencies `dmlc` (aka `dmlc-core`) and `rabit`. Afterwards, just follow these [instructions](https://xgboost.readthedocs.io/en/latest/build.html#building-the-shared-library) to build the shared library. That is an in-source built which implies building libraries of `dmlc` and `rabit` as well. If you wish you can of course build everything separately.

## Installation
This is installed using CMake according to
```bash
mkdir $BUILD_DIR; cd $BUILD_DIR
cmake [-DCMAKE_INSTALL_PREFIX=$INSTALL_DIR] [-DEigen_DIR=<top-Eigen-install-dir>] \
                                            [-DLWTNN_DIR=<top-LWTNN-install-dir>] \
                                            [-DBOOST_DIR=<top-Boost-install-dir>] \
                                            [-Ddmlc_DIR=<top-dmlc-install-dir>] \
                                            [-Drabit_DIR=<top-rabit-install-dir>] \
                                            [-DXGBoost_DIR=<top-XGBoost-install-dir>] \
                                            $SOURCE_DIR
make install
```
The backends are only installed if their respective dependencies are found. You will be informed by the output of `CMake` what is going to be built.
Set `BOOST_DIR` if you have `Boost` installed to a custom path. An in-source built has not been not tested and it is not recommended (especially to not mess up the source directory). So let's assume in the following the installation went to `$INSTALL_DIR`.

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
The core is the `MLManager` which is used to create ML kernels providing a unified interface to talk to them at the same time. Look into the [test directory](test) to get an idea about the basic usage of the `MLManager` as well as together with backends.

[WIP] Document in Wiki.

## Adding a custom backend

[WIP] Instruction of how to implement a custom backend.

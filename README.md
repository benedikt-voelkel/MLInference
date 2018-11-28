# Test and learn LWTNN application
This repository contains a small wrapper around [LWTNN](https://github.com/lwtnn/lwtnn) to apply Neural Networks trained in `Keras` in a `C++` environment. Features can be read directly from a given input `ROOT` file and the predictions are written to an output file. The branch type of the feature branches is in principle arbitrary as long as

1. there is a meaningful casting between this type and `double` is natively supported in `C++`,
2. it is a scalar branch (so no vectors or user types).

Implementation for vector like branches will be added which contain types meeting the above criteria.

## Requirements
* [CMake](https://cmake.org/) in order to install this project
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (required by LWTNN, see link below for further info)
* [Boost](https://www.boost.org/) (its property-tree is required by LWTNN, see link below for further info)
* [LWTNN](https://github.com/lwtnn/lwtnn)
* [ROOT](https://github.com/root-project/root) (code tested with v6.12.06).

## Installation
This is installed using CMake according to
```bash
mkdir $BUILD_DIR; cd $BUILD_DIR
cmake [-DCMAKE_INSTALL_PREFIX=$INSTALL_DIR] -DROOT_DIR=<top-ROOT-install-dir> \
                                            -DEIGEN_DIR=<top-Eigen-install-dir> \
                                            -DLWTNN_DIR=<top-LWTNN-install-dir> \
                                            [-DBOOST_ROOT=<top-LWTNN-install-dir>]
make install
```
Set `BOOST_ROOT` in case you have `Boost` installed to a custom path. Although you could try an in-source build (not tested) this is not recommended (especially to not mess up the source directory). So let's suggest you have it installed to `$INSTALL_DIR` in the following.
The library installed is for now not important. But the binaries might be useful and further info can be found in [Usage](#Usage). There are
* `$INSTALL_DIR/bin/trainNN`
* `$INSTALL_DIR/bin/applyLWTNN`
* `$INSTALL_DIR/bin/plotLWTNN`

## Usage
You can use anything from anywhere in your shell if you source `$INSTALL_DIR/bin/setup` first. It adds necessary paths to `PATH` and `LD_LIBRARY_PATH`/`DYLD_LIBRARY_PATH`. A few other environment variables like `LWTNN_CONVERTER_DIR/=$LWTNN_DIR/converters` and paths to includes and libraries are exported for convenience as well. Check what's there by typing `env` after sourcing.

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

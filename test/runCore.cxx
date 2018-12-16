// Test core functionality of MLInference

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MLInferenceCoreTest
#include <boost/test/unit_test.hpp>
#include <boost/test/results_collector.hpp>

#include <string>
#include <utility>
#include <vector>

#include <iostream>

#include "MLInference/Types.h"
#include "MLInference/MLManager.h"

// checking meta info and whether there is a value container the analysis can be conpared with
BOOST_AUTO_TEST_CASE(testMLManager)
{
  mlinference::base::MLManager& mgr = mlinference::base::MLManager::getInstance();

  // Check reading feature names from std::vector
  const std::vector<std::string> featureNames = {"feature1", "feature2", "feature42"};
  mgr.configure(featureNames);
  mgr.initialize();

  // Loop over features just set and set a value
  const std::vector<std::string>& featureNamesRet1 = mgr.getFeatureNames();
  mlinference::Inputs& inputs1 = mgr.getInputs();
  for(const auto& fn : featureNames) {
    BOOST_REQUIRE(std::find(featureNamesRet1.begin(), featureNamesRet1.end(), fn) != featureNamesRet1.end());
    inputs1[fn] = 42.;
  }
  // Reset the MLManager
  mgr.reset();

  // This time configure from JSON file assuming the cmd arg is path to one
  // Expect asme features as above
  BOOST_REQUIRE(boost::unit_test::framework::master_test_suite().argc == 2);
  mgr.configure(boost::unit_test::framework::master_test_suite().argv[1]);
  mgr.initialize();
  // Loop over features just set and set a value
  const std::vector<std::string>& featureNamesRet2 = mgr.getFeatureNames();
  mlinference::Inputs& inputs2 = mgr.getInputs();
  for(const auto& fn : featureNames) {
    BOOST_REQUIRE(std::find(featureNamesRet2.begin(), featureNamesRet2.end(), fn) != featureNamesRet2.end());
    inputs2[fn] = -42.;
  }
  mgr.reset();
}

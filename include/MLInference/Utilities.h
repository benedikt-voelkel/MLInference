#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

// Read a config JSON
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include <boost/exception/all.hpp>

#include "MLInference/Types.h"

// Few useful helpre functions


namespace mlinference {

  namespace utilities {

    /// Put JSON to map assuming flat JSON, value type being the same for
    /// all children
    template <typename T>
    void JSONToMap(const std::string& JSONFilepath,
                   std::unordered_map<std::string, T>& map)
    {
      namespace pt = boost::property_tree;
      pt::ptree filePt;
      // Try load property trees
      try {
        pt::read_json(JSONFilepath, filePt);

      } catch( const boost::exception& e ) {
        std::cerr << "ERROR: Could not read JSON file! " << JSONFilepath
                  << "\n";
        std::cerr << boost::diagnostic_information(e) << std::endl;
        exit(1);
      }
      for (pt::ptree::value_type& child : filePt) {
        map[child.first] = child.second.data();
      }
    }

    /// Put JSON to vector assuming flat JSON, value type being the same for
    /// all children
    template <typename T>
    void JSONToVec(const std::string& JSONFilepath,
                   std::vector<T>& vec)
    {
      namespace pt = boost::property_tree;
      pt::ptree filePt;
      // Try load property trees
      try {
        pt::read_json(JSONFilepath, filePt);

      } catch( const boost::exception& e ) {
        std::cerr << "ERROR: Could not read JSON file! " << JSONFilepath
                  << "\n";
        std::cerr << boost::diagnostic_information(e) << std::endl;
        exit(1);
      }
      for (pt::ptree::value_type& child : filePt) {
        vec.push_back(child.second.data());
      }
    }
  } // end namespace utilities
} // end namespace mlinference

#endif /* UTILITIES_H_ */

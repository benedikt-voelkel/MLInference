#ifndef ML_TYPES_H_
#define ML_TYPES_H_

#include <string>
#include <vector>
#include <unordered_map>

namespace mlinference {

  /// Specify the type of the ML algorithm
  enum class EMLType {kNN, kBDT, kSVM, kUnknown};
  /// Specify the backend
  enum class EMLBackend {kLWTNN, kXGBoost, kUnknown};

  typedef std::unordered_map<std::string, double> Predictions;
  typedef Predictions Inputs;
  typedef std::vector<Predictions> PredictionsVec;

} // end namespace cppmlinference

#endif /* ML_TYPES_H_ */

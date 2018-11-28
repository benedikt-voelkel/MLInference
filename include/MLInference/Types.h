#ifndef ML_TYPES_H_
#define ML_TYPES_H_

namespace mlinference {

  /// Specify the type of the ML algorithm
  enum class EMLType {kNN, kBDT, kSVM, kUnknown};
  /// Specify the backend
  enum class EMLBackend {kLWTNN, kUnknown};

} // end namespace cppmlinference

#endif /* ML_TYPES_H_ */

#ifndef ML_KERNEL_H_
#define ML_KERNEL_H_

#include "MLInference/Types.h"

namespace mlinference {

  namespace base {

    class MLKernel
    {
      public:
        /// Standard constructor
        MLKernel(unsigned int id, EMLType type, EMLBackend backend)
          : mId(id), mMLType(type), mMLBackend(backend)
        {}
        /// Delete default constructor since id and type are required
        MLKernel() = delete;
        /// Don't allow copying since algorithms are centrally managed by the
        /// ML manager and cannot e.g. assign an ID to themselves
        MLKernel(const MLKernel&) = delete;
        MLKernel& operator=(const MLKernel&) = delete;
        /// Virtual destructor since it's an abstract base class
        virtual ~MLKernel() = default;

        /// Get the unique ID of this MLKernel
        unsigned int getId() const { return mId; }
        /// Get the type
        EMLType getType() const { return mMLType; }
        /// Get the used backend
        EMLBackend getBackend() const { return mMLBackend; }

        /// When initializing input and prediction pointers are passed
        virtual void initialize(Inputs* inputs, Predictions* predictions) = 0;

        /// Get inputs and write to provided output.
        virtual void compute() = 0;


      private:
        /// Unique identification
        unsigned int mId;
        /// MLKernel type
        EMLType mMLType;
        /// Kernel computing the predictions form the inputs
        EMLBackend mMLBackend;

    };
  }
}

#endif

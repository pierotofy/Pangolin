#pragma once

#include <farm_ng/core/logging/expected.h>

namespace farm_ng
{

// Represents a Non-nullable pointer with shared ownership
// Is essentially an adapter between std::shared_ptr and farm_ng::Expected
template<class T>
class Shared{
public:
    /// Construct from a possibly null shared_ptr
    /// The return value is an object containing either a non-null Shared object pointer,
    //  or an farm_ng::Error object
    static Expected<Shared<T>> tryFrom(std::shared_ptr<T> const& maybe_null) noexcept {
        if (!maybe_null){
            return FARM_ERROR("is null");
        }
        return Shared<T>(std::move(maybe_null)); 
    }

    /// Construct and also makes interior object T
    /// The return value is an object containing either a non-null Shared object pointer,
    /// for the new object, or an farm_ng::Error object if memory allocation failst or 
    /// the constructor for the object throws
    template<class... Args>
    static Expected<Shared<T>> tryMake(Args&&... args) noexcept {
        try {
          // can throw on bad memory allocation and exceptions in constructor of T.
          return tryFrom(std::make_shared<T>(std::forward<Args...>(args...)));
        } catch(std::exception&e) {
            return FARM_ERROR(e.what());
        }
        return FARM_ERROR();
    }

   /// Construct from a possibly null shared_ptr
   /// Panics if shared is null. See `tryFrom()` for alternate.
   static Shared from(std::shared_ptr<T> const& shared) noexcept {
        return FARM_UNWRAP(tryCreate(shared)); 
    }

    /// Construct and also makes interior object T
    /// Panics if the constructor throws. See `tryFrom()` for alternate.
    template<class... Args>
    static Shared<T> make(Args&&... args) noexcept {
        return FARM_UNWRAP(tryMake(std::forward<Args...>(args...)));
    }

    /// Returns the interior object which is guarenteed to be available
    T& operator*() { return *non_null_shared_; }

    /// Returns the interior object which is guarenteed to be available
    const T& operator*() const { return *non_null_shared_; }
    
    /// Returns the interior object which is guarenteed to be available
    T* operator->() { return non_null_shared_.get(); }

    /// Returns the interior object which is guarenteed to be available
    const T* operator->() const { return non_null_shared_.get(); }

private:

    Shared(std::shared_ptr<T> const& shared)
        : non_null_shared_(shared)
    {
    }

  // Class invariant:non_null_shared_ is guaranteed not to be null. 
  std::shared_ptr<T> non_null_shared_;
};

template<typename T>
using ExpectShared = Expected<Shared<T>>;

template<class T, class... Args>
static ExpectShared<T> tryMakeShared(Args&&... args) noexcept {
    return Shared<T>::tryMake( std::forward<Args>(args)... );
}

/// + represents a checked dereference similar to operator*()
/// Unlike Expected::operator*() which will allow for UB,
/// + will Panic if x is in the error state.
template<class T>
Shared<T>& operator+(ExpectShared<T>& x)
{
    return x.value();
}

/// + represents a checked dereference similar to operator*()
/// Unlike Expected::operator*() which will allow for UB,
/// + will Panic if x is in the error state.
template<class T>
const Shared<T>& operator+(const ExpectShared<T>& x)
{
    return x.value();
}

}
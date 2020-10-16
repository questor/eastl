
// taken from google marl:

#include <functional>
#include <memory>

// Finally is a pure virtual base class, implemented by the templated
// FinallyImpl.
class Finally {
 public:
  virtual ~Finally() = default;
};

// FinallyImpl implements a Finally.
// The template parameter F is the function type to be called when the finally
// is destructed. F must have the signature void().
template <typename F>
class FinallyImpl : public Finally {
 public:
  inline FinallyImpl(const F& func);
  inline FinallyImpl(F&& func);
  inline FinallyImpl(FinallyImpl<F>&& other);
  inline ~FinallyImpl();

 private:
  FinallyImpl(const FinallyImpl<F>& other) = delete;
  FinallyImpl<F>& operator=(const FinallyImpl<F>& other) = delete;
  FinallyImpl<F>& operator=(FinallyImpl<F>&&) = delete;
  F func;
  bool valid = true;
};

template <typename F>
FinallyImpl<F>::FinallyImpl(const F& func) : func(func) {}

template <typename F>
FinallyImpl<F>::FinallyImpl(F&& func) : func(std::move(func)) {}

template <typename F>
FinallyImpl<F>::FinallyImpl(FinallyImpl<F>&& other)
    : func(std::move(other.func)) {
  other.valid = false;
}

template <typename F>
FinallyImpl<F>::~FinallyImpl() {
  if (valid) {
    func();
  }
}

template <typename F>
inline FinallyImpl<F> make_finally(F&& f) {
  return FinallyImpl<F>(std::move(f));
}

template <typename F>
inline std::shared_ptr<Finally> make_shared_finally(F&& f) {
  return std::make_shared<FinallyImpl<F>>(std::move(f));
}

#define MARL_CONCAT_(a, b) a##b
#define MARL_CONCAT(a, b) MARL_CONCAT_(a, b)

#define defer(x) \
  auto MARL_CONCAT(defer_, __LINE__) = make_finally([&] { x; })

#if 0
int main() {
    char *blub = new char[1024];
    defer(delete[] blub);
    blub[100] = 100;
    printf("%s", blub);
    blub[102] = 100;
    printf("%s", blub);
    blub[102] = 100;
    printf("%s", blub);
    blub[102] = 100;
    printf("%s", blub);
}
#endif

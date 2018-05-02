// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TASK_CALLBACK_H_
#define LYNX_BASE_TASK_CALLBACK_H_

#if GNU_SUPPORT || !OS_ANDROID
#include <stddef.h>
#include <tuple>
#include <type_traits>
#endif

#include "base/debug/memory_debug.h"
#include "base/scoped_ptr.h"
#include "base/weak_ptr.h"

namespace base {

class Clouse {
 public:
  Clouse() {}
  virtual ~Clouse() {}
  virtual void Run() {}
};
#if !GNU_SUPPORT && OS_ANDROID
template <class Class>
class Callback0 : public Clouse {
 public:
  typedef void (Class::*CallbackFunc)();

  Callback0(CallbackFunc func, WeakPtr<Class>& ptr)
      : callback_func_(func), ptr_(ptr) {}

  void Run() {
    if (ptr_.IsValid()) {
      (ptr_.Get()->*callback_func_)();
    }
  }

 private:
  CallbackFunc callback_func_;

  WeakPtr<Class> ptr_;
};

template <class Class, class FuncArg1, class Arg1>
class Callback1 : public Clouse {
 public:
  typedef void (Class::*CallbackFunc)(FuncArg1 arg1);

  void Run() {
    if (ptr_.IsValid()) {
      (ptr_.Get()->*callback_func_)(arg1_);
    }
  }

  Callback1(CallbackFunc func, WeakPtr<Class>& ptr, Arg1& arg1)
      : callback_func_(func), ptr_(ptr), arg1_(arg1) {}

 private:
  CallbackFunc callback_func_;

  WeakPtr<Class> ptr_;

  Arg1 arg1_;
};

template <class Class, class FuncArg1, class FuncArg2, class Arg1, class Arg2>
class Callback2 : public Clouse {
 public:
  typedef void (Class::*CallbackFunc)(FuncArg1 arg1, FuncArg2 arg2);

  void Run() {
    if (ptr_.IsValid()) {
      (ptr_.Get()->*callback_func_)(arg1_, arg2_);
    }
  }

  Callback2(CallbackFunc func, WeakPtr<Class>& ptr, Arg1& arg1, Arg2& arg2)
      : callback_func_(func), ptr_(ptr), arg1_(arg1), arg2_(arg2) {}

 private:
  CallbackFunc callback_func_;

  WeakPtr<Class> ptr_;

  Arg1 arg1_;

  Arg2 arg2_;
};

template <class Class>
Clouse* Bind(void (Class::*CallbackFunc)(), WeakPtr<Class>& ptr) {
  return lynx_new Callback0<Class>(CallbackFunc, ptr);
}

template <class Class, class FuncArg1, class Arg1>
Clouse* Bind(void (Class::*CallbackFunc)(FuncArg1 arg1),
             WeakPtr<Class>& ptr,
             Arg1& arg1) {
  return lynx_new Callback1<Class, FuncArg1, Arg1>(CallbackFunc, ptr, arg1);
}

template <class Class, class FuncArg1, class FuncArg2, class Arg1, class Arg2>
Clouse* Bind(void (Class::*CallbackFunc)(FuncArg1 arg1, FuncArg2 arg2),
             WeakPtr<Class>& ptr,
             Arg1& arg1,
             Arg2& arg2) {
  return lynx_new Callback2<Class, FuncArg1, FuncArg2, Arg1, Arg2>(
      CallbackFunc, ptr, arg1, arg2);
}

#else

template <size_t...>
struct IndexSequence {};

template <size_t... Ns>
struct MakeIndexSequenceImpl;

template <size_t... Ns>
struct MakeIndexSequenceImpl<0, Ns...> {
  using Type = IndexSequence<Ns...>;
};

template <size_t N, size_t... Ns>
struct MakeIndexSequenceImpl<N, Ns...>
    : MakeIndexSequenceImpl<N - 1, N - 1, Ns...> {};

template <size_t N>
using MakeIndexSequence = typename MakeIndexSequenceImpl<N>::Type;

template <typename Pointer>
struct IsWeakRef : public std::false_type {};

template <typename T>
struct IsWeakRef<WeakPtr<T> > : public std::true_type {};

template <bool IsWeakCall, typename P, typename F, typename T>
struct InvokeHelper;

template <typename P, typename F, typename T>
struct InvokeHelper<true, P, F, T> {
  template <typename std::size_t... I>
  static void Invoke(P p, F f, const T& t, IndexSequence<I...>) {
    if (p.IsValid()) {
      (p.Get()->*f)(std::get<I>(t)...);
    }
  }
};

template <typename P, typename F, typename T>
struct InvokeHelper<false, P, F, T> {
  template <typename std::size_t... I>
  static void Invoke(P& p, F f, const T& t, IndexSequence<I...>) {
    (p.Get()->*f)(std::get<I>(t)...);
  }
};

template <typename Functor, typename Pointer, typename... Args>
class Callback : public Clouse {
 public:
  using BoundIndices = MakeIndexSequence<sizeof...(Args)>;

  using PointerType = typename std::decay<Pointer>::type;

  using StorageType = typename std::tuple<Args...>;

  using IsWeak = IsWeakRef<Pointer>;

  Callback(Functor f, Pointer p, Args&... args)
      : f_(f), p_(p), args_storage_(args...) {}

  void Run() {
    InvokeHelper<IsWeak::value, Pointer, Functor, StorageType>::Invoke(
        p_, f_, args_storage_, BoundIndices());
  }

 private:
  Functor f_;
  Pointer p_;
  StorageType args_storage_;
};

template <typename Functor, typename Pointer, typename... Args>
Clouse* Bind(Functor f, Pointer p, Args&... args) {
  return lynx_new Callback<Functor, Pointer, Args...>(f, p, args...);
}
#endif

class CompletionCallbackBase {
 public:
  virtual void Run(int arg) {}
  virtual bool IsNull() { return false; }
};

template <typename Type>
class CompletionCallback : public CompletionCallbackBase {
 public:
  typedef void (Type::*CallbackFunc)(int);

  CompletionCallback() : callback_func_(nullptr), ptr_(nullptr) {}
  CompletionCallback(CallbackFunc func, Type* ptr)
      : callback_func_(func), ptr_(ptr) {}

  virtual void Run(int arg) { (ptr_->*callback_func_)(arg); }

  virtual bool IsNull() { return ptr_ != nullptr; }

 private:
  CallbackFunc callback_func_;

  Type* ptr_;

  base::ScopedPtr<Clouse> callback_;
};
}  // namespace base

#endif  // LYNX_BASE_TASK_CALLBACK_H_

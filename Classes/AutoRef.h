#pragma once

// 自动处理cocos2d-x里面Ref的retain和release
template <typename T>
class AutoRef {
 public:
  AutoRef();
  AutoRef(const AutoRef&);
  AutoRef(AutoRef&&);
  AutoRef(T*);
  ~AutoRef();
  // 赋值
  AutoRef& operator=(const AutoRef&);
  AutoRef& operator=(AutoRef&&);
  AutoRef& operator=(T*);
  operator T*() const;
  T* data() const;
  // 操作
  T* operator->() { return _ref; }
  T operator*() { return *_ref; }
 private:
  T* _ref;
};

template <typename T>
AutoRef<T>::AutoRef() : _ref(nullptr) {}

template <typename T>
AutoRef<T>::AutoRef(const AutoRef<T>& other) : AutoRef(other._ref) {}

template <typename T>
AutoRef<T>::AutoRef(AutoRef<T>&& other) : _ref(other._ref) {
  other._ref = nullptr;
}

template <typename T>
AutoRef<T>::AutoRef(T* ptr) : _ref(ptr) {
  _ref->retain();
}

template <typename T>
AutoRef<T>::~AutoRef() {
  if (_ref != nullptr) {
    _ref->release();
  }
}

template <typename T>
AutoRef<T>::operator T*() const {
  return _ref;
}

template <typename T>
T* AutoRef<T>::data() const {
  return ref;
}

template <typename T>
AutoRef<T>& AutoRef<T>::operator=(const AutoRef<T>& other) {
  if (_ref != nullptr) {
    ref->release();
  }
  _ref = other._ref;
  _ref->retain();
}

template <typename T>
AutoRef<T>& AutoRef<T>::operator=(AutoRef<T>&& other) {
  if (_ref != nullptr) {
    _ref->release();
  }
  _ref = other._ref;
  other._ref = nullptr;
  return *this;
}

template <typename T>
AutoRef<T>& AutoRef<T>::operator=(T* other) {
  return *this = AutoRef<T>(other);
}

// RUN: clang-cc -fsyntax-only -verify %s

template<typename T> inline void f(T) { }
template void f(int); // expected-note{{previous explicit instantiation}}
template void f(int); // expected-error{{duplicate explicit instantiation}}

template<typename T>
struct X0 {
  union Inner { };
  
  void f(T) { }
  
  static T value;
};

template<typename T>
T X0<T>::value = 3.14;

template struct X0<int>; // expected-note{{previous explicit instantiation}}
template struct X0<int>; // expected-error{{duplicate explicit instantiation}}

template void X0<float>::f(float); // expected-note{{previous explicit instantiation}}
template void X0<float>::f(float); // expected-error{{duplicate explicit instantiation}}

template union X0<float>::Inner; // expected-note{{previous explicit instantiation}}
template union X0<float>::Inner; // expected-error{{duplicate explicit instantiation}}

template float X0<float>::value; // expected-note{{previous explicit instantiation}}
template float X0<float>::value; // expected-error{{duplicate explicit instantiation}}

// RUN: clang-cc -fsyntax-only -verify %s
template<typename T, T Divisor>
class X {
public:
  static const T value = 10 / Divisor; // expected-error{{in-class initializer is not an integral constant expression}}
};

int array1[X<int, 2>::value == 5? 1 : -1];
X<int, 0> xi0; // expected-note{{in instantiation of template class 'class X<int, 0>' requested here}}


template<typename T>
class Y {
  static const T value = 0; // expected-error{{'value' can only be initialized if it is a static const integral data member}}
};

Y<float> fy; // expected-note{{in instantiation of template class 'class Y<float>' requested here}}


// out-of-line static member variables

template<typename T>
struct Z {
  static T value;
};

template<typename T>
T Z<T>::value; // expected-error{{no matching constructor}}

struct DefCon {};

struct NoDefCon { 
  NoDefCon(const NoDefCon&);
};

void test() {
  DefCon &DC = Z<DefCon>::value;
  NoDefCon &NDC = Z<NoDefCon>::value; // expected-note{{instantiation}}
}

// PR5609
struct X1 {
  ~X1();  // The errors won't be triggered without this dtor.
};

template <typename T>
struct Y1 {
  static char Helper(T);
  static const int value = sizeof(Helper(T()));
};

struct X2 {
  virtual ~X2();
};

namespace std {
  class type_info { };
}

template <typename T>
struct Y2 {
  static T &Helper();
  static const int value = sizeof(typeid(Helper()));
};

template <int>
struct Z1 {};

void Test() {
  Z1<Y1<X1>::value> x;
  int y[Y1<X1>::value];
  Z1<Y2<X2>::value> x2;
  int y2[Y2<X2>::value];
}

// PR5672
template <int n>
struct X3 {};

class Y3 {
 public:
  ~Y3();  // The error isn't triggered without this dtor.

  void Foo(X3<1>);
};

template <typename T>
struct SizeOf {
  static const int value = sizeof(T);
};

void MyTest3() {
   Y3().Foo(X3<SizeOf<char>::value>());
}
// Output file should have no calls to error() with folding.
// RUN: clang-cc -triple i386-unknown-unknown -O3 -emit-llvm -o %t %s
// RUN: FileCheck %s < %t

static unsigned pow(unsigned Base, unsigned Power) {
  unsigned Val = 1;
  while (Power--)
    Val *= Base;
  return Val;
}

struct TempTracker {
  unsigned Product, Index;

  TempTracker() : Product(1), Index(0) {}

};

// FIXME: This can be used to check elision as well, if P = 0 hacks are removed.
struct A {
  TempTracker &TT;
  mutable unsigned P;
  bool Truth;

  A(TempTracker &_TT, unsigned _P, bool _Truth = true)
    : TT(_TT), P(_P), Truth(_Truth) {}
  A(const A &RHS) : TT(RHS.TT), P(RHS.P), Truth(RHS.Truth) { RHS.P = 0; }
  ~A() {
    if (P)
      TT.Product *= pow(P, ++TT.Index);
  }

  A &operator=(const A &RHS) {
    TT = RHS.TT;
    P = RHS.P;
    Truth = RHS.Truth;
    RHS.P = 0;
    return *this;
  }

  operator bool () { return Truth; }
};

// 3, 7, 2
static unsigned f0(bool val = false) {
  TempTracker tt;
  {
    A a(tt, 2);
    if ((A(tt, 3), val))
      A b(tt, 5);
    A c(tt, 7);
  }
  return tt.Product;
}

// 3, 5, 7, 2
static unsigned f1(bool val = true) {
  TempTracker tt;
  {
    A a(tt, 2);
    if ((A(tt, 3), val))
      A b(tt, 5);
    A c(tt, 7);
  }
  return tt.Product;
}

// 5, 3, 7, 2
static unsigned f2() {
  TempTracker tt;
  {
    A a(tt, 2);
    if (A b = A(tt, 3))
      A c(tt, 5);
    A d(tt, 7);
  }
  return tt.Product;
}

// 7, 3, 11, 2
static unsigned f3() {
  TempTracker tt;
  {
    A a(tt, 2);
    if (A b = A(tt, 3, false))
      A c(tt, 5);
    else
      A c(tt, 7);
    A d(tt, 11);
  }
  return tt.Product;
}

// 3, 7, 2
static unsigned f4() {
  TempTracker tt;
  {
    A a(tt, 2);
    while (A b = A(tt, 3, false))
      A c(tt, 5);
    A c(tt, 7);
  }
  return tt.Product;
}

// 5, 3, 7, 2
static unsigned f5() {
  TempTracker tt;
  {
    A a(tt, 2);
    while (A b = A(tt, 3, true)) {
      A c(tt, 5);
      break;
    }
    A c(tt, 7);
  }
  return tt.Product;
}

// 3, 7, 11, 5, 13, 2
static unsigned f6() {
  TempTracker tt;
  {
    A a(tt, 2);
    for (A b = (A(tt, 3), A(tt, 5)), c = (A(tt, 7), A(tt, 11));;)
      break;
    A c(tt, 13);
  }
  return tt.Product;
}

extern "C" void error();
extern "C" void print(const char *Name, unsigned N);

#define ORDER3(a, b, c) (pow(a, 1) * pow(b, 2) * pow(c, 3))
#define ORDER4(a, b, c, d) (ORDER3(a, b, c) * pow(d, 4))
#define ORDER5(a, b, c, d, e) (ORDER4(a, b, c, d) * pow(e, 5))
#define ORDER6(a, b, c, d, e, f) (ORDER5(a, b, c, d, e) * pow(f, 6))
void test() {
// CHECK: call void @print(i8* {{.*}}, i32 1176)
  print("f0", f0());
  if (f0() != ORDER3(3, 7, 2))
    error();

// CHECK: call void @print(i8* {{.*}}, i32 411600)
  print("f1", f1());
  if (f1() != ORDER4(3, 5, 7, 2))
    error();

// CHECK: call void @print(i8* {{.*}}, i32 246960)
  print("f2", f2());
  if (f2() != ORDER4(5, 3, 7, 2))
    error();

// CHECK: call void @print(i8* {{.*}}, i32 1341648)
  print("f3", f3());
  if (f3() != ORDER4(7, 3, 11, 2))
    error();

// CHECK: call void @print(i8* {{.*}}, i32 1176)
  print("f4", f4());
  if (f4() != ORDER3(3, 7, 2))
    error();

// CHECK: call void @print(i8* {{.*}}, i32 246960)
  print("f5", f5());
  if (f5() != ORDER4(5, 3, 7, 2))
    error();

// FIXME: Clang/LLVM currently can't fold this to a constant. If the error check
// is present (since it avoids single-caller inlining). PR5645.

//  CHECK: call void @print(i8* {{.*}}, i32 1251552576)
  print("f6", f6());
//  if (f6() != ORDER6(3, 7, 11, 5, 13, 2))
//    error();
}



#ifdef HARNESS

#include <cstdlib>
#include <cstdio>

extern "C" void error() {
  abort();
}

extern "C" void print(const char *name, unsigned N) {
  printf("%s: %d\n", name, N);
}

int main() {
  test();
  return 0;
}

#endif

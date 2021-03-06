// RUN: clang-cc -fsyntax-only -verify %s

struct A { 
  virtual void f(int a = 7);
}; 

struct B : public A {
  void f(int a);
}; 

void m() {
  B* pb = new B; 
  A* pa = pb; 
  pa->f(); // OK, calls pa->B::f(7) 
  pb->f(); // expected-error{{too few arguments}}
}

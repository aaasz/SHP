// RUN: clang-cc -fsyntax-only -verify %s 
struct A {
};

struct ConvertibleToA {
  operator A();
};

struct ConvertibleToConstA {
  operator const A();
};

struct B {
  B& operator=(B&);  // expected-note 4 {{candidate function}}
};

struct ConvertibleToB {
  operator B();
};

struct ConvertibleToBref {
  operator B&();
};

struct ConvertibleToConstB {
  operator const B();
};

struct ConvertibleToConstBref {
  operator const B&();
};

struct C {
  int operator=(int); // expected-note{{candidate function}}
  long operator=(long); // expected-note{{candidate function}}
  int operator+=(int); // expected-note{{candidate function}}
  int operator+=(long); // expected-note{{candidate function}}
};

struct D {
  D& operator+=(const D &);
};

struct ConvertibleToInt {
  operator int();
};

void test() {
  A a, na;
  const A constA;
  ConvertibleToA convertibleToA;
  ConvertibleToConstA convertibleToConstA;

  B b, nb;
  const B constB;
  ConvertibleToB convertibleToB;
  ConvertibleToBref convertibleToBref;
  ConvertibleToConstB convertibleToConstB;
  ConvertibleToConstBref convertibleToConstBref;

  C c, nc;
  const C constC;

  D d, nd;
  const D constD;

  ConvertibleToInt convertibleToInt;

  na = a;
  na = constA;
  na = convertibleToA;
  na = convertibleToConstA;
  na += a; // expected-error{{no viable overloaded '+='}}

  nb = b;
  nb = constB;  // expected-error{{no viable overloaded '='}}
  nb = convertibleToB; // expected-error{{no viable overloaded '='}}
  nb = convertibleToBref;
  nb = convertibleToConstB; // expected-error{{no viable overloaded '='}}
  nb = convertibleToConstBref; // expected-error{{no viable overloaded '='}}

  nc = c;
  nc = constC;
  nc = 1;
  nc = 1L;
  nc = 1.0; // expected-error{{use of overloaded operator '=' is ambiguous}}
  nc += 1;
  nc += 1L;
  nc += 1.0; // expected-error{{use of overloaded operator '+=' is ambiguous}}

  nd = d;
  nd += d;
  nd += constD;

  int i;
  i = convertibleToInt;
  i = a; // expected-error{{incompatible type assigning 'struct A', expected 'int'}}
}


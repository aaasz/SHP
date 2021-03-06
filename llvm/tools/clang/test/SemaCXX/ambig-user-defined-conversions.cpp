// RUN: clang-cc -fsyntax-only -verify %s

// Test1
struct BASE { 
  operator int &(); // expected-note {{candidate function}}
}; 
struct BASE1 { 
  operator int &(); // expected-note {{candidate function}}
}; 

struct B : public BASE, BASE1 { 

}; 

extern B f(); 

B b1;
void func(const int ci, const char cc); // expected-note {{candidate function}}
void func(const char ci, const B b); // expected-note {{candidate function}}
void func(const B b, const int ci); // expected-note {{candidate function}}

const int Test1() {
  func(b1, f()); // expected-error {{call to 'func' is ambiguous}}
  return f(); // expected-error {{conversion from 'struct B' to 'int const' is ambiguous}}
}


// Test2
struct E;
struct A { 
  A (E&); 
};

struct E { 
  operator A (); 
};

struct C { 
  C (E&);  
};

void f1(A);	// expected-note {{candidate function}}
void f1(C);	// expected-note {{candidate function}}

void Test2()
{
	E b;
	f1(b);  // expected-error {{call to 'f1' is ambiguous}}	
                // ambiguous because b -> C via constructor and
                // b → A via constructor or conversion function.
}


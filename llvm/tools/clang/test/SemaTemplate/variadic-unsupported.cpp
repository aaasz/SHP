// RUN: clang-cc -fsyntax-only -verify %s

// Type parameter packs.
template <typename ... > struct T1 {}; // expected-error{{variadic templates are only allowed in C++0x}}


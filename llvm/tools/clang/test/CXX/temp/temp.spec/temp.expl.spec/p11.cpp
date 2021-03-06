// RUN: clang-cc -fsyntax-only -verify %s

template<class T> class Array { /* ... */ }; 
template<class T> void sort(Array<T>& v);

// explicit specialization for sort(Array<int>&) 
// with deduced template-argument of type int 
template<> void sort(Array<int>&);

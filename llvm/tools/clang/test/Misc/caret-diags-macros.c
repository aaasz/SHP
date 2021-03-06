// RUN: clang-cc -fsyntax-only %s > %t 2>&1

#define M1(x) x

// RUN: grep ":6:12: note: instantiated from:" %t
#define M2 1;

void foo() {
 // RUN: grep ":10:2: warning: expression result unused" %t
 M1(
 // RUN: grep ":12:5: note: instantiated from:" %t
    M2)
}

// RUN: grep ":16:11: note: instantiated from:" %t
#define A 1
// RUN: grep ":18:11: note: instantiated from:" %t
#define B A
// RUN: grep ":20:11: note: instantiated from:" %t
#define C B

void bar() {
  // RUN: grep  ":24:3: warning: expression result unused" %t
  C;
}


// RUN: not clang-cc -emit-llvm -o %S/doesnotexist/somename %s 2> %t
// RUN: FileCheck -check-prefix=OUTPUTFAIL -input-file=%t %s

// OUTPUTFAIL: Error opening output file '{{.*}}doesnotexist{{.*}}'

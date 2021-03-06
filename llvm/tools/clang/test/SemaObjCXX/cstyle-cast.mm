// RUN: clang -cc1 -fsyntax-only -verify %s

@protocol P @end
@interface I @end

struct X { X(); };

void test1(X x) {
  void *cft;
  id oct = (id)cft;

  Class ccct;
  ccct = (Class)cft;

  I* iict = (I*)cft;

  id<P> qid = (id<P>)cft;

  I<P> *ip = (I<P>*)cft;

  (id)x; // expected-error {{C-style cast from 'struct X' to 'id' is not allowed}}

  id *pid = (id*)ccct;

  id<P> *qpid = (id<P>*)ccct;

  int **pii;

  ccct = (Class)pii;

  qpid = (id<P>*)pii;

  iict = (I*)pii;

  pii = (int **)ccct;

  pii = (int **)qpid;
  
}


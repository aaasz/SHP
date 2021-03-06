// RUN: clang -cc1 -rewrite-objc %s -o -

typedef struct S {
	int * pint;
	int size;
}NSRec;

@interface SUPER
- (NSRec) MainMethod : (NSRec) Arg1 : (NSRec) Arg2;
@end

@interface MyDerived : SUPER
{
	NSRec d;
}
- (int) instanceMethod;
- (int) another : (int) arg;
- (NSRec) MainMethod : (NSRec) Arg1 : (NSRec) Arg2;
@end

@implementation MyDerived 
- (int) instanceMethod {
  return [self another : [self MainMethod : d : d].size];
}

- (int) another : (int) arg { return arg; }
- (NSRec) MainMethod : (NSRec) Arg1 : (NSRec) Arg2 { return Arg2; }
@end


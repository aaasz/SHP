// RUN: clang -cc1 -analyze -analyzer-experimental-internal-checks -checker-cfref -analyzer-constraints=basic -analyzer-store=basic %s -verify
// RUN: clang -cc1 -analyze -analyzer-experimental-internal-checks -checker-cfref -analyzer-constraints=basic -analyzer-store=region %s -verify

typedef struct Foo { int x; } Bar;

@interface MyClass {}
- (Bar)foo;
@end
@implementation MyClass
- (Bar)foo { 
  struct Foo f = { 0 };
  return f;
}
@end

void createFoo() {
  MyClass *obj = 0;  
  Bar f = [obj foo]; // expected-warning{{The receiver of message 'foo' is nil and returns a value of type 'Bar' that will be garbage}}
}

void createFoo2() {
  MyClass *obj = 0;  
  [obj foo]; // no-warning
  Bar f = [obj foo]; // expected-warning{{The receiver of message 'foo' is nil and returns a value of type 'Bar' that will be garbage}}
}


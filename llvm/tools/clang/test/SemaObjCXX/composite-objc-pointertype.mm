// RUN: clang -cc1 -fsyntax-only -verify %s

@interface Foo
@end

@implementation Foo
- (id)test {
        id bar;
    Class cl;
    Foo *f;

    (void)((bar!= 0) ? bar : 0);
    (void)((cl != 0) ? cl : 0);
    (void)((f != 0) ? 0 : f);
    return (0 == 1) ? 0 : bar;
}
@end


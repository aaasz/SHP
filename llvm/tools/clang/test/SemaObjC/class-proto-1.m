// RUN: clang -cc1 -fsyntax-only -verify %s

@interface INTF1 @end

@protocol p1,p2,p3;

@protocol p1;

@protocol PROTO1
- (INTF1<p1>*) meth;
@end

@protocol p1 @end

@interface I1 <p1> @end

@interface E1 <p2> @end	// expected-warning {{cannot find protocol definition for 'p2'}}

@protocol p2 @end


@interface I2 <p1,p2> @end

@interface E2 <p1,p2,p3> @end  // expected-warning {{cannot find protocol definition for 'p3'}}

@class U1, U2;

@interface E3 : U1 @end // expected-error {{cannot find interface declaration for 'U1', superclass of 'E3'}}


@interface I3 : E3  @end

@interface U2 @end

@interface I4 : U2 <p1,p2>
@end

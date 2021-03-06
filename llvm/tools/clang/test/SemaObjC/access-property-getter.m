// RUN: clang -cc1 -verify %s

@protocol NSObject
- (oneway void)release;
@end

@protocol XCOutputStreams <NSObject>
@end


@interface XCWorkQueueCommandInvocation 
{
    id <XCOutputStreams> _outputStream;
}
@end

@interface XCWorkQueueCommandSubprocessInvocation : XCWorkQueueCommandInvocation
@end

@interface XCWorkQueueCommandLocalSubprocessInvocation : XCWorkQueueCommandSubprocessInvocation
@end

@interface XCWorkQueueCommandDistributedSubprocessInvocation : XCWorkQueueCommandSubprocessInvocation
@end

@interface XCWorkQueueCommandCacheFetchInvocation : XCWorkQueueCommandSubprocessInvocation

@end

@implementation XCWorkQueueCommandCacheFetchInvocation
- (id)harvestPredictivelyProcessedOutputFiles
{
     _outputStream.release;
     return 0;
}
@end

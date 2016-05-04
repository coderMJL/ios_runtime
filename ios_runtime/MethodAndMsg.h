typedef struct objc_selector *SEL;
#pragma mark - 实例1
SEL sel1 = @selector(method1);
NSLog(@"sel : %p", sel1);
- (void)setWidth:(int)width;
- (void)setWidth:(double)width;
- (void)setWidthIntValue:(int)width;
- (void)setWidthDoubleValue:(double)width;
id (*IMP)(id, SEL, ...) typedef struct objc_method *Method;
struct objc_method
{
    SEL method_name OBJC2_UNAVAILABLE; // 方法名
    char *method_types OBJC2_UNAVAILABLE;
    IMP method_imp OBJC2_UNAVAILABLE; // 方法实现
} struct objc_method_description
{
    SEL name;
    char *types;
};
// 调用指定方法的实现
id method_invoke(id receiver, Method m, ...);
// 调用返回一个数据结构的方法的实现
void method_invoke_stret(id receiver, Method m, ...);
// 获取方法名
SEL method_getName(Method m);
// 返回方法的实现
IMP method_getImplementation(Method m);
// 获取描述方法参数和返回值类型的字符串
const char *method_getTypeEncoding(Method m);
// 获取方法的返回值类型的字符串
char *method_copyReturnType(Method m);
// 获取方法的指定位置参数的类型字符串
char *method_copyArgumentType(Method m, unsigned int index);
// 通过引用返回方法的返回值类型字符串
void method_getReturnType(Method m, char *dst, size_t dst_len);
// 返回方法的参数的个数
unsigned int method_getNumberOfArguments(Method m);
// 通过引用返回方法指定位置参数的类型字符串
void method_getArgumentType(Method m, unsigned int index, char *dst, size_t dst_len);
// 返回指定方法的方法描述结构体
struct objc_method_description *method_getDescription(Method m);
// 设置方法的实现
IMP method_setImplementation(Method m, IMP imp);
// 交换两个方法的实现
void method_exchangeImplementations(Method m1, Method m2);
// 返回给定选择器指定的方法的名称
const char *sel_getName(SEL sel);
// 在Objective-C Runtime系统中注册一个方法，将方法名映射到一个选择器，并返回这个选择器
SEL sel_registerName(const char *str);
// 在Objective-C Runtime系统中注册一个方法
SEL sel_getUid(const char *str);
// 比较两个选择器
BOOL sel_isEqual(SEL lhs, SEL rhs);
objc_msgSend(receiver, selector) objc_msgSend(receiver, selector, arg1, arg2, ...)
#pragma mark - 实例2
    - strange
{
    id target = getTheReceiver();
    SEL method = getTheMethod();
    if (target == self || method == _cmd)
        return nil;
    return [target performSelector:method];
}
void (*setter)(id, SEL, BOOL);
int i;
setter = (void (*)(id, SEL, BOOL))[target methodForSelector:@selector(setFilled:)];
for (i = 0; i < 1000; i++)
    setter(targetList[i], @selector(setFilled:), YES);
if ([self respondsToSelector:@selector(method)]) {
    [self performSelector:@selector(method)];
}
- [SUTRuntimeMethod method]:unrecognized selector sent to instance 0x100111940
    * **Terminating app due to uncaught exception 'NSInvalidArgumentException',
reason:'-[SUTRuntimeMethod method]: unrecognized selector sent to instance 0x100111940'
#pragma mark - 实例3
    void
    functionForMethod1(id self, SEL _cmd)
{
    NSLog(@"%@, %p", self, _cmd);
}
+ (BOOL)resolveInstanceMethod:(SEL)sel
{
    NSString *selectorString = NSStringFromSelector(sel);
    if ([selectorString isEqualToString:@"method1"]) {
        class_addMethod(self.class, @selector(method1), (IMP)functionForMethod1, "@:");
    }
    return [super resolveInstanceMethod:sel];
}
- (id)forwardingTargetForSelector:(SEL)aSelector
#pragma mark - 实例4
@interface SUTRuntimeMethodHelper : NSObject



- (void)method2;
@end @implementation SUTRuntimeMethodHelper - (void)method2
{
    NSLog(@"%@, %p", self, _cmd);
}
@end
#pragma mark -



@interface SUTRuntimeMethod ()
{
    SUTRuntimeMethodHelper *_helper;
}
@end @implementation SUTRuntimeMethod + (instancetype)object
{
    return [[self alloc] init];
}
- (instancetype)init
{
    self = [super init];
    if (self != nil) {
        _helper = [[SUTRuntimeMethodHelper alloc] init];
    }
    return self;
}
- (void)test
{
    [self performSelector:@selector(method2)];
}
- (id)forwardingTargetForSelector:(SEL)aSelector
{
    NSLog(@"forwardingTargetForSelector");
    NSString *selectorString = NSStringFromSelector(aSelector);
    // 将消息转发给_helper来处理
    if ([selectorString isEqualToString:@"method2"]) {
        return _helper;
    }
    return [super forwardingTargetForSelector:aSelector];
}
@end - (void)forwardInvocation : (NSInvocation *)anInvocation - (NSMethodSignature *)methodSignatureForSelector : (SEL)aSelector
#pragma mark - 实例5
                                                                                                                  - (NSMethodSignature *)methodSignatureForSelector : (SEL)aSelector
{
    NSMethodSignature *signature = [super methodSignatureForSelector:aSelector];
    if (!signature) {
        if ([SUTRuntimeMethodHelper instancesRespondToSelector:aSelector]) {
            signature = [SUTRuntimeMethodHelper instanceMethodSignatureForSelector:aSelector];
        }
    }
    return signature;
}
- (void)forwardInvocation:(NSInvocation *)anInvocation
{
    if ([SUTRuntimeMethodHelper instancesRespondToSelector:anInvocation.selector]) {
        [anInvocation invokeWithTarget:_helper];
    }
}
- (BOOL)respondsToSelector:(SEL)aSelector
{
    if ([super respondsToSelector:aSelector])
        return YES;
    else {
        /* Here, test whether the aSelector message can     *

         * be forwarded to another object and whether that  *

         * object can respond to it. Return YES if it can.  */
    }
    return NO;
}

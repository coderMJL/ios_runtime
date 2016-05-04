#pragma mark - 实例1
@interface MyViewController : UIViewController
@end
@implementation MyViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    // do something
    ...
}
@end
struct objc_super
{
    id receiver;
    Class superClass;
};
id objc_msgSendSuper(struct objc_super *super, SEL op, ...);
objc_msgSend(objc_super->receiver, @selector(viewDidLoad)) objc_msgSend(self, @selector(viewDidLoad)) @interface MyClass : NSObject @end @implementation MyClass - (void)test
{
    NSLog(@"self class: %@", self.class);
    NSLog(@"super class: %@", super.class);
}
@end
    // 获取所有加载的Objective-C框架和动态库的名称
    const char **
    objc_copyImageNames(unsigned int *outCount);
// 获取指定类所在动态库
const char *class_getImageName(Class cls);
// 获取指定库或框架中所有类的类名
const char **objc_copyClassNamesForImage(const char *image, unsigned int *outCount);
#pragma mark - 实例2
NSLog(@"获取指定类所在动态库");
NSLog(@"UIView's Framework: %s", class_getImageName(NSClassFromString(@"UIView")));
NSLog(@"获取指定库或框架中所有类的类名");
const char **classes = objc_copyClassNamesForImage(class_getImageName(NSClassFromString(@"UIView")), &outCount);
for (int i = 0; i < outCount; i++) {
    NSLog(@"class name: %s", classes[i]);
}
// 创建一个指针函数的指针，该函数调用时会调用特定的block
IMP imp_implementationWithBlock(id block);
// 返回与IMP(使用imp_implementationWithBlock创建的)相关的block
id imp_getBlock(IMP anImp);
// 解除block与IMP(使用imp_implementationWithBlock创建的)的关联关系，并释放block的拷贝
BOOL imp_removeBlock(IMP anImp);
#pragma mark - 实例3
@interface MyRuntimeBlock : NSObject
@end
@implementation MyRuntimeBlock
@end
// 测试代码
IMP imp = imp_implementationWithBlock(^(id obj, NSString *str) {
    NSLog(@"%@", str);
});
class_addMethod(MyRuntimeBlock.class, @selector(testBlock:), imp, "v@:@");
MyRuntimeBlock *runtime = [[MyRuntimeBlock alloc] init];
[runtime performSelector:@selector(testBlock:) withObject:@"hello world!"];
// 加载弱引用指针引用的对象并返回
id objc_loadWeak(id *location);
// 存储__weak变量的新值
id objc_storeWeak(id *location, id obj);
#define YES (BOOL)1
#define NO (BOOL)0
#define nil __DARWIN_NULL
#define Nil __DARWIN_NULL
#define OBJC_OLD_DISPATCH_PROTOTYPES 1
#define OBJC_ROOT_CLASS
__OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
OBJC_ROOT_CLASS
OBJC_EXPORT
@interface NSObject <NSObject> {
    Class isa OBJC_ISA_AVAILABILITY;
}
#define NS_VALID_UNTIL_END_OF_SCOPE
enum
{
    OBJC_ASSOCIATION_ASSIGN = 0,
    OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1,
    OBJC_ASSOCIATION_COPY_NONATOMIC = 3,
    OBJC_ASSOCIATION_RETAIN = 01401,
    OBJC_ASSOCIATION_COPY = 01403
};

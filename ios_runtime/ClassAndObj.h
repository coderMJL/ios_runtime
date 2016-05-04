
struct objc_class
{
    Class isa OBJC_ISA_AVAILABILITY;
#if !__OBJC2__
    Class super_class OBJC2_UNAVAILABLE; // 父类
    const char *name OBJC2_UNAVAILABLE; // 类名
    long version OBJC2_UNAVAILABLE; // 类的版本信息，默认为0
    long info OBJC2_UNAVAILABLE; // 类信息，供运行期使用的一些位标识
    long instance_size OBJC2_UNAVAILABLE; // 该类的实例变量大小
    struct objc_ivar_list *ivars OBJC2_UNAVAILABLE; // 该类的成员变量链表
    struct objc_method_list **methodLists OBJC2_UNAVAILABLE; // 方法定义的链表
    struct objc_cache *cache OBJC2_UNAVAILABLE; // 方法缓存
    struct objc_protocol_list *protocols OBJC2_UNAVAILABLE; // 协议链表
#endif
} OBJC2_UNAVAILABLE;
typedef struct objc_class *Class;
struct objc_object
{
    Class isa OBJC_ISA_AVAILABILITY;
};
typedef struct objc_object *id;
struct objc_cache
{
    unsigned int mask /* total = mask + 1 */ OBJC2_UNAVAILABLE;
    unsigned int occupied OBJC2_UNAVAILABLE;
    Method buckets[1] OBJC2_UNAVAILABLE;
};
#pragma mark -  实例1
void TestMetaClass(id self, SEL _cmd)
{
    NSLog(@"This objcet is %p", self);
    NSLog(@"Class is %@, super class is %@", [self class], [self superclass]);
    Class currentClass = [self class];
    for (int i = 0; i < 4; i++) {
        NSLog(@"Following the isa pointer %d times gives %p", i, currentClass);
        currentClass = objc_getClass((__bridge void *)currentClass);
    }
    NSLog(@"NSObject's class is %p", [NSObject class]);
    NSLog(@"NSObject's meta class is %p", objc_getClass((__bridge void *)[NSObject class]));
}
#pragma mark -
@implementation Test
- (void)ex_registerClassPair
{
    Class newClass = objc_allocateClassPair([NSError class], "TestClass", 0);
    class_addMethod(newClass, @selector(testMetaClass), (IMP)TestMetaClass, "v@:");
    objc_registerClassPair(newClass);
    id instance = [[newClass alloc] initWithDomain:@"some domain" code:0 userInfo:nil];
    [instance performSelector:@selector(testMetaClass)];
}
@end
// 获取类的类名
const char *class_getName(Class cls);
// 获取类的父类
Class class_getSuperclass(Class cls);
// 判断给定的Class是否是一个元类
BOOL class_isMetaClass(Class cls);
// 获取实例大小
size_t class_getInstanceSize(Class cls);
// 获取类中指定名称实例成员变量的信息
Ivar class_getInstanceVariable(Class cls, const char *name);
// 获取类成员变量的信息
Ivar class_getClassVariable(Class cls, const char *name);
// 添加成员变量
BOOL class_addIvar(Class cls, const char *name, size_t size, uint8_t alignment, const char *types);
// 获取整个成员变量列表
Ivar *class_copyIvarList(Class cls, unsigned int *outCount);
// 获取指定的属性
objc_property_t class_getProperty(Class cls, const char *name);
// 获取属性列表
objc_property_t *class_copyPropertyList(Class cls, unsigned int *outCount);
// 为类添加属性
BOOL class_addProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount);
// 替换类的属性
void class_replaceProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount);
//在MAC OS X系统中，我们可以使用垃圾回收器。runtime提供了几个函数来确定一个对象的内存区域是否可以被垃圾回收器扫描，以处理strong/weak引用。这几个函数定义如下
const uint8_t *class_getIvarLayout(Class cls);
void class_setIvarLayout(Class cls, const uint8_t *layout);
const uint8_t *class_getWeakIvarLayout(Class cls);
void class_setWeakIvarLayout(Class cls, const uint8_t *layout);
// 添加方法
BOOL class_addMethod(Class cls, SEL name, IMP imp, const char *types);
// 获取实例方法
Method class_getInstanceMethod(Class cls, SEL name);
// 获取类方法
Method class_getClassMethod(Class cls, SEL name);
// 获取所有方法的数组
Method *class_copyMethodList(Class cls, unsigned int *outCount);
// 替代方法的实现
IMP class_replaceMethod(Class cls, SEL name, IMP imp, const char *types);
// 返回方法的具体实现
IMP class_getMethodImplementation(Class cls, SEL name);
IMP class_getMethodImplementation_stret(Class cls, SEL name);
// 类实例是否响应指定的selector
BOOL class_respondsToSelector(Class cls, SEL sel);
// class_addMethod的实现会覆盖父类的方法实现，但不会取代本类中已存在的实现，如果本类中包含一个同名的实现，则函数会返回NO。如果要修改已存在实现，可以使用method_setImplementation。一个Objective-C方法是一个简单的C函数，它至少包含两个参数—self和_cmd。所以，我们的实现函数(IMP参数指向的函数)至少需要两个参数，如下所示：
void myMethodIMP(id self, SEL _cmd)
{
    // implementation ....
}
// 添加协议
BOOL class_addProtocol(Class cls, Protocol *protocol);
// 返回类是否实现指定的协议
BOOL class_conformsToProtocol(Class cls, Protocol *protocol);
// 返回类实现的协议列表
Protocol *class_copyProtocolList(Class cls, unsigned int *outCount);
// 获取版本号
int class_getVersion(Class cls);
// 设置版本号
void class_setVersion(Class cls, int version);
// runtime还提供了两个函数来供CoreFoundation的tool-free bridging使用，即：
Class objc_getFutureClass(const char *name);
void objc_setFutureClass(Class cls, const char *name);
#pragma mark - 实例2
//-----------------------------------------------------------
// MyClass.h
@interface MyClass : NSObject <NSCopying, NSCoding>
@property (nonatomic, strong) NSArray *array;
@property (nonatomic, copy) NSString *string;
- (void)method1;
- (void)method2;
+ (void)classMethod1;
@end
//-----------------------------------------------------------
// MyClass.m
#import "MyClass.h"
@interface MyClass () {
    NSInteger _instance1;
    NSString *_instance2;
}
@property (nonatomic, assign) NSUInteger integer;
- (void)method3WithArg1:(NSInteger)arg1 arg2:(NSString *)arg2;
@end
@implementation MyClass
+ (void)classMethod1
{
}
- (void)method1
{
    NSLog(@"call method method1");
}
- (void)method2
{
}
- (void)method3WithArg1:(NSInteger)arg1 arg2:(NSString *)arg2
{
    NSLog(@"arg1 : %ld, arg2 : %@", arg1, arg2);
}
@end
//-----------------------------------------------------------
// main.h
#import "MyClass.h"
#import "MySubClass.h"
#import <objc/runtime.h>
int main(int argc, const char *argv[])
{
    @autoreleasepool {
        MyClass *myClass = [[MyClass alloc] init];
        unsigned int outCount = 0;
        Class cls = myClass.class;
        // 类名
        NSLog(@"class name: %s", class_getName(cls));
        NSLog(@"==========================================================");
        // 父类
        NSLog(@"super class name: %s", class_getName(class_getSuperclass(cls)));
        NSLog(@"==========================================================");
        // 是否是元类
        NSLog(@"MyClass is %@ a meta-class", (class_isMetaClass(cls) ? @"" : @"not"));
        NSLog(@"==========================================================");
        Class meta_class = objc_getMetaClass(class_getName(cls));
        NSLog(@"%s's meta-class is %s", class_getName(cls), class_getName(meta_class));
        NSLog(@"==========================================================");
        // 变量实例大小
        NSLog(@"instance size: %zu", class_getInstanceSize(cls));
        NSLog(@"==========================================================");
        // 成员变量
        Ivar *ivars = class_copyIvarList(cls, &outCount);
        for (int i = 0; i < outCount; i++) {
            Ivar ivar = ivars[i];
            NSLog(@"instance variable's name: %s at index: %d", ivar_getName(ivar), i);
        }
        free(ivars);
        Ivar string = class_getInstanceVariable(cls, "_string");
        if (string != NULL) {
            NSLog(@"instace variable %s", ivar_getName(string));
        }
        NSLog(@"==========================================================");
        // 属性操作
        objc_property_t *properties = class_copyPropertyList(cls, &outCount);
        for (int i = 0; i < outCount; i++) {
            objc_property_t property = properties[i];
            NSLog(@"property's name: %s", property_getName(property));
        }
        free(properties);
        objc_property_t array = class_getProperty(cls, "array");
        if (array != NULL) {
            NSLog(@"property %s", property_getName(array));
        }
        NSLog(@"==========================================================");
        // 方法操作
        Method *methods = class_copyMethodList(cls, &outCount);
        for (int i = 0; i < outCount; i++) {
            Method method = methods[i];
            NSLog(@"method's signature: %s", method_getName(method));
        }
        free(methods);
        Method method1 = class_getInstanceMethod(cls, @selector(method1));
        if (method1 != NULL) {
            NSLog(@"method %s", method_getName(method1));
        }
        Method classMethod = class_getClassMethod(cls, @selector(classMethod1));
        if (classMethod != NULL) {
            NSLog(@"class method : %s", method_getName(classMethod));
        }
        NSLog(@"MyClass is%@ responsd to selector: method3WithArg1:arg2:", class_respondsToSelector(cls, @selector(method3WithArg1:arg2:)) ? @"" : @" not");
        IMP imp = class_getMethodImplementation(cls, @selector(method1));
        imp();
        NSLog(@"==========================================================");
        // 协议
        Protocol *__unsafe_unretained *protocols = class_copyProtocolList(cls, &outCount);
        Protocol *protocol;
        for (int i = 0; i < outCount; i++) {
            protocol = protocols[i];
            NSLog(@"protocol name: %s", protocol_getName(protocol));
        }
        NSLog(@"MyClass is%@ responsed to protocol %s", class_conformsToProtocol(cls, protocol) ? @"" : @" not", protocol_getName(protocol));
        NSLog(@"==========================================================");
    }
    return 0;
}
//动态创建类
// 创建一个新类和元类
Class objc_allocateClassPair(Class superclass, const char *name, size_t extraBytes);
// 销毁一个类及其相关联的类
void objc_disposeClassPair(Class cls);
// 在应用中注册由objc_allocateClassPair创建的类
void objc_registerClassPair(Class cls);
#pragma mark - 实例3
Class cls = objc_allocateClassPair(MyClass.class, "MySubClass", 0);
class_addMethod(cls, @selector(submethod1), (IMP)imp_submethod1, "v@:");
class_replaceMethod(cls, @selector(method1), (IMP)imp_submethod1, "v@:");
class_addIvar(cls, "_ivar1", sizeof(NSString *), log(sizeof(NSString *)), "i");
objc_property_attribute_t type = {"T", "@\"NSString\""};
objc_property_attribute_t ownership = {"C", ""};
objc_property_attribute_t backingivar = {"V", "_ivar1"};
objc_property_attribute_t attrs[] = {type, ownership, backingivar};
class_addProperty(cls, "property2", attrs, 3);
objc_registerClassPair(cls);
id instance = [[cls alloc] init];
[instance performSelector:@selector(submethod1)];
[instance performSelector:@selector(method1)];
//动态创建对象
// 创建类实例
id class_createInstance(Class cls, size_t extraBytes);
// 在指定位置创建类实例
id objc_constructInstance(Class cls, void *bytes);
// 销毁类实例
void *objc_destructInstance(id obj);
#pragma mark - 实例4
id theObject = class_createInstance(NSString.class, sizeof(unsigned));
id str1 = [theObject init];
NSLog(@"%@", [str1 class]);
id str2 = [[NSString alloc] initWithString:@"test"];
NSLog(@"%@", [str2 class]);
//函数操作
// 返回指定对象的一份拷贝
id object_copy(id obj, size_t size);
// 释放指定对象占用的内存
id object_dispose(id obj);
#pragma mark - 实例5
NSObject *a = [[NSObject alloc] init];
id newB = object_copy(a, class_getInstanceSize(MyClass.class));
object_setClass(newB, MyClass.class);
object_dispose(a);
// 修改类实例的实例变量的值
Ivar object_setInstanceVariable(id obj, const char *name, void *value);
// 获取对象实例变量的值
Ivar object_getInstanceVariable(id obj, const char *name, void **outValue);
// 返回指向给定对象分配的任何额外字节的指针
void *object_getIndexedIvars(id obj);
// 返回对象中实例变量的值
id object_getIvar(id obj, Ivar ivar);
// 设置对象中实例变量的值
void object_setIvar(id obj, Ivar ivar, id value);
// 返回给定对象的类名
const char *object_getClassName(id obj);
// 返回对象的类
Class object_getClass(id obj);
// 设置对象的类
Class object_setClass(id obj, Class cls);
// 获取已注册的类定义的列表
int objc_getClassList(Class *buffer, int bufferCount);
// 创建并返回一个指向所有已注册类的指针列表
Class *objc_copyClassList(unsigned int *outCount);
// 返回指定类的类定义
Class objc_lookUpClass(const char *name);
Class objc_getClass(const char *name);
Class objc_getRequiredClass(const char *name);
// 返回指定类的元类
Class objc_getMetaClass(const char *name);
#pragma mark -  实例6
int numClasses;
Class *classes = NULL;
numClasses = objc_getClassList(NULL, 0);
if (numClasses > 0) {
    classes = malloc(sizeof(Class) * numClasses);
    numClasses = objc_getClassList(classes, numClasses);
    NSLog(@"number of classes: %d", numClasses);
    for (int i = 0; i < numClasses; i++) {
        Class cls = classes[i];
        NSLog(@"class name: %s", class_getName(cls));
    }
    free(classes);
}
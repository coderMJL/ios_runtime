typedef struct objc_category *Category;
struct objc_category
{
    char *category_name OBJC2_UNAVAILABLE; // 分类名
    char *class_name OBJC2_UNAVAILABLE; // 分类所属的类名
    struct objc_method_list *instance_methods OBJC2_UNAVAILABLE; // 实例方法列表
    struct objc_method_list *class_methods OBJC2_UNAVAILABLE; // 类方法列表
    struct objc_protocol_list *protocols OBJC2_UNAVAILABLE; // 分类所实现的协议列表
} typedef struct objc_object Protocol;
#pragma mark - 实例1
@interface RuntimeCategoryClass : NSObject
- (void)method1;
@end
@interface RuntimeCategoryClass (Category)
- (void)method2;
@end
@implementation RuntimeCategoryClass
- (void)method1
{
}
@end
@implementation RuntimeCategoryClass (Category)
- (void)method2
{
}
@end
#pragma mark -
NSLog(@"测试objc_class中的方法列表是否包含分类中的方法");
unsigned int outCount = 0;
Method *methodList = class_copyMethodList(RuntimeCategoryClass.class, &outCount);
for (int i = 0; i < outCount; i++) {
    Method method = methodList[i];
    const char *name = sel_getName(method_getName(method));
    NSLog(@"RuntimeCategoryClass's method: %s", name);
    if (strcmp(name, sel_getName(@selector(method2)))) {
        NSLog(@"分类方法method2在objc_class的方法列表中");
    }
}
// 返回指定的协议
Protocol *objc_getProtocol(const char *name);
// 获取运行时所知道的所有协议的数组
Protocol **objc_copyProtocolList(unsigned int *outCount);
// 创建新的协议实例
Protocol *objc_allocateProtocol(const char *name);
// 在运行时中注册新创建的协议
void objc_registerProtocol(Protocol *proto);
// 为协议添加方法
void protocol_addMethodDescription(Protocol *proto, SEL name, const char *types, BOOL isRequiredMethod, BOOL isInstanceMethod);
// 添加一个已注册的协议到协议中
void protocol_addProtocol(Protocol *proto, Protocol *addition);
// 为协议添加属性
void protocol_addProperty(Protocol *proto, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount, BOOL isRequiredProperty, BOOL isInstanceProperty);
// 返回协议名
const char *protocol_getName(Protocol *p);
// 测试两个协议是否相等
BOOL protocol_isEqual(Protocol *proto, Protocol *other);
// 获取协议中指定条件的方法的方法描述数组
struct objc_method_description *protocol_copyMethodDescriptionList(Protocol *p, BOOL isRequiredMethod, BOOL isInstanceMethod, unsigned int *outCount);
// 获取协议中指定方法的方法描述
struct objc_method_description protocol_getMethodDescription(Protocol *p, SEL aSel, BOOL isRequiredMethod, BOOL isInstanceMethod);
// 获取协议中的属性列表
objc_property_t *protocol_copyPropertyList(Protocol *proto, unsigned int *outCount);
// 获取协议的指定属性
objc_property_t protocol_getProperty(Protocol *proto, const char *name, BOOL isRequiredProperty, BOOL isInstanceProperty);
// 获取协议采用的协议
Protocol **protocol_copyProtocolList(Protocol *proto, unsigned int *outCount);
// 查看协议是否采用了另一个协议
BOOL protocol_conformsToProtocol(Protocol *proto, Protocol *other);
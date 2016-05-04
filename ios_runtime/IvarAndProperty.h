#pragma mark - 实例1
float a[] = {1.0, 2.0, 3.0};
NSLog(@"array encoding type: %s", @encode(typeof(a)));
typedef struct objc_ivar *Ivar;
struct objc_ivar
{
    char *ivar_name OBJC2_UNAVAILABLE; // 变量名
    char *ivar_type OBJC2_UNAVAILABLE; // 变量类型
    int ivar_offset OBJC2_UNAVAILABLE; // 基地址偏移字节
#ifdef __LP64__
    int space OBJC2_UNAVAILABLE;
#endif
} typedef struct objc_property *objc_property_t;
typedef struct
{
    const char *name; // 特性名
    const char *value; // 特性值
} objc_property_attribute_t;
//我们可以把关联对象想象成一个Objective-C对象(如字典)，这个对象通过给定的key连接到类的一个实例上。不过由于使用的是C接口，所以key是一个void指针(const void
//*)。我们还需要指定一个内存管理策略，以告诉Runtime如何管理这个对象的内存。这个内存管理的策略可以由以下值指定：
//当宿主对象被释放时，会根据指定的内存管理策略来处理关联对象。如果指定的策略是assign，则宿主释放时，关联对象不会被释放；而如果指定的是retain或者是copy，则宿主释放时，关联对象会被释放。我们甚至可以选择是否是自动retain/copy。当我们需要在多个线程中处理访问关联对象的多线程代码时，这就非常有用了。
OBJC_ASSOCIATION_ASSIGN
OBJC_ASSOCIATION_RETAIN_NONATOMIC
OBJC_ASSOCIATION_COPY_NONATOMIC
OBJC_ASSOCIATION_RETAIN
OBJC_ASSOCIATION_COPY
#pragma mark - 实例2
static char myKey;
objc_setAssociatedObject(self, &myKey, anObject, OBJC_ASSOCIATION_RETAIN);
id anObject = objc_getAssociatedObject(self, &myKey);
- (void)setTapActionWithBlock:(void (^)(void))block
{
    UITapGestureRecognizer *gesture = objc_getAssociatedObject(self, &kDTActionHandlerTapGestureKey);
    if (!gesture) {
        gesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(__handleActionForTapGesture:)];
        [self addGestureRecognizer:gesture];
        objc_setAssociatedObject(self, &kDTActionHandlerTapGestureKey, gesture, OBJC_ASSOCIATION_RETAIN);
    }
    objc_setAssociatedObject(self, &kDTActionHandlerTapBlockKey, block, OBJC_ASSOCIATION_COPY);
}
- (void)__handleActionForTapGesture:(UITapGestureRecognizer *)gesture
{
    if (gesture.state == UIGestureRecognizerStateRecognized) {
        void (^action)(void) = objc_getAssociatedObject(self, &kDTActionHandlerTapBlockKey);
        if (action) {
            action();
        }
    }
}
// 获取成员变量名
const char *ivar_getName(Ivar v);
// 获取成员变量类型编码
const char *ivar_getTypeEncoding(Ivar v);
// 获取成员变量的偏移量
ptrdiff_t ivar_getOffset(Ivar v);
// 设置关联对象
void objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy);
// 获取关联对象
id objc_getAssociatedObject(id object, const void *key);
// 移除关联对象
void objc_removeAssociatedObjects(id object);
// 获取属性名
const char *property_getName(objc_property_t property);
// 获取属性特性描述字符串
const char *property_getAttributes(objc_property_t property);
// 获取属性中指定的特性
char *property_copyAttributeValue(objc_property_t property, const char *attributeName);
// 获取属性的特性列表
objc_property_attribute_t *property_copyAttributeList(objc_property_t property, unsigned int *outCount);
#pragma mark - 实例3
@interface MyObject : NSObject
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *status;
@end
@{@"name1" : "张三", @"status1" : @"start"} @{@"name2" : "张三", @"status2" : @"end"} static NSMutableDictionary *map = nil;
@implementation MyObject
+ (void)load
{
    map = [NSMutableDictionary dictionary];
    map[@"name1"] = @"name";
    map[@"status1"] = @"status";
    map[@"name2"] = @"name";
    map[@"status2"] = @"status";
}
@end
- (void)setDataWithDic:(NSDictionary *)dic
{
    [dic enumerateKeysAndObjectsUsingBlock:^(NSString *key, id obj, BOOL *stop) {
        NSString *propertyKey = [self propertyForKey:key];
        if (propertyKey) {
            objc_property_t property = class_getProperty([self class], [propertyKey UTF8String]);
            // TODO: 针对特殊数据类型做处理
            NSString *attributeString = [NSString stringWithCString:property_getAttributes(property) encoding:NSUTF8StringEncoding];
            ... [self setValue:obj forKey:propertyKey];
        }
    }];
}
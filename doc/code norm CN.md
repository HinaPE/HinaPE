# HinaPE 代码规范

## 代码规范

### 〇：命名规则

HinaPE采用近似Google代码规范的命名规则。

#### 文件名：snake式下划线

- 头文件：`my_class.h`
- cpp文件：`my_class.cpp`

#### 类命名：全首字母大写

```c++
class MyFirstClass
{
  	// content
};
```

#### 函数命名：snake式下划线

```c++
void my_function(int param0, int param1) {}
```

#### 类成员命名：下划线前置

```c++
class MyFirstClass
{
private:
  	int _my_int;
  	float _my_float;
};
```

#### namespace：全首字母大写

```c++
namespace HinaPE
{

}
```

### 一：大括号换行

```c++
void function()
{
  	// content
}
```

```c++
class MyClass
{
  	// content
};
```

单行的函数不换行

```c++
void get_position() { return _positions; }
```

### 二：函数返回值后置

为保达到头部对齐的美观目的，HinaPE采取函数返回值后置的规范。非`void`类型的函数需要使用后值返回值写法。

```c++
class Foo
{
 public:
    void function0();
    auto function1() -> int;
  	auto funciton2() -> float;
  	// ...
};
```

### 三：RAII规则：智能指针与禁止平凡实例化

由于C++没有内存垃圾回收机制，以及我们无法防止我们不想要的复制操作，在HinaPE中的所有类的创建均采用`shared_ptr`智能指针。

```c++
#include <memory>
class Foo
{
		// content
};
using FooPtr = std::shared_ptr<Foo>;

// 在需要实例化的地方
FooPtr my_foo = std::make_shared<Foo>();
```

禁止平凡实例化

- ~~`Foo foo();`~~
- ~~`Foo *foo = new Foo();`~~

尤其是第二种实例化，必须要手动delete才可以释放内存，是一件非常危险的事情。但是我们并不希望事情变得这么麻烦，尤其是在程序规模比较大了之后。

与此同时，如果我们这样写代码：

```c++
Foo foo();
Foo another_foo = foo;
```

我们完全不可控制第二行是否发生了复制操作。如果Foo类内部有一个极其大的数组，那么一旦发生复制，整个系统的性能就会瞬间下降，这是极其需要注意的地方。

如果采用智能指针的方案，那么上述这些问题就能够得到比较好的解决。

### 四：Class编写规范与内部排序规则

当我们的Class内部的内容变多之后，整个Class就会变得冗杂。因此，我们需要制定一个Class内部的排序规则规范，这样就能比较方便地去找到各个模块。

```c++
class MyFirstClass
{
public:
    void public_method1();
  	auto public_method2(int param) -> bool {}
  	auto public_method3() -> mVector3 {}
  
public:
  	struct Opt
    {
      	bool param1 = true;
      	int param2 = 0;
      	float param3 = 1.f;
    } _opt;
  
public:
  	MyFirstClass(VeryBigDataClassPtr ptr, Opt opt);
    MyFirstClass(const MyFirstClass &) = delete;
    MyFirstClass(MyFirstClass &&) = default;
    ~MyFirstClass();
    auto operator=(const MyFirstClass &) -> MyFirstClass & = delete;
    auto operator=(MyFirstClass &&) -> MyFirstClass & = default;
      
private:
    void private_method1();
  	auto private_method2(int param) -> bool {}
  	auto private_method3() -> mVector3 {}
  
private:
  	VeryBigDataClassPtr ptr; // 一个智能指针指向一个包含非常大的数据的类
};
```

#### 最高优先级：public methods

public methods一定是我们最需要关注的东西。因为它们定义了我们该如何在外部去操作这个类。所以，将所有public methods放在一个类的最顶部通常是一个不会错的选择。

#### 第二优先级：参数列表Option

对于一个类而言，我们需要划分出什么是`参数`，而什么是`成员数据`。

`参数`就是一些调整系数，可能需要频繁在外部调整的。而`成员数据`通常是一些别的类，或者是一些占据内存非常大的`数据`。因此，我们把`参数`直接public然后支持让外部直接access是一个比较好的方案。这样也可以避免一大堆get/set方法使得整个类变得非常臃肿。

#### 第三优先级：构造函数与析构函数

把所有的构造函数（包括复制构造函数和移动构造函数）和析构函数写到这个位置。

作为一个规范，除非是非常简单的类，不然一定要显式定义出所有的构造函数、析构函数和赋值/移动操作符，必要时将其删除（`= delete`），以防止不必要的复制行为。

#### 第四优先级：protected与private函数

其实这一层级的函数是最不必要被关注的。他们存在的意义就是被public methods给调用。之所以写在这里，是为了把private members给沉底，方便我们一拉就能拉到最底下看到这个类所有的成员变量。

#### 第五优先级：private members

把private members给沉底，方便我们一拉就能拉到最底下看到这个类所有的成员变量。

## 常见错误

### 〇：引用、复制、移动与生命周期

如何处理引用、复制、移动几乎是整个**<u>现代</u>**C++最需要关注的问题。我们无时无刻都必须注意我们的类实例的生命周期。

在引入智能指针之后，这个问题则更加需要我们的注意。

- 值传递
- 指针传递
- 智能指针传递
- 引用传递
- 移动引用传递

这五种最常用的变量传递方法我们需要熟练掌握。

### 一：Struct中的虚函数表

大意：虚函数需要维护一个虚函数表，这将占据struct的内存（8字节，但要考虑内存对齐的情况），需要特别注意。

https://stackoverflow.com/questions/9439240/sizeof-class-with-int-function-virtual-function-in-c

TODO
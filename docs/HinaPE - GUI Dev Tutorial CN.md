# HinaPE - GUI Dev Tutorial

> 写在前面
>
> 理论上而言，开发GUI的确只需要关注gui文件夹下的所有文件即可。但是在实际开发中，我们想要实现在图形界面中执行某个操作（比如修改某个参数的值），并希望将其实时应用到系统中，那么势必需要存在GUI对外部系统的操控。
>
> 因此事实上，GUI是整个逻辑系统的出口。所以，作为一个原则：
>
> **<span style="color:red;"><u>GUI需要依赖核心（Core）部分，但核心部分永远不应该依赖GUI。</u></span>**

## Chapter I - ImGui Basic

### 什么是ImGui？

[ImGui](https://github.com/ocornut/imgui.git)是一个跨平台的图形组件库。在深入了解ImGui之前，我们首先对它有个第一印象。

- 第一点，不要去试图点进任何ImGui的函数内部试图理解它，也不要去试图去理解文档。ImGui很复杂很复杂，连作者本人都写不好文档，所以直接用就好。
- 第二点：看到别人怎么写之后就照着写，不要尝试去创新。ImGui是一个，只要你看到过使用样例后照猫画虎就很好用的图形库。但与此同时，它是一个极其不容易自己创作的图形库。因此，作为一个准则，多记忆别人是怎么写的，然后融会贯通就好。

### 一些基本的的ImGui使用方法

**<u>作为一个原则：ImGui一定要自己用过，然后知道怎么写产生什么样的结果才行。不然只看文档是没啥用的。所以，这里只写代码，就不给效果图了。</u>**

**<u>作为一个原则：ImGui永远以程序中悬浮然后出来的函数参数为准，这里只是给个例子。</u>**

#### ImGui的结构

我们知道，ImGui是用来引入**<u>窗口</u>**的。因此，我们想要引入任何**<u>ImGui组件</u>**之前，需要新建一个ImGui窗口。

一个常见的新建窗口的流程大概是长这样的：

```c++
// 第一步，指定下一个要生成的窗口的位置。如果不指定，应该会紧贴着上一个窗口的下方。读者可以自己试试。注意这里的大括号，也可以在程序中悬浮看看函数接受的参数。
ImGui::SetNextWindowPos({width, height});
// 第二步，指定下一个窗口的大小
ImGui::SetNextWindowSizeConstraints({min_width, min_height}, {max_width, max_height});
//第三步，Begin
ImGui::Begin(const char *name, bool *ptr_open = __null, ImGuiWindowFlags flags = 0);
// 第四步，自由地写好多好多ImGui组件=
... ...
// 第五步，End
ImGui::End();
```

整个HinaPE（即Scotty3D）中的ImGui窗口都是长成这个模式的，读者在写的时候需要时刻注意，自己正在哪个窗口添加ImGui组件。

#### ImGui常用组件

##### Text

- `ImGui::Text("MyText")`

##### Button

```c++
if (ImGui::Button("ButtonName"))
{
  // 这里写如果这个按钮被按下会发生的事情
}
```

##### Slider

```c++
static float num = 0.f;
ImGui::SliderFloat("Name", &num, /**最小值**/0.f, /**最大值**/10.f, "%.2f"); //"%.2f"为format，指保留两位小数，和c语言的printf里的参数一样

static float num[2] = {0.f, 1.f};
ImGui::SliderFloat2("Name", num, /**最小值**/1.0f, /**最大值**/10.0f /**format是可以不指定的**/); //数组名本身就是指针
```
Origin Engine

I, what is Origin Engine ?
    an open source, cross-platform and multily renderer 3D MMORPG game engine


II, feature
1, 多渲染接口 DirectX/OpenGL
2, 跨平台 Window/Linux
3, 国际化, UNICODE/ANSI whatever


III, 初步引擎的主体框架, 分为 3 大模块:
1, OriginEngine 引擎核心 用于控制并协调其他模块协同工作
2, RenderD3D/RenderOpenGL 引擎的渲染模块 专门负责特定的渲染模块, 以组件的形式供用户选择, 支持 DirectX/OpenGL
3, OEApp 用户程序接口 用户程序与引擎的接入点


IV, 目录结构
bin                 // 程序运行初始目录
build               // 编译产生的临时文件
demo                // demo 演示程序
    demo_test       // 测试程序
include             // 引擎SDK文件头
lib                 // 引擎SDK库文件
source
    libOE           // Origin Engine 基本库, 所有应用程序必须静态链接该库
    OriginEngine    // 核心模块
    RendererD3D     // DirectX 渲染模块
    RendererOpenGL  // OpenGL 渲染模块


V, 如何编译,运行,调试
    目前只针对Windows平台, OriginEngine.sln 用于编译引擎, demo.sln 用于编译demo;

    以 demo_test 为列 调试程序时设置

    Debug:
    Command = trunk目录\bin\demo_test_D.exe
    Working Directory = trunk目录\bin\

    Release:
    Command = trunk目录\bin\demo_test.exe
    Working Directory = trunk目录\bin\

    Unicode Debug:
    Command = trunk目录\bin\demo_test_UD.exe
    Working Directory = trunk目录\bin\

    Unicode Release:
    Command = trunk目录\bin\demo_testU.exe
    Working Directory = trunk目录\bin\


VI, how it is running ?
    用户应用程序在 main 函数 new OEApp 并执行 IOEApp->Run() 函数, Run 函数将加载并初始化 OriginEngine 核心,
再由核心模块加载并初始化其他模块 (目前只有渲染模块), 在初始化模块的过程中传入的参数 OEHolder, 用于同步各个
模块间的接口指针, 待初始化完成后, 就会进入 IOEDevice->StartPerform, 以Window 平台为例: StartPerform 函数创
建 Windows 并初始化 D3D, 然后进入消息循环, 在循环期间每帧调用 IOEApp->Update, IOEApp->Render 进行更新与渲染
直到应用程序结束


VII, 编码规范
    1, char 类型别用, 取而代之 用 tchar
    2, std::string, std::wstring 类型别用, 取而代之 用 tstring

    3, 基本类型
    unsigned char -> uchar
    unsigned short -> ushort
    unsigned int -> uint
    (void*)0  -> NULL

    4, 所有接口类用 I[OE]XXX.h 写法, 所有接口的实现类用 C[OE]XXX_Impl.h C[OE]XXX_Impl.cpp 的写法

    5, 模块命名后缀
    (1), 后缀 _D ANSI 调试版本
    (2), 后缀 _UD UNICODE 调试版本
    (3), 后缀 U UNICODE 发行版本
    (4), 无后缀 ANSI 发行版本
    例: OriginEngine_UD.dll 为 UNICODE 调试版本, OriginEngine.dll 为 ANSI 发行版本






-------------------------------------------------------------------------------------------
以上所有文字为本人设计框架的总结与建议, 希望大家共同修订以形成规范 (zjhlogo 2009年5月24日)

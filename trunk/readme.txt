Origin Engine

I, what is Origin Engine ?
    an open source, cross-platform and multily renderer 3D MMORPG game engine


II, feature
1, ����Ⱦ�ӿ� DirectX/OpenGL
2, ��ƽ̨ Window/Linux
3, ���ʻ�, UNICODE/ANSI whatever


III, ���������������, ��Ϊ 3 ��ģ��:
1, OriginEngine ������� ���ڿ��Ʋ�Э������ģ��Эͬ����
2, RenderD3D/RenderOpenGL �������Ⱦģ�� ר�Ÿ����ض�����Ⱦģ��, ���������ʽ���û�ѡ��, ֧�� DirectX/OpenGL
3, OEApp �û�����ӿ� �û�����������Ľ����


IV, Ŀ¼�ṹ
bin                 // �������г�ʼĿ¼
build               // �����������ʱ�ļ�
demo                // demo ��ʾ����
    demo_test       // ���Գ���
include             // ����SDK�ļ�ͷ
lib                 // ����SDK���ļ�
source
    libOE           // Origin Engine ������, ����Ӧ�ó�����뾲̬���Ӹÿ�
    OriginEngine    // ����ģ��
    RendererD3D     // DirectX ��Ⱦģ��
    RendererOpenGL  // OpenGL ��Ⱦģ��


V, ��α���,����,����
    Ŀǰֻ���Windowsƽ̨, OriginEngine.sln ���ڱ�������, demo.sln ���ڱ���demo;

    �� demo_test Ϊ�� ���Գ���ʱ����

    Debug:
    Command = trunkĿ¼\bin\demo_test_D.exe
    Working Directory = trunkĿ¼\bin\

    Release:
    Command = trunkĿ¼\bin\demo_test.exe
    Working Directory = trunkĿ¼\bin\

    Unicode Debug:
    Command = trunkĿ¼\bin\demo_test_UD.exe
    Working Directory = trunkĿ¼\bin\

    Unicode Release:
    Command = trunkĿ¼\bin\demo_testU.exe
    Working Directory = trunkĿ¼\bin\


VI, how it is running ?
    �û�Ӧ�ó����� main ���� new OEApp ��ִ�� IOEApp->Run() ����, Run ���������ز���ʼ�� OriginEngine ����,
���ɺ���ģ����ز���ʼ������ģ�� (Ŀǰֻ����Ⱦģ��), �ڳ�ʼ��ģ��Ĺ����д���Ĳ��� OEHolder, ����ͬ������
ģ���Ľӿ�ָ��, ����ʼ����ɺ�, �ͻ���� IOEDevice->StartPerform, ��Window ƽ̨Ϊ��: StartPerform ������
�� Windows ����ʼ�� D3D, Ȼ�������Ϣѭ��, ��ѭ���ڼ�ÿ֡���� IOEApp->Update, IOEApp->Render ���и�������Ⱦ
ֱ��Ӧ�ó������


VII, ����淶
    1, char ���ͱ���, ȡ����֮ �� tchar
    2, std::string, std::wstring ���ͱ���, ȡ����֮ �� tstring

    3, ��������
    unsigned char -> uchar
    unsigned short -> ushort
    unsigned int -> uint
    (void*)0  -> NULL

    4, ���нӿ����� I[OE]XXX.h д��, ���нӿڵ�ʵ������ C[OE]XXX_Impl.h C[OE]XXX_Impl.cpp ��д��

    5, ģ��������׺
    (1), ��׺ _D ANSI ���԰汾
    (2), ��׺ _UD UNICODE ���԰汾
    (3), ��׺ U UNICODE ���а汾
    (4), �޺�׺ ANSI ���а汾
    ��: OriginEngine_UD.dll Ϊ UNICODE ���԰汾, OriginEngine.dll Ϊ ANSI ���а汾






-------------------------------------------------------------------------------------------
������������Ϊ������ƿ�ܵ��ܽ��뽨��, ϣ����ҹ�ͬ�޶����γɹ淶 (zjhlogo 2009��5��24��)

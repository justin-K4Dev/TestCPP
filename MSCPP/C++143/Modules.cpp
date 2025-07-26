#include "stdafx.h"


import CustomModule;

#include <iostream>


namespace Modules
{
    void module_use()
    {
        /*
            Module

            - C++20���� ���Ե� ���ο� �ڵ� ���� �� ������ ���� ���
            - �������� #include ��� ����� ������ ���� (�ߺ� ����, ������ �ӵ� ����, ��ũ�� ���� ��)
            - �����Ϸ��� ��⺰�� ��Ȯ�ϰ� �������̽�(����)�� ����(�����) ����
            - ����� export(����), module(����), import(���) Ű����� ����
            - ���� �ӵ� ����, �ڵ� ������ �� ĸ��ȭ ��ȭ, ODR(One Definition Rule) ���� ����
            - Ȯ����: ���� .ixx, .cppm, .mxx �� (�����Ϸ����� �ٸ�)
            - ���� �� �ݵ�� **��� ������ ���� ������**�ؾ� import�� ����
            - ���� �����Ϸ�: MSVC, GCC(13+), Clang(16+) ��
            - <module> ��� �ʿ� ���� (��� �⺻ ���)

            [cl ��ɾ�� ��� �����Ͻ� ���ǻ���]
              - Visual Studio�� "Developer Command Prompt for Visual Studio" �Ǵ�
                "x64 Native Tools Command Prompt for VS" �ֿܼ��� cl ��ɾ� ����ؾ� ��
              - �Ϲ� cmd.exe �Ǵ� PowerShell���� ������ ��� cl ��� �ν� �Ұ�
              - ���� �޴����� "Developer Command Prompt" �˻��Ͽ� ����
              - �ش� �ܼ� ���� �� �Ʒ��� ���� ������:
                  cl /std:c++20 /c custom_module.ixx
                  cl /std:c++20 main.cpp custom_module.obj


            [C++20 Module ���� ���� ����]

            1. ��� �ҽ� �ۼ�
                - ���� ����: CustomModule.ixx
                - ����:
                    export module CustomModule;
                    export int add(int a, int b) { return a + b; }
                    export int sub(int a, int b) { return a - b; }

            2. ��� ��� �ҽ� �ۼ�
                - ���� ����: Module.cpp
                - import CustomModule;
                  ... add, sub �Լ� ��� ...

            3. ���� ���� (Visual Studio IDE ����)
                - CustomModule.ixx ������ �ݵ�� "�ҽ� ����"�� ������Ʈ�� �߰�
                - ������Ʈ �Ӽ����� C++ ��� ǥ���� C++20(/std:c++20) �̻����� ����
                - ���� �� VS�� ����� ���� ������(.ifc, .obj ����) ��,
                  ������ �ҽ��� �Բ� �ڵ����� ��ũ�Ͽ� �������� ����

            4. ���� ���� (�����/Developer Command Prompt ����)
                # 1�ܰ�: ��� �ҽ�(.ixx)�� ���� ������Ʈ(.obj)�� ������
                    cl /std:c++20 /c CustomModule.ixx
                    // CustomModule.obj, CustomModule.ifc ������

                # 2�ܰ�: main.cpp/Module.cpp �� ��� ������
                         ������ ������ obj�� �Բ� ������ �� ��ũ
                    cl /std:c++20 main.cpp Module.cpp CustomModule.obj
                    // main.exe ����

                - �̶� .ixx ������ ���� main.cpp�� ���� ���������� ����!
                - �ݵ�� obj�� ���� ����� obj ���ϸ� ����

            5. �ֿ� ���ǻ���
                - "��� ����"�� �߰��ϸ� ����/��ũ ����� ���� ������ �ݵ�� "�ҽ� ����"�� �߰�
                - export�� �Լ��� import�� ������ ��� ����
                - ���� �� ���� ����ϴ� cpp ������ obj �ܰ迡�� ��� �����ؾ� unresolved external symbol ���� ����

            6. ���� ���� ���� �� �ذ��
                - ��� obj�� ��ũ���� ������: LNK2019(Ȯ���� �� ���� �ܺ� ��ȣ)
                - .ixx ������ ��� ���Ϸ� �߰��߰ų�, obj ���� ���� �� �߻�

            7. ���
                - ���� ���� ��, import�� ����� �Լ�(add, sub ��)�� main.cpp ��� ���� ��� ����

            ----------------------------------------------------------------

            [���� ���� ��� ����(�����)]
                cl /std:c++20 /c CustomModule.ixx
                cl /std:c++20 main.cpp Module.cpp CustomModule.obj

            [Visual Studio IDE]
                - �ҽ� ���Ϸ� .ixx �߰�
                - ���� �� �ڵ� ����

            ----------------------------------------------------------------

            [���� ��� ����]
                add(3, 2) = 5
                sub(7, 4) = 3
        */

        std::cout << add(3, 2) << std::endl;  // ���: 5
        std::cout << sub(7, 4) << std::endl;  // ���: 3
    }

    void Test()
    {
        module_use();
    }
}
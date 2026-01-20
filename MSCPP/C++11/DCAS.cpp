#include "stdafx.h"


#include <boost/thread.hpp>


namespace DCAS
{
    void DCAS_what()
    {
        /*
            📚 DCAS (Double-Compare-And-Swap)

              - DCAS는 2개의 메모리 주소(혹은 값)를 동시에 비교해서,
                둘 다 예상값과 같을 때만 동시에 새 값으로 교체하는 원자적 연산
              - 단일 CAS보다 복잡한 상태를 한 번에 동기화할 수 있어
                ABA(값이 A→B→A로 바뀌는 동시성 버그) 문제 방지에 효과적
              - 하드웨어 명령어로 지원되는 경우는 드물지만(대부분 CPU는 단일 CAS만 지원),
                소프트웨어적으로 포인터+버전(태그)을 한 번에 CAS하여 DCAS 효과를 낼 수 있음
              - lock-free 자료구조(stack, queue 등)에서 ABA 방지 및 일관성 보장에 자주 응용

              - 실전 구현 패턴(소프트웨어 DCAS):
                struct DcasNode {
                    void* ptr1;     // 예: top 포인터
                    size_t ver1;    // 예: 버전/세대/태그
                };
                std::atomic<DcasNode> head;
                // head.compare_exchange_strong(expected, desired)로 동시에 교체

              ※ C++ 표준 라이브러리에는 DCAS 내장 지원 없음, tagged pointer 등으로 구현

        */
    }

    struct Node {
        int value;
        Node* next;
        Node(int v) : value(v), next(nullptr) {}
    };

    // 포인터+버전 구조체 (DCAS 효과를 내기 위한 tagged pointer)
    struct TaggedPtr {
        Node* ptr;
        uint64_t tag; // 버전/세대

        // 동등성 비교 지원
        bool operator==(const TaggedPtr& rhs) const {
            return ptr == rhs.ptr && tag == rhs.tag;
        }
    };

    // 원자적 tagged pointer
    std::atomic<TaggedPtr> atomic_head;

    void dcas_push(int value) 
    {
        Node* new_node = new Node(value);
        TaggedPtr old_head = atomic_head.load(std::memory_order_acquire);
        while (true) {
            new_node->next = old_head.ptr;
            TaggedPtr new_head = { new_node, old_head.tag + 1 };
            // head 포인터와 버전(tag)이 모두 예상값과 같을 때만 교체(DCAS 효과)
            if (atomic_head.compare_exchange_weak(old_head, new_head, std::memory_order_release, std::memory_order_acquire))
                break;
            // 실패 시 old_head가 최신값으로 자동 갱신됨
        }
    }

    bool dcas_pop(int& value) 
    {
        TaggedPtr old_head = atomic_head.load(std::memory_order_acquire);

        while (true) {
            if (old_head.ptr == nullptr)
                return false;

            Node* node = old_head.ptr;
            TaggedPtr new_head = { node->next, old_head.tag + 1 };
            if (atomic_head.compare_exchange_weak(old_head, new_head, std::memory_order_release, std::memory_order_acquire)) {
                value = node->value;
                delete node;
                return true;
            }
        }
    }

    void DCAS_with_stack()
    {
        // 초기화
        atomic_head = TaggedPtr{ nullptr, 0 };

        // push 테스트
        dcas_push(10);
        dcas_push(20);
        dcas_push(30);

        // pop 테스트
        int v;
        while (dcas_pop(v)) {
            std::cout << "pop: " << v << std::endl;
        }
        /*
        출력:
            pop: 30
            pop: 20
            pop: 10
        */
    }


	void Test()
	{
        DCAS_with_stack();

        DCAS_what();
	}
}//DCAS


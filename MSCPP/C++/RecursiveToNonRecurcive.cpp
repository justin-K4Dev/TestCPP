#include "stdafx.h"


namespace RecursiveToNonRecursive
{
	// 재귀 호출을 한번할 경우

	int factorial_recursive(int n)
	{
		// by recursive

		if (0 == n) {
			return 1;
		}
		else {
			return n * factorial_recursive(n - 1);
		}
	}

	int factorial_non_recursive(int n)
	{
		// by iteration !!!

		int f = 1;

		while (n > 0) {
			f = f * n--;
		}

		return f;
	}

	void factorial()
	{
		printf("recursive - factorial_recursive: %d\n", factorial_recursive(5));

		printf("iteration - factorial_non_recursive: %d\n", factorial_non_recursive(5));

		system("pause");

		/*
		output:
			recursive - factorial_recursive: 120
			iteration - factorial_non_recursive: 120
		*/
	}


	struct node
	{
		int key;
		struct node *left;
		struct node *right;
	};

	node *head, *tail;

	#define MAX  100

	node *stack[MAX];
	
	int top;

	void init_stack()
	{
		top = -1;
	}

	void init_tree()
	{
		head = (node*)malloc(sizeof(node));
		tail = (node*)malloc(sizeof(node));

		head->left = tail;
		head->right = tail;
		
		tail->left = tail;
		tail->right = tail;
	}

	node* push(node *t)
	{
		if (top >= MAX - 1) {
			printf("failed to push cause Stack overflow !!!\n");
			return nullptr;
		}

		stack[++top] = t;
		return t;
	}

	node* pop()
	{
		if (top < 0) {
			return tail;
		}

		return stack[top--];
	}

	int is_stack_empty()
	{
		return (top == -1);
	}

	void visit(node *t)
	{
		printf("Node-Key:%c\n", t->key);
	}

	int is_operator(int k)
	{
		return (k == '+' || k == '-' || k == '*' || k == '/');
	}

	int is_legal(char *s)
	{
		int f = 0;
		while (*s)
		{
			while (*s == ' ')   /* remove space */
				s++;
			if (is_operator(*s))
				f--;
			else
				f++;
			if (f < 1)  break; /* check situation like A + B */
			s++;
		}
		return (f == 1);   /* legal if valuable - operator == 1 */
	}

	node *make_parse_tree(const char *p)
	{
		node *t;
		int preChildCount = 0;
		int currChildCount = 0;

		while (*p) {
			while (*p == ' ') {
				p++;
			}

			t = (node*)malloc(sizeof(node));

			if (is_operator(*p)) {
				preChildCount = currChildCount;
				currChildCount = 0;

				p++;

				t->key = *p;
				
				t->right = pop();
				t->left = pop();
			}
			else {
				currChildCount++;

				t->key = *p;

				if (0 < preChildCount) {
					t->right = pop();
					preChildCount--;
					t->left = pop();
					preChildCount--;
				}
				else {
					t->left = tail;
					t->right = tail;
				}
			}
			
			push(t);
			p++;
		}

		return pop();
	}


	// 주 로직이 처음 실행될 경우

	void preorder_traverse_recursive(node *t)
	{
		if (t != tail) {
			visit(t);								// 주 로직
			preorder_traverse_recursive(t->left);	// 재귀 1
			preorder_traverse_recursive(t->right);	// 재귀 2
		}
	}

	void preorder_traverse_non_recursive(node *t)
	{
		push(t);
		while (!is_stack_empty()) {
			t = pop();
			if (t != tail) {
				visit(t);		// 주 로직   
				push(t->right); // 재귀 2 - 인자 스택이기에 나중에 수행할 right를 먼저 등록
				push(t->left);	// 재귀 1 - 인자
			}
		}
	}

	void preorder_traverse()
	{
		init_stack();
		init_tree();

		std::string input("1+23+456+789");
		std::cout << "input: " << input << std::endl;
		std::reverse(input.begin(), input.end());

		head->right = make_parse_tree(input.c_str());

		std::cout << "preorder_traverse_recursive()" << std::endl;
		preorder_traverse_recursive(head->right);

		std::cout << "preorder_traverse_non_recursive()" << std::endl;
		preorder_traverse_non_recursive(head->right);

		system("pause");

		/*
		output:
			preorder_traverse_recursive()
			Node-Key:1
			Node-Key:2
			Node-Key:3
			Node-Key:4
			Node-Key:5
			Node-Key:9
			Node-Key:6
			Node-Key:7
			Node-Key:8
			preorder_traverse_non_recursive()
			Node-Key:1
			Node-Key:2
			Node-Key:3
			Node-Key:4
			Node-Key:5
			Node-Key:9
			Node-Key:6
			Node-Key:7
			Node-Key:8
		*/
	}


	// 주 로직이 중간에 실행될 경우

	void inorder_traverse_recursive(node *t)
	{
		if (t != tail) {
			inorder_traverse_recursive(t->left);	// 재귀 1
			visit(t);								// 주 로직
			inorder_traverse_recursive(t->right);	// 재귀 2
		}
	}

	void inorder_traverse_non_recursive(node *t)
	{
		while (true) {
			while (t != tail) {
				push(t);			// 재귀 1 - push in stack
				t = t->left;
			}

			if (!is_stack_empty()) {
				t = pop();
				visit(t);			// 주 로직
				t = t->right;		// 재귀 2 - 인자 변경
			}
			else {
				break;
			}
		}
	}

	void inorder_traverse()
	{
		init_stack();
		init_tree();

		std::string input("1+23+456+789");
		std::cout << "input: " << input << std::endl;
		std::reverse(input.begin(), input.end());

		head->right = make_parse_tree(input.c_str());

		std::cout << "inorder_traverse_recursive()" << std::endl;
		inorder_traverse_recursive(head->right);

		std::cout << "inorder_traverse_non_recursive()" << std::endl;
		inorder_traverse_non_recursive(head->right);

		system("pause");

		/*
		output:
			inorder_traverse_recursive()
			Node-Key:1
			Node-Key:2
			Node-Key:3
			Node-Key:4
			Node-Key:9
			Node-Key:5
			Node-Key:8
			Node-Key:6
			Node-Key:7
			inorder_traverse_non_recursive()
			Node-Key:1
			Node-Key:2
			Node-Key:3
			Node-Key:4
			Node-Key:9
			Node-Key:5
			Node-Key:8
			Node-Key:6
			Node-Key:7
		*/
	}


	// 주 로직이 마지막에 실행될 경우

	void postorder_traverse_recursive(node *t)
	{
		if (t != tail) {
			postorder_traverse_recursive(t->left);	// 재귀 1
			postorder_traverse_recursive(t->right);	// 재귀 2
			visit(t);						        // 주 로직
		}
	}

	void postorder_traverse_non_recursive(node *t)
	{
		int done = 0;
		node *s;	// 인자 리스트의 복사본

		while (!done) {
			while (t != tail) {				// 인자리스트 푸쉬
				push(t);
				t = t->left;				// 재귀 1 인자리스트 푸쉬
			}

			while (!is_stack_empty()) {
				s = t;						// 인자리스트 복사
				t = pop();
				if (t->right != tail) {
					if (t->right == s) {
						visit(t);			// 주 로직
					}
					else {
						push(t);
						t = t->right;		// 재귀 2 인자리스트
						break;
					}
				}
				else {
					visit(t);				// 주 로직
				}
				if (is_stack_empty()) {
					done = 1;
				}
			}
		}
	}

	void postorder_traverse()
	{
		init_stack();
		init_tree();

		std::string input("1+23+456+789");
		std::cout << "input: " << input << std::endl;
		std::reverse(input.begin(), input.end());

		head->right = make_parse_tree(input.c_str());

		std::cout << "postorder_traverse_recursive()" << std::endl;
		postorder_traverse_recursive(head->right);

		std::cout << "postorder_traverse_non_recursive()" << std::endl;
		postorder_traverse_non_recursive(head->right);

		system("pause");

		/*
		output:
			postorder_traverse_recursive()
			Node-Key:9
			Node-Key:8
			Node-Key:7
			Node-Key:6
			Node-Key:5
			Node-Key:4
			Node-Key:3
			Node-Key:2
			Node-Key:1
			postorder_traverse_non_recursive()
			Node-Key:9
			Node-Key:8
			Node-Key:7
			Node-Key:6
			Node-Key:5
			Node-Key:4
			Node-Key:3
			Node-Key:2
			Node-Key:1
		*/
	}


	void Test()
	{
		//factorial();

		//preorder_traverse();

		//inorder_traverse();

		//postorder_traverse();
	}
}
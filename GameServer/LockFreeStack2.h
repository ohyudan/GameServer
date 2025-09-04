#pragma once

template <typename T>
class LockFreeStack2
{
	struct Node {
		Node(const T& value) : data(make_shared<T>(value)), next(nullptr) {

		}
		shared_ptr<T> data;
		shared_ptr<Node> next;
	};
public:
	
	void Push(const T& value) {

		shared_ptr<Node> node = make_shared<Node>(value);
		node->next = std::atomic_load(&_head);


		while (std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
		{
		};

		//while (_head.compare_exchange_weak(node->next, node) == false)// 아토믹에서 제공하는 함수임 head가 아토믹이 아니라 못씀		

	}
	
	shared_ptr<T> TryPop() {
		
		//shared_ptr<Node> oldHead = _head; shared_ptr는 아토믹이 아니라 원자적으로 이뤄지지않음.
		shared_ptr<Node> oldHead = std::atomic_load(&_head);

		while(oldHead && std::atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false)
		{
		};
		if (oldHead == nullptr) {
			return shared_ptr<T>();
		}
		return oldHead->data;
	}
	
private:
	shared_ptr<Node> _head;
};



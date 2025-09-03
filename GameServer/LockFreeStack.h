#pragma once

template <typename T>
class LockFreeStack
{
	struct Node {
		Node(const T& value) : data(value) {

		}
		T data;
		Node* next;
	};


public:
	//1) 새노드를 만든다
	//2) 새노드의 next를 현재 head로 설정한다.
	//3) head를 새노드

	void Push(const T& value) {

		Node* node = new Node(value);
		node->next = _head;


		/*if (_head == node->next)
		{
			_head = node;
			return true;
		}else{
			node->next = _head;
			return false;
		}*/


		while (_head.compare_exchange_weak(node->next, node) == false)
		{

		};

	}

	//1) head를 읽는다.
	//2  head->next를 읽는다.
	//3) head를 head->next로 변경한다.
	//4) data를 반환한다.
	//5) 노드를 삭제한다.
	bool TryPop(T& value) {
		Node* oldHead = _head;


		/*if (_head == oldHead) {
			_head = oldHead->next;
			return true;
		}
		else {
			oldHead = _head;
			return false;
		}*/
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{

		};

		if (oldHead == nullptr)
			return false;

		value = oldHead->data;

		//delete oldHead;
		//GC 없음. 스마트포인터?...사용?
		return true;
	}

private:
	atomic<Node*> _head;
};


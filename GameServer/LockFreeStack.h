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
	//1) ����带 �����
	//2) ������� next�� ���� head�� �����Ѵ�.
	//3) head�� �����

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

	//1) head�� �д´�.
	//2  head->next�� �д´�.
	//3) head�� head->next�� �����Ѵ�.
	//4) data�� ��ȯ�Ѵ�.
	//5) ��带 �����Ѵ�.
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
		//GC ����. ����Ʈ������?...���?
		return true;
	}

private:
	atomic<Node*> _head;
};


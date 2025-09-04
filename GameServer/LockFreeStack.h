#pragma once

template <typename T>
class LockFreeStack
{
	struct Node {
		Node(const T& value) : data(value),next(nullptr) {

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
		++_popCount;
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
			--_popCount;
			return false;
		};

		if (oldHead == nullptr)
			return false;

		value = oldHead->data;


		//delete oldHead;	
		return true;
	}
	// ���� ���� �ʿ�
	// 1) �����ͺи�
	//2) Count üũ
	//3) Count�� 0�̸� ����
	void TryDelete(Node* oldHead) {
			//�� ���� �ֳ�
		if (_popCount == 1) {
						
			Node* node = _pendingList.exchange(nullptr);
			if (--_popCount == 0) {
				//����� �ְ� ���� -> ����
				//���� �ͼ� ����� ,��¼�� �����ʹ� �и��ص� ����
				DeleteNodes(node);
			}else if(node){
				//���ȯ
				ChainPendingNodeList(node);
			}

			delete oldHead;
		}
		else {
					//���� �ִ�. �׷� ���� �������� ���߿�	
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	static void DeleteNodes(Node* node) {
		
while (node) {
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

	void CainPendingNodeList(Node* first, Node* last) {
				last->next = _pendingList;
		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{
		};
	}

	void ChainPendingNodeList(Node* node) {
		Node* last = node;
		while(last->next) {
			last = last->next;
		}
		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node) {
		ChainPendingNodeList(node, node);
	}
private:
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0; //Pop�� �������� ������ ����

	atomic<Node*> _pendingList; // �����Ǿߵ� ����
};


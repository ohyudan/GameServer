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
	// 순서 보장 필요
	// 1) 데이터분리
	//2) Count 체크
	//3) Count가 0이면 삭제
	void TryDelete(Node* oldHead) {
			//나 말고 있나
		if (_popCount == 1) {
						
			Node* node = _pendingList.exchange(nullptr);
			if (--_popCount == 0) {
				//끼어든 애가 없음 -> 삭제
				//이제 와서 끼어들어도 ,어쩌피 데이터는 분리해둔 상태
				DeleteNodes(node);
			}else if(node){
				//재반환
				ChainPendingNodeList(node);
			}

			delete oldHead;
		}
		else {
					//누가 있다. 그럼 지금 삭제말고 나중에	
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

	atomic<uint32> _popCount = 0; //Pop을 실행중인 쓰레드 개수

	atomic<Node*> _pendingList; // 삭제되야될 노드들
};


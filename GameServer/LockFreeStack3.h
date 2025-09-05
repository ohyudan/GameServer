#pragma once

template <typename T>
class LockFreeStack3
{
	struct Node;

	struct CountedNodePtr {
		int32 externalCount = 0; //32 //16
		Node* ptr = nullptr;//64 //48 
	};

	struct Node {
		Node(const T& value) : data(make_shared<T>(value))
		{

		}
		shared_ptr<T> data;
		atomic<int32> internalCount = 0;
		CountedNodePtr next;
	};
public:

	void Push(const T& value) {
		CountedNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;

		//node.ptr->next = std::atomic_load(&_head);
		node.ptr->next = _head;
		while (std::atomic_compare_exchange_weak(&_head, &node.ptr->next, node) == false)
		{

		}

	}


	shared_ptr<T> TryPop() {

		//CountedNodePtr oldHead = std::atomic_load(&_head);
		CountedNodePtr oldHead = _head;
		while (true)
		{
			// 참조권 획득
			IncreaseHeadCount(oldHead);
			//최소한 externalCount >= 2 삭제X 
			Node* const ptr = oldHead.ptr;

			if(ptr == nullptr) {
				return shared_ptr<T>();
			}
			if (_head.compare_exchange_strong(oldHead, ptr->next))
			{
				shared_ptr<T> res;
				res.swap(ptr->data);

				//externalCount: 1 -> 2(+1) ->4(나 +1 남+2)
				//internalCount: 0

				//나말고 또 있나
				int32 const countIncrease = oldHead.externalCount - 2;
				// 더하기 이전의 값
				if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
				{
					delete ptr;
				}
				return res;
			}
			else if (ptr->internalCount.fetch_sub(1) == 1)
			{
				//참조권은 얻음 
				delete ptr;
			}
		}
	}
private:
	void IncreaseHeadCount(CountedNodePtr & oldCounter) {
			
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			newCounter.externalCount++;

			if (_head.compare_exchange_strong(oldCounter, newCounter))
			{
				break;
			}
		}
	}

private:
	shared_ptr<Node> _head;
};



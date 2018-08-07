#include "MEMORY.h"
void MemoryManager::InitList(PLIST pList, int iMemory)
{
	pList->iSeries = 0;

	pList->pHead = new NODE;
	pList->pHead->tMemory.iStart = pList->pHead->tMemory.iSize = 0;
	pList->pHead->pNext = new NODE;
	pList->pHead->pNext->tMemory.iStart = 0;
	pList->pHead->pNext->tMemory.iSize = (iMemory);

	pList->pTail = new NODE;
	pList->pTail->tMemory.iStart = pList->pTail->tMemory.iSize = 0;
	pList->pTail->pPrev = pList->pHead->pNext;

	pList->pHead->pNext->pPrev = pList->pHead;
	pList->pTail->pPrev->pNext = pList->pTail;

	pList->pHead->pPrev = NULL;
	pList->pTail->pNext = NULL;
}
void MemoryManager::Combine()
{
	PNODE TempAvail = Avail->pHead->pNext; // 탐색을 위한 임시 노드

	while (true)
	{
		// 해당 메모리 노드의 다음 메모리 노드가
		// 마지막 노드가 아닐 경우
		if (TempAvail->pNext != Avail->pTail)
		{
			// 해당 메모리 노드의 주소값과 
			// 해당 메모리 노드의 메모리 크기의 합이
			// 해당 메모리 노드의 다음 메모리 노드의 주소값과 같을 경우
			if ((TempAvail->tMemory.iStart + TempAvail->tMemory.iSize) 
				== TempAvail->pNext->tMemory.iStart)
			{
				// 해당 메모리 노드의 메모리 크기를 
				// 다음 메모리 노드의 메모리 크기만큼 증가
				// 다음 메모리 노드 삭제
				TempAvail->tMemory.iSize += TempAvail->pNext->tMemory.iSize;
				TempAvail->pNext = TempAvail->pNext->pNext;
				TempAvail->pNext->pPrev = TempAvail;
				// 노드의 갯수를 나타내는 변수 증가
				--Avail->iSeries;
				// 처음으로 돌아가서 재탐색
				TempAvail = Avail->pHead->pNext;
			}
			else
			{
				TempAvail = TempAvail->pNext;
			}
		}
		// 해당 메모리 노드의 다음 노드가
		// 마지막 노드일 경우
		else
			break;
	}
}
void MemoryManager::AvailInsert(PNODE pNode)
{	
	// 노드의 갯수를 나타내는 변수가 음수
	// 즉 모든 메모리 공간을 사용중일 경우
	if (Avail->iSeries < 0)
	{
		// 반환 노드를 삽입
		pNode->pNext = Avail->pTail;
		pNode->pPrev = Avail->pHead;
		Avail->pHead->pNext = pNode;
		Avail->pTail->pPrev = pNode;
		// 노드의 갯수를 나타내는 변수 증가
		++Avail->iSeries;
		return;
	}
	// 최초의 상태와 같을 경우
	if (Avail->iSeries == 0)
	{
		PNODE TempAvail = Avail->pHead->pNext; // 탐색을 위한 임시 노드
		// 반환 노드의 주소값이 해당 메모리 노드의 주소값보다 클 경우
		if (pNode->tMemory.iStart > TempAvail->tMemory.iStart)
		{
			// 해당 메모리 노드의 뒤에 노드를 삽입
			pNode->pNext = TempAvail->pNext;
			pNode->pPrev = TempAvail;
			TempAvail->pNext->pPrev = pNode;
			TempAvail->pNext = pNode;
			// 노드의 갯수를 나타내는 변수 증가
			++Avail->iSeries;
			// 메모리가 합병 가능한지 판단
			Combine();
			return;
		}
		// 반환 노드의 주소값이 해당 메모리 노드의 주소값보다 작을 경우
		else
		{
			// 해당 메모리 노드의 앞에 삽입
			pNode->pNext = TempAvail;
			pNode->pPrev = TempAvail->pPrev;
			TempAvail->pPrev->pNext = pNode;
			TempAvail->pPrev = pNode;
			// 노드의 갯수를 나타내는 변수 증가
			++Avail->iSeries;
			// 메모리가 합병 가능한지 판단
			Combine();
			return;
		}
	}
	// 노드가 여러 개
	else
	{
		PNODE TempAvail = Avail->pTail->pPrev; // 탐색을 위한 노드, 마지막 부터 탐색
		while (true)
		{
			// 반환 노드의 주소값이 해당 메모리 노드의 주소값보다 작으며
			// 반환 노드의 주소값이 해당 메모리 노드 이전 메모리 노드의 주소값보다 작을 경우
			if (pNode->tMemory.iStart < TempAvail->tMemory.iStart 
				&& pNode->tMemory.iStart < TempAvail->pPrev->tMemory.iStart)
			{
				// 이전 노드로 이동
				TempAvail = TempAvail->pPrev;
				// 가장 처음 노드일 경우
				if (TempAvail == Avail->pHead)
				{
					// 해당 메모리 노드 다음에
					// 반환 노드 삽입
					pNode->pNext = TempAvail->pNext;
					pNode->pPrev = TempAvail;
					TempAvail->pNext->pPrev = pNode;
					TempAvail->pNext = pNode;
					// 노드 갯수를 나타내는 변수 증가
					++Avail->iSeries;
					// 메모리가 합병 가능한지 판단
					Combine();
					break;
				}
			}
			// 반환 노드의 주소값이 해당 메모리 노드의 주소값보다 작으며
			// 반환 노드의 주소값이 해당 메모리 노드의 이전 메모리 노드의 주소값보다 클 경우
			else if (pNode->tMemory.iStart < TempAvail->tMemory.iStart 
				&& pNode->tMemory.iStart > TempAvail->pPrev->tMemory.iStart)
			{
				// 해당 메모리 노드와 이전 메모리 노드의
				// 사이에 반환 노드를 삽입
				pNode->pNext = TempAvail;
				pNode->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = pNode;
				TempAvail->pPrev = pNode;
				// 노드 갯수를 나타내는 변수 증가
				++Avail->iSeries;
				// 메모리가 합병 가능한지 판단
				Combine();
				break;
			}
			// 해당 메모리 노드의 이전 메모리 노드가 헤드 노드 일 경우
			else if (TempAvail->pPrev == Avail->pHead)
			{
				// 가장 처음에 반환 노드 삽입
				pNode->pNext = TempAvail;
				pNode->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = pNode;
				TempAvail->pPrev = pNode;
				// 노드 갯수를 나타내는 변수 증가
				++Avail->iSeries;
				// 메모리가 합병 가능한지 판단
				Combine();
				break;
			}
		}
	}
}
void MemoryManager::UsedInsert(PNODE pNode)
{
	PNODE TempUsed = Used->pHead->pNext; // 탐색을 위한 임시 노드

	// Used리스트에 아무것도 없을 경우
	// Used 리스트 초기화 시 주소값과 메모리크기가 모두 0인 노드가 1개
	if (TempUsed->tMemory.iSize == 0 && TempUsed->tMemory.iStart == 0)
	{
		// 해당 메모리 노드의 주소값에 할당 정보 노드의 주소값 삽입
		// 해당 메모리 노드의 메모리 크기에 할당 정보 노드의 메모리 크기 삽입
		TempUsed->tMemory.iSize = pNode->tMemory.iSize;
		TempUsed->tMemory.iStart = pNode->tMemory.iStart;

		// 노드 갯수를 나타내는 변수 증가
		++Used->iSeries;
	}

	// Used리스트에 노드가 존재 할 경우
	else
	{
		while (true)
		{
			// 할당 정보 노드의 주소값이
			// 해당 메모리 노드의 주소값보다 작을 경우
			if (pNode->tMemory.iStart < TempUsed->tMemory.iStart)
			{
				// 해당 메모리 노드의 앞에 삽입
				pNode->pNext = TempUsed;
				pNode->pPrev = TempUsed->pPrev;
				TempUsed->pPrev->pNext = pNode;
				TempUsed->pPrev = pNode;

				// 노드 갯수를 나타내는 변수 증가
				++Used->iSeries;
				break;
			}

			// 해당 메모리 노드가 리스트의 마지막일 경우
			else if (TempUsed == Used->pTail)
			{
				// 리스트의 마지막 앞에
				// 할당 정보 노드 삽입
				pNode->pNext = TempUsed;
				pNode->pPrev = TempUsed->pPrev;
				TempUsed->pPrev->pNext = pNode;
				TempUsed->pPrev = pNode;

				// 노드 갯수를 나타내는 변수 증가
				++Used->iSeries;
				break;
			}
			
			// 할당 정보 노드의 주소값이
			// 해당 메모리 노드의 주소값보다 클 경우
			// 다음 노드로 이동
			TempUsed = TempUsed->pNext;
		}
	}
}
void MemoryManager::AllocFail_WrongInput()
{
	cout << "Alloc Fail !! " << endl;
	cout << "Request size is Wrong !! " << endl;
}
void MemoryManager::AllocSuccess()
{
	cout << "Alloc Success !!" << endl;
}
void MemoryManager::AllocFail_Memory()
{
	cout << "Alloc Fail !! " << endl;
	cout << "InSufficent Memory" << endl;
}
void MemoryManager::FreeSuccess()
{
	cout << "Free Success !!" << endl;
}
void MemoryManager::FreeFail()
{
	cout << "Free Fail !!" << endl;
	cout << "The Address you are looking for is not in use " << endl;
}
void MemoryManager::AllocFail_DisContinuous()
{
	cout << "Alloc Fail !! " << endl;
	cout << "Memory is DIsContinuous" << endl;
}
bool MemoryManager::JudgeAlloc(int Request_size)
{
	PNODE TempAvail = Avail->pHead->pNext; // 탐색을 위한 임시 노드

	// 요청메모리가 0이거나 음수 일경우
	if (Request_size == 0 || Request_size < 0)
	{
		cout << "메모리 " << Request_size << "할당" << endl;
		AllocFail_WrongInput(); // 잘못된 할당 요청으로 인한 할당 실패
		cout << endl;
		return false; // false 반환
	}
	// 요청메모리가 가용메모리 공간보다 클 시
	if (Request_size > iMemory) 
	{
		cout << "메모리 " << Request_size << "할당" << endl;
		AllocFail_Memory(); // 메모리부족으로 인한 할당 실패 메시지 출력
		cout << endl;
		return false; // false 반환
	}	
	// 요청메모리가 해당 메모리 노드의 메모리 공간보다 작거나 같을 경우
	else if (TempAvail->tMemory.iSize >= Request_size) 
	{
		AllocSuccess(); // 할당 성공 메시지 출력
		cout << "메모리 " << Request_size << "할당" << endl;
		cout << endl;
		return true; // true 반환
	}

	// 요청메모리가 전체메모리 보다 작거나 같지만 비연속적인 부분이 있을 경우
	else if (iMemory >= Request_size && Avail->iSeries != 0)
	{
		while (true)
		{
			// 요청메모리가 해당 메모리 노드의 메모리 공간보다 클 경우 
			if (TempAvail->tMemory.iSize < Request_size)
			{
				// 다음 노드로 이동
				TempAvail = TempAvail->pNext;

				// 마지막 노드 일 경우
				if (TempAvail == Avail->pTail)
				{
					cout << "메모리 " << Request_size << "할당" << endl;
					AllocFail_DisContinuous(); // 비연속적인 메모리로 인한 할당 실패
					return false; // false 반환
				}

				// 요청메모리가 해당 메모리 노드의 메모리 공간보다 작거나 같을 경우
				if (TempAvail->tMemory.iSize >= Request_size)
				{
					AllocSuccess(); // 할당 성공
					return true; // true 반환
				}
			}
		}
	}
}
bool MemoryManager::JudgeFree(Addr Free)
{
	PNODE TempUsed = Used->pHead->pNext; // 탐색을 위한 임시 노드
	while (true)
	{
		// 해당 메모리 노드의 주소값이 반환 노드의 주소값과 같을 경우
		if (TempUsed->tMemory.iStart == Free->tMemory.iStart)
		{
			FreeSuccess(); // 반환 성공 메시지
			cout << "주소 " << Free->tMemory.iStart << "반환" << endl << endl;
			return true; // true 반환
		}

		// 해당 메모리 노드의 주소값이 반환 노드의 주소값과 다를 경우
		else
		{
			// 다음 노드로 이동
			TempUsed = TempUsed->pNext;

			// 리스트의 마지막 노드일 경우
			if (TempUsed == Used->pTail)
			{
				cout << "주소 " << Free->tMemory.iStart << "반환" << endl;
				FreeFail(); // 해당 주소값이 없으므로 반환 실패 메시지
				return false; // false 반환
			}
		}
	}
}
Addr MemoryManager::First_Fit_MyAlloc(int Request_size)
{
	PNODE pNode = new NODE; // 할당된 메모리의 정보를 담을 노드
	PNODE DeleteNode = new NODE; // 메모리 노드의 메모리가 0이 될 경우 삭제를 위한 노드
	PNODE TempAvail = Avail->pHead->pNext; // 탐색을 위한 임시 노드

	// JudgeAlloc함수가 false를 반환할 경우
	if (!JudgeAlloc(Request_size))
	{
		pNode->tMemory.iSize = -1; 
		pNode->tMemory.iStart = -1;
		return pNode; // -1의 정보를 가진 메모리 노드를 반환
	}

	// JudgeAlloc함수가 true를 반환할 경우
	else
	{
		while (true)
		{
			// 요청 메모리가 해당 메모리 노드의 메모리 공간보다 클 경우
			if (Request_size > TempAvail->tMemory.iSize)
			{
				TempAvail = TempAvail->pNext; // 다음 노드로 이동
			}

			// 요청 메모리가 해당 메모리 노드의 메모리 공간보다 작거나 같을 경우
			else
			{
				// 할당정보 노드의 주소값에 해당 메모리 노드의 주소값 삽입
				// 할당정보 노드의 메모리 공간에 요청메모리 크기만큼 삽입
				pNode->tMemory.iStart = TempAvail->tMemory.iStart;
				pNode->tMemory.iSize = Request_size;

				// 해당 메모리 노드의 주소값을 요청메모리 크기만큼 증가
				// 해당 메모리 노드의 메모리 공간을 요청메모리 크기만큼 감소
				TempAvail->tMemory.iStart += Request_size;
				TempAvail->tMemory.iSize -= Request_size;

				// 전체 메모리 공간을 요청메모리 크기만큼 감소
				iMemory -= Request_size;

				// 해당 메모리 노드의 메모리 공간이 0이 될경우
				if (TempAvail->tMemory.iSize == 0)
				{

					// 리스트에서 해당 메모리 노드 삭제
					DeleteNode = TempAvail;
					TempAvail->pNext->pPrev = TempAvail->pPrev;
					TempAvail->pPrev->pNext = TempAvail->pNext;

					// 노드 개수를 나타내는 변수 감소
					--Avail->iSeries;
				}

				// 할당 후 남은 메모리 노드끼리 합병 가능한가 확인
				Combine();
				break;
			}
		}
	}
	delete DeleteNode; // 삭제노드 삭제
	UsedInsert(pNode); // Used 리스트에 할당정보 노드 삽입
	return pNode; // 할당정보 노드 반환
}
Addr MemoryManager::Best_Fit_MyAlloc(int Request_size)
{
	PNODE pNode = new NODE; // 할당된 메모리의 정보를 담을 노드
	PNODE DeleteNode = new NODE; // 메모리 노드의 메모리가 0이 될 경우 삭제를 위한 노드
	PNODE TempAvail = Avail->pHead->pNext; // 탐색을 위한 노드
	int iMin = 100; // 차이를 비교하기 위한 변수

	// JudgeAlloc이 false를 반환할 경우
	if (!JudgeAlloc(Request_size))
	{
		pNode->tMemory.iSize = -1;
		pNode->tMemory.iStart = -1;
		return pNode; // -1의 정보를 담은 메모리 노드 반환
	}

	// JudgeAlloc이 true를 반환할 경우
	else
	{
		// 가용 메모리공간이 연속적 즉, 메모리 노드가 1개 일 경우
		if (Avail->iSeries == 0)
		{
			// 할당정보 노드의 주소값에 해당 노드의 주소값 삽입
			// 할당정보 노드의 메모리 크기에 요청 메모리 크기 삽입
			pNode->tMemory.iStart = TempAvail->tMemory.iStart;
			pNode->tMemory.iSize = Request_size;

			// 해당 노드의 주소값을 요청 메모리 크기만큼 증가
			// 해당 노드의 메모리 크기를 요청 메모리 크기만큼 감소
			TempAvail->tMemory.iStart += Request_size;
			TempAvail->tMemory.iSize -= Request_size;

			// 전체 메모리 공간을 요청 메모리 크기만큼 감소
			iMemory -= Request_size;

			// 해당 노드의 메모리 크기가 0이 될 경우
			if (TempAvail->tMemory.iSize == 0)
			{
				// 리스트에서 해당 노드 삭제
				DeleteNode = TempAvail; 
				TempAvail->pNext->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = TempAvail->pNext;

				// 노드 갯수를 나타내는 변수 감소
				--Avail->iSeries; 
			}
			UsedInsert(pNode); // Used 리스트에 삽입
			return pNode; // 할당정보 노드를 반환
		}

		// 메모리공간이 비연속적일 경우
		else
		{
			while (true)
			{
				// 요청 메모리의 크기가 해당 노드의 메모리 크기보다 클 경우
				if (TempAvail->tMemory.iSize < Request_size)
				{
					// 다음 노드로 이동
					TempAvail = TempAvail->pNext;
				}

				// 요청 메모리의 크기가 해당 노드의 메모리 크기보다 작거나 같을 경우
				else 
				{
					// 차이를 나타내는 변수 보다
					// 해당 노드의 메모리 크기와 요청 메모리의 차이가 작을 경우
					if (iMin > TempAvail->tMemory.iSize - Request_size)
					{
						// 차이를 나타내는 변수에 삽입
						iMin = TempAvail->tMemory.iSize - Request_size;
					}

					// 다음 노드로 이동
					TempAvail = TempAvail->pNext;

					// 마지막 노드까지 탐색을 했을 경우
					if (TempAvail == Avail->pTail)
					{
						break; // 반복문 중단
					}
				}
			}
		}

		// 리스트의 처음으로 이동
		TempAvail = Avail->pHead->pNext;
		while (true)
		{
			// 차이를 나타내는 변수와
			// 해당 노드의 메모리 크기와 요청 메모리의 차이가 같을 경우
			if (iMin == TempAvail->tMemory.iSize - Request_size)
			{
				// 할당 정보 노드의 주소값에 해당 노드의 주소값 삽입
				// 할당 정보 노드의 메모리 크기에 요청 메모리를 삽입
				pNode->tMemory.iStart = TempAvail->tMemory.iStart;
				pNode->tMemory.iSize = Request_size;

				// 해당 메모리 노드의 주소값을 요청 메모리 만큼 증가
				// 해당 메모리 노드의 메모리 크기를 요청 메모리 만큼 감소
				TempAvail->tMemory.iStart += Request_size;
				TempAvail->tMemory.iSize -= Request_size;

				// 전체 메모리 공간을 요청 메모리 만큼 감소
				iMemory -= Request_size;

				// 해당 메모리 노드의 메모리 크기가 0이 될 경우
				if (TempAvail->tMemory.iSize == 0)
				{
					// 해당 메모리 노드 삭제
					DeleteNode = TempAvail;
					TempAvail->pNext->pPrev = TempAvail->pPrev;
					TempAvail->pPrev->pNext = TempAvail->pNext;

					// 노드 갯수를 나타내는 변수 감소
					--Avail->iSeries;
				}

				// 할당 후 남은 노드끼리 합병이 가능한가 판단
				Combine();
				break;
			}

			// 차이를 나타내는 변수와
			// 해당 노드의 메모리 크기와 요청 메모리의 차이가 다를 경우
			else
				TempAvail = TempAvail->pNext; // 다음 노드로 이동
		}
	}
	delete DeleteNode; // 삭제노드 삭제
	UsedInsert(pNode); // Used 리스트에 할당정보 노드 삽입
	return pNode; // 할당정보 노드 반환
}
void MemoryManager::MyFree(Addr FreeNode)
{
	PNODE TempUsed = Used->pHead->pNext; // 탐색을 위한 임시 노드

	// JudgeFree 함수가 false를 반환할 경우
	if (!JudgeFree(FreeNode))
	{
		return; // 함수 종료
	}

	// JudgeFree 함수가 true를 반환할 경우
	else
	{
		while (true)
		{
			// 해당 메모리 노드의 주소값과 반환 노드의 주소값이 같을 경우
			if (TempUsed->tMemory.iStart == FreeNode->tMemory.iStart)
			{
				// 해당 메모리 노드 삭제
				TempUsed->pPrev->pNext = TempUsed->pNext;
				TempUsed->pNext->pPrev = TempUsed->pPrev;

				// 전체 메모리 공간을 반환 노드의 메모리 크기만큼 증가
				iMemory += FreeNode->tMemory.iSize;

				// 노드 갯수를 나타내는 변수 감소
				--Used->iSeries;

				// 노드 갯수가 0이 될 경우
				if (Used->iSeries == 0)
				{					
					InitList(Used, 0); // Used 리스트 재 초기화
				}
				break;
			}

			// 해당 메모리 노드의 주소값과 반환 노드의 주소값이 다를 경우
			// 다음 노드로 이동
			TempUsed = TempUsed->pNext;
		}
	}
	
	//반환 노드를 Avail 리스트에 삽입
	AvailInsert(FreeNode); 
}
void MemoryManager::OutputMemory()
{
	PNODE TempUsed = Used->pHead->pNext;
	PNODE TempAvail = Avail->pHead->pNext;
	cout << "Avail Memory" << endl;
	while (true)
	{
		if (Avail->iSeries == 0 || Avail->iSeries <0)
		{
			if (Avail->iSeries < 0)
			{
				cout << "\tAll Memory is using !!" << endl;
				break;
			}
			cout << "\tStart_Address : " << Avail->pHead->pNext->tMemory.iStart << "\t" << "Size : " << Avail->pHead->pNext->tMemory.iSize << endl << endl;
			break;
		}
		else
		{
			cout << "\tStart_Address : " << TempAvail->tMemory.iStart << "\t" << "Size : " << TempAvail->tMemory.iSize << " ----->" << endl << endl;
			TempAvail = TempAvail->pNext;
			if (TempAvail == Avail->pTail)
				break;
		}
	}

	cout << "Used Memory" << endl;
	while (true)
	{
		if (Used->iSeries == 0)
		{
			cout << "\tNot Used Address" << endl << endl;
			break;
		}
		else
		{
			cout << "\tStart_Address : " << TempUsed->tMemory.iStart << "\t" << "Size : " << TempUsed->tMemory.iSize << " ----->" << endl << endl;
			TempUsed = TempUsed->pNext;
			if (TempUsed == Used->pTail)
				break;
		}
	}
}
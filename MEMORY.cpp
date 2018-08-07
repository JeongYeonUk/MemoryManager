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
	PNODE TempAvail = Avail->pHead->pNext; // Ž���� ���� �ӽ� ���

	while (true)
	{
		// �ش� �޸� ����� ���� �޸� ��尡
		// ������ ��尡 �ƴ� ���
		if (TempAvail->pNext != Avail->pTail)
		{
			// �ش� �޸� ����� �ּҰ��� 
			// �ش� �޸� ����� �޸� ũ���� ����
			// �ش� �޸� ����� ���� �޸� ����� �ּҰ��� ���� ���
			if ((TempAvail->tMemory.iStart + TempAvail->tMemory.iSize) 
				== TempAvail->pNext->tMemory.iStart)
			{
				// �ش� �޸� ����� �޸� ũ�⸦ 
				// ���� �޸� ����� �޸� ũ�⸸ŭ ����
				// ���� �޸� ��� ����
				TempAvail->tMemory.iSize += TempAvail->pNext->tMemory.iSize;
				TempAvail->pNext = TempAvail->pNext->pNext;
				TempAvail->pNext->pPrev = TempAvail;
				// ����� ������ ��Ÿ���� ���� ����
				--Avail->iSeries;
				// ó������ ���ư��� ��Ž��
				TempAvail = Avail->pHead->pNext;
			}
			else
			{
				TempAvail = TempAvail->pNext;
			}
		}
		// �ش� �޸� ����� ���� ��尡
		// ������ ����� ���
		else
			break;
	}
}
void MemoryManager::AvailInsert(PNODE pNode)
{	
	// ����� ������ ��Ÿ���� ������ ����
	// �� ��� �޸� ������ ������� ���
	if (Avail->iSeries < 0)
	{
		// ��ȯ ��带 ����
		pNode->pNext = Avail->pTail;
		pNode->pPrev = Avail->pHead;
		Avail->pHead->pNext = pNode;
		Avail->pTail->pPrev = pNode;
		// ����� ������ ��Ÿ���� ���� ����
		++Avail->iSeries;
		return;
	}
	// ������ ���¿� ���� ���
	if (Avail->iSeries == 0)
	{
		PNODE TempAvail = Avail->pHead->pNext; // Ž���� ���� �ӽ� ���
		// ��ȯ ����� �ּҰ��� �ش� �޸� ����� �ּҰ����� Ŭ ���
		if (pNode->tMemory.iStart > TempAvail->tMemory.iStart)
		{
			// �ش� �޸� ����� �ڿ� ��带 ����
			pNode->pNext = TempAvail->pNext;
			pNode->pPrev = TempAvail;
			TempAvail->pNext->pPrev = pNode;
			TempAvail->pNext = pNode;
			// ����� ������ ��Ÿ���� ���� ����
			++Avail->iSeries;
			// �޸𸮰� �պ� �������� �Ǵ�
			Combine();
			return;
		}
		// ��ȯ ����� �ּҰ��� �ش� �޸� ����� �ּҰ����� ���� ���
		else
		{
			// �ش� �޸� ����� �տ� ����
			pNode->pNext = TempAvail;
			pNode->pPrev = TempAvail->pPrev;
			TempAvail->pPrev->pNext = pNode;
			TempAvail->pPrev = pNode;
			// ����� ������ ��Ÿ���� ���� ����
			++Avail->iSeries;
			// �޸𸮰� �պ� �������� �Ǵ�
			Combine();
			return;
		}
	}
	// ��尡 ���� ��
	else
	{
		PNODE TempAvail = Avail->pTail->pPrev; // Ž���� ���� ���, ������ ���� Ž��
		while (true)
		{
			// ��ȯ ����� �ּҰ��� �ش� �޸� ����� �ּҰ����� ������
			// ��ȯ ����� �ּҰ��� �ش� �޸� ��� ���� �޸� ����� �ּҰ����� ���� ���
			if (pNode->tMemory.iStart < TempAvail->tMemory.iStart 
				&& pNode->tMemory.iStart < TempAvail->pPrev->tMemory.iStart)
			{
				// ���� ���� �̵�
				TempAvail = TempAvail->pPrev;
				// ���� ó�� ����� ���
				if (TempAvail == Avail->pHead)
				{
					// �ش� �޸� ��� ������
					// ��ȯ ��� ����
					pNode->pNext = TempAvail->pNext;
					pNode->pPrev = TempAvail;
					TempAvail->pNext->pPrev = pNode;
					TempAvail->pNext = pNode;
					// ��� ������ ��Ÿ���� ���� ����
					++Avail->iSeries;
					// �޸𸮰� �պ� �������� �Ǵ�
					Combine();
					break;
				}
			}
			// ��ȯ ����� �ּҰ��� �ش� �޸� ����� �ּҰ����� ������
			// ��ȯ ����� �ּҰ��� �ش� �޸� ����� ���� �޸� ����� �ּҰ����� Ŭ ���
			else if (pNode->tMemory.iStart < TempAvail->tMemory.iStart 
				&& pNode->tMemory.iStart > TempAvail->pPrev->tMemory.iStart)
			{
				// �ش� �޸� ���� ���� �޸� �����
				// ���̿� ��ȯ ��带 ����
				pNode->pNext = TempAvail;
				pNode->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = pNode;
				TempAvail->pPrev = pNode;
				// ��� ������ ��Ÿ���� ���� ����
				++Avail->iSeries;
				// �޸𸮰� �պ� �������� �Ǵ�
				Combine();
				break;
			}
			// �ش� �޸� ����� ���� �޸� ��尡 ��� ��� �� ���
			else if (TempAvail->pPrev == Avail->pHead)
			{
				// ���� ó���� ��ȯ ��� ����
				pNode->pNext = TempAvail;
				pNode->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = pNode;
				TempAvail->pPrev = pNode;
				// ��� ������ ��Ÿ���� ���� ����
				++Avail->iSeries;
				// �޸𸮰� �պ� �������� �Ǵ�
				Combine();
				break;
			}
		}
	}
}
void MemoryManager::UsedInsert(PNODE pNode)
{
	PNODE TempUsed = Used->pHead->pNext; // Ž���� ���� �ӽ� ���

	// Used����Ʈ�� �ƹ��͵� ���� ���
	// Used ����Ʈ �ʱ�ȭ �� �ּҰ��� �޸�ũ�Ⱑ ��� 0�� ��尡 1��
	if (TempUsed->tMemory.iSize == 0 && TempUsed->tMemory.iStart == 0)
	{
		// �ش� �޸� ����� �ּҰ��� �Ҵ� ���� ����� �ּҰ� ����
		// �ش� �޸� ����� �޸� ũ�⿡ �Ҵ� ���� ����� �޸� ũ�� ����
		TempUsed->tMemory.iSize = pNode->tMemory.iSize;
		TempUsed->tMemory.iStart = pNode->tMemory.iStart;

		// ��� ������ ��Ÿ���� ���� ����
		++Used->iSeries;
	}

	// Used����Ʈ�� ��尡 ���� �� ���
	else
	{
		while (true)
		{
			// �Ҵ� ���� ����� �ּҰ���
			// �ش� �޸� ����� �ּҰ����� ���� ���
			if (pNode->tMemory.iStart < TempUsed->tMemory.iStart)
			{
				// �ش� �޸� ����� �տ� ����
				pNode->pNext = TempUsed;
				pNode->pPrev = TempUsed->pPrev;
				TempUsed->pPrev->pNext = pNode;
				TempUsed->pPrev = pNode;

				// ��� ������ ��Ÿ���� ���� ����
				++Used->iSeries;
				break;
			}

			// �ش� �޸� ��尡 ����Ʈ�� �������� ���
			else if (TempUsed == Used->pTail)
			{
				// ����Ʈ�� ������ �տ�
				// �Ҵ� ���� ��� ����
				pNode->pNext = TempUsed;
				pNode->pPrev = TempUsed->pPrev;
				TempUsed->pPrev->pNext = pNode;
				TempUsed->pPrev = pNode;

				// ��� ������ ��Ÿ���� ���� ����
				++Used->iSeries;
				break;
			}
			
			// �Ҵ� ���� ����� �ּҰ���
			// �ش� �޸� ����� �ּҰ����� Ŭ ���
			// ���� ���� �̵�
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
	PNODE TempAvail = Avail->pHead->pNext; // Ž���� ���� �ӽ� ���

	// ��û�޸𸮰� 0�̰ų� ���� �ϰ��
	if (Request_size == 0 || Request_size < 0)
	{
		cout << "�޸� " << Request_size << "�Ҵ�" << endl;
		AllocFail_WrongInput(); // �߸��� �Ҵ� ��û���� ���� �Ҵ� ����
		cout << endl;
		return false; // false ��ȯ
	}
	// ��û�޸𸮰� ����޸� �������� Ŭ ��
	if (Request_size > iMemory) 
	{
		cout << "�޸� " << Request_size << "�Ҵ�" << endl;
		AllocFail_Memory(); // �޸𸮺������� ���� �Ҵ� ���� �޽��� ���
		cout << endl;
		return false; // false ��ȯ
	}	
	// ��û�޸𸮰� �ش� �޸� ����� �޸� �������� �۰ų� ���� ���
	else if (TempAvail->tMemory.iSize >= Request_size) 
	{
		AllocSuccess(); // �Ҵ� ���� �޽��� ���
		cout << "�޸� " << Request_size << "�Ҵ�" << endl;
		cout << endl;
		return true; // true ��ȯ
	}

	// ��û�޸𸮰� ��ü�޸� ���� �۰ų� ������ �񿬼����� �κ��� ���� ���
	else if (iMemory >= Request_size && Avail->iSeries != 0)
	{
		while (true)
		{
			// ��û�޸𸮰� �ش� �޸� ����� �޸� �������� Ŭ ��� 
			if (TempAvail->tMemory.iSize < Request_size)
			{
				// ���� ���� �̵�
				TempAvail = TempAvail->pNext;

				// ������ ��� �� ���
				if (TempAvail == Avail->pTail)
				{
					cout << "�޸� " << Request_size << "�Ҵ�" << endl;
					AllocFail_DisContinuous(); // �񿬼����� �޸𸮷� ���� �Ҵ� ����
					return false; // false ��ȯ
				}

				// ��û�޸𸮰� �ش� �޸� ����� �޸� �������� �۰ų� ���� ���
				if (TempAvail->tMemory.iSize >= Request_size)
				{
					AllocSuccess(); // �Ҵ� ����
					return true; // true ��ȯ
				}
			}
		}
	}
}
bool MemoryManager::JudgeFree(Addr Free)
{
	PNODE TempUsed = Used->pHead->pNext; // Ž���� ���� �ӽ� ���
	while (true)
	{
		// �ش� �޸� ����� �ּҰ��� ��ȯ ����� �ּҰ��� ���� ���
		if (TempUsed->tMemory.iStart == Free->tMemory.iStart)
		{
			FreeSuccess(); // ��ȯ ���� �޽���
			cout << "�ּ� " << Free->tMemory.iStart << "��ȯ" << endl << endl;
			return true; // true ��ȯ
		}

		// �ش� �޸� ����� �ּҰ��� ��ȯ ����� �ּҰ��� �ٸ� ���
		else
		{
			// ���� ���� �̵�
			TempUsed = TempUsed->pNext;

			// ����Ʈ�� ������ ����� ���
			if (TempUsed == Used->pTail)
			{
				cout << "�ּ� " << Free->tMemory.iStart << "��ȯ" << endl;
				FreeFail(); // �ش� �ּҰ��� �����Ƿ� ��ȯ ���� �޽���
				return false; // false ��ȯ
			}
		}
	}
}
Addr MemoryManager::First_Fit_MyAlloc(int Request_size)
{
	PNODE pNode = new NODE; // �Ҵ�� �޸��� ������ ���� ���
	PNODE DeleteNode = new NODE; // �޸� ����� �޸𸮰� 0�� �� ��� ������ ���� ���
	PNODE TempAvail = Avail->pHead->pNext; // Ž���� ���� �ӽ� ���

	// JudgeAlloc�Լ��� false�� ��ȯ�� ���
	if (!JudgeAlloc(Request_size))
	{
		pNode->tMemory.iSize = -1; 
		pNode->tMemory.iStart = -1;
		return pNode; // -1�� ������ ���� �޸� ��带 ��ȯ
	}

	// JudgeAlloc�Լ��� true�� ��ȯ�� ���
	else
	{
		while (true)
		{
			// ��û �޸𸮰� �ش� �޸� ����� �޸� �������� Ŭ ���
			if (Request_size > TempAvail->tMemory.iSize)
			{
				TempAvail = TempAvail->pNext; // ���� ���� �̵�
			}

			// ��û �޸𸮰� �ش� �޸� ����� �޸� �������� �۰ų� ���� ���
			else
			{
				// �Ҵ����� ����� �ּҰ��� �ش� �޸� ����� �ּҰ� ����
				// �Ҵ����� ����� �޸� ������ ��û�޸� ũ�⸸ŭ ����
				pNode->tMemory.iStart = TempAvail->tMemory.iStart;
				pNode->tMemory.iSize = Request_size;

				// �ش� �޸� ����� �ּҰ��� ��û�޸� ũ�⸸ŭ ����
				// �ش� �޸� ����� �޸� ������ ��û�޸� ũ�⸸ŭ ����
				TempAvail->tMemory.iStart += Request_size;
				TempAvail->tMemory.iSize -= Request_size;

				// ��ü �޸� ������ ��û�޸� ũ�⸸ŭ ����
				iMemory -= Request_size;

				// �ش� �޸� ����� �޸� ������ 0�� �ɰ��
				if (TempAvail->tMemory.iSize == 0)
				{

					// ����Ʈ���� �ش� �޸� ��� ����
					DeleteNode = TempAvail;
					TempAvail->pNext->pPrev = TempAvail->pPrev;
					TempAvail->pPrev->pNext = TempAvail->pNext;

					// ��� ������ ��Ÿ���� ���� ����
					--Avail->iSeries;
				}

				// �Ҵ� �� ���� �޸� ��峢�� �պ� �����Ѱ� Ȯ��
				Combine();
				break;
			}
		}
	}
	delete DeleteNode; // ������� ����
	UsedInsert(pNode); // Used ����Ʈ�� �Ҵ����� ��� ����
	return pNode; // �Ҵ����� ��� ��ȯ
}
Addr MemoryManager::Best_Fit_MyAlloc(int Request_size)
{
	PNODE pNode = new NODE; // �Ҵ�� �޸��� ������ ���� ���
	PNODE DeleteNode = new NODE; // �޸� ����� �޸𸮰� 0�� �� ��� ������ ���� ���
	PNODE TempAvail = Avail->pHead->pNext; // Ž���� ���� ���
	int iMin = 100; // ���̸� ���ϱ� ���� ����

	// JudgeAlloc�� false�� ��ȯ�� ���
	if (!JudgeAlloc(Request_size))
	{
		pNode->tMemory.iSize = -1;
		pNode->tMemory.iStart = -1;
		return pNode; // -1�� ������ ���� �޸� ��� ��ȯ
	}

	// JudgeAlloc�� true�� ��ȯ�� ���
	else
	{
		// ���� �޸𸮰����� ������ ��, �޸� ��尡 1�� �� ���
		if (Avail->iSeries == 0)
		{
			// �Ҵ����� ����� �ּҰ��� �ش� ����� �ּҰ� ����
			// �Ҵ����� ����� �޸� ũ�⿡ ��û �޸� ũ�� ����
			pNode->tMemory.iStart = TempAvail->tMemory.iStart;
			pNode->tMemory.iSize = Request_size;

			// �ش� ����� �ּҰ��� ��û �޸� ũ�⸸ŭ ����
			// �ش� ����� �޸� ũ�⸦ ��û �޸� ũ�⸸ŭ ����
			TempAvail->tMemory.iStart += Request_size;
			TempAvail->tMemory.iSize -= Request_size;

			// ��ü �޸� ������ ��û �޸� ũ�⸸ŭ ����
			iMemory -= Request_size;

			// �ش� ����� �޸� ũ�Ⱑ 0�� �� ���
			if (TempAvail->tMemory.iSize == 0)
			{
				// ����Ʈ���� �ش� ��� ����
				DeleteNode = TempAvail; 
				TempAvail->pNext->pPrev = TempAvail->pPrev;
				TempAvail->pPrev->pNext = TempAvail->pNext;

				// ��� ������ ��Ÿ���� ���� ����
				--Avail->iSeries; 
			}
			UsedInsert(pNode); // Used ����Ʈ�� ����
			return pNode; // �Ҵ����� ��带 ��ȯ
		}

		// �޸𸮰����� �񿬼����� ���
		else
		{
			while (true)
			{
				// ��û �޸��� ũ�Ⱑ �ش� ����� �޸� ũ�⺸�� Ŭ ���
				if (TempAvail->tMemory.iSize < Request_size)
				{
					// ���� ���� �̵�
					TempAvail = TempAvail->pNext;
				}

				// ��û �޸��� ũ�Ⱑ �ش� ����� �޸� ũ�⺸�� �۰ų� ���� ���
				else 
				{
					// ���̸� ��Ÿ���� ���� ����
					// �ش� ����� �޸� ũ��� ��û �޸��� ���̰� ���� ���
					if (iMin > TempAvail->tMemory.iSize - Request_size)
					{
						// ���̸� ��Ÿ���� ������ ����
						iMin = TempAvail->tMemory.iSize - Request_size;
					}

					// ���� ���� �̵�
					TempAvail = TempAvail->pNext;

					// ������ ������ Ž���� ���� ���
					if (TempAvail == Avail->pTail)
					{
						break; // �ݺ��� �ߴ�
					}
				}
			}
		}

		// ����Ʈ�� ó������ �̵�
		TempAvail = Avail->pHead->pNext;
		while (true)
		{
			// ���̸� ��Ÿ���� ������
			// �ش� ����� �޸� ũ��� ��û �޸��� ���̰� ���� ���
			if (iMin == TempAvail->tMemory.iSize - Request_size)
			{
				// �Ҵ� ���� ����� �ּҰ��� �ش� ����� �ּҰ� ����
				// �Ҵ� ���� ����� �޸� ũ�⿡ ��û �޸𸮸� ����
				pNode->tMemory.iStart = TempAvail->tMemory.iStart;
				pNode->tMemory.iSize = Request_size;

				// �ش� �޸� ����� �ּҰ��� ��û �޸� ��ŭ ����
				// �ش� �޸� ����� �޸� ũ�⸦ ��û �޸� ��ŭ ����
				TempAvail->tMemory.iStart += Request_size;
				TempAvail->tMemory.iSize -= Request_size;

				// ��ü �޸� ������ ��û �޸� ��ŭ ����
				iMemory -= Request_size;

				// �ش� �޸� ����� �޸� ũ�Ⱑ 0�� �� ���
				if (TempAvail->tMemory.iSize == 0)
				{
					// �ش� �޸� ��� ����
					DeleteNode = TempAvail;
					TempAvail->pNext->pPrev = TempAvail->pPrev;
					TempAvail->pPrev->pNext = TempAvail->pNext;

					// ��� ������ ��Ÿ���� ���� ����
					--Avail->iSeries;
				}

				// �Ҵ� �� ���� ��峢�� �պ��� �����Ѱ� �Ǵ�
				Combine();
				break;
			}

			// ���̸� ��Ÿ���� ������
			// �ش� ����� �޸� ũ��� ��û �޸��� ���̰� �ٸ� ���
			else
				TempAvail = TempAvail->pNext; // ���� ���� �̵�
		}
	}
	delete DeleteNode; // ������� ����
	UsedInsert(pNode); // Used ����Ʈ�� �Ҵ����� ��� ����
	return pNode; // �Ҵ����� ��� ��ȯ
}
void MemoryManager::MyFree(Addr FreeNode)
{
	PNODE TempUsed = Used->pHead->pNext; // Ž���� ���� �ӽ� ���

	// JudgeFree �Լ��� false�� ��ȯ�� ���
	if (!JudgeFree(FreeNode))
	{
		return; // �Լ� ����
	}

	// JudgeFree �Լ��� true�� ��ȯ�� ���
	else
	{
		while (true)
		{
			// �ش� �޸� ����� �ּҰ��� ��ȯ ����� �ּҰ��� ���� ���
			if (TempUsed->tMemory.iStart == FreeNode->tMemory.iStart)
			{
				// �ش� �޸� ��� ����
				TempUsed->pPrev->pNext = TempUsed->pNext;
				TempUsed->pNext->pPrev = TempUsed->pPrev;

				// ��ü �޸� ������ ��ȯ ����� �޸� ũ�⸸ŭ ����
				iMemory += FreeNode->tMemory.iSize;

				// ��� ������ ��Ÿ���� ���� ����
				--Used->iSeries;

				// ��� ������ 0�� �� ���
				if (Used->iSeries == 0)
				{					
					InitList(Used, 0); // Used ����Ʈ �� �ʱ�ȭ
				}
				break;
			}

			// �ش� �޸� ����� �ּҰ��� ��ȯ ����� �ּҰ��� �ٸ� ���
			// ���� ���� �̵�
			TempUsed = TempUsed->pNext;
		}
	}
	
	//��ȯ ��带 Avail ����Ʈ�� ����
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
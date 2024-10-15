#include <iostream>
#include <fstream>
using namespace std;
class CNode
{
public:
	int Count, CountBit, * Binary, Bit;
	char Letter;
	CNode* Right, * Left, * Next;
	CNode()
	{
		Letter = Count = CountBit = Bit = 0, Right = Left = Next = NULL, Binary = NULL;
	}
};
class CSList
{
public:
	CNode* pHead;
	CNode* pTail;
	CSList()
	{
		pHead = pTail = NULL;
	}
	void AttachBits(CNode* pnn)
	{
		if (pHead == NULL) pHead = pnn, pTail = pnn;
		else pTail->Next = pnn, pTail = pnn;
	}
	void Attach(CNode* pnn)
	{
		if (pHead == NULL) pHead = pnn;
		else
		{
			CNode* pb = NULL, * pTrav = pHead;
			while (pTrav->Count < pnn->Count)
			{
				pb = pTrav, pTrav = pTrav->Next;
				if (pTrav == NULL) break;
			}
			if (pb != NULL) pnn->Next = pTrav, pb->Next = pnn;
			else pnn->Next = pTrav, pHead = pnn;
		}
	}
	CNode* Pop()
	{
		CNode* Take = pHead;
		pHead = pHead->Next;
		Take->Next = NULL;
		return Take;
	}
	void Display()
	{
		CNode* pTrav = pHead;
		while (pTrav != NULL)
		{
			cout << pTrav->Letter << " = " << pTrav->Count << endl;
			pTrav = pTrav->Next;
		}
	}
	void DisplayBinary()
	{
		CNode* ptrav = pHead;
		while (ptrav != NULL)
		{
			cout << "The letter \'" << ptrav->Letter << "\' appeared \"" << ptrav->Count << "\" Times and Number of bits is " << ptrav->CountBit << "\tBinary: ";
			for (int i = 0;i < ptrav->CountBit;i++) cout << ptrav->Binary[i];
			cout << endl;
			ptrav = ptrav->Next;
		}
	}
};
class CBSTree
{
public:
	CNode* Root;
	CBSTree()
	{
		Root = NULL;
	}
	void MakeTree(CSList& ListLetters)
	{
		while (ListLetters.pHead->Next != NULL)
		{
			Root = new CNode;
			Root->Left = ListLetters.Pop();
			Root->Right = ListLetters.Pop();
			Root->Count = Root->Left->Count + Root->Right->Count;
			ListLetters.Attach(Root);
		}
	}
	void BinaryCompressing(CNode* ptrav, int Count, int Bit, CNode* Backtrav)
	{
		if (ptrav == NULL) return;
		if (ptrav != Backtrav)
		{
			ptrav->Binary = new int[Count];
			for (int i = 0;i < Count - 1;i++) ptrav->Binary[i] = Backtrav->Binary[i];
			ptrav->Binary[Count - 1] = Bit;
			ptrav->CountBit = Count;
		}
		if (ptrav->Letter == (char)0) Backtrav = ptrav;
		BinaryCompressing(ptrav->Left, Count + 1, 0, Backtrav);
		BinaryCompressing(ptrav->Right, Count + 1, 1, Backtrav);
	}
	void DispAll(CNode* ptrav)
	{
		if (ptrav == NULL) return;
		DispAll(ptrav->Left);
		if (ptrav->Letter != (char)0)
		{
			cout << "The letter \'" << ptrav->Letter << "\' appeared \"" << ptrav->Count << "\" Times and Number of bits is " << ptrav->CountBit << "\tBinary: ";
			for (int i = 0;i < ptrav->CountBit;i++) cout << ptrav->Binary[i];
			cout << endl;
		}
		DispAll(ptrav->Right);
	}
	void GoodBye(CNode* ptrav)
	{
		if (ptrav == NULL) return;
		GoodBye(ptrav->Left);
		GoodBye(ptrav->Right);
		delete ptrav;
	}
	~CBSTree()
	{
		GoodBye(Root);
	}
};
void MakeListLetters(char* Read, CSList& ListLetters, int Byte)
{
	int Count = 0;
	for (int i = 0, Saw = 0;i < Byte;i++, Saw = 0, Count++)
	{
		CNode* ptrav = ListLetters.pHead;
		while (ptrav != NULL)
		{
			if (ptrav->Letter == Read[i]) Saw = 1;
			ptrav = ptrav->Next;
		}
		if (Saw == 0)
		{
			CNode* pnn = new CNode;
			for (int k = i;k < Byte;k++) if (Read[k] == Read[i]) pnn->Count++;
			pnn->Letter = Read[i];
			ListLetters.Attach(pnn);
		}
	}
}
void MakeListBinary(CSList& ListBinary, CNode* ptrav)
{
	if (ptrav == NULL) return;
	if (ptrav->Left == NULL && ptrav->Right == NULL)
	{
		CNode* pnn = new CNode;
		pnn->Letter = ptrav->Letter;
		pnn->Count = ptrav->Count;
		pnn->CountBit = ptrav->CountBit;
		pnn->Binary = new int[pnn->CountBit];
		for (int i = 0;i < ptrav->CountBit;i++) pnn->Binary[i] = ptrav->Binary[i];
		ListBinary.Attach(pnn);
		return;
	}
	MakeListBinary(ListBinary, ptrav->Left);
	MakeListBinary(ListBinary, ptrav->Right);
}
char* Compress(CSList& ListBinary, char *Read, int Byte, int& NumArrayCom)
{
	char* ArrayCom = NULL, tmp = 0, mask = 1;
	int iBit = 7;
	for (int Array = 0;Array < Byte;Array++)
	{
		CNode* ptrav = ListBinary.pHead;
		while (ptrav != NULL)
		{
			if (ptrav->Letter == Read[Array]) break;
			ptrav = ptrav->Next;
		}
		if (ptrav != NULL)
		{
			for (int i = 0;i < ptrav->CountBit;i++, iBit--)
			{
				if (iBit == -1)
				{
					if (NumArrayCom == 1) ArrayCom = new char[1], ArrayCom[0] = tmp;
					else
					{
						char* temparr = new char[NumArrayCom];
						for (int k = 0;k < NumArrayCom;k++) temparr[k] = ArrayCom[k];
						temparr[NumArrayCom - 1] = tmp;
						delete[] ArrayCom;
						ArrayCom = temparr;
					}
					tmp = 0, iBit = 7, NumArrayCom++;
				}
				tmp |= (((char)ptrav->Binary[i] << iBit) & (mask << iBit));
			}
		}
	}
	char* temparr = new char[NumArrayCom];
	for (int k = 0;k < NumArrayCom - 1;k++) temparr[k] = ArrayCom[k];
	temparr[NumArrayCom - 1] = tmp;
	delete[] ArrayCom;
	ArrayCom = temparr;
	return ArrayCom;
}
void Decompress(CSList& ListBinary, char* Compressed, int NumArrayCom, char* Decompressed)
{
	CNode* ptrav = ListBinary.pHead;
	CSList BitsFound;
	char mask = 1;
	for (int iComp = 0, Found = 0;iComp < NumArrayCom;iComp++)
	{
		for (int iBit = 7;iBit > -1;iBit--)
		{
			CNode* pnn = new CNode;
			if ((Compressed[iComp] & (mask << iBit)) != 0) pnn->Bit = 1;
			else pnn->Bit = 0;
			BitsFound.AttachBits(pnn);
			CNode* ptrav = ListBinary.pHead;
			while (ptrav != NULL)
			{
				CNode* ptrav2 = BitsFound.pHead;
				int i = 0, Good = 1;
				while (ptrav2 != NULL && i < ptrav->CountBit)
				{
					if (ptrav2->Bit != ptrav->Binary[i])
					{
						Good = 0;
						break;
					}
					ptrav2 = ptrav2->Next, i++;
				}
				if (ptrav2 == NULL && i != ptrav->CountBit || ptrav2 != NULL && i == ptrav->CountBit) Good = 0;
				if (Good == 1)
				{
					CNode* ptrav2 = BitsFound.pHead;
					while (ptrav2 != NULL)
					{
						CNode* pb = ptrav2;
						ptrav2 = ptrav2->Next;
						delete pb;
					}
					BitsFound.pHead = BitsFound.pTail = NULL;
					Decompressed[Found] = ptrav->Letter, Found++;
				}
				ptrav = ptrav->Next;
			}
		}
	}
}
void main()
{
	ofstream fl2("img4.jpeg", ofstream::binary);
	ifstream fl("img.jpeg", ifstream::binary);
	ofstream write;
	write.open("com.txt");
	CSList ListLetters, ListBinary;
	CBSTree Tree;
	int NumArrayCom = 1;
	char* Compressed = NULL, * Decompressed = NULL, * Read = NULL, read;
	fl.seekg(0, fl.end);
	int Byte = fl.tellg();
	Read = new char[Byte];
	Decompressed = new char[Byte];
	fl.seekg(0, fl.beg);
	for (int i = 0;i < Byte;i++)
	{
		fl.read(&read, 1);
		Read[i] = read;
	}
	MakeListLetters(Read, ListLetters, Byte);
	ListLetters.Display();
	Tree.MakeTree(ListLetters);
	Tree.BinaryCompressing(Tree.Root, 0, 0, Tree.Root);
	MakeListBinary(ListBinary, Tree.Root);
	ListBinary.DisplayBinary();
	Compressed = Compress(ListBinary, Read, Byte, NumArrayCom);
	for (int i = 0;i < NumArrayCom;i++)
	{
		write << Compressed[i];
	}
	Decompress(ListBinary, Compressed, NumArrayCom, Decompressed);
	for (int i = 0;i < Byte;i++)
	{
		read = Decompressed[i];
		fl2.write(&read, 1);
	}
	fl2.close();
	fl.close();
	write.close();
	cout << "Done";
	delete[] Compressed;
	delete[] Read;
}
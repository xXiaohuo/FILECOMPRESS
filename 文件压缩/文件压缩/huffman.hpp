#pragma once 
#include<queue>
using namespace std;
template<class W>

struct HTNode
{
	HTNode(const W & weight)
	:_pLeft(nullptr)
	, _pRight(nullptr)
	, _pParent(nullptr)
	, _weight(weight)
	{	}
	HTNode* _pLeft;
	HTNode *_pRight;
	HTNode * _pParent;
    W _weight;

};

//仿函数
template <class	W>
struct Compara
{

	bool operator()(HTNode<W>* pLeft, HTNode<W>* pRight)
	{
		if (pLeft->_weight > pRight->_weight)
		{
			return true;
		}
		return false;
	}
};

template <class W>
class HuffmanTree
{
	typedef HTNode<W> Node;
	typedef Node* PNode;
public:
	HuffmanTree()
		:_pRoot(nullptr)
	{	}
	
	~HuffmanTree()
	{
		Destroy(_pRoot);
	}

	void CreatHuffmanTree(const vector<W>& v,const W& invalid)
	{
		if (v.empty())
		{
			return;
		}
		//创建小堆
		//默认大堆，需要小堆
		//写一个比较规则
		//创建二叉树森林
		std::priority_queue<PNode, vector<PNode>, Compara<W>> hp;
		size_t i = 0;
		for (i = 0; i < v.size(); ++i)
		{
			if (v[i]!=invalid)
			hp.push( new Node(v[i]));
		}
		//

		while (hp.size()>1)
		{
			//从堆中获去两个权值最小的两颗树
			PNode pLeft = hp.top();
			hp.pop();
			PNode pRight = hp.top();
			hp.pop();
			//以pLEFT与pright 权值之和构建新的节点
			PNode pParent = new Node(pLeft->_weight + pRight->_weight);
			//
			pParent->_pLeft = pLeft;
			pLeft->_pParent = pParent;

			pParent->_pRight = pRight;
			pRight->_pParent = pParent;

			hp.push(pParent);
		}
		//堆定就是创建好的二叉树
		_pRoot = hp.top();
	}

	PNode GetRoot()
	{
		return _pRoot;
	}
private:

	void Destroy(PNode& pRoot)
	{
		if (pRoot)
		{

			Destroy(pRoot->_pLeft);
			Destroy(pRoot->_pRight);
			delete pRoot;
			pRoot = nullptr;
		}

	}
private:
	PNode _pRoot;
};
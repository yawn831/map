#pragma once
#include<iostream>
using namespace std;
enum Colour
{
	RED,
	BLACK
};

template<typename T>
struct RBTreeNode
{
	RBTreeNode<T>* Left;
	RBTreeNode<T>* Right;
	RBTreeNode<T>* Parent;
	T Data;
	Colour Col;
	RBTreeNode(const T& InOther) :
		Left(nullptr),
		Right(nullptr),
		Parent(nullptr),
		Data(InOther),
		Col(RED)
	{ }

	// 拷贝构造
	RBTreeNode(const RBTreeNode<T>*& InOther) :
		Left(nullptr),
		Right(nullptr),
		Parent(nullptr),
		Data(InOther->Data),
		Col(InOther->Col)
	{ }
};


template<typename T,typename Ref,typename Ptr>
struct TreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef TreeIterator<T, Ref, Ptr> Self;
	Node* node;

	TreeIterator() :
		node{ nullptr }
	{ }

	TreeIterator(Node* InNode) :
		node{ InNode }
	{ }

	typedef TreeIterator<T, T&, T*> iterator;

	// 拷贝构造函数
	TreeIterator(const iterator& InIterator):
		node{ InIterator.node }
	{ }

	Ref operator*()
	{
		return node->Data;
	}

	Ptr operator->()
	{
		return &node->Data;
	}

	bool operator!=(const Self& InIterator) const
	{
		return node != InIterator.node;
	}

	bool operator==(const Self& InIterator) const
	{
		return !(node != InIterator.node);
	}

	// 中序遍历的规则
	Self operator++()
	{
		// 当存在右子树时，我们需要遍历到右子树的最左结点，然后访问该结点。
		if (node->Right)
		{
			Node* left = node->Right;
			while (left->Left)
			{
				left = left->Left;
			}
			node = left;
		} 
		else
		{
			Node* cur = node;
			Node* parent = cur->Parent;
			while (parent && cur == parent->Right)// 如果是右孩子需要回溯
			{
				cur = cur->Parent;
				parent = parent->Parent;
			}
			// 说明是左孩子
			node = parent;
		}
		return *this;
	}

	Self operator++(int)
	{
		Self tmp = *this;
		++(*this);
		return *this;
	}

	Self operator--()
	{
		if (node->Left) // 根节点左子树不为空
		{
			Node* right = node->Left;// 找左子树的最右节点
			while (right->Right)
			{
				right = right->Right;
			}
			node = right;
		}
		else
		{
			Node* cur = node;
			Node* parent = cur->Parent;
			while (parent && cur == parent->Left)
			{
				cur = cur->Parent;
				parent = parent->Parent;
			}
			node = parent;
		}
		return *this;
	}

	Self operator--(int)
	{
		Self tmp = *this;
		--(*this);
		return tmp;
	}
};


template <typename K,typename T,typename KeyOfT>
class RBTree
{ 
	typedef RBTreeNode<T> Node;
public:
	typedef TreeIterator<T, T&, T*> iterator;
	typedef TreeIterator<T, const T&,  const T*> const_iterator;

	RBTree():
		root{nullptr}
	{ }

	~RBTree()
	{
		Destroy(root);
	}

	void Destroy(Node* InRoot)
	{
		if (InRoot == nullptr)
		{
			return;
		}
		Destroy(InRoot->Left);

		Destroy(InRoot->Right);
		delete InRoot;
	}

	// 拷贝构造函数
	RBTree(const RBTree<K, T, KeyOfT>& InTree) :
		root(nullptr)
	{
		root = copy(InTree.root);
	}

	Node* copy(const Node* InRoot)
	{
		if (InRoot == nullptr)
			return nullptr;

		Node* newnode = new Node(InRoot);
		
		newnode->Left = copy(InRoot->Left);
		newnode->Right = copy(InRoot->Right);

		if (newnode->Right)
		{
			newnode->Right->Parent = newnode;
		}

		if (newnode->Left)
		{
			newnode->Left->Parent = newnode;
		}
		return newnode;
	}

	RBTree<K, T, KeyOfT>& operator=(RBTree<K, T, KeyOfT> InTree)
	{
		swap(root, InTree.root);
		return *this;
	}

	iterator begin()
	{
		Node* left = root;
		while (left && left->Left)
		{
			left = left->Left;
		}
		return  iterator(left);
	}

	iterator end()
	{
		return nullptr;
	}

	const_iterator begin() const
	{
		Node* left = root;
		while (left && left->Left)
		{
			left = left->Left;
		}
		return const_iterator(left);
	}

	const_iterator end() const
	{
		return nullptr;
	}

	bool empty() const
	{
		return root == nullptr;
	}

	int size()
	{
		int count = 0;
		for (auto& e : *this)
		{
			count++;
		}
		return count;
	}

	int count(const K& key) const
	{
		KeyOfT kot;
		int Count = 0;
		for (auto& e : *this)
		{
			if (kot(e) == key)
			{
				Count++;
			}
		}
		return Count;
	}

	iterator find(const K& InKey) const
	{
		// 使用类实例化一个对象oft，通过oft去调用map/set 中相同的operator() ，取出对应的key值
		KeyOfT oft;
		if (root == nullptr)
		{
			return iterator(nullptr);
		}
		Node* cur = root;
		while (cur)
		{
			if (oft(cur->Data) > InKey)
			{
				cur = cur->Left;
			}
			else if (oft(cur->Data) < InKey)
			{
				cur = cur->Right;
			}
			else
			{
				return iterator(cur);
			}
		}
		return iterator(nullptr);
	}

	pair<iterator, bool> insert(const T& InOther)
	{
		KeyOfT oft;
		if (root == nullptr)
		{
			root = new Node(InOther);

			root->Col = BLACK;
			return make_pair(root, true);
		}
		Node* parent = nullptr;
		Node* cur = root;
		while (cur)
		{
			if (oft(cur->Data) < oft(InOther))
			{
				parent = cur;
				cur = cur->Right;
			}
			else if (oft(cur->Data) > oft(InOther))
			{
				parent = cur;
				cur = cur->Left;
			}
			else
			{
				return make_pair(iterator(root), false);
			}
		}
		Node* newnode = new Node(InOther);
		newnode->Col = RED;
		if (oft(parent->Data) < oft(InOther))
		{
			parent->Right = newnode;
			newnode->Parent = parent;
		}
		else
		{
			parent->Left = newnode;
			newnode->Parent = parent;
		}
		cur = newnode;

		while (parent && parent->Col == RED)
		{
			Node* gfather = parent->Parent;
			if (parent = gfather->Left)
			{
				Node* uncle = gfather->Right;
				if (uncle && uncle->Col == RED)
				{
					parent->Col = uncle->Col = BLACK;
					gfather->Col = RED;
					cur = gfather;
					parent = cur->Parent;
				}
				else
				{
					if (cur == parent->Left)
					{
						RotateR(gfather);
						gfather->Col = RED;
						parent->Col = BLACK;
					}
					else
					{
						RotateL(parent);
						RotateR(gfather);
						cur->Col = BLACK;
						gfather->Col = RED;
					}
					break;
				}
			}
			else
			{
				Node* uncle = gfather->Left;
				if (uncle && uncle->Col == RED)
				{
					uncle->Col = BLACK;
					gfather->Col = RED;
					cur = gfather;
					parent = cur->Parent;
				}
				else
				{
					if (cur == parent->Right)
					{
						RotateL(gfather);
						parent->Col = BLACK;
						gfather->Col = RED;
					}
					else
					{
						RotateR(parent);
						RotateL(gfather);
						cur->Col = BLACK;
						gfather->Col = RED;
					}
					break;
				}
			}
		}
		root->Col = BLACK;
		return make_pair(iterator(newnode), true);
	}

	int getheight() const
	{
		return GetHeight(root);
	}

	int GetHeight(Node* InRoot)const
	{
		if (InRoot == nullptr)
		{
			return 0;
		}

		int left = GetHeight(InRoot->Left);
		int right = GetHeight(InRoot->Right);
		
		return 1 + max(left, right);
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->Left;
		Node* subLR = subL->Right;
		RotateL(parent->Left);
		RotateR(parent);
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->Right;
		Node* subRL = subR->Left;
		RotateR(parent->Right);
		RotateL(parent);
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->Right;
		Node* subRL = subR->Left;
		Node* gfather = parent->Parent;
		parent->Right = subRL;
		if (subRL)
		{
			subRL->Parent = parent;
		}
		subR->Left = parent;
		parent->Parent = subR;
		if (parent == root)
		{
			root = subR;
			subR->Parent = nullptr;
		}
		else
		{
			if (gfather->Left == parent)
			{
				gfather->Left = subR;
			}
			else
			{
				gfather->Right= subR;
			}
			subR->Parent = gfather;
		}
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->Left;
		Node* subLR = subL->Right;
		parent->Left = subLR;
		if (subLR)
		{
			subLR->Parent = parent;
		}
		subL->Right = parent;
		Node* gfather = parent->Parent;
		parent->Parent = subL;
		if (parent == root)
		{
			root = subL;
			root->Parent = nullptr;
		}
		else
		{
			if (gfather->Left == parent)
			{
				gfather->Left = subL;
			}
			else
			{
				gfather->Right = subL;
			}
			subL->Parent = gfather;
		}
	}
private:
	Node* root;
};
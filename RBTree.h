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

	// ��������
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

	// �������캯��
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

	// ��������Ĺ���
	Self operator++()
	{
		// ������������ʱ��������Ҫ�������������������㣬Ȼ����ʸý�㡣
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
			while (parent && cur == parent->Right)// ������Һ�����Ҫ����
			{
				cur = cur->Parent;
				parent = parent->Parent;
			}
			// ˵��������
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
		if (node->Left) // ���ڵ���������Ϊ��
		{
			Node* right = node->Left;// �������������ҽڵ�
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

	// �������캯��
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


	void erase(iterator InIterator)
	{
		Node* child, *parent;
		Colour color;
		Node* cur = InIterator.node;
		if ((cur->Left != nullptr) && (cur->Right) != nullptr)
		{
			Node* repalce = cur;
			repalce = repalce->Right;
			while (repalce->Left != nullptr)
				repalce = repalce->Left;
			if (cur->Parent)
			{
				if (cur->Parent->Left == cur)
					cur->Parent->Left = repalce;
				else
					cur->Parent->Right = repalce;
			}
			else
				root = repalce;

			child = repalce->Right;
			parent = repalce->Parent;

			color = repalce->Col;

			if (parent == cur)
			{
				parent = repalce;
			}
			else
			{
				if (child)
					child->Parent = parent;

				parent->Left = child;
				
				repalce->Right = cur->Right;
				cur->Right->Parent = repalce;
			}

			repalce->Parent = cur->Parent;
			repalce->Col = cur->Col;
			repalce->Left = cur->Left;
			cur->Left->Parent = repalce;
			//
			delete cur;
			return;
		}
		if (cur->Left != nullptr)
			child = cur->Left;
		else
			child = cur->Right;

		parent = cur->Parent;

		color = cur->Col;
		if (child)
			child->Parent = parent;
		if (parent)
		{
			if (parent->Left == cur)
				parent->Left = child;
			else
				parent->Right = child;
		}
		else
			root = child;

		delete cur;
	}

	void erase(const K& InKey)
	{
		erase(find(InKey));
	}

	iterator find(const K& InKey) const
	{
		// ʹ����ʵ����һ������oft��ͨ��oftȥ����map/set ����ͬ��operator() ��ȡ����Ӧ��keyֵ
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
		if (root == nullptr)
		{
			root = new Node(InOther);

			root->Col = BLACK;
			return make_pair(root, true);
		}
		KeyOfT oft;
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
		cur = new Node(InOther);
		Node* newnode = cur;
		if (oft(parent->Data) < oft(InOther))
		{
			parent->Right = cur;
		}
		else
		{
			parent->Left = cur;
		}
		cur->Parent = parent;

		while (parent && parent->Col == RED)
		{
			Node* gfather = parent->Parent;
			if (parent == gfather->Left)
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
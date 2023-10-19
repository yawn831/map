#pragma once
#include<iostream>
using namespace std;
enum Colour
{
	RED,
	BLACK,
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
		return tmp;
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
public:
	typedef RBTreeNode<T> Node;
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


	iterator begin()
	{
		Node* left = root;
		while (left && left->Left)
		{
			left = left->Left;
		}
		return  (left);
	}

	iterator end()
	{
		return iterator(nullptr);
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
		return const_iterator(nullptr);
	}

	Node* find(const K& InKey)
	{
		KeyOfT oft;
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
				return cur;
			}
		}
		return nullptr;
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
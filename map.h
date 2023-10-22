#pragma once
#include"RBTree.h"
using namespace std;
	template<typename K, typename V>
	class map
	{
	public:

		// 仿函数：获取key的值
		struct MapKeyOfT
		{
			const K& operator()(const pair<const K, V>& kv) 
			{
				return kv.first;
			}
		};
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::iterator iterator;
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::const_iterator const_iterator;

		iterator begin()
		{
			return rbt.begin();
		}

		iterator end()
		{
			return rbt.end();
		}

		const_iterator begin()const
		{
			return rbt.begin();
		}

		const_iterator end()const
		{
			return rbt.end();
		}

		pair<iterator, bool> insert(const pair<K, V>& kv)
		{
			return rbt.insert(kv);
		}

		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = insert(make_pair(key, V()));
			return ret.first->second;
		}

		bool empty() const
		{
			return rbt.empty();
		}

		iterator find(const K& InKey) const
		{
			return rbt.find(InKey);
		}

		int size()
		{
			return rbt.size();
		}

		int count()const
		{
			return rbt.count();
		}

		int getheight()const
		{
			return rbt.getheight();
		}

		void erase(iterator InIterator)
		{
			rbt.erase(InIterator);
		}

	private:
		RBTree<K, pair<const K, V>, MapKeyOfT> rbt;
	};
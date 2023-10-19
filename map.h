#pragma once
#include"RBTree.h"
using namespace std;
	template<typename K, typename V>
	class map
	{
	public:
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


		pair<iterator, bool> insert(const pair<const K, V>& kv)
		{
			return  rbt.insert(kv);
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
	private:
		RBTree<K, pair<const K, V>, MapKeyOfT> rbt;
	};
#pragma once
#include"HashTable.h"
namespace yiming
{
	template<class K, class T>
	class unordered_map
	{
		struct MapKeyOfT
		{
			const K& operator()(const pair<K, T>& kv)
			{
				return kv.first;
			}
		};
	public:
		bool insert(const pair<K, T>& kv)
		{
			return _t.Insert(kv);
		}
	private:
		HashTable<K, pair<K, T>, MapKeyOfT> _t;
	};

}

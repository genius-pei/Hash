#pragma once
#include"HashTable.h"
namespace yiming
{
	template<class K>
	class unordered_set
	{
		struct SetKeyOfT
		{
			const K& operator()(const K& key)
			{
				return key;
			}
		};
	public:
		bool insert(const K& k)
		{
			return _t.Insert(k);
		}
	private:
		HashTable<K, K, SetKeyOfT> _t;
	};
}

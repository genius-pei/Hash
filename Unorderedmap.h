#pragma once

#include"HashTablecopy.h"

namespace bit
{
	template<class K, class V, class Hash = HashFunc<K>>
	class unordered_map
	{
		struct MapKeyOfT
		{
			const K& operator()(const pair<K, V>& kv)
			{
				return kv.first;
			}
		};
	public:
		typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::Iterator iterator;
		typedef typename HashTable<K, pair<const K, V>, MapKeyOfT, Hash>::ConstIterator const_iterator;

		iterator begin()
		{
			return _t.Begin();
		}

		iterator end()
		{
			return _t.End();
		}

		const_iterator begin() const
		{
			return _t.Begin();
		}

		const_iterator end() const
		{
			return _t.End();
		}

		pair<iterator, bool> insert(const pair<K, V>& kv)
		{
			return _t.Insert(kv);
		}

		V& operator[](const K& key)
		{
			pair<iterator, bool> ret = insert({ key, V() });

			return ret.first->second;
		}

		bool erase(const K& key)
		{
			return _t.Erase(key);
		}

		iterator find(const K& key)
		{
			return _t.Find(key);
		}
	private:
		HashTable<K, pair<const K, V>, MapKeyOfT, Hash> _t;
	};

	void test_umap()
	{
		unordered_map<string, string> dict;
		dict.insert({ "insert", "≤Â»Î" });
		dict.insert({ "sort", "≈≈–Ú" });
		dict.insert({ "test", "≤‚ ‘" });

		for (auto& [k, v] : dict)
		{
			// k += 'x';

			cout << k << ":" << v << endl;
		}

		dict["string"] = "◊÷∑˚¥Æ";
		dict["key"];
		dict["key"] = "πÿº¸◊÷";
		dict["for"];


		for (auto& [k, v] : dict)
		{
			cout << k << ":" << v << endl;
		}
	}
};

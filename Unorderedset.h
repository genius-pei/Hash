#pragma once
#include"HashTable.h"
namespace yiming
{
	template<class K,class Hash=HashFunc<K>>
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
		typedef typename HashTable<K, const K, SetKeyOfT, Hash>::Iterator iterator;
		iterator begin()
		{
			return _t.Begin();
		}
		iterator end()
		{
			return _t.End();
		}
		bool insert(const K& k)
		{
			return _t.Insert(k);
		}
		

		void Func(const unordered_set<int>& s)
		{
			auto it1 = s.begin();
			while (it1 != s.end())
			{
				// *it1 = 1;

				cout << *it1 << " ";
				++it1;
			}
			cout << endl;
		}

		void test_uset1()
		{
			unordered_set<int> s1;
			s1.insert(45);
			s1.insert(5);
			s1.insert(13);
			s1.insert(45);
			s1.insert(452);
			s1.insert(4513);
			s1.insert(333);
			s1.insert(123);

			Func(s1);

			auto it1 = s1.begin();
			while (it1 != s1.end())
			{
				//*it1 = 1;
				cout << *it1 << " ";
				++it1;
			}
			cout << endl;
		}

	private:
		HashTable<K, K, SetKeyOfT, Hash> _t;
	};
}

#pragma once

#include"HashTablecopy.h"

namespace yiming
{
	template<class K, class Hash = HashFunc<K>>
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
		typedef typename HashTable<K, const K, SetKeyOfT, Hash>::ConstIterator const_iterator;

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

		pair<iterator, bool> insert(const K& k)
		{
			return _t.Insert(k);
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
		HashTable<K, const K, SetKeyOfT, Hash> _t;
	};

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

	struct Date
	{
		int _year;
		int _month;
		int _day;

		bool operator==(const Date& d) const
		{
			return _year == d._year
				&& _month == d._month
				&& _day == d._day;
		}
	};

	struct DateHashFunc
	{
		// BKDR
		size_t operator()(const Date& d)
		{
			//2025 1 9
			//2025 9 1
			//2025 2 8
			size_t hash = 0;
			hash += d._year;
			hash *= 131;

			hash += d._month;
			hash *= 131;

			hash += d._day;
			hash *= 131;

			return hash;
		}
	};


	void test_uset2()
	{
		unordered_set<Date, DateHashFunc> s1;
		s1.insert({ 2025, 9, 15 });
		s1.insert({ 2025, 9, 18 });
	}
};

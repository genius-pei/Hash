#pragma once
#include<vector>
#include<utility>

using namespace std;

enum Status
{
	EXIST,
	EMPTY,
	DELETE
};
template<class K, class V>
struct HashData
{
	pair<K, V> _kv;
	Status _status=EMPTY;
};
template<class K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
	}
};
template<class K, class V,class Hash=HashFunc<K>>
class HashTable
{
	
public:
	HashTable()
		:_tables(11)
		, _n(0)
	{

	}
	bool Insert(const pair<K, V>& kv)
	{
		if ((double)_n /(double) _tables.size() >= 0.7)//扩容
		{
			//std::vector<HashData<K, V>> newtables(_tables.size()*2);
			////遍历旧表将其映射到新表
			//for (auto& data : _tables)
			//{
			//	if (data._status == EXIST)
			//	{
			//		//...
			//	}

			//}
			//_tables.swap(newtables);


			HashTable<K, V> newHT;
			newHT._tables.resize(_tables.size() * 2);  // 新表大小为原表2倍
			//遍历旧表将其映射到新表
			for (auto& data : _tables)
			{
				if (data._status == EXIST)
				{
					newHT.Insert(data._kv);
				}

			}
			_tables.swap(newHT._tables);
		}
		Hash hs;
		size_t hash0 = hs(kv.first) % _tables.size();
		size_t hashi = hash0;
		size_t i = 1;
		//线性探测
		while (_tables[hashi]._status == EXIST)//当前位置已经存在时
		{
			hashi = (hash0 + i) % _tables.size();
			++i;
		}
		_tables[hashi]._kv = kv;
		_tables[hashi]._status = EXIST;
		++_n;
		return true;
	}
	HashData<K, V>* Find(const K& key)
	{
		Hash hs;
		size_t hash0 = hs(key) % _tables.size();
		size_t hashi = hash0;
		size_t i = 1;
		while (_tables[hashi]._status != EMPTY)
		{
			if (_tables[hashi]._status==EXIST&&_tables[hashi]._kv.first ==key )
			{
				return &_tables[hashi];
			}
			hashi = (hash0 + i) % _tables.size();
			++i;
		}
		return nullptr;
	}
	bool Erase(const K& key)
	{
		auto* ptr = Find(key);
		if (ptr)
		{
			ptr->_status = DELETE;
			--_n;
		}
		else
		{
			return false;
		}
	}
private:
	std::vector<HashData<K, V>> _tables;
	size_t _n = 0;//有效数据的个数



};
void testhash1()
{
	HashTable<int, int> ht;
	int a[] = { 19,30,5,36,13,20,21,12,58 };
	for (auto e : a)
	{
		ht.Insert({ e, e });
	}

	cout << ht.Find(5) << endl;
	cout << ht.Find(58) << endl;
	ht.Erase(5);
	cout << ht.Find(5) << endl;
	cout << ht.Find(58) << endl;

	for (size_t i = 100; i < 200; i++)
	{
		ht.Insert({ i, i });
	}
}
void testhash2()
{
	 
}
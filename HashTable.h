#pragma once
#include<vector>

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
template<class K, class V>
class HashTable
{
public:
	bool Insert(const pair<K, V>& kv)
	{
		size_t hash0 = kv.first & _tables.size();
		size_t hashi = hash0;
		size_t i = 1;
		//线性探测
		while (_tables[hashi].status == EXIST)//当前位置已经存在时
		{
			hashi = (haso + i) % _tables.size();
			++i;
		}
		_tables[hashi]._kv = kv;
		_tables[hashi].status = EXIST;
		++_n;
		return true;
	}
private:
	std::vector<HashData<K, V>> _tables;
	size_t _n = 0;//有效数据的个数



};

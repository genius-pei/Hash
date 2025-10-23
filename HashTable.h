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
template<>  // ģ���ػ����
struct HashFunc<string>  // �ػ�string����
{
	size_t operator()(const string& str)  // �������BKDR�㷨
	{
		size_t hash = 0;
		for (auto ch : str)
		{
			hash += ch;
			hash *= 131;
		}
		return hash;
	}
};

namespace open_way
{
	template<class K, class V, class Hash = HashFunc<K>>
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
			if ((double)_n / (double)_tables.size() >= 0.7)//����
			{
				//std::vector<HashData<K, V>> newtables(_tables.size()*2);
				////�����ɱ���ӳ�䵽�±�
				//for (auto& data : _tables)
				//{
				//	if (data._status == EXIST)
				//	{
				//		//...
				//	}

				//}
				//_tables.swap(newtables);


				HashTable<K, V, Hash> newHT;
				newHT._tables.resize(_tables.size() * 2);  // �±��СΪԭ��2��
				//�����ɱ���ӳ�䵽�±�
				for (auto& data : _tables)
				{
					if (data._status == EXIST)
					{
						newHT.Insert(data._kv);
					}

				}
				_tables.swap(newHT._tables);
				_n = newHT._n;  // ͬ���±����ЧԪ����
			}
			Hash hs;
			size_t hash0 = hs(kv.first) % _tables.size();
			size_t hashi = hash0;
			size_t i = 1;
			//����̽��
			while (_tables[hashi]._status == EXIST)//��ǰλ���Ѿ�����ʱ
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
				if (_tables[hashi]._status == EXIST && _tables[hashi]._kv.first == key)
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
				return true;
			}
			else
			{
				return false;
			}
		}

	private:
		std::vector<HashData<K, V>> _tables;
		size_t _n = 0;//��Ч���ݵĸ���



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
	struct StringHashFunc
	{
		// BKDR
		size_t operator()(const string& str)
		{
			size_t hash = 0;
			for (auto ch : str)
			{
				hash += ch;
				hash *= 131;
			}

			return hash;
		}
	};
	void testhash2()
	{
		HashTable<string, string, StringHashFunc> dict;
		dict.Insert({ "insert", "����" });

		auto ptr = dict.Find("insert");
		if (ptr)
		{
			cout << ptr->_kv.second << endl;
		}

		StringHashFunc hf;
		cout << hf("abcd") << endl;
		cout << hf("bcad") << endl;
		cout << hf("aadd") << endl;
	}
}




namespace Hash_bucket
{
	template<class K,class V>
	struct HashNode
	{
		pair<K, V> _KV;
		HashNode<K, V>* _next;
	};
	template<class K, class V>
	class HashTable
	{
		typedef HashNode<K, V> Node;
	public:
		bool Insert(const pair<K, V>& kv)
		{
			//����
			if (_n == _tables)
			{

			}


			size_t hashi = kv.first % _tables.size();
			//ͷ��
			Node* newNode = new Node(kv);
			newNode->_next = _tables[hashi];
			_tables[hashi] = newNode;
			++_n;
			return true;
		}
	private:
		vector<Node*> _tables;
		size_t _n = 0;
	};
}
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
template<class K, class T>
struct HashData
{
	pair<K, T> _kv;
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
	template<class K, class T, class Hash = HashFunc<K>>
	class HashTable
	{

	public:
		HashTable()
			:_tables(11)
			, _n(0)
		{

		}
		bool Insert(const pair<K, T>& kv)
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


				HashTable<K, T, Hash> newHT;
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
		HashData<K, T>* Find(const K& key)
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
		std::vector<HashData<K, T>> _tables;
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
	template<class K,class T>
	struct HashNode
	{
		T _data;
		HashNode<T>* _next;
		HashNode(cont T& data)
			:_data(data)
			, _next(nullptr)
			{

			}
		
	};


	// ǰ������
	template<class K, class T, class Hash = HashFunc<K>>
	class HashTable;

	template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
	struct HTIterator
	{
		typedef HashNode<T> Node;
		typedef HashTble<K, T, KeyofT, Hash> HT;
		typedef HTIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

		Node* _node;
		const HT* _ht;

		HTIterator(Node* node, const HT* ht)
			:_node(node)
			,_ht(ht)
		{

		}

		Ref operator*()
		{
			return _node->_data;
		}
		Ptr operator->()
		{
			return &_node->_data;
		}
		Self& operator++()
		{
			if (_node->_next)//��ǰ���нڵ�
			{
				_node = _node->_next;
			}
			else//��ǰͰΪ�գ��ҵ���һ����Ϊ�յ�Ͱ
			{
				size_t hashi = Hash()(KeyofT()(_node->_data)) % _ht._tables.size();
				++hashi;
				while (hashi != _ht._tables.size())
				{
					if (_ht._tables[hashi])
					{
						_node = _ht._tables[hashi];
						break;
					}
					hashi++;
				}
			}
			if (hashi == _ht._tables.size())//���һ��Ͱ�����һ���ڵ��Ѿ������������ߵ�endȥ��nullptr��Ϊend()
			{
				_node = nullptr;
			}
		}
		bool operator!=(const Self& s)const
		{
			return _node != s._node;
		}


	};

	template<class K, class T, class Hash = HashFunc<K>>
	class HashTable
	{
		typedef HashNode<K, T> Node;
	public:
		typedef HTIterator<K, T,T&,T*,KeyofT, Hash> Iterator;
		Iterator Begin()
		{

		}
		Itearator End()
		{
			return Iterator(nullptr, this);
		}

		HashTable()
			:_tables(11)
			,_n(0)
		{

		}
		~HashTable()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				// ��ǰͰ�Ľڵ�����ӳ��ҵ��±�
				while (cur)
				{
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}

				_tables[i] = nullptr;
			}
		}
		bool Insert(const pair<K, T>& kv)
		{
			Hash hs;
			//����
			if (_n == _tables.size())
			{
				//HashTable<K, V> newHT;
				//newHT._tables.resize(_tables.size() * 2);
				//for (auto cur : _tables)
				//{
				//	newHT.Insert(cur->_kv);
				//	cur = cur->_next; 
				//}
				//_tables.swap(newHT._tables);

				vector<Node*> newtables(_tables.size() * 2);
				for (size_t i = 0;i < _tables.size();i++)
				{
					Node* cur = _tables[i];
					while (cur)
					{
						Node* next = cur->_next;
						size_t hashi = hs(cur->_data.first) % newtables.size();
						cur->_next = newtables[hashi];
						newtables[hashi] = cur;
						cur = next;
					}
					_tables[i] = nullptr;
				 }
				_tables.swap(newtables);

			}


			size_t hashi = hs(kv.first) % _tables.size();
			//ͷ��
			Node* newNode = new Node(kv);
			newNode->_next = _tables[hashi];
			_tables[hashi] = newNode;
			++_n;
			return true;
		}
		Node* Find(const K& key)
		{
			size_t hashi = hs(key) % _tables.size();
			Node* cur = _tables[hashi];
			while(cur)
			{
				if (cur->_data.first == key)
				{
					return cur;
				}
				cur = cur->next;
			}
			return nullptr;
		}
		bool Erase(const K& key)
		{
			size_t hashi = hs(key) % _tables.size();
			Node* prev=nullptr;
			Node* cur = _tables[hashi];
			while(cur)
			{
				if (cur->_kv.first == key)
				{
					if (prev == nullptr)//��һ���ڵ�
					{
						_tables[hashi] = cur->next;
					}
					else
					{
						prev->_next = cur->_next;
					}
					delete cur;
					return true;
				}
				prev = cur;
				cur = cur->next;
			}
		}
	private:
		vector<Node*> _tables;
		size_t _n = 0;
	};
	void testhash1()
	{
		HashTable<int, int> ht;
		int a[] = { 19,30,5,36,13,20,21,12,58 };
		for (auto e : a)
		{
			ht.Insert({ e, e });
		}

		//cout << ht.Find(5) << endl;
		//cout << ht.Find(58) << endl;
		//ht.Erase(5);
		//cout << ht.Find(5) << endl;
		//cout << ht.Find(58) << endl;

	//	for (size_t i = 100; i < 200; i++)
	//	{
	//		ht.Insert({ i, i });
	//	}
	}
}
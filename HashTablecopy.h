#pragma once
#include<vector>
#include<iostream>
using namespace std;

template<class K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
	}
};


template<>
struct HashFunc<string>
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


inline unsigned long __stl_next_prime(unsigned long n)
{
	// Note: assumes long is at least 32 bits.
	static const int __stl_num_primes = 28;
	static const unsigned long __stl_prime_list[__stl_num_primes] =
	{
		53, 97, 193, 389, 769,
		1543, 3079, 6151, 12289, 24593,
		49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457,
		1610612741, 3221225473, 4294967291
	};
	const unsigned long* first = __stl_prime_list;
	const unsigned long* last = __stl_prime_list + __stl_num_primes;
	const unsigned long* pos = lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}


template<class T>
struct HashNode
{
	T _data;
	HashNode<T>* _next;

	HashNode(const T& data)
		:_data(data)
		, _next(nullptr)
	{}
};

// 前置声明
template<class K, class T, class KeyOfT, class Hash>
class HashTable;

template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
struct HTIterator
{
	typedef HashNode<T> Node;
	typedef HashTable<K, T, KeyOfT, Hash> HT;
	typedef HTIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

	Node* _node;
	const HT* _ht;

	HTIterator(Node* node, const HT* ht)
		:_node(node)
		, _ht(ht)
	{}

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
		if (_node->_next)  // 当前还有节点
		{
			_node = _node->_next;
		}
		else  // 当前桶为空，找下一个不为空的桶的第一个
		{
			size_t hashi = Hash()(KeyOfT()(_node->_data)) % _ht->_tables.size();
			++hashi;
			while (hashi != _ht->_tables.size())
			{
				if (_ht->_tables[hashi])
				{
					_node = _ht->_tables[hashi];
					break;
				}

				hashi++;
			}

			// 最后一个桶的最后一个节点已经遍历结束，走到end()去，nullptr充当end()
			if (hashi == _ht->_tables.size())
			{
				_node = nullptr;
			}
		}

		return *this;
	}

	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}

	bool operator==(const Self& s) const
	{
		return _node == s._node;
	}
};


template<class K, class T, class KeyOfT, class Hash>
class HashTable
{
	// 友元声明
	template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
	friend struct HTIterator;

	typedef HashNode<T> Node;
public:
	typedef HTIterator<K, T, T&, T*, KeyOfT, Hash> Iterator;
	typedef HTIterator<K, T, const T&, const T*, KeyOfT, Hash> ConstIterator;

	Iterator Begin()
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			if (_tables[i])
			{
				return Iterator(_tables[i], this);
			}
		}

		return End();
	}

	Iterator End()
	{
		return Iterator(nullptr, this);
	}

	ConstIterator Begin() const
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			if (_tables[i])
			{
				return ConstIterator(_tables[i], this);
			}
		}

		return End();
	}

	ConstIterator End() const
	{
		return ConstIterator(nullptr, this);
	}

	HashTable()
		:_tables(__stl_next_prime(1), nullptr)
		, _n(0)
	{}

	~HashTable()
	{
		for (size_t i = 0; i < _tables.size(); i++)
		{
			Node* cur = _tables[i];
			// 当前桶的节点重新映射挂到新表
			while (cur)
			{
				Node* next = cur->_next;
				delete cur;
				cur = next;
			}

			_tables[i] = nullptr;
		}
	}

	pair<Iterator, bool> Insert(const T& data)
	{
		KeyOfT kot;
		auto it = Find(kot(data));
		if (it != End())
			return { it, false };

		Hash hs;
		// 负载因子==1扩容
		if (_n == _tables.size())
		{
			//HashTable<K, V> newHT;
			//newHT._tables.resize(_tables.size()*2);
			//// 遍历旧表将所有值映射到新表
			//for (auto cur : _tables)
			//{
			//	while (cur)
			//	{
			//		newHT.Insert(cur->_kv);
			//		cur = cur->_next;
			//	}
			//}
			//_tables.swap(newHT._tables);

			vector<Node*> newtables(__stl_next_prime(_tables.size() + 1));
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				// 当前桶的节点重新映射挂到新表
				while (cur)
				{
					Node* next = cur->_next;

					// 插入到新表
					size_t hashi = hs(kot(cur->_data)) % newtables.size();
					cur->_next = newtables[hashi];
					newtables[hashi] = cur;

					cur = next;
				}

				_tables[i] = nullptr;
			}

			_tables.swap(newtables);
		}

		size_t hashi = hs(kot(data)) % _tables.size();
		// 头插
		Node* newNode = new Node(data);
		newNode->_next = _tables[hashi];
		_tables[hashi] = newNode;

		++_n;
		return { Iterator(newNode, this), true };
	}

	Iterator Find(const K& key)
	{
		KeyOfT kot;
		Hash hs;
		size_t hashi = hs(key) % _tables.size();
		Node* cur = _tables[hashi];
		while (cur)
		{
			if (kot(cur->_data) == key)
				return { cur, this };

			cur = cur->_next;
		}

		return End();
	}

	bool Erase(const K& key)
	{
		KeyOfT kot;
		Hash hs;
		size_t hashi = hs(key) % _tables.size();
		Node* prev = nullptr;
		Node* cur = _tables[hashi];
		while (cur)
		{
			if (kot(cur->_data) == key)
			{
				if (prev == nullptr)
				{
					_tables[hashi] = cur->_next;
				}
				else
				{
					prev->_next = cur->_next;
				}

				delete cur;

				return true;
			}

			prev = cur;
			cur = cur->_next;
		}

		return false;
	}

private:
	//vector<list<pair<K, V>>> _tables;
	vector<Node*> _tables;
	size_t _n = 0;  // 实际存储的数据个数

};

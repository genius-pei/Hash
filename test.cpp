#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<unordered_set>
#include<unordered_map>
#include<set>
using namespace std;

void test01()
{
	unordered_set<int> s1 = { 1,3,5,6,7,2 };
	s1.insert(11);
	auto it = s1.begin();
	while (it != s1.end())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;
	unordered_map<string, string> dict;
	dict.insert({ "apple","苹果" });
	dict.insert({ "orange","橘子" });
	dict.insert({ "banana","香蕉" });
	for (auto& [k, v] : dict)
	{
		cout << k << ":" << v << endl;
	}

}
int test02()
{
	const size_t N = 1000000;
	unordered_set<int> us;
	set<int> s;

	vector<int> v;
	v.reserve(N);
	srand(time(0));
	for (size_t i = 0; i < N; ++i)
	{
		//v.push_back(rand()); // N比较大时，重复值比较多
		v.push_back(rand() + i); // 重复值相对少
		//v.push_back(i); // 没有重复，有序
	}
	size_t begin1 = clock();
	for (auto e : v)
	{
		s.insert(e);
	}
	size_t end1 = clock();
	cout << "set insert:" << end1 - begin1 << endl;

	size_t begin2 = clock();
	us.reserve(N);
	for (auto e : v)
	{
		us.insert(e);
	}
	size_t end2 = clock();
	cout << "unordered_set insert:" << end2 - begin2 << endl;

	int m1 = 0;
	size_t begin3 = clock();
	for (auto e : v)
	{
		auto ret = s.find(e);
		if (ret != s.end())
		{
			++m1;
		}
	}
	size_t end3 = clock();
	cout << "set find:" << end3 - begin3 << "->" << m1 << endl;

	int m2 = 0;
	size_t begin4 = clock();
	for (auto e : v)
	{
		auto ret = us.find(e);
		if (ret != us.end())
		{
			++m2;
		}
	}
	size_t end4 = clock();
	cout << "unorered_set find:" << end4 - begin4 << "->" << m2 << endl;

	cout << "插入数据个数：" << s.size() << endl;
	cout << "插入数据个数：" << us.size() << endl << endl;

	size_t begin5 = clock();
	for (auto e : v)
	{
		s.erase(e);
	}
	size_t end5 = clock();
	cout << "set erase:" << end5 - begin5 << endl;

	size_t begin6 = clock();
	for (auto e : v)
	{
		us.erase(e);
	}
	size_t end6 = clock();
	cout << "unordered_set erase:" << end6 - begin6 << endl << endl;

	return 0;

}

int main()
{
	//test01();
	test02();
	return 0;
}
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void Vote_Counting(map<string, int> &Vote_List, const string &name)
{
	pair<std::map<string, int>::iterator, bool> ret;
	ret = Vote_List.insert(make_pair(name, 1));

	//있던 이름이면 +1
	if (ret.second == false)
	{
		Vote_List.find(name)->second++;
	}
	return;
}

bool cmp(pair<int, string> v1, pair<int, string> v2)
{
	return v1.first > v2.first;
}

vector<pair<int, string>> Sort_List(map<string, int> &Vote_Map)
{
	vector<pair<int, string>> Sort_Vector;
	map<string, int>::iterator m_it;

	//Map -> Vector
	for (m_it = Vote_Map.begin(); m_it != Vote_Map.end(); m_it++)
	{
		Sort_Vector.push_back(make_pair(m_it->second, m_it->first));
	}

	//Vector 정렬
	sort(Sort_Vector.begin(), Sort_Vector.end(), cmp);
	return Sort_Vector;
}

void main()
{
	ifstream fin;
	ofstream fout;
	string name;

	map<string, int> Vote_Map; //이름을 Key로 가진 Map
	vector<pair<int, string>> Value_Sort_Map; //Count로 정렬한 Vector

	fin.open("11.input.txt");
	fout.open("11.output.txt");

	if (fin.is_open())
	{
		while(fin >> name) 
		{
			//사람 이름 수집 (Map에 저장 Key : 이름, Value : Counting)
			Vote_Counting(Vote_Map, name);
		}

/*
		//Map에 제대로 들어갔는가 확인
		map<string, int>::iterator it;
		for (it = Vote_Map.begin() ; it != Vote_Map.end() ; it++)
		{
		printf("%s %d \n", it->first.c_str(), it->second);
		fout << "Sting : " << it->first << " / Counting : " << it->second << "\n";
		}
*/

		//Value 으로 정렬 -> Vector에 넣고 Value Sort
		Value_Sort_Map = Sort_List(Vote_Map);

/*
		//Vector에 제대로 들어갔는가 확인
		vector<pair<int, string>>::iterator v_it;
		for (v_it = Value_Sort_Map.begin() ; v_it != Value_Sort_Map.end() ; v_it++)
		{
			printf("%s %d \n", v_it->second.c_str(), v_it->first);
			fout << "Sting : " << v_it->second << " : Counting : " << v_it->first << "\n";
		}
*/

		//상위 String 출력
		vector<pair<int, string>>::iterator v_it;
		int count = 0, pre_count = 0;
		for (v_it = Value_Sort_Map.begin(); v_it != Value_Sort_Map.end(); v_it++)
		{
			count = v_it->first;
			if (count < pre_count)
			{
				break;
			}
			//printf("%s %d \n", v_it->second.c_str(), v_it->first);
			fout << v_it->second << "\n";
			pre_count = count;
		}
	}
	else
	{
		printf("File : Not Open \n");
	}

	fin.close();
	fout.close();

	return;
}
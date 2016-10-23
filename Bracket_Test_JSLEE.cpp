#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>

using namespace std;

typedef struct Bracket_Info
{
	char Bracket;
	char Operator;
}Bracket_Info;

bool Next_Postfix(stack<Bracket_Info> &bracket_stack, const Bracket_Info &info, string &post_string, stack<int> &plus_count_List)
{
	bool plus_check = false;

	switch (bracket_stack.top().Bracket) //각자 상황에 따른 예외를 처리
	{
	case '(':
		if (info.Bracket == '}' || info.Bracket == ']') //괄호 구성 오류
		{
			return false;
		}
		else if (info.Bracket == ')')
		{
			if(bracket_stack.top().Operator != '*') //이전 괄호가 *로 시작하는 것이 아니라면 ex) ()
			{ 
				plus_count_List.top()++; //더하기 정보 추가
			}
			else //이전 괄호가 * 정보라면 해당 괄호 안의 정보를 나열
			{
				plus_check = true;
			}
			bracket_stack.pop();
		}
		else
		{
			if (bracket_stack.top().Operator != '*') //이전 괄호가 *하기 정보가 아니라면 ex) ([]{
			{
				bracket_stack.top().Operator = '*';
				plus_count_List.top()++;
				plus_count_List.push(0); //괄호 안의 새로운 + 정보 수집
			}
			bracket_stack.push(info);
		}
		break;
	case '{':
		if (info.Bracket == ')' || info.Bracket == ']') //괄호 구성 오류
		{
			return false;
		}
		else if (info.Bracket == '}')
		{
			if (bracket_stack.top().Operator != '*')
			{
				plus_count_List.top()++;
			}
			else
			{
				plus_check = true;
			}
			bracket_stack.pop();
		}
		else
		{
			if (bracket_stack.top().Operator != '*')
			{
				bracket_stack.top().Operator = '*';
				plus_count_List.top()++;
				plus_count_List.push(0);
			}
			bracket_stack.push(info);
		}
		break;
	case '[':
		if (info.Bracket == ')' || info.Bracket == '}') //괄호 구성 오류
		{
			return false;
		}
		else if (info.Bracket == ']')
		{
			if (bracket_stack.top().Operator != '*')
			{
				plus_count_List.top()++;
			}
			else
			{
				plus_check = true;
			}
			bracket_stack.pop();
		}
		else
		{
			if (bracket_stack.top().Operator != '*')
			{
				bracket_stack.top().Operator = '*';
				plus_count_List.top()++;
				plus_count_List.push(0);
			}
			bracket_stack.push(info);
		}
		break;
	default :
		return false;
	}

	//조건 시, Plus 붙이기
	if (plus_check == true)
	{
		for (int i = 0; i < plus_count_List.top() - 1; i++)
		{
			post_string += '+';
		}
		post_string += '*';
		plus_count_List.pop();
	}

	return true;
}

bool Bracket_To_Postfix(const string &problem, string &post_string)
{
	stack<Bracket_Info> bracket_stack;
	stack<int> plus_count_list;
	int len = problem.length();
	

	//시작이 닫는 칸이면 오류
	if (problem.at(0) == ')' || problem.at(0) == '}' || problem.at(0) == ']')
	{
		return false;
	}
	
	Bracket_Info tmp;
	for (int i = 0; i < len; i++)
	{
		tmp.Bracket = problem.at(i);
		tmp.Operator = '0';
		if (tmp.Bracket == '(')
		{
			post_string += '1';
		}
		else if (tmp.Bracket == '{')
		{
			post_string += '2';
		}
		else if (tmp.Bracket == '[')
		{
			post_string += '3';
		}
		
		if (bracket_stack.size() == 0) //처음 Setting
		{
			//괄호 넣고
			bracket_stack.push(tmp);
			if (i == 0)
			{
				plus_count_list.push(0);
			}
			continue;
		}
		
		// 이전 이후 괄호에 따른 정보 정리
		if (!Next_Postfix(bracket_stack, tmp, post_string, plus_count_list)) //문자열 오류시 전달
		{
			return false;
		}
	}

	//Stack에 괄호가 남아 있어도 오류
	if (bracket_stack.size() != 0)
	{
		return false;
	}

	//나머지 Plus 추가
	for (int i = 0; i < plus_count_list.top() - 1; i++)
	{
		post_string += '+';
	}

	return true;
}

int post_calc(const string &post_string)
{
	stack<int> int_stack;
	int left, right, result = 0;

	for (int i = 0; i < post_string.length(); i++)
	{
		if (isdigit(post_string.at(i))) //숫자는 스택에 push
		{
			string num = "";
			num += post_string.at(i);

			int_stack.push(atoi(num.c_str()));
		}
		else if (post_string.at(i) == '+' || post_string.at(i) == '*')  //연산자는 두 수를 꺼내서 계산 후 push
		{
			right = int_stack.top();
			int_stack.pop();
			left = int_stack.top();
			int_stack.pop();

			switch (post_string.at(i)) //+,*는 나머지 결합법칙 적용
			{
			case '+':
				int_stack.push((left % 100000000) + (right % 100000000));
				break;
			case '*':
				int_stack.push((left % 100000000) * (right % 100000000));
				break;
			default:
				break;
			}
		}
	}

	//결과 도출
	if (int_stack.size())
	{
		result = int_stack.top();
	}
	else
	{
		result = 0;
	}

	return result;
}

void main()
{
	ifstream fin;
	ofstream fout;
	string problem;
	string post_string = "";
	int problem_count;

	fin.open("10.input.txt");
	fout.open("10.output.txt");

	if (fin.is_open())
	{
		//문제 수 받기
		fin >> problem_count;

		for (int i = 0; i < problem_count; i++)
		{
			//입력
			fin >> problem;

			//괄호식을 후위표기법으로 -> 오류 시 0 출력
			if (!Bracket_To_Postfix(problem, post_string))
			{
				//printf("0\n");
				fout << "0\n";
				continue;
			}

			//printf("%s\n", post_string.c_str());

			//후위표기법 -> Stack으로 계산 -> 출력
			//printf("%d \n", post_calc(post_string) % 100000000);
			fout << post_calc(post_string) % 100000000 << "\n";

			post_string.clear();
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


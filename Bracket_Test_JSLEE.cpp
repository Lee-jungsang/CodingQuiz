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

	switch (bracket_stack.top().Bracket) //���� ��Ȳ�� ���� ���ܸ� ó��
	{
	case '(':
		if (info.Bracket == '}' || info.Bracket == ']') //��ȣ ���� ����
		{
			return false;
		}
		else if (info.Bracket == ')')
		{
			if(bracket_stack.top().Operator != '*') //���� ��ȣ�� *�� �����ϴ� ���� �ƴ϶�� ex) ()
			{ 
				plus_count_List.top()++; //���ϱ� ���� �߰�
			}
			else //���� ��ȣ�� * ������� �ش� ��ȣ ���� ������ ����
			{
				plus_check = true;
			}
			bracket_stack.pop();
		}
		else
		{
			if (bracket_stack.top().Operator != '*') //���� ��ȣ�� *�ϱ� ������ �ƴ϶�� ex) ([]{
			{
				bracket_stack.top().Operator = '*';
				plus_count_List.top()++;
				plus_count_List.push(0); //��ȣ ���� ���ο� + ���� ����
			}
			bracket_stack.push(info);
		}
		break;
	case '{':
		if (info.Bracket == ')' || info.Bracket == ']') //��ȣ ���� ����
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
		if (info.Bracket == ')' || info.Bracket == '}') //��ȣ ���� ����
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

	//���� ��, Plus ���̱�
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
	

	//������ �ݴ� ĭ�̸� ����
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
		
		if (bracket_stack.size() == 0) //ó�� Setting
		{
			//��ȣ �ְ�
			bracket_stack.push(tmp);
			if (i == 0)
			{
				plus_count_list.push(0);
			}
			continue;
		}
		
		// ���� ���� ��ȣ�� ���� ���� ����
		if (!Next_Postfix(bracket_stack, tmp, post_string, plus_count_list)) //���ڿ� ������ ����
		{
			return false;
		}
	}

	//Stack�� ��ȣ�� ���� �־ ����
	if (bracket_stack.size() != 0)
	{
		return false;
	}

	//������ Plus �߰�
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
		if (isdigit(post_string.at(i))) //���ڴ� ���ÿ� push
		{
			string num = "";
			num += post_string.at(i);

			int_stack.push(atoi(num.c_str()));
		}
		else if (post_string.at(i) == '+' || post_string.at(i) == '*')  //�����ڴ� �� ���� ������ ��� �� push
		{
			right = int_stack.top();
			int_stack.pop();
			left = int_stack.top();
			int_stack.pop();

			switch (post_string.at(i)) //+,*�� ������ ���չ�Ģ ����
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

	//��� ����
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
		//���� �� �ޱ�
		fin >> problem_count;

		for (int i = 0; i < problem_count; i++)
		{
			//�Է�
			fin >> problem;

			//��ȣ���� ����ǥ������� -> ���� �� 0 ���
			if (!Bracket_To_Postfix(problem, post_string))
			{
				//printf("0\n");
				fout << "0\n";
				continue;
			}

			//printf("%s\n", post_string.c_str());

			//����ǥ��� -> Stack���� ��� -> ���
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


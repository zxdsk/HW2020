#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <sstream>
#include <fstream>
#include <deque>
#include <algorithm>
#include <ctime>

using namespace std;

//#define max_size 30005 //�ڽӾ����С
#define min_cir 3 //��·�ڵ���С����
#define max_cir 7 //��·�ڵ�������

//bool graph_matrix[max_size][max_size]; //graph_matrix: �ڽӾ���
//int vertex[max_size], v_visit[max_size]; //vertex: �ڵ�����, v_visit: �ڵ����״̬����
unordered_map<int, vector<int>> edge_mapping;
unordered_map<int, int> vertex_visit;
vector<deque<int>> all_path, result; //all_path: δ����Ļ�·���� result: �����Ļ�·����

									 /*=======================================================================
									 load_data: ��txt�ı��ж�ȡ���ݣ��������ڵıߺͽڵ��ڷֱ�graph_matrix,
									 vertex����1
									 =======================================================================*/
void load_data(string path)
{
	ifstream infile(path.c_str());
	string line;

	if (!infile) {
		cout << "�ļ���ʧ��!" << endl;
		exit(0);
	}

	unordered_map<int, vector<int>>::iterator edge_iter;
	unordered_map<int, int>::iterator vertex_iter;
	while (infile)
	{
		getline(infile, line);
		stringstream sin(line);
		char ch;
		int v, u, w;
		sin >> v >> ch >> u >> ch >> w;
		edge_iter = edge_mapping.find(v);
		if (edge_mapping.empty() || edge_iter == edge_mapping.end())
		{
			vector<int> edge_set;
			edge_set.push_back(u);
			edge_mapping.insert(pair<int, vector<int>>(v, edge_set));
		}
		else if (edge_iter != edge_mapping.end())
			edge_mapping[v].push_back(u);

		vertex_iter = vertex_visit.find(v);
		if (vertex_visit.empty() || vertex_iter == vertex_visit.end())
		{
			vertex_visit.insert(pair<int, int>(v, 0));
		}
	}
	infile.close();
}

/*=======================================================================
circle_sort: ƽ�Ƹı价·unsort_path�нڵ�˳����С�Ľڵ������λ
=======================================================================*/
deque<int> circle_sort(deque<int> &unsort_path)
{
	deque<int> sort_path;
	int min_val = 0; //��С�ڵ�����
	for (int i = 0; i < unsort_path.size(); i++)
	{
		if (unsort_path[i] < unsort_path[min_val])
			min_val = i;
	}
	//ƽ�Ƹı�˳��
	for (int k = min_val; k < (unsort_path.size() + min_val); k++)
	{
		sort_path.push_back(unsort_path[k % unsort_path.size()]);
	}
	return sort_path;
}

/*=======================================================================
save_path: ��·���ڵ�ջcur_path��ȡ����·��vΪջ��Ԫ�أ�iΪ��·���λ�ã�
����· i->...->...->v->i
=======================================================================*/
void save_path(stack<int> cur_path, int v, int i)
{
	stack<int> copy;
	deque<int> unsort_path, sort_path;
	copy = cur_path;
	while (copy.top() != i)
	{
		unsort_path.push_front(copy.top());
		copy.pop();
	}
	unsort_path.push_front(copy.top());
	if (unsort_path.size() < 3 || unsort_path.size() > 7) return;
	sort_path = circle_sort(unsort_path); //�Ի�·�ڵ�����
	all_path.push_back(sort_path); //��������all_path
}

/*=======================================================================
DFS: ͨ���ݹ齫��ǰ·���Ľڵ�ѹ��ջcur_path����Ѱ�ҵ�ǰ·���еĻ�·
v_visit��ȡֵ������:
ȡ0: �ڵ�δ���ʹ���
ȡ1: �ڵ��ڵ�ǰ·��(ջ)�У�
ȡ2���ڵ��������ʹ��������ڵ�ǰ·����
=======================================================================*/
void DFS(stack<int>& cur_path, int v)
{
	cur_path.push(v);
	if (vertex_visit[v] == 0) vertex_visit[v] = 1;
	for (int i = 0; i < edge_mapping[v].size(); i++)
	{
		//��(v, i)���ڣ����ڵ�i���ڵ�ǰ·��
		if (vertex_visit[edge_mapping[v][i]] != 1)
		{
			vertex_visit[edge_mapping[v][i]] = 1;
			DFS(cur_path, edge_mapping[v][i]); //·�����죬�����ݹ�
		}
		//��(v, i)�����ҽڵ�i�ڵ�ǰ·�����γɻ�·
		else if (vertex_visit[edge_mapping[v][i]] == 1)
		{
			save_path(cur_path, v, edge_mapping[v][i]); //���滷·
		}
	}
	vertex_visit[cur_path.top()] = 2;
	cur_path.pop();
}

/*=======================================================================
find_circle: Ѱ��graph_matrix, vertex�����л�·
=======================================================================*/
void find_circle()
{
	unordered_map<int, int>::iterator vertex_iter;
	for (vertex_iter = vertex_visit.begin(); vertex_iter != vertex_visit.end(); vertex_iter++)
	{
		if (vertex_iter->second == 0)
		{
			stack<int> cur_path;
			DFS(cur_path, vertex_iter->first);
		}
	}
}

/*=======================================================================
circle_unique_and_sort: ȥ���ظ���·��������ĿҪ�����򣬽������result��
=======================================================================*/
void circle_unique_and_sort()
{
	sort(all_path.begin(), all_path.end());
	all_path.erase(unique(all_path.begin(), all_path.end()), all_path.end());

	for (int i = min_cir; i <= max_cir; i++)
	{
		for (int j = 0; j < all_path.size(); j++)
		{
			if (all_path[j].size() == i)
				result.push_back(all_path[j]);
		}
	}
}

/*=======================================================================
display: ��ʾ���
=======================================================================*/
void display()
{
	cout << "����ͼ�л�·������Ϊ��" << result.size() << endl;
	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < result[i].size(); j++)
		{
			cout << result[i][j] << " ";
		}
		cout << endl;
	}
}

/*=======================================================================
save_data: ���ո�ʽ��������ָ��·��path
=======================================================================*/
void save_data(string path) {
	string line;
	ofstream outfile(path.c_str());

	if (!outfile.is_open()) {
		cout << "�ļ�����ʧ�ܣ�" << endl;
	}

	outfile << result.size() << endl;
	for (int i = 0; i < result.size(); ++i) {
		for (int j = 0; j < result[i].size(); ++j) {
			outfile << result[i][j];
			if (j != result[i].size() - 1)
				outfile << ',';
		}
		outfile << endl;
	}
	outfile.close();
}

int main()
{
	clock_t start, finish, t1, t2;
	start = clock();
	string path = "test_data.txt";

	load_data(path);
	t1 = clock();
	find_circle();
	circle_unique_and_sort();
	//display();
	save_data("result_1.txt");
	finish = clock();

	cout << (double)(t1 - start) / CLOCKS_PER_SEC << endl;
	cout << (double)(finish - t1) / CLOCKS_PER_SEC << endl;

	system("pause");
	return 0;
}
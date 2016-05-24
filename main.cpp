#include "AstarNode.h"
#include "AstarSearch.h"
#include <iomanip>

#define PUZZLENUMS 25
#define SETPS 100
bool haveSolve(int *Tiles, int len);
void init();
void Input(State& goalState, Node& initNode);
void RandomInput(State& goalState, Node& initNode);
void Output(int optimalCost, int generatedStates);

int main()
{
	//������ʼ��
	init();
	State goalState;
	Node  initNode;
	int result;

	//Input(goalState, initNode);
	RandomInput(goalState, initNode);
	for (int i = 2;i <= 3;i++) {
		//����������ʽ�����ֱ��������������
		g_k = i;
		AStarSearch as = AStarSearch(goalState, initNode);
		result = as.Solve();
		cout << "����ʽ����" << i << ":" << endl;
		Output(result, g_states);
		g_states = 1;
	}

	system("pause");
	return 0;
}

bool haveSolve(int *Tiles, int len) {
	return true;
}

void init() {
	/**************************************************************************
	int g_k;			// ѡ������ʽ����
	int g_n;			// n-Puzzle
	int g_countTiles;	//��ש���������ո��ש
	int g_states;		//������¼״̬��������Ҫ����
	***************************************************************************/
	//���ø������,
	g_countTiles = PUZZLENUMS;
	//ѡ������ʽ����,Ĭ��Ϊ2
	g_k = 2;
	//����n
	g_n = sqrt(g_countTiles);
}

void Input(State& goalState, Node& initNode)
{
	int tile;
	int* initTiles;
	int* goalTiles;
	//�����ڴ�
	initTiles = new int[g_countTiles];
	goalTiles = new int[g_countTiles];

	//����
	cout << "���뿪ʼ״̬[0Ϊ�ո�����λ��]" << endl;
	for (int i = 0; i < g_countTiles; i++)
	{
		cin >> tile;
		initTiles[i] = tile;
	}

	if (!haveSolve(initTiles, g_countTiles)) {
		//����޽�Ļ���������
		cout << "����������޽⣬����������" << endl;
		for (int i = 0; i < g_countTiles; i++)
		{
			cin >> tile;
			initTiles[i] = tile;
		}
	}

	cout << "����Ŀ��״̬" << endl;
	for (int i = 0; i < g_countTiles; i++)
	{
		cin >> tile;
		goalTiles[i] = tile;
	}

	goalState.SetTiles(goalTiles);

	initNode.m_state.SetTiles(initTiles);
	initNode.ComputeF(goalState);

	g_states = 1;


	delete[] initTiles;
	delete[] goalTiles;
}

void RandomInput(State& goalState, Node& initNode)
{
	//�������һ���н�����룬���״̬�̶�Ϊ0��24
	int tile;
	int* initTiles;
	int* goalTiles;
	//�����ڴ�
	initTiles = new int[g_countTiles];
	goalTiles = new int[g_countTiles];
	srand(time(0));
	//�������puzzle�͹̶����еĴ�0��24��Ŀ��puzzle
	for (int i = 0; i < g_countTiles; i++)
	{
		initTiles[i] = i;
		goalTiles[i] = i;
	}

	//����ƶ�initTiles�����Ϊ�ƶ�100������
	int blankpos = 0;
	int steplen[4] = { -1,-5,1,5 };
	for (int i = 0;i <= SETPS ;i++) {
		int randDirect = rand() % 4;
		int goalpos = blankpos + steplen[randDirect];
		if (goalpos >= 0 && goalpos < g_countTiles) {
			if ((blankpos + 1) % g_n == 1 && randDirect == 0 ) {
				//����߽�ʱ�����������ƶ�,�������½���
				i--;
				continue;
			}
			if ((blankpos + 1) % g_n == 0 && randDirect == 2) {
				//���ұ߽�ʱ�����������ƶ�,�������½���
				i--;
				continue;

			}
			if (blankpos + 1 <= g_n  && randDirect == 1) {
				//���ϱ߽�ʱ�����������ƶ�,�������½���
				i--;
				continue;
			}
			if (blankpos + 1 >= g_countTiles - g_n && randDirect == 3) {
				//���±߽�ʱ�����������ƶ�,�������½���
				i--;
				continue;
			}
			//���û���߳��߽�Ļ�,������
			initTiles[blankpos] = initTiles[goalpos];
			initTiles[goalpos] = 0;
			blankpos = goalpos;
			/*for (int i = 0; i < g_countTiles; i++)
			{
				cout << setw(2) << initTiles[i] << " ";
				if ((i + 1) % g_n == 0) {
					cout << endl;
				}
			}*/
			continue;
		}
		i--;
	}

	cout << "���ɵ����puzzle����" << endl;
	for (int i = 0; i < g_countTiles; i++)
	{
		cout<<setw(2)<<initTiles[i]<<" ";
		if ((i + 1)% g_n == 0) {
			cout << endl;
		}
	}

	cout << "Ŀ��״̬����:" << endl;
	for (int i = 0; i < g_countTiles; i++)
	{
		cout << setw(2)<< goalTiles[i] << " ";
		if ((i + 1) % g_n == 0) {
			cout << endl;
		}
	}

	goalState.SetTiles(goalTiles);

	initNode.m_state.SetTiles(initTiles);
	initNode.ComputeF(goalState);

	g_states = 1;

	delete[] initTiles;
	delete[] goalTiles;
}



void Output(int optimalCost, int generatedStates)
{
	//����������Ĳ�����״̬��
	cout << "����:" << optimalCost << " ״̬��:" << generatedStates << endl;
}

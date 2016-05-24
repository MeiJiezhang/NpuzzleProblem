#pragma once
#include "AstarNode.h"
class AStarSearch
{
private:

	/*
	���ڱȽ������ڵ�gֵ�����f�����Ƚ�pathCost
	*/
	class Compare_f
	{
	public:
		bool operator() (const Node *x, const Node *y) const
		{
			if (x->m_f == y->m_f)
				return x->m_pathCost < y->m_pathCost;
			else
				return x->m_f > y->m_f;
		}
	};

	/*
	�����ж������ڵ��״̬�Ƿ�һ��
	*/
	class Compare_state
	{
	public:
		bool operator() (const Node* x, const Node* y) const
		{
			for (int i = 0; i < g_countTiles; i++)
				if (x->m_state.m_tiles[i] != y->m_state.m_tiles[i])
					return x->m_state.m_tiles[i] < y->m_state.m_tiles[i];

			return false;
		}
	};

public:
	State m_goalState;
	priority_queue<Node*, vector<Node*>, Compare_f> m_openNodes;		// open nodes
	set<Node*, Compare_state> m_allNodes;								// generated nodes

	AStarSearch(const State& goalState, Node& initNode) : m_goalState(goalState)
	{
		//��ʼ�ڵ�û�и��׽ڵ�
		initNode.m_state.m_parentAction = NA;

		m_openNodes.push(&initNode);
		m_allNodes.insert(&initNode);
	}

	~AStarSearch()
	{
		//�����ͷ���Դ
	}

	/*
	���N-puzzle���⣬�������-1�����޷����
	*/
	int Solve()
	{
		vector<State*> successors;		// ��ʱ�洢ĳ�ڵ���Ϊ���ڵ�ʱ���ɴﵽ�Ľڵ�״̬
		set<Node*>::iterator existed;	// ����½ڵ��Ƿ��Ѵ���
		Node* newNode;
		int newPathCost;

		//��������б�Ϊ�վ�һֱ���
		while (!m_openNodes.empty())
		{
			//�����ʱ�б�
			for (int i = 0; i < successors.size(); i++)
				delete successors[i];
			successors.clear();

			// ���F()ֵ��͵Ľڵ�
			Node *bestNode = m_openNodes.top();
			m_openNodes.pop();

			// �����ǰ����Ѿ���Ŀ��ڵ㣬���ص�ǰ����ʵ�ʴ���
			if (bestNode->m_state.IsSameState(m_goalState))
				return bestNode->m_pathCost;

			// ����������������Ĳ���

			// Ϊ��ǰ���½ڵ������ɵ�����ӽڵ�
			newPathCost = bestNode->m_pathCost + 1;
			bestNode->m_state.GenerateSuccessors(successors);

			//�������ӽڵ�״̬
			for (int i = 0; i < successors.size(); i++)
			{
				newNode = new Node(*successors[i], newPathCost);

				//�ظ�״̬��⣬���Ƿ��Ѿ����ֹ��ýڵ�
				existed = m_allNodes.find(newNode);
				if (existed != m_allNodes.end())
				{
					//����Ѿ����ֹ��ýڵ㣬�ж���ڵ�ԭ����·�������ڵ�����ĸ���
					if ((*existed)->m_pathCost <= newPathCost)
					{
						//����Ѵ��ڵĽڵ�����ڵĽڵ���ã����Ѹ��٣��Ͳ����κβ������ж���һ���ӽڵ�
						continue;
					}
					else
					{
						m_allNodes.erase(existed);
					}
				}
				else g_states++;

				//�ڵ�ͨ������
				newNode->ComputeF(m_goalState);

				m_openNodes.push(newNode);
				m_allNodes.insert(newNode);
			}

			//�Ե��������������ƣ������������������ж�Ϊʧ��
			if ((g_n == 3 && g_states >= 250000) ||
				(g_n == 4 && g_states >= 600000) ||
				(g_n >= 5 && g_states >= 1000000))
				return -1;
		}

		return -1;

	}
};
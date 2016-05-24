#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>
#include <set>
#include <ctime>
#include <queue>

using namespace std;

int g_k;			// ѡ������ʽ����
int g_n;			// n-Puzzle
int g_countTiles;	//��ש���������ո��ש
int g_states;		//������¼״̬��������Ҫ����

enum Move { MOVE_UP = -1, MOVE_DOWN = 1, MOVE_LEFT = -2, MOVE_RIGHT = 2, NA = 0 };

class State
{
public:
	int* m_tiles;
	Move  m_parentAction;

private:
	/*
	��ÿմ�ש0��λ��
	*/
	int GetBlankTilePosition()
	{
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] == 0)
				return i;
	}	

	/*
	����moveָ���ƶ���ǰ��ש
	*/
	bool MoveBlankTile(Move move)
	{
		int blankPos = GetBlankTilePosition();
		int newPos;

		// ���ָ���Ƿ���Ч���Ƿ��ڱ߽��ϻ������ƶ�
		if (move == MOVE_UP && blankPos / g_n == 0 ||
			move == MOVE_DOWN && blankPos / g_n == g_n - 1 ||
			move == MOVE_LEFT && blankPos % g_n == 0 ||
			move == MOVE_RIGHT && blankPos % g_n == g_n - 1)
			return false;

		// �ƶ��󣬻���µĿմ�ש��λ��
		if (move == MOVE_UP || move == MOVE_DOWN)
			newPos = blankPos + move*g_n;
		else
			newPos = blankPos + move / 2;

		// �޸Ĵ�ש�����е�ֵ
		m_tiles[blankPos] = m_tiles[newPos];
		m_tiles[newPos] = 0;
		return true;
	}		

	/*
	Ϊ��ǰ�ڵ������ɴﵽ�Ľڵ�״̬
	*/
	void AddSuccessor(vector<State*>& successors, Move move)
	{
		State* newState = new State(m_tiles);
		if (newState->MoveBlankTile(move))
		{
			newState->m_parentAction = move;
			successors.push_back(newState);
		}
		else
			delete newState;
	}	

public:

	/*
	����Tiles��ֵ��Ϊ�������
	*/
	void SetTiles(int* tiles)
	{
		m_tiles = new int[g_countTiles];
		for (int i = 0; i < g_countTiles; i++)
			m_tiles[i] = tiles[i];
	}

	State(int* tiles)
	{
		SetTiles(tiles);
	}

	State(const State& state)
	{
		SetTiles(state.m_tiles);
	}

	State() {}

	~State()
	{
		delete[] m_tiles;
	}


	/*
	�ж��Ƿ���ͬһ״̬
	*/
	bool IsSameState(const State& state) const
	{
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] != state.m_tiles[i])
				return false;

		return true;
	}  



	/*
	�ҳ����е�ǰ���ɵ����״̬
	*/
	void GenerateSuccessors(vector<State*>& successors)
	{
		successors.clear();
		// ���ĸ��������������ұ���������
		if (m_parentAction != MOVE_DOWN) AddSuccessor(successors, MOVE_UP);
		if (m_parentAction != MOVE_UP) AddSuccessor(successors, MOVE_DOWN);
		if (m_parentAction != MOVE_RIGHT) AddSuccessor(successors, MOVE_LEFT);
		if (m_parentAction != MOVE_LEFT) AddSuccessor(successors, MOVE_RIGHT);
	}

	/*
	����ʽ����1�������λ�Ĵ�ש����Ŀ���������ʽ������Ч�����ģ�
	*/
	int HMisplacedTiles(const State& goalState)
	{
		int h = 0;
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
				h++;

		return h;
	}	/* -----  end of function HMisplacedTiles  ----- */

	/*
	����ʽ����2�����������ש����Ŀ��Ŀ��״̬��Ӧ��Ŀ�ľ��Բ���ܺ�
	*/
	int HManhattanDistance(const State& goalState)
	{
		int h = 0;
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
				for (int j = 0; j < g_countTiles; j++)
					if (m_tiles[i] == goalState.m_tiles[j])
					{
						h += abs(float(i%g_n - j%g_n)) + abs(float(i / g_n - j / g_n));
						break;
					}

		return h;
	}	

	/*
	����ʽ����3�����������ש���Ӧ����ȷλ�õľ��룬����ֱ��������������ľ���������
	*/
	int HCustom(const State& goalState)
	{
		int h = 0;
		for (int i = 0; i < g_countTiles; i++)
			if (m_tiles[i] != 0 && m_tiles[i] != goalState.m_tiles[i])
				for (int j = 0; j < g_countTiles; j++)
					if (m_tiles[i] == goalState.m_tiles[j])
					{
						h += sqrt((i%g_n - j%g_n)*(i%g_n - j%g_n) + (i / g_n - j / g_n)*(i / g_n - j / g_n));
						break;
					}

		return h;
	}

	/*
	������ѡ�������ʽ��������������ʽ����
	*/
	int H(const State& goalState)
	{
		if (g_k == 1)
			return HMisplacedTiles(goalState);
		else if (g_k == 2)
			return HManhattanDistance(goalState);
		else if (g_k == 3)
			return HCustom(goalState);
	}
};

class Node
{
public:
	State m_state;
	int   m_pathCost;  //�ڵ�������·������
	int   m_f;

	Node(State state, int pathCost) : m_state(state), m_pathCost(pathCost) {}

	Node() : m_pathCost(0) {}

	void ComputeF(const State& goalState)
	{
		m_f = m_pathCost + m_state.H(goalState);
	}
};
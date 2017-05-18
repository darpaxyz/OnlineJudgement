#include<iostream>
using namespace std;
#define MaxVex 100
#define INFINITY 65536
typedef int VertexTpye;
typedef int EdgeType;
int visited[MaxVex];

typedef struct {
	VertexTpye vexs[MaxVex];
	EdgeType arc[MaxVex][MaxVex];
	int numVertexes, numEdges;
}MGraph;

//������ȱ�������Ҫ��ѭ������
typedef struct {
	int data[MaxVex];
	int front, rear;
}Queue;

//��ʼ������
void init_queue(Queue* Q)
{
	Q->front = Q->rear = 0;
}

//���
void enqueue(Queue* Q, int e)
{
	if ((Q->rear + 1) % MaxVex == Q->front)
	{
		return;
	}
	Q->data[Q->rear] = e;
	Q->rear = (Q->rear + 1) % MaxVex;
}

//�п�
bool check_empty(Queue* Q)
{
	if (Q->front == Q->rear)
	{
		return true;
	}
	else {
		return false;
	}
}

//����
void dequeue(Queue* Q, int* e)
{
	if (Q->front == Q->rear)
	{
		return;
	}
	*e = Q->data[Q->front];
	Q->front = (Q->front + 1) % MaxVex;
}

//����ͼ���ڽӾ���
void create_graph(MGraph* G)
{
	cin >> G->numVertexes >> G->numEdges;

	//Ԥ�ýڵ�ֵ
	for (int i = 0; i < G->numVertexes; i++)
	{
		G->vexs[i] = i + 1;
	}

	//Ԥ���ڽӾ���
	for (int i = 0; i < G->numVertexes; i++)
	{
		for (int j = 0; j < G->numVertexes; j++)
		{
			G->arc[i][j] = INFINITY;
		}
	}
	//������ͨ������
	for (int i,j,k = 0; k < G->numEdges; k++)
	{
		cin >> i >> j;
		G->arc[i-1][j-1] = 1;
		G->arc[j-1][i-1] = G->arc[i-1][j-1];
	}
}

//���,������
void display_graph(MGraph* G)
{
	for (int i = 0; i < G->numVertexes; i++)
	{
		for (int j = 0; j < G->numVertexes; j++)
		{
			cout << G->arc[i][j];
		}
		cout << endl;
	}
}

//ͼ��������ȱ�������һ��·
void DFS(MGraph* G, int i, Queue* temp1)
{
	visited[i] = true;
	//cout << G->vexs[i];
	enqueue(temp1, G->vexs[i]);

	for (int j = 0; j < G->numVertexes; j++)
	{
		if (G->arc[i][j] != INFINITY && !visited[j])
		{
			DFS(G, j, temp1);
		}
	}
}

//������������������нڵ�
void DFS_traversal(MGraph* G)
{
	Queue temp1;//���ڴ洢���ʼ�¼�Ķ���
	init_queue(&temp1);

	for (int i = 0; i < G->numVertexes; i++)
	{
		visited[i] = false;
	}

	for (int i = 0; i < G->numVertexes; i++)
	{
		if (!visited[i])
		{
			DFS(G, i, &temp1);
		}
	}

	for (int i = 0; i < G->numVertexes-1; i++)
	{
		int output;
		dequeue(&temp1, &output);
		cout << output << " ";
	}
	int output;
	dequeue(&temp1, &output);
	cout << output << endl;
}

//ͼ�Ĺ�����ȱ���
void BFS_traversal(MGraph* G)
{
	Queue Q;
	init_queue(&Q);
	Queue temp2;//���ڴ洢���ʼ�¼�Ķ���
	init_queue(&temp2);

	for (int i = 0; i < G->numVertexes; i++)
	{
		visited[i] = false;
	}

	for (int i = 0; i < G->numVertexes; i++)
	{
		if (!visited[i])
		{
			visited[i] = true;
			//cout << G->vexs[i];//�����ǰ��㣬���������
			enqueue(&temp2, G->vexs[i]);
			enqueue(&Q, i);

			while (!check_empty(&Q))
			{
				dequeue(&Q, &i);
				for (int j = 0; j < G->numVertexes; j++)
				{
					if (!visited[j] && G->arc[i][j] != INFINITY)
					{
						visited[j] = true;
						//cout << G->vexs[j];
						enqueue(&temp2, G->vexs[j]);
						enqueue(&Q, j);
					}
				}
			}
		}
	}
	for (int i = 0; i < G->numVertexes - 1; i++)
	{
		int output;
		dequeue(&temp2, &output);
		cout << output << " ";
	}
	int output;
	dequeue(&temp2, &output);
	cout << output << endl;
}
int main()
{
	MGraph G;
	create_graph(&G);
	DFS_traversal(&G);
	BFS_traversal(&G);
	system("pause");
	return 0;
}
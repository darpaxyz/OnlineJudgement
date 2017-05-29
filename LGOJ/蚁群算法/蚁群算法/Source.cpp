/* #include "stdafx.h" */
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

const int	N_ANT_COUNT = 60;           /* ����������һ��ȡֵԭ��Ϊ���������� / �������� = 1.5���� */
const int	N_CITY_COUNT = 92;           /* �������� */
const int	N_IT_COUNT = 10000;        /* ���������������������� */


const double	DB_Q = 100;          /* �ܵ���Ϣ�� */
const double	DB_ALPHA = 1;            /* ��Ϣ����Ҫ�̶� */
const double	DB_BETA = 3;            /* �����Խ������������Ϣ�ش�ĵط��ߵĸ��ʾ�Խ�� */

const double DB_ROU = 0.5;                      /* ������Ϣ�ػӷ��ٶ� */


int besttour[N_CITY_COUNT];                     /* ���·���б� */

												/* ���ָ����Χ�ڵ�һ������� */
double rnd(int low, double uper)
{
	double p = (rand() / (double)RAND_MAX) * ((uper)-(low)) + (low);
	return(p);
};

/* ���ָ�����޷�Χ�ڵ�һ������� */
int rnd(int uper)
{
	return(rand() % uper);
};

/* tsp��ͼ��Ϣ����������Ϣ�أ����о��룬����Ϣ�ر仯���� */
class GInfo
{
public:
	double	m_dDeltTrial[N_CITY_COUNT][N_CITY_COUNT];       /* ��ʱ������Ϣ�أ����»�����Ϣ�ص�ʱ��ʹ�ã�ÿֻ����������������к�ʼ���� */
	double	m_dTrial[N_CITY_COUNT][N_CITY_COUNT];           /* ���м����Ϣ�أ����ǻ�����Ϣ�� */
	double	distance[N_CITY_COUNT][N_CITY_COUNT];           /* ���м���� */
};


GInfo Map;

/* ���������� */
class ant
{
private:
	int ChooseNextCity();                   /*ѡ����һ������ */


	double	prob[N_CITY_COUNT];             /* ��ʱ�������飬ѡ����һ�����е�ʱ�򣬱���������б�ѡ�еĸ���ֵ */
	int	m_nCityCount;                   /* ��¼�����Ѿ��߹��ĳ�����Ŀ */
	int	AllowedCity[N_CITY_COUNT];      /* û���߹��ĳ��У�����������Ӧ���б�� */
public:
	double	m_dLength;
	double	m_dShortest;
	int	tabu[N_CITY_COUNT];             /* ��¼�߹��ĳ��У��������ǳ��еı�ţ�����������ʾ�ߵ�˳�� */
public:
	ant();
	void addcity(int city);


	void Clear();


	void UpdateResult();


	void move();


	void move2last();
};

/* ֻʣ��һ������û�߹�ʱ�ŵ��ã�ֱ���ƶ������һ������ */
void ant::move2last()
{
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		if (AllowedCity[i] == 1)
		{
			addcity(i);
			break;
		}
	}
}


/* ������ݣ�����������������к�Ҫ���¿�ʼ���θ�������ʱ���� */
void ant::Clear()
{
	m_dLength = 0;
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		prob[i] = 0;
		AllowedCity[i] = 1;
	}

	int i = tabu[N_CITY_COUNT - 1]; /* �����һ��������Ϊ�������� */
	m_nCityCount = 0;
	addcity(i);
}


/* ��ʼ�� */
ant::ant()
{
	m_dLength = m_dShortest = 0;
	m_nCityCount = 0;

	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		AllowedCity[i] = 1;
		prob[i] = 0;
	}
}


/* ����һ�����е��߹��ĳ��������У����ı�û�߹��ĳ��������еı�־ */
void ant::addcity(int city)
{
	/* add city to tabu; */
	tabu[m_nCityCount] = city;
	m_nCityCount++;
	AllowedCity[city] = 0;
}


int ant::ChooseNextCity()
{
	/*
	* Update the probability of path selection
	* select a path from tabu[m_nCityCount-1] to next
	*/

	int	j = 10000;
	double	temp = 0.0;

	int curCity = tabu[m_nCityCount - 1]; /* ��ǰ�ߵ��Ǹ������� */

										  /* �ȼ��㵱ǰ���к�û���߹��ĳ��У�����֮�����Ϣ�ص��ܺ� */
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		if (AllowedCity[i] == 1)
		{
			temp = temp + pow((1.0 / Map.distance[curCity][i]), DB_BETA) * pow((Map.m_dTrial[curCity][i]), DB_ALPHA);
		}
	}

	/* ����û���߹��ĳ��б�ѡ�еĸ��� */
	double sel = 0;
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		if (AllowedCity[i] == 1)
		{
			prob[i] = pow((1.0 / Map.distance[curCity][i]), DB_BETA) * pow((Map.m_dTrial[curCity][i]), DB_ALPHA) / temp;
			sel += prob[i];
		}
		else {
			prob[i] = 0;
		}
	}


	/*����Ĳ���ʵ���Ͼ����Ŵ��㷨�е�����ѡ�� */
	double	mRate = rnd(0, sel);
	double	mSelect = 0;
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		if (AllowedCity[i] == 1)
		{
			mSelect += prob[i];
		}

		if (mSelect >= mRate)
		{
			j = i;
			break;
		}
	}


	/* �������ֻ����temp=0.0��ʱ��Ż���� */
	if (j == 10000)
	{
		for (int i = 0; i < N_CITY_COUNT; i++)
		{
			if (AllowedCity[i] == 1)
			{
				j = i;
				break;
			}
		}
	}

	return(j);
}


/* ������������к��߹���·������ */
void ant::UpdateResult()
{
	/* Update the length of tour */
	for (int i = 0; i < N_CITY_COUNT - 1; i++)
	{
		m_dLength += Map.distance[tabu[i]][tabu[i + 1]];
	}

	m_dLength += Map.distance[tabu[N_CITY_COUNT - 1]][tabu[0]]; /* �����кͿ�ʼ���м�ľ��� */
}


/* �ƶ�����һ������ */
void ant::move()
{
	/* the ant move to next town and add town ID to tabu. */
	int n = ChooseNextCity();
	addcity(n);
}


class project
{
public:

	double	m_dLength;
	ant	ants[N_ANT_COUNT];


public:
	project();

	void UpdateTrial();


	void initmap();


	void GetAnt();


	void StartSearch();
};


/*
* ���»�����Ϣ��
* ������õ��� ANT-CYCLE ģʽ����ÿֻ������������к�Ÿ���
* ����������ʽΪ����ÿ��һ�����о͸���һ�Σ���������������������Ÿ��±ȽϺ�
*/
void project::UpdateTrial()
{
	/* calculate the changes of trial information */
	int	m = 0;
	int	n = 0;

	for (int i = 0; i < N_ANT_COUNT; i++)                 /* ����ÿֻ�������������м����µ���Ϣ�أ������߹���·��Խ�̣����µ���Ϣ����ֵԽ�� */
	{
		for (int j = 0; j < N_CITY_COUNT - 1; j++)    /* �����������м����Ϣ�� */
		{
			m = ants[i].tabu[j];
			n = ants[i].tabu[j + 1];

			Map.m_dDeltTrial[m][n] += DB_Q / ants[i].m_dLength;
			Map.m_dDeltTrial[n][m] += DB_Q / ants[i].m_dLength;
		}

		/* �����е���ʼ���м����Ϣ�� */
		m = ants[i].tabu[N_CITY_COUNT - 1];
		n = ants[i].tabu[0];

		Map.m_dDeltTrial[m][n] += DB_Q / ants[i].m_dLength;
		Map.m_dDeltTrial[n][m] += DB_Q / ants[i].m_dLength;
	}


	/* ���µĻ�����Ϣ�� = ��ʧ������Ϣ�� +  �����µ���Ϣ�� */
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		for (int j = 0; j < N_CITY_COUNT; j++)
		{
			Map.m_dTrial[i][j] = (DB_ROU * Map.m_dTrial[i][j] + Map.m_dDeltTrial[i][j]);
			Map.m_dDeltTrial[i][j] = 0;
		}
	}
}


/* ��ʼ�� */
void project::initmap()
{
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		for (int j = 0; j < N_CITY_COUNT; j++)
		{
			Map.m_dTrial[i][j] = 1;
			Map.m_dDeltTrial[i][j] = 0;
		}
	}
}


project::project()
{
	/* initial map */
	initmap();
	m_dLength = 10e9;

	struct city
	{
		int	num;
		double	x;
		double	y;
	} cc[N_CITY_COUNT];

	/* ����-���������꣬�������԰ٶȵ�ͼ */
	double x_Ary[92] =
	{
		116.403438, 116.417003, 116.274870, 116.016033, 116.227731, 116.392931, 116.400494, 116.698904, 116.078663, 116.335107,
		116.372616, 116.196307, 116.406274, 116.406274, 116.376972, 116.334877, 117.262423, 117.152828, 117.319770, 117.256029,
		117.173805, 116.631465, 116.682471, 116.694698, 117.284599, 116.834458, 116.817818, 116.912755, 116.015528, 116.044506,
		116.256924, 116.988837, 116.419671, 116.407951, 116.447234, 116.433796, 116.400582, 116.417003, 116.395480, 116.343376,
		116.347634, 116.403092, 116.348822, 116.388222, 116.501591, 116.404928, 116.398488, 116.532576, 116.409248, 116.488036,
		116.554850, 116.304980, 116.214833, 116.200407, 116.313710, 116.324710, 116.326379, 116.103331, 116.293700, 116.158462,
		116.127118, 116.358986, 116.232484, 116.192635, 116.215484, 116.037320, 116.093809, 115.781250, 115.952929, 115.940343,
		115.606468, 115.706057, 117.466929, 117.017184, 117.017184, 117.448210, 117.410073, 117.519154, 117.188130, 117.172324,
		117.101021, 117.193571, 117.668900, 117.204471, 117.221004, 117.716281, 117.547861, 117.481279, 117.200692, 117.831600,
		117.199567, 117.286958
	};

	double y_Ary[92] =
	{
		39.924455, 39.886474, 39.998475, 40.364233, 40.255906, 39.941975, 39.999603, 40.187838, 40.299791, 40.320998,
		40.191595, 40.236241, 40.187238, 40.187238, 40.110177, 39.502581, 40.222785, 40.086212, 40.189006, 40.266759,
		40.249042, 40.381134, 40.401514, 40.459545, 40.66035,  40.559149, 40.550312, 40.385444, 40.554295, 40.344582,
		40.666794, 40.270992, 39.953084, 39.906181, 39.884502, 39.907267, 39.916702, 39.886474, 39.932909, 39.947735,
		39.950257, 39.932079, 39.912174, 39.880237, 39.874106, 40.012344, 39.989306, 39.916648, 39.982398, 39.950229,
		40.02567,  40.018229, 40.007659, 40.001879, 39.925473, 39.948412, 39.922649, 40.11507,	39.816754, 39.800961,
		39.869091, 39.841958, 39.857906, 39.960973, 39.917498, 39.911474, 39.875627, 39.615459, 39.801902, 39.69431,
		39.644162, 39.847687, 39.02545,	 39.1409,   39.162205, 40.246643, 40.050992, 39.292482, 39.098637, 39.098374,
		39.14468,  39.160001, 39.01517,	 39.140409, 39.175574, 38.98334,  40.192761, 39.192409, 39.110526, 39.014349,
		39.147345, 40.100119
	};

	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		cc[i].x = x_Ary[i];
		cc[i].y = y_Ary[i];
		cc[i].num = i;
	}


	/*
	* �����������м����,һγ�ȼ��111km��һ���ȼ��111*cos��km ���-����������ȡ40��
	* ���·��ԼΪ
	*/
	for (int i = 0; i < N_CITY_COUNT; i++)
	{
		for (int j = 0; j < N_CITY_COUNT; j++)
		{
			Map.distance[i][j] = (double)(sqrt(pow((cc[i].x - cc[j].x) * 111 * 0.766, 2) + pow((cc[i].y - cc[j].y) * 111, 2)) + 0.5);
		}
	}
}


void project::GetAnt()
{
	/* ��ʼ��������� */
	srand((unsigned)time(NULL));

	/* Ϊÿֻ�����������һ���������� */
	int city = 0;
	for (int i = 0; i < N_ANT_COUNT; i++)
	{
		city = rnd(N_CITY_COUNT);
		ants[i].addcity(city);
	}
}


void project::StartSearch()
{
	/* begin to find best solution */
	int	max = 0; /* every ant tours times */
	double	temp;
	int	temptour[N_CITY_COUNT];

	double dbMin = 0.0;

	while (max < N_IT_COUNT)
	{
		dbMin = 100000000.0; /* ���ε����е���С·������ */

		for (int j = 0; j < N_ANT_COUNT; j++)
		{
			for (int i = 0; i < N_CITY_COUNT - 1; i++)
			{
				ants[j].move();
			}
		}

		for (int j = 0; j < N_ANT_COUNT; j++)
		{
			ants[j].move2last();
			ants[j].UpdateResult(); /* ����·���ܳ��� */
		}

		/* find out the best solution of the step and put it into temp */
		temp = ants[0].m_dLength;
		for (int t = 0; t < N_CITY_COUNT; t++)
		{
			temptour[t] = ants[0].tabu[t];
		}

		for (int j = 0; j < N_ANT_COUNT; j++)
		{
			if (temp > ants[j].m_dLength)
			{
				temp = ants[j].m_dLength;
				for (int t = 0; t < N_CITY_COUNT; t++)
				{
					temptour[t] = ants[j].tabu[t];
				}
			}

			if (dbMin > ants[j].m_dLength)
			{
				dbMin = ants[j].m_dLength;
			}
		}

		if (temp < m_dLength)
		{
			m_dLength = temp;
			for (int t = 0; t < N_CITY_COUNT; t++)
			{
				besttour[t] = temptour[t];
			}
		}

		printf("%d : %.0f\n", max, m_dLength);

		UpdateTrial();                          /* ȫ�����ϱ����������к󣬸��»�����Ϣ�� */

		for (int j = 0; j < N_ANT_COUNT; j++) /* ������һ�� */
		{
			ants[j].Clear();
		}

		max++;
	}

	printf("The shortest toure is : %f\n", m_dLength);

	for (int t = 0; t < N_CITY_COUNT; t++)
	{
		printf(" %d ", besttour[t] + 1); /*�������Ϊ����ж�Ӧ����*/
	}
}


/* ��������� */
int main()
{
	project TSP;
	TSP.GetAnt();
	TSP.StartSearch();

	getchar();
	system("pause");
	return(0);
}



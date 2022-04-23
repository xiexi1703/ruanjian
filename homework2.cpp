#define _CRT_SECURE_NO_WARNINGS
#define OK 1
#define TRUE 1
#define FALSE 0
#define ERROR 0
typedef int Status;
typedef struct Event/*�¼���*/
{
	int iden;
	int Occurtime;//����ʱ��
	int NType;//���� 0��ʾ���룬1~4��ʾ�뿪
	struct Event* next;
}Event, * EventList;
typedef struct Elem/*����*/
{
	int identity;//0~10  0��ʾvip
	int ArrivalTime;//����ʱ��
	int handleTime;//����ʱ��
	struct Elem* next;
}Elem, * Eptr;
typedef struct//����ָ��
{
	Eptr rear;//��β
	Eptr front;//����
}Customer,*Cupp;
typedef struct
{
	int TatolT;
	int Cnum;
}El;

	Status Open(EventList & ev, Event en, Cupp & q);//����
	Status CArrived(EventList & ev, Cupp & q, Event en);//�˿͵���
	Status CLeave(EventList & ev, Cupp & q, Event en);//�˿��뿪
	Status Close();//����
	Status OrderInser(EventList & ev, Event en);//�¼����뵽˳�����(��ʱ��˳�򣩣��˿͵�����У�
	Status QLength(Customer qn);//���ڳ��Ⱥ���
	Status MinWicket(Cupp q);//����̴���
	Status DelFirstEvent(EventList & ev);//ɾ���¼���ĵ�һ���¼�
	Status InitQueue(Customer & qn);//��ʼ�����ڶ���
	Status EnQueue(Customer & qn, Eptr Q);//�¼������Ӧ����
	Status DeQueue(Customer & qn, Elem & Q);//�¼��뿪����
	Status GetQHead(Customer & qn, Elem & Q);//��ö��е����¼�
	Status DestoryQueue(Customer qn);//���ٶ���
	void print(Customer QCu[]);
	void test(char str[]);//��ӡ��Ӧ���
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<iostream>
	using namespace std;
	int TotalTime = 0, Customernum = 0;//�˿Ͷ����ۼ�ʱ�䣬�˿�����
	int CloseTime;//����ʱ�䣨����ʱ�䣩
	El ck[5] = { 0,0 };
	
	int main()
	{
		EventList ev;
		Event en = { 0,0,NULL };
		Cupp Qcu = NULL;
		Open(ev, en, Qcu);
		while (ev->next)
		{
			en.iden = ev->next->iden;
			en.NType = ev->next->NType;
			en.Occurtime = ev->next->Occurtime;
			DelFirstEvent(ev);
			if (en.NType == 0)
				CArrived(ev, Qcu, en);
			else
				CLeave(ev, Qcu, en);
			print(Qcu);
		}
		Close();
		return OK;
	}
	Status Open(EventList& ev, Event en, Cupp& q)
	{
		int temp = 0;
		cout << "������������ӣ�����0���ʾ��ȡϵͳʱ����ȷ���������)";
		cin >> temp;
		if (temp == 0)
			srand((unsigned)time(NULL));
		else
			srand(temp);

		cout << "���뵱��Ӫҵ��ʱ��(��λ�����ӣ���";
		cin >> temp;
		CloseTime = temp;
		
		TotalTime = 0;
		Customernum = 0;
		en.Occurtime = 0;
		en.NType = 0;
		en.iden = 0;
		en.next= NULL;
		ev = (EventList)malloc(sizeof(Event));
		ev->next = NULL;
		OrderInser(ev, en);

		q = (Customer*)malloc(5 * sizeof(Customer));
		for (int i = 1; i <=4; i++)
		{
			InitQueue(q[i]);
		}

		return OK;
	}

	Status CArrived(EventList& ev, Cupp& q, Event en)
	{
		cout << ">>>�˿͵�������>>>";
		Customernum++;
		int durtime = rand() % 30 + 1;//�¼�����ʱ��
		int intertime = rand() % 5+ 1;//�˿͵�����ʱ��
		int id = rand() % 10;
		/*���������0~30*/
		Event temp;
		/*���뵽�¼���*/
		int t = en.Occurtime + intertime;
		temp.Occurtime = t;
		temp.iden = id;
		temp.NType = 0;
		temp.next = NULL;
		if (t < CloseTime)
			OrderInser(ev, temp);
		cout << "����ʱ��;"<<t<<endl;

		Eptr Q;
		Q = (Eptr)malloc(sizeof(Elem));
		Q->identity = en.iden;
		Q->ArrivalTime = en.Occurtime;
		Q->handleTime = durtime;
		Q->next = NULL;

		int i = MinWicket(q);
		if (Q->identity == 0)//vip
		{
			EnQueue(q[4], Q);			
			i = 4;
			ck[i].Cnum++;
		}
		else
		{
			if (QLength(q[4]) == 0)//VIP���ڴ�����ͨ�ͻ�
			{
				EnQueue(q[4], Q);
				i = 4;
				ck[i].Cnum++;
			}
			else 
			{				
				EnQueue(q[i], Q);
				ck[i].Cnum++;
			}
		}

		temp.Occurtime = en.Occurtime + durtime;
		temp.NType = i;
		temp.iden = en.iden;
		if (QLength(q[i]) == 1)
			OrderInser(ev, temp);
		return OK;
	}

	Status CLeave(EventList& ev, Cupp& q, Event en)
	{
		cout << ">>>�˿��뿪>>>";
		int i = en.NType;
		cout << "�뿪ʱ��:" << en.Occurtime<<endl;
		if (en.Occurtime > CloseTime)
			DestoryQueue(q[i]);
		else
		{
			Elem customer;
			DeQueue(q[i], customer);
			TotalTime += en.Occurtime - customer.ArrivalTime;
			ck[i].TatolT+= en.Occurtime - customer.ArrivalTime;
			cout << "��ʱ��Ϊ" << TotalTime<<endl;
			if (q[i].front->next)
			{
				GetQHead(q[i], customer);
				Event temp;
				temp.Occurtime = en.Occurtime + customer.handleTime;
				temp.NType = i;
				OrderInser(ev, temp);
			}
		
		}
		return OK;
	}

	Status Close()
	{
		cout << "********************************"<< endl;
		cout << endl;
		printf("*  ���й˿�ҵ�������ʱ��:%d����\n", TotalTime);
		printf("*  ҵ�����˿���:%d\n", Customernum);
		printf("*  ƽ��ÿ�˰���ʱ��:%.4f\n", (float)TotalTime / (float)Customernum);
		printf("*   1�Ŵ��ڰ�����ʱ�䣺%d ������˿�����%d\n", ck[1].TatolT, ck[1].Cnum);
		printf("*   2�Ŵ��ڰ�����ʱ�䣺%d ������˿�����%d\n", ck[2].TatolT, ck[2].Cnum);
		printf("*   3�Ŵ��ڰ�����ʱ�䣺%d ������˿�����%d\n", ck[3].TatolT, ck[3].Cnum);
		printf("* vip�Ŵ��ڰ�����ʱ�䣺%d ������˿�����%d\n", ck[4].TatolT, ck[4].Cnum);
		printf("*   1�Ŵ���ƽ��ÿ�˰���ʱ�䣺%.4f\n", (float)ck[1].TatolT/ (float)ck[1].Cnum);
		printf("*   2�Ŵ���ƽ��ÿ�˰���ʱ�䣺%.4f\n", (float)ck[2].TatolT / (float)ck[2].Cnum);
		printf("*   3�Ŵ���ƽ��ÿ�˰���ʱ�䣺%.4f\n", (float)ck[3].TatolT / (float)ck[3].Cnum);
		printf("* vip�Ŵ���ƽ��ÿ�˰���ʱ�䣺%.4f\n", (float)ck[4].TatolT / (float)ck[4].Cnum);
		printf("*\n");
		cout << "********************************" << endl;
		return OK;
	}

Status OrderInser(EventList& ev, Event en)//��ͷ��㣩�¼����һ��λ��Ϊ��ǲ�������
{
	EventList ent, qt;
	ent = (EventList)malloc(sizeof(Event));
	ent->Occurtime = en.Occurtime;
	ent->NType = en.NType;
	ent->iden = en.iden;
	ent->next = NULL;
	if (!ev->next)
	{
		ev->next = ent;
		return OK;
	}
	qt = ev;
	while (qt->next && qt->next->Occurtime < en.Occurtime)
	{
		qt = qt->next;
	}
	ent->next = qt->next;
	qt->next = ent;
	return OK;
}

Status QLength(Customer qn)
{
	Eptr qt;
	int i = 0;
	qt = qn.front->next;
	while (qt)
	{
		qt = qt->next;
		i++;
	}
	return i;
}

Status MinWicket(Cupp q)
{
	int i;
	int min;
	for (i = 1, min = 1; i <=3; i++)
	{
		min = QLength(q[min]) <= QLength(q[i]) ? min : i;
	}
	return min;
}

Status DelFirstEvent(EventList& ev)
{
	EventList p;
	p = ev->next;
	ev->next = p->next;
	free(p);
	return OK;
}

Status InitQueue(Customer& qn)
{
	qn.front = (Eptr)malloc(sizeof(Elem));
	qn.front->next = NULL;
	qn.rear = qn.front;
	return OK;
}

Status EnQueue(Customer& qn, Eptr Q)
{
	qn.rear->next = Q;
	qn.rear = Q;
	return OK;
}

Status DeQueue(Customer& qn, Elem& Q)
{
	Eptr qt;
	qt = qn.front->next;
	Q.identity = qt->identity;
	Q.ArrivalTime = qt->ArrivalTime;
	Q.handleTime = qt->handleTime;
	qn.front->next = qt->next;
	if (qn.rear ==qt)
		qn.rear = qn.front;
	free(qt);
	return OK;
}

Status GetQHead(Customer& qn, Elem& Q)
{
	Q.ArrivalTime = qn.front->next->ArrivalTime;
	Q.handleTime = qn.front->next->handleTime;
	Q.identity = qn.front->next->identity;
	return OK;
		
}

Status DestoryQueue(Customer qn)
{
	Eptr p;
	while (qn.front->next)
	{
		p = qn.front->next;
		qn.front->next = p->next;
		free(p);
	}
	qn.front->next = NULL;
	qn.rear = qn.front;
	return OK;
}

void print(Customer QCu[])
{
	int L;
	for (int i = 1; i <= 4; i++)
	{
		L = QLength(QCu[i]);
		if (i == 4)
			printf("vip����: ����%d:", L);
		else
			printf("%d�Ŵ���: ����%d:", i, L);
		for (int n = 1; n <= L; n++)
		{
			printf("$");
		}
		printf("\n");
	}
}

void test(char str[])
{
	printf("����%s����", str);
}
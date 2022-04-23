#define _CRT_SECURE_NO_WARNINGS
#define OK 1
#define TRUE 1
#define FALSE 0
#define ERROR 0
typedef int Status;
typedef struct Event/*事件表*/
{
	int iden;
	int Occurtime;//发生时间
	int NType;//类型 0表示进入，1~4表示离开
	struct Event* next;
}Event, * EventList;
typedef struct Elem/*窗口*/
{
	int identity;//0~10  0表示vip
	int ArrivalTime;//到达时间
	int handleTime;//办理时间
	struct Elem* next;
}Elem, * Eptr;
typedef struct//窗口指针
{
	Eptr rear;//队尾
	Eptr front;//队首
}Customer,*Cupp;
typedef struct
{
	int TatolT;
	int Cnum;
}El;

	Status Open(EventList & ev, Event en, Cupp & q);//开门
	Status CArrived(EventList & ev, Cupp & q, Event en);//顾客到达
	Status CLeave(EventList & ev, Cupp & q, Event en);//顾客离开
	Status Close();//关门
	Status OrderInser(EventList & ev, Event en);//事件插入到顺序表中(按时间顺序）（顾客到达队列）
	Status QLength(Customer qn);//窗口长度函数
	Status MinWicket(Cupp q);//求最短窗口
	Status DelFirstEvent(EventList & ev);//删除事件表的第一个事件
	Status InitQueue(Customer & qn);//初始化窗口队列
	Status EnQueue(Customer & qn, Eptr Q);//事件进入对应窗口
	Status DeQueue(Customer & qn, Elem & Q);//事件离开窗口
	Status GetQHead(Customer & qn, Elem & Q);//获得队列的首事件
	Status DestoryQueue(Customer qn);//销毁队列
	void print(Customer QCu[]);
	void test(char str[]);//打印对应语句
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<iostream>
	using namespace std;
	int TotalTime = 0, Customernum = 0;//顾客逗留累计时间，顾客数量
	int CloseTime;//关门时间（工作时间）
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
		cout << "输入随机数种子（输入0则表示采取系统时间来确定随机种子)";
		cin >> temp;
		if (temp == 0)
			srand((unsigned)time(NULL));
		else
			srand(temp);

		cout << "输入当天营业总时长(单位：分钟）：";
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
		cout << ">>>顾客到达银行>>>";
		Customernum++;
		int durtime = rand() % 30 + 1;//事件处理时间
		int intertime = rand() % 5+ 1;//顾客到达间断时间
		int id = rand() % 10;
		/*产生随机数0~30*/
		Event temp;
		/*插入到事件表*/
		int t = en.Occurtime + intertime;
		temp.Occurtime = t;
		temp.iden = id;
		temp.NType = 0;
		temp.next = NULL;
		if (t < CloseTime)
			OrderInser(ev, temp);
		cout << "到达时间;"<<t<<endl;

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
			if (QLength(q[4]) == 0)//VIP窗口处理普通客户
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
		cout << ">>>顾客离开>>>";
		int i = en.NType;
		cout << "离开时间:" << en.Occurtime<<endl;
		if (en.Occurtime > CloseTime)
			DestoryQueue(q[i]);
		else
		{
			Elem customer;
			DeQueue(q[i], customer);
			TotalTime += en.Occurtime - customer.ArrivalTime;
			ck[i].TatolT+= en.Occurtime - customer.ArrivalTime;
			cout << "总时间为" << TotalTime<<endl;
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
		printf("*  所有顾客业务办理总时间:%d分钟\n", TotalTime);
		printf("*  业务办理顾客数:%d\n", Customernum);
		printf("*  平均每人办理时间:%.4f\n", (float)TotalTime / (float)Customernum);
		printf("*   1号窗口办理总时间：%d ；办理顾客数：%d\n", ck[1].TatolT, ck[1].Cnum);
		printf("*   2号窗口办理总时间：%d ；办理顾客数：%d\n", ck[2].TatolT, ck[2].Cnum);
		printf("*   3号窗口办理总时间：%d ；办理顾客数：%d\n", ck[3].TatolT, ck[3].Cnum);
		printf("* vip号窗口办理总时间：%d ；办理顾客数：%d\n", ck[4].TatolT, ck[4].Cnum);
		printf("*   1号窗口平均每人办理时间：%.4f\n", (float)ck[1].TatolT/ (float)ck[1].Cnum);
		printf("*   2号窗口平均每人办理时间：%.4f\n", (float)ck[2].TatolT / (float)ck[2].Cnum);
		printf("*   3号窗口平均每人办理时间：%.4f\n", (float)ck[3].TatolT / (float)ck[3].Cnum);
		printf("* vip号窗口平均每人办理时间：%.4f\n", (float)ck[4].TatolT / (float)ck[4].Cnum);
		printf("*\n");
		cout << "********************************" << endl;
		return OK;
	}

Status OrderInser(EventList& ev, Event en)//（头结点）事件表第一个位置为标记不存数据
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
			printf("vip窗口: 长度%d:", L);
		else
			printf("%d号窗口: 长度%d:", i, L);
		for (int n = 1; n <= L; n++)
		{
			printf("$");
		}
		printf("\n");
	}
}

void test(char str[])
{
	printf("——%s——", str);
}
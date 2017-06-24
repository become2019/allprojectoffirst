#include <stdio.h>
#include <stdlib.h>
typedef struct Listnode
{
	float coef;
	float expn;
	struct Listnode *next;
}ListNode;
typedef ListNode * LinkList;

int CreatPolyn(LinkList *p);
void InitList(LinkList *p);
int LocateElem(LinkList p,ListNode e);
void Insert(LinkList *h,ListNode e); 
void display(LinkList h);
int getLength(LinkList *h);
int Comp(LinkList *h,int i,int j);
void Change(LinkList *h,int i,int k);
void OrderList(LinkList *h);
void add(LinkList *h1,LinkList *h2);
void checkZero(LinkList *h);
void mul(LinkList *h1,LinkList *h2);

FILE *fp;

int main()
{
	int i;
	ListNode *P1,*P2;
	if( (fp=fopen("C:\\Users\\DSK\\Desktop\\学习工作\\工程实践2结题\\Polyn.txt","w")) == NULL ){
        puts("文件打开错误");
        return -1;
    }
	P1=P2=NULL;
	puts("********************************************************************************");
	puts("			  欢迎使用一元多项式计算器!\n");
	puts("********************************************************************************");
	puts("请选择将要使用的功能:\n");
	puts("  1.一元多项式相加\n");
	puts("  2.一元多项式相乘\n");
	puts("  3.退出系统\n");
	puts("********************************************************************************");
	printf("请输入数字以选择对应的功能:");
	scanf("%d",&i);
	while(i<0||i>3)
	{
		printf("输入错误，请重新输入:");
		scanf("%d",&i);
	}
	switch(i)
	{
	case 1:
		system("cls");
		puts("********************************************************************************");
		puts("			  欢迎使用一元多项式加法功能!\n");
		puts("********************************************************************************");
		if(CreatPolyn(&P1))
			return 0;
		printf("\n");
		if(CreatPolyn(&P2))
			return 0;
		puts("\n第一个多项式为:");
		fputs("\n第一个多项式为:",fp);
		display(P1);
		puts("\n第二个多项式为:");
		fputs("\n第二个多项式为:",fp);
		display(P2);
		add(&P1,&P2);
		puts("\n两者之和为:");
		fputs("\n两者之和为:",fp);
		display(P1);
		break;
	case 2:
		system("cls");
		puts("********************************************************************************");
		puts("			  欢迎使用一元多项式乘法功能!\n");
		puts("********************************************************************************");
		if(CreatPolyn(&P1))
			return 0;
		printf("\n");
		if(CreatPolyn(&P2))
			return 0;
		puts("\n第一个多项式为:");
		fputs("\n第一个多项式为:",fp);
		display(P1);
		puts("\n第二个多项式为:");
		fputs("\n第二个多项式为:",fp);
		display(P2);
		mul(&P1,&P2);
		puts("\n两者之积为:");
		fputs("\n两者之积为:",fp);
		display(P1);
		break;
	default:break;
	}
	fclose(fp);
	return 0;
}

int CreatPolyn(LinkList *p)//建立多项式 
{
	int i;
	float f1,f2;
	ListNode e;
	InitList(p);
	puts("请输入多项式(输入0表示结束):");  //printf("请输入多项式:\n");
	for(i=0;;i++)
	{
		scanf("%f",&f1);
		if(f1==0)
			break;
		if(i%2==1)  //表示已经输入了两个数
		{
			e.coef=f2;
			e.expn=f1;
			if(0==LocateElem(*p,e))
				Insert(p,e);
		}
		f2=f1;
	}
	if(i%2!=0)
	{
		puts("输入错误!数据个数为单数!");
		return 1;
	}
	OrderList(p);
	return 0;
}

void InitList(LinkList *p)//初始化链表 
{
	*p=(ListNode *)malloc(sizeof(ListNode));
	(*p)->next=NULL;
	(*p)->coef=0;
	(*p)->expn=-1;
}

int LocateElem(LinkList p,ListNode e)//合并同类项 
{
	int i=0;  //表示未找到指数相同的单项式 
	while(p!=NULL)
	{
		if(p->expn==e.expn)
		{
			i=1;  //表示已找到指数相同的单项式
			p->coef +=e.coef;  //指数相同的系数相加 
			break; 
		}
		p=p->next;
	}
	return i;
}

void Insert(LinkList *h,ListNode e)  //在链表中插入新节点 
{
	LinkList p=NULL;
	p=(ListNode *)malloc(sizeof(ListNode));
	p->coef=e.coef;
	p->expn=e.expn;
	p->next=(*h)->next;
	(*h)->next=p;
}

void display(LinkList h)//输出多项式 
{
	int flag=0;
	h=h->next;
	while(h!=NULL)
	{
		if(flag!=0)
			if(h->coef>0)
			{
				if(h->coef==1)
					if(h->expn==1){
						printf("+x");
						fprintf(fp,"+x");
					}
					else{
						printf("+x^(%.0f)",h->expn);
						fprintf(fp,"+x^(%.0f)",h->expn);
					}
				else{
					printf("+%.2fx^(%.0f)",h->coef,h->expn);
					fprintf(fp,"+%.2fx^(%.0f)",h->coef,h->expn);
				}
			}
			else
			{
				if(h->coef==-1)
					if(h->expn==1){
						printf("-x");
						fprintf(fp,"-x");
					}
					else{
						printf("-x^(%.0f)",h->expn);
						fprintf(fp,"-x^(%.0f)",h->expn);
					}
				else{
					printf("%.2fx^(%.0f)",h->coef,h->expn);
					fprintf(fp,"%.2fx^(%.0f)",h->coef,h->expn);
				}
			}
		else
		{
			if(h->coef>0)
			{
				if(h->coef==1)
					if(h->expn==1){
						printf("x");
						fprintf(fp,"x");
					}
					else{
						printf("x^(%.0f)",h->expn);
						fprintf(fp,"x^(%.0f)",h->expn);
					}
				else{
					printf("%.2fx^(%.0f)",h->coef,h->expn);
					fprintf(fp,"%.2fx^(%.0f)",h->coef,h->expn);
				}
			}
			else
			{
				if(h->coef==-1)
					if(h->expn==1){
						printf("-x");
						fprintf(fp,"-x");
					}
					else{
						printf("-x^(%.0f)",h->expn);
						fprintf(fp,"-x^(%.0f)",h->expn);
					}
				else{
					printf("%.2fx^(%.0f) ",h->coef,h->expn);
					fprintf(fp,"%.2fx^(%.0f) ",h->coef,h->expn);
				}
			}
		}
		flag=1;
		h=h->next;
	}
	putchar('\n');
	fprintf(fp,"\n");
}

void OrderList(LinkList *h)//多项式中的每一项按指数升序排列
{
	int i,j,k,n;
	n=getLength(h);
	for(i=0;i<n-1;i++)
	{
		k=i;
		for(j=i+1;j<n;j++)
			if(Comp(h,k,j)==1)
				k=j;
		if(k!=i)
			Change(h,i,k);
	}

}

void checkZero(LinkList *h)//去除指数为0的项 
{
	LinkList p,p1;
	p1=*h;
	p=p1->next;
	while(p!=NULL)
	{
		if(p->coef==0)
			p1->next=p->next;
		else
			p1=p;
		p=p->next;
	}
}

int getLength(LinkList *h)//得多项式长度 
{
	int i=0;
	LinkList p;
	p=*h;
	p=p->next;
	while(p!=NULL)
	{
		i++;
		p=p->next;
	}
	return i;
}

int Comp(LinkList *h,int i,int j)//比较两单项式指数的大小 
{
	float a,b;
	int n;
	LinkList p;
	p=*h;
	p=p->next;
	for(n=0;n<i;n++)
		p=p->next;
	a=p->expn;
	p=*h;
	p=p->next;
	for(n=0;n<j;n++)
		p=p->next;
	b=p->expn;
	if(a>b)
		return 1;
	else
		return 0;
}

void Change(LinkList *h,int i,int k)//交换两个单项式的系数和指数
{
	int n;
	float e;
	float f;
	LinkList p,q;
	p=*h;
	p=p->next;
	for(n=0;n<i;n++)
		p=p->next;
	q=*h;
	q=q->next;
	for(n=0;n<k;n++)
		q=q->next;
	f=p->coef;
	p->coef=q->coef;
	q->coef=f;
	e=p->expn;
	p->expn=q->expn;
	q->expn=e;
}

void getElem(LinkList *h,int i,ListNode *e)//得到链表中第i个元素
{
	int n;
	LinkList p;
	p=*h;
	p=p->next;
	for(n=0;n<i;n++)
		p=p->next;
	e->coef=p->coef;
	e->expn=p->expn;
}

void add(LinkList *h1,LinkList *h2)//加法运算 
{
	int i,l;
	ListNode e;
	l=getLength(h2);
	for(i=0;i<l;i++)
	{
		getElem(h2,i,&e);
		if(0==LocateElem(*h1,e))
			Insert(h1,e);
	}
	OrderList(h1);
	checkZero(h1);
}

void mul(LinkList *h1,LinkList *h2)//乘法运算 
{
	int i,j,l1,l2;
	ListNode *p;
	ListNode e1,e2,e3;
	InitList(&p);
	l1=getLength(h1);
	l2=getLength(h2);
	for(i=0;i<l1;i++)
	{
		getElem(h1,i,&e1);
		for(j=0;j<l2;j++)
		{
			getElem(h2,j,&e2);
			e3.coef=e1.coef*e2.coef;
			e3.expn=e1.expn+e2.expn;
			if(0==LocateElem(p,e3))
				Insert(&p,e3);
		}
	}
	OrderList(&p);
	checkZero(&p);
	*h1=p;
}

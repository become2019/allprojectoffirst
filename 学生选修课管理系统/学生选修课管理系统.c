#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<windows.h>
#define LEN sizeof(struct Kecheng)

struct Kecheng
{
	int num;
	char name[21];
	char xingzhi[11];
	int timezong;
	int timeke;
	int timeji;
	int fen;
	int term;
	struct Kecheng *next;
};

void zhuyemian(void);
void luru(void);
void xinjian(void);
void write(struct Kecheng *head);
struct Kecheng *read(void);
void xiugai(void);
void shanchu(void);
void liulan(void);
void chaxun(void);
void xuanke(void);
void print(struct Kecheng *head);
struct Kecheng *paixunum(struct Kecheng *head);
struct Kecheng *paixuxingzhi(struct Kecheng *head);

int main()
{
	zhuyemian();
	return 0;
}

void zhuyemian(void)
{
	int i;
	system("cls");
	puts("********************************************************************************");
	puts("			  欢迎使用学生选修管理系统!\n");
	puts("********************************************************************************");
	puts("请选择将要使用的功能:\n");
	puts("  1.课程录入\n");
	puts("  2.课程浏览\n");
	puts("  3.课程查询\n");
	puts("  4.学生选课\n");
	puts("  5.退出系统\n");
	puts("********************************************************************************");
	printf("请输入数字以选择对应的功能:");
	scanf("%d",&i);
	while(i<0||i>5)
	{
		printf("输入错误，请重新输入:");
		scanf("%d",&i);
	}
	switch(i)
	{
	case 1:luru();break;
	case 2:liulan();break;
	case 3:chaxun();break;
	case 4:xuanke();break;
	default:break;
	}
}

void luru(void)
{
	int i;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入课程录入界面!\n");
	puts("********************************************************************************");
	puts("请选择将要使用的功能:\n");
	puts("  1.新建课程\n");
	puts("  2.课程修改\n");
	puts("  3.课程删除\n");
	puts("  4.返回主界面\n");
	puts("********************************************************************************");
	printf("请输入数字以选择对应的功能:");
	scanf("%d",&i);
	while(i<0||i>4)
	{
		printf("输入错误，请重新输入:");
		scanf("%d",&i);
	}
	switch(i)
	{
	case 1:xinjian();break;
	case 2:xiugai();break;
	case 3:shanchu();break;
	default:zhuyemian();
	}
}

void xinjian(void)
{
	char c;
	struct Kecheng *head,*p1,*p2;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入新建界面!\n");
	puts("********************************************************************************");
	puts("请输入要新建的课程信息(数据输入全0时输入结束)");
	puts("课程编号 课程名称 课程性质 总学时 授课学时 实验或上机学时 学分 开课学期");
	head=NULL;
	p1=p2=(struct Kecheng *)malloc(LEN);
	scanf("%d %s %s %d %d %d %d %d",&p2->num,p2->name,p2->xingzhi,&p2->timezong,&p2->timeke,&p2->timeji,&p2->fen,&p2->term);
	while(p2->num!=0)
	{
		if(head==NULL)
			head=p2;
		else
		{
			p1->next=p2;
			p1=p2;
		}
		p2=(struct Kecheng *)malloc(LEN);
		scanf("%d %s %s %d %d %d %d %d",&p2->num,p2->name,p2->xingzhi,&p2->timezong,&p2->timeke,&p2->timeji,&p2->fen,&p2->term);
	}
	p1->next=NULL;
	free(p2);
	write(head);	
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

void write(struct Kecheng *head)  
{
	FILE *fp;
	struct Kecheng *p;
	p=head;
	if((fp=fopen("学生选修课管理系统.txt","w"))==NULL)
		puts("文件创建失败!");
	else
	{
		while(p!=NULL)
		{
			fprintf(fp,"%d %s %s %d %d %d %d %d ",p->num,p->name,p->xingzhi,p->timezong,p->timeke,p->timeji,p->fen,p->term);
			p=p->next;
		}
	}
	fclose(fp);
}

struct Kecheng *read(void)
{
	FILE *fp;
	struct Kecheng *head,*p1,*p2;
	head=NULL;
	if((fp=fopen("学生选修课管理系统.txt","r"))==NULL)
		puts("文件打开失败!");
	else
	{
		p1=p2=(struct Kecheng *)malloc(LEN);
		fscanf(fp,"%d %s %s %d %d %d %d %d",&p2->num,p2->name,p2->xingzhi,&p2->timezong,&p2->timeke,&p2->timeji,&p2->fen,&p2->term);
		while(!feof(fp))
		{
			if(head==NULL)
				head=p2;
			else
			{
				p1->next=p2;
				p1=p2;
			}
			p2=(struct Kecheng *)malloc(LEN);
			fscanf(fp,"%d %s %s %d %d %d %d %d",&p2->num,p2->name,p2->xingzhi,&p2->timezong,&p2->timeke,&p2->timeji,&p2->fen,&p2->term);
		}
		p1->next=NULL;
		free(p2);
	}
	fclose(fp);
	return head;
}

void xiugai(void)
{
	char c,s[21];
	int i,j,flag=0;
	struct Kecheng *head,*p;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入课程修改界面!\n");
	puts("********************************************************************************");
	do{
		puts("请选择输入信息种类查找要修改的课程(1为课程编号 2为课程名称):");
		scanf("%d",&i);
		while((i!=1)&&(i!=2))
		{
			printf("输入错误，请重新输入:");
			scanf("%d",&i);
		}
		head=read();
		p=head;
		switch(i)
		{
		case 1:
			{
				printf("请输入要修改课程的编号:");
				scanf("%d",&j);
				while((p!=NULL)&&(p->num!=j))
				p=p->next;
				if(p==NULL)
				{
					if(flag==1)
						write(head);
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		case 2:
			{
				printf("请输入要修改课程的名称:");
				c=getchar();
				gets(s);
				while((p!=NULL)&&(strcmp(p->name,s)!=0))
					p=p->next;
				if(p==NULL)
				{
					if(flag==1)
						write(head);
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		default:break;
		}
		puts("课程已找到");
		puts("课程编号 课程名称 课程性质 总学时 授课学时 实验或上机学时 学分 开课学期");
		printf("%d %s %s %d %d %d %d %d\n",p->num,p->name,p->xingzhi,p->timezong,p->timeke,p->timeji,p->fen,p->term);
		printf("请输入修改后的内容:\n");
		scanf("%d %s %s %d %d %d %d %d",&p->num,p->name,p->xingzhi,&p->timezong,&p->timeke,&p->timeji,&p->fen,&p->term);
		flag=1;
		printf("是否继续修改(Y or N):");
		c=getchar();
		scanf("%c",&c);
	}while((c=='Y')||(c=='y'));
	write(head);	
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

void shanchu(void)
{
	char c,s[21];
	int i,j,flag=0;
	struct Kecheng *head,*p1,*p2;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入课程删除界面!\n");
	puts("********************************************************************************");
	do{
		puts("请选择输入信息种类查找要删除的课程(1为课程编号 2为课程名称):");
		scanf("%d",&i);
		while((i!=1)&&(i!=2))
		{
			printf("输入错误，请重新输入:");
			scanf("%d",&i);
		}
		head=read();
		p1=p2=head;
		switch(i)
		{
		case 1:
			{
				printf("请输入要删除课程的编号:");
				scanf("%d",&j);
				while((p2!=NULL)&&(p2->num!=j))
				{
					p1=p2;
					p2=p2->next;
				}
				if(p2==NULL)
				{
					if(flag==1)
						write(head);
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		case 2:
			{
				printf("请输入要删除课程的名称:");
				c=getchar();
				gets(s);
				while((p2!=NULL)&&(strcmp(p2->name,s)!=0))
				{
					p1=p2;
					p2=p2->next;
				}
				if(p2==NULL)
				{
					if(flag==1)
						write(head);
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		default:break;
		}
		puts("课程已找到");
		puts("课程编号 课程名称 课程性质 总学时 授课学时 实验或上机学时 学分 开课学期");
		printf("%d %s %s %d %d %d %d %d\n",p2->num,p2->name,p2->xingzhi,p2->timezong,p2->timeke,p2->timeji,p2->fen,p2->term);
		printf("是否确认删除该门课程(Y or N):");
		c=getchar();
		scanf("%c",&c);
		if((c=='Y')||(c=='y'))
			if(p2!=head)
				p1->next=p2->next;
			else
				head=p2->next;
		flag=1;
		printf("是否继续删除(Y or N):");
		c=getchar();
		scanf("%c",&c);
	}while((c=='Y')||(c=='y'));
	write(head);	
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

void liulan(void)
{
	char c;
	int i,flag=0;
	struct Kecheng *head;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入课程浏览界面!\n");
	puts("********************************************************************************");
	puts("请选择输入信息种类以确定课程排序方式(1为课程编号 2为性质):");
	scanf("%d",&i);
	while((i!=1)&&(i!=2))
	{
		printf("输入错误，请重新输入:");
		scanf("%d",&i);
	}
	puts("课程编号 课程名称 课程性质 总学时 授课学时 实验或上机学时 学分 开课学期");
	head=read();
	if(head==NULL)
	{
		printf("没有课程记录!");
		exit(0);
	}
	switch(i)
		{
		case 1:head=paixunum(head);break;
		case 2:head=paixuxingzhi(head);break;
			
		default:break;
		}
	print(head);
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

void chaxun(void)
{
	int i,j;
	char c,s[21];
	struct Kecheng *head,*p;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入课程查询界面!\n");
	puts("********************************************************************************");
	do{
		puts("请选择输入信息种类以查找课程(1为课程编号 2为课程名称):");
		scanf("%d",&i);
		while((i!=1)&&(i!=2))
		{
			printf("输入错误，请重新输入:");
			scanf("%d",&i);
		}
		head=read();
		p=head;
		switch(i)
		{
		case 1:
			{
				printf("请输入要查找课程的编号:");
				scanf("%d",&j);
				while((p!=NULL)&&(p->num!=j))
					p=p->next;
				if(p==NULL)
				{
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		case 2:
			{
				printf("请输入要查找课程的名称:");
				c=getchar();
				gets(s);
				while((p!=NULL)&&(strcmp(p->name,s)!=0))
					p=p->next;
				if(p==NULL)
				{
					printf("没有找到该门课程,是否返回主页面从新操作(Y or N):");
					c=getchar();
					scanf("%c",&c);
					if((c=='Y')||(c=='y'))
						zhuyemian();
					else
						exit(0);
				}
				break;
			}
		default:break;
		}
		puts("课程已找到");
		puts("课程编号 课程名称 课程性质 总学时 授课学时 实验或上机学时 学分 开课学期");
		printf("%d %s %s %d %d %d %d %d\n",p->num,p->name,p->xingzhi,p->timezong,p->timeke,p->timeji,p->fen,p->term);
		printf("是否继续查找(Y or N):");
		c=getchar();
		scanf("%c",&c);
	}while((c=='Y')||(c=='y'));
	write(head);	
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

void print(struct Kecheng *head)
{
	struct Kecheng *p;
	p=head;
	while(p!=NULL)
	{
		printf("%d %s %s %d %d %d %d %d\n",p->num,p->name,p->xingzhi,p->timezong,p->timeke,p->timeji,p->fen,p->term);
		p=p->next;
	}
}

void xuanke(void)
{
	struct Kecheng *head,*p,*p1,*p2,*p3;
	int s=0,i=0,j,flag=0;
	char c;
	p=NULL;
	system("cls");
	puts("********************************************************************************");
	puts("			   欢迎进入学生选课界面!\n");
	puts("********************************************************************************");
	puts("目前可选课程有:");
	head=read();
	head=paixunum(head);
	print(head);
	p3=p2=(struct Kecheng *)malloc(LEN);
	do
	{
		printf("\n请输入想要您要选择课程的编号:");
		scanf("%d",&j);
		p1=head;
		while((p1!=NULL)&&(p1->num!=j))
			p1=p1->next;
		if(p1==NULL)
			printf("没有找到该课程!");
		else
		{
			if(p1->term==1)
			{
				puts("您要选择的课程是:");
				printf("%d %s %s %d %d %d %d %d\n",p1->num,p1->name,p1->xingzhi,p1->timezong,p1->timeke,p1->timeji,p1->fen,p1->term);
				if(i==0)
				{
					*p2=*p1;
					p=p2;
				}
				else
				{
					p3=p2;
					p2=(struct Kecheng *)malloc(LEN);
					*p2=*p1;
					p3->next=p2;	
				}
				i++;
				s=s+p1->fen;
				if(s>80)
				{
					puts("您所选课程已超过80学分,最后一门课程选课失败!");
					s=s-p1->fen;
					i--;
					flag=1;
				}
				printf("现在您已选 %d 门课程,共有 %d 个学分\n",i,s);
			}
			else
				puts("选课失败，您所选的课程非本学期课程");
		}
		printf("是否继续选课(Y or N):");
		c=getchar();
		scanf("%c",&c);
	}while((c=='Y')||(c=='y'));
	if(s>60)
	{
		if(flag==0)
			p2->next=NULL;
		else
			p3->next=NULL;
		puts("\n您所选课程为:");
		print(p);
	}
	else
	{
		puts("您所选课程不足60学分,所选课程不被记录!");
	}
	printf("是否返回主界面(Y or N):");
	c=getchar();
	scanf("%c",&c);
	if((c=='Y')||(c=='y'))
		zhuyemian();
}

struct Kecheng *paixunum(struct Kecheng *head)
{
	int flag=0;
	struct Kecheng *p1,*p2,*p3,*p4;
	if(head==NULL)
		return head;
	else
		p1=p2=head->next;
	while(p2!=NULL)
	{
		p1=p1->next;
		if(p2->num<head->num)
		{
			p2->next=head;
			if(flag==0)
				head->next=NULL;
			head=p2;
			flag++;
		}
		else
		{
			if(flag>0)
			{
				p3=p4=head;
				while((p3!=NULL)&&(p2->num>p3->num))
				{
					p4=p3;
					p3=p3->next;
				}
				p4->next=p2;
				p2->next=p3;
			}
			if(flag==0)
			{
				flag++;
				p2->next=NULL;
			}
		}
		p2=p1;
	}
	return head;
}

struct Kecheng *paixuxingzhi(struct Kecheng *head)
{
	int flag=0;
	struct Kecheng *p1,*p2,*p3,*p4;
	if(head==NULL)
		return head;
	else
		p1=p2=head->next;
	while(p2!=NULL)
	{
		p1=p1->next;
		if(strcmp(p2->xingzhi,head->xingzhi)<0)
		{
			p2->next=head;
			if(flag==0)
				head->next=NULL;
			head=p2;
			flag++;
		}
		else
		{
			if(flag>0)
			{
				p3=p4=head;
				while((p3!=NULL)&&(strcmp(p2->xingzhi,p3->xingzhi)>0))
				{
					p4=p3;
					p3=p3->next;
				}
				p4->next=p2;
				p2->next=p3;
			}
			if(flag==0)
			{
				flag++;
				p2->next=NULL;
			}
		}
		p2=p1;
	}
	return head;
}
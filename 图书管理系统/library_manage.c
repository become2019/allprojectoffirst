/**--------------File Info---------------------------------------------------------------------------------
** File name:              library_manage.c
** Latest modified Date:    2017-6-24(加信息)
** Latest Version:          1.0
** Descriptions:           图书管理系统
**
**--------------------------------------------------------------------------------------------------------
** Created by:             FirendSaber
** Created date:             2016（大一下学期工程实践）
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> 

#define  LEN sizeof(Books_style) 



//会员信息 
typedef struct HUIYUAN
{
	char zhanghao[20];//账号 
	char mima[12]; //密码 
	char jyqingkuang[3];//借阅情况 
	double  date_jine; //逾期金额
	int  jieyueshu;
}HUIY_data;

//图书信息 
typedef struct BOOK
{	
	char num[10];/*书号*/
	char name[20];/*书名*/
	char author[12];/*作者*/		
	char zhanghao1[20];//借阅者
	char zhuangtai[4];//借阅情况
	double yuqimoney;//逾期金额

	int  cishu;

	struct BOOK *next;
}Books_style;

struct shijian
{
    int nian;
	int yue;
	int ri;
};


void guanliyuan();
void Huiyuan();
void zhuce();
void denglu();
void chajian(char str[],int di);
void show_menu();//显示主菜单
void Show_ALL();//显示文本内容，若文件为空即添加（模块1）
Books_style *creat();//创建链表
void input();//为文件为空时，向文件添加内容（添加创建的链表内容）
void output();//从文件读取在屏幕显示文件内容，即查询
void Turn_menu();//页面跳转函数
void Turn_menu1();//显示管理员界面
void Turn_menu2(char str[],int di);//登录、注册界面
void Turn_menu3();//会员界面
void Add_Book();
void Modofy_Book();
void Query_Book();
void Delete_Book();
void fondpaixu_Book();
void lent_book(char str[],int di);
void back_Book(char str[],int di);
void huiyuan_list();
void zhuangt_Book();
void jyqkuang_Book();
void jieyue_Book(char string[],int n);
void jilutime(int n);
void jilutime2();
void jilutime3();
double yuqifond_Book(int n);
void xiugaimima(int n);
void xiugaimima2();
void xianshi();

Books_style *rep;
HUIY_data huiyuan[100];
int r=1;
int shu=0,shu1=0;
struct shijian   first[20],second;


int main()
{

	show_menu();
	
	return 0;
}
//主菜单
void show_menu()
{
	int g,jishu=0;
	
	strcpy(huiyuan[0].zhanghao,"root");
	strcpy(huiyuan[0].mima,"123456");
	system("cls");
	printf("\n\n\t||=======================================||");
	printf("\n\t||     1.以管理员的身份进入              ||");
	printf("\n\t||     2.以会员的身份进入                ||");
	printf("\n\t||     3.退出                            ||");
	printf("\n\t||=======================================||");
	
	printf("\n请输入你的选择（1-3）:");
	scanf("%d",&g);
	
	while(g>3||g<1)
	{	
		printf("请重新输入(1-3):");
		scanf("%d",&g);
		
	}
	switch(g)
	{
		case 1:guanliyuan();break;
		case 2:Huiyuan();break;
		case 3:exit(0);
		default:break;
		
	} 
	Turn_menu();
}


void guanliyuan()
{
	int n;
	system("cls");
		printf("\n\n\t||==================================================||\n");
		printf("\t||            欢迎进入鑫鑫图书屋                    ||\n");
		printf("\t||==================================================||\n");
		printf("\t||		1-显示所有                          ||\n");
		printf("\t||		2-添加图书                          ||\n");
		printf("\t||		3-修改图书                          ||\n");
		printf("\t||		4-删除图书                          ||\n");
		printf("\t||		5-书籍状态                          ||\n");
		printf("\t||		6-借阅情况                          ||\n");
		printf("\t||		7-逾期罚款                          ||\n");
		printf("\t||		8-会员名单                          ||\n");
		printf("\t||		9-修改密码                          ||\n");
		printf("\t||		10-退出                             ||\n");
		printf("\t||==================================================||\n");
		printf("\n请输入你的选择（1-10）:");
		scanf("%d",&n);
		
		while(n>10||n<1)
		{	
			printf("请重新输入(1-10):");
			scanf("%d",&n);
		
		}


		switch(n)
		{
			case 1:
				Show_ALL();/*调用显示所有图书的过程*/
				break;
			case 2:
				Add_Book();/*添加图书信息*/
				break;
			case 3:
				Modofy_Book();/*修改图书信息*/
				break;
			case 4:
				Delete_Book();/*删除图书信息*/
				break;
			case 5:
				zhuangt_Book();/*书籍状态统计*/
				break;
			case 6:
				jyqkuang_Book();/*借阅情况排序10*/
				break;
			case 7:
				fondpaixu_Book();/*逾期罚款排序5*/
				break;
			case 8:
				huiyuan_list();/*会员名单*/
				break;
			case 9:
				xiugaimima2();/*修改密码*/
				break;
			case 10:
				Turn_menu();/*退出*/
			default:break;
		
		}		
		Turn_menu1();
}




//***********************************************************************************************************************************************************//
void Show_ALL()
{
	char ch;

	
	FILE *fp;
	fp=fopen("F:\\Booklist.txt","r");

	printf("||===================================================||\n");
	printf("||                显示图书信息                       ||\n");
	printf("||===================================================||\n");
	
	if(fp==NULL)
	{
		printf("------------------图书列表内容为空！--------\n");
		printf("------------------是否添加图书？Y/N:");
		
		scanf("%c",&ch);
		//会自动生成换行符
		scanf("%c",&ch);
		while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n')
		{
			
				printf("\n请重新输入（Y/N）:");
				scanf("%c",&ch);
				scanf("%c",&ch);
				
		}
				if(ch=='Y'||ch=='y')
				{
					printf("----------------即将添加图书---------------\n");
					rep=creat();
					input();
				}

				else if(ch=='N'||ch=='n')
				{
					Turn_menu1();
				}
		
	}
	if(fp!=NULL)
	{
		printf("\n-----------------图书列表如下-----------------\n\n");
		printf("书号\t\t书名\t\t\t作者\t\t书籍状态\t借阅者\n");
		output();	/*显示文件内容*/
		Turn_menu1();
	}

}

// (1)   创建链表
Books_style *creat()
{
	Books_style *head;
	Books_style *p1,*p2;
	
	
	head=NULL;

	
	printf("请输入以下信息以“0 0 0”结束\n");
	printf("书号\t书名\t作者\n");
	

	p1=p2=(Books_style *)malloc(LEN);
	scanf("%s %s %s",p2->num,p2->name,p2->author);	
	strcpy(p2->zhuangtai,"可借 ");
	strcpy(p2->zhanghao1,"        ");
	
	p2->cishu=0;
	shu++;
	while( strcmp(p2->name,"0")!=0  ||strcmp(p2->num,"0")!=0|| strcmp(p2->author,"0")!=0)
	{	
		if(head==NULL)
		{
			head=p2;
			
		}
		else
		{
			p1->next=p2;
		}
		p1=p2;
		p2=(Books_style *)malloc(LEN);
		scanf("%s %s %s",p2->num,p2->name,p2->author);
		strcpy(p2->zhuangtai,"可借");
		strcpy(p2->zhanghao1," ");
		
		p2->cishu=0;
		shu++;
	}
	p1->next=NULL;
	free(p2);
	
	return head;
}



//  (2)    (将链表内容写入文本)
void input()/*返回输入馆藏总数*/
{
	FILE *fp;
	Books_style *p1=rep;
	

	if((fp=fopen("F:\\Booklist.txt","w"))==NULL)
	{
		printf("创建文本失败！");
		exit(0);
	}
		for(;p1->next!=NULL;p1=p1->next)
		{
				
				fprintf(fp,"%-8s\t%-20s\t%-12s\t%-4s\t%-10s\n",p1->num,p1->name,p1->author,p1->zhuangtai,p1->zhanghao1);
			
			
			if(ferror(fp))
			{
				printf("调用输入输出函数有错！");
				exit(1);
			}
		}
		//此时p1的数据还未输入，即链表的最后数据
	
		fprintf(fp,"%-8s\t%-20s\t%-12s\t%-4s\t%-10s\n",p1->num,p1->name,p1->author,p1->zhuangtai,p1->zhanghao1);
		

		fclose(fp);
		
		
}


// (3)    将文件里的内容显示到屏幕上   
void output()
{
		FILE *out;
		char ch;

		if((out=fopen("F:\\Booklist.txt","r"))==NULL)
		{
			printf("打开文本失败！");
			Turn_menu1();
		}
		
		while(!feof(out))
		{
			ch=fgetc(out);
			putchar(ch);
			
		}

		fclose(out);
}

//  (4)    跳转页面设置,询问是否跳到主界面
void Turn_menu()
{
	char ch2;

	printf("\n----------------是否返回上一级?(Y/N) :");
	scanf("%c",&ch2);
	scanf("%c",&ch2);

	while(ch2!='Y'&&ch2!='y'&&ch2!='N'&&ch2!='n')
	{
		printf("请重新输入（Y/N）:");
		scanf("%c",&ch2);
		scanf("%c",&ch2);
	}	

	if(ch2=='Y'||ch2=='y')
	{
		show_menu();
	}
	if(ch2=='N'||ch2=='n')
	{
		printf("------------------退出系统!---------------------\n");
		exit(0);			
	}
  	
}
void Turn_menu1()
{
	char ch2;


	printf("\n----------------是否返回上一级?(Y/N) :");
	scanf("%c",&ch2);
	scanf("%c",&ch2);


	while(ch2!='Y'&&ch2!='y'&&ch2!='N'&&ch2!='n')
	{
		printf("请重新输入（Y/N）:");
		scanf("%c",&ch2);
		
	}	

	if(ch2=='Y'||ch2=='y')
	{
		guanliyuan();
	}
	if(ch2=='N'||ch2=='n')
	{
		printf("------------------退出系统!---------------------\n");
		exit(0);			
	}
  	
}
void Turn_menu2(char str[],int di)
{
	char ch2;

	printf("\n----------------是否返回上一级?(Y/N) :");
	scanf("%c",&ch2);
	scanf("%c",&ch2);

	while(ch2!='Y'&&ch2!='y'&&ch2!='N'&&ch2!='n')
	{
		printf("请重新输入（Y/N）:");
		scanf("%c",&ch2);
		scanf("%c",&ch2);
	}	

	if(ch2=='Y'||ch2=='y')
	{
		chajian(str,di);
	}
	if(ch2=='N'||ch2=='n')
	{
		printf("------------------退出系统!---------------------\n");
		exit(0);			
	}
  	
}

void Turn_menu3()
{
	char ch2;

	printf("\n----------------是否返回上一级?(Y/N) :");
	scanf("%c",&ch2);
	scanf("%c",&ch2);

	while(ch2!='Y'&&ch2!='y'&&ch2!='N'&&ch2!='n')
	{
		printf("请重新输入（Y/N）:");
		scanf("%c",&ch2);
		scanf("%c",&ch2);
	}	

	if(ch2=='Y'||ch2=='y')
	{
		Huiyuan();
	}
	if(ch2=='N'||ch2=='n')
	{
		printf("------------------退出系统!---------------------\n");
		exit(0);			
	}
  	
}

//***********************************************************************************************************************************************************//


//添加图书
void Add_Book()
{
	Books_style *p=rep;
	Books_style *B;
	Books_style *t=rep;

	printf("||===================================================||\n");
	printf("||                添加图书信息                       ||\n");
	printf("||===================================================||\n");

	printf("----------请输入要向文本添加的内容-----------------\n");
	B=creat();

	if (rep==NULL)//A链表为空的情况，把bhead赋给ahead,使起始地址依然为ahead(即bhead)；
	{
		rep=B;
	}
	else 
	{
		while (p->next != NULL)
		{
			p = p->next;
		}
		
		p->next = B;//B接上A
	}

	input();
	printf("\n-------------添加到文件成功!-----------------------\n");
	printf("\n  温馨提示:若返回上级（且按1）可查看全文和刚添加的内容------------\n");
	
} 

//***********************************************************************************************************************************************************//

//修改图书 
void Modofy_Book()
{
	
	int n;
	Books_style Mod_data;

	char num1[10],name1[20],author1[10];
	Books_style  *p=rep;


	int flag=0;
	

	
	printf("||===================================================||\n");
	printf("||                修改图书信息                       ||\n");
	printf("||===================================================||\n");

	printf("\n   ----------原图书信息如下------------\n");
	output();

	printf("||==========================================||\n");
	printf("  ||                                          ||\n");
	printf("  || 1、按书号修改                            ||\n");
	printf("  || 2、按书名修改                            ||\n");
	printf("  ||                                          ||\n");
	printf("  ||==========================================||\n");

	
	printf("\n请输入你的选择（1-2）:");
	scanf("%d",&n);
	while(n>2||n<1)
	{	
		printf("请重新输入(1-2):");
		scanf("%d",&n);
	
	}
	
	if(n==1)
	{
		printf("请输入你想要修改图书的书号：");
		scanf("%s",Mod_data.num);
			
		while (p != NULL)
		{
			if(strcmp(Mod_data.num,p->num)==0)
			{
				flag=1;
				printf("请输入这本书修改过后的内容:\n");
				printf("书号：\n");
				scanf("%s",num1);
				strcpy(p->num,num1);
				printf("书名：\n");
				scanf("%s",name1);
				strcpy(p->name,name1);
				printf("作者：\n");
				scanf("%s",author1);
				strcpy(p->author,author1);
			}
			p = p->next;
		}	
	}
	p=rep;
	if(n==2)
	{
		printf("请输入你想要修改图书的书名：");
		scanf("%s",Mod_data.name);

		while (p != NULL)
		{
			
			if(strcmp(Mod_data.name,p->name)==0)
			{
				
				flag=1;
				printf("请输入这本书修改过后的内容:\n");
				printf("书号：\n");
				scanf("%s",num1);
				strcpy(p->num,num1);
				printf("书名：\n");
				scanf("%s",name1);
				strcpy(p->name,name1);
				printf("作者：\n");
				scanf("%s",author1);
				strcpy(p->author,author1);
			}
			p = p->next;
		}
		
	
	}
		if(flag==0)
		{
			printf("没有找到！");
		}
		else
		{
			printf("修改后的结果如下：\n");
			input();
			output();
		}

}


//***********************************************************************************************************************************************************//
//删除图书
void Delete_Book()
{
	int n;
	int flag=0;
	char num2[10],name2[20];
	Books_style  *p=rep;
	Books_style  *p1;

	printf("\n   ----------原图书信息如下------------\n");
	output();

	printf("||==========================================||\n");
	printf("  ||                                          ||\n");
	printf("  || 1、按书号删除                            ||\n");
	printf("  || 2、按书名删除                            ||\n");
	printf("  ||                                          ||\n");
	printf("  ||==========================================||\n");

	
	printf("\n请输入你的选择（1-2）:");
	scanf("%d",&n);
	while(n>2||n<1)
	{	
		printf("请重新输入(1-2):");
		scanf("%d",&n);
	
	}

	if(n==1)
	{
		printf("请输入你想要删除的图书的书号：");
		scanf("%s",num2);
			
		if(strcmp(num2,rep->num)==0)
		{
			flag=1;
			rep=rep->next;
		}
			else
		{
			while((strcmp(num2,p->num)!=0) && p != NULL)
			{
				p1=p;
				p = p->next;
			}	
			flag=1;
			p1->next = p->next;
		}
	}
	p=rep;
	if(n==2)
	{
		printf("请输入你想要删除的图书的书名：");
		scanf("%s",name2);
		
		if(strcmp(name2,p->name)==0)
		{
			flag=1;
			rep=rep->next;
		}
		else
		{
			while((strcmp(name2,p->name)!=0) && p != NULL)
			{
				p1=p;
				p = p->next;
			}	
			flag=1;
			p1->next= p->next;
		}		
	}
	
	if(flag==0)
	{
		printf("没有找到！");	
	}	
	else
		{
			printf("修改后的结果如下：\n");
			input();
			output();
		}
}

//会员清单
void huiyuan_list()
{
	int te;
	
	printf("\n共有%d位会员\n",r);
	printf("会员名:root\t\t密码:123456\n");
	for(te=1;te<r;te++)
	{
		printf("会员名:%s\t\t密码:%s\n",huiyuan[te].zhanghao,huiyuan[te].mima);
	}

}

//状态情况
void zhuangt_Book()
{

	printf("\n\n\t馆藏书籍总数(本)\t已借出总数(本)\t在馆书籍数(本)\n");
	printf("\t%d\t%d\t%d",shu,shu1,shu-shu1);

}

//借阅情况排序
void jyqkuang_Book()
{
	Books_style  *p=rep;
	Books_style  paixu[100];
	int ci=0,su=0,tu=0,ku=0,t,kawei=0;
	char string[20];

	while(p!=NULL)
	{

		strcpy(paixu[ci].num,p->num);/*书号*/
		strcpy(paixu[ci].name,p->name);/*书名*/
		strcpy(paixu[ci].author,p->author);/*作者*/		
		strcpy(paixu[ci].zhuangtai,p->zhuangtai); //借阅情况 
		paixu[ci].cishu=p->cishu;
		p=p->next;
		ci++;
	}
	ci--;
	for(su=0;su<ci-1;su++)
	{   ku=su;
		for(tu=su+1;tu<ci;tu++)
		if(paixu[tu].cishu>paixu[ku].cishu)
	 	ku=tu;
		if(ku!=su)
		{t=paixu[su].cishu;paixu[su].cishu=paixu[ku].cishu;paixu[ku].cishu=t;
		strcpy(string,paixu[su].num);strcpy(paixu[su].num,paixu[ku].num);strcpy(paixu[ku].num,string);
		strcpy(string,paixu[su].name);strcpy(paixu[su].name,paixu[ku].name);strcpy(paixu[ku].name,string);
		strcpy(string,paixu[su].author);strcpy(paixu[su].author,paixu[ku].author);strcpy(paixu[ku].author,string);}	
	}

	printf("\n------------------借阅情况排行榜------------------\n");
	printf("\t书号\t\t书名\t\t\t作者\t\t被借阅次数\n");
	for(su=0;su<ci;su++)
	{
		if(kawei>=10)
		break;
		
		
		printf("\t%-8s\t%-20s\t%-12s\t%d\n",paixu[su].num,paixu[su].name,paixu[su].author,paixu[su].cishu);
		kawei++;
	}

}

//逾期金额排序
void fondpaixu_Book()
{
	int su,ku,tu;
	double t;
	int kawei=0;
	char string[20];
	printf("\n\t------------------逾期金额排行榜----------------------\n");
	for(su=0;su<r-1;su++)
	{   ku=su;
		for(tu=su+1;tu<r;tu++)
		if(huiyuan[tu].date_jine>huiyuan[ku].date_jine)
	 	ku=tu;
		if(ku!=su)
		{t=huiyuan[su].date_jine;huiyuan[su].date_jine=huiyuan[ku].date_jine;huiyuan[ku].date_jine=t;
		strcpy(string,huiyuan[su].zhanghao);strcpy(huiyuan[su].zhanghao,huiyuan[ku].zhanghao);strcpy(huiyuan[ku].zhanghao,string);
		}	
	}
	printf("\n\t借阅者\t\t逾期金额\n");
	for(su=0;su<r;su++)
	{
		if(kawei>=5)
		break;		
		
		printf("\t%s\t\t%d\n",huiyuan[su].zhanghao,huiyuan[su].date_jine);
		kawei++;
	}
	

}


//修改账号密码
void xiugaimima2()
{

	int jishu=0,i;
	char mima2[12],zhanghao1[20];
	int keep=0;
	int flag=1;

	printf("账号:");
	scanf("%s",zhanghao1);
	for(i=0;i<r;i++)
	{
		if(strcmp(zhanghao1,huiyuan[i].zhanghao)==0)
		{		
			flag=0	;	
			printf("新密码：");
			scanf("%s",huiyuan[i].mima);
			printf("再次输入密码：");
			scanf("%s",mima2);
			while(strcmp(mima2,huiyuan[i].mima)!=0)
			{
				keep++;
				printf("\n两次密码不一样!\n");
				printf("再次输入密码：");
				scanf("%s",mima2);
				if(keep>3)
				{
					printf("已三次输入错误！自动退出");
					Turn_menu3();
				}
			}
			printf("\n\n--------------修改成功！--------------------\n\n");
		}
	
	}
	if(flag==1)
	{
		printf("没有该账号！");
	}
			
}



//***********************************************************************************************************************************************************//
//***********************************************************************************************************************************************************//
//会员信息
void Huiyuan()
{
	int gt;
	
	
	system("cls");
		printf("\n\n\t--------------欢迎进入登录页面---------------\n\n");
		printf("\n\t||=======================================||");
		printf("\n\t||     1.登录                            ||");
		printf("\n\t||     2.注册                            ||");
		printf("\n\t||     3.退出                            ||");
		printf("\n\t||=======================================||");
		printf("\n请输入你的选择（1-3）:");
			scanf("%d",&gt);
			while(gt>3||gt<1)
			{	
				printf("请重新输入(1-3):");
				scanf("%d",&gt);
			}
			switch(gt)
			{
				case 1:
					denglu();/*登录*/
					break;
				case 2:
					zhuce();/*注册*/
					break;
				case 3:
					Turn_menu();/*退出*/	
			}
		
		Turn_menu3();
		
		
}
void denglu()
{

	int i;
	char zhanghao1[20],mima1[12];
	int jishu=0,flag=1;
		
	printf("%d",r);
	for(i=0;i<r;i++)
	{
		printf("%s",huiyuan[i].zhanghao);
		printf("\t%s",huiyuan[i].mima);
		printf("\n");
	}
	printf("账号:");
	scanf("%s",zhanghao1);
	for(i=0;i<r;i++)
	{
		if(strcmp(zhanghao1,huiyuan[i].zhanghao)==0)
		{	flag=0;
			printf("密码:");
			scanf("%s",mima1);

			while(strcmp(mima1,huiyuan[i].mima)!=0)
			{
				printf("请重新输入密码：\n");
				scanf("%s",mima1);
				jishu++;
				if(jishu==3)
				{
					printf("\n\n----------------密码三次输入错误!----------------------\n");
					Turn_menu();
				}
			}		
		}					
	
	}
	i--;
	if(flag==1)
	{
		printf("没有该账号！");
	}
	else
		chajian(zhanghao1,i);
}

void chajian(char str[],int di)
{
	int n;
	system("cls");
			printf("\n\n\t||==================================================||\n");
			printf("\t||            欢迎进入鑫鑫图书屋                    ||\n");
			printf("\t||==================================================||\n");
			printf("\t||		1-显示所有                          ||\n");
			printf("\t||		2-查询图书                          ||\n");
			printf("\t||		3-借阅图书                          ||\n");
			printf("\t||		4-归还图书                          ||\n");
			printf("\t||		5-借阅情况                          ||\n");
			printf("\t||		6-修改密码                          ||\n");
			printf("\t||		7-退出                              ||\n");
			printf("\t||==================================================||\n");
			printf("\n请输入你的选择（1-7）:");
			scanf("%d",&n);
			while(n>7||n<1)
			{	
				printf("请重新输入(1-7):");
				scanf("%d",&n);
			}
			printf("\n%d\n",di);
			switch(n)
			{
				case 1:
					xianshi();/*调用显示所有图书的过程*/
					break;
				case 2:
					Query_Book();/*查询图书信息*/
					break;
				case 3:
					lent_book(str,di);input();/*借阅图书*/
					break;
			    case 4:
					back_Book(str,di);input();/*归还图书*/
					break;
		   	    case 5:
					jieyue_Book(str,di);/*借阅情况*/
					break;
				case 6:	
					xiugaimima(di);/*修改密码*/
					break;
				case 7:
					Turn_menu3();/*退出*/
				default:break;		
			}
						
	Turn_menu2(str,di);
}

void zhuce()
{

	int jishu=0;
	char mima2[12];
	int keep=0,zh=0,ko=0;

			printf("%d",r);
			printf("\n--------------欢迎进入注册页面！--------------\n");
			printf("注册账号：");	
			scanf("%s",huiyuan[r].zhanghao);

			for(zh=0;zh<r;zh++)
			{
				if(strcmp(huiyuan[r].zhanghao,huiyuan[zh].zhanghao)==0)
				{
					printf("\n-----------------此账号已被注册！！！----------------\n");
					printf("请重新输入：");
					printf("注册账号：");	
					scanf("%s",huiyuan[r].zhanghao);
					zh=-1;
				}
			}

			printf("密码：");
			scanf("%s",huiyuan[r].mima);
			printf("再次输入密码：");
			scanf("%s",mima2);
			while(strcmp(mima2,huiyuan[r].mima)!=0)
			{
				keep++;
				printf("\n两次密码不一样!\n");
				printf("再次输入密码：");
				scanf("%s",mima2);
				if(keep>3)
				{
					printf("已三次输入错误！自动退出");
					Turn_menu3();
				}
			}
			printf("\n\n--------------注册成功！--------------------\n\n");
			r++;
			printf("%d",r);
			
}

//查询图书
void Query_Book()
{
	int n;
	int flag=0;
	char num2[10],name2[20];
	Books_style  *p=rep;

	

	printf("\n\n\t||==========================================||\n");
	printf("\t||                                          ||\n");
	printf("\t|| 1、按书号查询                            ||\n");
	printf("\t|| 2、按书名查询                            ||\n");
	printf("\t||                                          ||\n");
	printf("\t||==========================================||\n");

	
	printf("\n请输入你的选择（1-2）:");
	scanf("%d",&n);
	while(n>2||n<1)
	{	
		printf("请重新输入(1-2):");
		scanf("%d",&n);
	
	}
	
	if(n==1)
	{
		printf("请输入你想要查询的图书的书号：");
		scanf("%s",num2);
			
		while (p != NULL)
		{	
			if(strcmp(num2,p->num)==0)
			{
				flag=1;
				printf("学号：%s\t姓名：%s\t作者：%s",p->num,p->name,p->author);
				printf("\t书籍状态:%s\n",p->zhuangtai);
			}
			p = p->next;
		}	
	}
	p=rep;
	if(n==2)
	{
		printf("请输入你想要查询的图书的书名：");
		scanf("%s",name2);

		while (p != NULL)
		{
			if(strcmp(name2,p->name)==0)
			{
				flag=1;
				printf("学号：%s\t姓名：%s\t作者：%s",p->num,p->name,p->author);
				printf("\t书籍状态:%s\n",p->zhuangtai);
			}
			p = p->next;
		}
		
	}
	p=rep;
	if(flag==0)
	{
		printf("没有找到！");	
	}	
	
}


//借阅图书
void lent_book(char str[],int di)
{
	int n;
	int flag=0;
	char gt,ch;
	char num2[10],name2[20];
	Books_style  *p=rep;

	printf("\n\t----------原图书信息如下------------\n");
	xianshi();
	printf("\n\n\t||==========================================||\n");
	printf("\t||                                          ||\n");
	printf("\t|| 1、按书号借阅                            ||\n");
	printf("\t|| 2、按书名借阅                            ||\n");
	printf("\t||                                          ||\n");
	printf("\t||==========================================||\n");

	
	printf("\n请输入你的选择（1-2）:");
	scanf("%d",&n);
	while(n>2||n<1)
	{	
		printf("请重新输入(1-2):");
		scanf("%d",&n);
		
	
	}
	
	if(n==1)
	{
		printf("请输入你想要借阅的图书的书号：");
		scanf("%s",num2);
			
		while (rep != NULL)
		{	
			if(strcmp(num2,rep->num)==0)
			{
				flag=1;
				printf("\n该书的图书信息：\n");
				printf("学号：%s\t姓名：%s\t作者：%s",rep->num,rep->name,rep->author);
				printf("\t书籍状态:%s\n",rep->zhuangtai);
				if(strcmp(rep->zhuangtai,"已借出")==0)
				{
					
					printf("\n-------------------该书已被借出！----------------------\n");
					Turn_menu2(str,di);
				}
				printf("请确认是否借阅此书(Y/N):");
				scanf("%c",&ch);
				scanf("%c",&ch);
				while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n')
				{
					printf("请重新输入（Y/N）");
					scanf("%c",&ch);
				
				}	

				if(ch=='Y'||ch=='y')
				{
					strcpy(rep->zhuangtai,"已借出");
					shu1++;
					rep->cishu++;
					strcpy(rep->zhanghao1,str);
					jilutime(di);
					printf("------------------借阅成功!---------------------\n");
					break;
				}
				if(ch=='N'||ch=='n')
				{
					printf("------------------退出系统!---------------------\n");			
				}
				
			}
			rep = rep->next;
		}	
	}
	rep=p;
	if(n==2)
	{
		printf("请输入你想要借阅的图书的书名：");
		scanf("%s",name2);

		while (rep != NULL)
		{
			if(strcmp(name2,rep->name)==0)
			{
				flag=1;
				printf("学号：%s\t姓名：%s\t作者：%s",rep->num,rep->name,rep->author);
				printf("\t书籍状态:%s\n",rep->zhuangtai);
				printf("请确认是不是此书！（Y/N）:");
				scanf("%c",&gt);
				scanf("%c",&gt);
				while(gt!='Y'&&gt!='y'&&gt!='N'&&gt!='n')
				{
					printf("请重新输入（Y/N）");
					scanf("%c",&gt);
					scanf("%c",&gt);
				}	
				if((gt=='Y'||gt=='y'))
				{
					if(strcmp(rep->zhuangtai,"已借出")==0)
					{
						printf("\n-------------------该书已被借出！----------------------\n");
						Turn_menu2(str,di);
					}
					printf("请确认是否借阅此书(Y/N):");
					scanf("%c",&ch);
					while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n')
					{
						printf("请重新输入（Y/N）");
						scanf("%c",&ch);
						scanf("%c",&ch);
					}	

					if(ch=='Y'||ch=='y')
					{
						strcpy(rep->zhuangtai,"已借出");
						shu++;
						rep->cishu++;
						strcpy(rep->zhanghao1,str);
						jilutime(di);
						printf("------------------借阅成功!---------------------\n");
						break;
					}
					if(ch=='N'||ch=='n')
					{
						printf("------------------退出系统!---------------------\n");
						Turn_menu2(str,di);
						
					}
				}

			}
			rep =rep->next;
		}
		
	}
	rep=p;
	if(flag==0)
	{
		printf("没有找到！");	
	}	
	
}


//归还图书
void back_Book(char str[],int di)
{
	int n;
	int flag=0;
	char gt,ch;
	char num2[10],name2[20];
	Books_style  *p=rep;
	

	printf("\n\t----------借阅信息如下------------\n");
	jieyue_Book(str,di);

	printf("\n\n\t||==========================================||\n");
	printf("\t||                                          ||\n");
	printf("\t|| 1、按书号归还                            ||\n");
	printf("\t|| 2、按书名归还                            ||\n");
	printf("\t||                                          ||\n");
	printf("\t||==========================================||\n");

	
	printf("\n请输入你的选择（1-2）:");
	scanf("%d",&n);
	while(n>2||n<1)
	{	
		printf("请重新输入(1-2):");
		scanf("%d",&n);
	
	}
	
	if(n==1)
	{
		printf("请输入你想要归还的图书的书号：");
		scanf("%s",num2);
			
			
		while (p != NULL)
		{	
			if(strcmp(num2,p->num)==0)
			{
				flag=1;
				printf("\n该书的图书信息：\n");
				printf("学号：%s\t姓名：%s\t作者：%s",p->num,p->name,p->author);
				printf("\t书籍状态:%s\n",p->zhuangtai);
				if(strcpy(p->zhuangtai,"可借")==0)
				{
					printf("\n-------------------该书未被借出！----------------------\n");
					Turn_menu2(str,di);
				}
				printf("请确认是否归还此书(Y/N):");
				scanf("%c",&ch);
				scanf("%c",&ch);
				while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n')
				{
					printf("请重新输入（Y/N）");
					scanf("%c",&ch);
					scanf("%c",&ch);
				}	

				if(ch=='Y'||ch=='y')
				{
					strcpy(p->zhuangtai,"可借 ");
					shu1--;
					strcpy(rep->zhanghao1,"   ");
					
					printf("------------------归还成功!---------------------\n");
					break;
				}
				if(ch=='N'||ch=='n')
				{
					printf("------------------退出系统!---------------------\n");			
				}
				
			}
			p = p->next;
		}	
	}
	p=rep;
	if(n==2)
	{
		printf("请输入你想要查询的图书的书名：");
		scanf("%s",name2);

		while (p != NULL)
		{
			if(strcmp(name2,p->name)==0)
			{
				flag=1;
				printf("学号：%s\t姓名：%s\t作者：%s",p->num,p->name,p->author);
				printf("\t书籍状态:%s\n",p->zhuangtai);
				
				if(strcpy(p->zhuangtai,"可借")==0)
				{
					printf("\n-------------------该书未被借出！----------------------\n");
					Turn_menu2(str,di);
				}
				printf("请确认是不是此书！（Y/N）：");
				scanf("%c",&gt);
				scanf("%c",&gt);
				
				while(gt!='Y'&&gt!='y'&&gt!='N'&&gt!='n')
				{
					printf("请重新输入（Y/N）");
					scanf("%c",&gt);
					scanf("%c",&ch);
				}	
				if((gt=='Y'||gt=='y'))
				{
					printf("请确认是否借归还此书(Y/N):%c",&ch);
					while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n')
					{
						printf("请重新输入（Y/N）");
						scanf("%c",&ch);
						scanf("%c",&ch);
					}	

					if(ch=='Y'||ch=='y')
					{
						strcpy(p->zhuangtai,"可借 ");
						shu1--;
						strcpy(rep->zhanghao1,"  ");
						
						printf("------------------归还成功!---------------------\n");
						break;
					}
					if(ch=='N'||ch=='n')
					{
						printf("------------------退出系统!---------------------\n");
						Turn_menu2(str,di);
						
					}
				}

			}
			p = p->next;
		}
		
	}
	p=rep;
	if(flag==0)
	{
		printf("没有找到！");	
	}	
	
}



void jieyue_Book(char string[],int n)
{	
	int zongs=0;
	Books_style  *p=rep;
	double zongmon=0;
	jilutime3();
	
	while (p != NULL)
		{	
			if(strcmp(string,p->zhanghao1)==0)
			{
				printf("\n\n\t书号\t\t书名\t\t\t作者\t\t逾期金额\n");
				p->yuqimoney=yuqifond_Book(n);
				printf("\t%-8s\t%-20s\t%-12s\t%f\n",p->num,p->name,p->author,p->yuqimoney);
				zongmon=zongmon+p->yuqimoney;
				zongs++;
			}
			p = p->next;
		}
	huiyuan[n].jieyueshu=zongs;
	printf("\n共借本数:%d\n",huiyuan[n].jieyueshu);
	huiyuan[n].date_jine=zongmon;
}


double yuqifond_Book(int n)
{
	int year1;
	int wei;
	double chaoqi=0,sum=0; 

	jilutime2();
	year1=second.nian-first[n].nian;
	if(year1==0)
	{
		for(wei=first[n].yue;wei<second.yue;wei++)
		{
			switch(wei)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:sum=sum+31;break;
				case 4:
				case 6:
				case 9:
				case 11:sum=sum+30;break;
				case 2:sum=sum+28;break;
			}
		
		}
	
	}
	else
	{
		
		for(wei=first[n].yue;wei<=12;wei++)
		{
			switch(wei)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:sum=sum+31;break;
				case 4:
				case 6:
				case 9:
				case 11:sum=sum+30;break;
				case 2:sum=sum+28;break;
				default:break;
			}
		
		}	
		
		for(wei=1;wei<second.yue;wei++)
		{
			if(second.yue==1)break;
			switch(wei)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:sum=sum+31;break;
				case 4:
				case 6:
				case 9:
				case 11:sum=sum+30;break;
				case 2:sum=sum+28;break;
				default:break;
			}	
		}
		
		sum=sum+second.ri;
		sum=sum+year1*365;					
	}

	if(sum>30)
	{
		chaoqi=0.50*sum;
	}

	return chaoqi;
}


void jilutime(int n)
{
	 struct tm *local;
  	 time_t t; 
  	 t=time(NULL); 
  	 local=localtime(&t); 
	 printf ("\n%d-%d-%d  ", 1900+local->tm_year, 1+local->tm_mon,local->tm_mday);
	 printf("%d:%d:%d\n",  local->tm_hour, local->tm_min, local->tm_sec);
	
	 first[n].nian=1900+local->tm_year;
	first[n].yue=1+local->tm_mon;
	first[n].ri=local->tm_mday;
   	
}


void jilutime2()
{
	 struct tm *local;
  	 time_t t; 
  	 t=time(NULL); 
  	 local=localtime(&t); 
	 second.nian=1900+local->tm_year;
	second.yue=1+local->tm_mon;
	second.ri=local->tm_mday;
   	
}


void jilutime3()
{
	 struct tm *local;
  	 time_t t; 
  	 t=time(NULL); 
  	 local=localtime(&t);
	 printf("\n本次查询时间： ");
	 printf ("%d-%d-%d  ", 1900+local->tm_year, 1+local->tm_mon,local->tm_mday);
	 printf("%d:%d:%d\n",  local->tm_hour, local->tm_min, local->tm_sec);
	
   	
}


void xiugaimima(int n)
{

	int jishu=0;
	char mima2[12];
	int keep=0;
	char mima1[12];

		
			printf("旧密码:");
			scanf("%s",mima1);

			while(strcmp(mima1,huiyuan[n].mima)!=0)
			{
				printf("请重新输入密码：\n");
				scanf("%s",mima1);
				jishu++;
				if(jishu==3)
				{
					printf("\n\n----------------密码三次输入错误!----------------------\n");
					Turn_menu();
				}
			}		
						
			printf("新密码：");
			scanf("%s",huiyuan[n].mima);
			printf("再次输入密码：");
			scanf("%s",mima2);
			while(strcmp(mima2,huiyuan[n].mima)!=0)
			{
				keep++;
				printf("\n两次密码不一样!\n");
				printf("再次输入密码：");
				scanf("%s",mima2);
				if(keep>3)
				{
					printf("已三次输入错误！自动退出");
					Turn_menu3();
				}
			}
			printf("\n\n--------------修改成功！--------------------\n\n");
			Turn_menu3();		
}

void xianshi()
{
	Books_style  *p=rep;	
	printf("\n\n\t书号\t\t书名\t\t\t作者\t\t书籍状态\n");
	while (p != NULL)
		{	
				printf("\t%-8s\t%-20s\t%-12s\t%-3s \n",p->num,p->name,p->author,p->zhuangtai);
			p = p->next;
		}

}

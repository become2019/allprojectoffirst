#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h> 
#include<conio.h> 
#include "小超市商品管理系统.h"



//结构体的声明，宏定义，函数声明文件
 char s[10][15]={
"编号", 
"商品名称",
"进价(元)",
"售价(元)",
"进货量",
"销售量",
"剩余数",
"毛利(元)",
"过期时间",
"销售额(元)"};



void UserInformation()//用户信息管理系统 
{ 
	char choose;
	DrawUser();
	fflush(stdin); 
	while(1)
	{
		JudgeChoose(&choose);
		switch(choose)
		{
			case '1':UserLogin();break;
			case '2':UserRegister();break;
			case '3':exit(0);break;
			default:printf("输入不正确，请重新输入！\n");
		}
		if(choose=='1')
		  break;
	}
}

void DrawUser()//选择用户界面  
{
	system("cls");
	printf("\n\n");
	printf("\t\t****************************************************************\n\n");
	printf("\t\t\t\t\t1.用户登录\n\n");
	printf("\t\t\t\t\t2.用户注册\n\n");
	printf("\t\t\t\t\t3.退出系统\n\n");
	printf("\t\t****************************************************************\n");
	printf("请输入您的选择：（1-3）:"); 
} 

void UserLogin()//用户登录 
{
	 int j;
	 int i=3;//登录次数 
	 char name[100];
	 char password[100]; 
	 char account[10];
    char passwrd[17];
	 FILE *faccount;
	 faccount=fopen("password.txt","r");
	 fscanf(faccount,"%s %s",account,passwrd); 
	 system("cls");
	 while(1)
	 {
	 	printf("请输入账号：");
	    scanf("%100s",name);
	    fflush(stdin);//防止溢出攻击 
	    printf("请输入密码：");
	    for(j=0;j<100;j++)
	    {
	    	password[j]=getch();
	    	if(password[j]=='\r')
	    	     {
	    	     	password[j]='\0';
	    	     	break;
	    	     }
	    	else
	    	printf("*");
	    }
	    fflush(stdin);
	    if(strcmp(account,name)==0&&strcmp(password,passwrd)==0)
	               {
	               	printf("成功");
	               	 fclose(faccount);
	               	break;//返回useriformation函数 
	               }
	    if(i==0)
		{
			fseek(faccount,0,0); 
			printf("\n账号已经被锁定！请联系作者！");
			fprintf(faccount,"%d",i);
			fclose(faccount);
			exit(0); 
		} 
	    printf("\n账号错误或密码错误！你还有%d次机会输入\n",i--);
     }
}

void UserRegister()//用户注册 
{
	char choose;
	system("cls");
	printf("\t\t****************************************************************\n\n");
	printf("\t\t\t\t\t注册请联系QQ:1246767689\n");
	printf("\t\t\t\t\t  作者：Alexander\n");
	printf("\t\t\t\t\t   按1键返回\n");
	printf("\t\t****************************************************************\n\n");
	printf("请输入您的选择：");
	while(1)
	{
		JudgeChoose(&choose);
		switch(choose)
		{
			case '1':DrawUser();break;
			default:printf("选择错误，请重新输入！");
		}
		if(choose=='1')
		 break;
	}
	
}

void CommodityInformation()//商品信息管理 
{
	HISTORY *headhistory=NULL;
	FILE *fp;
	COMMODITY *head=NULL;
	char choose;
	char string[30]="      用户登录成功!!\n";
	headhistory=OpenLog();
	
	WriteHistory(headhistory,string);
	fp=fopen("小超市商品管理.txt","r+");
	if(fp==NULL)
	{
		printf("资源加载失败！请检查！"); 
		exit(0);
	}
	DrawCommodity();
	head=MakeList(fp);//创建链表 
	while(1)
	{
		JudgeChoose(&choose);
		switch(choose)
		{
			case '1':AddCommodity(head);WriteHistory(headhistory,"  1.  添加商品\n");break;
			case '2':ChangeCommodity(head);WriteHistory(headhistory,"  2.  修改商品信息\n");break;
			case '3':InquiryInformation(head,headhistory);break;
			case '4':BuyCommodity(head);WriteHistory(headhistory,"  4.  进货添加\n");break;
			case '5':DeleteCommodity(head);WriteHistory(headhistory,"  5.  删除商品\n");break;
			case '6':LookUpCommodity(head,headhistory);break;
			case '7':LookHistory(headhistory);break; 
			case '8':EXIT(head,headhistory);break;
			case '9':EXIT(head,headhistory);exit(0);break;
			default:printf("选择错误，请重新输入！");
		}
	 if(choose=='8')
	   break;
	}
	UserInformation();
	CommodityInformation();
} 


void DrawCommodity()//选择功能界面 
{
	system("cls");
	
    printf("\t\t****************************************************************\n\n");
    printf("\t\t\t\t\t1.添加商品\n\n");
    printf("\t\t\t\t\t2.修改商品信息\n\n");
    printf("\t\t\t\t\t3.查询\n\n");
    printf("\t\t\t\t\t4.进货添加和购买商品\n\n");
    printf("\t\t\t\t\t5.删除商品\n\n");
    printf("\t\t\t\t\t6.查看商品\n\n");
    printf("\t\t\t\t\t7.历史记录\n\n");
    printf("\t\t\t\t\t8.返回登录页面\n\n");
    printf("\t\t\t\t\t9.完全退出\n\n");
	printf("\t\t****************************************************************\n\n");
	printf("请输入您的选择(1-7)：");
}

void AddCommodity(COMMODITY *head)//添加商品 
{
	COMMODITY *current,**link=&head;
	char choose;
	system("cls");
	while(current=*link)
	 link=&current->next;
    while(1)
    {
    	current=(COMMODITY *)malloc(sizeof(COMMODITY));
    	printf("%s：",s[0]);
    	scanf("%d",&current->num); 
    	printf("%s：",s[1]);
    	scanf("%s",current->name);
    	printf("%s：",s[2]);
    	scanf("%f",&current->purchase_price);
    	printf("%s：",s[3]);
    	scanf("%f",&current->selling_price);
    	printf("%s：",s[4]);
    	scanf("%d",&current->purchae_num);
    	printf("%s：",s[5]);
    	scanf("%d",&current->selling_num);
    	printf("%s：",s[6]);
    	scanf("%d",&current->surplus_num);
    	printf("%s：",s[7]);
    	scanf("%f",&current->profit);
    	printf("%s：",s[8]);
    	scanf("%s",current->date);
    	printf("%s：",s[9]);
    	scanf("%f",&current->salves_tall);
		Calculate(current);
		*link=current;
		link=&current->next;
		printf("是否继续输入（Y？N）:");
		//添加选择算法判断 
		fflush(stdin); 
	   do
		{
		scanf("%c",&choose);
		getchar();
		switch(choose)
		{
			case 'Y':;break;
			case 'y':;break;
			case 'N':DrawCommodity(); break;
			case 'n':DrawCommodity();break;
			default:printf("\n输入错误，是否继续添加（Y？N）:"),choose=0;
		} 
	   }while(choose==0);
		if(choose=='N'||choose=='n')
		   break;
    }
	*link=NULL; 
	
}


void ChangeCommodity(COMMODITY *head)//修改商品信息 
{
	//储存修改后的商品信息 
   	COMMODITY temp; 
    COMMODITY *current,**link=&head;
	char choose;
	system("cls");
	current=InquiryName(head);
	if(current)
{
    printf("请输入修改后的商品信息:\n");
    	printf("%s：",s[0]);
    	scanf("%d",&current->num); 
    	printf("%s：",s[1]);
    	scanf("%s",current->name);
    	printf("%s：",s[2]);
    	scanf("%f",&current->purchase_price);
    	printf("%s：",s[3]);
    	scanf("%f",&current->selling_price);
    	printf("%s：",s[4]);
    	scanf("%d",&current->purchae_num);
    	printf("%s：",s[5]);
    	scanf("%d",&current->selling_num);
    	printf("%s：",s[6]);
    	scanf("%d",&current->surplus_num);
    	printf("%s：",s[7]);
    	scanf("%f",&current->profit);
    	printf("%s：",s[8]);
    	scanf("%s",current->date);
    	printf("%s：",s[9]);
    	scanf("%f",&current->salves_tall);
		//读取修改信息末尾的换行符
	  getchar(); 
    Calculate(current);;
	printf("修改成功!\n");
}
	printf("按1返回............................\n");
	do
	{
		JudgeChoose(&choose); 
		if(choose=='1')
		  break;
		else
		printf("请多点真诚，少点套路,重新输入：\n");
	}while(choose=='0');
	DrawCommodity();
}

void InquiryInformation(COMMODITY *head,HISTORY *headhistory)//查询信息 
{
	char choose;
	system("cls");
	WriteHistory(headhistory,"  3.  查询\n");
	 printf("\t\t****************************************************\n\n");
	printf("\t\t\t1.按商品名称查询\n\n");
	printf("\t\t\t2.输入剩余数查询\n\n"); 
	printf("\t\t\t3.返回\n\n");
	printf("\t\t****************************************************\n\n");
	do
	{
		printf("请输入您的选择（1~3）：");
		JudgeChoose(&choose);
		switch(choose)
		{
			case '1':InquiryName(head);WriteHistory(headhistory,"  3.1 按商品名称查询\n");break;
			case '2':InquirySurplus(head);WriteHistory(headhistory,"  3.2 输入剩余数查询\n");break;
			case '3':WriteHistory(headhistory,"  3.3 返回主界面\n");break;
			default :printf("输入错误，"); 
			
		}
		if(choose=='3')
			break;
	}while(1);
	DrawCommodity();
}

COMMODITY *InquiryName(COMMODITY *head)//按名称查询 
{
    COMMODITY *current,**link=&head;
    char name[15];
    printf("请输入要查询的商品名称：");
    gets(name);
	//进行遍历查找 
    while((current=*link)!=NULL&&strcmp(name,current->name)!=0)
         link=&current->next;
    if(*link) 
    {
    printf("%4s%13s%10s%12s%10s%12s%12s%12s%14s%14s\n",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9]);
    printf("%-4d%13s%10.1f%12.1f%10d%12d%12d%12.1f%14s%14.1f\n",current->num,current->name,current->purchase_price,
		current->selling_price,current->purchae_num,current->selling_num,current->surplus_num,
		current->profit,current->date,current->salves_tall);
	} 
	else 
	printf("没有查到该商品的信息，请确认后查询\n");
	return current; 
}

void InquirySurplus(COMMODITY *head)//按剩余数查询 
{
	//剩余数 
	int surplus;
	 COMMODITY *current,**link=&head;
	 printf("请输入查询的剩余数：");
	 scanf("%d",&surplus); 
	 getchar();
	 printf("%4s%13s%10s%12s%10s%12s%12s%12s%14s%14s\n",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9]);
	 while((current=*link)!=NULL)
         {
         	if(current->surplus_num<=surplus)
        printf("%-4d%13s%10.1f%12.1f%10d%12d%12d%12.1f%14s%14.1f\n",current->num,current->name,current->purchase_price,
		current->selling_price,current->purchae_num,current->selling_num,current->surplus_num,
		current->profit,current->date,current->salves_tall);
		     link=&current->next;
         }
     
}


void BuyCommodity(COMMODITY *head) //进货 
{
	char choose;
    char name[15];
	int num; 
    COMMODITY *current,**link=&head;
    system("cls");
    printf("\t\t****************************************************\n\n");
	printf("\t\t\t1.进货\n\n");
	printf("\t\t\t2.购买商品\n\n"); 
	
	printf("\t\t****************************************************\n\n");
    printf("请输入你的选择(1-2):");
    while(1)
    {
    	choose=getchar();
switch(choose)
{
	case '1': 
	printf("请输入商品和要增加的商品数量：\n");
    scanf("%s %d",name,&num);
    //进行遍历查找 
    while((current=*link)!=NULL&&strcmp(name,current->name)!=0)
         link=&current->next;
	if(current)
	{
	 current->purchae_num+=num;
	 Calculate(current);
	 printf("进货成功！\n");
	} 
	 else
	 printf("进货失败，请检查商品名称是否正确！\n"); 
	 fflush(stdin);
	 break;
	 case '2':
	 	printf("请输入商品和要购买的商品数量：\n");
	 	scanf("%s %d",name,&num);
    //进行遍历查找 
    while((current=*link)!=NULL&&strcmp(name,current->name)!=0)
         link=&current->next;
	if(current)
	{
	 current->selling_num+=num;
	 Calculate(current);
	 printf("购买成功！\n");
	} 
	 else
	 printf("购买失败，请检查商品名称是否正确！\n"); 
	 fflush(stdin);
	 break;
	 
}
    if(choose=='1'||choose=='2')
	   break;
	 else
	 {
	 	printf("输入错误，请重新输入！\n");
	 	getchar();
	 } 
}
	do
	{
		printf("按1返回......\n");
		fflush(stdin);
		choose=getch();
		if(choose=='1')
		  break;
		else
		 choose='0';
	}while(choose=='0') ;
    DrawCommodity();
}

void DeleteCommodity(COMMODITY *head)//删除商品 
{
	char c; 
	char choose;
	COMMODITY *current,**link=&head;
	char name[15];
	system("cls");
	printf("请输入要删除的商品：");
	gets(name);
	while((current=*link)!=NULL&&strcmp(name,current->name)!=0)
         link=&current->next;
    if(current==NULL)
    {
    	printf("输入错误，请检查输入的商品名称是否正确！\n");
    }
    else
    {
	printf("是否删除？Y/N\n");
	if((c=getchar())=='Y'||c=='y')
	{
		
         *link=current->next;
		 free(current);
		 printf("删除成功！\n");
	}
    }
	do
	{
		printf("按1键返回....\n");
		 fflush(stdin);
		choose=getch();
		if(choose=='1')
		  break;
		else
		 choose='0';
	}while(choose=='0');
	DrawCommodity();
	
} 

void LookUpCommodity(COMMODITY *head,HISTORY *headhistory)//查看商品 
{
	COMMODITY *current,**link=&head;
	char choose;
	WriteHistory(headhistory,"  6.  查看商品\n"); 
	fflush(stdin);
	system("cls"); 
	do
	{
	
	printf("\t\t****************************************************\n\n");
	printf("\t\t\t1.正常顺序显示\n\n");
	printf("\t\t\t2.按毛利高到低的顺序显示\n\n");
	printf("\t\t\t3.按销售高到低的顺序显示\n\n"); 
	printf("\t\t\t4.按过期时间近到远的顺序显示\n\n"); 
	printf("\t\t\t5.返回\n\n");
	printf("\t\t****************************************************\n\n");
	choose=getch();
	switch(choose)
	{
		case '1': head=sort(head,comp);print(head);WriteHistory(headhistory,"  6.1 正常顺序显示\n");break;
		case '2': head=sort(head,compprofit);print(head);WriteHistory(headhistory,"  6.2 按毛利高到低的顺序显示\n");break;
		case '3': head=sort(head,compsell);print(head);WriteHistory(headhistory,"  6.3 按销售高到低的顺序显示\n");break; 
		case '4': head=sort(head,compdate);print(head);WriteHistory(headhistory,"  6.4 按过期时间近到远的顺序显示\n");break; 
		case '5':choose='0';WriteHistory(headhistory,"  6.5 返回主界面\n");break;
	}
	if(choose=='0')
	break;
	} while(1);
	DrawCommodity();
} 


void LookHistory(HISTORY *head)
{
	HISTORY *p;
	char choose;
	system("cls");
	p=head->next;
	if(p)
	{
		while(p)
		{
			printf("%s",p->operate_time);
			p=p->next;
		}
	}
	else
	{
		printf("没有历史记录！\n");
	}
	while(1)
	{
	printf("按1返回主界面...........\n");
	choose=getch();
	if(choose=='1')
	   break;
    }
    DrawCommodity();
}


void JudgeChoose(char *choose)//菜单选择使用的算法 
{
	scanf("%c",choose);
		if(getchar()!='\n')
		  {
		  	   *choose='0';
		  	   fflush(stdin);
		  } 
}



COMMODITY *MakeList(FILE *fp)//创建链表 
{
	COMMODITY *head;
	COMMODITY *current;
	COMMODITY **link=&head;
	//读取文本中的第一行 
  	char abandon[200];
	fgets(abandon,200,fp);
	while(1)
	{
		current=(COMMODITY *)malloc(sizeof(COMMODITY ));
		fscanf(fp,"%d %s %f %f %d %d %d %f %s %f",&current->num,current->name,&current->purchase_price,
		&current->selling_price,&current->purchae_num,&current->selling_num,&current->surplus_num,
		&current->profit,current->date,&current->salves_tall);
		Calculate(current);
		 if(feof(fp)!=0)
		   break;
		*link=current;
		link=&current->next;
	
	}
	*link=NULL;
	link=&head;
	//关闭缓冲区，程序运行时信息在链表中储存，退出时更新文件 
	fclose(fp); 
	return head;
}



int comp(const void *a,const void *b)
{
	return (*(COMMODITY *)a).num>(*(COMMODITY*)b).num?1:-1;
}

int compprofit(const void *a,const void *b)
{
return (*(COMMODITY *)b).profit>(*(COMMODITY*)a).profit?1:-1;
}

int compsell(const void *a,const void *b)
{
	return (*(COMMODITY*)b).salves_tall>(*(COMMODITY *)a).salves_tall?1:-1; 
}


int compdate(const void *a,const void *b)//多关键字 
{
	int cyear,dyear;
	int cmonth,dmonth;
	int cday,dday;
	COMMODITY *c=(COMMODITY*)a;
	COMMODITY *d=(COMMODITY*)b;
	sscanf(c->date,"%d/%d/%d",&cyear,&cmonth,&cday);
	sscanf(d->date,"%d/%d/%d",&dyear,&dmonth,&dday);
	if(cyear!=dyear)
	 return cyear-dyear;
	else if(cmonth!=dmonth)
	 return cmonth-dmonth;
	else if (cday!=dday)
	return cday-dday;
} 

COMMODITY *sort(COMMODITY *head,int (*comp)(const void *a,const void *b))//用选择排序法进行链表的排序 
{
	 COMMODITY *current,**link=&head,*current1,**link1=&head,**templink,*tempcurrent,*temp;
	 while(current=*link)
	 {
	 	link1=&current->next;
	 	templink=link;
	 	tempcurrent=current;
	 	while(current1=*link1)//往后扫描比当前最小的 
	 	{
	 		if(comp(tempcurrent,current1)>0)
	 		{
	 			    templink=link1;
	 			    tempcurrent=current1;
	 		}
	 		link1=&current1->next;
	 	}
	 	if(current->next==tempcurrent)//当当前的节点的下一个节点是tempcurrent时 
	 	{
	 		*link=tempcurrent;
	 		temp=tempcurrent->next;
	 		tempcurrent->next=current;
	 		current->next=temp;
	 		link=&tempcurrent->next;
	 	}
	 	else if(tempcurrent!=current)
	 	{
	 		temp=current->next;
	 		current->next=tempcurrent->next;
	 		tempcurrent->next=temp;
	 		*link=tempcurrent;
	 		*templink=current;
	 		link=&tempcurrent->next;
	 	}
	 	else
	 	{
	 		link=&current->next;
	 	}
	 }
	 return head;
} 


void print(COMMODITY *head)
{
	COMMODITY **link=&head,*current;
	printf("%4s%13s%10s%12s%10s%12s%12s%12s%14s%14s\n",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9]);
	while(current=*link)
	{
		printf("%-4d%13s%10.1f%12.1f%10d%12d%12d%12.1f%14s%14.1f\n",current->num,current->name,current->purchase_price,
		current->selling_price,current->purchae_num,current->selling_num,current->surplus_num,
		current->profit,current->date,current->salves_tall);
		link=&current->next;
	}
	printf("\n"); 
}

void Calculate(COMMODITY *current)//自动计算 
{
	current->salves_tall=current->selling_price*current->selling_num;
	current->surplus_num=current->purchae_num-current->selling_num;
   	current->profit=current->salves_tall-current->purchase_price*current->selling_num;
}

HISTORY *OpenLog()
{
   char history[200];
   FILE *flog;
   HISTORY  *head=NULL,*current,*p;
	if((flog=fopen("log.txt","r+"))==NULL)
	{
		      printf("open the history failed!\n");
			  exit(0); 
	}
	head=(HISTORY *)malloc(sizeof(HISTORY));
	head->next=NULL;
	p=head;
	for(;;)//按正常读入顺序建立链表 
	{ 
		fgets(history,200,flog);
		if(feof(flog)!=0)
		    break;
		if(strcmp(history,"\n")==0)
		    break;
		current=(HISTORY *)malloc(sizeof(HISTORY));
		strcpy(current->operate_time,history);
		p->next=current;
		p=current;
		
	}
	p->next=NULL;
	fclose(flog);
	return head; 
}


void  WriteHistory(HISTORY *head,const char *operate)
{
	time_t t;
    struct tm *p;
    HISTORY *current; 
    int i;
    t=time(NULL);
    p=localtime(&t);
    current=(HISTORY *)malloc(sizeof(HISTORY));//按后进先出的顺序打造链表 
    strcpy(current->operate_time,asctime(p));
    for(i=0;current->operate_time[i]!='\n';i++)
                   ;
    current->operate_time[i]='\0';
    strcat(current->operate_time,operate);
    current->next=head->next;
    head->next=current;
} 

void EXIT(COMMODITY *head,HISTORY *headhistory) 
{
	COMMODITY *current=head;
	FILE *fp,*flog;
	fp=fopen("小超市商品管理.txt","w+");
	flog=fopen("log.txt","w+");
	fprintf(fp,"%4s%13s%10s%12s%10s%12s%12s%12s%14s%14s\n",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9]);
	while(current)//退出时更新文件 
	{
		fprintf(fp,"%-4d%13s%10.1f%12.1f%10d%12d%12d%12.1f%14s%14.1f\n",current->num,current->name,current->purchase_price,
		current->selling_price,current->purchae_num,current->selling_num,current->surplus_num,
		current->profit,current->date,current->salves_tall);
		current=current->next;
	}
	headhistory=headhistory->next;
	while(headhistory)
	{
		fprintf(flog,"%s",headhistory->operate_time);
		headhistory=headhistory->next;
	}
	
}






typedef struct Commodity
{
	int num;
	char name[15];//名称 
	float purchase_price;//进价 
	float selling_price;//售价 
	int purchae_num;//库存数量 
	int selling_num;//销售数量 
	int surplus_num;//剩余量 
	float profit;//毛利
	char date[12];//过期时间 
	float salves_tall;//销售额 
	struct Commodity *next;
}COMMODITY;


typedef struct history
{ 
	char operate_time[80];
	struct history *next;
}HISTORY;




 

void JudgeChoose(char *choose);//选择菜单算法 

COMMODITY *MakeList(FILE *fp);//创建链表 

void UserInformation();//用户信息管理

void DrawUser();//选择用户界面 

void UserLogin();//用户登录 

void UserRegister();//用户注册 

void CommodityInformation();//商品信息管理

void DrawCommodity();//选择功能界面  

void AddCommodity(COMMODITY *head);//添加商品 

void ChangeCommodity(COMMODITY *head);//修改商品信息 

void InquiryInformation(COMMODITY *head,HISTORY *headhistory);//查询信息 

COMMODITY * InquiryName(COMMODITY *head);//按名称查询

void InquirySurplus(COMMODITY *head);//按剩余数查询 

void BuyCommodity(COMMODITY *head) ;//进货 

void DeleteCommodity(COMMODITY *head);//删除商品 

int comp(const void *a,const void *b);//决定编号排序顺序 

int compprofit(const void *a,const void *b);//决定利润排序顺序 

int compsell(const void *a,const void *b);//决定销售额排序顺序 

int compdate(const void *a,const void *b);//决定过期时间排序顺序 

COMMODITY *sort(COMMODITY *head,int (*comp)(const void *a,const void *b));//排序 

void print(COMMODITY *head);//输出 

void LookUpCommodity(COMMODITY *head,HISTORY *headhistory);//查看商品 

void Calculate(COMMODITY *current);//自动计算 

HISTORY *OpenLog();//打开日志

void  WriteHistory(HISTORY *head,const char *operate);//写入日志

void LookHistory(HISTORY *head);//查看日志

void EXIT(COMMODITY *head,HISTORY *headhistory);//退出更新文件 

#ifndef SFL_H
#define	SFL_H

#define	MAXINSTUCTION	48		//虚拟机支持的最大机器指令数
#define	MAXSYMLEN		32		//符号地址所允许的最大长度
#define	SYMTABLEN		50		//汇编器符号表的最大容量
#define	BAD_CODE		0xff	//错误指令
#define	MAXTOKEN		30		//Token的最大长度
#define	MAXDATALEN		50		//data的最大长度
struct ADDRNode{
	unsigned int addr;
	ADDRNode *next;
};
//符号表结构
struct symboltab {
	char symname[MAXSYMLEN];			//符号地址定义名
	unsigned char type;					//符号类型
	unsigned int symaddr;				//符号地址定义名所代表的内存地址
	unsigned int datalen;				//data类型字符串声明的长度
	unsigned int referredtime;			//符号被引用的次数
	ADDRNode *referencelist;			//符号被引用位置列表
	ADDRNode *lastreferredlocation;		//引用链表尾指针
};
void initTable();						//初始化符号表

#endif // !SFL_H
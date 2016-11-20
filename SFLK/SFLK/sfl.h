#ifndef SFL_H
#define	SFL_H

#define	MAXINSTUCTION	48		//�����֧�ֵ�������ָ����
#define	MAXSYMLEN		32		//���ŵ�ַ���������󳤶�
#define	SYMTABLEN		50		//��������ű���������
#define	BAD_CODE		0xff	//����ָ��
#define	MAXTOKEN		30		//Token����󳤶�
#define	MAXDATALEN		50		//data����󳤶�
struct ADDRNode{
	unsigned int addr;
	ADDRNode *next;
};
//���ű�ṹ
struct symboltab {
	char symname[MAXSYMLEN];			//���ŵ�ַ������
	unsigned char type;					//��������
	unsigned int symaddr;				//���ŵ�ַ��������������ڴ��ַ
	unsigned int datalen;				//data�����ַ��������ĳ���
	unsigned int referredtime;			//���ű����õĴ���
	ADDRNode *referencelist;			//���ű�����λ���б�
	ADDRNode *lastreferredlocation;		//��������βָ��
};
void initTable();						//��ʼ�����ű�

#endif // !SFL_H
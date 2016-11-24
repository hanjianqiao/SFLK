#ifndef SFL_H
#define	SFL_H

#define	MAXINSTUCTION	48
#define	MAXSYMLEN		32
#define	SYMTABLEN		50
#define	BAD_CODE		0xff
#define	MAXTOKEN		30
#define	MAXDATALEN		50
struct ADDRNode{
	unsigned int addr;
	ADDRNode *next;
};
struct symboltab {
	char symname[MAXSYMLEN];
	unsigned char type;
	unsigned int symaddr;
	unsigned int datalen;
	unsigned int referredtime;
	ADDRNode *referencelist;
	ADDRNode *lastreferredlocation;	
};
void initTable();

#endif // !SFL_H
#include"sflk.h"
#include<iostream>
#include<cstring>
using namespace std;
symboltab table1[SYMTABLEN];
symboltab table2[SYMTABLEN];

void initTable(){
	for (int i = 0; i < SYMTABLEN; i++){
		table1[i].datalen = 0;
		table1[i].lastreferredlocation = nullptr;
		table1[i].referencelist = nullptr;
		table1[i].referredtime = 0;
		table1[i].symaddr = 0;
		table1[i].type = 0;
		table2[i].datalen = 0;
		table2[i].lastreferredlocation = nullptr;
		table2[i].referencelist = nullptr;
		table2[i].referredtime = 0;
		table2[i].symaddr = 0;
		table2[i].type = 0;
	}
}

void load(unsigned char *&data, unsigned char *&program, unsigned int &dsize, unsigned int &psize, unsigned int &gsize, unsigned int &esize, unsigned int &ssize, FILE *&file, symboltab table[]){

	ADDRNode *adnode;
	char ch;
	int temp_i;
	int symcnt;
	unsigned char header[32];

	fread(header, 1, 32, file);
	dsize = 0x00000000 | header[0] | header[1] << 8 | header[2] << 16 | header[3] << 24;
	psize = 0x00000000 | header[4] | header[5] << 8 | header[6] << 16 | header[7] << 24;
	gsize = 0x00000000 | header[8] | header[9] << 8 | header[10] << 16 | header[11] << 24;
	esize = 0x00000000 | header[12] | header[13] << 8 | header[14] << 16 | header[15] << 24;
	ssize = 0x00000000 | header[16] | header[17] << 8 | header[18] << 16 | header[19] << 24;
	data = new unsigned char[dsize * 4];
	program = new unsigned char[psize];
	fread(data, 4, dsize, file);
	fread(program, 1, psize, file);

	symcnt = 0;
	while (gsize > 0){
		temp_i = 0;
		while ((ch = getc(file)) != ' '){
			table[symcnt].symname[temp_i++] = ch;
			gsize--;
		}
		table[symcnt].symname[temp_i] = '\0';
		gsize--;
		for (int i = 0; i < 4; i++){
			ch = getc(file);
			table[symcnt].symaddr |= (ch << (i * 8));
			gsize--;
		}
		table[symcnt].type = 0x40;
		symcnt++;
	}
	while (esize > 0){
		temp_i = 0;
		while ((ch = getc(file)) != ' '){
			table[symcnt].symname[temp_i++] = ch;
			esize--;
		}
		table[symcnt].symname[temp_i] = '\0';
		esize--;
		for (int i = 0; i < 4; i++){
			ch = getc(file);
			table[symcnt].referredtime |= (ch << (i * 8));
			esize--;
		}
		for (unsigned int j = 0; j < table[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file);
				adnode->addr |= (ch << (i * 8));
				esize--;
			}
			if (table[symcnt].referencelist == nullptr){
				table[symcnt].referencelist = adnode;
				table[symcnt].lastreferredlocation = table[symcnt].referencelist;
			}
			else{
				table[symcnt].lastreferredlocation->next = adnode;
				table[symcnt].lastreferredlocation = table[symcnt].lastreferredlocation->next;
			}
		}
		table[symcnt].type = 0x02;
		symcnt++;
	}
	while (ssize > 0){
		temp_i = 0;
		while ((ch = getc(file)) != ' '){
			table[symcnt].symname[temp_i++] = ch;
			ssize--;
		}
		table[symcnt].symname[temp_i] = '\0';
		ssize--;
		for (int i = 0; i < 4; i++){
			ch = getc(file);
			table[symcnt].symaddr |= (ch << (i * 8));
			ssize--;
		}
		for (int i = 0; i < 4; i++){
			ch = getc(file);
			table[symcnt].referredtime |= (ch << (i * 8));
			ssize--;
		}
		for (unsigned int j = 0; j < table[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file);
				adnode->addr |= (ch << (i * 8));
				ssize--;
			}
			if (table[symcnt].referencelist == nullptr){
				table[symcnt].referencelist = adnode;
				table[symcnt].lastreferredlocation = table[symcnt].referencelist;
			}
			else{
				table[symcnt].lastreferredlocation->next = adnode;
				table[symcnt].lastreferredlocation = table[symcnt].lastreferredlocation->next;
			}
		}
		table[symcnt].type = 0x01;
		symcnt++;
	}
}


int main(int argc, char* argv[]){
	FILE *file1;
	FILE *file2;
	ADDRNode *adnode;
	char ch;
	int temp_i;
	int symcnt;
	unsigned char *data1, *program1, *data2, *program2;
	unsigned int dsize1 = 0, psize1 = 0, gsize1 = 0, esize1 = 0, ssize1 = 0;
	unsigned int dsize2 = 0, psize2 = 0, gsize2 = 0, esize2 = 0, ssize2 = 0;
	unsigned char header[32];
	if (argc < 3){
		cout << "Too few parameters.\n";
		return -1;
	}

	initTable();



	file1 = fopen(argv[1], "r");
	file2 = fopen(argv[2], "r");

//	load(data1, program1, dsize1, psize1, gsize1, esize1, ssize1, file1, table1);
//	load(data2, program2, dsize2, psize2, gsize2, esize2, ssize2, file2, table2);
	
	fread(header, 1, 32, file1);
	dsize1 = 0x00000000 | header[0] | header[1] << 8 | header[2] << 16 | header[3] << 24;
	psize1 = 0x00000000 | header[4] | header[5] << 8 | header[6] << 16 | header[7] << 24;
	gsize1 = 0x00000000 | header[8] | header[9] << 8 | header[10] << 16 | header[11] << 24;
	esize1 = 0x00000000 | header[12] | header[13] << 8 | header[14] << 16 | header[15] << 24;
	ssize1 = 0x00000000 | header[16] | header[17] << 8 | header[18] << 16 | header[19] << 24;
	data1 = new unsigned char[dsize1 * 4];
	program1 = new unsigned char[psize1];
	fread(data1, 4, dsize1, file1);
	fread(program1, 1, psize1, file1);

	symcnt = 0;
	while (gsize1 > 0){
		temp_i = 0;
		while ((ch = getc(file1)) != ' '){
			table1[symcnt].symname[temp_i++] = ch;
			gsize1--;
		}
		table1[symcnt].symname[temp_i] = '\0';
		gsize1--;
		for (int i = 0; i < 4; i++){
			ch = getc(file1);
			table1[symcnt].symaddr |= (ch << (i * 8));
			gsize1--;
		}
		table1[symcnt].type = 0x40;
		symcnt++;
	}
	while (esize1 > 0){
		temp_i = 0;
		while ((ch = getc(file1)) != ' '){
			table1[symcnt].symname[temp_i++] = ch;
			esize1--;
		}
		table1[symcnt].symname[temp_i] = '\0';
		esize1--;
		for (int i = 0; i < 4; i++){
			ch = getc(file1);
			table1[symcnt].referredtime |= (ch << (i * 8));
			esize1--;
		}
		for (unsigned int j = 0; j < table1[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file1);
				adnode->addr |= (ch << (i * 8));
				esize1--;
			}
			if (table1[symcnt].referencelist == nullptr){
				table1[symcnt].referencelist = adnode;
				table1[symcnt].lastreferredlocation = table1[symcnt].referencelist;
			}
			else{
				table1[symcnt].lastreferredlocation->next = adnode;
				table1[symcnt].lastreferredlocation = table1[symcnt].lastreferredlocation->next;
			}
		}
		table1[symcnt].type = 0x02;
		symcnt++;
	}
	while (ssize1 > 0){
		temp_i = 0;
		while ((ch = getc(file1)) != ' '){
			table1[symcnt].symname[temp_i++] = ch;
			ssize1--;
		}
		table1[symcnt].symname[temp_i] = '\0';
		ssize1--;
		for (int i = 0; i < 4; i++){
			ch = getc(file1);
			table1[symcnt].symaddr |= (ch << (i * 8));
			ssize1--;
		}
		for (int i = 0; i < 4; i++){
			ch = getc(file1);
			table1[symcnt].referredtime |= (ch << (i * 8));
			ssize1--;
		}
		for (unsigned int j = 0; j < table1[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file1);
				adnode->addr |= (ch << (i * 8));
				ssize1--;
			}
			if (table1[symcnt].referencelist == nullptr){
				table1[symcnt].referencelist = adnode;
				table1[symcnt].lastreferredlocation = table1[symcnt].referencelist;
			}
			else{
				table1[symcnt].lastreferredlocation->next = adnode;
				table1[symcnt].lastreferredlocation = table1[symcnt].lastreferredlocation->next;
			}
		}
		table1[symcnt].type = 0x01;
		symcnt++;
	}


	fread(header, 1, 32, file2);
	dsize2 = 0x00000000 | header[0] | header[1] << 8 | header[2] << 16 | header[3] << 24;
	psize2 = 0x00000000 | header[4] | header[5] << 8 | header[6] << 16 | header[7] << 24;
	gsize2 = 0x00000000 | header[8] | header[9] << 8 | header[10] << 16 | header[11] << 24;
	esize2 = 0x00000000 | header[12] | header[13] << 8 | header[14] << 16 | header[15] << 24;
	ssize2 = 0x00000000 | header[16] | header[17] << 8 | header[18] << 16 | header[19] << 24;
	data2 = new unsigned char[dsize2 * 4];
	program2 = new unsigned char[psize2];
	fread(data2, 4, dsize2, file2);
	fread(program2, 1, psize2, file2);
	symcnt = 0;
	while (gsize2 > 0){
		temp_i = 0;
		while ((ch = getc(file2)) != ' '){
			table2[symcnt].symname[temp_i++] = ch;
			gsize2--;
		}
		table2[symcnt].symname[temp_i] = '\0';
		gsize2--;
		for (int i = 0; i < 4; i++){
			ch = getc(file2);
			table2[symcnt].symaddr |= (ch << (i * 8));
			gsize2--;
		}
		table2[symcnt].type = 0x04;
		symcnt++;
	}
	while (esize2 > 0){
		temp_i = 0;
		while ((ch = getc(file2)) != ' '){
			table2[symcnt].symname[temp_i++] = ch;
			esize2--;
		}
		table2[symcnt].symname[temp_i] = '\0';
		esize2--;
		for (int i = 0; i < 4; i++){
			ch = getc(file2);
			table2[symcnt].referredtime |= (ch << (i * 8));
			esize2--;
		}
		for (unsigned int j = 0; j < table2[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file2);
				adnode->addr |= (ch << (i * 8));
				esize2--;
			}
			if (table2[symcnt].referencelist == nullptr){
				table2[symcnt].referencelist = adnode;
				table2[symcnt].lastreferredlocation = table2[symcnt].referencelist;
			}
			else{
				table2[symcnt].lastreferredlocation->next = adnode;
				table2[symcnt].lastreferredlocation = table2[symcnt].lastreferredlocation->next;
			}
		}
		table2[symcnt].type = 0x02;
		symcnt++;
	}
	while (ssize2 > 0){
		temp_i = 0;
		while ((ch = getc(file2)) != ' '){
			table2[symcnt].symname[temp_i++] = ch;
			ssize2--;
		}
		table2[symcnt].symname[temp_i] = '\0';
		ssize2--;
		for (int i = 0; i < 4; i++){
			ch = getc(file2);
			table2[symcnt].symaddr |= (ch << (i * 8));
			ssize2--;
		}
		for (int i = 0; i < 4; i++){
			ch = getc(file2);
			table2[symcnt].referredtime |= (ch << (i * 8));
			ssize2--;
		}
		for (unsigned int j = 0; j < table2[symcnt].referredtime; j++){
			adnode = new ADDRNode();
			adnode->next = nullptr;
			for (int i = 0; i < 4; i++){
				ch = getc(file2);
				adnode->addr |= (ch << (i * 8));
				ssize2--;
			}
			if (table2[symcnt].referencelist == nullptr){
				table2[symcnt].referencelist = adnode;
				table2[symcnt].lastreferredlocation = table2[symcnt].referencelist;
			}
			else{
				table2[symcnt].lastreferredlocation->next = adnode;
				table2[symcnt].lastreferredlocation = table2[symcnt].lastreferredlocation->next;
			}
		}
		table2[symcnt].type = 0x01;
		symcnt++;
	}
	
	fclose(file1);
	fclose(file2);


	cout << "Global:\n";
	for (int i = 0; i < SYMTABLEN; i++){
		if (table1[i].type & 0x04){
			printf("Name: %s\n", table1[i].symname);
		}
		if (table2[i].type & 0x04){
			printf("Name: %s\n", table2[i].symname);
		}
	}
	cout << "Extern:\n";
	for (int i = 0; i < SYMTABLEN; i++){
		if (table1[i].type & 0x02){
			printf("Name: %s\n", table1[i].symname);
		}
		if (table2[i].type & 0x02){
			printf("Name: %s\n", table2[i].symname);
		}
	}
	cout << "Self:\n";
	for (int i = 0; i < SYMTABLEN; i++){
		if (table1[i].type & 0x01){
			printf("Name: %s\n", table1[i].symname);
		}
		if (table2[i].type & 0x01){
			printf("Name: %s\n", table2[i].symname);
		}
	}


	for (int i = 0; i < SYMTABLEN; i++){
		if (table2[i].type == 0x04){
			table2[i].symaddr += psize1;
		}
		else if (table2[i].type == 0x02){
		}
		else if (table2[i].type == 0x01){
			ADDRNode *ptr = table2[i].referencelist;
			for (unsigned int j = 0; j < table2[i].referredtime; j++){
				if (program2[ptr->addr - 1] >= 0xa1 && program2[ptr->addr - 1] <= 0xa5){
				}
				else{
					int k0 = 0, k1 = 0;
					for (int k = 0; k < 4; k++){
						k0 |= (program2[ptr->addr + k] << (k * 8));
					}
					k0 += psize1;
					for (int k = 0; k < 4; k++){
						program2[ptr->addr + k] = (k0 >> (k * 8)) & 0xff;
					}
				}
				ptr = ptr->next;
			}
		}
	}

	
	for (int i = 0; i < SYMTABLEN; i++){
		if (table1[i].type == 0x04){
		}
		else if (table1[i].type == 0x02){
			for (int j = 0; j < SYMTABLEN; j++){
				if (table2[j].type == 0x04 && strcmp(table1[i].symname, table2[j].symname) == 0){
					ADDRNode *ptr = table1[i].referencelist;
					while (ptr != nullptr){
						for (int k = 0; k < 4; k++){
							program1[ptr->addr + k] = (table2[j].symaddr >> (k * 8)) & 0xff;
						}
						ptr = ptr->next;
					}
				}
			}
		}
		else if (table1[i].type == 0x01){
		}
	}

	FILE *file;
	file = fopen("linkedout.bin", "wb");
	for (int i = 0; i < 4; i++){
		header[i] = (dsize1 + dsize2) >> (i * 8);
	}
	fwrite(header, 1, 4, file);
	for (int i = 0; i < 4; i++){
		header[i] = (psize1 + psize2) >> (i * 8);
	}
	fwrite(header, 1, 4, file);

	fwrite(header + 8, 1, 32 - 8, file);

	fwrite(data1, 1, dsize1 * 4, file);
	fwrite(data2, 1, dsize2 * 4, file);
	fwrite(program1, 1, psize1, file);
	fwrite(program2, 1, psize2, file);
	
	fclose(file);
	return 0;
}
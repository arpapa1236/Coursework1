#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#define RECORDS 10

typedef struct
{
	/*���*/
	char* name;
	/*����*/
	int glasses;
} Record;

Record* RecordLoad();

void RecordSave(Record*);

void RecordDestroy(Record*);
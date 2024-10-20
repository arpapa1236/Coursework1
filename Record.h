#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#define RECORD_N 10
#define RECORD_FILE_NAME "records"

typedef struct
{
	/*Имя*/
	char name[10];
	/*Очки*/
	int glasses;
} Record;

void RecordsPrint(Record** rec);

Record* RecordCreate(const char* name, int glasses);

Record** RecordsLoad();

void RecordsSave(Record**);

void RecordsDestroy(Record**);

void NewRecord(Record** recs, Record* newrec);
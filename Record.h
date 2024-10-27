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
//создание структуры рекорда
Record* RecordCreate(const char* name, int glasses);
//Загрузка рекордов из файла
Record** RecordsLoad();
//Сохранение рекордов
void RecordsSave(Record**);
//Отчистка массива рекордов
void RecordsDestroy(Record**);
//Проверка и вставка если нужно нового рекорда
void NewRecord(Record** recs, Record* newrec);

bool IfRecord(Record** recs, int time);
#include "Record.h"

void RecordsPrint(Record** rec)
{
	for (int i = 0; i < RECORD_N; i++)
		printf("%-10s:%d\n", rec[i]->name, rec[i]->glasses);
}

Record* RecordCreate(const char* name, int glasses)
{
	Record* rez = (Record*)malloc(sizeof(Record));
	strcpy(rez->name, name);
	rez->glasses = glasses;
	return rez;
}

Record** RecordsLoad()
{
	Record** rez = (Record**)calloc(sizeof(Record*), RECORD_N);
	for (int i = 0; i < RECORD_N; i++)
		rez[i] = (Record*)calloc(sizeof(Record), 1);

	FILE* file = fopen(RECORD_FILE_NAME, "rt");

	if (file)
	{
		char* p;
		for (int i = 0; i < RECORD_N; i++) fscanf(file, "%s%d", rez[i]->name, &(rez[i]->glasses));
		fclose(file);
	}
	else
	{
		for (int i = 0; i < RECORD_N; i++)
		{
			strcpy(rez[i]->name, "-");
			rez[i]->glasses = INT_MAX;
		}
	}
	return rez;
}

void RecordsSave(Record** rec)
{
	FILE* file = fopen(RECORD_FILE_NAME, "wt");
	if (file) for (int i = 0; i < RECORD_N; i++) fprintf(file, "%s\n%d\n", rec[i]->name, rec[i]->glasses);
	else exit(-1);
	fclose(file);
}

void RecordsDestroy(Record** recs)
{
	for (int i = 0; i < RECORD_N; i++)
		free(recs[i]);
	free(recs);
}

void NewRecord(Record** recs, Record* newrec)
{
	Record* ptr = newrec, * buff;
	for (int i = 0; i < RECORD_N; i++)
	{
		if (recs[i]->glasses > ptr->glasses)
		{
			buff = recs[i];
			recs[i] = ptr;
			ptr = buff;
		}
	}
	if (ptr != newrec)
		free(ptr);
}

bool IfRecord(Record** recs, int time)
{
	for (int i = 0; i < RECORD_N; i++)
		if (recs[i]->glasses > time) return true;
	return false;
}
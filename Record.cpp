#include "Record.h"

Record* RecordLoad()
{
	FILE* file = fopen("Records", "rt");
	Record* rez = (Record*)calloc(RECORDS, sizeof(Record));
	for (int i = 0; i < RECORDS; i++)
		rez->name = (char*)calloc(50, sizeof(char));
	if (file == NULL)
	{
		file = fopen("Records", "wt");
		if (file == NULL) exit(0b0110);
		for (int i = 0; i < RECORDS; i++) fprintf(file, "-\n");
	}
	else
		for (int i = 0; i < RECORDS; i++) fscanf(file, "%s:%d", rez[i].name, &rez[0].glasses);
	fclose(file);
	return rez;
}

void RecordSave(Record* rec)
{
	FILE* file = fopen("Records", "wt");
	if (file) for (int i = 0; i < RECORDS; i++) fprintf(file, "%s.%d", rec[i].name, rec[i].glasses);
	else exit(0b01110);
}

void RecordDestroy(Record* rec)
{
	for (int i = 0; i < RECORDS; i++)
		free(rec->name);
	free(rec);
}
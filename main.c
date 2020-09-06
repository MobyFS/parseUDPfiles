// ���������� �������������� ��� ������ ������������ ������� ��
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ������� ����������� ������� ����� � ������
long int filesize(FILE* fp)
{
	long int savePos, sizeOfFile;

	savePos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	sizeOfFile = ftell(fp);
	fseek(fp, savePos, SEEK_SET);
	return sizeOfFile;
}

// ������� �������
int main(int argc, char* argv[])
{
	FILE* fp;

	char* buf;
	char* sbuf;
	char* str;
	char* foutName;

	long int len = 0;
	long int i = 0;
	long int j = 0;
	long int k = 0;

	int ind = 1;
	int flag = 1;

	if (argc < 2)
	{
		puts("Input the filename as an argument!");
		exit(3);
	}

	// �������� ����� ��� ������
	if ((fp = fopen(argv[1], "rb+")) == NULL)
	{
		puts("Cannot open file!");
		exit(1);
	}

	// ������ ����� ����� � ������
	len = filesize(fp);

	// ���� ����� ������ 16 ����, �� ��� ������������ ����
	if (len < 16)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}

	// ��������� ������ ��� �����
	buf = (char*)malloc(len);
	sbuf = (char*)malloc(len);
	str = (char*)malloc(10);
	foutName = (char*)malloc(strlen(argv[1]) + 10);

	// ���������� ����� � �����
	fread(buf, len, 1, fp);

	// �������� �����
	fclose(fp);

	// �������� �� ��������� ������
	for (i = 0; i < len - 12; i++)
		if (buf[i] == (char)0x26 && (buf[i + 1] == (char)0xcc || buf[i + 1] == (char)0xcd) && buf[i + 2] == (char)0x1a &&
			buf[i + 3] == (char)0x07 && buf[i + 4] == (char)0x32 && buf[i + 5] == (char)0x05 &&
			buf[i + 6] == 't' && buf[i + 7] == 'a' && buf[i + 8] == 'n' &&
			buf[i + 9] == 'g' && buf[i + 10] == 'o' && buf[i + 11] == ':')
		{
			flag = 0;
			break;
		}

	// �� ���� ���������� ��������� �� ��� ������ -> ��� ������������ ����
	if (flag)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}

	flag = 1;

	// �������� �� ���������� ���������� ����� ������
	for (i = 10; i < len - 1; i++)
		if (buf[i] == (char)0x08 && buf[i + 1] == (char)0x01)
		{
			flag = 0;
			break;
		}

	// �� ���� ���������� ���������� ����� ������ �� ���� ������� -> ��� ������������ ����
	if (flag)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}
	
	// �������� �������� �� ������������ ����� �������� -> ������ ���������
	i = 0;
	while (i < len - 12)
		if (buf[i] == (char)0x26 && (buf[i + 1] == (char)0xcc || buf[i + 1] == (char)0xcd) && buf[i + 2] == (char)0x1a &&
			buf[i + 3] == (char)0x07 && buf[i + 4] == (char)0x32 && buf[i + 5] == (char)0x05 &&
			buf[i + 6] == 't' && buf[i + 7] == 'a' && buf[i + 8] == 'n' &&
			buf[i + 9] == 'g' && buf[i + 10] == 'o' && buf[i + 11] == ':')
		{
			// ���������� �������� ����� ������; c������� �� ��������� + 2 ����� �� ��� ������
			for (j = i + 14, k = 0; j < len - 1; j++, k++)
			{
				if (buf[j] == (char)0x08 && buf[j + 1] == (char)0x01)
					break;

				sbuf[k] = buf[j];
			}

			// ���������� ����� ������
			sbuf[k] = '\0';

			// �������������� �������� ������ ����� ������ � ������ ��� ����� �����	
			_itoa(ind, str, 10);

			// ������������ ����� ����� ��� �������� ����� ������
			strcpy(foutName, argv[1]);
			strcat(foutName, " + ");
			strcat(foutName, str);

			// ���������� ������� ��� ������������ ����
			if ((fp = fopen(foutName, "wb+")) == NULL)
			{
				puts("Cannot open file!");
				exit(1);
			}

			// ������ �������� ����� ������ � ����
			fwrite(sbuf, k, 1, fp);

			// �������� �����
			fclose(fp);

			// ����� ����� �����, ���� ���� ������������ ������	
			puts(foutName);

			// ��������� ��������������� ����� ������
			ind++;

			// ����� ���������� �����
			i = j + 2;
		}
		else
		{ 
			i++;
		}

	// ������� ������
	free(buf);
	free(sbuf);
	free(str);
	free(foutName);
	return 0;
}

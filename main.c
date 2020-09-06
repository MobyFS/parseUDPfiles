// Отключение предупреждений для старых небезопасных функций Си
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Функция определения размера файла в байтах
long int filesize(FILE* fp)
{
	long int savePos, sizeOfFile;

	savePos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	sizeOfFile = ftell(fp);
	fseek(fp, savePos, SEEK_SET);
	return sizeOfFile;
}

// Главная функция
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

	// Открытие файла для чтения
	if ((fp = fopen(argv[1], "rb+")) == NULL)
	{
		puts("Cannot open file!");
		exit(1);
	}

	// Расчёт длины файла в байтах
	len = filesize(fp);

	// Если длина меньше 16 байт, то это некорректный файл
	if (len < 16)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}

	// Выделение памяти для строк
	buf = (char*)malloc(len);
	sbuf = (char*)malloc(len);
	str = (char*)malloc(10);
	foutName = (char*)malloc(strlen(argv[1]) + 10);

	// Считывание файла в буфер
	fread(buf, len, 1, fp);

	// Закрытие файла
	fclose(fp);

	// Проверка на заголовок данных
	for (i = 0; i < len - 12; i++)
		if (buf[i] == (char)0x26 && (buf[i + 1] == (char)0xcc || buf[i + 1] == (char)0xcd) && buf[i + 2] == (char)0x1a &&
			buf[i + 3] == (char)0x07 && buf[i + 4] == (char)0x32 && buf[i + 5] == (char)0x05 &&
			buf[i + 6] == 't' && buf[i + 7] == 'a' && buf[i + 8] == 'n' &&
			buf[i + 9] == 'g' && buf[i + 10] == 'o' && buf[i + 11] == ':')
		{
			flag = 0;
			break;
		}

	// Ни один корректный заголовок не был найден -> это некорректный файл
	if (flag)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}

	flag = 1;

	// Проверка на корректное завершение блока данных
	for (i = 10; i < len - 1; i++)
		if (buf[i] == (char)0x08 && buf[i + 1] == (char)0x01)
		{
			flag = 0;
			break;
		}

	// Ни одно корректное завершение блока данных не было найдено -> это некорректный файл
	if (flag)
	{
		puts("There is wrong format of the file!");
		exit(2);
	}
	
	// Основные проверки на корректность файла пройдены -> начало обработки
	i = 0;
	while (i < len - 12)
		if (buf[i] == (char)0x26 && (buf[i + 1] == (char)0xcc || buf[i + 1] == (char)0xcd) && buf[i + 2] == (char)0x1a &&
			buf[i + 3] == (char)0x07 && buf[i + 4] == (char)0x32 && buf[i + 5] == (char)0x05 &&
			buf[i + 6] == 't' && buf[i + 7] == 'a' && buf[i + 8] == 'n' &&
			buf[i + 9] == 'g' && buf[i + 10] == 'o' && buf[i + 11] == ':')
		{
			// Считывание текущего блока данных; cмещение от заголовка + 2 байта на тип данных
			for (j = i + 14, k = 0; j < len - 1; j++, k++)
			{
				if (buf[j] == (char)0x08 && buf[j + 1] == (char)0x01)
					break;

				sbuf[k] = buf[j];
			}

			// Завершение блока данных
			sbuf[k] = '\0';

			// Преобразование текущего номера блока данных в строку для имени файла	
			_itoa(ind, str, 10);

			// Формирование имени файла для текущего блока данных
			strcpy(foutName, argv[1]);
			strcat(foutName, " + ");
			strcat(foutName, str);

			// Невозможно создать или перезаписать файл
			if ((fp = fopen(foutName, "wb+")) == NULL)
			{
				puts("Cannot open file!");
				exit(1);
			}

			// Запись текущего блока данных в файл
			fwrite(sbuf, k, 1, fp);

			// Закрытие файла
			fclose(fp);

			// Вывод имени файла, куда была осуществлена запись	
			puts(foutName);

			// Инкремент индентификатора блока данных
			ind++;

			// Поиск следующего блока
			i = j + 2;
		}
		else
		{ 
			i++;
		}

	// Очистка памяти
	free(buf);
	free(sbuf);
	free(str);
	free(foutName);
	return 0;
}

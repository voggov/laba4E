// mailClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;

HANDLE hMailSlot;
char szBuf[512];
DWORD cbWritten;
LPCWSTR lpszMailSlotName = L"\\\\.\\mailslot\\mailslot";

int main()
{
	hMailSlot = CreateFile(lpszMailSlotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile Error!\n");
		_getch();
		return 0;
	}
	while (1) {
		//scanf_s("%s %s", &szBuf);
		printf("Enter time to set ringer: ");
		gets_s(szBuf);
		//strcpy_s(szBuf, "Test message from console");
		if (!WriteFile(hMailSlot, szBuf, strlen(szBuf) + 1, &cbWritten, NULL))
			printf("Error Data Transfer!\n");
		else
			printf("Transferred %d bytes: <%s>\n", cbWritten, szBuf);
	}
	CloseHandle(hMailSlot);
	_getch();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

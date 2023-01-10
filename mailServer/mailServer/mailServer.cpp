// mailServer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <ctime>

BOOL fReturnCode;
DWORD cbMessages;
DWORD cbMsgNumber;
HANDLE hMailslot;
HANDLE ringers[5];
LPCWSTR lpszMailSlotName = L"\\\\.\\mailslot\\mailslot";
char szBuf[512];
DWORD cbRead;
int initialCount = 0;
int lock = 0;

LARGE_INTEGER ringer(int h, int m);

VOID CALLBACK TimerAPCProc(
	LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue
	);

int main()
{
	hMailslot = CreateMailslot(lpszMailSlotName, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (hMailslot == INVALID_HANDLE_VALUE)
	{
		printf("Error MailSlot Creating!\n");
		_getch();
		return 0;
	}
	printf("This is server! VT ver:2.0.15\n");
	while (1)
	{
		fReturnCode = GetMailslotInfo(hMailslot, NULL, &cbMessages, &cbMsgNumber, NULL);
		if (!fReturnCode)
		{
			printf("Get MailSlotInfo Error!\n");
			_getch();
			return 0;
		}

		if (cbMsgNumber != 0)
		{
			if (ReadFile(hMailslot, szBuf, 512, &cbRead, NULL))
			{
				printf("Received %d bytes: <%s>\n", cbRead, szBuf);
#pragma region setTimer
				int hour = 0, minute = 0;
				LARGE_INTEGER lnTime;
				sscanf_s(szBuf, "%d %d", &hour, &minute);

				lnTime = ringer(hour, minute);
				ringers[initialCount] = CreateWaitableTimer(NULL, TRUE, L"SPO_Timer");
				if (!SetWaitableTimer(ringers[initialCount], &lnTime, 0, TimerAPCProc, NULL, FALSE))
				{
					printf("Error at set timer\n");
					return 1;
				}
				initialCount++;
#pragma endregion 
			}
			else
				printf("Error Data Transfer!\n");
		}
		if (initialCount != lock) {
			while (1)
			{
				static int nCount = 0;
				// Set thread to alertable state
				SleepEx(INFINITE, TRUE);
				break;
			}
			lock++;
		}
	}
	CloseHandle(hMailslot);
	return 0;
}

VOID CALLBACK TimerAPCProc(
	LPVOID lpArgToCompletionRoutine,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue
	)
{
	MessageBeep(10);
	printf("good morning!");
	return;
}

LARGE_INTEGER ringer(int h, int m) {
	struct tm *u = new tm;
	const time_t timer = time(NULL);
	localtime_s(u,&timer);
	const int nTimerUnitsPerSecond = 10000000;
	LARGE_INTEGER ring;
	long time;

		time = (h - u->tm_hour ) * 60 * 60
			+ (m- u->tm_min ) * 60;
	ring.QuadPart = -(time * nTimerUnitsPerSecond);

	return ring;
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

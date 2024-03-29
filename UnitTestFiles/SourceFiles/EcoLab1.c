/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdio.h>
#include <stdlib.h>
#include "IEcoCalculatorY.h"
#include "IEcoCalculatorX.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/* Компаратор для интов */
int32_t __cdecl compInts(const void *a_ptr, const void *b_ptr) {
    const int32_t a = *(int32_t *)a_ptr;
    const int32_t b = *(int32_t *)b_ptr;
    return (a > b) - (a < b);
}

/* Компаратор для символов */
int32_t __cdecl compChars(const void *a_ptr, const void *b_ptr) {
    const char a = *(char *)a_ptr;
    const char b = *(char *)b_ptr;
    return (a > b) - (a < b);
}

/* Создание массива больших интов */
int32_t* CreateIntArray(IEcoMemoryAllocator1* pIMem, size_t n_elements) {
    int32_t *out = (int32_t*) pIMem->pVTbl->Alloc(pIMem, n_elements * sizeof(int32_t));
	size_t i;
    srand(time(0));
    for (i = 0; i < n_elements; ++i) {
        int range = (rand() << 10);
        out[i] = (rand() << 10) - range / 2;
    }
    return out;
}

/* Создание массива интов для проверки работоспособности сортировки */
int32_t* CreateSimpleIntArray(IEcoMemoryAllocator1* pIMem, size_t n_elements) {
    int32_t *out = (int32_t*) pIMem->pVTbl->Alloc(pIMem, n_elements * sizeof(int32_t));
	size_t i;
	int range = 2001;
    srand(time(0));
    for (i = 0; i < n_elements; ++i) {
        out[i] = (int32_t)(rand()) % range - range / 2;
    }
    return out;
}

/* Создание массива символов */
char* CreateCharArray(IEcoMemoryAllocator1* pIMem, size_t n_elements) {
    char *out = (char*) pIMem->pVTbl->Alloc(pIMem, n_elements * sizeof(char));
	size_t i;
    srand(time(0));
    for (i = 0; i < n_elements; ++i) {
        out[i] = (char)(rand() % 26 + 97);
    }
    return out;
}

/* Фукция очистки массива */
void DeleteArray(IEcoMemoryAllocator1* pIMem, void* array_ptr) {
    pIMem->pVTbl->Free(pIMem, array_ptr);
}

/* Функция печати массива интов */
void PrintIntArray(int32_t* array, size_t n_elems) {
	size_t i;
    int32_t* result = (int32_t*) array;
    for (i = 0; i < n_elems; ++i) {
        printf("%d ", *(result + i));
    }
    printf("\n");
}

/* Функция печати массива символов */
void PrintCharArray(char* array, size_t n_elems) {
    char* result = (char*) array;
	size_t i;
    for (i = 0; i < n_elems; ++i) {
        printf("%c", *(result + i));
    }
    printf("\n");
}

/* Проверка работоспособности сортировки подсчетом целых чисел */
void PrintSimpleCountingSortTestResultsInt(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_elems, int sortingOrder) {
	int32_t* arr;
    printf("SimpleTestCountingSortInt\n");
    arr = CreateSimpleIntArray(pIMem, n_elems);
    printf("Before:\n");
    PrintIntArray(arr, n_elems);
    lab1->pVTbl->CountingSort(lab1, arr, n_elems, sortingOrder);
    printf("After\n");
    PrintIntArray(arr, n_elems);
    DeleteArray(pIMem, arr);
}

/* Проверка работоспособности сортировки подсчетом символов */
void PrintSimpleCountingSortTestResultsChar(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_elems, int sortingOrder) {
	char* arr;
    printf("SimpleTestCountingSortChar\n");
    arr = CreateCharArray(pIMem, n_elems);
    printf("Before:\n");
    PrintCharArray(arr, n_elems);
    lab1->pVTbl->CountingSort(lab1, arr, n_elems, sortingOrder);
    printf("After\n");
    PrintCharArray(arr, n_elems);
    DeleteArray(pIMem, arr);
}

/* Проверка работоспособности быстрой сортировки целых чисел */
void PrintSimpleQuickSortTestResultsInt(IEcoMemoryAllocator1 *pIMem, size_t n_elems) {
	int32_t* arr;
    printf("SimpleTestQuickSortInt\n");
    arr = CreateSimpleIntArray(pIMem, n_elems);
    printf("Before:\n");
    PrintIntArray(arr, n_elems);
    qsort(arr, n_elems, sizeof(int32_t), compInts);
    printf("After\n");
    PrintIntArray(arr, n_elems);
    DeleteArray(pIMem, arr);
}

/* Проверка работоспособности быстрой сортировки символов */
void PrintSimpleQuickSortTestResultsChar(IEcoMemoryAllocator1 *pIMem, size_t n_elems) {
	char* arr;
    printf("SimpleTestQuickSortChar\n");
    arr = CreateCharArray(pIMem, n_elems);
    printf("Before:\n");
    PrintCharArray(arr, n_elems);
    qsort(arr, n_elems, sizeof(char), compChars);
    printf("After\n");
    PrintCharArray(arr, n_elems);
    DeleteArray(pIMem, arr);
}

/* INT32_T */
/* Тестирование сортировки подсчетом для дальнейшего сравнения с быстрой сортировкой */
void PrintHardCountingSortTestResultsInt(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_elems, int sortingOrder, FILE *file) {
	clock_t before, after;
    double countingSortDuration;
    int32_t* arr = CreateIntArray(pIMem, n_elems);
    printf("TestCountingSortInt\n");
    before = clock();
    lab1->pVTbl->CountingSort(lab1, arr, n_elems, sortingOrder);
    after = clock();
    countingSortDuration = (double) (after - before) / CLOCKS_PER_SEC;
    printf("%lf %d\n", countingSortDuration, n_elems);
    fprintf(file, "%s,%s,%d,%lf\n", "counting sort", "int", n_elems, countingSortDuration);
    DeleteArray(pIMem, arr);
}

/* Тестирование быстрой сортировки для дальнейшего сравнения с сортировкой подсчетом */
void PrintHardQuickSortTestResultsInt(IEcoMemoryAllocator1 *pIMem, size_t n_elems, FILE *file) {
	clock_t before, after;
    double quickSortDuration;
    int32_t* arr = CreateIntArray(pIMem, n_elems);
    printf("TestQuickSortInt\n");
    before = clock();
    qsort(arr, n_elems, sizeof(int32_t), compInts);
    after = clock();
    quickSortDuration = (double) (after - before) / CLOCKS_PER_SEC;
    fprintf(file, "%s,%s,%d,%lf\n", "quick sort", "int", n_elems, quickSortDuration);
    printf("%lf %d\n", quickSortDuration, n_elems);
    DeleteArray(pIMem, arr);
}

/* CHAR */
/* Тестирование сортировки подсчетом для дальнейшего сравнения с быстрой сортировкой */
void PrintHardCountingSortTestResultsChar(IEcoMemoryAllocator1 *pIMem, IEcoLab1 *lab1, size_t n_elems, int sortingOrder, FILE *file) {
	clock_t before, after;
    double countingSortDuration;
    char* arr = CreateCharArray(pIMem, n_elems);
    printf("TestCountingSortChar\n");
    before = clock();
    lab1->pVTbl->CountingSort(lab1, arr, n_elems, sortingOrder);
    after = clock();
    countingSortDuration = (double) (after - before) / CLOCKS_PER_SEC;
    printf("%lf %d\n", countingSortDuration, n_elems);
    fprintf(file, "%s,%s,%d,%lf\n", "counting sort", "char", n_elems, countingSortDuration);
    DeleteArray(pIMem, arr);
}

/* Тестирование быстрой сортировки для дальнейшего сравнения с сортировкой подсчетом */
void PrintHardQuickSortTestResultsChar(IEcoMemoryAllocator1 *pIMem, size_t n_elems, FILE *file) {
    clock_t before, after;
    double quickSortDuration;
    char* arr = CreateCharArray(pIMem, n_elems);
	printf("TestQuickSortChar\n");
    before = clock();
    qsort(arr, n_elems, sizeof(char), compChars);
    after = clock();
    quickSortDuration = (double) (after - before) / CLOCKS_PER_SEC;
    fprintf(file, "%s,%s,%d,%lf\n", "quick sort", "char", n_elems, quickSortDuration);
    printf("%lf %d\n", quickSortDuration, n_elems);
    DeleteArray(pIMem, arr);
}
/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатели на тестируемые интерфейсы */
    IEcoLab1* pIEcoLab1 = 0;
    IEcoLab1* pIEcoLab1String = 0;
	IEcoCalculatorX* pIEcoCalculatorX = 0;
    IEcoCalculatorY* pIEcoCalculatorY = 0;

	/* Переменные для проверки операций сторонних компонентов */
	int16_t x = 20;
	int16_t y = 5;
	int16_t foo_res;

	int i;
    /* Файл для записи результатов */
    FILE *results;
    /* Массив с длинами массивов для тестирования сортировок */
    int arraySizes[] = {500000, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000, 15000000};
	int arrayCharSizes[] = {5000000, 10000000, 20000000, 30000000, 40000000, 50000000, 60000000, 70000000, 80000000, 90000000, 100000000, 150000000};
    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1String, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }


	printf("Test interfaces:\n");

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
    if (result != 0) {
		printf("Query: IEcoCalculatorX from EcoLab1 is failed!");
        goto Release;
    }
	printf("Query: IEcoCalculatorX from EcoLab1 is correct!\n");
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
    if (result != 0) {
		printf("Query: IEcoCalculatorY from EcoLab1 is failed!\n");
        goto Release;
    }
	printf("Query: IEcoCalculatorY from EcoLab1 is correct!\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);

	result = pIEcoCalculatorY->pVTbl->QueryInterface(pIEcoCalculatorY, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
	if (result != 0) {
		printf("Query: IEcoCalculatorX from IEcoCalculatorY is failed!\n");
        goto Release;
    }
	printf("Query: IEcoCalculatorX from IEcoCalculatorY is correct!\n");
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	/* запрос интерфейса IEcoCalculatorY через IEcoCalculatorX */
	result = pIEcoCalculatorX->pVTbl->QueryInterface(pIEcoCalculatorX, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
	if (result != 0) {
		printf("Query: IEcoCalculatorY from IEcoCalculatorX is failed!\n");
        goto Release;
    }
	printf("Query: IEcoCalculatorY from IEcoCalculatorX is correct!\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);


	printf("\nTest operations:\n");

	/* запрос интерфейса IEcoCalculatorX через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
    if (result != 0 || pIEcoCalculatorX == 0) {
        goto Release;
    }

	printf("_______________________\n\n");
	printf("Addition test\n");
	foo_res = pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, x, y);
	if (foo_res == x + y) {
		printf("%d + %d == %4d - Equal!\n", x, y, foo_res);
	} else {
		printf("%d + %d == %4d - Wrong!\n", x, y, foo_res);
	}

	printf("_______________________\n\n");
	printf("Subtraction test\n");
	foo_res = pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, x, y);
	if (foo_res == x - y) {
		printf("%d - %d == %4d - Equal!\n", x, y, foo_res);
	} else {
		printf("%d - %d == %4d - Wrong!\n", x, y, foo_res);
	}
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	 /* запрос интерфейса IEcoCalculatorY через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
    if (result != 0 || pIEcoCalculatorY == 0) {
        goto Release;
    }

	printf("_______________________\n\n");
	printf("Multiplication test\n");
	foo_res = pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, x, y);
	if (foo_res == x * y) {
		printf("%d * %d == %4d - Equal!\n", x, y, foo_res);
	} else {
		printf("%d * %d == %4d - Wrong!\n", x, y, foo_res);
	}

	printf("_______________________\n\n");
	printf("Division test\n");
	foo_res = pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, x, y);
	if (foo_res == x / y) {
		printf("%d / %d == %4d - Equal!\n", x, y, foo_res);
	} else {
		printf("4d / %d == %4d - Wrong!\n", x, y, foo_res);
	}
	printf("_______________________\n\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);


    /* Проверка работоспособности сортировок на небольшом массиве */
    //PrintSimpleCountingSortTestResultsInt(pIMem, pIEcoLab1, 20, 1);
    //PrintSimpleQuickSortTestResultsInt(pIMem, 20);

    //PrintSimpleCountingSortTestResultsChar(pIMem, pIEcoLab1String, 20, 1);
    //PrintSimpleQuickSortTestResultsChar(pIMem, 20);

    /* Тестирование сортировок для дальнейшего сравнения */

    ///* INT32_T */
    ///* Открытие файла */
    //if (!fopen_s(&results, "output_int.csv", "w")) {
    //    printf("file is opened\n");
    //} else {
    //    printf("no");
    //}

    ///* Запись заголовков таблицы */
    //fprintf(results, "sort, type, size, time\n");
    //for (i = 0; i < 12; ++i) {
    //    PrintHardCountingSortTestResultsInt(pIMem, pIEcoLab1, arraySizes[i], 1, results);
    //    PrintHardQuickSortTestResultsInt(pIMem, arraySizes[i], results);
    //}
    ///* Закрытие файла */
    //fclose(results);

    ///* CHAR */
    ///* Открытие файла */
    //if (!fopen_s(&results, "output_char.csv", "w")) {
    //    printf("file is opened\n");
    //} else {
    //    printf("no");
    //}

    ///* Запись заголовков таблицы */
    //fprintf(results, "sort, type, size, time\n");

    //for (i = 0; i < 12; ++i) {
    //    PrintHardCountingSortTestResultsChar(pIMem, pIEcoLab1String, arrayCharSizes[i], 1, results);
    //    PrintHardQuickSortTestResultsChar(pIMem, arrayCharSizes[i], results);
    //}
    ///* Закрытие файла */
    //fclose(results);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }
    if (pIEcoLab1String != 0) {
        pIEcoLab1String->pVTbl->Release(pIEcoLab1String);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

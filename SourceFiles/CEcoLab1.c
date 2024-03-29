/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"


/* Функция для создания индекса для буфера */
uint32_t MakeInd(int32_t a, int32_t b) {
    uint32_t result = a;
    if (a > 0 && b < 0) {
        result -= b;
        return result;
    } else {
        return (uint32_t)(a - b);
    }
}

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY) && pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция MyFunction
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Int_CountingSort(/* in */ struct IEcoLab1* me, /* in */ void* in_data, size_t size, int sortingOrder) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int32_t* data = (int32_t*)in_data;
	int32_t min = MAXINT32;
    int32_t max = MININT32;
	size_t i;
	size_t buff_size;
	uint32_t* buff;
    /* Проверка указателей */
    if (me == 0 || data == 0) {
        return -1;
    }
    /* Поиск максимального и минимального значений массива для установки границ буфера */
    for (i = 0; i < size; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
        if (data[i] < min) {
            min = data[i];
        }
    }
    /* Создание буфера (с учетом того, что необходимо поддерживать отрицательные числа) */
    buff_size = max - min + 1;
    buff = (uint32_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, buff_size * sizeof(int32_t));
    for (i = 0; i < size; ++i) {
        /* Прибавляем 1 к ячейке буфера, индекс которого равен текущему числу
         * (с учетом сдвига в строну отрицательных чисел, для поиска индекса использую вспомогательную функцию) */
        ++buff[MakeInd(data[i], min)];
    }
    /* Если нужно отсортировать в порядке возрастания */
    if (sortingOrder > 0) {
        /* Начинаем идти с начала буфера и вставлять в исходный массив подсчитанные количества значений */
        size_t current_index = 0;
        for (i = 0; i < buff_size; ++i) {
			size_t j;
            for (j = 0; j < buff[i]; ++j) {
                data[current_index++] = i + min;
            }
        }
    } else {
        /* Иначе идем с конца */
        size_t current_index = size - 1;
        for (i = 0; i < buff_size; ++i) {
			size_t j;
            for (j = 0; j < buff[i]; ++j) {
                data[current_index--] = i + min;
            }
        }
    }
    /* Очищаем буфер в конце работы сортировки */
    pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, buff);
    return 0;
}

int16_t ECOCALLMETHOD CEcoLab1_Addition(/* in */ struct IEcoLab1* me, /* in */ int16_t x, /* in */ int16_t y) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
	int16_t result = 0;
    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

	 if (pCMe->m_pIEcoCalculatorX != 0) {
        result = pCMe->m_pIEcoCalculatorX->pVTbl->Addition(pCMe->m_pIEcoCalculatorX, x, y);
    }

    return result;

}

int16_t ECOCALLMETHOD CEcoLab1_Subtraction(/* in */ struct IEcoLab1* me, /* in */ int16_t x, /* in */ int16_t y) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

	int16_t result = 0;
    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

	if (pCMe->m_pIEcoCalculatorX != 0) {
        result = pCMe->m_pIEcoCalculatorX->pVTbl->Subtraction(pCMe->m_pIEcoCalculatorX, x, y);
    }

    return result;

}

int16_t ECOCALLMETHOD CEcoLab1_Multiplication(/* in */ struct IEcoLab1* me, /* in */ int16_t x, /* in */ int16_t y) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
	int16_t result = 0;

    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

	if (pCMe->m_pIEcoCalculatorY != 0) {
        result = pCMe->m_pIEcoCalculatorY->pVTbl->Multiplication(pCMe->m_pIEcoCalculatorY, x, y);
    }
    return result;

}

int16_t ECOCALLMETHOD CEcoLab1_Division(/* in */ struct IEcoLab1* me, /* in */ int16_t x, /* in */ int16_t y) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
	int16_t result = -1;

    /* Проверка указателей */
    if (me == 0) {
        return result;
    }

	if (pCMe->m_pIEcoCalculatorY != 0) {
        result = pCMe->m_pIEcoCalculatorY->pVTbl->Division(pCMe->m_pIEcoCalculatorY, x, y);
    }

    return result;
}


/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;
	/* Указатель на интерфейс внутреннего агрегируемого компонента */
    IEcoUnknown* pOuterUnknown = (IEcoUnknown*)me;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

	/* Включение IEcoCalculatorX из CEcoCalculatorA */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIEcoCalculatorX);
    if (result != 0 || pCMe->m_pIEcoCalculatorX == 0) {
		 /* В случае ошибки включение IEcoCalculatorX из CEcoCalculatorB */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, 0, &IID_IEcoCalculatorX, (void**)&pCMe->m_pIEcoCalculatorX);
    }
	if (result == 0) {
        pCMe->m_pVTblIEcoCalculatorX = pCMe->m_pIEcoCalculatorX->pVTbl;
	}

	/* Агрегирование CEcoCalculatorE для доступа к IEcoCalculatorY */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, pOuterUnknown, &IID_IEcoUnknown, (void**)&pCMe->m_pInnerUnknown);
	if (result != 0 || pCMe->m_pInnerUnknown == 0) {
		/* В случае ошибки включаем IEcoCalculatorY из CEcoCalculatorD */
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**)&pCMe->m_pIEcoCalculatorY);
		if (result == 0) {
            pCMe->m_pVTblIEcoCalculatorY = pCMe->m_pIEcoCalculatorY->pVTbl;
        }
    }

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
        CEcoLab1_QueryInterface,
        CEcoLab1_AddRef,
        CEcoLab1_Release,
        CEcoLab1Int_CountingSort,
		CEcoLab1_Addition,
		CEcoLab1_Subtraction,
		CEcoLab1_Multiplication,
		CEcoLab1_Division
};




/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
		if (pCMe->m_pIEcoCalculatorX != 0) {
            pCMe->m_pISys->pVTbl->Release((pCMe->m_pIEcoCalculatorX));
        }
        if (pCMe->m_pVTblIEcoCalculatorX != 0) {
            pCMe->m_pISys->pVTbl->Release((pCMe->m_pVTblIEcoCalculatorX));
        }
        if (pCMe->m_pIEcoCalculatorY != 0) {
            pCMe->m_pISys->pVTbl->Release((pCMe->m_pIEcoCalculatorY));
        }
        if (pCMe->m_pVTblIEcoCalculatorY != 0) {
            pCMe->m_pISys->pVTbl->Release((pCMe->m_pVTblIEcoCalculatorY));
        }
        if (pCMe->m_pInnerUnknown != 0) {
            pCMe->m_pISys->pVTbl->Release((pCMe->m_pInnerUnknown));
		}
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
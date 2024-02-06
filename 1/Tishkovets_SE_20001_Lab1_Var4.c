/*Файл содержит следующую информацию (построчно): фамилия, имя, отчество 
Считать файл в список таким образом, чтобы он оставался отсортирован по фамилиям
(при совпадении фамилий  по именам; при совпадении и фамилий, и имен по отчествам)
Вывести список фамилий на экран
Ввести с клавиатуры начальные буквы фамилии, имени и отчества;
Проверить, есть ли люди с такой фамилией в списке. Пример:
Ivanov Ivan Ivanovich
Petrov Ivan Petrovich
Запрос «I» «I» «» выдает первую фамилию в списке
Запрос «» «Iv» «» выдает обе фамилии в списке*/
#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define SIZE 20
#define NOTSAME 1
#define SAME 0
#define ERROR_OPENING_FILE -1
#define SUCCESS_ADD 0
#define ADDED 2

//структура List, представляющая собой узел
typedef struct List {
    char surname[SIZE];
    char name[SIZE];
    char father[SIZE];
    struct List* next; //указатель на следующий узел
} List;

//копирует данные из строки surname в элемент узла
void GetStr(List* man, char* surname, char* name, char* father) {
    strcpy(man->surname, surname);
    strcpy(man->name, name);
    strcpy(man->father, father);
}

//получает узел списка из массива
List* TransformToList(char* tmp) {

    List* result;

    int i = 0, j = 0, k = 0;
    //фамилия, имя, отчество
    char surname[SIZE], name[SIZE], father[SIZE];
    char q;
    //считывание фамилии
    while ((q = tmp[i]) != ' ') {
        surname[i] = q;
        i++;
    }
    //конец считывания
    surname[i] = '\0';
    i++;
    //считывание имени
    while ((q = tmp[i]) != ' ') {
        name[k] = q;
        i++;
        k++;
    }
    //конец считывания
    name[k] = '\0';
    i++;
    //считывание отчества
    while ((q = tmp[i]) != '\0') {
        father[j] = q;
        i++;
        j++;
    }
    //конец считывания
    if (father[j - 1] == '\n' && j > 0) {
        father[j - 1] = '\0';
    }
    else {
        father[j] = '\0';
    }
    //создает новый узел
    result = (List*)malloc(sizeof(List));
    //проверка успешного выделения памяти
    if (result == NULL) {
        printf("not enough memory");
        return NULL;
    }

    GetStr(result, surname, name, father);

    return result;
}

//печатает список
void PrintList(List** top) {
    List* tmp = *top;
    while (tmp != NULL) {
        //фамилия имя отчество (через пробел)
        printf("%s ", tmp->surname);
        printf("%s ", tmp->name);
        printf("%s\n", tmp->father);
        tmp = tmp->next; //переход к следующему узлу
    }
}

//печатает запрос с консоли в случае совпадения 
int PrintRequest(List** top, List* input) {
    int count = 0;
    List* tmp = (*top);

    while (tmp != NULL) {
        if (Compare(tmp, input) == SAME) {
            printf("%s ", tmp->surname);
            printf("%s ", tmp->name);
            printf("%s\n", tmp->father);
            count++;
        }
        tmp = tmp->next; //переход к следующему узлу
    }
    return count; //возвращает количество совпадений
}

//создает список, отсортированный по алфавиту
int AddToList(List** top, List* man) {
    List* tmp = (*top);
    List* prev = NULL;

    if (*top == NULL) {
        man->next = *top;
        *top = man;
        return SUCCESS_ADD;
    }

    while (tmp != NULL) {
        //побайтно сравнивает коды символов двух строк и возвращает:
        //1. 0 – если сравниваемее строки идентичны
        //2. Положительное число – если строки отличаются и код первого
        //отличающегося символа в строке man больше кода символа на той же позиции в строке tmp
        //3. Отрицательное число - наоборот (т.е. меньше)
        int SurnameFlag = strcmp(man->surname, tmp->surname);
        int NameFlag = strcmp(man->name, tmp->name);
        int FatherFlag = strcmp(man->father, tmp->father);
        //сравнение для фамилии
        if (SurnameFlag < 0) {
            if (prev == NULL) {
                man->next = (*top);
                (*top) = man;
                return SUCCESS_ADD;
            }
            prev->next = man;
            man->next = tmp;
            return SUCCESS_ADD;
        }
        //сравнение для имени
        else if (SurnameFlag == 0 && NameFlag < 0) {
            if (prev == NULL) {
                man->next = (*top);
                (*top) = man;
                return SUCCESS_ADD;
            }
            prev->next = man;
            man->next = tmp;
            return SUCCESS_ADD;
        } //сравнение для отчества
        else if (SurnameFlag == 0 && NameFlag == 0 && FatherFlag < 0) {
            if (prev == NULL) {
                man->next = (*top);
                (*top) = man;
                return SUCCESS_ADD;
            }
            prev->next = man;
            man->next = tmp;
            return SUCCESS_ADD;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    prev->next = man;
    man->next = NULL;
    return SUCCESS_ADD;
}

//сравнивает элементы списка
int Compare(List* man, List* input) {
    int length;
    //сравнение для фамилии
    if (input->surname != "") {
        length = strlen(input->surname); //длина строки
        //сверяет посимвольно элементы списка с тем, что ввели с консоли
        for (int i = 0; i < length; i++) {
            if (man->surname[i] != input->surname[i]) {
                return NOTSAME;
            }
        }
    }
    //сравнение для имени
    if (input->name != "") {
        length = strlen(input->name); //длина строки
        //сверяет посимвольно элементы списка с тем, что ввели с консоли
        for (int i = 0; i < length; i++) {
            if (man->name[i] != input->name[i]) {
                return NOTSAME;
            }
        }
    }
    //сравнение для отчества
    if (input->father != "") {
        length = strlen(input->father); //длина строки
        //сверяет посимвольно элементы списка с тем, что ввели с консоли
        for (int i = 0; i < length; i++) {
            if (man->father[i] != input->father[i]) {
                return NOTSAME;
            }
        }
    }
    //выводит 0 в случае совпадения
    return SAME;
}

//очищает выделенную память
void FreeList(List** top) {
    List* tmp = (*top);
    //конец, если и так пусто
    if (tmp == NULL)
        return;

    while (*top != NULL) {
        tmp = *top;
        *top = (*top)->next;
        free(tmp);
    }
    free(*top);
}

int main() {
    List* top = NULL;
    List* input = NULL;
    FILE* file;

    file = fopen("List.txt", "r"); //открывает файл с ФИО разных людей
    //проверка успешного открытия файла
    if (file == NULL) {
        printf("cannot open file");
        return ERROR_OPENING_FILE;
    }

    List* person = NULL;
    char* buffer = (char*)calloc(3 * SIZE, sizeof(char));
    //считывает данные из файла в массив, затем в список
    while (!feof(file)) {
        fgets(buffer, 3 * SIZE, file);
        person = TransformToList(buffer);
        AddToList(&top, person);
    }
    //печатает отсортированный список
    PrintList(&top);
    
    printf("Enter the request in format <<surname>> <<name>> <<second name>> : ");
    //проверяет, есть ли в списке люди с нужной фамилией
    fgets(buffer, 3 * SIZE, stdin);
    input = TransformToList(buffer);
    int count = PrintRequest(&top, input);
    if (count == 0) { //если нет совпадений
        printf("no matches found in list");
    }

    free(input);
    free(buffer);
    FreeList(&top);

    fclose(file);
    return 0;
}
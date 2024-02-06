/*���� �������� ��������� ���������� (���������): �������, ���, �������� 
������� ���� � ������ ����� �������, ����� �� ��������� ������������ �� ��������
(��� ���������� �������  �� ������; ��� ���������� � �������, � ���� �� ���������)
������� ������ ������� �� �����
������ � ���������� ��������� ����� �������, ����� � ��������;
���������, ���� �� ���� � ����� �������� � ������. ������:
Ivanov Ivan Ivanovich
Petrov Ivan Petrovich
������ �I� �I� �� ������ ������ ������� � ������
������ �� �Iv� �� ������ ��� ������� � ������*/
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

//��������� List, �������������� ����� ����
typedef struct List {
    char surname[SIZE];
    char name[SIZE];
    char father[SIZE];
    struct List* next; //��������� �� ��������� ����
} List;

//�������� ������ �� ������ surname � ������� ����
void GetStr(List* man, char* surname, char* name, char* father) {
    strcpy(man->surname, surname);
    strcpy(man->name, name);
    strcpy(man->father, father);
}

//�������� ���� ������ �� �������
List* TransformToList(char* tmp) {

    List* result;

    int i = 0, j = 0, k = 0;
    //�������, ���, ��������
    char surname[SIZE], name[SIZE], father[SIZE];
    char q;
    //���������� �������
    while ((q = tmp[i]) != ' ') {
        surname[i] = q;
        i++;
    }
    //����� ����������
    surname[i] = '\0';
    i++;
    //���������� �����
    while ((q = tmp[i]) != ' ') {
        name[k] = q;
        i++;
        k++;
    }
    //����� ����������
    name[k] = '\0';
    i++;
    //���������� ��������
    while ((q = tmp[i]) != '\0') {
        father[j] = q;
        i++;
        j++;
    }
    //����� ����������
    if (father[j - 1] == '\n' && j > 0) {
        father[j - 1] = '\0';
    }
    else {
        father[j] = '\0';
    }
    //������� ����� ����
    result = (List*)malloc(sizeof(List));
    //�������� ��������� ��������� ������
    if (result == NULL) {
        printf("not enough memory");
        return NULL;
    }

    GetStr(result, surname, name, father);

    return result;
}

//�������� ������
void PrintList(List** top) {
    List* tmp = *top;
    while (tmp != NULL) {
        //������� ��� �������� (����� ������)
        printf("%s ", tmp->surname);
        printf("%s ", tmp->name);
        printf("%s\n", tmp->father);
        tmp = tmp->next; //������� � ���������� ����
    }
}

//�������� ������ � ������� � ������ ���������� 
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
        tmp = tmp->next; //������� � ���������� ����
    }
    return count; //���������� ���������� ����������
}

//������� ������, ��������������� �� ��������
int AddToList(List** top, List* man) {
    List* tmp = (*top);
    List* prev = NULL;

    if (*top == NULL) {
        man->next = *top;
        *top = man;
        return SUCCESS_ADD;
    }

    while (tmp != NULL) {
        //�������� ���������� ���� �������� ���� ����� � ����������:
        //1. 0 � ���� ������������ ������ ���������
        //2. ������������� ����� � ���� ������ ���������� � ��� �������
        //������������� ������� � ������ man ������ ���� ������� �� ��� �� ������� � ������ tmp
        //3. ������������� ����� - �������� (�.�. ������)
        int SurnameFlag = strcmp(man->surname, tmp->surname);
        int NameFlag = strcmp(man->name, tmp->name);
        int FatherFlag = strcmp(man->father, tmp->father);
        //��������� ��� �������
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
        //��������� ��� �����
        else if (SurnameFlag == 0 && NameFlag < 0) {
            if (prev == NULL) {
                man->next = (*top);
                (*top) = man;
                return SUCCESS_ADD;
            }
            prev->next = man;
            man->next = tmp;
            return SUCCESS_ADD;
        } //��������� ��� ��������
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

//���������� �������� ������
int Compare(List* man, List* input) {
    int length;
    //��������� ��� �������
    if (input->surname != "") {
        length = strlen(input->surname); //����� ������
        //������� ����������� �������� ������ � ���, ��� ����� � �������
        for (int i = 0; i < length; i++) {
            if (man->surname[i] != input->surname[i]) {
                return NOTSAME;
            }
        }
    }
    //��������� ��� �����
    if (input->name != "") {
        length = strlen(input->name); //����� ������
        //������� ����������� �������� ������ � ���, ��� ����� � �������
        for (int i = 0; i < length; i++) {
            if (man->name[i] != input->name[i]) {
                return NOTSAME;
            }
        }
    }
    //��������� ��� ��������
    if (input->father != "") {
        length = strlen(input->father); //����� ������
        //������� ����������� �������� ������ � ���, ��� ����� � �������
        for (int i = 0; i < length; i++) {
            if (man->father[i] != input->father[i]) {
                return NOTSAME;
            }
        }
    }
    //������� 0 � ������ ����������
    return SAME;
}

//������� ���������� ������
void FreeList(List** top) {
    List* tmp = (*top);
    //�����, ���� � ��� �����
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

    file = fopen("List.txt", "r"); //��������� ���� � ��� ������ �����
    //�������� ��������� �������� �����
    if (file == NULL) {
        printf("cannot open file");
        return ERROR_OPENING_FILE;
    }

    List* person = NULL;
    char* buffer = (char*)calloc(3 * SIZE, sizeof(char));
    //��������� ������ �� ����� � ������, ����� � ������
    while (!feof(file)) {
        fgets(buffer, 3 * SIZE, file);
        person = TransformToList(buffer);
        AddToList(&top, person);
    }
    //�������� ��������������� ������
    PrintList(&top);
    
    printf("Enter the request in format <<surname>> <<name>> <<second name>> : ");
    //���������, ���� �� � ������ ���� � ������ ��������
    fgets(buffer, 3 * SIZE, stdin);
    input = TransformToList(buffer);
    int count = PrintRequest(&top, input);
    if (count == 0) { //���� ��� ����������
        printf("no matches found in list");
    }

    free(input);
    free(buffer);
    FreeList(&top);

    fclose(file);
    return 0;
}
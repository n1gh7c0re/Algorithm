/* ���������, �������������� ������ � ����� ������ ��������� ������.
������� ������ - ������� � �������. ���� ��� ������ � �������.
���� ������ �������� � ������ � ���� ������� ������������������ �������,
������������������� ������� ��������. ���������� ����������� � ������ ���������������� ������.
�������� ��������� ������, �������� � ���������� ��������� ������.
���� ������ ������������ � ����� � ����������� � ����. */
#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_SURNAME_LENGTH 50 //����� �������
#define MAX_PHONE_LENGTH 11 //����� ������ ��������
#define ENGLISH_ALPHABET 26 //���������� ������� � ��

//���������, �������������� ����� ����
struct People {
	char surname[MAX_SURNAME_LENGTH];
	char phone[MAX_PHONE_LENGTH];
	struct People* next;
};

struct PeopleList {
	struct People* head;
};

//������ ��� �������� ���� ������
struct PhoneBook {
	struct PeopleList alphabet[ENGLISH_ALPHABET];
};

//�������������� ���� ������
void initPhoneBook(struct PhoneBook* phonebook) {
	for (int i = 0; i < ENGLISH_ALPHABET; i++) {
		phonebook->alphabet[i].head = NULL;
	}
}

//��������� ����� � ���� ������
void addPeople(struct PhoneBook* phonebook, const char* surname, const char* phone) {
	struct PeopleList* list;
	/* ���������� ������ �������, ���� ����� ������� �������
	   65 - ��� ��� ������� 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];

	struct People* newPeople = (struct People*)malloc(sizeof(struct People));
	if (newPeople == NULL){ //�������� ��������� ��������� ������
		printf("������ ��������� ������");
		return;
	}
	//�������� ������ �� ������ surname � phone � ����
	strcpy(newPeople->surname, surname);
	strcpy(newPeople->phone, phone);
	newPeople->next = list->head;
	list->head = newPeople;
}

//���� � ���� ������ �������� �� �������
struct People* findPeople(struct PhoneBook* phonebook, const char* surname) {
	struct PeopleList* list;
	/* ���������� ������ �������, ���� ����� ������� �������
	   65 - ��� ��� ������� 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];
	struct People* current = list->head;
	/*  �������� ���������� ���� �������� ���� ����� � ���������� :
		1. 0 � ���� ����������� ������ ���������
		2. ������������� ����� � ���� ������ ���������� � ��� �������
		������������� ������� � ������ current ������ ���� ������� �� ��� �� ������� � ������ surname
		3. ������������� ����� - �������� (�.�. ������) */
	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

//������� �� ���� ������ �������� �� �������
void deletePeople(struct PhoneBook* phonebook, const char* surname) {
	struct PeopleList* list;
	/* ���������� ������ �������, ���� ����� ������� �������
	   65 - ��� ��� ������� 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];
	struct People* current = list->head; //�������
	struct People* prev = NULL; //����������

	while (current != NULL) {
		/* �������� ���������� ���� �������� ���� ����� � ���������� :
		   1. 0 � ���� ������������ ������ ���������
		   2. ������������� ����� � ���� ������ ���������� � ��� �������
		   ������������� ������� � ������ current ������ ���� ������� �� ��� �� ������� � ������ surname
		   3. ������������� ����� - �������� (�.�. ������) */
		if (strcmp(current->surname, surname) == 0) {
			if (prev != NULL) {
				prev->next = current->next;
			}
			else {
				list->head = current->next;
			}
			free(current); //������� ���������� � ��������
			return;
		}
		prev = current;
		current = current->next;
	}
}

//��������� ������ �� ��������
void insertPeopleSorted(struct People** sortedList, struct People* newPeople) {
	struct People* current;
	/* �������� ���������� ���� �������� ���� ����� � ���������� :
	   1. 0 � ���� ������������ ������ ���������
	   2. ������������� ����� � ���� ������ ���������� � ��� �������
	   ������������� ������� � ������ current ������ ���� ������� �� ��� �� ������� � ������ surname
	   3. ������������� ����� - �������� (�.�. ������) */
	if (*sortedList == NULL || strcmp(newPeople->surname, (*sortedList)->surname) <= 0) {
		newPeople->next = *sortedList;
		*sortedList = newPeople;
	}
	else {
		current = *sortedList;
		while (current->next != NULL && strcmp(newPeople->surname, current->next->surname) > 0) {
			current = current->next;
		}
		newPeople->next = current->next;
		current->next = newPeople;
	}
}

//��������� ������ ������ ���� ������ �� ��������
void sortPhoneBook(struct PhoneBook* phonebook) {
	struct PeopleList* list;

	for (int i = 0; i < ENGLISH_ALPHABET; i++) {
		list = &phonebook->alphabet[i];
		struct People* current = list->head;
		struct People* sortedList = NULL;
		struct People* next;

		while (current != NULL) {
			next = current->next;
			insertPeopleSorted(&sortedList, current);
			current = next;
		}
		list->head = sortedList;
	}
}

//��������� ���� ������ � ����
void savePhoneBook(struct PhoneBook* phonebook, const char* filename) {
	FILE* file = fopen(filename, "wb"); //��������� ���� ��� ������ ������
	if (!file) { //�������� ��������� �������� �����
		printf("������ ��� �������� �����");
		return;
	}

	struct PeopleList* list;
	struct People* current;

	for (int i = 0; i < ENGLISH_ALPHABET; i++) {
		list = &phonebook->alphabet[i];
		current = list->head;
		while (current != NULL) { //���������� � ���� ������
			fwrite(current, sizeof(struct People), 1, file);
			fwrite("\n", sizeof(char), 1, file);
			current = current->next;
		}
	}
	fclose(file);
}

//��������� ���� ������ �� �����
void loadPhoneBook(struct PhoneBook* phonebook, const char* filename) {
	FILE* file = fopen(filename, "rb"); //��������� ���� ��� ������
	if (!file) { //�������� ��������� �������� �����
		printf("������ ��� �������� �����");
		exit(1);
	}

	initPhoneBook(phonebook);
	struct People people;
	/* ����������, � ����� ������ (������� �������) �������� �������� 
	   � ����� ���������� ���������� � ���� ������ */
	while (fread(&people, sizeof(struct People), 1, file) == 1) {
		char first = people.surname[0];
		int index = first - 65;
		addPeople(&phonebook->alphabet[index], people.surname, people.phone);
	}
	fclose(file);
}

int main() {
	setlocale(LC_CTYPE, "Russian");

	struct PhoneBook phonebook;
	initPhoneBook(&phonebook);

	//�������� �������� �������� ���� ������ �� �����
	loadPhoneBook(&phonebook, "PhoneBook.txt");
	//�������� ��������� ���������� 5 ������� � ���� ������
	addPeople(&phonebook, "Tishkovets", "89111234567");
	addPeople(&phonebook, "Andreev", "89819876543");
	addPeople(&phonebook, "Sergeev", "89956752134");
	addPeople(&phonebook, "Borisenko", "89218983434");
	addPeople(&phonebook, "Drekalov", "89054217889");

	//�������� ��������� ������ �������� �� �������
	struct People* foundPeople = findPeople(&phonebook, "Sergeev");
	if (foundPeople != NULL) {
		printf("�������: %s %s\n", foundPeople->surname, foundPeople->phone);
	}
	else printf("��� ����������\n");
	
	//�������� ��������� �������� �������� �� ���� ������
	deletePeople(&phonebook, "Tishkovets");
	//�������� �������� ����������
	sortPhoneBook(&phonebook);
	//�������� ��������� ���������� ���� ������ � ����
	savePhoneBook(&phonebook, "PhoneBook.txt");

	return 0;
}
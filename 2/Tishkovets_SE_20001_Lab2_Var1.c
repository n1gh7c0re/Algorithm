/* Программа, осуществляющая работу с базой данных «Записная книжка».
Элемент данных - фамилия и телефон. Ключ для поиска – фамилия.
База данных хранится в памяти в виде массива самоорганизующихся списков,
проиндексированного буквами алфавита. Добавление выполняется в начало соответствующего списка.
Написаны процедуры поиска, удаления и сортировки заданного списка.
База данных зачитывается с файла и сохраняется в файл. */
#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_SURNAME_LENGTH 50 //длина фамилии
#define MAX_PHONE_LENGTH 11 //длина номера телефона
#define ENGLISH_ALPHABET 26 //количество списков в бд

//структура, представляющая собой узел
struct People {
	char surname[MAX_SURNAME_LENGTH];
	char phone[MAX_PHONE_LENGTH];
	struct People* next;
};

struct PeopleList {
	struct People* head;
};

//массив для хранения базы данных
struct PhoneBook {
	struct PeopleList alphabet[ENGLISH_ALPHABET];
};

//инициализирует базу данных
void initPhoneBook(struct PhoneBook* phonebook) {
	for (int i = 0; i < ENGLISH_ALPHABET; i++) {
		phonebook->alphabet[i].head = NULL;
	}
}

//добавляет людей в базу данных
void addPeople(struct PhoneBook* phonebook, const char* surname, const char* phone) {
	struct PeopleList* list;
	/* определяет индекс массива, куда будет записан человек
	   65 - это код символа 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];

	struct People* newPeople = (struct People*)malloc(sizeof(struct People));
	if (newPeople == NULL){ //проверка успешного выделения памяти
		printf("Ошибка выделения памяти");
		return;
	}
	//копирует данные из строки surname и phone в узел
	strcpy(newPeople->surname, surname);
	strcpy(newPeople->phone, phone);
	newPeople->next = list->head;
	list->head = newPeople;
}

//ищет в базе данных человека по фамилии
struct People* findPeople(struct PhoneBook* phonebook, const char* surname) {
	struct PeopleList* list;
	/* определяет индекс массива, куда будет записан человек
	   65 - это код символа 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];
	struct People* current = list->head;
	/*  побайтно сравнивает коды символов двух строк и возвращает :
		1. 0 – если сравниваеме строки идентичны
		2. Положительное число – если строки отличаются и код первого
		отличающегося символа в строке current больше кода символа на той же позиции в строке surname
		3. Отрицательное число - наоборот (т.е. меньше) */
	while (current != NULL) {
		if (strcmp(current->surname, surname) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

//удаляет из базы данных человека по фамилии
void deletePeople(struct PhoneBook* phonebook, const char* surname) {
	struct PeopleList* list;
	/* определяет индекс массива, куда будет записан человек
	   65 - это код символа 'A' */
	int index = surname[0] - 65;
	list = &phonebook->alphabet[index];
	struct People* current = list->head; //текущий
	struct People* prev = NULL; //предыдущий

	while (current != NULL) {
		/* побайтно сравнивает коды символов двух строк и возвращает :
		   1. 0 – если сравниваемое строки идентичны
		   2. Положительное число – если строки отличаются и код первого
		   отличающегося символа в строке current больше кода символа на той же позиции в строке surname
		   3. Отрицательное число - наоборот (т.е. меньше) */
		if (strcmp(current->surname, surname) == 0) {
			if (prev != NULL) {
				prev->next = current->next;
			}
			else {
				list->head = current->next;
			}
			free(current); //удаляет информацию о человеке
			return;
		}
		prev = current;
		current = current->next;
	}
}

//сортирует список по алфавиту
void insertPeopleSorted(struct People** sortedList, struct People* newPeople) {
	struct People* current;
	/* побайтно сравнивает коды символов двух строк и возвращает :
	   1. 0 – если сравниваемое строки идентичны
	   2. Положительное число – если строки отличаются и код первого
	   отличающегося символа в строке current больше кода символа на той же позиции в строке surname
	   3. Отрицательное число - наоборот (т.е. меньше) */
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

//сортирует каждый список базы данных по алфавиту
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

//сохраняет базу данных в файл
void savePhoneBook(struct PhoneBook* phonebook, const char* filename) {
	FILE* file = fopen(filename, "wb"); //открывает файл для записи данных
	if (!file) { //проверка успешного открытия файла
		printf("Ошибка при открытии файла");
		return;
	}

	struct PeopleList* list;
	struct People* current;

	for (int i = 0; i < ENGLISH_ALPHABET; i++) {
		list = &phonebook->alphabet[i];
		current = list->head;
		while (current != NULL) { //записывает в файл данные
			fwrite(current, sizeof(struct People), 1, file);
			fwrite("\n", sizeof(char), 1, file);
			current = current->next;
		}
	}
	fclose(file);
}

//загружает базу данных из файла
void loadPhoneBook(struct PhoneBook* phonebook, const char* filename) {
	FILE* file = fopen(filename, "rb"); //открывает файл для чтения
	if (!file) { //проверка успешного открытия файла
		printf("Ошибка при открытии файла");
		exit(1);
	}

	initPhoneBook(phonebook);
	struct People people;
	/* определяет, в какой список (элемент массива) добавить человека 
	   и потом записывает информацию в базу данных */
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

	//проверка успешной загрузки базы данных из файла
	loadPhoneBook(&phonebook, "PhoneBook.txt");
	//проверка успешного добавления 5 человек в базу данных
	addPeople(&phonebook, "Tishkovets", "89111234567");
	addPeople(&phonebook, "Andreev", "89819876543");
	addPeople(&phonebook, "Sergeev", "89956752134");
	addPeople(&phonebook, "Borisenko", "89218983434");
	addPeople(&phonebook, "Drekalov", "89054217889");

	//проверка успешного поиска человека по фамилии
	struct People* foundPeople = findPeople(&phonebook, "Sergeev");
	if (foundPeople != NULL) {
		printf("Найдено: %s %s\n", foundPeople->surname, foundPeople->phone);
	}
	else printf("Нет совпадений\n");
	
	//проверка успешного удаления человека из базы данных
	deletePeople(&phonebook, "Tishkovets");
	//проверка успешной сортировки
	sortPhoneBook(&phonebook);
	//проверка успешного сохранения базы данных в файл
	savePhoneBook(&phonebook, "PhoneBook.txt");

	return 0;
}
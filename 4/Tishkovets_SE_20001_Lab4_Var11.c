#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

typedef struct {
	char* data;
	struct tree_t* left;// лево = да
	struct tree_t* right;//право = нет
}tree_t;

typedef struct {
	tree_t* tree;
	struct turn_t* next;
}turn_t;

char* processing_console(int* flag) {//флаг = 1 => вводится yes, флаг = 2 => конец программы, инчаче: no, или это загаданное слово, или его характеристика
	char c;
	char* string = malloc(sizeof(char) * 2);
	int count = 1;
	string[0] = '"';
	c = getchar();
	while (c != '\n') {
		string[count] = c;
		count += 1;
		string = realloc(string, sizeof(char) * (count + 1));
		c = getchar();
	}
	string[count] = '"';
	count += 1;
	string = realloc(string, sizeof(char) * (count + 1));
	string[count] = '\0';
	if (count == 5 && string[1] == 'e' && string[2] == 'n' && string[3] == 'd') *flag = 2;
	else if (count == 5 && string[1] == 'y' && string[2] == 'e' && string[3] == 's') *flag = 1;
	else *flag = 0;
	return string;
}

int processing_no(tree_t** tree) {
	printf("What characterizes the hidden word?\n");
	int tmp_flag;
	tree_t* new_tree = malloc(sizeof(tree));
	new_tree->right = (*tree);
	new_tree->data = processing_console(&tmp_flag);
	if (tmp_flag == 2) return tmp_flag;
	printf("What a word?\n");
	tree_t* new_tree_left = malloc(sizeof(tree_t));
	new_tree_left->data = processing_console(&tmp_flag);
	if (tmp_flag == 2) return tmp_flag;
	new_tree_left->left = NULL;
	new_tree_left->right = NULL;
	new_tree->left = new_tree_left;
	(*tree) = new_tree;
}

int processing_user(tree_t** tree) {
	tree_t* tmp_tree = (*tree);
	while (tmp_tree != NULL) {
		printf("%s ?\n", tmp_tree->data);
		int flag;
		char* tmp_string = processing_console(&flag);
		if (flag == 2) return 2;
		if (flag == 1) {
			tmp_tree = tmp_tree->left;
			if (tmp_tree != NULL) tree = &((*tree)->left);
		}
		else {
			tmp_tree = tmp_tree->right;
			if (tmp_tree != NULL) tree = &((*tree)->right);
		}
		free(tmp_string);
		if (flag == 0 && tmp_tree == NULL) {
			flag = processing_no(tree);
			if (flag == 2) return flag;
		}
	}
}

int first_tree(tree_t* tree) {
	printf("Enter any characteristic\n");
	int tmp_flag;
	tree->data = processing_console(&tmp_flag);
	if (tmp_flag == 2) return tmp_flag;
	printf("Enter a word that satisfies this characteristic, then a word that does not satisfy it\n");
	tree_t* left_tree = malloc(sizeof(tree));
	left_tree->data = processing_console(&tmp_flag);
	if (tmp_flag == 2) return tmp_flag;
	left_tree->left = NULL;
	left_tree->right = NULL;
	tree_t* right_tree = malloc(sizeof(tree));
	right_tree->data = processing_console(&tmp_flag);
	if (tmp_flag == 2) return tmp_flag;
	right_tree->left = NULL;
	right_tree->right = NULL;
	tree->left = left_tree;
	tree->right = right_tree;
}

char** processing_file(FILE* file, int* size) {
	char c = fgetc(file);
	char** massive = malloc(sizeof(char*));
	int size_massive = 0;
	int size_string = 0;
	char* string = malloc(sizeof(char));
	int flag = 1;
	while (c != EOF) {
		string[size_string] = c;
		size_string += 1;
		string = realloc(string, sizeof(char) * (size_string + 1));
		c = fgetc(file);
		if (c == '"') {
			string[size_string] = c;
			size_string += 1;
			string = realloc(string, sizeof(char) * (size_string + 1));
			string[size_string] = '\0';
			size_string = 0;
			massive[size_massive] = string;
			string = malloc(sizeof(char));
			size_massive += 1;
			massive = realloc(massive, sizeof(char*) * (size_massive + 1));
			c = fgetc(file);
			//if (c != EOF) c = fgetc(file);
		}
	}
	massive[size_massive] = NULL;
	*size = size_massive;
	return massive;
}

tree_t* create_tree_from_massive(int size_massive, char** massive, int step) {
	if (step < size_massive) {
		tree_t* tree = malloc(sizeof(tree));
		if (massive[step][1] == 'N' && massive[step][2] == 'U' && massive[step][3] == 'L' && massive[step][4] == 'L') return NULL;
		tree->data = massive[step];
		tree->left = create_tree_from_massive(size_massive, massive, step * 2 + 1);
		tree->right = create_tree_from_massive(size_massive, massive, step * 2 + 2);
		return tree;
	}
	else return NULL;
}

void addturn(tree_t* tree, turn_t** turn) {
	while ((*turn) != NULL) {
		turn = &((*turn)->next);
	}
	turn_t* newturn = malloc(sizeof(turn));
	newturn->tree = tree;
	newturn->next = (*turn);
	(*turn) = newturn;
}

void deleteturn(turn_t** turn, turn_t* delturn) {
	turn_t** tmp_turn = turn;
	while ((*turn) != delturn) {//мб убрать
		tmp_turn = turn;
		turn = &((*turn)->next);
	}
	(*tmp_turn)->next = (*turn)->next;
}

int check_turn(turn_t* turn) {
	int flag = 1;
	while (flag == 1) {
		if (turn == NULL) break;
		if (turn->tree != NULL) flag = 0;
		turn = turn->next;
	}
	return flag;
}

void print_tree_in_file(tree_t* tree, FILE* file) {
	turn_t* turn = NULL;
	int flag = 0;
	addturn(tree, &turn);
	turn_t* tmp_turn = turn;
	int count = 0;
	while (flag == 0) {
		if (turn->tree != NULL) {
			fprintf(file, turn->tree->data);
			/*fprintf(file, " ");*/
			addturn(turn->tree->left, &tmp_turn);
			addturn(turn->tree->right, &tmp_turn);
			turn = turn->next;
			deleteturn(&tmp_turn, tmp_turn);
			tmp_turn = turn;
		}
		else {
			flag = check_turn(turn);
			if (flag != 0) break;
			fprintf(file, "\"NULL\"");
			addturn(NULL, &tmp_turn);
			addturn(NULL, &tmp_turn);
			turn = turn->next;
			deleteturn(&tmp_turn, tmp_turn);
			tmp_turn = turn;
		}
		count += 1;
	}
}

void main() {
	FILE* file = fopen("alg4.txt", "r");
	tree_t* tree = malloc(sizeof(tree_t));
	int size_massive;
	char** massive = processing_file(file, &size_massive);
	fclose(file);
	int flag = 0;
	if (massive[0] == NULL) {
		flag = first_tree(tree);
		free(massive);
	}
	else {
		tree = create_tree_from_massive(size_massive, massive, 0);
		free(massive);
	}
	while (flag != 2) {//end = выход
		flag = processing_user(&tree);
		printf("\ninput \"end\" to exite or 'cont' to continue\n");
		char* tmp = processing_console(&flag);
		free(tmp);
	}
	massive = malloc(sizeof(char*));
	size_massive = 0;
	file = fopen("alg4.txt", "w");
	print_tree_in_file(tree, file);
	fclose(file);
}
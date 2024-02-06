#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a, b) ((a > b) ? a : b) 

typedef struct tree_t {
    int length; //length of word
    struct tree_t* left;
    struct tree_t* parent;
    struct tree_t* right;
    int subtreeWigth; //sum of word lengths of sons,grandsons, etc. until leaves
    char* word;
} tree_t;

typedef enum {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2
}way_t;

void NodeAdd(tree_t** tree, char* buff) {
    tree_t* temp = (*tree), * newnode, * ptr = (*tree);
    int length = strlen(buff);
    int i, subtreelength;
    newnode = (tree_t*)malloc(sizeof(tree_t));
    if (!newnode)
        return;

    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;
    newnode->length = length;
    newnode->subtreeWigth = 0;
    newnode->length = length;
    newnode->word = (char*)malloc((length + 1) * sizeof(char));
    if (!newnode->word)
        return;

    strcpy(newnode->word, buff);

    if (!(*tree)) {
        (*tree) = newnode;
        return;
    }

    while (ptr) {
        if (length < ptr->length) {
            if (ptr->left)
                ptr = ptr->left;
            else
                break;
        }

        else if (length > ptr->length) {
            if (ptr->right)
                ptr = ptr->right;
            else
                break;
        }

        else { //node with such length exists
            free(newnode);
            return;
        }
    }

    //put new node
    if (length < ptr->length)
        ptr->left = newnode;
    else
        ptr->right = newnode;

    newnode->parent = ptr;

    //update subtreelengths
    subtreelength = 0;
    temp = newnode;
    while (temp->parent) {
        temp = temp->parent;
        temp->subtreeWigth += length;
    }
    newnode->subtreeWigth += length; //то, чего не хватало!!!
}

void DestroyTree(tree_t* tree) {
    if (tree) {
        DestroyTree(tree->left);
        DestroyTree(tree->right);
        free(tree->word);
        free(tree);
    }
}

tree_t* NodeFind(tree_t* tree, char* buff) {
    tree_t* temp = tree;

    int length = strlen(buff);

    while (temp) {
        if (length < temp->length)
            temp = temp->left;

        else if (length > temp->length)
            temp = temp->right;

        //check if words are equal
        else {
            if (strcmp(temp->word, buff) != 0)
                return NULL;
            return temp; //equal
        }
    }

    return NULL;
}

void NodeDelete(tree_t** tree, tree_t* vertex) {
    if (!vertex)
        return;
    int temp, i;
    tree_t* maximum = vertex, * parent, * child, * temp2; //go to left child of vertex and then to right 
    way_t parentway;
    char* help;

    //no childs, free the element
    if (vertex->left == NULL && vertex->right == NULL) {
        if ((vertex == (*tree))) {
            free(vertex->word);
            free(vertex);
            (*tree) = NULL;
            return;
        }

        parent = vertex->parent;
        parentway = parent->left == vertex ? LEFT : RIGHT;

        if (parentway == LEFT)
            parent->left = NULL;
        else
            parent->right = NULL;

        //update subtreewidth
        temp2 = vertex;
        while (temp2->parent) {
            temp2 = temp2->parent;
            temp2->subtreeWigth -= vertex->length;
        }

        free(vertex->word);
        free(vertex);

        return;
    }

    //has one child, put child instead of itself
    if ((vertex->left != NULL && vertex->right == NULL) || (vertex->left == NULL && vertex->right != NULL)) {

        if (vertex == (*tree)) { //no parent
            if (vertex->left)
                (*tree) = vertex->left;
            else
                (*tree) = vertex->right;
            (*tree)->parent = NULL;
            free(vertex);
            return;
        }

        parent = vertex->parent;
        parentway = parent->left == vertex ? LEFT : RIGHT;

        if (vertex->left) //only left child
            child = vertex->left;
        else //only right child
            child = vertex->right;

        child->parent = vertex->parent;

        if (parentway == LEFT)
            parent->left = child;
        else
            parent->right = child;

        //update subtreewidth
        temp2 = vertex;
        while (temp2->parent) {
            temp2 = temp2->parent;
            temp2->subtreeWigth -= vertex->length;
        }

        free(vertex->word);
        free(vertex);
        return;
    }

    //node has two childs
    maximum = maximum->right;

    while (maximum->left)
        maximum = maximum->left;

    temp = maximum->length;
    maximum->length = vertex->length;
    vertex->length = temp;

    help = (char*)realloc(vertex->word, (max(vertex->length, maximum->length) + 1) * sizeof(char)); //need more memory
    if (help) {
        vertex->word = help;
        help = NULL;
    }
    else {
        free(vertex->word);
        return;
    }

    for (i = 0; i <= max(vertex->length, maximum->length); ++i)
        vertex->word[i] = maximum->word[i];

    temp2 = maximum;
    while (temp2->parent != vertex->parent) {
        temp2 = temp2->parent;
        temp2->subtreeWigth += (maximum->length - vertex->length);
    }

    //and now need to delete max

    //max can have one child on the right
    if (maximum->right) {
        child = maximum->right;
        parent = maximum->parent;
        parentway = parent->left == maximum ? LEFT : RIGHT;

        if (parentway == LEFT)
            parent->left = child;
        else
            parent->right = child;

        child->parent = parent;
    }

    //max has no childs
    else {
        parent = maximum->parent;
        parentway = parent->left == maximum ? LEFT : RIGHT;

        if (parentway == LEFT)
            parent->left = NULL;
        else
            parent->right = NULL;
    }
    //update subtreewidth
    temp2 = maximum;
    while (temp2->parent) {
        temp2 = temp2->parent;
        temp2->subtreeWigth -= maximum->length;
    }

    free(maximum->word);
    free(maximum);
    return;
}


/* printing tree rotated on 90 degrees. Root of tree on the left, leaves on the right
   structure of printing:
   1. word
   2. length of word
   3. length of subtree
*/
void PrintTree(tree_t* tree, int level) {
    if (tree) {
        int i;
        PrintTree(tree->left, level + (tree->length)); //tree->length - length of word
        for (i = 0; i < level; i++)
            printf("   ");

        for (i = 0; i < tree->length; ++i)
            printf("%c", tree->word[i]);

        printf("\n");
        for (i = 0; i < level; i++)
            printf("   ");
        printf("%d\n", tree->length);

        for (i = 0; i < level; i++)
            printf("   ");
        printf("%d\n", tree->subtreeWigth);

        PrintTree(tree->right, level + (tree->length));
    }
}

int main(void) {
    tree_t* tree = NULL;

    NodeAdd(&tree, "MERCEDES");
    NodeAdd(&tree, "BMW");
    NodeAdd(&tree, "AUDI");
    NodeAdd(&tree, "VOLKSWAGEN");
    NodeAdd(&tree, "RENAULT");
    NodeAdd(&tree, "TOYOTA");
    NodeAdd(&tree, "HONDA");
    NodeAdd(&tree, "LAMBORGHINI");
    NodeDelete(&tree, NodeFind(tree, "RENAULT"));
    PrintTree(tree, 1);

    DestroyTree(tree);

    return 0;
}
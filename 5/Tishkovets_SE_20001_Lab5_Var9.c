#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct tree_t {
    int num;
    int order; //order from the smallest number in tree to the biggest
    struct tree_t* left;
    struct tree_t* parent;
    struct tree_t* right;
} tree_t;

typedef enum {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2
}way_t;

//will place order in appending function
void UpdateOrder(tree_t* tree, int* order) {
    if (tree) {
        if (tree->left)
            UpdateOrder(tree->left, order);

        tree->order = *order;
        (*order)++;

        if (tree->right)
            UpdateOrder(tree->right, order);
    }
}

void NodeAdd(tree_t** tree, int num) {
    tree_t* temp = (*tree), * newnode, * ptr = (*tree);
    int order = 1;
    newnode = (tree_t*)malloc(sizeof(tree_t));
    if (!newnode)
        return;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;
    newnode->num = num;

    if (!(*tree)) {
        (*tree) = newnode;
        UpdateOrder((*tree), &order);
        return;
    }

    while (ptr) {
        if (num < ptr->num) {
            if (ptr->left)
                ptr = ptr->left;
            else
                break;
        }

        else if (num > ptr->num) {
            if (ptr->right)
                ptr = ptr->right;
            else
                break;
        }

        else { //such node exists
            free(newnode);
            return;
        }
    }

    //put new node
    if (num < ptr->num)
        ptr->left = newnode;
    else
        ptr->right = newnode;

    newnode->parent = ptr;

    UpdateOrder((*tree), &order);
}

void DestroyTree(tree_t* tree) {
    if (tree) {
        DestroyTree(tree->left);
        DestroyTree(tree->right);
        free(tree);
    }
}

tree_t* KLowerNodeFind(tree_t* tree, int k) {
    if (tree) {

        if (tree->order == k)
            return tree;

        if (KLowerNodeFind(tree->left, k))
            return tree->left;

        if (KLowerNodeFind(tree->right, k))
            return tree->right;
    }
    return NULL;
}

tree_t* NodeFind(tree_t* tree, int num) {
    tree_t* temp = tree;

    while (temp) {

        if (num < temp->num)
            temp = temp->left;

        else if (num > temp->num)
            temp = temp->right;

        else// found
            return temp;
    }

    return NULL;
}

void NodeDelete(tree_t** tree, tree_t* vertex) {
    if (!vertex)
        return;
    int temp, order = 1;
    tree_t* max = vertex, * parent, * child; //go to left child of vertex and then to right 
    way_t parentway;

    //no childs, free the element
    if (vertex->left == NULL && vertex->right == NULL) {
        if ((vertex == (*tree))) {
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

        free(vertex);
        UpdateOrder((*tree), &order);
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
            UpdateOrder((*tree), &order);
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

        free(vertex);
        UpdateOrder((*tree), &order);
        return;
    }

    //node has two childs
    max = max->right;

    while (max->left)
        max = max->left;

    temp = max->num;
    max->num = vertex->num;
    vertex->num = temp;

    //no need to delete max

    //max can have one child on the right
    if (max->right) {
        child = max->right;
        parent = max->parent;
        parentway = parent->left == max ? LEFT : RIGHT;

        if (parentway == LEFT)
            parent->left = child;
        else
            parent->right = child;

        child->parent = parent;
    }

    //max has no childs
    else {
        parent = max->parent;
        parentway = parent->left == max ? LEFT : RIGHT;

        if (parentway == LEFT)
            parent->left = NULL;
        else
            parent->right = NULL;
    }
    free(max);
    UpdateOrder((*tree), &order);
    return;
}

//printing tree rotated on 90 degrees without vertexes which order >= k
void PrintLowerKTree(tree_t* tree, int level, int k) {
    if (tree) {
        PrintLowerKTree(tree->left, level + 1, k);
        if (tree->order < k) {
            for (int i = 0; i < level; i++)
                printf("    ");
            printf("%d\n", tree->num);
        }
        PrintLowerKTree(tree->right, level + 1, k);
    }
}

int main(void) {
    const int k = 5; //order
    tree_t* tree = NULL, *find = NULL;

    NodeAdd(&tree, -3);
    NodeAdd(&tree, -110);
    NodeAdd(&tree, 7);
    NodeAdd(&tree, -1);
    NodeAdd(&tree, 50);
    NodeAdd(&tree, 10);
    NodeAdd(&tree, 92);
   
    NodeDelete(&tree, NodeFind(tree, 2));

    find = KLowerNodeFind(tree, k);

    if (!((find->num) % 2))
        PrintLowerKTree(tree, 1, k);
    else printf("Not today");

    DestroyTree(tree);
    return 0;
}
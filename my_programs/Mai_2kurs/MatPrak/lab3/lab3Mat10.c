#include <stdio.h>
#include <stdlib.h>

struct TreeNode 
{
    char data;
    struct TreeNode* parent;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
};

struct TreeNode* createTreeNode(char data) 
{
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if (node != NULL) 
    {
        node->data = data;
        node->parent = NULL;
        node->firstChild = NULL;
        node->nextSibling = NULL;
    }
    return node;
}

void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        struct TreeNode* child = root->firstChild;
        while (child != NULL) {
            struct TreeNode* next_sibling = child->nextSibling;
            freeTree(child);
            child = next_sibling;
        }
        free(root);
    }
}

void addChild(struct TreeNode* parent, struct TreeNode* child) 
{
    if (parent != NULL && child != NULL) 
    {
        child->parent = parent;
        if (parent->firstChild == NULL) 
        {
            parent->firstChild = child;
        } 
        else 
        {
            struct TreeNode* sibling = parent->firstChild;
            while (sibling->nextSibling != NULL) 
            {
                sibling = sibling->nextSibling;
            }
            sibling->nextSibling = child;
        }
    }
}

void printTree(struct TreeNode* node, FILE* output, int depth, int flag) 
{
    if (node != NULL) 
    {
        for (int i = 0; i < depth; i++) 
        {
            fprintf(output, " ");
        } 
        if (node->parent != NULL)
        {
            fprintf(output, "%c\n", node->data);
        }
        struct TreeNode* child = node->firstChild;
        while (child != NULL) 
        {
            printTree(child, output, depth + 1,1);
            child = child->nextSibling;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: path/input_file path/output_file\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");

    if (input_file == NULL && output_file != NULL) {
        printf("Error opening input file.\n");
        fclose(output_file);
        return 1;
    }
    if (input_file != NULL && output_file == NULL)
    {
        printf("Error opening output file.\n");
        fclose(output_file);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), input_file) != NULL) {
        int index = 0;
        struct TreeNode* root = createTreeNode('\0');
        struct TreeNode* current = root;

        while (line[index] != '\0') {
            char c = line[index++];
            if (c == '(') 
            {
                struct TreeNode* child = createTreeNode(' ');
                addChild(current, child);
                current = child;
            } 
            else if (c == ')') 
            {
                current = current->parent;
            } 
            else if (c != ' ' && c != ',' && c != '\n') 
            {
                struct TreeNode* node = createTreeNode(c);
                addChild(current, node);
            }
        }
        printTree(root, output_file, 0, 1);
        fprintf(output_file, "\n");
        freeTree(root);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}

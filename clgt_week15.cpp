#include <iostream>
using namespace std;

const int MAX_C= 10; 
const int NAME_SIZE = 50;  

struct Node {
    char name[NAME_SIZE];     
    int pages;                
    int totalPages;           
    int numC;          
    Node* c[MAX_C];

    Node(const char* n, int p) {
        for (int i = 0; i < NAME_SIZE && n[i] != '\0'; ++i)
            name[i] = n[i];
        name[NAME_SIZE - 1] = '\0'; 
        pages = p;
        totalPages = p;
        numC = 0;
        for (int i = 0; i < MAX_C; ++i)
            c[i] = nullptr;
    }

    void addChild(Node* child) {
        if (numC < MAX_C) {
            c[numC++] = child;
            updatePages();
        }
    }

    void updatePages() {
        totalPages = pages;
        for (int i = 0; i < numC; ++i) {
            if (c[i])
                totalPages += c[i]->totalPages;
        }
    }
};

int countChapters(Node* root) {
    if (!root) return 0;
    return root->numC;
}

Node* findLongestChapter(Node* root) {
    if (!root || root->numC == 0) return nullptr;

    Node* longest = root->c[0];
    for (int i = 1; i < root->numC; ++i) {
        if (root->c[i]->totalPages > longest->totalPages)
            longest = root->c[i];
    }
    return longest;
}

bool deleteNode(Node*& root, const char* targetName) {
    if (!root) return false;

    for (int i = 0; i < root->numC; ++i) {
        if (root->c[i] && strcmp(root->c[i]->name, targetName) == 0) {
            delete root->c[i];
            for (int j = i; j < root->numC - 1; ++j)
                root->c[j] = root->c[j + 1];
            root->c[--root->numC] = nullptr;
            root->updatePages();
            return true;
        }
    }
    for (int i = 0; i < root->numC; ++i) {
        if (deleteNode(root->c[i], targetName)) {
            root->updatePages();
            return true;
        }
    }
    return false;
}

void printTree(Node* root, int level = 0) {
    if (!root) return;
    for (int i = 0; i < level; ++i) cout << "  ";
    cout << "- " << root->name << " (Pages: " << root->pages << ", Total: " << root->totalPages << ")\n";
    for (int i = 0; i < root->numC; ++i) {
        printTree(root->c[i], level + 1);
    }
}

int main() {
    Node* book = new Node("Book", 0);

    Node* chapter1 = new Node("Chapter 1", 10);
    Node* chapter2 = new Node("Chapter 2", 20);
    Node* chapter3 = new Node("Chapter 3", 15);

    book->addChild(chapter1);
    book->addChild(chapter2);
    book->addChild(chapter3);


    chapter1->addChild(new Node("Section 1.1", 5));
    chapter1->addChild(new Node("Section 1.2", 8));

    chapter2->addChild(new Node("Section 2.1", 7));

    cout << "Book :\n";
    printTree(book);

    cout << "\nNumber of chapters: " << countChapters(book) << "\n";

    Node* longestChapter = findLongestChapter(book);
    if (longestChapter)
        cout << "Longest chapter: " << longestChapter->name << " (Total Pages: " << longestChapter->totalPages << ")\n";

    cout << "\nDeleting 'Section 1.2'...\n";
    deleteNode(book, "Section 1.2");
    printTree(book);

    delete book;
    return 0;
}

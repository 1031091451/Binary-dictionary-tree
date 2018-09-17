/*************************************************************************
	> File Name: 二叉字典树2.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年02月27日 星期二 11时24分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 50005

typedef struct Node {
    int freq;
    unsigned char data;
    struct Node *lchild, *rchild;
} Node;

typedef struct trie {
    struct trie *next[2];
    int flag;
} TrieNode, *Trie;

void calculate(unsigned char *str, int *freq) {
    while (str[0]) {
        freq[str[0]]++;
        str++;
    }
}

Node *get_new_node() {
    Node *p = (Node *)malloc(sizeof(Node));
    p->lchild = p->rchild = NULL;
    p->data = 0;
    return p;
}

#define swap(x, y) {\
    __typeof(x)__temp = x;\
    x = y;\
    y = __temp;\
}
Node *build(Node **huffman_node, int n) {
    #define hswap(x, y) {\
        swap(huffman_node[x], huffman_node[y]);\
    }
    Node *root;
    for (int i = 0; i < n - 1; ++i) {
        int ind1 = n - i - 1;
        int ind2 = n - i - 2;
        if (huffman_node[ind1]->freq > huffman_node[ind2]->freq) {
            hswap(ind1, ind2);
        } 
        for (int j = 0; j < n- i - 2; ++j) {
            if (huffman_node[ind1]->freq > huffman_node[j]->freq ) {
                hswap(j, ind1);
                hswap(j, ind2);
            } else if(huffman_node[j] < huffman_node[ind2]){
                hswap(j, ind2);
            }
        }
        root = get_new_node();
        root->lchild = huffman_node[ind1];
        root->rchild = huffman_node[ind2];
        root->freq = huffman_node[ind1]->freq + huffman_node[ind2]->freq;
        huffman_node[ind2] = root;
    }

    #undef hswap
    return root;
}

void get_code(Node *root, int k, char *buffer, unsigned char **huffmancode) {
    if (root == NULL) return ;
    if (root->lchild == NULL) {
        buffer[k] = '\0';
        memcpy(huffmancode[root->data], buffer, strlen(buffer));
        printf("%c -> %s\n", root->data, huffmancode[root->data]);
        return ;
    }
    buffer[k] = '0';
    get_code(root->lchild, k + 1, buffer, huffmancode);
    buffer[k] = '1';
    get_code(root->rchild, k + 1, buffer, huffmancode);
}

TrieNode *init_node() {
    TrieNode *p = (TrieNode *)malloc(sizeof(TrieNode));
    p->next[0] = p->next[1] = NULL;
    p->flag = 0;
    return p;
}

int insert(Trie tree, unsigned char **huffmancode, unsigned char *buffer) {
    TrieNode *p = tree;
    int len = strlen(buffer);
    unsigned char *temp = (unsigned char *)malloc(20);
    int ind = 0, ind1;
    while (buffer[0]) {
        ind1 = 0;
        ind = strlen(huffmancode[buffer[0]]);
        for (int i = 0; i < ind; ++i) {
            temp[ind1] = huffmancode[buffer[0]][i];
            if (p->next[temp[ind1] - '0'] == NULL) {
                p->next[temp[ind1] - '0'] = init_node();
            }
            p = p->next[temp[ind1] - '0'];
            ++ind1;
        }
        temp[ind1] = '\0';
        ++buffer;
    }
    p->flag = 1;
    return 1;
}

int search(TrieNode *root, unsigned char *str, unsigned char **huffmancode) {
    int length = strlen(str);
    int ind = 0, ind1 = 0;
    unsigned char *temp = (char *)malloc(sizeof(char) * 20 * length);
    while(str[0]) {
        ind = strlen(huffmancode[str[0]]);
        for (int i = 0; i < ind; ++i) {
            temp[ind1++] = huffmancode[str[0]][i];
        }
        ++str;
    }
    temp[ind1] = '\0';
    TrieNode *p = root;
    while (temp[0] && p) {
        p = p->next[temp[0] - '0'];
        temp++;
    }
    return p && p->flag;
}

int main() {
    int n; 
    scanf("%d", &n);
    unsigned char **str = (unsigned char **)malloc(sizeof(unsigned char *));
    int *freq = (int *)malloc(sizeof(int) * 256);
    memset(freq, 1, sizeof(int) * 256);
    
    for (int i = 0; i < n; ++i) {
        str[i] = (unsigned char *)malloc(sizeof(unsigned char) * max);
        scanf("%s", str[i]);
        printf("%s\n", str[i]);
        calculate(str[i], freq);
    }
    Node **huffman_node = (Node **)malloc(sizeof(Node *) * 256);
    int ind = 0;
    for (int i = 0; i < 256; ++i) {
        if (freq[i]) {
            huffman_node[ind] = get_new_node();
            huffman_node[ind]->data = (unsigned char)(i);
            huffman_node[ind]->freq = freq[i];
            ind++;
        }
    }
    unsigned char **huffmancode = (unsigned char **)malloc(sizeof(char *) * 256);
    for (int i = 0; i < 256; ++i) {
        huffmancode[i] = (unsigned char *)malloc(sizeof(char) * 20);
        huffmancode[i][0] = '\0';
    }
    printf("ind = %d\n", ind);
    Node *root;
    root = build(huffman_node, ind);
    char encode[20];
    encode[0] = '\0';
    get_code(root, 0, encode, huffmancode);
    TrieNode *tree = init_node();
    int m, t;
    scanf("%d%d", &m, &t);
    unsigned char *buffer = (unsigned char *)malloc(max);
    for (int i = 0; i < m; ++i) {
        scanf("%s", buffer);
        insert(tree, huffmancode, buffer); 
    }
    char *pattern = (char *)malloc(sizeof(char) * 50);
    for (int i = 0; i < t; ++i) {
        scanf("%s", pattern);
        printf("%s ----> ", pattern);
        int ans = search(tree, pattern, huffmancode);
        if (ans == 1) printf("Yes\n");
        else printf("No\n");   
    }
    return 0;
    
}

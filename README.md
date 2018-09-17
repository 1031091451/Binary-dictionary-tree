# Binary-dictionary-tree
节约字典树的存储空间，并且将字符集扩展到任意字符

使用无符号字符存储任意字符
然后读取大文本统计所有字符的频率（一个char占８位，总共用２５６总字符）
然后对这256总字符进行huffman code
这样构建字典树的时候只有o,1两种分之，可以节省大量的空间。

还有一种方式是将字典树设置为孩子、兄弟结构
typedef struct Node {
  char ch;
  struct Node *child;
  struct Node *brother;
} Node;
这样会有一个问题就是查询的效率很低，可能每次都需要遍历一层所有的节点

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
typedef struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int wordCount;
} TrieNode;

typedef struct Trie
{
    struct TrieNode* root;
} Trie;

// Inserts the word to the trie structure
void insert(Trie *pTrie, char *word)
{
    TrieNode* node = pTrie->root;
    while(*word)
    {
        if(*word < 'a' || *word > 'z')
        {
            word++;
            continue;
        }
        int index = *word - 'a';
        if(node->children[index] == NULL)
        {
            node->children[index] = (TrieNode*)malloc(sizeof(TrieNode));
            node->children[index]->wordCount = 0;
            for(int i = 0; i< ALPHABET_SIZE; i++)
            {
                node->children[index]->children[i] = NULL;
            }
        }
        node = node->children[index];
        word++;
    }
    node->wordCount++;
}

// computes the number of occurances of the word
int numberOfOccurances(Trie *pTrie, char *word)
{
    TrieNode* node = pTrie->root;
    while(*word)
    {
        int index = *word - 'a';
        if(node->children[index] == NULL)
        {
            return 0;
        }
        node = node->children[index];
        word++;
    }
    return node->wordCount;
}

void deallocateTrieNodes(TrieNode* node)
{
    if(node != NULL)
    {
        for(int i = 0; i < ALPHABET_SIZE; i++)
        {
            deallocateTrieNodes(node->children[i]);
        }
        free(node);
    }
}

// deallocate the trie structure
Trie *deallocateTrie(Trie *pTrie)
{
    if(pTrie != NULL)
    {
        deallocateTrieNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
Trie *createTrie()
{
    Trie* pTrie = (Trie*)malloc(sizeof(Trie));
    pTrie->root = (TrieNode*)malloc(sizeof(TrieNode));
    pTrie->root->wordCount = 0;
    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        pTrie->root->children[i] = NULL;
    }
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        return 0;
    }
    char word[100];
    int count = 0;
    while(fgets(word, sizeof(word), file) && count < 256)
    {
        word[strcspn(word, "\n")] = 0;
        pInWords[count] = strdup(word);
        count++;
    }
    fclose(file);
    return count;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}
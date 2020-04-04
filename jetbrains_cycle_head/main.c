#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Node {
    int value;
    struct Node *next;
};

struct Element {
    struct Node* pointer;
    short exist;
};

struct HashTable {
    int current_size;
    int element_count;
    struct Element* table;
};

struct Node* newNode(int value) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->value = value;
    node->next = NULL;
    
    return (node);
}

struct Element* newElement(struct Node* pointer) {
    struct Element* temp = (struct Element*)malloc(sizeof(struct Element));
    temp->pointer = pointer;
    temp->exist = 0;
    
    return (temp);
}

struct HashTable* newHashTable(int def_size){
    struct HashTable* temp = (struct HashTable*)malloc(sizeof(struct HashTable));
    temp->current_size = def_size;
    temp->element_count = 0;
    temp->table = (struct Element*)malloc(sizeof(struct Element) * temp->current_size);
    for (int i = 0; i < temp->current_size; ++i){
        temp->table[i].pointer = NULL;
        temp->table[i].exist = 0;
    }
    return (temp);
}

int hash(struct Node* pointer, struct HashTable* table){
    return (int)pointer % table->current_size;
}

double load_factor(struct HashTable* table){
    return table->element_count / (double)table->current_size;
}

void grow(struct HashTable* table);

void insert(struct HashTable* table, struct Node* pointer){
    int i = 0;
    while (table->table[(hash(pointer, table) + i * (i + 1)) % table->current_size].exist == 1){
        ++i;
    }
    table->table[(hash(pointer, table) + i * (i + 1)) % table->current_size].pointer = pointer;
    table->table[(hash(pointer, table) + i * (i + 1)) % table->current_size].exist = 1;
    ++table->element_count;
    if (load_factor(table) >= 0.6){
        grow(table);
    }
}

void grow(struct HashTable* table){
    struct Element buff[table->current_size];
    for (int i = 0; i < table->current_size; ++i){
        buff[i] = table->table[i];
    }
    int buff_size = table->current_size;
    table->current_size *= 2;
    free(table->table);
    table->table = (struct Element*)malloc(sizeof(struct Element) * table->current_size);
    for (int i = 0; i < table->current_size; ++i){
        table->table[i].pointer = NULL;
        table->table[i].exist = 0;
    }
    table->element_count = 0;
    for (int i = 0; i < buff_size; ++i){
        if (buff[i].exist == 1){
            insert(table, buff[i].pointer);
        }
    }
}

short search(struct HashTable* table, struct Node* pointer){
    int i = 0;
    while (table->table[(hash(pointer, table) + i * (i + 1)) % table->current_size].exist == 1){
        if (table->table[(hash(pointer, table) + i * (i + 1)) % table->current_size].pointer == pointer){
            return 1;
        }
        ++i;
    }
    return 0;
}

struct Node* cycleStart(struct Node* head) {
    if (head == NULL){
        return NULL;
    }
    struct HashTable* ht = newHashTable(11);
    struct Node* current_node = head;
    while (current_node->next != NULL){
        if (search(ht, current_node) == 1){
            return current_node;
        }
        else {
            insert(ht, current_node);
            current_node = current_node->next;
        }
    }
    return NULL;
}

int main() {
    struct Node* p = newNode(-1);
    struct Node* c = p;
    for (int i = 0; i < 10; ++i){
        c->next = newNode(i);
        c = c->next;
    }
    c->next = p->next;
    struct Node* k = cycleStart(p);
    printf("%i\n", k->value);
    
    return 0;
}

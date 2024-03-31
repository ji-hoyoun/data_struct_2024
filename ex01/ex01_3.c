#include <stdio.h>
#include <stdlib.h>

// 링크드 리스트 노드 구조체
typedef struct ListNode {
    int data;
    struct ListNode* next;
} ListNode;

// 링크드 리스트 구조체
typedef struct {
    ListNode* tail;
    int numItems;
} CircularLinkedList;

// LRUCache 구조체
typedef struct {
    int capacity;
    CircularLinkedList* cache;
    int cache_hit;
    int total_refer;
} LRUCache;

// CircularLinkedList 초기화 함수
void initialize(CircularLinkedList* list) {
    list->tail = NULL;
    list->numItems = 0;
}

// CircularLinkedList에 데이터 추가하는 함수
void append(CircularLinkedList* list, int newData) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = newData;
    if (list->tail == NULL) {
        list->tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = list->tail->next;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->numItems++;
}

// CircularLinkedList에서 데이터 삭제하는 함수
void removeNode(CircularLinkedList* list, int data) {
    if (list->tail == NULL)
        return;
    ListNode* prev = list->tail;
    ListNode* curr = list->tail->next;
    do {
        if (curr->data == data) {
            if (curr == list->tail && list->numItems == 1) {
                free(curr);
                list->tail = NULL;
                list->numItems--;
                return;
            } else if (curr == list->tail) {
                prev->next = curr->next;
                list->tail = prev;
                free(curr);
                list->numItems--;
                return;
            } else {
                prev->next = curr->next;
                free(curr);
                list->numItems--;
                return;
            }
        }
        prev = curr;
        curr = curr->next;
    } while (curr != list->tail->next);
}

// LRUCache 초기화 함수
void LRUCache_init(LRUCache* lru, int capacity) {
    lru->capacity = capacity;
    lru->cache = (CircularLinkedList*)malloc(sizeof(CircularLinkedList));
    initialize(lru->cache);
    lru->cache_hit = 0;
    lru->total_refer = 0;
}

// LRUCache에 데이터 추가 및 관리 함수
void refer(LRUCache* lru, int page) {
    lru->total_refer++;
    ListNode* curr = lru->cache->tail == NULL ? NULL : lru->cache->tail->next;
    while (curr != NULL) {
        if (curr->data == page) {
            removeNode(lru->cache, page);
            lru->cache_hit++;
            break;
        }
        if (curr == lru->cache->tail)
            break;
        curr = curr->next;
    }
    if (lru->cache->numItems == lru->capacity) {
        removeNode(lru->cache, lru->cache->tail->data);
    }
    append(lru->cache, page);
}

// LRUCache의 통계 출력 함수
void print_stats(LRUCache* lru) {
    float hit_ratio = lru->total_refer > 0 ? (float)lru->cache_hit / lru->total_refer : 0;
    printf("cache_slot = %d cache_hit = %d hit ratio = %.5f\n", lru->capacity, lru->cache_hit, hit_ratio);
}

int main() {
    FILE* data_file = fopen("./linkbench.trc", "r");
    if (data_file == NULL) {
        printf("Error: Failed to open the file.\n");
        return 1;
    }
    
    char line[256];
    LRUCache lru;
    for (int cache_capacity = 100; cache_capacity <= 1000; cache_capacity += 100) {
        LRUCache_init(&lru, cache_capacity);
        while (fgets(line, sizeof(line), data_file) != NULL) {
            int page = atoi(strtok(line, " "));
            refer(&lru, page);
        }
        print_stats(&lru);
        rewind(data_file);
    }

    fclose(data_file);
    return 0;
}
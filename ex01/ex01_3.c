#include <stdio.h>
#include <stdlib.h>

// ListNode 구조체 정의
typedef struct ListNode {
    int data;
    struct ListNode* next;
} ListNode;

// CircularLinkedList 구조체 정의
typedef struct CircularLinkedList {
    ListNode* tail;
    int numItems;
} CircularLinkedList;

// CircularLinkedList 초기화 함수
void initCircularLinkedList(CircularLinkedList* list) {
    list->tail = NULL;
    list->numItems = 0;
}

// CircularLinkedList에 새로운 노드를 추가하는 함수
void appendCircularLinkedList(CircularLinkedList* list, int newData) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = newData;
    if (list->tail == NULL) {
        newNode->next = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->tail->next;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->numItems++;
}

// CircularLinkedList에서 노드를 제거하는 함수
void removeCircularLinkedList(CircularLinkedList* list, int target) {
    if (list->tail == NULL) return;
    ListNode* curr = list->tail->next;
    ListNode* prev = list->tail;
    while (curr != list->tail) {
        if (curr->data == target) {
            prev->next = curr->next;
            if (curr == list->tail->next) {
                list->tail->next = curr->next;
            }
            free(curr);
            list->numItems--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    if (curr->data == target) {
        if (list->numItems == 1) {
            free(curr);
            list->tail = NULL;
        } else {
            prev->next = curr->next;
            if (curr == list->tail) {
                list->tail = prev;
            }
            free(curr);
        }
        list->numItems--;
    }
}

// LRUCache 구조체 정의
typedef struct LRUCache {
    int capacity;
    CircularLinkedList cache;
    int cache_hit;
    int total_refer;
} LRUCache;

// LRUCache 초기화 함수
void initLRUCache(LRUCache* cache, int capacity) {
    cache->capacity = capacity;
    initCircularLinkedList(&(cache->cache));
    cache->cache_hit = 0;
    cache->total_refer = 0;
}

// LRUCache에 페이지를 참조하는 함수
void referLRUCache(LRUCache* cache, int page) {
    cache->total_refer++;
    ListNode* curr = cache->cache.tail ? cache->cache.tail->next : NULL;
    ListNode* prev = cache->cache.tail;
    while (curr != cache->cache.tail) {
        if (curr->data == page) {
            removeCircularLinkedList(&(cache->cache), page);
            appendCircularLinkedList(&(cache->cache), page);
            cache->cache_hit++;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    if (curr && curr->data == page) {
        removeCircularLinkedList(&(cache->cache), page);
        appendCircularLinkedList(&(cache->cache), page);
        cache->cache_hit++;
    } else {
        if (cache->cache.numItems == cache->capacity) {
            ListNode* toRemove = cache->cache.tail->next;
            cache->cache.tail->next = toRemove->next;
            if (toRemove == cache->cache.tail) {
                cache->cache.tail = cache->cache.tail->next;
            }
            free(toRemove);
            cache->cache.numItems--;
        }
        appendCircularLinkedList(&(cache->cache), page);
    }
}

// LRUCache의 통계 정보를 출력하는 함수
void printLRUCacheStats(LRUCache* cache) {
    float hit_ratio = (float)cache->cache_hit / cache->total_refer;
    printf("cache_slot = %d cache_hit = %d hit ratio = %.5f\n", cache->capacity, cache->cache_hit, hit_ratio);
}

int main() {
    FILE* data_file = fopen("./linkbench.trc", "r");
    if (data_file == NULL) {
        printf("Error: Unable to open file.\n");
        return 1;
    }

    char buffer[256];
    int page;
    LRUCache lru_cache;

    // 캐시 크기를 100에서 1000까지 변화시키면서 테스트
    for (int cache_capacity = 100; cache_capacity <= 1000; cache_capacity += 100) {
        initLRUCache(&lru_cache, cache_capacity);
        while (fgets(buffer, sizeof(buffer), data_file)) {
            sscanf(buffer, "%d", &page);
            referLRUCache(&lru_cache, page);
        }
        printLRUCacheStats(&lru_cache);
        rewind(data_file);
    }

    fclose(data_file);
    return 0;
}
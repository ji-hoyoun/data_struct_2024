from circularLinkedList import CircularLinkedList

class LRUCache:
    def __init__(self, capacity):
        self.capacity = capacity
        self.cache = CircularLinkedList()
        self.cache_hit = 0
        self.total_refer = 0

    def refer(self, page):
        self.total_refer += 1
        if self.cache.count(page) > 0:
            # 페이지가 이미 캐시에 있는 경우, 해당 페이지를 맨 앞으로 이동
            self.cache.remove(page)
            self.cache_hit += 1
        else:
            if self.cache.size() == self.capacity:
                # 캐시가 꽉 찬 경우, 가장 오래된 페이지를 제거
                self.cache.pop(0)
        # 새 페이지를 캐시에 추가
        self.cache.append(page)

    def print_stats(self):
        hit_ratio = self.cache_hit / self.total_refer if self.total_refer > 0 else 0
        print("cache_slot =", self.capacity, "cache_hit =", self.cache_hit, "hit ratio =", hit_ratio)

if __name__ == "__main__":
    # 트레이스 파일을 읽고 캐시에 대한 테스트를 수행
    data_file = open("./linkbench.trc")
    lines = data_file.readlines()

    # 캐시 크기를 100에서 1000까지 변화시키면서 테스트
    for cache_capacity in range(100, 1001, 100):
        lru_cache = LRUCache(cache_capacity)
        for line in lines:
            page = int(line.split()[0])
            lru_cache.refer(page)
        lru_cache.print_stats()
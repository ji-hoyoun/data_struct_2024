

class CacheSimulator:
    def __init__(self, cache_slots):
        self.cache_slots = cache_slots
        self.cache = []
        self.cache_map = {}
        self.cache_hit = 0
        self.cache_miss = 0
        self.tot_cnt = 1
    
    def do_sim(self, page):
        self.tot_cnt += 1
        
        # 페이지가 캐시에 있는지 확인
        if page in self.cache_map:
            # 캐시 히트
            self.cache_hit += 1
            # 캐시에서 해당 페이지 제거
            self.cache.remove(page)
        else:
            # 캐시 미스
            self.cache_miss += 1
            # 캐시가 가득 찼는지 확인
            if len(self.cache) == self.cache_slots:
                # LRU 페이지 제거
                lru_page = self.cache.pop(0)
                del self.cache_map[lru_page]
        
        # 새 페이지를 캐시에 추가하고 가장 최근에 사용된 것으로 갱신
        self.cache.append(page)
        self.cache_map[page] = True
    
    def print_stats(self):
        hit_ratio = self.cache_hit / self.tot_cnt
        print("cache_slot =", self.cache_slots, "cache_hit =", self.cache_hit, "hit ratio =", hit_ratio)


if __name__ == "__main__":

    data_file = open("./linkbench.trc")
    lines = data_file.readlines()
    for cache_slots in range(100, 1001, 100):
        cache_sim = CacheSimulator(cache_slots)
        for line in lines:
            page = line.split()[0]
            cache_sim.do_sim(page)
        
        cache_sim.print_stats()
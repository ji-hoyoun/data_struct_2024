from listNode import ListNode

class CircularLinkedList:
    def __init__(self):
        self.__tail = ListNode('dummy', None)
        self.__tail.next = self.__tail
        self.__numItems = 0

    def insert(self, i:int, newItem):
        if i >= 0 and i <= self.__numItems:
            prev = self.__getNode(i - 1)
            newNode = ListNode(newItem, prev.next)
            prev.next = newNode
            self.__numItems += 1
            if i == self.__numItems - 1:
                self.__tail = newNode
        else:
            print("index", i, ": out of bound in insert()")

    def append(self, newItem):
        newNode = ListNode(newItem, self.__tail.next)
        self.__tail.next = newNode
        self.__tail = newNode
        self.__numItems += 1

    def pop(self, i:int):
        if (i<0):
            i = ((i*-1)%self.__numItems)*-1+self.__numItems
        if (i > self.__numItems - 1):
            i = i%self.__numItems
        prev = self.__getNode(i-1)
        curr = prev.next
        prev.next = curr.next
        retItem = curr.item
        if i == self.__numItems - 1:
            self.__tail = prev
        self.__numItems -= 1
        return retItem

    def remove(self, x):
        (prev, curr) = self.__findNode(x)
        if curr != None:
            prev.next = curr.next
            if curr == self.__tail:
                self.__tail = prev
            self.__numItems -= 1
            return x
        else:
            return None

    def get(self, i:int):
        if self.isEmpty():
            return None
        if (i >= 0 and i <= self.__numItems - 1):
            return self.__getNode(i).item
        else:
            return None

    def index(self, x) -> int:
        curr = self.__tail.next
        for index in range(self.__numItems):
            if curr.item == x:
                return index
            else:
                curr = curr.next
        return -2

    def isEmpty(self) -> bool:
        return self.__numItems == 0

    def size(self) -> int:
        return self.__numItems

    def clear(self):
        self.__tail = ListNode("dummy", None)
        self.__tail.next = self.__tail
        self.__numItems = 0

    def count(self, x) -> int:
        cnt = 0
        curr = self.__tail.next
        while curr != self.__tail:
            if curr.item == x:
                cnt += 1
            curr = curr.next
        if curr.item == x:
            cnt += 1
        return cnt

    def extend(self, a):
        for index in range(a.size()):
            self.append(a.get(index))

    def copy(self):
        a = CircularLinkedList()
        for index in range(self.__numItems):
            a.append(self.get(index))
        return a

    def reverse(self):
        a = CircularLinkedList()
        for index in range(self.__numItems):
            a.insert(0, self.get(index))
        self.clear()
        for index in range(a.size()):
            self.append(a.get(index))

    def sort(self) -> None:
        a = []
        for index in range(self.__numItems):
            a.append(self.get(index))
        a.sort()
        self.clear()
        for index in range(len(a)):
            self.append(a[index])

    def __findNode(self, x) -> (ListNode, ListNode):
        prev = self.__tail
        curr = prev.next
        while curr != self.__tail:
            if curr.item == x:
                return (prev, curr)
            else:
                prev = curr
                curr = curr.next
        if curr.item == x:
            return (prev, curr)
        return (None, None)

    def __getNode(self, i:int) -> ListNode:
        curr = self.__tail.next
        for index in range(i):
            curr = curr.next
        return curr

    def printList(self):
        curr = self.__tail.next
        while curr != self.__tail:
            print(curr.item, end=' ')
            curr = curr.next
        print(curr.item)

    def __iter__(self):
        return CircularLinkedListIterator(self.__tail.next)


class CircularLinkedListIterator:
    def __init__(self, start):
        self.current = start
        self.start = start

    def __iter__(self):
        return self

    def __next__(self):
        if self.current.next == self.start:
            raise StopIteration
        else:
            item = self.current.item
            self.current = self.current.next
            return item
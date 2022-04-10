#ifndef __LIST_H__
#define __LIST_H__
#include <string>

namespace saber_test
{
// структуру ListNode модифицировать нельзя
struct ListNode
{
    ListNode*   next;
    ListNode*   prev;
    ListNode*   rand;
    std::string data;
};
class List
{
public:
    List()
        : count(0)
    {
    }
    List(const List&)   = delete;
    List&       operator=(const List&) = delete;
    List&       operator=(List&&) = default;
    friend bool operator==(const List& lhs, const List& rhs);
    List(List&&) = default;
    ~List();

    /// @brief serializes given list to file.
    /// [{rand_id(uint32_t) - id of random node, data_size (uint32_t), data - data of node}]
    /// @param file Name of file to write to.
    void Serialize(FILE* file);   // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

    static List generateLinkedList(int count = 10);
    friend void testWorkOnList(); // testing purposes

private:
    ListNode* head;
    ListNode* tail;
    int       count;
    void      applyToEachNode(std::function<void(ListNode*)> func) const;
};
void testWorkOnList();
} // namespace saber_test
#endif // __LIST_H__
#include "list.h"

#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>
namespace saber_test
{

List::List(List&& rhs)
    : head(std::exchange(rhs.head, nullptr))
    , tail(std::exchange(rhs.tail, nullptr))
    , count(std::exchange(rhs.count, 0))
{
}

List::~List()
{
    ListNode* current = head;
    while (current)
    {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}
bool operator==(const List& lhs, const List& rhs)
{
    if (lhs.count != rhs.count)
        return false;
    auto* lhh = lhs.head;
    auto* rhh = rhs.head;
    while (lhh and rhh)
    {
        if (lhh->data != rhh->data)
            return false;
        if (lhh->rand and rhh->rand)
            if (lhh->rand->data != rhh->rand->data)
            {
                std::cout << "\t " << lhh->rand->data << " != " << rhh->rand->data << std::endl;
                return false;
            }
        lhh = lhh->next;
        rhh = rhh->next;
    }
    return true;
}
List List::generateLinkedList(int count)
{
    List list;

    std::unordered_map<int, ListNode*> map;
    ListNode*                          first = new ListNode;
    list.head = list.tail = first;
    first->data           = "first-node string";
    map[0]                = first;
    for (int i = 1; i < count; ++i)
    {
        ListNode* node = new ListNode;
        node->data     = "payload purpose data string_" + std::to_string(i);

        list.tail->next = node;
        node->prev      = list.tail;
        list.tail       = node;

        map[i] = node;
    }

    ListNode*        current = list.head;
    std::vector<int> randoms(count);
    std::iota(randoms.begin(), randoms.end(), 1);

    std::shuffle(randoms.begin(), randoms.end(), std::mt19937{ std::random_device{}() });
    for (int i = 0; i < count; ++i)
    {
        current->rand = map[randoms[i]];
        current       = current->next;
    }
    list.count = count;
    return list;
}

void List::Deserialize(FILE* file)
{
    if (not file)
        return;

    struct NodeHeader
    {
        uint32_t rand_id;
        uint32_t data_size;
    } sHeader;

    uint32_t node_id = 1;

    std::unordered_map<uint32_t, ListNode*> idToPtrMap;
    idToPtrMap[node_id] = new ListNode;
    head                = idToPtrMap[node_id];
    tail                = idToPtrMap[node_id];

    auto appendNode = [&](std::string&& str, uint32_t rand_id) {
        ListNode* node = nullptr;
        if (not idToPtrMap[node_id])
            idToPtrMap[node_id] = new ListNode;

        node       = idToPtrMap[node_id];
        node->data = std::forward<std::string>(str);

        tail->next = node;
        node->prev = tail;
        tail       = node;

        if (rand_id && not idToPtrMap[rand_id])
            idToPtrMap[rand_id] = new ListNode;

        node->rand = idToPtrMap[rand_id];
    };

    while (fread(&sHeader, sizeof(sHeader), 1, file) == 1)
    {
        std::string readStr;
        readStr.resize(sHeader.data_size);

        fread(readStr.data(), sHeader.data_size, 1, file);
        appendNode(std::move(readStr), sHeader.rand_id);
        ++node_id;
    }
    count = node_id - 1;
}
void List::applyToEachNode(std::function<void(ListNode*)> func) const
{
    ListNode* current = head;
    while (current)
    {
        func(current);
        current = current->next;
    }
}
void List::Serialize(FILE* file)
{
    if (not file)
        return;

    std::unordered_map<ListNode*, uint32_t> ptrToIdMap;

    uint32_t node_id = 1;
    applyToEachNode([&ptrToIdMap, &node_id](ListNode* node) {
        ptrToIdMap[node] = node_id++;
    });

    applyToEachNode([file, &ptrToIdMap](ListNode* node) {
        fwrite(&ptrToIdMap[node->rand], sizeof(uint32_t), 1, file);
        uint32_t data_size = node->data.size();
        fwrite(&data_size, sizeof(uint32_t), 1, file);
        fwrite(node->data.c_str(), sizeof(char), data_size, file);
    });
}

void printLinkedList(ListNode* node)
{
    int i = 0;
    while (node)
    {
        std::cout << "node " << i++ << " " << node->data << " -> " << ((node->rand == nullptr) ? "null" : node->rand->data) << "\n";
        node = node->next;
    }
    std::cout << std::endl;
}

void testWorkOnList()
{
    List list = List::generateLinkedList(5);

    std::cout << "list before serialization: " << list.count << "\n";

    FILE* file;
    file = fopen("serialized_list", "wb");
    list.Serialize(file);
    fclose(file);

    List listRead;
    file = fopen("serialized_list", "rb");
    listRead.Deserialize(file);
    fclose(file);
    std::cout << "list after deserialize: " << listRead.count << "\n";

    assert(listRead == list);
    std::cout << "Succeeded serialization and deserialization\n";
}
}; // namespace saber_test

#include<iostream>
#include<conio.h>
#include<fstream>
#include<windows.h>
using namespace std;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

template<class T>
class DoublyLinkedList {
    struct Node {
        T val;
        Node* next;
        Node* prev;
        Node() : next(nullptr), prev(nullptr) {}
        Node(T val, Node* nptr = nullptr, Node* pptr = nullptr) : val(val), next(nptr), prev(pptr) {}
    };

    Node* head;
    Node* tail;
    int size;
    int cursorpos;
    Node* cursor;
public:
    DoublyLinkedList() : size(0) {
        head = new Node;
        tail = head;
        cursorpos = 0;
        cursor = head->next;
    }
    void insertAtTail(T val) {
        Node* newnode = new Node(val, nullptr, tail);
        tail->next = newnode;
        tail = newnode;
        size++;
    }
    void insertBeforeNode(Node* node, T val) {
        if (node == nullptr)
            return;

        Node* newnode = new Node(val, node, node->prev);// Because of dummy node there is no 
        node->prev->next = newnode;                   // need of check of(if node->prev != nullptr)
        node->prev = newnode;                         // because if in the node there is value of our
        size++;                                       // head than in its prev there will be dummy not nullptr 
        // due to which we also dont need insertathead() function
    }
    void deleteATHead() {
        if (size == 0)
            return;

        Node* temp = head->next;
        head->next = temp->next;
        if (head->next != nullptr) {
            head->next->prev = head;
        }
        else {
            tail = head;
        }
        delete temp;
        size--;
    }
    void deleteATTail() {
        if (size == 0) return;

        Node* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }
        else {
            head->next = nullptr;
        }
        delete temp;
        size--;
    }
    void deleteNode(Node* nptr) {
        if (nptr == nullptr)
            return;

        if (nptr == head->next) {
            deleteATHead();
        }
        else if (nptr == tail) {
            deleteATTail();
        }
        else {
            nptr->prev->next = nptr->next;
            if (nptr->next != nullptr) {
                nptr->next->prev = nptr->prev;
            }
            delete nptr;
            size--;
        }
    }
    class Iterator
    {
        Node* ptr;
    public:
        Iterator(Node* p) : ptr(p)
        {}
        Iterator& operator ++() {
            ptr = ptr->next;
            return *this;
        }
        Iterator& operator++(int) {
            Iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        Iterator& operator --() {
            ptr = ptr->prev;
            return *this;
        }
        Iterator& operator--(int) {
            Iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        T& operator *() const
        {
            return ptr->data;
        }
        bool operator != (const Iterator& other) const {
            return !(ptr == other.ptr);
        }
    };
    Iterator begin() {
        return Iterator(head->next);
    }
    Iterator end() {
        return Iterator(tail->next);
    }
    Iterator r_begin() {
        return Iterator(tail);
    }
    Iterator r_end() {
        return Iterator(head);
    }
    void printdllwithcursor(int currpos) {
        int index = 0;
        Node* temp = head->next;
        for (Iterator it = begin(); it != end(); ++it) {
            if (index == currpos) {
                SetConsoleTextAttribute(h, 9);
                cout << "|";
                SetConsoleTextAttribute(h, 15);
            }
            cout << temp->val;
            temp = temp->next;
            index++;
        }
        if (currpos == size) {
            SetConsoleTextAttribute(h, 9);
            cout << "|";
            SetConsoleTextAttribute(h, 15);
        }

        cout << endl;
        SetConsoleTextAttribute(h, 6);
        cout << "Cursor Position: " << cursorpos << " / " << size << endl;
        SetConsoleTextAttribute(h, 15);
    }
    void saveToFile() {
        cout << "\nEnter the name of your file from which you can open it next time ";
        string filename;
        SetConsoleTextAttribute(h, 4);
        cin >> filename;
        SetConsoleTextAttribute(h, 15);
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
        Node* temp = head->next;
        for (Iterator it = begin(); it != end(); ++it) {
            file << temp->val;
            temp = temp->next;
        }
        SetConsoleTextAttribute(h, 6);
        cout << filename << ".txt" << " is saved in this notepad you can open it whenever you want by writing its name ";
        Sleep(3000);
        SetConsoleTextAttribute(h, 15);
        file.close();
    }
    void loadFromFile() {
        SetConsoleTextAttribute(h, 4);
        cout << "Enter the filename to open: ";
        SetConsoleTextAttribute(h, 15);
        string filename;
        cin >> filename;
        cursor = tail;
        cursorpos = size;
        ifstream file(filename);
        if (!file.is_open()) {
            SetConsoleTextAttribute(h, 6);
            cout << "This File does not exist " << endl;
            Sleep(1000);
            SetConsoleTextAttribute(h, 15);
            simplenotepad();
        }
        T value;
        while (file.get(value)) {
            insertAtTail(value);
            cursorpos++;
        }
        cursor = tail;
        cursorpos = size;
        printdllwithcursor(cursorpos);
        file.close();
        SetConsoleTextAttribute(h, 6);
        cout << "File opened successfully You can modify it " << endl;
        Sleep(2000);
        SetConsoleTextAttribute(h, 15);
    }
    void simplenotepad() {
        char ch;
        cout << "If you want to open an existing file then press 1 and if you want to create a new file then press 2 ";
        char choice;
        cin >> choice;
        if (choice != '1' && choice != '2') {
            SetConsoleTextAttribute(h, 4);
            cout << "Invalid Please press 1 or 2\n";
            SetConsoleTextAttribute(h, 15);
            simplenotepad();
        }

        if (choice == '1') {
            loadFromFile();
        }
        else if (choice == '2') {
            SetConsoleTextAttribute(h, 6);
            cout << "You have created a new file Successfully ";
            Sleep(1000);
            SetConsoleTextAttribute(h, 15);
        }
        while (true) {
            system("cls");
            printdllwithcursor(cursorpos);
            SetConsoleTextAttribute(h, 4);
            cout << "Press ESC to exit and Press CTRL+S to save ";
            SetConsoleTextAttribute(h, 15);
            ch = _getch();
            if (ch == 27) {
                SetConsoleTextAttribute(h, 15);
                cout << " \nThank you for using our notepad ";
                break;
            }
            else if (ch == 19) {
                saveToFile();
            }
            else if (ch == 8 && size > 0) {
                if (cursorpos > 0) {
                    Node* temp = cursor->prev;
                    deleteNode(cursor);
                    cursor = temp;
                    cursorpos--;
                }
            }
            else if (ch == -32 || ch == 224) {
                ch = _getch();
                if (ch == 75 && cursorpos > 0) {
                    cursor = cursor->prev;
                    cursorpos--;
                }
                else if (ch == 77 && cursorpos < size) {
                    cursor = cursor->next;
                    cursorpos++;
                }
            }
            else if (ch == 13) {
                if (cursorpos == size) {
                    insertAtTail('\n');
                    cursor = tail;
                }
                else {
                    insertBeforeNode(cursor->next, '\n');
                    cursor = cursor->next;
                }
                cursorpos++;
            }
            else {
                if (cursorpos == size) {
                    insertAtTail(ch);
                    cursor = tail;
                }
                else {
                    insertBeforeNode(cursor->next, ch);
                    cursor = cursor->next;
                }
                cursorpos++;
            }
        }
    }
    ~DoublyLinkedList()
    {
        Node* curr = head;
        Node* prev;
        while (curr != nullptr)
        {
            prev = curr;
            curr = curr->next;
            delete prev;
        }
    }
};
int main() {
    DoublyLinkedList<char> dll;
    dll.simplenotepad();
    return 0;
}
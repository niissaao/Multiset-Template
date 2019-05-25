#include <iostream>
#include <string.h>
#include <assert.h>
using namespace std;
template < class T > class Multiset
{
    int m_uniqueSize;
    int m_size;
    struct Node //structura pentru nod
    {
        T data;
        int nr = 0;
        Node* next;
        Node* prev;
    };
    Node* m_head;
public:
    Multiset(); //constructor fara parametri care initializeaza un multiset gol
    Multiset(const Multiset& other); //constructor de copiere
    Multiset& operator=(const Multiset& ob); //operatorul de atribuire
    void insert(const T& value); //metoda pentru adaugare element in multiset
    void erase(const T& value); //metoda pentru stergere element din multiset
    int count(const T& value) const; //metoda care intoarce numarul de aparitii ale unui element
    bool contains(const T& value) const; //metoda care verifica daca un element se afla in multiset
    void eraseAll(const T& value); //metoda care elimina toate aparitiile unui element din multiset
    int uniqueSize() const; //metoda care intoarce numarul de elemente distincte din multiset
    friend ostream &operator<< (ostream&, Multiset < T > const&); //supraincarcarea operatorului de afisare
    ~Multiset(); //destructor
};

template < class T >
Multiset < T > ::Multiset()
{
    m_uniqueSize = 0;
    m_size = 0;
    m_head = new Node;
    m_head->next = m_head;  //nod fictiv
    m_head->prev = m_head;
}

template < class T >
Multiset < T > ::Multiset(const Multiset& ob)
{
    m_size = ob.m_size;
    m_uniqueSize = ob.m_uniqueSize;

    m_head = new Node;
    m_head->next = m_head;
    m_head->prev = m_head;   //nod fictiv

    Node* p = m_head;
    Node* p2 = ob.m_head->next;

    while(p2 != ob.m_head)
    {
        p->next = new Node;
        p->next->data = p2->next->data;
        p->next->nr = p2->next->nr;
        p->next->prev = p;
        p2 = p2->next;
        p = p->next;
    }
    p->next = m_head;
    m_head->prev = p;
}

template < class T >
Multiset < T > & Multiset < T > ::operator=(const Multiset& ob)
{
    if(this != &ob)
    {
        //deletion part incase you are copying into something of bigger size
        Node* m_p = m_head->next;
        while(m_p != m_head)
        {
            m_head->next = m_p ->next;
            delete m_p;
            m_p = m_head->next;
        }
        delete m_head;

        //now it wil copy everything over
        m_size = ob.m_size;
        m_uniqueSize = ob.m_uniqueSize;

        m_head = new Node;
        m_head->next = m_head;      //creating new node (dummy)
        m_head->prev = m_head;

        Node* p = m_head;
        Node* p2 = ob.m_head->next;

        while(p2 != ob.m_head)
        {
            p->next = new Node;
            p->next->data = p2->data;
            p->next->nr = p2->nr;
            p->next->prev = p;
            p2 = p2->next;
            p = p->next;
        }
        p->next = m_head;
        m_head->prev = p;
    }
    return *this;
}

template < class T >
void Multiset < T > ::insert(const T& value)
{
    Node* p;
    for(p = m_head->next; p != m_head; p = p->next)
    {
        if(value == p->data)
        {
            p->nr++;
            m_size++;
            return;
        }
    }                                   //loop to see if there is already a value
    if(m_size == 0)
    {
        Node* firstnode = new Node;
        firstnode->data = value;
        firstnode->next = m_head;
        m_head->next = firstnode;
        m_head->prev = firstnode;           //create the first node
        firstnode->prev = m_head;
        firstnode->nr++;
        m_uniqueSize++;
        m_size++;
    }
    else
    {
        Node * newnode = new Node;
        newnode->data = value;
        newnode->next = m_head;
        m_head->prev->next = newnode;
        m_head->prev = newnode;
        newnode->prev = m_head->prev;             //create the next nodes
        newnode->nr++;
        m_uniqueSize++;
        m_size++;
    }
}

template < class T >
void Multiset < T > ::erase(const T& value)
{
    Node* p;
    for(p = m_head->next; p != m_head; p = p->next)
    {
        if(p->data == value)
        {
            p->nr--;
            m_size--;
            if(p->nr == 0)          //daca era singurul, sterge nodul
            {
                p->prev->next = p->next;
                p->next->prev = p->prev;
                m_uniqueSize--;
            }
        }
    }
}

template < class T >
int Multiset < T > ::count(const T& value) const
{
    Node* p = m_head->next;
    while(p != m_head)
    {
        if(p->data == value)
            return p->nr;
        p = p->next;
    }
    return 0;
}

template < class T >
bool Multiset < T > ::contains(const T& value) const
{
    Node* p = m_head->next;
    while(p != m_head)
    {
        if(p->data == value)
            return true;
        p = p->next;
    }
    return false;
}

template < class T >
void Multiset < T > ::eraseAll(const T& value)
{
    Node* p;
    for(p = m_head->next; p != m_head; p = p->next)
    {
        if(p->data == value)
        {
            int m_removed = p->nr;
            p->prev->next = p->next;
            p->next->prev = p->prev;
            p = p->next;
            m_uniqueSize--;
            m_size -= m_removed;
        }
    }
}

template < class T >
int Multiset < T > ::uniqueSize() const
{
    return m_uniqueSize;
}

template < class T >
ostream &operator<<(ostream& output, Multiset < T > const& m)
{
    typename Multiset< T >::Node *p = m.m_head->next;
    int i;
    for(i = 0; i < m.m_size; i++)
    {
        output << p->data << " ";
        p = p->next;
    }
    output << endl;
    return output;
}
ostream &operator<<(ostream& output, Multiset < int > const& m)
{
    typename Multiset< int >::Node *p = m.m_head->next;
    int i;
    for(i = 0; i < m.m_size; i++)
    {
        output << p->data << " ";
        p = p->next;
    }
    output << endl;
    return output;
}
ostream &operator<<(ostream& output, Multiset < float > const& m)
{
    typename Multiset< float >::Node *p = m.m_head->next;
    int i;
    for(i = 0; i < m.m_size; i++)
    {
        output << p->data << " ";
        p = p->next;
    }
    output << endl;
    return output;
}
ostream &operator<<(ostream& output, Multiset < char > const& m)
{
    typename Multiset< char >::Node *p = m.m_head->next;
    int i;
    for(i = 0; i < m.m_size; i++)
    {
        output << p->data << " ";
        p = p->next;
    }
    output << endl;
    return output;
}

template < class T >
Multiset < T > ::~Multiset()
{
    Node* p = m_head->next;
    while(p != m_head)
    {
        m_head->next = p->next;
        delete p;
        p = m_head->next;
    }
    delete m_head;
}
int main()
{
    Multiset < int > m, m2;
    //se insereaza elementele:
    m.insert('9');
    m.insert('9');
    m.insert('9');
    m.insert('9');
    m.insert('8');
    m.insert('7');
    m.insert('6');
    cout << "Multisetul m: ";
    cout << m << endl;
    cout << "Multisetul m contine elementul 8? ";
    if(m.contains('8') == 1)
        cout << "Da. " << endl;
    else
        cout << "Nu. " << endl;
    cout << "Multisetul m contine elementul 25? ";
    if(m.contains('25') == 1)
        cout << "Da. " << endl;
    else
        cout << "Nu. " << endl;
    cout << "In multisetul m exista " << m.count('9') << " aparitii ale elementului 9." << endl;
    m.erase('9');
    cout << "S-a sters un 9." << endl;
    cout << "In multisetul m exista " << m.count('9') << " aparitii ale elementului 9." << endl;
    m.eraseAll('9');
    cout << "S-au sters toate aparitiile lui 9." << endl;
    cout << "In multisetul m exista " << m.count('9') << " aparitii ale elementului 9." << endl;
    cout << "In multisetul m exista " << m.uniqueSize() << " elemente distincte." << endl;
    m2 = m;
    cout << "S-a copiat multisetul m in multisetul m2." << endl;
    cout << "Multisetul m2 contine elementul 7? ";
    if(m2.contains('7') == 1)
        cout << "Da. " << endl;
    else
        cout << "Nu. " << endl;
    //se mai insereaza elemente in multisetul m2
    m2.insert('6');
    m2.insert('6');
    cout << "In multisetul m2 exista " << m2.count('6') << " aparitii ale elementului 6." << endl;
    //cout << m2;
    return 0;
}

#include <iostream>
#include <string>
using namespace std;

/*single link list*/
template <class T>
struct LNode
{
	T data;
	LNode<T> *next;
};

template <class T>
class LinkList {
	LNode<T> *head;
public:
	LinkList(){
		head = new LNode<T>;
		head->next = NULL;
	}
	LinkList(T a[], int size){
		head = new LNode<T>;
		LNode<T> *p = head;
		for (int i = 0; i < size; ++i) {
			LNode<T> *s = new LNode<T>;
			s->data = a[i];
			p->next = s;// insert element in the rear
			p = s;
		}
		p->next = NULL;
	}
	~LinkList(){
		LNode<T> *p = head;
		while(p)
		{
			LNode<T> *q = p;
			p = p->next;
			delete q;
		}
		head = NULL;
	}
	int size(){
		int sum = 0;
		if (head->next==NULL)
		    return sum;
		LNode<T> *p = head->next;
		while(p)
		{
			p = p->next;
			sum++;
		}
		return sum;
	}
	T get(int i){
		LNode<T> *p = head->next;
		while(--i && p)
		{
			p = p->next;
		}
		if (!p||i<0)
		{
			cerr<<"cannot get element "<<i<<endl;
			exit(1);
		} else {
			return p->data;
		}
	}
	int locate(T x){
		LNode<T> *p = head;
		int l = 0;
		while(++l && p)
		{
			if (p->data == x)
				return l;
			p = p->next;
		}
		cerr<<"cannot find the element"<<endl;
		exit(1);
	}
	void Insert(int i, T item){
		LNode<T> *p = head;
		int j = 0;
		while(p && j<i-1)
		{
			p = p->next;
			j++;
		}
		if (!p) {
			cerr<<"invalid insert position"<<endl;
			exit(1);
		} else {
			LNode<T> *s = new LNode<T>;
			s->data = item;
			s->next = p->next;
			p->next = s;
		}
	}
	T Delete(int i){
		LNode<T> *p = head;
		int j = 0;
		while(p && j<i-1)
		{
			p = p->next;
			j++;
		}
		if (!p || !p->next)
		{
			cerr<<"invalid delete position"<<endl;
			exit(1);
		} else {
			LNode<T> *s = p->next;
			int x = s->data;
			p->next = s->next;
			delete s;
			return x;
		}
	}
	void printList(){
		LNode<T> *p = head->next;
		while(p) {
			cout<<p->data<<" ";
			p = p->next;
		}
	}
	void clear(){
		LNode<T> *p = head->next;
		while(p) {
			LNode<T> *q = p;
			p = p->next;
			delete q;
		}
		head->next = NULL;
	}
};

template <class T>
class CLinkList {
	LNode<T> *pin;
public:
	CLinkList(){
		pin = new LNode<T>;
		pin->next = pin;
	}
	CLinkList(T a[], int size){
		pin = new LNode<T>;
		LNode<T> *p = pin;
		p->data = a[0];
		for (int i = 1; i < size; ++i) {
			LNode<T> *s = new LNode<T>;
			s->data = a[i];
			p->next = s;// insert element in the rear
			p = s;
		}
		p->next = pin;
	}
	~CLinkList(){
		LNode<T> *p = pin;
		while(p)
		{
			LNode<T> *q = p;
			p = p->next;
			delete q;
		}
		pin = NULL;
	}
	int size(){
		int sum = 0;
		if (pin==NULL) {
		    cerr<<"empty!"<<endl;
		    return 0;
		}
		LNode<T> *p = pin->next;
		while(p!=pin)
		{
			p = p->next;
			sum++;
		}
		return sum;
	}
	LNode<T>* find(T item){
		LNode<T> *p = pin->next;
		while(p->data!=item)
		{
			if (p==pin)
			{
				cerr<<"cannot find element "<<endl;
				exit(1);
			}
			p = p->next;
		}
		return p;
	}
	void printList(){
		LNode<T> *p = pin;
		while(p) {
			cout<<p->data<<" ";
			p = p->next;
			if (p==pin)
				return;
		}
	}
	void clear(){
		LNode<T> *p = pin;
		while(p) {
			LNode<T> *q = p;
			p = p->next;
			delete q;
			if (p==pin)
				break;
		}
		pin = NULL;
	}
};

/* Double link list*/
template <class T>
struct DNode
{
    T data;
    DNode<T> *prior;
    DNode<T> *next;
};


template <class T>
class DLinkList {
    DNode<T> *head;
public:
    DLinkList(){
        head = new DNode<T>;
        head->prior = NULL;
        head->next = NULL;
    }
    DLinkList(T a[], int size){
        head = new DNode<T>;
        head->prior = NULL;
        head->next = NULL;
        head->data = a[0];
        DNode<T> *p = head;
        for (int i = 1; i < size; ++i) {
            DNode<T> *s = new DNode<T>;
            s->data = a[i];
            p->next = s;// insert element in the rear
            s->prior = p;
            p = s;
        }
        p->next = NULL;
    }
    ~DLinkList(){
        DNode<T> *p = head;
        while(p)
        {
            DNode<T> *q = p;
            p = p->next;
            delete q;
        }
        head = NULL;
    }
    int size(){
        int sum = 0;
        if (head->next==NULL)
            return sum;
        DNode<T> *p = head;
        while(p)
        {
            p = p->next;
            sum++;
        }
        return sum;
    }
    T get(int i){
        LNode<T> *p = head;
        while(--i && p)
        {
            p = p->next;
        }
        if (!p||i<0)
        {
            cerr<<"cannot get element "<<i<<endl;
            exit(1);
        } else {
            return p->data;
        }
    }
    int locate(T x){
        LNode<T> *p = head;
        int l = 0;
        while(++l && p)
        {
            if (p->data == x)
                return l;
            p = p->next;
        }
        cerr<<"cannot find the element"<<endl;
        exit(1);
    }
    void Insert(int i, T item){
        if (i==0) {
            DNode<T> *t = new DNode<T>;
            t->data = item;
            t->prior = NULL;
            t->next = this->head;
            this->head->prior = t;
            return;
        }
        DNode<T> *p = head;
        int j = 0;
        while(p && j<i-1)
        {
            p = p->next;
            j++;
        }
        if (!p) {
            cerr<<"invalid insert position"<<endl;
            exit(1);
        } else {
            DNode<T> *s = new DNode<T>;
            s->data = item;
            s->next = p->next;
            s->prior = p;
            p->next->prior = s;
            p->next = s;
        }
    }
//    T Delete(int i){
//
//    }
    void printList(){
        LNode<T> *p = head->next;
        while(p) {
            cout<<p->data<<" ";
            p = p->next;
        }
    }
//    void clear(){
//    }
};

template <class T>
void shellsort(T &a, int n) {
    int j, gap, temp;
    for (gap = n; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            temp = a[i];
            for (j = i; j >= gap; j -= gap) {
                if (temp < a[j-gap])
                    a[j] = a[j-gap];
                else
                    break;
            }
            a[j] = temp;
        }
    }
}
#include <string>
#include <iostream>
#include <fstream>

#include "holder.h"
#include "Timer.h"

using namespace std;

ostream & operator << (ostream & out, Stack & L)
{
	L.print(out);
	return out;
}

// ArrayStack

ArrayStack::ArrayStack(int cap) : Stack("AS"), capacity(cap), size(0)
{
	buf = new string[capacity];
}

void ArrayStack::push(const string & word)
{
	// check if stack is full
	if ( is_full() )
	{
		throw out_of_range("Cannot push on full array stack");
	}
	buf[size++] = word;
}

void ArrayStack::pop()
{
	// check if stack is empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot pop on empty array stack");
	}
	--size;
}

string ArrayStack::top()
{
	// check if stack is empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot top on empty array stack");
	}
	return buf[ size - 1 ]; // returns top element
}

bool ArrayStack::is_empty()
{
	return size == 0;
}

bool ArrayStack::is_full()
{
	return capacity == size;
}

void ArrayStack::print(ostream & out)
{
	for (int i = size - 1; i >= 0; --i)
	{
		out << buf[i];
		out << ' ';
	}
}

ArrayStack::~ArrayStack()
{
	delete[] buf;
}

// List Node

void ListNode::print(ostream & out, ListNode * L)
{
	out << L->data << ' ';
}

void ListNode::delete_list(ListNode * L)
{
	for (ListNode * curr = L; curr != nullptr;)
	{
		ListNode * temp = curr->next;
		delete curr;
		curr = temp;
	}
}

// Linked Stack

LinkedStack::LinkedStack() : Stack("LS"), head(nullptr)
{}

void LinkedStack::push(const string & word)
{
	if ( is_full() )
	{
		throw out_of_range("Cannot push on full linked stack");
	}
	head = new ListNode{ word, head };
}

void LinkedStack::pop()
{
	if ( is_empty() )
	{
		throw out_of_range("Cannot pop on empty linked stack");
	}
	ListNode * temp = head;
	head = head->next;
	delete temp;
}

string LinkedStack::top() 
{
	if ( is_empty() )
	{
		throw out_of_range("Cannot top on empty linked stack");
	}
	return head->data;
}

bool LinkedStack::is_empty()
{
	return head == nullptr;
}

bool LinkedStack::is_full()
{
	return false;
}

void LinkedStack::print(ostream & out)
{
	for (ListNode * temp = head; temp != nullptr; temp = temp->next)
	{
		ListNode::print(out, temp);
	}
}

LinkedStack::~LinkedStack()
{
	ListNode::delete_list(head);
}

// Array Queue

ArrayQueue::ArrayQueue(int cap) : Queue("AQ"), capacity(cap + 1), front(0), rear(0)
{
	buf = new string[capacity + 1];
}

void ArrayQueue::enq(const string & word)
{
	// check if full
	if ( is_full() )
	{
		throw out_of_range("Cannot enque to full queue");
	}
	buf[rear] = word;
	rear = (rear + 1) % capacity;
}

void ArrayQueue::deq()
{
	// check if empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot deque to empty queue");
	}
	front = (front + 1) % capacity;
}

string ArrayQueue::next()
{
	// check if empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot see next item in empty queue");
	}
	return buf[front];
}

bool ArrayQueue::is_empty()
{
	return front == rear;
}

bool ArrayQueue::is_full()
{
	return (rear + 1) % capacity == front;
}

void ArrayQueue::print(ostream & out)
{
	for (int i = front; i != rear; i = (i + 1) % capacity)
	{
		out << buf[i];
		out << ' ';
	}
}

ArrayQueue::~ArrayQueue()
{
	delete[] buf;
}

// Linked Queue

LinkedQueue::LinkedQueue() : Queue("LQ"), head(nullptr), tail(nullptr)
{}

void LinkedQueue::enq(const string & word)
{
	// check if full
	if ( is_full() )
	{
		throw out_of_range("Cannot enq full linked queue");
	}
	// check if empty
	else if ( is_empty() )
	{
		head = tail = new ListNode{ word, tail };
	}
	else 
	{
		tail = tail->next = new ListNode{ word, nullptr };
	}
}

void LinkedQueue::deq()
{
	// check if empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot deq empty linked queue");
	}
	ListNode * temp = head;
	head = head->next;
	delete temp;

	if ( is_empty() )
	{
		tail = nullptr;
	}
}

string LinkedQueue::next()
{
	// check if empty
	if ( is_empty() )
	{
		throw out_of_range("Cannot deq empty linked queue");
	}
	return head->data;
}

bool LinkedQueue::is_empty()
{
	return head == nullptr;
}

bool LinkedQueue::is_full()
{
	return false;
}

void LinkedQueue::print(ostream & out)
{
	for (ListNode * temp = head; temp != nullptr; temp = temp->next)
	{
		ListNode::print(out, temp);
	}
}

LinkedQueue::~LinkedQueue()
{
	ListNode::delete_list(head);
}

// measurements

void error(string word, string msg)
{
	cout << word << msg << endl;
} 

void insert_all_words(int k, string file_name, Holder & L)
{
	Timer t;
	double eTime;
	ifstream in(file_name);
	int limit = k * NWORDS / 10;
	t.start();
	for (string word; (in >> word) && (limit > 0); --limit)
	{
		L.insert(word);
	}
	t.elapsedUserTime(eTime);
	in.close();
	cout << "\t\tI = " << eTime << endl;
}

void remove_all_words(int k, string file_name, Holder & L)
{
	Timer t;
	double eTime;
	ifstream in(file_name);
	int limit = k * NWORDS / 10;
	t.start();
	for (string word; (in >> word) && (limit > 0); --limit)
	{
		L.remove();
	}
	t.elapsedUserTime(eTime);
	in.close();
	cout << "\t\tR = " << eTime << endl;
}

void measure_holder(string file_name, Holder & L)
{
	cout << L.name << endl;
	for (int K = 1; K <= 10; ++K)
	{
		cout << "\tK = " << K << endl;
		insert_all_words(K, file_name, L);
		remove_all_words(K, file_name, L);
		if ( !L.is_empty() )
		{
			error(L.name, "is not empty");
		}
	}
}

void measure_holders(string input_file)
{
	ArrayStack AS(NWORDS);
	LinkedStack LS;

	ArrayQueue AQ(NWORDS);
	LinkedQueue LQ;

	measure_holder(input_file, AS);
	measure_holder(input_file, LS);
	measure_holder(input_file, AQ);
	measure_holder(input_file, LQ);

}

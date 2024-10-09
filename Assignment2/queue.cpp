/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}

// Insert at a specific position
void Queue::insert(Data d, unsigned position)
{
    if (position > size())
    {
        errorHandle(3);  // Handle range error
    }

    QElement* newElement = new QElement(d);

    if (position == 0)
    {
        // Insert at the front
        newElement->next = head;
        head = newElement;
        if (size() == 0)
        {
            tail = newElement; // Special case for the first element
        }
    }
    else if (position == size())
    {
        // Insert at the tail
        tail->next = newElement;
        tail = newElement;
    }
    else
    {
        // Insert in the middle
        QElement* current = head;
        for (unsigned i = 0; i < position - 1; ++i)
        {
            current = current->next;
        }
        newElement->next = current->next;
        current->next = newElement;
    }

    nelements++;
}


// Error handling function
void Queue::errorHandle(int errorCode)
{
    switch (errorCode)
    {
    case 3:
        throw std::out_of_range("insert: range error.");
    default:
        throw std::runtime_error("Unknown error.");
    }
}


bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return nelements;
}

/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
   Description: test code
*/

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "queue.h"

/**
   Compare the given queue to the given array of data elements.  Return
   true if they are all equal.
 */
bool testQueueEquality( Queue & q, std::vector<Data> const & array)
{
    if (q.size() != array.size())
	return false;

    for (unsigned i = 0; i < q.size(); ++i)
    {
        Data d(0,0);
	q.remove(&d);
	if ( !d.equals(array[i]) )
	    return false;
	q.insert(d);
    }
    return true;
}


int main()
{
    using namespace std;

    //cout << "Testing queue.\n";
    Queue q1;

    q1.insert(Data(1,1));
    q1.insert(Data(3,3));
    q1.insert(Data(5,5));

    //q1.print();

    vector<Data> dataVec;
    dataVec.push_back(Data(1,1));
    dataVec.push_back(Data(3,3));
    dataVec.push_back(Data(5,5));

    assert(testQueueEquality(q1, dataVec));

    Data d44(4, 4);
    bool found = q1.search(d44);
    assert(found == false);


    q1.insert(d44);  // now is (1,1),(3,3),(5,5),(4,4)
    found = q1.search(d44);
    assert(found == true);

    // now queue is(1,1),(3,3),(5,5),(4,4) and 
    // dataVec has (1,1),(3,3),(5,5).  Not equal
    assert(testQueueEquality(q1, dataVec) == false);

    Data temp;
    q1.remove(&temp);  // now q1 is (3,3),(5,5),(4,4)

    Data temp2(1,1);
    assert(temp.equals(temp2));  // (1,1) == (1,1)

    Data temp3(6,6);
    found = q1.search(temp3);
    assert(found == false);

    // Additional tests for insertions at specific positions

    // Test inserting at the head (position 0)
    Queue q2;
    q2.insert(Data(10, 10));  // (10,10)
    q2.insert(Data(20, 20));  // (10,10), (20,20)
    q2.insert(Data(30, 30), 0);  // Insert at head (30,30), (10,10), (20,20)
    assert(testQueueEquality(q2, {Data(30,30), Data(10,10), Data(20,20)}));

    // Test inserting at the tail (position size)
    q2.insert(Data(40, 40), q2.size());  // Insert at tail
    assert(testQueueEquality(q2, {Data(30,30), Data(10,10), Data(20,20), Data(40,40)}));


    // Test inserting in the middle
    q2.insert(Data(25, 25), 2);  // Insert in the middle
    assert(testQueueEquality(q2, {Data(30,30), Data(10,10), Data(25,25), Data(20,20), Data(40,40)}));


    // Test illegal position (out of bounds)
    try {
        q2.insert(Data(50, 50), q2.size() + 1);  // Should trigger range error
        cerr << "Error: out of bounds insert did not trigger error.\n";
    } catch (const std::out_of_range& e) {
        // Expected behavior
    }

    // Test inserting into an empty queue
    Queue q3;
    q3.insert(Data(100, 100), 0);  // Insert at head in empty queue
    assert(testQueueEquality(q3, {Data(100, 100)}));

    // test cases provided by the prof 
    Queue q;
    q.insert(Data(1,2));
    q.insert(Data(3,4));
    q.insert(Data(5,6));

    q.insert(Data(-2,-3), 1);  // Insert at position 1
    q.insert(Data(-4,-5), 3);  // Insert at position 3
    assert(testQueueEquality(q, {Data(1,2), Data(-2,-3), Data(3,4), Data(-4,-5), Data(5,6)}));


    cout << "All tests passed!\n";
}

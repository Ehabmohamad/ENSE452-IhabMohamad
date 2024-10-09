### Modifications made in queue.h
## - Added the prototype for 
##      void insert(Data d, unsigned position);
##      void errorHandle(int errorCode);

### Modifications made in queue.cpp
## - Added the void insert(Data d, unsigned position) method to insert elements at a specific position in the queue.
# - If position == 0, the element is inserted at the head.
# - If position == size(), the element is inserted at the tail.
# - For other valid positions, the element is inserted in the middle.

## - Error Handling for Out-of-Bounds Insertions
# - Checks if the specified position is greater than the current size of the queue. If so, a out_of_range exception is thrown with the message "insert: range error."

### Modifications made in testq.cpp
## - Tests for Insertion at Specific Positions
# - Added test cases to validate that elements are inserted correctly at various positions.
## -  Test for Out-of-Bounds Insertion
# - Verify that the program correctly handles an invalid position 
## - Test for Insertion into an Empty Queue
# -  Ensure that elements can be inserted into an empty queue at position 0.
## - Additional Test provided in the Assignment Handout 
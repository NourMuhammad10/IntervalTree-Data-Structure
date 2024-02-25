#include <iostream>
using namespace std;

//Interval struct to store the low and high of the interval
struct Interval{
    int l;
    int h;
    Interval(int l, int h)
    {
        this -> l = l;
        this -> h = h;
    }
};

//Interval tree node struct
struct Node{
    /* each node has and interval,
     * mx representing the maximum high in the intervals of the node and its left and right subtrees
     * a pointer to the left child
     * a pointer to the right child
     * */
    Interval *interval{};
    int mx;
    Node* left;
    Node* right;
    //parameterized constructor to set the values
    Node(int l, int h)
    {
        interval = new Interval(l, h);
        mx = -1;
        left = nullptr;
        right = nullptr;
    }
};

//Interval tree class
class IntervalTree {
    Node *root;
public:
    IntervalTree() {
        root = nullptr;
    }
    //insert function to insert an interval
    Node *insert(Node *root, Node *newNode) {
        //if the tree is empty, make the root point to the new node
        if (root == nullptr)
        {
            root = newNode;
        }
        //if the interval low is lower than the current node low
        //move to the left subtree
        else if (newNode -> interval -> l < root -> interval -> l)
        {
            root -> left = insert(root -> left, newNode);
        }
        //if the interval low is greater than the current node low
        //move to the right subtree
        else
        {
            root -> right = insert(root -> right, newNode);
        }
        //update the mx in the current node
        if (newNode -> interval -> h > root -> mx)
        {
            root -> mx = newNode -> interval -> h;
        }
        return root;
    }
    //insert function that takes the low and high
    void insertInterval(int l, int h) {
        Node *newNode = new Node(l, h);
        root = insert(root, newNode);
    }
    //function to check if the two intervals overlap
    static bool doOverlap(Interval* i1, Interval* i2)
    {
        if(i1 -> l <= i2 -> h && i2 -> l <= i1 -> h){
            return true;
        }
        return false;
    }
    //function that has the logic of searching for the first overlapping interval it encounters
    Interval* searchFirstInterval(Interval * interval)
    {
        //start the path from the root
        Node * currNode = root;
        Interval * currInterval = root -> interval;
        //continue updating the path until an overlapping interval is found
        while(!doOverlap(currInterval, interval))
        {
            //the mx of the left smaller than the lower limit  of the query interval
            if(currNode -> left != nullptr && currNode -> left -> mx < interval -> l && currNode -> right != nullptr)
            {
                //move to the right subtree
                currInterval = currNode -> right -> interval;
                currNode = currNode -> right;
            }
            //the left subtree might be having an over lapping interval
            else if(currNode -> left != nullptr)
            {
                //move to the left subtree
                currInterval = currNode -> left -> interval;
                currNode = currNode -> left;
            }
            //no interval is found after finishing the path
            else
            {
                //return null pointer
                return nullptr;
            }
        }
        return currInterval;
    }
    //function to search for the first encountered interval overlapping with the query interval
    Interval * searchInterval(Interval * interval)
    {
        return searchFirstInterval(interval);
    }
    //function that has the logic of searching for the overlapping interval with the minimum low
    Interval* searchMinLow(Node * currNode, Interval * interval)
    {
        //the left subtree mx is less than or equal the query interval low
        if(currNode -> left != nullptr && interval -> l <= currNode -> left -> mx)
        {
            //check the left subtree first
            Interval * result = searchMinLow(currNode -> left, interval);
            if(result != nullptr)
            {
                return result;
            }
            //if the current node overlaps return it
            if(doOverlap(interval, currNode -> interval))
            {
                return currNode -> interval;
            }
            //if no overlapping interval return null
            return nullptr;
        }
        //if the condition foo the left subtree is not obtained, move to the right subtree
        else{
            //check the current node first if it overlaps
            if(doOverlap(interval, currNode -> interval))
            {
                return currNode -> interval;
            }
            //move to the right subtree
            return searchMinLow(currNode -> right, interval);
        }
    }
    //function to search for the interval with minimum low overlapping with the query interval
    Interval * searchMinLowInterval(Interval * interval)
    {
        return searchMinLow(root, interval);
    }
};

int main() {
    IntervalTree tree;
    tree.insertInterval(1,5);
    tree.insertInterval(3,7);
    tree.insertInterval(9,12);
    tree.insertInterval(6,10);
    tree.insertInterval(13,15);
    tree.insertInterval(17,19);
    tree.insertInterval(14,18);
    tree.insertInterval(20,25);

    Interval *result;
    auto* queryInterval = new Interval(8, 16);
    result = tree.searchMinLowInterval(queryInterval);

    if(result == nullptr)
    {
        cout << "Not found";
    }
    else
    {
        cout << "Overlapping interval with " << "[" << queryInterval -> l << ", " << queryInterval -> h<< "]"
        << " having the minimum low is ["<< result -> l << ", " << result -> h << "]";
    }
}

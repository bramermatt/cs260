#include <algorithm> // Included for std::atoi
#include <iostream>
#include <string>

// updated 2024.05.28 Matthew Bramer

using namespace std;

// Structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() : amount(0.0) {}
};

// Structure for a tree node
struct Node {
    Bid bid;
    Node* parentPtr;
    Node* leftPtr;
    Node* rightPtr;

    Node(Bid b) : bid(b), parentPtr(nullptr), leftPtr(nullptr), rightPtr(nullptr) {}
};

// Function to display bid information
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
}

// Binary Search Tree class definition
class BinarySearchTree {

private:
    Node* root;

    // Private methods
    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    Node* findMin(Node* node);
    void freeNode(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    Node* SearchNode(Node* node, string bidId);
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    freeNode(root);
}

// Recursive function to free nodes in the tree
void BinarySearchTree::freeNode(Node* node) {
    if (node != nullptr) {
        freeNode(node->leftPtr);
        freeNode(node->rightPtr);
        delete node;
    }
}

// Public method to initiate in-order traversal
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Private method for in-order traversal
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->leftPtr);
        displayBid(node->bid);
        inOrder(node->rightPtr);
    }
}

// Public method to insert a bid into the tree
void BinarySearchTree::Insert(Bid bid) {
    if (root == nullptr) {
        root = new Node(bid);
    } else {
        addNode(root, bid);
    }
}

// Private method to recursively add a bid to the tree
void BinarySearchTree::addNode(Node* node, Bid bid) {
    int curKey = atoi(node->bid.bidId.c_str());
    int key = atoi(bid.bidId.c_str());

    if (key > curKey) {
        if (node->rightPtr == nullptr) {
            node->rightPtr = new Node(bid);
            node->rightPtr->parentPtr = node;
        } else {
            addNode(node->rightPtr, bid);
        }
    } else {
        if (node->leftPtr == nullptr) {
            node->leftPtr = new Node(bid);
            node->leftPtr->parentPtr = node;
        } else {
            addNode(node->leftPtr, bid);
        }
    }
}

// Private method to find the node with the minimum value in a subtree
Node* BinarySearchTree::findMin(Node* node) {
    while (node->leftPtr != nullptr) {
        node = node->leftPtr;
    }
    return node;
}

// Private method to remove a node from the tree
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    if (node == nullptr) return node;

    int key = atoi(bidId.c_str());
    int curKey = atoi(node->bid.bidId.c_str());

    if (key < curKey) {
        node->leftPtr = removeNode(node->leftPtr, bidId);
    } else if (key > curKey) {
        node->rightPtr = removeNode(node->rightPtr, bidId);
    } else {
        if (node->leftPtr == nullptr && node->rightPtr == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->leftPtr == nullptr) {
            Node* temp = node;
            node->rightPtr->parentPtr = node->parentPtr;
            node = node->rightPtr;
            delete temp;
        } else if (node->rightPtr == nullptr) {
            Node* temp = node;
            node->leftPtr->parentPtr = node->parentPtr;
            node = node->leftPtr;
            delete temp;
        } else {
            Node* temp = findMin(node->rightPtr);
            node->bid = temp->bid;
            node->rightPtr = removeNode(node->rightPtr, temp->bid.bidId);
        }
    }
    return node;
}

// Public method to remove a bid from the tree
void BinarySearchTree::Remove(string bidId) {
    root = removeNode(root, bidId);
}

// Public method to search for a bid by bidId
Bid BinarySearchTree::Search(string bidId) {
    Node* nodePtr = SearchNode(root, bidId);
    if (nodePtr == nullptr) {
        return Bid(); // Return an empty bid if not found
    } else {
        return nodePtr->bid; // Return the found bid
    }
}

// Private method to recursively search for a bid node by bidId
Node* BinarySearchTree::SearchNode(Node* node, string bidId) {
    if (node == nullptr) {
        return nullptr; // Base case: key not found
    } else if (node->bid.bidId == bidId) {
        return node; // Base case: key found
    } else {
        int curKey = atoi(node->bid.bidId.c_str());
        int key = atoi(bidId.c_str());
        if (key > curKey) {
            return SearchNode(node->rightPtr, bidId);
        } else {
            return SearchNode(node->leftPtr, bidId);
        }
    }
    return nullptr; // Default return statement (should not be reached)
}

// Function to get bid input from user
void getBidInput(Bid& bid) {
    cout << "Enter Bid ID: ";
    cin >> bid.bidId;
    cout << "Enter Title: ";
    cin.ignore();
    getline(cin, bid.title);
    cout << "Enter Fund: ";
    getline(cin, bid.fund);
    cout << "Enter Amount: ";
    cin >> bid.amount;
}

// Main function to run the program
int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    Bid bid;
    string bidKey;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Add Bid" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            getBidInput(bid);
            bst->Insert(bid);
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            cout << "Enter Bid ID to search: ";
            cin >> bidKey;
            bid = bst->Search(bidKey);
            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid ID " << bidKey << " not found." << endl;
            }
            break;

        case 4:
            cout << "Enter Bid ID to remove: ";
            cin >> bidKey;
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Goodbye." << endl;

    delete bst;

    return 0;
}

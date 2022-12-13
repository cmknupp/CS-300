//============================================================================
// Name        : ABCU_Advising_Program.cpp
// Author      : Connie Knupp
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
// Code adapted from prior assingments.
//============================================================================

#include <iostream>
#include <ctype.h>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
    string prerequisites;
};

// Internal structure for tree node
struct Node {
   Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    Node* temp = root->right;
    while (temp != nullptr) {
        Node* next = temp->right;
        removeNode(root, temp->course.courseId);
        temp = next;
    }
    temp = root->left;
    while (temp != nullptr) {
        Node* next = temp->left;
        removeNode(root, temp->course.courseId);
        temp = next;
    }
    root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}


/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID
    this->removeNode(root, courseId);
}

/*
*Removes a node
*/
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    //if this node is empty then return (avoid crashing)
    if (node == nullptr) {
        return node;
    }
    // recurse down left subtree
    if (courseId.compare(node->course.courseId) < 0) {
        node->left - removeNode(node->left, courseId);
    }
    //else recurse down right subtree
    else if (courseId.compare(node->course.courseId) > 0) {
        node->right - removeNode(node->right, courseId);
    }
    else {
        // no children so this is leaf node
        if (node->left == nullptr && node->right == nullptr) {
            //delete node
            delete node;
            //set node to nullptr
            node = nullptr;
        }
        //one child to the left 
        else if (node->left != nullptr && node->right == nullptr) {
            //create temp node pointer
            Node* temp = node;
            //set node equal to left child node
            node = node->left;
            //delete temp
            delete temp;
        }
        //one child to the right
        else if (node->left == nullptr && node->right != nullptr) {
            //create temp node pointer
            Node* temp = node;
            // set node equal to right child node
            node = node->right;
            //delete temp
            delete temp;
        }
        //else has two children
        else {
            //create temp node
            Node* temp = node->right;
            //tranverse to bottom of left subtree
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            //set node to temp course
            node->course = temp->course;
            //transverse down right subtree
            node->right = removeNode(node->right, temp->course.courseId);
        }
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseId.compare(courseId) == 0) {
            cout << current->course.courseId << ", " << current->course.courseTitle << endl;
            cout << "Prerequisites: " << current->course.prerequisites << endl;
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    cout << courseId << " not found." << endl;
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare (course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //recurse down left
        inOrder(node->left);
        //output courseID, title,and prerequisits
        cout << node->course.courseId << ", " << node->course.courseTitle << endl;
        // recurse down right
        inOrder(node->right);
    }
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.courseId << ", " << course.courseTitle << endl;
    cout << "Prerequisites: " << course.prerequisites << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            if (((file[i][0].length()) > 0) && ((file[i][1].length()) > 0)) {
                // Create a data structure and add to the collection of bids
                Course course;
                course.courseId = file[i][0];
                course.courseTitle = file[i][1];
                course.prerequisites = "None";

                //add prerequisites if any
                unsigned int numPrereqs = file[i].size() - 2;  //computes number of prereqs
                //loop through prereqs
                for (unsigned int k = 0; k < numPrereqs; k++) {
                    string prereqToAdd = file[i][2 + k];
                    // if not blank field
                    if (prereqToAdd != "") {
                        //initiate boolean for is found, set to false
                        bool isFound = false;
                        //loop through file check for prereq existance
                        for (unsigned int l = 0; l < file.rowCount(); l++) {
                            //if match found
                            if (prereqToAdd == file[l][0]) {
                                //if no exisiting prereqs, set prereqs to current
                                if (course.prerequisites == "None") {
                                    course.prerequisites = prereqToAdd;
                                }
                                //if prereqs exist, add to end of prereqs
                                else {
                                    course.prerequisites = course.prerequisites + ", " + prereqToAdd;
                                }
                                //set is found to true
                                isFound = true;
                            }
                        }
                        // if not found print error statement
                        if (isFound == false) {
                            cout << "Unable to add invalid prerequisite for " << course.courseId << ", "
                                << course.courseTitle << endl;
                        }

                    }
                }
            //cout << course.courseId << ", " << course.courseTitle << endl;
            //cout << "Prerequisites: " << course.prerequisites << endl;

            //add this course to the binarty tree
            bst->Insert(course);
            }

            //if missing data error statement
            else {
                cout << "Invalid course format, unable to add course " << file[i][0] << "."
                    << endl;
            }
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "CSCI100";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.csv";
        courseKey = "CSCI100";
    }


    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    cout << "Welcome to the course planner." << endl;
    int choice = 0;
    while (choice != 9) {
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;

        //if fails clear input stream
        if (cin.fail()) {
            cout << "Invalid Entry" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }
        else {
            switch (choice) {

            case 1: //load data
               cout << "Enter the file you would like to load:" << endl;
               cin >> csvPath;

                // Loads course information from file
                loadCourses(csvPath, bst);
                break;

            case 2: //print all courses
                cout << "Here is a sample schedule:" << endl;
                cout << endl;
                bst->InOrder();
                break;

            case 3: //print one course
                cout << "What course do you want to know about?" << endl;
                cin >> courseKey;
                for (auto& c : courseKey) c = toupper(c);
                remove(courseKey.begin(), courseKey.end(), ' ');
                bst->Search(courseKey);
                break;

            case 9: //exit program
                break;

            default: //error statement, reset loop
                cout << choice << " is not a valid option." << endl;
                choice = 0;
            }
        }
    }
    cout << "Thank you for using the course planner." << endl;

    return 0;
}

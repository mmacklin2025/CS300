// Name: ProjectTwo.cpp
// Author: Kayla Macklin
// Date: 08/11/2025
// Description: 7-1 Project Two

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <ctime>
using namespace std;

const unsigned int DEFAULT_SIZE = 179;

//==============================
// Course Structure
//==============================
struct Course {
    string courseId;               // unique identifier for the course number
    string courseTitle;           // unique identifier for the course name
    vector<string> prerequisites; // lists any prerequisite courses required

    Course() {}
};

//==============================
// HashTable Class
//==============================
class HashTable {
    // define structure to hold courses
private:
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;
    unsigned int tableSize = DEFAULT_SIZE;

    // Hash function based on courseId string
    unsigned int hash(string key) {
        unsigned int hash = 0;
        for (char ch : key) {
            hash += ch;
        }
        return hash % tableSize;
    }
public:
    HashTable() {
        nodes.resize(tableSize);
    }

    // function to insert a course into the hash table
    void Insert(Course course);

    // function to search for a course using the course ID
    Course Search(string courseId);

    // function to get all courses
    vector<Course> GetAllCourses();
};

//==============================
// HashTable Method Definitions
//==============================

// insert a course
void HashTable::Insert(Course course) {
    unsigned key = hash(course.courseId);
    Node* node = &nodes.at(key);

    if (node->key == UINT_MAX) {
        node->key = key;
        node->course = course;
    }
    else {
        while (node->next != nullptr) {
            node = node->next;
        }
        node->next = new Node(course, key);
    }
}

// search for a course
Course HashTable::Search(string courseId) {
    unsigned key = hash(courseId);
    Node* node = &nodes.at(key);

    while (node != nullptr) {
        if (node->course.courseId == courseId) {
            return node->course;
        }
        node = node->next;
    }

    return Course(); // empty course if not found
}

// get all courses
vector<Course> HashTable::GetAllCourses() {
    vector<Course> allCourses;
    for (Node& node : nodes) {
        if (node.key != UINT_MAX) {
            allCourses.push_back(node.course);
            Node* temp = node.next;
            while (temp != nullptr) {
                allCourses.push_back(temp->course);
                temp = temp->next;
            }
        }
    }
    return allCourses;
}

//==============================
// File Loading Function
//==============================

// function to load courses
void loadCourses(string filename, HashTable* table) {
    ifstream file(filename);
    string line;

    // outputs an error if there is an error opening a file
    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseId, courseTitle, prereq;
        vector<string> prereqs;

        getline(ss, courseId, ',');
        getline(ss, courseTitle, ',');

        while (getline(ss, prereq, ',')) {
            prereqs.push_back(prereq);
        }

        Course course;
        course.courseId = courseId;
        course.courseTitle = courseTitle;
        course.prerequisites = prereqs;

        table->Insert(course);
        count++;
    }

    file.close();
    cout << count << " courses loaded.\n";
}

//==============================
// Display Functions
//==============================

// displays course information
void displayCourse(Course course, HashTable* table) {
    cout << course.courseId << ": " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (string prereq : course.prerequisites) {
            Course prereqCourse = table->Search(prereq);
            cout << prereq << " (" << prereqCourse.courseTitle << ") ";
        }
        cout << endl;
    }
    else {
        cout << "Prerequisites: None\n";
    }
}

void displayAllCourses(HashTable* table) {
    vector<Course> courses = table->GetAllCourses();
    sort(courses.begin(), courses.end(), [](Course a, Course b) {
        return a.courseId < b.courseId;
        });

    for (Course c : courses) {
        cout << c.courseId << ", " << c.courseTitle << endl;
    }
}

//==============================
// Main Menu
//==============================

// main method
int main() {
    HashTable* courseTable = new HashTable();
    string input;
    int choice = 0;
    clock_t ticks;

    // prints the menu of choices
    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "  1. Load Data\n";
        cout << "  2. Display All Courses\n";
        cout << "  3. Display Course Details\n";
        cout << "  9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter file name: ";
            cin >> input;

            // initialize a timer variable before loading courses
            ticks = clock();

            // complete method call to load the courses
            loadCourses(input, courseTable);

            // calculate the elapsed time and display results
            ticks = clock() - ticks;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds\n";
            break;
        }
        case 2: {
            ticks = clock();
            displayAllCourses(courseTable);
            ticks = clock() - ticks;
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds\n";
            break;
        }
        case 3: {
            cout << "Enter course ID: ";
            cin >> input;
            ticks = clock();
            Course course = courseTable->Search(input);
            ticks = clock() - ticks;
            if (!course.courseId.empty()) {
                displayCourse(course, courseTable);
            }
            else {
                cout << "Course not found.\n";
            }
            cout << "Time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds\n";
            break;
        }
        case 9: {
            cout << "Goodbye!\n";
            break;
        }
        default:
            cout << "Invalid option. Try again.\n";
        }
    }

    delete courseTable;
    return 0;
}

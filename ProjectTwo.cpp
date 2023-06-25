//============================================================================
// Name        : Project 1
// Author      : Ryan Hubbuck
// Date        : 06/14/2023
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <utility>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "CSVparser.hpp"

using namespace std;

//define a structure to hold course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites; // vector to store all prerequisites (if any)
    Course() {}
};

void displayCourse(Course course) {
    cout << course.courseNumber << ": " << course.courseName << endl;
    return;
}

// function to read file, parse data, and add valid data to data structure
vector<Course> loadCourses(string fname) {
    vector<string> possibleCourses; // vector for courses that appear and are allowed to be a prerequisite
    vector<Course> validCourses; // vector for final courses after validation
 
    // Define a vector data structure to hold a collection of courses
    vector<Course> courses;


    try {
        vector<vector<string>> content; // initialize vector of string vectors for file content
        vector<string> row; // initialize vector of strings for each row
        string line, word; // initialzie string variables to store individual parts of rows

        fstream file(fname, ios::in); // declare string to read in
        if (file.is_open()) // if file is open
        {
            while (getline(file, line)) // while there is a line to get
            {
                row.clear(); // clear current row

                stringstream str(line); // get current line

                while (getline(str, word, ',')) // get each word seperated by commas
                    row.push_back(word); // add the word to the row
                content.push_back(row); // add the row to the content vector
            }
        }
        else // else if file could not open
            cout << "Could not open the file\n";

        for (int i = 0;i < content.size();i++) // for each element in the content vector
        {
            Course course; // declare new Course object

            course.courseNumber = content[i][0]; // store course number in Course
            possibleCourses.push_back(content[i][0]); // and add course number to list of possible courses

            if (content[i][1] != "") { // if there is a course name, store that in course object
                course.courseName = content[i][1];
            }

            for (int j = 0;j < content[i].size();j++) // loop through each row
            {
                if (j > 1 && j < content[i].size()) { // for any word/cell that exists after the second (course number)
                    if (content[i][j] != "") { // that is not empty
                        course.prerequisites.push_back(content[i][j]); // add as a prerequisite
                    }
                }
            }     
            courses.push_back(course); // add course to courses vector
        }

    }
    catch (csv::Error& e) { // catch any errors with file read
        std::cerr << e.what() << std::endl;
    }

    // loop through vector of all courses read in
    for (int i = 0; i < courses.size(); ++i) {
        Course currentCourse = courses[i]; // set current course to current iteration
        bool isValid = true; // initialize boolean used to validate course

        if (currentCourse.prerequisites.size() > 0) { // if the course object has prerequisites
            for (int j = 0; j < currentCourse.prerequisites.size(); ++j) { // loop through prerequisite vector
                // check if each prerequisite is in the vector of possibleCourses
                if (std::find(possibleCourses.begin(), possibleCourses.end(), currentCourse.prerequisites[j]) != possibleCourses.end()) {
                    isValid = true; // if yes, the course is valid
                }
                else { // if not in vector, the course is not valid and can not be used
                    isValid = false;
                }
            }
        }
        if (isValid) { // add vector to validCourses vector if it is valid
            validCourses.push_back(currentCourse);
        }
    }
    return validCourses; // return valid courses only
}


// Function to partition objects before sorting
int partition(vector<Course>& courses, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;
    // pick the middle element as pivot point
    string pivot = courses[low + (high - low) / 2].courseNumber;
    bool done = false; // initialize done boolean

    while (!done) { // while not done
        while (courses[low].courseNumber.compare(pivot) < 0) {  // while low is below pivot
            ++low; // increment low
        }

        while (courses[high].courseNumber.compare(pivot) > 0) { // while high is above pivor
            --high; // decrement high
        }

        if (low >= high) { // if 1 or 1 elements remain, partition is done. return high
            done = true;
        }
        else { // else swap low and high value
            std::swap(courses.at(low), courses.at(high));
            ++low; // incriment low toward pivot
            --high; // decrement high toward pivot
        }
    }
    return high;  //return high;
}

// Function to quick sort objects by courseNumber
void quickSort(vector<Course>& courses, int begin, int end) {
    //set mid equal to 0
    int mid = 0;

    if (begin >= end) { // base case: if 0 or 1 elements it is sorted; return
        return;
    }

    mid = partition(courses, begin, end); // partition courses into low/high and set mid as last element of low
    quickSort(courses, begin, mid); // recursively sort low partition (begin to mid)
    quickSort(courses, mid + 1, end); // recursively sort high partition (mid+1 to end)

}
// main function
int main(int argc, char* argv[]) {

    // initialize vector of Course objects
    vector<Course> courses;

    string fname;
    string userFile; // initialize string for user to specify file
    int choice = 0; // initialize user menu input
    string userCourse; // initialize string for user to specify course to search for
    bool found = false; // initialize boolean for course searching
    while (choice != 9) { // display user menu

        vector<vector<string>> content;
        vector<string> row; 
        string line, word;

        fstream file(fname, ios::in);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                row.clear();

                stringstream str(line);

                while (getline(str, word, ','))
                    row.push_back(word);
                content.push_back(row);
            }
        }
        else
           // cout << "Could not open the file\n";

        for (int i = 0;i < content.size();i++)
        {
            cout << "line" << endl;
            for (int j = 0;j < content[i].size();j++)
            {
                cout << content[i][j] << " ";
            }
            string test = content[i][0];
            string testName = content[i][1];
           // cout << test << ":" << testName << endl;
           // cout << "\n";
        }
        

        cout << "Welcome to the course planner:" << endl << endl << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl << endl << endl << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Get user input for file name
            cout << "Please enter the exact file name you wish to load. (case sensitive / include .csv)" << endl;
            cin >> fname;
            // Complete the method call to load the courses
            courses = loadCourses(fname);
            // display number of courses loaded in to user
            cout << courses.size() << " courses read" << endl;
            break;

        case 2:
            // Sort courses in alphanumeric order by courseNumber
            quickSort(courses, 0, courses.size() - 1);
            cout << "Here is a sample schedule:" << endl << endl;
            for (int i = 0; i < courses.size(); ++i) { // loop and display each course name and number
                displayCourse(courses[i]);
            }
            cout << endl;
            break;

        case 3:
            cout << endl << "What course do you want to know about? ";
            cin >> userCourse; // get user input for course to search for and make all letters uppercase
            std::transform(userCourse.begin(), userCourse.end(), userCourse.begin(), ::toupper);

            // loop through courses vector
            for (int i = 0; i < courses.size(); i++) {
                if (courses[i].courseNumber == userCourse) { // at each location, if the courseNumber matches that object courseNumber
                    cout << endl << endl << courses[i].courseNumber << ", " << courses[i].courseName << endl; // display course information
                    cout << "Prerequisites: ";  // loop through vector of prerquisites
                    for (int j = 0; j < courses[i].prerequisites.size(); ++j) {
                        cout << courses[i].prerequisites[j] << " "; // display each prerequisite
                    }
                    cout << endl << endl;
                    found = true; // set found to true
                    break;
                }
            }
            break;

        default: // prompt for valid choice if 1,2,3,9 not entered
            cout << endl << "Please enter a valid input." << endl << endl;
        }
    }
    // exit when 9 entered
    cout << "Good bye." << endl;

    return 0;
}

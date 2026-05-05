#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Student {
public:
    string firstName;
    string lastName;
    string group;
    int age;

    Student(string fName, string lName, string grp, int a) {
        firstName = fName;
        lastName = lName;
        group = grp;
        age = a;
    }

    void printInfo() const {
        cout << firstName << " " << lastName
            << " | Група: " << group
            << " | Вік: " << age << endl;
    }
};

int main() {
    string filename = "students.txt";
    vector<Student> studentVector;
    ifstream inFile(filename);

    if (inFile.is_open()) {
        string fName, lName, grp;
        int a;

        while (inFile >> fName >> lName >> grp >> a) {
            studentVector.push_back(Student(fName, lName, grp, a));
        }
        inFile.close();
    }
    else {
        cout << "Помилка відкриття файлу!" << endl;
        return 1;
    }

    for (int i = 0; i < studentVector.size(); i++) {
        studentVector[i].printInfo();
    }

    return 0;
}
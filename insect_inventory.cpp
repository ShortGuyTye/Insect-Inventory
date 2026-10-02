#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

class insect {
    public:
    string collector;
    string location;
    string date;
    string family;

    insect () {
        collector = "";
        location = "";
        date = "";
        family = "";
    }

    insect (string collector, string location, int date, string family) {
        this->collector = collector;
        this->location = location;
        this->date = date;
        this->family = family;
    }
};

//Write all of data in current storage to file
int write(insect collection[], int size) {
    ofstream storage("storage.txt");
    int curr = 0;

    while (curr < size) {
        if (collection[curr].family == "") {
            break;
        } else if (storage.is_open()) {
            storage << collection[curr].collector << endl
                << collection[curr].location << endl
                << collection[curr].date << endl
                << collection[curr].family << endl;
                curr++;
        } else {
            cerr << "Unable to write to storage\n";
        }
        }
     storage.close();
     return curr;
}

//Read all of data from file and put into current storage
void load (insect collection[], int size) {
    ifstream storage("storage.txt");
    int curr = 0;
    while (curr < size) {
        if (storage.is_open()) {
            string tester;            
            getline(storage, tester);
            if (tester == "") {
                break;
            }
            collection[curr].collector = tester;
            getline(storage, collection[curr].location);
            getline(storage, collection[curr].date);
            getline(storage, collection[curr].family);
            curr++;
        }
        else {
            cerr << "Unable to open storage\n";
        }
    }
}

//Print all of current data
void display (insect collection[], int size) {
    int curr = 0;
    while (curr < size) {
        if (collection[curr].family == "") {
            break;
        }
        cout << "Collector name: " << collection[curr].collector << " || "
            << "Location: " << collection[curr].location << " || "
            << "Date caught: " << collection[curr].date << " || "
            << "Family name: " << collection[curr].family << endl;
            curr++;

    } 
}

//Main loop
int main () {
    const int COLLECTION_SIZE = 120;
    int family_num = 0;
    string command;
    insect collection[COLLECTION_SIZE];

    cout << "Exit, Save, Load, Print, Add\n";
    getline(cin, command);
    transform(command.begin(), command.end(), command.begin(), [](char a) {return tolower(a);});

    while (command != "exit") {
        if (command == "save") {
            family_num = write(collection, COLLECTION_SIZE);
        } else if (command == "load") {
            load(collection, COLLECTION_SIZE);
        } else if (command == "print") {
            display(collection, COLLECTION_SIZE);
        } else {
            string collector;
            string location;
            string date;
            string family;

            cout << "Enter name of collector: \n";
            getline(cin, collector);
            cout << "Enter name of location: \n";
            getline(cin, location);
            cout << "Enter date caught (mm/dd/yy): \n";
            getline(cin, date);
            cout << "Enter name of insect family: \n";
            getline(cin, family);

            collection[family_num].collector = collector;
            collection[family_num].location = location;
            collection[family_num].date = date;
            collection[family_num].family = family;
            family_num++;
        }
        cout << "Exit, Save, Load, Print, Add\n";
        getline(cin, command);
        transform(command.begin(), command.end(), command.begin(), [](char a) {return tolower(a);});
    }
}
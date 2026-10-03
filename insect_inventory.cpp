#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <regex>

using namespace std;

//############### CLASSES ################
class insect {
    private:
    string collector;
    string location;
    int date;
    string family;

    public:
    insect () {
        collector = "";
        location = "";
        date = 0;
        family = "";
    }

    insect (string collector, string location, int date, string family): 
        collector(collector), location(location), date(date), family(family){}

    insect (string collector, string location, string date, string family): 
        collector(collector), location(location), family(family){
            if (date == "null") {
                this->date = 0;
            } else {
                int month;
                int day;
                int year;

                string num;
                stringstream str(date);
                getline(str, num, '/');
                month = stoi(num);
                getline(str, num, '/');
                day = stoi(num);
                getline(str, num, '/');
                year = stoi(num);
                this->date = (year * 10000) + (month * 100) + day;
            }
        }

    bool operator==(const insect& other) const{
        return (collector == other.get_collector() || other.get_collector() == "null") &&
            (location == other.get_location() || other.get_location() == "null") &&
            (date == other.get_date() || other.date == 0) &&
            (family == other.get_family() || other.get_family() == "null");

    }

    //Getters
    string get_collector() const{
        return collector;
    }
    string get_location() const{
        return location;
    }
    int get_date() const{
        return date;
    }
    string get_family() const{
        return family;
    }
};

class collection {
    private: 
    vector <insect> families;

    //################ HELPERS ###################

    int find_insect (const insect& target) const {
        for (int i = 0; i < families.size(); i++) {
            if (families[i] == target) return i;
        }
        return -1;
    }

    //make a string date into int
    int date_to_int(string date) const {
        int month;
        int day;
        int year;

        string num;
        stringstream str(date);
        getline(str, num, '/');
        month = stoi(num);
        getline(str, num, '/');
        day = stoi(num);
        getline(str, num, '/');
        year = stoi(num);

        return (year * 10000) + (month * 100) + day;
    }

    //make an int date into string
    string date_to_string(int date) const {
        string lit = to_string(date);
        string day = lit.substr(6, 2);
        string month = lit.substr(4, 2);
        string year = lit.substr(0, 4);
        return month + "/" + day + "/" + year;
    }

    public:
    vector<insect>& get_families() {
        return families;
    }

    //############### FUNCTIONS ###################

    //Write all of data in current storage to file
    void write(string filename) const{
        ofstream storage(filename);
        int curr = 0;

        if (storage.is_open()) {
            while (curr < families.size()) {
                const insect& curr_insect = families[curr];
                storage << curr_insect.get_collector() << endl
                    << curr_insect.get_location() << endl
                    << date_to_string(curr_insect.get_date()) << endl
                    << curr_insect.get_family() << endl;
                    curr++;
            }
        } else {
            cerr << "Unable to write to storage\n";
        }
        storage.close();
    }

    //Read all of data from file and put into current storage
    void load (string filename) {
        ifstream storage(filename);
        int curr = 0;
        string collector;
        string location;
        string date;
        string family;

        if (storage.is_open()) {
            while (getline(storage, collector)) {
                getline(storage, location);
                getline(storage, date);
                getline(storage, family);
                int date_num = date_to_int(date);
                families.push_back(insect(collector, location, date_num, family));
                curr++;
            }
        } else {
            cerr << "Unable to open storage\n";
        }
    }

    //Print all of current data
    void display (vector<insect>& bugs) const {
        int curr = 0;
        while (curr < bugs.size()) {
            const insect& curr_insect = bugs[curr];
            cout << "Collector name: " << curr_insect.get_collector() << " || "
                << "Location: " << curr_insect.get_location() << " || "
                << "Date caught: " << date_to_string(curr_insect.get_date()) << " || "
                << "Family name: " << curr_insect.get_family() << endl;
                curr++;
        } 
    }

    //Add data
    void add (string filename) {
        string command;
        regex date_pattern(R"([0-1][0-9]/[0-3][0-9]/[0-9]{4})");
        cout << "Enter q at anytime to quit\n";
        while (true) {
            string collector;
            string location;
            string date;
            string family;

            cout << "Enter name of collector: \n";
            getline(cin, collector);
            transform(collector.begin(), collector.end(), collector.begin(), [](char a) {return tolower(a);});
            if (collector == "q") break;

            cout << "Enter name of location: \n";
            getline(cin, location);
            transform(location.begin(), location.end(), location.begin(), [](char a) {return tolower(a);});
            if (location == "q") break;

            cout << "Enter date caught (mm/dd/yyyy): \n";
            getline(cin, date);
            while (!regex_match(date, date_pattern)) {
                cout << "Invalid date. Enter date caught (mm/dd/yyyy): \n";
                if (date == "q") break;
                getline(cin, date);
            }
            if (date == "q") break;

            cout << "Enter name of insect family: \n";
            getline(cin, family);
            transform(family.begin(), family.end(), family.begin(), [](char a) {return tolower(a);});
            if (family == "q") break;

            int date_num = date_to_int(date);
            families.push_back(insect(collector, location, date_num, family));
            cout << "Successfully added insect\n";
        }
        write(filename);
    }

    //add for in line data
    void add (string filename, const insect& target) {
        families.push_back(target);
        write(filename);
        cout << "Successfully added insect\n";
    }

    void remove (string filename) {
        string command;
        regex date_pattern(R"([0-1][0-9]/[0-3][0-9]/[0-9]{4})");
        cout << "Enter q at anytime to quit\n";
        while (true) {
            string collector;
            string location;
            string date;
            string family;

            cout << "Enter name of collector you would like to remove: \n";
            getline(cin, collector);
            if (collector == "q") break;
            cout << "Enter name of location you would like to remove: \n";
            getline(cin, location);
            if (location == "q") break;
            cout << "Enter date caught (mm/dd/yyyy) you would like to remove: \n";
            getline(cin, date);
            while (!regex_match(date, date_pattern)) {
                cout << "Invalid date. Enter date (mm/dd/yyyy) you would like to remove: \n";
                if (date == "q") break;
                getline(cin, date);
            }
            if (date == "q") break;
            cout << "Enter name of insect family you would like to remove: \n";
            getline(cin, family);
            if (family == "q") break;

            int date_num = date_to_int(date);
            int index = find_insect(insect(collector, location, date_num, family));
            if (index != -1) {
                families.erase(families.begin() + index);
                cout << "Successfully removed insect\n";
            } else {
                cout << "Error: Could not find insect\n";
            }
        }
        write(filename);
    }

    void remove (string filename, const insect& target) {
        int index = find_insect(target);
        if (index != -1) {
                families.erase(families.begin() + index);
                cout << "Successfully removed insect\n";
            } else {
                cout << "Error: Could not find insect\n";
            }
    }

    void search() const {
        string command;
        regex date_pattern(R"([0-1][0-9]/[0-3][0-9]/[0-9]{4})");
        string collector;
        string location;
        string date;
        string family;

        cout << "Enter name of collector you would like to search for. If you don't want to search by collector, enter null: \n";
        getline(cin, collector);
        cout << "Enter name of location you would like to search for. If you don't want to search by location, enter null: \n";
        getline(cin, location);
        cout << "Enter date caught (mm/dd/yyyy) you would like to search for. If you don't want to search by date, enter null: \n";
        getline(cin, date);
        while (!regex_match(date, date_pattern)) {
            cout << "Invalid date. Enter date (mm/dd/yyyy) you would like to search for. If you don't want to search by date, enter null: \n";
            if (date == "q") break;
            getline(cin, date);
        }
        cout << "Enter name of insect family you would like to search for. If you don't want to search by family, enter null: \n";
        getline(cin, family);

        insect target = insect(collector, location, date, family);
        vector<insect> filtered;
        for (const insect& hexapod : families) {
            if (hexapod == target) {
                filtered.push_back(hexapod);
            }
        }
        display(filtered);
    }

    void search(const insect& target) const {
        vector<insect> filtered;
        for (const insect& hexapod : families) {
            if (hexapod == target) {
                filtered.push_back(hexapod);
            }
        }
        display(filtered);
    }

    void reset(string filename) {
        string answer;
        cout << "WARNING: THIS ACTION CAN NOT BE UNDONE. ARE YOU SURE? TYPE YES TO CONTINUE\n";
        cin >> answer;
        transform(answer.begin(), answer.end(), answer.begin(), [](char a) {return tolower(a);});
        if (answer == "yes") {
            families.clear();
            write(filename);
        }

    }
};

vector<string> prompt () {
    string command;
    cout << "Enter command. Enter help for options.\n";
    getline(cin, command);
    transform(command.begin(), command.end(), command.begin(), [](char a) {return tolower(a);});

    //tokenize
    vector<string> arguments;
    string token;
    bool in_quotes = false;

    for (char c : command) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ' ' && !in_quotes) {
            arguments.push_back(token);
            token.clear();

        } else {
            token += c;
        }
    }
    arguments.push_back(token);
    return arguments;
}

//################# MAIN LOOP ##################
int main () {
    string filename = "storage.txt";
    regex date_pattern(R"([0-1][0-9]/[0-3][0-9]/[0-9]{4})");
    collection coll = collection();
    coll.load(filename);

    vector<string> arguments = prompt();
    string command = arguments[0];

    while (command != "exit") {
        if (command == "print") {
            coll.display(coll.get_families());
        } else if (command == "add") {
            if (arguments.size() == 1) {
                coll.add(filename);
            } else if (arguments.size() == 5) {
                if (regex_match(arguments[3], date_pattern)) {
                    coll.add(filename, insect(arguments[1], arguments[2], arguments[3], arguments[4]));
                }
            }
        } else if (command == "delete") {
            if (arguments.size() == 1) {
                coll.remove(filename);
            } else if (arguments.size() == 5) {
                coll.remove(filename, insect(arguments[1], arguments[2], arguments[3], arguments[4]));
            }
        } else if (command == "search") {
            if (arguments.size() == 1) {
                coll.search();
            } else if (arguments.size() == 5) {
                insect target = insect(arguments[1], arguments[2], arguments[3], arguments[4]);
                coll.search(target);
            }
        } else if (command == "reset") {
            coll.reset(filename);
        } else if (command == "help") {
            cout << "---------------------------------------------------------------------------------------" << endl
                << "Note - nothing is case sensitive" << endl
                << "Exit: exits program" << endl
                << "print: displays all data stored" << endl
                << "add: adds data to storage. Can have one argument, or four in the format - add \"name\" \"location\" mm/dd/yyyy \"family\"" << endl
                << "delete: removes data from storage. Can have one argument, or four in the format - delete \"name\" \"location\" mm/dd/yyyy \"family\"" << endl
                << "search: filters and displays data. Enter \"null\" for any category you do not want to filter by." << endl
                << "reset: completely clears all data. Can not be undone." << endl
                << "Can have one argument, or four in the format - search \"name\" \"location\" mm/dd/yyyy \"family\"" << endl
                << "help: list all commands" << endl
                << "---------------------------------------------------------------------------------------" << endl;
        }
        arguments = prompt();
        command = arguments[0];
    }
    cout << "Successfully exited\n";
}
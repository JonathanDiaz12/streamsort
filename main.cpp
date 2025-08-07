#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

struct Show {
    string title;
    string genre;
    int episodes;
    double rating;
};

vector<Show> queue;

// Function Prototypes
void displayMenu();
void addShow();
void removeShow();
void displayQueueRecursive(const vector<Show>&, size_t);
void sortQueue();
void searchShow();
void saveQueueToFile(const string&);
void loadQueueFromFile(const string&);
bool confirmDoubleInput(double, double, double);
bool confirmIntInput(int, int);

int main() {
    loadQueueFromFile("queue.txt");
    int choice;
    do {
        displayMenu();
        try {
            cout << "Enter your choice: ";
            if (!(cin >> choice)) throw invalid_argument("Input must be a number.");
            switch (choice) {
                case 1: addShow(); break;
                case 2: removeShow(); break;
                case 3: displayQueueRecursive(queue, 0); break;
                case 4: sortQueue(); break;
                case 5: searchShow(); break;
                case 6: saveQueueToFile("queue.txt"); cout << "Queue saved. Goodbye!\n"; break;
                case 7: loadQueueFromFile("queue.txt"); break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } catch (exception& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 6);

    return 0;
}

// 1. Menu
void displayMenu() {
    cout << "\n🎬 StreamSort++ Menu:\n";
    cout << "1. Add a show to the queue\n";
    cout << "2. Remove a show by title\n";
    cout << "3. Display the current queue (recursively)\n";
    cout << "4. Sort the queue\n";
    cout << "5. Search for a show by title\n";
    cout << "6. Save and Exit\n";
    cout << "7. Load queue from file\n";
}

// 2. Add Show
void addShow() {
    Show s;
    cout << "Enter title: ";
    cin.ignore(); getline(cin, s.title);
    cout << "Enter genre: ";
    getline(cin, s.genre);

    cout << "Enter number of episodes: ";
    if (!(cin >> s.episodes) || !confirmIntInput(s.episodes, 1))
        throw invalid_argument("Episodes must be a positive integer.");

    cout << "Enter rating (1.0 - 5.0): ";
    if (!(cin >> s.rating) || !confirmDoubleInput(s.rating, 1.0, 5.0))
        throw invalid_argument("Rating must be between 1.0 and 5.0.");

    queue.push_back(s);
    cout << "✅ Show added successfully!\n";
}

// 3. Remove Show
void removeShow() {
    string target;
    cout << "Enter the title to remove: ";
    cin.ignore(); getline(cin, target);

    auto it = find_if(queue.begin(), queue.end(), [&target](const Show& s) {
        return s.title == target;
    });

    if (it != queue.end()) {
        queue.erase(it);
        cout << "✅ Show removed.\n";
    } else {
        throw runtime_error("❌ Show not found in queue.");
    }
}

// 4. Recursive Display
void displayQueueRecursive(const vector<Show>& q, size_t index) {
    if (index >= q.size()) return;
    cout << index + 1 << ". " << q[index].title << " | " << q[index].genre 
         << " | " << q[index].episodes << " episodes | Rating: " << q[index].rating << endl;
    displayQueueRecursive(q, index + 1);
}

// 5. Sort
void sortQueue() {
    int opt;
    cout << "Sort by:\n1. Title\n2. Rating\nChoice: ";
    cin >> opt;

    if (opt == 1) {
        sort(queue.begin(), queue.end(), [](const Show& a, const Show& b) {
            return a.title < b.title;
        });
    } else if (opt == 2) {
        sort(queue.begin(), queue.end(), [](const Show& a, const Show& b) {
            return a.rating > b.rating;
        });
    } else {
        throw invalid_argument("Invalid sort option.");
    }

    cout << "✅ Queue sorted.\n";
}

// 6. Search
void searchShow() {
    string target;
    cout << "Enter title to search: ";
    cin.ignore(); getline(cin, target);

    auto it = find_if(queue.begin(), queue.end(), [&target](const Show& s) {
        return s.title == target;
    });

    if (it != queue.end()) {
        cout << "🎯 Found: " << it->title << " | " << it->genre << " | "
             << it->episodes << " episodes | Rating: " << it->rating << endl;
    } else {
        cout << "❌ Show not found.\n";
    }
}

// 7. Save
void saveQueueToFile(const string& filename) {
    ofstream out(filename);
    for (const auto& show : queue) {
        out << show.title << "|" << show.genre << "|" 
            << show.episodes << "|" << show.rating << "\n";
    }
    out.close();
}

// 8. Load
void loadQueueFromFile(const string& filename) {
    queue.clear();
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        Show s;
        string ep, rate;
        getline(ss, s.title, '|');
        getline(ss, s.genre, '|');
        getline(ss, ep, '|');
        getline(ss, rate, '|');
        try {
            s.episodes = stoi(ep);
            s.rating = stod(rate);
            queue.push_back(s);
        } catch (...) {
            continue; // Skip invalid lines
        }
    }
    in.close();
    cout << "📂 Queue loaded from file (" << queue.size() << " shows).\n";
}

// Helpers
bool confirmDoubleInput(double val, double min, double max) {
    return (val >= min && val <= max);
}

bool confirmIntInput(int val, int min) {
    return val >= min;
}

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <functional>

using namespace std;

// ==================== ANSI COLOR CODES ====================
namespace Color {
    const string RESET = "\033[0m";
    const string RED = "\033[91m";
    const string GREEN = "\033[92m";
    const string YELLOW = "\033[93m";
    const string BLUE = "\033[94m";
    const string MAGENTA = "\033[95m";
    const string CYAN = "\033[96m";
    const string WHITE = "\033[97m";
    const string BOLD = "\033[1m";
    const string DIM = "\033[2m";
}

// ==================== UTILITY FUNCTIONS ====================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader(const string& title) {
    cout << "\n" << Color::CYAN << Color::BOLD;
    cout << "  ╔════════════════════════════════════════════════════════╗\n";
    cout << "  ║" << Color::YELLOW << setw(40) << title << setw(18) << "" << Color::CYAN << "║\n";
    cout << "  ╚════════════════════════════════════════════════════════╝\n";
    cout << Color::RESET << "\n";
}

void printSubHeader(const string& title) {
    cout << Color::MAGENTA << "  ┌─ " << title << " ─┐" << Color::RESET << "\n\n";
}

void printSuccess(const string& msg) {
    cout << Color::GREEN << "  [OK] " << msg << Color::RESET << "\n";
}

void printError(const string& msg) {
    cout << Color::RED << "  [X] " << msg << Color::RESET << "\n";
}

void printInfo(const string& msg) {
    cout << Color::CYAN << "  [i] " << msg << Color::RESET << "\n";
}

void pressEnter() {
    cout << Color::DIM << "\n  Press Enter to continue..." << Color::RESET;
    cin.ignore();
    cin.get();
}

// ==================== HASH TABLE FOR PASSWORDS (DATA STRUCTURE 1) ====================
class HashTable {
private:
    static const int TABLE_SIZE = 101;
    
    struct HashNode {
        string username;
        size_t hashedPassword;
        string role;
        string societyName;
        HashNode* next;
        
        HashNode(string u, size_t p, string r, string s = "") 
            : username(u), hashedPassword(p), role(r), societyName(s), next(nullptr) {}
    };
    
    HashNode* table[TABLE_SIZE];
    
    int hashFunction(const string& key) {
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % TABLE_SIZE;
    }
    
public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }
    
    size_t hashPassword(const string& password) {
        hash<string> hasher;
        return hasher(password);
    }
    
    void insert(const string& username, const string& password, const string& role, const string& society = "") {
        int index = hashFunction(username);
        size_t hashedPwd = hashPassword(password);
        HashNode* newNode = new HashNode(username, hashedPwd, role, society);
        newNode->next = table[index];
        table[index] = newNode;
    }
    
    bool authenticate(const string& username, const string& password, string& role, string& society) {
        int index = hashFunction(username);
        HashNode* current = table[index];
        size_t hashedPwd = hashPassword(password);
        
        while (current) {
            if (current->username == username && current->hashedPassword == hashedPwd) {
                role = current->role;
                society = current->societyName;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    bool userExists(const string& username) {
        int index = hashFunction(username);
        HashNode* current = table[index];
        while (current) {
            if (current->username == username) return true;
            current = current->next;
        }
        return false;
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current) {
                file << current->username << "|" << current->hashedPassword << "|" 
                     << current->role << "|" << current->societyName << "\n";
                current = current->next;
            }
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, hashStr, role, society;
            getline(ss, username, '|');
            getline(ss, hashStr, '|');
            getline(ss, role, '|');
            getline(ss, society, '|');
            
            if (!username.empty()) {
                int index = hashFunction(username);
                HashNode* newNode = new HashNode(username, stoull(hashStr), role, society);
                newNode->next = table[index];
                table[index] = newNode;
            }
        }
        file.close();
    }
};

// ==================== LINKED LIST FOR SOCIETIES (DATA STRUCTURE 2) ====================
struct Society {
    string name;
    string president;
    string description;
    double budget;
    int memberCount;
    Society* next;
    
    Society(string n, string p, string d, double b) 
        : name(n), president(p), description(d), budget(b), memberCount(0), next(nullptr) {}
};

class SocietyList {
private:
    Society* head;
    int count;
    
public:
    SocietyList() : head(nullptr), count(0) {}
    
    void addSociety(const string& name, const string& president, const string& desc, double budget) {
        Society* newSociety = new Society(name, president, desc, budget);
        if (!head) {
            head = newSociety;
        } else {
            Society* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newSociety;
        }
        count++;
    }
    
    Society* findSociety(const string& name) {
        Society* temp = head;
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    
    bool deleteSociety(const string& name) {
        if (!head) return false;
        if (head->name == name) {
            Society* temp = head;
            head = head->next;
            delete temp;
            count--;
            return true;
        }
        Society* prev = head;
        Society* curr = head->next;
        while (curr) {
            if (curr->name == name) {
                prev->next = curr->next;
                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
    
    void displayAll() {
        if (!head) {
            printInfo("No societies registered yet.");
            return;
        }
        Society* temp = head;
        int idx = 1;
        cout << Color::CYAN << "  ┌────┬────────────────────┬────────────────────┬──────────────┐\n";
        cout << "  │ #  │ Society Name       │ President          │ Budget       │\n";
        cout << "  ├────┼────────────────────┼────────────────────┼──────────────┤\n" << Color::RESET;
        while (temp) {
            cout << Color::WHITE << "  │ " << setw(2) << idx++ << " │ " 
                 << setw(18) << left << temp->name.substr(0, 18) << " │ "
                 << setw(18) << left << temp->president.substr(0, 18) << " │ "
                 << Color::GREEN << "$" << setw(11) << right << fixed << setprecision(2) << temp->budget 
                 << Color::WHITE << " │\n" << Color::RESET;
            temp = temp->next;
        }
        cout << Color::CYAN << "  └────┴────────────────────┴────────────────────┴──────────────┘\n" << Color::RESET;
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        Society* temp = head;
        while (temp) {
            file << temp->name << "|" << temp->president << "|" 
                 << temp->description << "|" << temp->budget << "|" << temp->memberCount << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, president, desc, budgetStr, countStr;
            getline(ss, name, '|');
            getline(ss, president, '|');
            getline(ss, desc, '|');
            getline(ss, budgetStr, '|');
            getline(ss, countStr, '|');
            if (!name.empty()) {
                addSociety(name, president, desc, stod(budgetStr));
            }
        }
        file.close();
    }
    
    int getCount() { return count; }
    Society* getHead() { return head; }
};

// ==================== LINKED LIST FOR MEMBERS (DATA STRUCTURE 3) ====================
struct Member {
    string id;
    string name;
    string email;
    string societyName;
    string joinDate;
    Member* next;
    
    Member(string i, string n, string e, string s, string d) 
        : id(i), name(n), email(e), societyName(s), joinDate(d), next(nullptr) {}
};

class MemberList {
private:
    Member* head;
    int count;
    
public:
    MemberList() : head(nullptr), count(0) {}
    
    void addMember(const string& id, const string& name, const string& email, 
                   const string& society, const string& date) {
        Member* newMember = new Member(id, name, email, society, date);
        if (!head) {
            head = newMember;
        } else {
            Member* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newMember;
        }
        count++;
    }
    
    void displayBySociety(const string& societyName) {
        Member* temp = head;
        int idx = 1;
        bool found = false;
        
        cout << Color::CYAN << "  ┌────┬────────────┬────────────────────┬─────────────────────────┐\n";
        cout << "  │ #  │ ID         │ Name               │ Email                   │\n";
        cout << "  ├────┼────────────┼────────────────────┼─────────────────────────┤\n" << Color::RESET;
        
        while (temp) {
            if (temp->societyName == societyName) {
                found = true;
                cout << Color::WHITE << "  │ " << setw(2) << idx++ << " │ " 
                     << setw(10) << left << temp->id.substr(0, 10) << " │ "
                     << setw(18) << left << temp->name.substr(0, 18) << " │ "
                     << setw(23) << left << temp->email.substr(0, 23) << " │\n" << Color::RESET;
            }
            temp = temp->next;
        }
        cout << Color::CYAN << "  └────┴────────────┴────────────────────┴─────────────────────────┘\n" << Color::RESET;
        
        if (!found) {
            printInfo("No members found in this society.");
        }
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        Member* temp = head;
        while (temp) {
            file << temp->id << "|" << temp->name << "|" << temp->email << "|" 
                 << temp->societyName << "|" << temp->joinDate << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, email, society, date;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, society, '|');
            getline(ss, date, '|');
            if (!id.empty()) {
                addMember(id, name, email, society, date);
            }
        }
        file.close();
    }
    
    int getCount() { return count; }
};

// ==================== QUEUE FOR MEMBERSHIP REQUESTS (DATA STRUCTURE 4) ====================
struct MemberRequest {
    string id;
    string name;
    string email;
    string societyName;
    string requestDate;
    MemberRequest* next;
    
    MemberRequest(string i, string n, string e, string s, string d) 
        : id(i), name(n), email(e), societyName(s), requestDate(d), next(nullptr) {}
};

class RequestQueue {
private:
    MemberRequest* front;
    MemberRequest* rear;
    int count;
    
public:
    RequestQueue() : front(nullptr), rear(nullptr), count(0) {}
    
    void enqueue(const string& id, const string& name, const string& email, 
                 const string& society, const string& date) {
        MemberRequest* newReq = new MemberRequest(id, name, email, society, date);
        if (!rear) {
            front = rear = newReq;
        } else {
            rear->next = newReq;
            rear = newReq;
        }
        count++;
    }
    
    MemberRequest* dequeue() {
        if (!front) return nullptr;
        MemberRequest* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        count--;
        return temp;
    }
    
    MemberRequest* peek() { return front; }
    bool isEmpty() { return front == nullptr; }
    int getCount() { return count; }
    
    void displayAll() {
        if (!front) {
            printInfo("No pending membership requests.");
            return;
        }
        MemberRequest* temp = front;
        int idx = 1;
        
        cout << Color::YELLOW << "  ┌────┬────────────┬────────────────────┬────────────────────┐\n";
        cout << "  │ #  │ ID         │ Name               │ Society            │\n";
        cout << "  ├────┼────────────┼────────────────────┼────────────────────┤\n" << Color::RESET;
        
        while (temp) {
            cout << Color::WHITE << "  │ " << setw(2) << idx++ << " │ " 
                 << setw(10) << left << temp->id.substr(0, 10) << " │ "
                 << setw(18) << left << temp->name.substr(0, 18) << " │ "
                 << setw(18) << left << temp->societyName.substr(0, 18) << " │\n" << Color::RESET;
            temp = temp->next;
        }
        cout << Color::YELLOW << "  └────┴────────────┴────────────────────┴────────────────────┘\n" << Color::RESET;
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        MemberRequest* temp = front;
        while (temp) {
            file << temp->id << "|" << temp->name << "|" << temp->email << "|" 
                 << temp->societyName << "|" << temp->requestDate << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, email, society, date;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, society, '|');
            getline(ss, date, '|');
            if (!id.empty()) {
                enqueue(id, name, email, society, date);
            }
        }
        file.close();
    }
};

// ==================== STACK FOR PAST EVENTS HISTORY (DATA STRUCTURE 5) ====================
struct Event {
    string id;
    string name;
    string societyName;
    string date;
    string venue;
    int priority;
    double expense;
    
    Event(string i, string n, string s, string d, string v, int p, double e) 
        : id(i), name(n), societyName(s), date(d), venue(v), priority(p), expense(e) {}
};

class EventStack {
private:
    struct StackNode {
        Event* event;
        StackNode* next;
        StackNode(Event* e) : event(e), next(nullptr) {}
    };
    
    StackNode* top;
    int count;
    
public:
    EventStack() : top(nullptr), count(0) {}
    
    void push(Event* event) {
        StackNode* newNode = new StackNode(event);
        newNode->next = top;
        top = newNode;
        count++;
    }
    
    Event* pop() {
        if (!top) return nullptr;
        StackNode* temp = top;
        Event* event = temp->event;
        top = top->next;
        delete temp;
        count--;
        return event;
    }
    
    Event* peek() { return top ? top->event : nullptr; }
    bool isEmpty() { return top == nullptr; }
    int getCount() { return count; }
    
    void displayAll() {
        if (!top) {
            printInfo("No past events in history.");
            return;
        }
        StackNode* temp = top;
        int idx = 1;
        
        cout << Color::DIM << "  (Most recent first)\n" << Color::RESET;
        cout << Color::MAGENTA << "  ┌────┬────────────────────┬────────────────────┬────────────┐\n";
        cout << "  │ #  │ Event Name         │ Society            │ Date       │\n";
        cout << "  ├────┼────────────────────┼────────────────────┼────────────┤\n" << Color::RESET;
        
        while (temp) {
            cout << Color::WHITE << "  │ " << setw(2) << idx++ << " │ " 
                 << setw(18) << left << temp->event->name.substr(0, 18) << " │ "
                 << setw(18) << left << temp->event->societyName.substr(0, 18) << " │ "
                 << setw(10) << left << temp->event->date << " │\n" << Color::RESET;
            temp = temp->next;
        }
        cout << Color::MAGENTA << "  └────┴────────────────────┴────────────────────┴────────────┘\n" << Color::RESET;
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        StackNode* temp = top;
        while (temp) {
            file << temp->event->id << "|" << temp->event->name << "|" << temp->event->societyName << "|"
                 << temp->event->date << "|" << temp->event->venue << "|" << temp->event->priority << "|"
                 << temp->event->expense << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        EventStack tempStack;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, society, date, venue, prioStr, expStr;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, society, '|');
            getline(ss, date, '|');
            getline(ss, venue, '|');
            getline(ss, prioStr, '|');
            getline(ss, expStr, '|');
            if (!id.empty()) {
                Event* e = new Event(id, name, society, date, venue, stoi(prioStr), stod(expStr));
                tempStack.push(e);
            }
        }
        while (!tempStack.isEmpty()) {
            push(tempStack.pop());
        }
        file.close();
    }
};

// ==================== PRIORITY QUEUE FOR UPCOMING EVENTS (DATA STRUCTURE 6) ====================
class EventPriorityQueue {
private:
    struct PQNode {
        Event* event;
        PQNode* next;
        PQNode(Event* e) : event(e), next(nullptr) {}
    };
    
    PQNode* head;
    int count;
    
public:
    EventPriorityQueue() : head(nullptr), count(0) {}
    
    void insert(Event* event) {
        PQNode* newNode = new PQNode(event);
        
        if (!head || event->priority < head->event->priority ||
            (event->priority == head->event->priority && event->date < head->event->date)) {
            newNode->next = head;
            head = newNode;
        } else {
            PQNode* temp = head;
            while (temp->next && 
                   (temp->next->event->priority < event->priority ||
                   (temp->next->event->priority == event->priority && temp->next->event->date <= event->date))) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        count++;
    }
    
    Event* extractMin() {
        if (!head) return nullptr;
        PQNode* temp = head;
        Event* event = temp->event;
        head = head->next;
        delete temp;
        count--;
        return event;
    }
    
    bool isEmpty() { return head == nullptr; }
    int getCount() { return count; }
    
    void displayAll() {
        if (!head) {
            printInfo("No upcoming events scheduled.");
            return;
        }
        PQNode* temp = head;
        int idx = 1;
        
        cout << Color::GREEN << "  ┌────┬────────────────────┬────────────────────┬────────────┬──────────┐\n";
        cout << "  │ #  │ Event Name         │ Society            │ Date       │ Priority │\n";
        cout << "  ├────┼────────────────────┼────────────────────┼────────────┼──────────┤\n" << Color::RESET;
        
        while (temp) {
            string prioStr = temp->event->priority == 1 ? "HIGH" : 
                            (temp->event->priority == 2 ? "MEDIUM" : "LOW");
            string prioColor = temp->event->priority == 1 ? Color::RED : 
                              (temp->event->priority == 2 ? Color::YELLOW : Color::GREEN);
            
            cout << Color::WHITE << "  │ " << setw(2) << idx++ << " │ " 
                 << setw(18) << left << temp->event->name.substr(0, 18) << " │ "
                 << setw(18) << left << temp->event->societyName.substr(0, 18) << " │ "
                 << setw(10) << left << temp->event->date << " │ "
                 << prioColor << setw(8) << prioStr << Color::WHITE << " │\n" << Color::RESET;
            temp = temp->next;
        }
        cout << Color::GREEN << "  └────┴────────────────────┴────────────────────┴────────────┴──────────┘\n" << Color::RESET;
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        PQNode* temp = head;
        while (temp) {
            file << temp->event->id << "|" << temp->event->name << "|" << temp->event->societyName << "|"
                 << temp->event->date << "|" << temp->event->venue << "|" << temp->event->priority << "|"
                 << temp->event->expense << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name, society, date, venue, prioStr, expStr;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, society, '|');
            getline(ss, date, '|');
            getline(ss, venue, '|');
            getline(ss, prioStr, '|');
            getline(ss, expStr, '|');
            if (!id.empty()) {
                Event* e = new Event(id, name, society, date, venue, stoi(prioStr), stod(expStr));
                insert(e);
            }
        }
        file.close();
    }
};

// ==================== FINANCE TRACKING (DATA STRUCTURE 7 - LINKED LIST) ====================
struct Transaction {
    string id;
    string societyName;
    string type;
    double amount;
    string description;
    string date;
    Transaction* next;
    
    Transaction(string i, string s, string t, double a, string d, string dt) 
        : id(i), societyName(s), type(t), amount(a), description(d), date(dt), next(nullptr) {}
};

class FinanceTracker {
private:
    Transaction* head;
    int count;
    
public:
    FinanceTracker() : head(nullptr), count(0) {}
    
    void addTransaction(const string& id, const string& society, const string& type,
                        double amount, const string& desc, const string& date) {
        Transaction* newTrans = new Transaction(id, society, type, amount, desc, date);
        if (!head) {
            head = newTrans;
        } else {
            Transaction* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newTrans;
        }
        count++;
    }
    
    void displayBySociety(const string& societyName) {
        Transaction* temp = head;
        double totalIncome = 0, totalExpense = 0;
        bool found = false;
        
        cout << Color::BLUE << "  ┌──────┬──────────┬──────────────┬──────────────────────────────┐\n";
        cout << "  │ Type │ Amount   │ Date         │ Description                  │\n";
        cout << "  ├──────┼──────────┼──────────────┼──────────────────────────────┤\n" << Color::RESET;
        
        while (temp) {
            if (temp->societyName == societyName) {
                found = true;
                string typeColor = temp->type == "income" ? Color::GREEN : Color::RED;
                string typeStr = temp->type == "income" ? " + " : " - ";
                
                cout << Color::WHITE << "  │" << typeColor << setw(5) << typeStr << Color::WHITE << " │ $"
                     << setw(8) << fixed << setprecision(2) << temp->amount << " │ "
                     << setw(12) << left << temp->date << " │ "
                     << setw(28) << left << temp->description.substr(0, 28) << " │\n" << Color::RESET;
                
                if (temp->type == "income") totalIncome += temp->amount;
                else totalExpense += temp->amount;
            }
            temp = temp->next;
        }
        
        cout << Color::BLUE << "  └──────┴──────────┴──────────────┴──────────────────────────────┘\n" << Color::RESET;
        
        if (found) {
            cout << "\n";
            cout << Color::GREEN << "  Total Income:  $" << fixed << setprecision(2) << totalIncome << Color::RESET << "\n";
            cout << Color::RED << "  Total Expense: $" << fixed << setprecision(2) << totalExpense << Color::RESET << "\n";
            cout << Color::CYAN << "  ─────────────────────\n";
            cout << Color::BOLD << "  Net Balance:   $" << (totalIncome - totalExpense) << Color::RESET << "\n";
        } else {
            printInfo("No financial records found for this society.");
        }
    }
    
    void saveToFile(const string& filename) {
        ofstream file(filename);
        Transaction* temp = head;
        while (temp) {
            file << temp->id << "|" << temp->societyName << "|" << temp->type << "|"
                 << temp->amount << "|" << temp->description << "|" << temp->date << "\n";
            temp = temp->next;
        }
        file.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, society, type, amtStr, desc, date;
            getline(ss, id, '|');
            getline(ss, society, '|');
            getline(ss, type, '|');
            getline(ss, amtStr, '|');
            getline(ss, desc, '|');
            getline(ss, date, '|');
            if (!id.empty()) {
                addTransaction(id, society, type, stod(amtStr), desc, date);
            }
        }
        file.close();
    }
};

// ==================== QUICK SORT FOR RANKING (SORTING ALGORITHM) ====================
void swap(Society** a, Society** b) {
    Society* temp = *a;
    *a = *b;
    *b = temp;
}

void quickSort(Society** arr, int low, int high) {
    if (low < high) {
        double pivot = arr[high]->budget;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j]->budget >= pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void displayRankedSocieties(SocietyList& societies) {
    int n = societies.getCount();
    if (n == 0) {
        printInfo("No societies to rank.");
        return;
    }
    
    Society** arr = new Society*[n];
    Society* temp = societies.getHead();
    for (int i = 0; i < n; i++) {
        arr[i] = temp;
        temp = temp->next;
    }
    
    quickSort(arr, 0, n - 1);
    
    cout << Color::YELLOW << "  ┌──────┬────────────────────────────┬──────────────┐\n";
    cout << "  │ Rank │ Society Name               │ Budget       │\n";
    cout << "  ├──────┼────────────────────────────┼──────────────┤\n" << Color::RESET;
    
    for (int i = 0; i < n; i++) {
        string medal = i == 0 ? " 1st" : (i == 1 ? " 2nd" : (i == 2 ? " 3rd" : "    "));
        string medalColor = i == 0 ? Color::YELLOW : (i == 1 ? Color::WHITE : (i == 2 ? Color::MAGENTA : Color::WHITE));
        cout << Color::WHITE << "  │" << medalColor << setw(5) << medal << Color::WHITE << " │ "
             << setw(26) << left << arr[i]->name << " │ "
             << Color::GREEN << "$" << setw(11) << right << fixed << setprecision(2) << arr[i]->budget 
             << Color::WHITE << " │\n" << Color::RESET;
    }
    cout << Color::YELLOW << "  └──────┴────────────────────────────┴──────────────┘\n" << Color::RESET;
    
    delete[] arr;
}

// ==================== MAIN SYSTEM CLASS ====================
class SocietyManagementSystem {
private:
    HashTable users;
    SocietyList societies;
    MemberList members;
    RequestQueue requests;
    EventStack pastEvents;
    EventPriorityQueue upcomingEvents;
    FinanceTracker finances;
    
    string currentUser;
    string currentRole;
    string currentSociety;
    int eventCounter;
    int transactionCounter;
    
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        ss << setfill('0') << setw(4) << (1900 + ltm->tm_year) << "-"
           << setw(2) << (1 + ltm->tm_mon) << "-" << setw(2) << ltm->tm_mday;
        return ss.str();
    }
    
    void initializeDefaultData() {
        if (!users.userExists("admin")) {
            users.insert("admin", "admin123", "admin");
        }
        
        if (societies.getCount() == 0) {
            societies.addSociety("Tech Society", "Alice", "Technology & Innovation", 5000.0);
            societies.addSociety("Arts Club", "Bob", "Creative Arts & Design", 3000.0);
            societies.addSociety("Sports Society", "Charlie", "Athletics & Games", 4000.0);
            
            users.insert("alice", "alice123", "president", "Tech Society");
            users.insert("bob", "bob123", "president", "Arts Club");
            users.insert("charlie", "charlie123", "president", "Sports Society");
            users.insert("member1", "pass123", "member");
        }
    }
    
public:
    SocietyManagementSystem() : eventCounter(1), transactionCounter(1) {
        loadAllData();
        initializeDefaultData();
    }
    
    void loadAllData() {
        users.loadFromFile("users.dat");
        societies.loadFromFile("societies.dat");
        members.loadFromFile("members.dat");
        requests.loadFromFile("requests.dat");
        pastEvents.loadFromFile("past_events.dat");
        upcomingEvents.loadFromFile("upcoming_events.dat");
        finances.loadFromFile("finances.dat");
    }
    
    void saveAllData() {
        users.saveToFile("users.dat");
        societies.saveToFile("societies.dat");
        members.saveToFile("members.dat");
        requests.saveToFile("requests.dat");
        pastEvents.saveToFile("past_events.dat");
        upcomingEvents.saveToFile("upcoming_events.dat");
        finances.saveToFile("finances.dat");
    }
    
    // ==================== 3-ATTEMPT LOGIN SYSTEM ====================
    bool login() {
        clearScreen();
        printHeader("FAST UNIVERSITY SOCIETY MANAGEMENT");
        
        cout << Color::CYAN << "  ╭─────────────────────────────────────╮\n";
        cout << "  │          " << Color::YELLOW << "SECURE LOGIN" << Color::CYAN << "               │\n";
        cout << "  ╰─────────────────────────────────────╯\n\n" << Color::RESET;
        
        int attempts = 3;
        
        while (attempts > 0) {
            string username, password;
            
            cout << Color::WHITE << "  Username: " << Color::RESET;
            cin >> username;
            cout << Color::WHITE << "  Password: " << Color::RESET;
            cin >> password;
            
            if (users.authenticate(username, password, currentRole, currentSociety)) {
                currentUser = username;
                printSuccess("Login successful! Welcome, " + username);
                pressEnter();
                return true;
            }
            
            attempts--;
            if (attempts > 0) {
                printError("Invalid credentials! " + to_string(attempts) + " attempt(s) remaining.");
                cout << "\n";
            }
        }
        
        printError("Account locked! Too many failed attempts.");
        pressEnter();
        return false;
    }
    
    // ==================== ADMIN MENU ====================
    void adminMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("ADMIN DASHBOARD");
            
            cout << Color::CYAN << "  ┌─────────────────────────────────────┐\n";
            cout << "  │  " << Color::WHITE << "1." << Color::YELLOW << " Manage Societies              " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "2." << Color::YELLOW << " View All Members               " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "3." << Color::YELLOW << " Process Membership Requests    " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "4." << Color::YELLOW << " View All Events                " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "5." << Color::YELLOW << " Financial Reports              " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "6." << Color::YELLOW << " Society Rankings               " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "7." << Color::YELLOW << " Register New User              " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "0." << Color::RED << " Logout                         " << Color::CYAN << "│\n";
            cout << "  └─────────────────────────────────────┘\n\n" << Color::RESET;
            
            cout << Color::WHITE << "  Enter choice: " << Color::RESET;
            cin >> choice;
            
            switch (choice) {
                case 1: manageSocieties(); break;
                case 2: viewAllMembers(); break;
                case 3: processRequests(); break;
                case 4: viewAllEvents(); break;
                case 5: financialReports(); break;
                case 6: 
                    clearScreen();
                    printHeader("SOCIETY RANKINGS");
                    displayRankedSocieties(societies);
                    pressEnter();
                    break;
                case 7: registerNewUser(); break;
                case 0: 
                    saveAllData();
                    printInfo("Logging out...");
                    break;
            }
        } while (choice != 0);
    }
    
    void manageSocieties() {
        int choice;
        do {
            clearScreen();
            printHeader("SOCIETY MANAGEMENT");
            
            cout << Color::CYAN << "  ┌─────────────────────────────────────┐\n";
            cout << "  │  " << Color::WHITE << "1." << Color::YELLOW << " View All Societies            " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "2." << Color::YELLOW << " Add New Society               " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "3." << Color::YELLOW << " Delete Society                " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "0." << Color::RED << " Back                           " << Color::CYAN << "│\n";
            cout << "  └─────────────────────────────────────┘\n\n" << Color::RESET;
            
            cout << Color::WHITE << "  Enter choice: " << Color::RESET;
            cin >> choice;
            
            if (choice == 1) {
                clearScreen();
                printHeader("ALL SOCIETIES");
                societies.displayAll();
                pressEnter();
            } else if (choice == 2) {
                clearScreen();
                printHeader("ADD NEW SOCIETY");
                
                string name, president, desc;
                double budget;
                
                cin.ignore();
                cout << Color::WHITE << "  Society Name: " << Color::RESET;
                getline(cin, name);
                cout << Color::WHITE << "  President Name: " << Color::RESET;
                getline(cin, president);
                cout << Color::WHITE << "  Description: " << Color::RESET;
                getline(cin, desc);
                cout << Color::WHITE << "  Initial Budget: $" << Color::RESET;
                cin >> budget;
                
                societies.addSociety(name, president, desc, budget);
                printSuccess("Society '" + name + "' added successfully!");
                pressEnter();
            } else if (choice == 3) {
                clearScreen();
                printHeader("DELETE SOCIETY");
                societies.displayAll();
                
                string name;
                cin.ignore();
                cout << Color::WHITE << "\n  Enter society name to delete: " << Color::RESET;
                getline(cin, name);
                
                if (societies.deleteSociety(name)) {
                    printSuccess("Society deleted successfully!");
                } else {
                    printError("Society not found!");
                }
                pressEnter();
            }
        } while (choice != 0);
    }
    
    void viewAllMembers() {
        clearScreen();
        printHeader("SELECT SOCIETY");
        societies.displayAll();
        
        string society;
        cin.ignore();
        cout << Color::WHITE << "\n  Enter society name: " << Color::RESET;
        getline(cin, society);
        
        clearScreen();
        printHeader("MEMBERS OF " + society);
        members.displayBySociety(society);
        pressEnter();
    }
    
    void processRequests() {
        clearScreen();
        printHeader("MEMBERSHIP REQUESTS");
        requests.displayAll();
        
        if (!requests.isEmpty()) {
            cout << Color::WHITE << "\n  Process next request? (y/n): " << Color::RESET;
            char ch;
            cin >> ch;
            
            if (ch == 'y' || ch == 'Y') {
                MemberRequest* req = requests.dequeue();
                if (req) {
                    cout << Color::WHITE << "  Approve request for " << req->name << "? (y/n): " << Color::RESET;
                    cin >> ch;
                    
                    if (ch == 'y' || ch == 'Y') {
                        members.addMember(req->id, req->name, req->email, req->societyName, getCurrentDate());
                        printSuccess("Member " + req->name + " approved and added!");
                    } else {
                        printInfo("Request rejected.");
                    }
                    delete req;
                }
            }
        }
        pressEnter();
    }
    
    void viewAllEvents() {
        int choice;
        do {
            clearScreen();
            printHeader("EVENT MANAGEMENT");
            
            cout << Color::CYAN << "  ┌─────────────────────────────────────┐\n";
            cout << "  │  " << Color::WHITE << "1." << Color::YELLOW << " Upcoming Events               " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "2." << Color::YELLOW << " Past Events History           " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "0." << Color::RED << " Back                           " << Color::CYAN << "│\n";
            cout << "  └─────────────────────────────────────┘\n\n" << Color::RESET;
            
            cout << Color::WHITE << "  Enter choice: " << Color::RESET;
            cin >> choice;
            
            if (choice == 1) {
                clearScreen();
                printHeader("UPCOMING EVENTS");
                upcomingEvents.displayAll();
                pressEnter();
            } else if (choice == 2) {
                clearScreen();
                printHeader("PAST EVENTS HISTORY");
                pastEvents.displayAll();
                pressEnter();
            }
        } while (choice != 0);
    }
    
    void financialReports() {
        clearScreen();
        printHeader("FINANCIAL REPORTS");
        societies.displayAll();
        
        string society;
        cin.ignore();
        cout << Color::WHITE << "\n  Enter society name: " << Color::RESET;
        getline(cin, society);
        
        clearScreen();
        printHeader("FINANCES: " + society);
        finances.displayBySociety(society);
        pressEnter();
    }
    
    void registerNewUser() {
        clearScreen();
        printHeader("REGISTER NEW USER");
        
        string username, password, role, society;
        
        cin.ignore();
        cout << Color::WHITE << "  Username: " << Color::RESET;
        getline(cin, username);
        
        if (users.userExists(username)) {
            printError("Username already exists!");
            pressEnter();
            return;
        }
        
        cout << Color::WHITE << "  Password: " << Color::RESET;
        getline(cin, password);
        
        cout << Color::CYAN << "\n  Roles: admin, president, member\n" << Color::RESET;
        cout << Color::WHITE << "  Role: " << Color::RESET;
        getline(cin, role);
        
        if (role == "president" || role == "member") {
            societies.displayAll();
            cout << Color::WHITE << "\n  Society Name: " << Color::RESET;
            getline(cin, society);
        }
        
        users.insert(username, password, role, society);
        printSuccess("User '" + username + "' registered successfully!");
        pressEnter();
    }
    
    // ==================== PRESIDENT MENU ====================
    void presidentMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("PRESIDENT DASHBOARD - " + currentSociety);
            
            cout << Color::CYAN << "  ┌─────────────────────────────────────┐\n";
            cout << "  │  " << Color::WHITE << "1." << Color::YELLOW << " View Society Members          " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "2." << Color::YELLOW << " Process Join Requests         " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "3." << Color::YELLOW << " Schedule New Event            " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "4." << Color::YELLOW << " Mark Event as Complete        " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "5." << Color::YELLOW << " View Upcoming Events          " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "6." << Color::YELLOW << " View Past Events              " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "7." << Color::YELLOW << " Add Transaction               " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "8." << Color::YELLOW << " View Finances                 " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "0." << Color::RED << " Logout                         " << Color::CYAN << "│\n";
            cout << "  └─────────────────────────────────────┘\n\n" << Color::RESET;
            
            cout << Color::WHITE << "  Enter choice: " << Color::RESET;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    clearScreen();
                    printHeader("SOCIETY MEMBERS");
                    members.displayBySociety(currentSociety);
                    pressEnter();
                    break;
                case 2: processRequests(); break;
                case 3: scheduleEvent(); break;
                case 4: markEventComplete(); break;
                case 5:
                    clearScreen();
                    printHeader("UPCOMING EVENTS");
                    upcomingEvents.displayAll();
                    pressEnter();
                    break;
                case 6:
                    clearScreen();
                    printHeader("PAST EVENTS HISTORY");
                    pastEvents.displayAll();
                    pressEnter();
                    break;
                case 7: addTransaction(); break;
                case 8:
                    clearScreen();
                    printHeader("SOCIETY FINANCES");
                    finances.displayBySociety(currentSociety);
                    pressEnter();
                    break;
                case 0:
                    saveAllData();
                    printInfo("Logging out...");
                    break;
            }
        } while (choice != 0);
    }
    
    void scheduleEvent() {
        clearScreen();
        printHeader("SCHEDULE NEW EVENT");
        
        string name, date, venue;
        int priority;
        double budget;
        
        cin.ignore();
        cout << Color::WHITE << "  Event Name: " << Color::RESET;
        getline(cin, name);
        cout << Color::WHITE << "  Date (YYYY-MM-DD): " << Color::RESET;
        getline(cin, date);
        cout << Color::WHITE << "  Venue: " << Color::RESET;
        getline(cin, venue);
        cout << Color::CYAN << "\n  Priority: 1=High, 2=Medium, 3=Low\n" << Color::RESET;
        cout << Color::WHITE << "  Priority: " << Color::RESET;
        cin >> priority;
        cout << Color::WHITE << "  Estimated Budget: $" << Color::RESET;
        cin >> budget;
        
        string eventId = "E" + to_string(eventCounter++);
        Event* event = new Event(eventId, name, currentSociety, date, venue, priority, budget);
        upcomingEvents.insert(event);
        
        printSuccess("Event '" + name + "' scheduled successfully!");
        pressEnter();
    }
    
    void markEventComplete() {
        clearScreen();
        printHeader("MARK EVENT COMPLETE");
        upcomingEvents.displayAll();
        
        if (!upcomingEvents.isEmpty()) {
            cout << Color::WHITE << "\n  Mark next event as complete? (y/n): " << Color::RESET;
            char ch;
            cin >> ch;
            
            if (ch == 'y' || ch == 'Y') {
                Event* event = upcomingEvents.extractMin();
                if (event) {
                    pastEvents.push(event);
                    printSuccess("Event '" + event->name + "' moved to past events!");
                }
            }
        }
        pressEnter();
    }
    
    void addTransaction() {
        clearScreen();
        printHeader("ADD TRANSACTION");
        
        string type, desc;
        double amount;
        
        cout << Color::CYAN << "  Type: income / expense\n" << Color::RESET;
        cout << Color::WHITE << "  Transaction Type: " << Color::RESET;
        cin >> type;
        cout << Color::WHITE << "  Amount: $" << Color::RESET;
        cin >> amount;
        cin.ignore();
        cout << Color::WHITE << "  Description: " << Color::RESET;
        getline(cin, desc);
        
        string transId = "T" + to_string(transactionCounter++);
        finances.addTransaction(transId, currentSociety, type, amount, desc, getCurrentDate());
        
        printSuccess("Transaction recorded successfully!");
        pressEnter();
    }
    
    // ==================== MEMBER MENU ====================
    void memberMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("MEMBER DASHBOARD");
            
            cout << Color::CYAN << "  ┌─────────────────────────────────────┐\n";
            cout << "  │  " << Color::WHITE << "1." << Color::YELLOW << " View All Societies            " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "2." << Color::YELLOW << " Request to Join Society       " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "3." << Color::YELLOW << " View Upcoming Events          " << Color::CYAN << "│\n";
            cout << "  │  " << Color::WHITE << "0." << Color::RED << " Logout                         " << Color::CYAN << "│\n";
            cout << "  └─────────────────────────────────────┘\n\n" << Color::RESET;
            
            cout << Color::WHITE << "  Enter choice: " << Color::RESET;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    clearScreen();
                    printHeader("ALL SOCIETIES");
                    societies.displayAll();
                    pressEnter();
                    break;
                case 2: requestJoinSociety(); break;
                case 3:
                    clearScreen();
                    printHeader("UPCOMING EVENTS");
                    upcomingEvents.displayAll();
                    pressEnter();
                    break;
                case 0:
                    saveAllData();
                    printInfo("Logging out...");
                    break;
            }
        } while (choice != 0);
    }
    
    void requestJoinSociety() {
        clearScreen();
        printHeader("JOIN A SOCIETY");
        societies.displayAll();
        
        string name, email, society;
        cin.ignore();
        
        cout << Color::WHITE << "\n  Your Name: " << Color::RESET;
        getline(cin, name);
        cout << Color::WHITE << "  Your Email: " << Color::RESET;
        getline(cin, email);
        cout << Color::WHITE << "  Society to Join: " << Color::RESET;
        getline(cin, society);
        
        if (societies.findSociety(society)) {
            requests.enqueue(currentUser, name, email, society, getCurrentDate());
            printSuccess("Join request submitted! Awaiting approval.");
        } else {
            printError("Society not found!");
        }
        pressEnter();
    }
    
    // ==================== MAIN RUN FUNCTION ====================
    void run() {
        bool running = true;
        
        while (running) {
            if (login()) {
                if (currentRole == "admin") {
                    adminMenu();
                } else if (currentRole == "president") {
                    presidentMenu();
                } else {
                    memberMenu();
                }
            }
            
            clearScreen();
            printHeader("SESSION ENDED");
            cout << Color::WHITE << "  Login again? (y/n): " << Color::RESET;
            char ch;
            cin >> ch;
            running = (ch == 'y' || ch == 'Y');
        }
        
        saveAllData();
        clearScreen();
        cout << Color::GREEN << Color::BOLD;
        cout << "\n  ============================================================\n";
        cout << "          Thank you for using the Society System!            \n";
        cout << "                        Goodbye!                             \n";
        cout << "  ============================================================\n\n";
        cout << Color::RESET;
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    SocietyManagementSystem system;
    system.run();
    return 0;
}

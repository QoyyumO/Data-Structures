#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>

using namespace std;

// Participant Node for Linked List
class ParticipantNode {
public:
    string name;
    string studentID;
    string contactInfo;
    ParticipantNode* next;

    ParticipantNode(string n = "", string id = "", string contact = "") 
        : name(n), studentID(id), contactInfo(contact), next(nullptr) {}
};

// Linked List for Participants
class ParticipantList {
private:
    ParticipantNode* head;

public:
    ParticipantList() : head(nullptr) {}

    void addParticipant(const string& name, const string& studentID, const string& contactInfo) {
        ParticipantNode* newNode = new ParticipantNode(name, studentID, contactInfo);
        newNode->next = head;
        head = newNode;
    }

    void removeParticipant(const string& studentID) {
        ParticipantNode* current = head;
        ParticipantNode* prev = nullptr;
        while (current) {
            if (current->studentID == studentID) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    void displayParticipants() const {
        ParticipantNode* current = head;
        while (current) {
            cout << "- " << current->name 
                 << " (ID: " << current->studentID << ")\n";
            current = current->next;
        }
    }

    ~ParticipantList() {
        while (head) {
            ParticipantNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Node for BST
class EventNode {
public:
    string name;
    string category;
    EventNode* left;
    EventNode* right;
    ParticipantList participants;

    EventNode(string n, string c) : name(n), category(c), left(nullptr), right(nullptr) {}
};

// BST for Event Searching and Organization
class EventBST {
public:
    EventNode* root;

    EventBST() : root(nullptr) {}

    void insert(string name, string category) {
        root = insertHelper(root, name, category);
    }

    EventNode* search(string name) {
        return searchHelper(root, name);
    }

    void displayByCategory(const string& category) {
        displayByCategoryHelper(root, category);
    }

    void inorderTraversal() {
        inorderHelper(root);
    }

    void deleteEvent(const string& name) {
        root = deleteHelper(root, name);
    }

private:
    EventNode* insertHelper(EventNode* node, string name, string category) {
        if (!node) return new EventNode(name, category);
        if (name < node->name) node->left = insertHelper(node->left, name, category);
        else if (name > node->name) node->right = insertHelper(node->right, name, category);
        return node;
    }

    EventNode* searchHelper(EventNode* node, string name) {
        if (!node || node->name == name) return node;
        if (name < node->name) return searchHelper(node->left, name);
        return searchHelper(node->right, name);
    }

    void displayByCategoryHelper(EventNode* node, const string& category) {
        if (node) {
            displayByCategoryHelper(node->left, category);
            if (node->category == category) {
                cout << "Event: " << node->name << "\n";
                node->participants.displayParticipants();
            }
            displayByCategoryHelper(node->right, category);
        }
    }

    void inorderHelper(EventNode* node) {
        if (node) {
            inorderHelper(node->left);
            cout << "Event: " << node->name 
                 << " (Category: " << node->category << ")\n";
            node->participants.displayParticipants();
            inorderHelper(node->right);
        }
    }

    EventNode* deleteHelper(EventNode* node, const string& name) {
        if (!node) return node;
        if (name < node->name) node->left = deleteHelper(node->left, name);
        else if (name > node->name) node->right = deleteHelper(node->right, name);
        else {
            if (!node->left) {
                EventNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                EventNode* temp = node->left;
                delete node;
                return temp;
            }

            EventNode* temp = findMin(node->right);
            node->name = temp->name;
            node->category = temp->category;
            node->right = deleteHelper(node->right, temp->name);
        }
        return node;
    }

    EventNode* findMin(EventNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }
};

// Event Management System Class
class EventManagementSystem {
private:
    EventBST eventBST;
    priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> scheduledEvents;
    stack<EventNode*> undoStack;
    stack<EventNode*> redoStack;
    queue<ParticipantNode*> checkInQueue;

public:
    // Create Event
    void createEvent(const string& name, const string& category, int priority) {
        EventNode* event = new EventNode(name, category);
        eventBST.insert(name, category);
        scheduledEvents.push({priority, event});
        undoStack.push(event);
        while (!redoStack.empty()) redoStack.pop();
    }

    // View Events by Category
    void viewEventsByCategory(const string& category) {
        cout << "Events in " << category << " category:\n";
        eventBST.displayByCategory(category);
    }

    // View Scheduled Events
    void viewScheduledEvents() {
        priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> temp = scheduledEvents;
        cout << "Scheduled Events (Priority Order):\n";
        while (!temp.empty()) {
            auto [priority, event] = temp.top();
            temp.pop();
            cout << "- " << event->name << " (Priority: " << priority << ")\n";
        }
    }

    // Update Event
    void updateEvent(string oldName, const string& newName, const string& category, int priority) {
        EventNode* event = eventBST.search(oldName);
        if (event) {
            undoStack.push(event);
            eventBST.deleteEvent(oldName);
            createEvent(newName, category, priority);
            cout << "Event updated successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    // Delete Event
    void deleteEvent(const string& name) {
        EventNode* event = eventBST.search(name);
        if (event) {
            undoStack.push(event);
            eventBST.deleteEvent(name);
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    // Register Participant
    void registerParticipant(const string& eventName, const string& name, const string& studentID, const string& contactInfo) {
        EventNode* event = eventBST.search(eventName);
        if (event) {
            event->participants.addParticipant(name, studentID, contactInfo);
            checkInQueue.push(new ParticipantNode(name, studentID, contactInfo));
            cout << "Participant registered successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    // Process Check-in
    void processCheckIn() {
        if (!checkInQueue.empty()) {
            ParticipantNode* p = checkInQueue.front();
            checkInQueue.pop();
            cout << "Checked in: " << p->name << " (ID: " << p->studentID << ")\n";
            delete p;
        } else {
            cout << "No participants in check-in queue.\n";
        }
    }

    // View Next Check-in
    void viewNextCheckIn() {
        if (!checkInQueue.empty()) {
            ParticipantNode* p = checkInQueue.front();
            cout << "Next in line: " << p->name << " (ID: " << p->studentID << ")\n";
        } else {
            cout << "No participants in check-in queue.\n";
        }
    }

    // Undo Operation
    void undoOperation() {
        if (!undoStack.empty()) {
            EventNode* lastEvent = undoStack.top();
            undoStack.pop();
            redoStack.push(lastEvent);
            cout << "Last operation undone.\n";
        } else {
            cout << "No operations to undo.\n";
        }
    }

    // Redo Operation
    void redoOperation() {
        if (!redoStack.empty()) {
            EventNode* lastEvent = redoStack.top();
            redoStack.pop();
            undoStack.push(lastEvent);
            cout << "Last operation redone.\n";
        } else {
            cout << "No operations to redo.\n";
        }
    }
    void generateEventReport() {
    cout << "----- Event Report -----\n";
    eventBST.inorderTraversal(); // Assuming this method displays events and their participants
    cout << "-------------------------\n";
    }
    void generateParticipantReport() {
    cout << "----- Participant Report -----\n";
    // Assuming you have a way to iterate through all events
    // You might need to modify EventBST to allow this
    // For now, we will just display participants for each event
    eventBST.inorderTraversal(); // This should be modified to show only participants
    cout << "-------------------------------\n";
    }
    void generateCheckInStatistics() {
    cout << "----- Check-in Statistics -----\n";
    cout << "Total Check-ins: " << checkInQueue.size() << "\n";
    cout << "Participants Checked In:\n";
    
    // Create a temporary queue to display participants without modifying the original queue
    queue<ParticipantNode*> tempQueue = checkInQueue;
    while (!tempQueue.empty()) {
        ParticipantNode* p = tempQueue.front();
        cout << "- " << p->name << " (ID: " << p->studentID << ")\n";
        tempQueue.pop();
    }
    cout << "-------------------------------\n";
}



};

int main() {
    EventManagementSystem ems;
    int choice;

    do {
        cout << "\n--- Event Management System ---\n";
        cout << "1. Create Event\n";
        cout << "2. View Events by Category\n";
        cout << "3. View Scheduled Events\n";
        cout << "4. Update Event\n";
        cout << "5. Delete Event\n";
        cout << "6. Register Participant\n";
        cout << "7. Process Check-in\n";
        cout << "8. View Next Check-in\n";
        cout << "9. Undo Last Operation\n";
        cout << "10. Redo Last Operation\n";
        cout << "11. Generate Event Report\n"; // New option
        cout << "12. Generate Participant Report\n"; // New option
        cout << "13. Generate Check-in Statistics\n"; // New option
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, category;
                int priority;
                cout << "Enter event name: ";
                getline(cin, name);
                cin.ignore();
                cout << "Enter event category: ";
                getline(cin, category);
                cin.ignore();
                cout << "Enter event priority: ";
                cin >> priority;
                ems.createEvent(name, category, priority);
                break;
            }
            case 2: {
                string category;
                cout << "Enter category: ";
                getline(cin,category);
                cin.ignore();
                ems.viewEventsByCategory(category);
                break;
            }
            case 3:
                ems.viewScheduledEvents();
                break;
            case 4: {
                string oldName, newName, category;
                int priority;
                cout << "Enter current event name: ";
                getline(cin, oldName);
                cin.ignore();
                cout << "Enter new event name: ";
                getline(cin, newName);
                cin.ignore();
                cout << "Enter new category: ";
                getline(cin, category);
                cin.ignore();
                cout << "Enter new priority: ";
                cin >> priority;
                ems.updateEvent(oldName, newName, category, priority);
                break;
            }
            case 5: {
                string name;
                cout << "Enter event name: ";
                getline(cin, name);
                cin.ignore();
                ems.deleteEvent(name);
                break;
            }
            case 6: {
                string eventName, name, studentID, contactInfo;
                cout << "Enter event name: ";
                getline(cin, eventName);
                cin.ignore();
                cout << "Enter participant name: ";
                getline(cin, name);
                cin.ignore();
                cout << "Enter participant ID: ";
                getline(cin, studentID);
                cin.ignore();
                cout << "Enter participant contact info: ";
                getline(cin, contactInfo);
                ems.registerParticipant(eventName, name, studentID, contactInfo);
                break;
            }
            case 7:
                ems.processCheckIn();
                break;
            case 8:
                ems.viewNextCheckIn();
                break;
            case 9:
                ems.undoOperation();
                break;
            case 10:
                ems.redoOperation();
                break;
            case 11:
                ems.generateEventReport();
                break;
            case 12:
                ems.generateParticipantReport();
                break;
            case 13:
                ems.generateCheckInStatistics();
                break;
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}

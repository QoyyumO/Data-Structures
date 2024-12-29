#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// Node for BST
class EventNode {
public:
    string name;
    string category;
    EventNode* left;
    EventNode* right;
    list<pair<string, string>> participants; // Pair of participant name and ID
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
        string lowerCategory = category;
        transform(lowerCategory.begin(), lowerCategory.end(), lowerCategory.begin(), ::tolower);
        displayByCategoryHelper(root, lowerCategory);
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
            
            // Convert the event's category to lower case for comparison
            string eventCategory = node->category;
            transform(eventCategory.begin(), eventCategory.end(), eventCategory.begin(), ::tolower);
            
            if (eventCategory == category) {
                cout << "Event: " << node->name << "\n";
                for (const auto& participant : node->participants) {
                    cout << "- " << participant.first << " (ID: " << participant.second << ")\n";
                }
            }
            
            displayByCategoryHelper(node->right, category);
        }
    }

    void inorderHelper(EventNode* node) {
        if (node) {
            inorderHelper(node->left);
            cout << "Event: " << node->name 
                 << " (Category: " << node->category << ")\n";
            for (const auto& participant : node->participants) {
                cout << "- " << participant.first << " (ID: " << participant.second << ")\n";
            }
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

// Unified Action Structure for Undo/Redo
struct Action {
    enum ActionType { REGISTER_PARTICIPANT, UPDATE_EVENT } type;
    string eventName; // For both actions
    string participantName; // For registration
    string participantID; // For registration
    string oldEventName; // For event update
    string newEventName; // For event update
    string category; // For event update
    int priority; // For event update
};

// Event Management System Class
class EventManagementSystem {
private:
    EventBST eventBST;
    priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> scheduledEvents;
    stack<Action> undoStack; // Unified stack for undo actions
    stack<Action> redoStack; // Unified stack for redo actions
    queue<pair<string, string>> checkInQueue; // Pair of participant name and ID
    queue<pair<string,string>> checkedInParticipants; // To track checked-in participants
    int participantIDCounter = 1; // Automatic ID counter

public:
    // Create Event
    void createEvent(const string& name, const string& category, int priority) {
        EventNode* event = new EventNode(name, category);
        eventBST.insert(name, category);
        scheduledEvents.push({priority, event});
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
            // Store the old event details for undo
            string oldCategory = event->category;

            // Remove the old event from the scheduled events queue
            priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> tempQueue;
            while (!scheduledEvents.empty()) {
                auto scheduledEvent = scheduledEvents.top();
                scheduledEvents.pop();
                if (scheduledEvent.second->name != oldName) {
                    tempQueue.push(scheduledEvent);
                }
            }
            scheduledEvents = tempQueue; // Update the scheduled events queue

            // Proceed with the update
            eventBST.deleteEvent(oldName);
            eventBST.insert(newName, category);

            // Add the updated event back to the scheduled events queue
            EventNode* updatedEvent = eventBST.search(newName);
            if (updatedEvent) {
                scheduledEvents.push({priority, updatedEvent});
            }

            cout << "Event updated successfully.\n";

            // Push the action onto the undo stack for the event update
            Action action = {Action::UPDATE_EVENT, "", "", "", oldName, newName, oldCategory, priority};
            undoStack.push(action);
            while (!redoStack.empty()) redoStack.pop(); // Clear redo stack
        } else {
            cout << "Event not found for update.\n";
        }
    }


    // Delete Event
    void deleteEvent(const string& name) {
        EventNode* event = eventBST.search(name);
        if (event) {
            // Remove the event from the scheduled events queue
            // Create a temporary queue to hold scheduled events
            priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> tempQueue;
            while (!scheduledEvents.empty()) {
                auto scheduledEvent = scheduledEvents.top();
                scheduledEvents.pop();
                if (scheduledEvent.second->name != name) {
                    tempQueue.push(scheduledEvent); // Keep events that are not being deleted
                }
            }
            scheduledEvents = tempQueue; // Update the scheduled events queue

            eventBST.deleteEvent(name);
            cout << "Event deleted successfully.\n";
        } else {
            cout << "Event not found.\n";
        }
    }

    // Register Participant
    void registerParticipant(const string& eventName, const string& name) {
        EventNode* event = eventBST.search(eventName);
        if (event) {
            string participantID = "P" + to_string(participantIDCounter++);
            event->participants.push_back({name, participantID});
            checkInQueue.push({name, participantID});
            cout << "Participant registered successfully.\n";

            // Push the action onto the undo stack
            Action action = {Action::REGISTER_PARTICIPANT, eventName, name, participantID, "", "", "", 0};
            undoStack.push(action);
            while (!redoStack.empty()) redoStack.pop(); // Clear redo stack
        } else {
            cout << "Event not found.\n";
        }
    }

    // Process Check-in
    void processCheckIn() {
        if (!checkInQueue.empty()) {
            auto [name, studentID] = checkInQueue.front();
            checkInQueue.pop();
            checkedInParticipants.push({name, studentID});
            cout << "Checked in: " << name << " (ID: " << studentID << ")\n";
        } else {
            cout << "No participants in check-in queue.\n";
        }
    }

    // View Next Check-in
    void viewNextCheckIn() {
        if (!checkInQueue.empty()) {
            auto [name, studentID] = checkInQueue.front();
            cout << "Next in line: " << name << " (ID: " << studentID << ")\n";
        } else {
            cout << "No participants in check-in queue.\n";
        }
    }
    void removeFromScheduledEvents(EventNode* eventToRemove) {
    priority_queue<pair<int, EventNode*>, vector<pair<int, EventNode*>>, greater<>> tempQueue;

    while (!scheduledEvents.empty()) {
        auto scheduledEvent = scheduledEvents.top();
        scheduledEvents.pop();

        // Skip the event to be removed
        if (scheduledEvent.second != eventToRemove) {
            tempQueue.push(scheduledEvent);
        }
    }

    scheduledEvents = tempQueue; // Update the scheduled events queue
}

     // Unified Undo Operation
    void undoOperation() {
    if (!undoStack.empty()) {
        Action lastAction = undoStack.top();
        undoStack.pop();
        redoStack.push(lastAction); // Push the action onto the redo stack

        if (lastAction.type == Action::REGISTER_PARTICIPANT) {
            // Undo participant registration
            EventNode* event = eventBST.search(lastAction.eventName);
            if (event) {
                event->participants.remove_if([&](const pair<string, string>& participant) {
                    return participant.first == lastAction.participantName && participant.second == lastAction.participantID;
                });
                cout << "Undid registration of participant: " << lastAction.participantName << "\n";
            }
        } else if (lastAction.type == Action::UPDATE_EVENT) {
            // Undo event update
            // Remove the updated event from the scheduled events queue
            EventNode* updatedEvent = eventBST.search(lastAction.newEventName);
            if (updatedEvent) {
                removeFromScheduledEvents(updatedEvent); // Remove from queue
                eventBST.deleteEvent(lastAction.newEventName); // Delete the updated event
            }

            // Reinsert the old event into the BST and queue
            eventBST.insert(lastAction.oldEventName, lastAction.category);
            EventNode* oldEvent = eventBST.search(lastAction.oldEventName);
            if (oldEvent) {
                scheduledEvents.push({lastAction.priority, oldEvent});
            }

            cout << "Undid update of event: " << lastAction.newEventName << "\n";
        }
    } else {
        cout << "No actions to undo.\n";
    }
}
    // Unified Redo Operation
    void redoOperation() {
    if (!redoStack.empty()) {
        Action lastAction = redoStack.top();
        redoStack.pop();
        undoStack.push(lastAction); // Push the action onto the undo stack

        if (lastAction.type == Action::REGISTER_PARTICIPANT) {
            // Redo participant registration
            registerParticipant(lastAction.eventName, lastAction.participantName);
        } else if (lastAction.type == Action::UPDATE_EVENT) {
            // Redo event update
            // Remove the old event from the scheduled events queue
            EventNode* oldEvent = eventBST.search(lastAction.oldEventName);
            if (oldEvent) {
                removeFromScheduledEvents(oldEvent); // Remove from queue
                eventBST.deleteEvent(lastAction.oldEventName); // Delete the old event
            }

            // Insert the new event into the BST and queue
            eventBST.insert(lastAction.newEventName, lastAction.category);
            EventNode* updatedEvent = eventBST.search(lastAction.newEventName);
            if (updatedEvent) {
                scheduledEvents.push({lastAction.priority, updatedEvent});
            }

            cout << "Redid update of event: " << lastAction.newEventName << "\n";
        }
    } else {
        cout << "No actions to redo.\n";
    }
}

    void generateEventReport() {
        cout << "----- Event Report -----\n";
        eventBST.inorderTraversal();
        cout << "-------------------------\n";

        cout << "----- Check-in Statistics -----\n";
        cout << "Check-In Queue: " << checkInQueue.size() << "\n";
        cout << "Number that has Checked in: " << checkedInParticipants.size() << "\n";
        cout << "-------------------------------\n";
        cout << "Participants Checked In:\n";
        // Display participants who have checked in
        queue<pair<string, string>> tempQueue = checkedInParticipants; // Create a copy to iterate
        while (!tempQueue.empty()) {
            auto participant = tempQueue.front();
            tempQueue.pop();
            cout << "- " << participant.first << " (ID: " << participant.second << ")\n"; // Access name and ID
        }
        cout << "-------------------------------\n";
    };
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
        cout << "11. Generate Event Report\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        // Validate menu choice
        while (!(cin >> choice) || choice < 0 || choice > 11) {
            cout << "Invalid input. Please enter a number between 0 and 11: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: {
                string name, category;
                int priority;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                // Validate event name
                do {
                    cout << "Enter event name (cannot be empty): ";
                    getline(cin, name);
                    if (name.empty()) {
                        cout << "Event name cannot be empty. Please try again.\n";
                    }
                } while (name.empty());

                // Validate category
                do {
                    cout << "Enter event category (cannot be empty): ";
                    getline(cin, category);
                    if (category.empty()) {
                        cout << "Category cannot be empty. Please try again.\n";
                    }
                } while (category.empty());

                // Validate priority
                do {
                    cout << "Enter event priority (1-10): ";
                    while (!(cin >> priority)) {
                        cout << "Invalid input. Enter a number between 1-10: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (priority < 1 || priority > 10);

                ems.createEvent(name, category, priority);
                break;
            }
            case 2: {
                string category;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                do {
                    cout << "Enter category (cannot be empty): ";
                    getline(cin, category);
                    if (category.empty()) {
                        cout << "Category cannot be empty. Please try again.\n";
                    }
                } while (category.empty());
                
                ems.viewEventsByCategory(category);
                break;
            }
            case 3:
                ems.viewScheduledEvents();
                break;
            case 4: {
                string oldName, newName, category;
                int priority;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                // Validate old event name
                do {
                    cout << "Enter old event name (cannot be empty): ";
                    getline(cin, oldName);
                    if (oldName.empty()) {
                        cout << "Event name cannot be empty. Please try again.\n";
                    }
                } while (oldName.empty());

                // Validate new event name
                do {
                    cout << "Enter new event name (cannot be empty): ";
                    getline(cin, newName);
                    if (newName.empty()) {
                        cout << "Event name cannot be empty. Please try again.\n";
                    }
                } while (newName.empty());

                // Validate new category
                do {
                    cout << "Enter new category (cannot be empty): ";
                    getline(cin, category);
                    if (category.empty()) {
                        cout << "Category cannot be empty. Please try again.\n";
                    }
                } while (category.empty());

                // Validate new priority
                do {
                    cout << "Enter new priority (1-10): ";
                    while (!(cin >> priority)) {
                        cout << "Invalid input. Enter a number between 1-10: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (priority < 1 || priority > 10);

                ems.updateEvent(oldName, newName, category, priority);
                break;
            }
            case 5: {
                string name;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                do {
                    cout << "Enter event name to delete (cannot be empty): ";
                    getline(cin, name);
                    if (name.empty()) {
                        cout << "Event name cannot be empty. Please try again.\n";
                    }
                } while (name.empty());

                ems.deleteEvent(name);
                break;
            }
            case 6: {
                string eventName, participantName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                // Validate event name
                do {
                    cout << "Enter event name (cannot be empty): ";
                    getline(cin, eventName);
                    if (eventName.empty()) {
                        cout << "Event name cannot be empty. Please try again.\n";
                    }
                } while (eventName.empty());

                // Validate participant name
                do {
                    cout << "Enter participant name (cannot be empty): ";
                    getline(cin, participantName);
                    if (participantName.empty()) {
                        cout << "Participant name cannot be empty. Please try again.\n";
                    }
                } while (participantName.empty());

                ems.registerParticipant(eventName, participantName);
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
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
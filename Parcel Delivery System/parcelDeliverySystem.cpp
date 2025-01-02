#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>
#include <algorithm>
#include <limits> // for validation

using namespace std;

// Parcel structure
struct Parcel {
    int id;
    string recipient;
    string address;
    int priority; // Lower number means higher priority
};

// Comparator for priority queue
struct ComparePriority {
    bool operator()(Parcel const& p1, Parcel const& p2) {
        return p1.priority > p2.priority; // Higher priority parcels come first
    }
};

// Node for Binary Search Tree
struct BSTNode {
    Parcel parcel;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Parcel p) : parcel(p), left(nullptr), right(nullptr) {}
};

// Class for the Parcel Delivery System
class ParcelDeliverySystem {
private:
    list<Parcel> parcelList;
    priority_queue<Parcel, vector<Parcel>, ComparePriority> priorityQueue;
    queue<Parcel> loadingQueue;
    list<Parcel> deliveredParcels;
    stack<pair<string, Parcel>> actionStack; // Stack for undo actions
    stack<pair<string, Parcel>> redoStack; // Stack for redo actions
    BSTNode* root;
    int totalDelivered;
    int nextParcelId; // To keep track of the next parcel ID

    // function to insert into BST
    BSTNode* insertBST(BSTNode* node, Parcel parcel) {
        if (node == nullptr) {
            return new BSTNode(parcel);
        }
        if (parcel.id < node->parcel.id) {
            node->left = insertBST(node->left, parcel);
        } else {
            node->right = insertBST(node->right, parcel);
        }
        return node;
    }

    // function to search in BST
    BSTNode* searchBST(BSTNode* node, int id) {
        if (node == nullptr || node->parcel.id == id) {
            return node;
        }
        if (id < node->parcel.id) {
            return searchBST(node->left, id);
        }
        return searchBST(node->right, id);
    }

public:
    ParcelDeliverySystem() : root(nullptr), totalDelivered(0), nextParcelId(1) {}

    // Register a parcel
    void registerParcel(string recipient, string address, int priority) {
        Parcel parcel = {nextParcelId++, recipient, address, priority};
        parcelList.push_back(parcel);
        priorityQueue.push(parcel);
        root = insertBST(root, parcel);
        actionStack.push({"register", parcel}); // Record action for undo
        while (!redoStack.empty()) redoStack.pop(); // Clear redo stack on new action
        cout << "Parcel registered: ID " << parcel.id << endl;
    }

    // Load parcel one by one unto delivery truck
    void loadParcels() {
        if (!parcelList.empty()) {
            Parcel parcel = parcelList.front();
            parcelList.pop_front(); // Remove from the list
            loadingQueue.push(parcel); // Add to the loading queue
            actionStack.push({"load", parcel}); // Record the action for undo
            while (!redoStack.empty()) redoStack.pop(); // Clear redo stack on new action
            cout << "Parcel loaded onto the truck: ID " << parcel.id << endl;
        } else {
            cout << "No parcels left to load." << endl;
        }
    }

    // Deliver a parcel based on priority
    void deliverParcel() {
        if (!loadingQueue.empty()) {
            Parcel parcel = loadingQueue.front();
            loadingQueue.pop(); // Remove from the loading queue
            deliveredParcels.push_back(parcel); // Add to delivered parcels list
            totalDelivered++;
            cout << "Delivered Parcel ID: " << parcel.id << " (Priority: " << parcel.priority << ")" << endl;
        } else {
            cout << "No parcels to deliver." << endl;
        }
    }

    // Search for a parcel by ID
    void searchParcelById(int id) {
        BSTNode* result = searchBST(root, id);
        if (result) {
            cout << "Parcel found: ID: " << result->parcel.id << ", Recipient: " << result->parcel.recipient << endl;
        } else {
            cout << "Parcel not found." << endl;
        }
    }

    // Undo last action
    void undoLastAction() {
        if (!actionStack.empty()) {
            auto lastAction = actionStack.top();
            actionStack.pop();
            redoStack.push(lastAction); // Push to redo stack

            if (lastAction.first == "register") {
                // Undo registration
                parcelList.remove_if([&](const Parcel& p) { return p.id == lastAction.second.id; });
                priorityQueue = priority_queue<Parcel, vector<Parcel>, ComparePriority>(); // Rebuild priority queue
                for (const auto& p : parcelList) {
                    priorityQueue.push(p);
                }
                root = nullptr; // Rebuild BST
                for (const auto& p : parcelList) {
                    root = insertBST(root, p);
                }
                cout << "Undid registration for Parcel ID: " << lastAction.second.id << endl;
            } else if (lastAction.first == "load") {
                // Undo loading
                if (!loadingQueue.empty() && loadingQueue.back().id == lastAction.second.id) {
                    loadingQueue.pop(); // Remove from loading queue
                    parcelList.push_front(lastAction.second); // Re-add to parcelList
                    cout << "Undid loading for Parcel ID: " << lastAction.second.id << endl;
                }
            }
        } else {
            cout << "No actions to undo." << endl;
        }
    }

    // Redo last undone action aided by AI
    void redoLastAction() {
        if (!redoStack.empty()) {
            auto lastUndone = redoStack.top();
            redoStack.pop();
            actionStack.push(lastUndone); // Push back to action stack

            if (lastUndone.first == "register") {
                // Redo registration
                parcelList.push_back(lastUndone.second);
                priorityQueue.push(lastUndone.second);
                root = insertBST(root, lastUndone.second);
                cout << "Redid registration for Parcel ID: " << lastUndone.second.id << endl;
            } else if (lastUndone.first == "load") {
                // Redo loading
                loadingQueue.push(lastUndone.second); // Re-add to loading queue
                parcelList.remove_if([&](const Parcel& p) { return p.id == lastUndone.second.id; });
                cout << "Redid loading for Parcel ID: " << lastUndone.second.id << endl;
            }
        } else {
            cout << "No actions to redo." << endl;
        }
    }

    // Generate reports
    void generateReports() {

        cout << "Total parcels delivered: " << totalDelivered << endl;
        // Delivered parcels
        cout << "Delivered parcels:\n";
        for (const auto& parcel : deliveredParcels) {
            cout << "ID: " << parcel.id << ", Recipient: " << parcel.recipient << endl;
        };
        // Parcels pending delivery by priority aided by AI
            cout << "Parcels pending delivery by priority:\n";
            vector<Parcel> pendingParcels;
            queue<Parcel> tempQueue = loadingQueue;
            while (!tempQueue.empty()) {
                pendingParcels.push_back(tempQueue.front());
                tempQueue.pop();
            }
            sort(pendingParcels.begin(), pendingParcels.end(), [](const Parcel& p1, const Parcel& p2) {
                return p1.priority < p2.priority;
            });
            for (const auto& parcel : pendingParcels) {
                cout << "ID: " << parcel.id << ", Priority: " << parcel.priority << endl;
            }

            // Delivery routes used
           cout << "Delivery routes used (order of delivery as entered):\n";
            if (!deliveredParcels.empty()) {
                auto it = deliveredParcels.begin();
                cout << "ID: " << it->id;
                ++it;
                for (; it != deliveredParcels.end(); ++it) {
                    cout << " - ID: " << it->id;
                }
                cout << endl;
            } else {
                cout << "No delivery routes used." << endl;
            }    
    }
};

int main() {
    ParcelDeliverySystem system;
    int choice, priority, id;
    string recipient, address;

    do {
        cout << "\nParcel Delivery System Menu:\n";
        cout << "1. Register Parcel\n";
        cout << "2. Load Parcels\n";
        cout << "3. Deliver Parcel\n";
        cout << "4. Search for Parcel by ID\n";
        cout << "5. Generate Reports\n";
        cout << "6. Undo Last Action\n";
        cout << "7. Redo Last Action\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";

        // Validate menu choice
        while (!(cin >> choice) || choice < 1 || choice > 8) {
            cout << "Invalid input. Please enter a number between 1 and 8: ";
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }

        cin.ignore(); 

        switch (choice) {
            case 1:
                cout << "Enter Recipient Name: ";
                getline(cin, recipient);
                while (recipient.empty()) {
                    cout << "Recipient name cannot be empty. Please enter a valid name: ";
                    getline(cin, recipient);
                }

                cout << "Enter Address: ";
                getline(cin, address);
                while (address.empty()) {
                    cout << "Address cannot be empty. Please enter a valid address: ";
                    getline(cin, address);
                }

                cout << "Enter Priority (lower number means higher priority): ";
                while (!(cin >> priority) || priority < 1) {
                    cout << "Invalid input. Please enter a valid priority (positive number): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                system.registerParcel(recipient, address, priority);
                break;

            case 2:
                system.loadParcels();
                break;

            case 3:
                system.deliverParcel();
                break;

            case 4:
                cout << "Enter Parcel ID to search: ";
                while (!(cin >> id) || id < 1) {
                    cout << "Invalid input. Please enter a valid Parcel ID (positive number): ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                system.searchParcelById(id);
                break;

            case 5:
                system.generateReports();
                break;

            case 6:
                system.undoLastAction();
                break;

            case 7:
                system.redoLastAction();
                break;

            case 8:
                cout << "Exiting the system." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}

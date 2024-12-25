#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>

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
    priority_queue<Parcel, vector<Parcel>, ComparePriority> priorityQueue; // Priority queue
    queue<Parcel> loadingQueue; // Queue for loading parcels
    list<Parcel> deliveredParcels; // Linked list for delivered parcels
    stack<pair<string, Parcel>> actionStack; // Stack for undo actions
    stack<pair<string, Parcel>> redoStack; // Stack for redo actions
    BSTNode* root; // Root of the BST for searching parcels
    int totalDelivered;
    int nextParcelId; // To keep track of the next parcel ID

    // Helper function to insert into BST
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

    // Helper function to search in BST
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

    // Load parcels onto delivery truck
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

    // Deliver a parcel
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

    // Redo last undone action
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

    // Search for a parcel by ID
    void searchParcelById(int id) {
        BSTNode* result = searchBST(root, id);
        if (result) {
            cout << "Parcel found: ID: " << result->parcel.id << ", Recipient: " << result->parcel.recipient << endl;
        } else {
            cout << "Parcel not found." << endl;
        }
    }

    // Generate reports
    void generateReports() {
        cout << "Total parcels delivered: " << totalDelivered << endl;
        cout << "Parcels pending delivery: " << loadingQueue.size() << endl;
        cout << "Delivered parcels:\n";
        for (const auto& parcel : deliveredParcels) {
            cout << "ID: " << parcel.id << ", Recipient: " << parcel.recipient << endl;
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
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Recipient Name: ";
                cin.ignore();
                getline(cin, recipient);
                cout << "Enter Address: ";
                getline(cin, address);
                cout << "Enter Priority (lower number means higher priority): ";
                cin >> priority;
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
                cin >> id;
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

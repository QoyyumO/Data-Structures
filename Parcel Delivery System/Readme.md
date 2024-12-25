# Parcel Delivery System

## Overview
The Parcel Delivery System is a C++ program designed to manage parcel deliveries efficiently. It includes features for registering parcels, managing loading and delivery processes, searching parcels by ID, generating reports, and undoing or redoing actions. It uses a combination of data structures like queues, stacks, priority queues, linked lists, and binary search trees to ensure optimal functionality.

## Features
1. **Register Parcel**: Add new parcels with recipient details, address, and priority.
2. **Load Parcels**: Load parcels onto the delivery truck.
3. **Deliver Parcels**: Deliver loaded parcels and record them as delivered.
4. **Search Parcel by ID**: Search for parcels by their unique ID using a Binary Search Tree (BST).
5. **Generate Reports**: View the total number of delivered parcels, pending deliveries, and details of delivered parcels.
6. **Undo Last Action**: Undo the last action (parcel loading or registration cancellation).
7. **Redo Last Action**: Redo the last undone action.

## Data Structures Used
- **Priority Queue**: To prioritize parcels based on their urgency.
- **Queue**: To manage parcels being loaded for delivery.
- **Stack**: For implementing undo and redo functionalities.
- **Linked List**: To store delivered parcels.
- **Binary Search Tree (BST)**: For efficient parcel ID search.

## How to Use
1. Compile the program using any C++ compiler.
   ```bash
   g++ -o ParcelDeliverySystem ParcelDeliverySystem.cpp
   ```
2. Run the compiled program.
   ```bash
   ./ParcelDeliverySystem
   ```
3. Follow the menu options:
   - **Register Parcel**: Input recipient details, address, and priority.
   - **Load Parcels**: Move parcels from the list to the delivery queue.
   - **Deliver Parcels**: Deliver parcels in the queue.
   - **Search Parcel by ID**: Search for parcels using their unique ID.
   - **Generate Reports**: View delivery and pending parcel statistics.
   - **Undo Last Action**: Undo the last loading or registration cancellation.
   - **Redo Last Action**: Redo the last undone action.

## Code Walkthrough
### Key Classes and Structures
- **`Parcel`**: Represents a parcel with attributes like ID, recipient name, address, and priority.
- **`BSTNode`**: Represents a node in the binary search tree, storing a parcel.
- **`ParcelDeliverySystem`**: Main class managing all operations.

### Main Functions
- `registerParcel`: Registers a new parcel and adds it to the necessary data structures.
- `loadParcels`: Loads parcels onto the delivery truck and records the action for undo.
- `deliverParcel`: Delivers parcels and moves them to the delivered list.
- `undoLastAction`: Undoes the last loading or registration cancellation.
- `redoLastAction`: Redoes the last undone action.
- `searchParcelById`: Searches for a parcel in the BST by its ID.
- `generateReports`: Displays delivery statistics.

### Undo and Redo Functionalities
- **Undo**: Cancels the last action (loading or registration cancellation) by retrieving it from the undo stack.
- **Redo**: Reapplies the last undone action by retrieving it from the redo stack.

## Example Workflow
1. Register a parcel:
   - Input recipient: `John Doe`
   - Input address: `123 Main Street`
   - Input priority: `1`

2. Load the parcel onto the truck.

3. Deliver the parcel.

4. Undo the last action (e.g., cancel loading).

5. Redo the last undone action.

6. Search for the parcel by its ID.

7. Generate a delivery report.

## Menu Options
```
1. Register Parcel
2. Load Parcels
3. Deliver Parcel
4. Search for Parcel by ID
5. Generate Reports
6. Undo Last Action
7. Redo Last Action
8. Exit
```

## Sample Output
```
Parcel Delivery System Menu:
1. Register Parcel
2. Load Parcels
3. Deliver Parcel
4. Search for Parcel by ID
5. Generate Reports
6. Undo Last Action
7. Redo Last Action
8. Exit
Enter your choice: 1
Enter Recipient Name: John Doe
Enter Address: 123 Main Street
Enter Priority (lower number means higher priority): 1
Parcel registered: ID 1
```

## Requirements
- C++ Compiler supporting C++11 or later

## Future Enhancements
- Add parcel tracking by location.
- Improve undo/redo functionalities to handle complex actions.
- Support bulk parcel registration.

## License
This project is licensed under the MIT License.

---

Developed as a demonstration of efficient data structure usage in C++ for a parcel delivery system.


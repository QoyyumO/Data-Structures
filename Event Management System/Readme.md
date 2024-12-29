# Event Management System

## Overview
The Event Management System is a C++ application that provides comprehensive functionality for managing events, participants, and check-ins. The system uses a Binary Search Tree (BST) for efficient event organization and includes features like priority scheduling, participant registration, and undo/redo capabilities.

## Requirements
### System Requirements
- C++ compiler with C++11 support or higher


### Dependencies
- Standard Template Library (STL)
- iostream
- string
- list
- queue
- stack
- map
- algorithm
- limits

### Build Requirements
- CMake 3.10 or higher (recommended)
- Make or equivalent build system
- Git (for version control and updates)

## Features
- Event Management
  - Create, update, and delete events
  - Organize events by category
  - Priority-based event scheduling
  - View events by category or scheduled priority

- Participant Management
  - Register participants for events
  - Automatic participant ID generation
  - Check-in queue system
  - Track checked-in participants

- System Operations
  - Undo/Redo functionality for event updates and participant registration
  - Generate comprehensive event reports
  - Input validation for all operations

## Data Structures Used
- Binary Search Tree (BST) for event organization and searching
- Priority Queue for scheduled events
- Stack for undo/redo operations
- Queue for check-in management
- List for participant storage

## Class Structure

### EventNode
- Represents a single event in the system
- Contains event details and participant information
- Supports BST operations

### EventBST
- Implements the Binary Search Tree for events
- Provides methods for:
  - Insertion
  - Deletion
  - Searching
  - Category-based display
  - Inorder traversal

### EventManagementSystem
- Main system class that integrates all functionality
- Manages:
  - Event operations
  - Participant registration
  - Check-in processing
  - Undo/redo operations
  - Report generation

## Menu Options
1. Create Event
2. View Events by Category
3. View Scheduled Events
4. Update Event
5. Delete Event
6. Register Participant
7. Process Check-in
8. View Next Check-in
9. Undo Last Operation
10. Redo Last Operation
11. Generate Event Report
0. Exit

## Input Validation
- All user inputs are validated for:
  - Empty strings
  - Invalid numerical values
  - Priority range (1-10)
  - Menu choice validation

## Usage Example
```cpp
EventManagementSystem ems;
// Create an event
ems.createEvent("Annual Conference", "Conference", 1);
// Register a participant
ems.registerParticipant("Annual Conference", "John Doe");
// Process check-in
ems.processCheckIn();
// Generate report
ems.generateEventReport();
```

## Implementation Details
- The system uses modern C++ features
- Incorporates STL containers for efficient data management
- Implements comprehensive error handling
- Supports case-insensitive category matching
- Automatic ID generation for participants

## Error Handling
- Input validation for all user inputs
- Proper error messages for invalid operations
- Graceful handling of empty data structures
- Protection against invalid menu selections

## Performance Considerations
- BST provides O(log n) search complexity
- Priority queue ensures efficient event scheduling
- Stack-based undo/redo operations for constant time access
- Efficient participant check-in queue management

## Future Enhancements
1. User Authentication and Authorization
2. Database Integration
3. Covert to a Web app
4. Reporting and Analytics

## License
MIT License

Copyright (c) 2024 Event Management System

---

Developed as a demonstration of efficient data structure usage in C++ for a parcel delivery system.

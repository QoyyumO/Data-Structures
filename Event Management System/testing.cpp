// Method 1: Manual Unit Testing Function
#include <iostream>
#include <cassert>
#include <vector>

class UnitTester {
private:
    int totalTests = 0;
    int passedTests = 0;

public:
    // Test Event Creation
    void testEventCreation() {
        totalTests++;
        try {
            Event event("Tech Seminar", "Academic", "Annual Tech Conference", 8);
            
            assert(event.name == "Tech Seminar");
            assert(event.category == "Academic");
            assert(event.description == "Annual Tech Conference");
            assert(event.priority == 8);
            assert(event.participants.empty());
            
            passedTests++;
            std::cout << "✅ Event Creation Test Passed\n";
        } catch (const std::exception& e) {
            std::cout << "❌ Event Creation Test Failed: " << e.what() << "\n";
        }
    }

    // Test Participant Registration
    void testParticipantRegistration() {
        totalTests++;
        try {
            Event event("Workshop", "Training", "Programming Workshop", 7);
            Participant participant("John Doe", "12345", "john@example.com");
            
            event.addParticipant(participant);
            
            assert(event.participants.size() == 1);
            assert(event.participants[0].name == "John Doe");
            assert(event.participants[0].studentID == "12345");
            
            passedTests++;
            std::cout << "✅ Participant Registration Test Passed\n";
        } catch (const std::exception& e) {
            std::cout << "❌ Participant Registration Test Failed: " << e.what() << "\n";
        }
    }

    // Test Participant Removal
    void testParticipantRemoval() {
        totalTests++;
        try {
            Event event("Conference", "Academic", "Annual Conference", 9);
            
            // Add multiple participants
            event.addParticipant(Participant("Alice", "1001", "alice@example.com"));
            event.addParticipant(Participant("Bob", "1002", "bob@example.com"));
            event.addParticipant(Participant("Charlie", "1003", "charlie@example.com"));
            
            assert(event.participants.size() == 3);
            
            // Remove a specific participant
            event.removeParticipant("1002");
            
            assert(event.participants.size() == 2);
            
            // Verify remaining participants
            bool bobRemoved = true;
            for (const auto& participant : event.participants) {
                if (participant.studentID == "1002") {
                    bobRemoved = false;
                    break;
                }
            }
            assert(bobRemoved);
            
            passedTests++;
            std::cout << "✅ Participant Removal Test Passed\n";
        } catch (const std::exception& e) {
            std::cout << "❌ Participant Removal Test Failed: " << e.what() << "\n";
        }
    }

    // Test Event Priority Comparison
    void testEventPriorityComparison() {
        totalTests++;
        try {
            Event highPriorityEvent("High Priority", "Urgent", "Critical Event", 9);
            Event lowPriorityEvent("Low Priority", "Regular", "Routine Event", 2);
            
            assert(highPriorityEvent < lowPriorityEvent == false);
            assert(lowPriorityEvent < highPriorityEvent == true);
            
            passedTests++;
            std::cout << "✅ Event Priority Comparison Test Passed\n";
        } catch (const std::exception& e) {
            std::cout << "❌ Event Priority Comparison Test Failed: " << e.what() << "\n";
        }
    }

    // Comprehensive Test Suite
    void runAllTests() {
        std::cout << "=== Running Comprehensive Test Suite ===\n";
        
        testEventCreation();
        testParticipantRegistration();
        testParticipantRemoval();
        testEventPriorityComparison();
        
        std::cout << "\nTest Summary:\n";
        std::cout << "Total Tests: " << totalTests << "\n";
        std::cout << "Passed Tests: " << passedTests << "\n";
        std::cout << "Failed Tests: " << (totalTests - passedTests) << "\n";
        
        // Optional: Set exit code based on test results
        if (passedTests != totalTests) {
            std::cerr << "❌ Some tests failed!\n";
            exit(1);
        }
    }
};

// Standalone Test Runner
int main() {
    UnitTester tester;
    tester.runAllTests();
    
    return 0;
}
#include <iostream>
#include <string>
#include "Autholas.h"
#include "android_device_id.h" // Your device ID function
#include "obfuscate.h"

int main() {
    std::cout << "========================" << std::endl;
    std::cout << "Autholas Login System" << std::endl;
    std::cout << "Android C++ Example Code" << std::endl;
    std::cout << "========================" << std::endl;
    
    // Initialize authentication
    Autholas auth;
    auth.hwid = get_device_id(); // Get device hardware ID
    auth.device_name = "User Android";
    
    std::string username, password;
    
    // Get user credentials
    std::cout << "Username: ";
    std::cin >> username;
    
    std::cout << "Password: ";
    std::cin >> password;
    
    printf("\nAttempting login...\n");
    
    // Attempt login
    if (auth.login(username, password)) {
        printf("\n\033[32mAuthentication successful!\033[0m\n");
        
        // Show session info if available
        if (auth.isSessionValid()) {
            printf("Session token: %s...\n", auth.getSessionToken().substr(0, 20).c_str());
            printf("Session expires: %s\n", auth.session_expires.c_str());
        }
        
        // Your main application code goes here
        printf("\n\033[32m=== APPLICATION STARTED ===\033[0m\n");
        printf("You can now run your main application logic here.\n");
        printf("User '%s' is authenticated and ready to use the application.\n", username.c_str());
        
        // Example: Keep application running
        printf("\nPress Enter to exit...\n");
        std::cin.ignore();
        std::cin.get();
        
        // Logout when done
        auth.logout();
        printf("Logged out successfully.\n");
        
    } else {
        printf("\n\033[31mAuthentication failed!\033[0m\n");
        printf("Please check your username and password.\n");
        return 1;
    }
    
    return 0;
}
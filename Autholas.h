#pragma once
#include <curl/curl.h>
#include <json/json.h> // You'll need jsoncpp library
#include "obfuscate.h"
// See Example.cpp for example

#define strg std::string
#define OBF(string) AY_OBFUSCATE(string)

class Autholas {
private:
  const char* ApiKey = OBF("YOUR_API_KEY_HERE"); // Your API Key
  const char* Auth_Link = OBF("https://autholas.nicholasdevs.xyz/api/auth"); // Your auth endpoint
  
  struct AuthResponse {
    bool success;
    std::string message;
    std::string error;
    std::string error_code;
    std::string session_token;
    std::string expires_at;
  };

  AuthResponse ProcessRequest(std::string resp) {
    AuthResponse result = {false, "", "", "", "", ""};
    
    if (resp.empty()) {
      result.error = "Empty response from server";
      result.error_code = "EMPTY_RESPONSE";
      return result;
    }

    try {
      Json::Value jsonResponse;
      Json::Reader reader;
      
      if (!reader.parse(resp, jsonResponse)) {
        result.error = "Invalid JSON response";
        result.error_code = "INVALID_JSON";
        return result;
      }

      result.success = jsonResponse.get("success", false).asBool();
      result.message = jsonResponse.get("message", "").asString();
      result.error = jsonResponse.get("error", "").asString();
      result.error_code = jsonResponse.get("error_code", "").asString();
      
      if (result.success) {
        result.session_token = jsonResponse.get("session_token", "").asString();
        result.expires_at = jsonResponse.get("expires_at", "").asString();
        printf("Authentication successful!\n");
        if (!result.session_token.empty()) {
          printf("Session token received\n");
        }
      } else {
        // Handle different error cases
        if (result.error_code == "INVALID_API_KEY") {
          printf("Invalid API key. Please check your developer credentials.\n");
        } else if (result.error_code == "INVALID_CREDENTIALS") {
          printf("Invalid username or password.\n");
        } else if (result.error_code == "USER_BANNED") {
          printf("Your account has been banned.\n");
        } else if (result.error_code == "SUBSCRIPTION_EXPIRED") {
          printf("Your subscription has expired.\n");
        } else if (result.error_code == "HWID_BANNED") {
          printf("Your device has been banned.\n");
        } else if (result.error_code == "MAX_DEVICES_REACHED") {
          printf("Maximum devices limit reached.\n");
        } else if (result.error_code == "HWID_ALREADY_USED") {
          printf("This device is already registered to another user.\n");
        } else if (result.error_code == "RATE_LIMIT_EXCEEDED") {
          printf("API rate limit exceeded. Try again later.\n");
        } else if (result.error_code == "DEVELOPER_SUSPENDED") {
          printf("Developer account has been suspended.\n");
        } else {
          printf("Authentication failed: %s\n", result.error.c_str());
        }
      }
      
    } catch (const std::exception& e) {
      result.error = "JSON parsing error";
      result.error_code = "JSON_PARSE_ERROR";
      printf("Error parsing response: %s\n", e.what());
    }
    
    return result;
  }

public:
  // Public Variables
  std::string hwid = ""; // Hardware ID (replaces device_id)
  std::string device_name = ""; // Optional device name
  std::string session_token = ""; // Store session token
  std::string session_expires = ""; // Session expiration
  
  static size_t WCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
  }

  std::string RequestPost(const char* url, const char* request_headers, const char* post_fields, bool verify_host = false, bool verify_peer = false) {
    CURL *curl;
    std::string readBuffer;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    CURLcode res;
    
    if (curl) {
      headers = curl_slist_append(headers, request_headers);
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000); // Increased timeout
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_host);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer);
      curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
      
      res = curl_easy_perform(curl);
      
      if (res != CURLE_OK) {
        printf("Failed to connect to server: %s\n", curl_easy_strerror(res));
        readBuffer = "";
      }
      
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      return readBuffer;
    }
    
    return "";
  }

  // Create JSON payload for authentication
  std::string CreateAuthPayload(const std::string& username, const std::string& password) {
    Json::Value payload;
    payload["api_key"] = this->ApiKey;
    payload["username"] = username;
    payload["password"] = password;
    payload["hwid"] = this->hwid;
    
    if (!this->device_name.empty()) {
      payload["device_name"] = this->device_name;
    }
    
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    return Json::writeString(builder, payload);
  }

  // Main login function (username and password required for your API)
  bool login(std::string username, std::string password) {
    if (hwid.empty() || hwid == "") {
      printf("HWID is not set, exiting...\n");
      exit(1);
    }
    
    std::string jsonPayload = CreateAuthPayload(username, password);
    std::string resp = RequestPost(Auth_Link, "Content-Type: application/json", jsonPayload.c_str(), true, true);
    
    AuthResponse result = this->ProcessRequest(resp);
    
    if (result.success) {
      this->session_token = result.session_token;
      this->session_expires = result.expires_at;
    }
    
    return result.success;
  }

  // Overloaded version for backward compatibility (username only)
  // Note: Your server requires password, so this will always fail
  bool login(std::string username) {
    printf("Warning: Username-only login is not supported by this server.\n");
    printf("Password is required for authentication.\n");
    return false;
  }

  // Check if session is still valid
  bool isSessionValid() {
    return !session_token.empty() && !session_expires.empty();
  }

  // Get current session token
  std::string getSessionToken() {
    return session_token;
  }

  // Clear session data
  void logout() {
    session_token = "";
    session_expires = "";
  }
};
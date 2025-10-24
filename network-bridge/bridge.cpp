#include <iostream>
#include <string.h>
#include <cstdlib>
#include <climits>
#include <ctime>
using namespace std;

#define AGEING_TIME 300

struct Node {
    char MAC[19];
    int port;
    time_t timestamp;
    struct Node* next;
};

void initialize(Node* current, char MAC[], int port) {
    strcpy(current->MAC, MAC);
    current->port = port;
    current->timestamp = time(NULL);
}

Node* remove(Node *current) {
    if (current == NULL || current->next == NULL) return current;
	Node *temp = current->next->next;
	delete current->next;
	current->next = temp;
	return current->next;
}

void printList(Node* first) {
    Node* current = first;
    time_t now = time(NULL);
    cout << "[MAC TABLE STATE] | Timestamp: " << ctime(&(now));
    cout << "-------------------------------------------------------------------------" << endl;
    while (current != NULL) {
        cout << "MAC: " << current->MAC << " | ";
        cout << "Port: " << current->port << " | ";
        cout << "Timestamp: " << ctime(&(current->timestamp));
        current = current->next;
    }
    cout << "-------------------------------------------------------------------------" << endl << endl;
}

Node* cleanList(Node* first) {
    if (first == nullptr) return nullptr;
    Node* current = first;

    if(first != NULL && (time(NULL) - first->timestamp) > AGEING_TIME) {
        cout << "[MAINTENANCE] Cleaning MAC table: removing old entities." << endl;
        cout << "[MAINTENANCE] Cleaning MAC table: removed 1 old entity::MAC: " << first->MAC << endl;
        Node* temp = first;
        first = first->next;
        delete temp;
        current = first;
    }
    while (current != NULL && current->next != NULL) {
        if ((time(NULL) - current->next->timestamp) > AGEING_TIME) {
            cout << "[MAINTENANCE] Cleaning MAC table: removing old entities." << endl;
            cout << "[MAINTENANCE] Cleaning MAC table: removed 1 old entity::MAC: " << current->next->MAC << endl;
            current = remove(current);
        } else {
            current = current->next;
        }
    }

    if (first == nullptr) {
        cout << "[MAINTENANCE] MAC table is empty after cleaning." << endl;
    }

    time_t now = time(NULL);

    cout << "[MAINTENANCE] Cleaning MAC table: removing stale entries older than 300 seconds." << endl;
    cout << "[MAINTENANCE RESULT] MAC table updated | Timestamp: " << ctime(&now) << endl;
    return first;
}

int searchPort(Node* first, char MAC_receiver[], int values[]) {
    if (first-> next == NULL) return -2;
    Node* current = first;
    int i = 0;
    int port = 0;
    while (current != NULL) {
        if (strcmp(current->MAC, MAC_receiver) == 0) {
            port = current->port;
            return port;
        } else {
            values[i] = current->port;
            i++;
        }
    current = current->next;
    }
    return -1;
}

void filtering(char MAC_sender[], int port, char MAC_receiver[], Node* first, int values[]) {
    if(strcmp(MAC_sender, MAC_receiver) == 0) {
        cout << "[FILTERING DECISION] Frame ignored — source and destination MAC addresses identical." << endl;
    } else {
        for (int i = 0; i < 100; i++) {
            values[i] = 0;
        }
        int p = searchPort(first, MAC_receiver, values);
        if (p >= 0) {
            cout << "[FILTERING DECISION] Forwarding frame to port " << p << endl;
        } else if (p == -1) {
            cout << "[FILTERING DECISION] Destination MAC not found. Broadcasting frame to all ports: "; 
            for(int i=0; i<100; i++) {
                if (values[i] == 0) break;
                if (values[i] != port) {
                    cout << values[i] << " ";
                }
            }
            cout << endl;
        } else {
            cout << "[FILTERING DECISION] No port to broadcast to" << endl;
        }
    }
}

Node* learning(char MAC_sender[], int port, Node* first, bool &IsEmpty) {
    if (IsEmpty) {
        first = new Node();
        initialize(first, MAC_sender, port);
        first->next = nullptr;
        IsEmpty = false;
        return first;
    }
    
    Node* current = first;

    while (current != NULL) {
        if (strcmp(current->MAC, MAC_sender) == 0) {
            if (current->port != port) {
                current->port = port;
            }
            current->timestamp = time(NULL);
            return first;
        }
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }

    current->next = new Node();
    current = current->next;
    initialize(current, MAC_sender, port);
    
    return first;
}

int main() {
    time_t now = time(NULL);
    cout << "[BRIDGE MODULE] Initializing Store-and-Forward Algorithm..." << endl << "[BRIDGE MODULE] Learning Table Initialized | Timestamp: " << ctime(&now) << endl;
    Node* first = nullptr;

    bool IsEmpty = true;
    Node* current = first;

    char tokens[40];
    char MAC_sender[18];
    int port;
    char MAC_receiver[18];
    int values[100] = {};

    do {
        cin.getline(tokens, 43);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        cout << endl << endl;

        if (strcmp(tokens, "stop") == 0) {
            break;
        }

        if (tokens[17] != ',' || tokens[22] != ',' || strlen(tokens) != 40) continue;
        char* token = strtok(tokens, ",");
        if (token == NULL) continue;
        strcpy(MAC_sender, token);
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        port = atoi(token);
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(MAC_receiver, token);
        bool error = false;
        for (int i = 0; i < 17; i++) {
            if (i % 3 == 2) {
                if (MAC_sender[i] != ':' || MAC_receiver[i] != ':') error = true; continue;
            } else {
                if (!(MAC_sender[i] >= '0' && MAC_sender[i] <= 'F') || !(MAC_receiver[i] >= '0' && MAC_receiver[i] <= 'F')) error = true; continue;
            }
        }
        if (error) continue;

        first = learning(MAC_sender, port, first, IsEmpty);
        filtering(MAC_sender, port, MAC_receiver, first, values);

        first = cleanList(first);
        printList(first);
    } while (strcmp(tokens, "stop") != 0);

    return 0;
}

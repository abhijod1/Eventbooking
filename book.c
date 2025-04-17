#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

// -------------------- Linked List for Users -----------------------
typedef struct User {
    int user_id;
    char name[NAME_LEN];
    struct User* next;
} User;

User* add_user(User* head, int user_id, const char* name) {
    User* new_user = (User*)malloc(sizeof(User));
    new_user->user_id = user_id;
    strcpy(new_user->name, name);
    new_user->next = head;
    printf("User %s added.\n", name);
    return new_user;
}

void display_users(User* head) {
    while (head) {
        printf("User ID: %d, Name: %s\n", head->user_id, head->name);
        head = head->next;
    }
}

// -------------------- Queue (Waiting List) ------------------------
typedef struct QueueNode {
    User* user;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, User* user) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->user = user;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    printf("%s added to waiting queue.\n", user->name);
}

User* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    QueueNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    User* user = temp->user;
    free(temp);
    return user;
}

void display_queue(Queue* q) {
    QueueNode* temp = q->front;
    printf("Waiting Queue:\n");
    while (temp) {
        printf("- %s\n", temp->user->name);
        temp = temp->next;
    }
}

// -------------------- Tree for Events -----------------------------
typedef struct Event {
    int event_id;
    char name[NAME_LEN];
    int total_seats;
    int booked_count;
    User* booked_users[100];
    Queue* waiting_queue;
    struct Event* child[10];
    int child_count;
} Event;

Event* create_event(int id, const char* name, int seats) {
    Event* e = (Event*)malloc(sizeof(Event));
    e->event_id = id;
    strcpy(e->name, name);
    e->total_seats = seats;
    e->booked_count = 0;
    e->waiting_queue = create_queue();
    e->child_count = 0;
    return e;
}

void add_sub_event(Event* parent, Event* child) {
    if (parent->child_count < 10) {
        parent->child[parent->child_count++] = child;
    }
}

void book_ticket(Event* event, User* user) {
    if (event->booked_count < event->total_seats) {
        event->booked_users[event->booked_count++] = user;
        printf("Ticket booked for %s in %s\n", user->name, event->name);
    } else {
        enqueue(event->waiting_queue, user);
    }
}

void show_event(Event* event, int level) {
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s [Seats Left: %d]\n", event->name, event->total_seats - event->booked_count);
    for (int i = 0; i < event->child_count; i++) {
        show_event(event->child[i], level + 1);
    }
}

void show_bookings(Event* event) {
    printf("Event: %s\n", event->name);
    printf("Booked Users:\n");
    for (int i = 0; i < event->booked_count; i++) {
        printf("- %s\n", event->booked_users[i]->name);
    }
    display_queue(event->waiting_queue);
}

// -------------------- Main Program -------------------------------
int main() {
    User* user_list = NULL;

    // Add users
    user_list = add_user(user_list, 1, "Abhi");
    user_list = add_user(user_list, 2, "Sita");
    user_list = add_user(user_list, 3, "Hari");

    // Create events
    Event* root_event = create_event(101, "Music Festival", 2);
    Event* rock_stage = create_event(102, "Rock Stage", 1);
    Event* jazz_stage = create_event(103, "Jazz Stage", 1);

    add_sub_event(root_event, rock_stage);
    add_sub_event(root_event, jazz_stage);

    // Booking
    User* u1 = user_list;
    User* u2 = user_list->next;
    User* u3 = user_list->next->next;

    printf("\nBooking Tickets...\n");
    book_ticket(root_event, u1); // Abhi
    book_ticket(root_event, u2); // Sita
    book_ticket(root_event, u3); // Hari (goes to waiting)

    // Display
    printf("\nEvent Hierarchy:\n");
    show_event(root_event, 0);

    printf("\nBookings & Waiting List:\n");
    show_bookings(root_event);

    printf("\nAll Registered Users:\n");
    display_users(user_list);

    return 0;
}

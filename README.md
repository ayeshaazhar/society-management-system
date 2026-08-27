# 🏢 Society Management System — C++ DSA Project

A console-based Society Management System developed in C++ to manage societies, members, events, membership requests, and finances.

The project was built to apply fundamental **Data Structures, Algorithms, Object-Oriented Programming, pointers, dynamic memory management, and file handling** concepts to a practical management system.

---

## 📌 Overview

The Society Management System provides a centralized way to manage different aspects of a university society environment.

The system supports multiple user roles, including:

- **Admin**
- **Society President**
- **Member**

Each role has different permissions and functionality.

The project uses custom implementations of several fundamental data structures instead of relying on ready-made container classes for the core functionality.

---

## ✨ Features

### 🔐 Authentication

- Role-based login system
- Admin, President, and Member accounts
- Password hashing
- 3-attempt login system
- Account lockout after failed attempts
- User registration
- Society association for Presidents and Members

### 🏢 Society Management

- View all societies
- Add new societies
- Delete societies
- Store society information
- Track society budgets
- Track society presidents
- Search for societies

### 👥 Member Management

- View society members
- Add approved members
- Store member information
- Track member society
- Store joining dates
- Process membership requests

### 📝 Membership Requests

- Members can request to join a society
- Requests are stored in a Queue
- Requests are processed using FIFO order
- Admins/Presidents can approve or reject requests

### 📅 Event Management

- Schedule upcoming events
- Assign event priority
- Store event date and venue
- Mark events as completed
- Move completed events to past-event history
- View upcoming events
- View past events

### 💰 Financial Management

- Record income and expenses
- Store transaction descriptions
- Track transaction dates
- Calculate total income
- Calculate total expenses
- Calculate net balance
- View financial records by society

### 📊 Society Rankings

- Rank societies according to their budget
- Implemented using Quick Sort
- Displays ranked societies with their budgets

### 💾 Data Persistence

System data is stored in files so that information remains available after the program is closed.

The system saves and loads:

- Users
- Societies
- Members
- Membership requests
- Upcoming events
- Past events
- Financial transactions

### 🎨 Console Interface

- ANSI color-coded output
- Formatted tables
- Structured menus
- Success, error, and information messages
- Separate dashboards for each user role

---

# 🧠 Data Structures & Algorithms

One of the main goals of this project was to implement and apply fundamental data structures and algorithms in a practical application.

| Data Structure / Algorithm | Application |
|---|---|
| **Hash Table** | User authentication and credential lookup |
| **Linked List** | Societies |
| **Linked List** | Members |
| **Linked List** | Financial transactions |
| **Queue** | Membership requests |
| **Stack** | Past event history |
| **Priority Queue** | Upcoming event scheduling |
| **Quick Sort** | Ranking societies by budget |
| **Searching** | Finding users and societies |
| **File Handling** | Persistent data storage |

---

## 🔎 Data Structure Implementation

### 1. Hash Table

A custom hash table is used for storing and retrieving user accounts.

It supports:

- User insertion
- User lookup
- Authentication
- Duplicate username checking
- Password hashing

Hash collisions are handled using **separate chaining with linked lists**.

**Used for:**

```text
Username → Password → Role → Society

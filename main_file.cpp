#include <iostream>
#include <string>
#include <vector>
#include <fstream> 

struct DateTime {
    int day;
    int month;
    int year;

    DateTime(int d, int m, int y) : day(d), month(m), year(y) {}
};

struct Task {
    std::string name;
    std::string description;
    int priority;
    DateTime deadline;
    std::string status;

    Task(std::string n, std::string desc, int prio, DateTime ddl, std::string stat)
        : name(n), description(desc), priority(prio), deadline(ddl), status(stat) {}
};

struct TaskByDay {
    std::vector<Task> tasks;
};

void addTask(std::vector<TaskByDay> &tasksByDay) {
    int day;
    std::cout << "Enter the day of the deadline (1-31): ";
    std::cin >> day;

    if (day < 1 || day > 31) {
        std::cout << "Invalid day. Please enter a valid day (1-31).\n";
        return;
    }

    std::cin.ignore();
    std::cout << "Task Name: ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::string description;
    std::getline(std::cin, description);
    std::cout << "Priority (1-5): ";
    int priority;
    std::cin >> priority;
    std::cin.ignore();
    std::cout << "Status (completed/in progress/not started): ";
    std::string status;
    std::getline(std::cin, status);

    if (priority < 1 || priority > 5) {
        std::cout << "Invalid input. Priority should be between 1-5.\n";
    } else {
        DateTime deadline(day, 0, 0);
        tasksByDay[day - 1].tasks.push_back(Task(name, description, priority, deadline, status));
        std::cout << "Task added for day " << day << ".\n";
    }
}

void editTask(std::vector<TaskByDay> &tasksByDay) {
    int day;
    std::cout << "Enter the day of the task you want to edit (1-31): ";
    std::cin >> day;

    if (day < 1 || day > 31) {
        std::cout << "Invalid day. Please enter a valid day (1-31).\n";
        return;
    }

    if (tasksByDay[day - 1].tasks.empty()) {
        std::cout << "No tasks for day " << day << " to edit.\n";
        return;
    }

    std::cout << "Tasks for day " << day << ":\n";
    for (size_t i = 0; i < tasksByDay[day - 1].tasks.size(); ++i) {
        const Task &task = tasksByDay[day - 1].tasks[i];
        std::cout << i + 1 << ". " << task.name << " (Priority: " << task.priority << " Status: " << task.status << ")\n";
    }

    int taskIndex;
    std::cout << "Enter the index of the task you want to edit: ";
    std::cin >> taskIndex;

    if (taskIndex < 1 || taskIndex > static_cast<int>(tasksByDay[day - 1].tasks.size())) {
        std::cout << "Invalid task index. Please try again.\n";
        return;
    }

    Task &taskToEdit = tasksByDay[day - 1].tasks[taskIndex - 1];

    std::cin.ignore();
    std::cout << "Edit Task Name: ";
    std::getline(std::cin, taskToEdit.name);
    std::cout << "Edit Description: ";
    std::getline(std::cin, taskToEdit.description);
    std::cout << "Edit Priority (1-5): ";
    std::cin >> taskToEdit.priority;
    std::cin.ignore();
    std::cout << "Edit Status (completed/in progress/not started): ";
    std::getline(std::cin, taskToEdit.status);

    std::cout << "Task edited successfully.\n";
}

void displayTasksByDays(const std::vector<TaskByDay> &tasksByDay) {
    int numDays;
    std::cout << "Enter the number of days you want to view tasks for: ";
    std::cin >> numDays;

    std::vector<int> selectedDays;

    for (int i = 1; i < numDays+1; ++i) {
        // int day;
        // std::cout << "Enter day " << i + 1 << ": ";
        // std::cin >> day;
        selectedDays.push_back(i);
    }

    std::cout << "Task List for Selected Days:\n";
    for (int day : selectedDays) {
        if (day < 1 || day > 31) {
            std::cout << "Day " << day << " is out of range (1-31).\n";
            continue;
        }

        std::cout << "Day " << day << ":\n";
        bool tasksFound = false;

        for (const Task &task : tasksByDay[day - 1].tasks) {
            tasksFound = true;
            std::cout << " - " << task.name << " (Priority: " << task.priority << " Status: " << task.status << ")\n";
        }

        if (!tasksFound) {
            std::cout << "No tasks for day " << day << ".\n";
        }
    }
}

void deleteTask(std::vector<TaskByDay> &tasksByDay) {
    int day;
    std::cout << "Enter the day of the task you want to delete (1-31): ";
    std::cin >> day;

    if (day < 1 || day > 31) {
        std::cout << "Invalid day. Please enter a valid day (1-31).\n";
        return;
    }

    if (tasksByDay[day - 1].tasks.empty()) {
        std::cout << "No tasks for day " << day << " to delete.\n";
        return;
    }

    std::cout << "Tasks for day " << day << ":\n";
    for (size_t i = 0; i < tasksByDay[day - 1].tasks.size(); ++i) {
        const Task &task = tasksByDay[day - 1].tasks[i];
        std::cout << i + 1 << ". " << task.name << " (Priority: " << task.priority << " Status: " << task.status << ")\n";
    }

    int taskIndex;
    std::cout << "Enter the index of the task you want to delete: ";
    std::cin >> taskIndex;

    if (taskIndex < 1 || taskIndex > static_cast<int>(tasksByDay[day - 1].tasks.size())) {
        std::cout << "Invalid task index. Please try again.\n";
        return;
    }

    tasksByDay[day - 1].tasks.erase(tasksByDay[day - 1].tasks.begin() + taskIndex - 1);
    std::cout << "Task deleted successfully.\n";
}


void saveTasksToFile(const std::vector<TaskByDay> &tasksByDay, const std::string &filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
        return;
    }

    for (const TaskByDay &dayTasks : tasksByDay) {
        for (const Task &task : dayTasks.tasks) {
            file << task.name << '\n';
            file << task.description << '\n';
            file << task.priority << '\n';
            file << task.deadline.day << ' ' << task.deadline.month << ' ' << task.deadline.year << '\n';
            file << task.status << '\n';
        }
    }

    file.close();
    std::cout << "Tasks saved to file: " << filename << std::endl;
}

void loadTasksFromFile(std::vector<TaskByDay> &tasksByDay, const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file for reading." << std::endl;
        return;
    }

    tasksByDay.clear(); 

    int day, month, year;
    while (!file.eof()) {
        std::string name, description, status;
        int priority;
        if (!std::getline(file, name) || !std::getline(file, description) || !(file >> priority) ||
            !(file >> day) || !(file >> month) || !(file >> year) || !std::getline(file, status)) {
            break;
        }
        file.ignore(); 

        DateTime deadline(day, month, year);
        int taskDay = deadline.day;
        tasksByDay[taskDay - 1].tasks.push_back(Task(name, description, priority, deadline, status));
    }

    file.close();
    std::cout << "Tasks loaded from file: " << filename << std::endl;
}

int main() {
    std::vector<TaskByDay> tasksByDay(31);

    const std::string filename = "tasks.txt";
    loadTasksFromFile(tasksByDay, filename);

    char choice;

    do {
        std::cout << "Task Scheduling Application\n";
        std::cout << "1. Add Task\n";
        std::cout << "2. Edit Task\n";
        std::cout << "3. Display Tasks for Specific Days\n";
        std::cout << "4. Save and Exit\n"; 
        std::cout << "5. Delete Task\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                addTask(tasksByDay);
                break;
            case '2':
                editTask(tasksByDay);
                break;
            case '3':
                displayTasksByDays(tasksByDay);
                break;
            case '4':
                saveTasksToFile(tasksByDay, filename); 
                std::cout << "Exiting the application.\n";
                break;
            case '5':
                deleteTask(tasksByDay);
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != '4');

    return 0;
}

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

template<typename T>
class CustomStack {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    Node* top;
    size_t stackSize;
    
public:
    // Constructor
    CustomStack();
    
    // Destructor
    ~CustomStack();
    
    // Copy constructor
    CustomStack(const CustomStack& other);
    
    // Assignment operator
    CustomStack& operator=(const CustomStack& other);
    
    // Stack operations
    void push(const T& item);
    void pop();
    T& peek();
    const T& peek() const;
    bool isEmpty() const;
    size_t size() const;
    void clear();
    
    // Display stack contents (for debugging)
    void display() const;
    
    // Check if stack has elements
    bool hasElements() const;
};

// Template implementation must be in header file
template<typename T>
CustomStack<T>::CustomStack() : top(nullptr), stackSize(0) {}

template<typename T>
CustomStack<T>::~CustomStack() {
    clear();
}

template<typename T>
CustomStack<T>::CustomStack(const CustomStack& other) : top(nullptr), stackSize(0) {
    *this = other;
}

template<typename T>
CustomStack<T>& CustomStack<T>::operator=(const CustomStack& other) {
    if (this != &other) {
        clear();
        
        if (other.top != nullptr) {
            // Create a temporary stack to reverse the order
            CustomStack<T> temp;
            Node* current = other.top;
            
            while (current != nullptr) {
                temp.push(current->data);
                current = current->next;
            }
            
            // Now push from temp to this stack to maintain order
            while (!temp.isEmpty()) {
                push(temp.peek());
                temp.pop();
            }
        }
    }
    return *this;
}

template<typename T>
void CustomStack<T>::push(const T& item) {
    Node* newNode = new Node(item);
    newNode->next = top;
    top = newNode;
    stackSize++;
}

template<typename T>
void CustomStack<T>::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty - cannot pop");
    }
    
    Node* temp = top;
    top = top->next;
    delete temp;
    stackSize--;
}

template<typename T>
T& CustomStack<T>::peek() {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty - cannot peek");
    }
    return top->data;
}

template<typename T>
const T& CustomStack<T>::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty - cannot peek");
    }
    return top->data;
}

template<typename T>
bool CustomStack<T>::isEmpty() const {
    return top == nullptr;
}

template<typename T>
size_t CustomStack<T>::size() const {
    return stackSize;
}

template<typename T>
void CustomStack<T>::clear() {
    while (!isEmpty()) {
        pop();
    }
}

template<typename T>
bool CustomStack<T>::hasElements() const {
    return !isEmpty();
}

template<typename T>
void CustomStack<T>::display() const {
    if (isEmpty()) {
        std::cout << "Stack is empty\n";
        return;
    }
    
    std::cout << "Stack contents (top to bottom):\n";
    Node* current = top;
    int index = 0;
    
    while (current != nullptr) {
        std::cout << "[" << index << "] " << current->data << "\n";
        current = current->next;
        index++;
    }
}

#endif // STACK_H
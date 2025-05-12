#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

template <typename T>
class Queue {
private:
    T* data;            // Penyimpanan data
    int frontIdx;       // Indeks depan
    int rearIdx;        // Indeks belakang
    int capacity;       // Kapasitas maksimum
    int currentSize;    // Jumlah elemen saat ini

    // Fungsi untuk memperbesar kapasitas
    void resize() {
        int newCapacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = new T[newCapacity];
        
        // Salin data ke array baru
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[(frontIdx + i) % capacity];
        }
        
        delete[] data;
        data = newData;
        frontIdx = 0;
        rearIdx = currentSize;
        capacity = newCapacity;
    }

public:
    // Constructor
    Queue() : data(nullptr), frontIdx(0), rearIdx(0), capacity(0), currentSize(0) {}
    
    // Destructor
    ~Queue() {
        delete[] data;
    }
    
    // Menambahkan elemen ke belakang queue (enqueue)
    void enqueue(const T& item) {
        if (currentSize == capacity) {
            resize();
        }
        data[rearIdx] = item;
        rearIdx = (rearIdx + 1) % capacity;
        currentSize++;
    }
    
    // Menghapus elemen dari depan queue (dequeue)
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue kosong");
        }
        T item = data[frontIdx];
        frontIdx = (frontIdx + 1) % capacity;
        currentSize--;
        return item;
    }
    
    // Melihat elemen depan tanpa menghapus (peek)
    T peek() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue kosong");
        }
        return data[frontIdx];
    }
    
    // Cek apakah queue kosong
    bool isEmpty() const {
        return currentSize == 0;
    }
    
    // Mendapatkan jumlah elemen
    int size() const {
        return currentSize;
    }
    
    // Mendapatkan kapasitas maksimum
    int getCapacity() const {
        return capacity;
    }
    
    // Membersihkan seluruh queue
    void clear() {
        frontIdx = 0;
        rearIdx = 0;
        currentSize = 0;
    }
};

#endif // QUEUE_H
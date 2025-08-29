#ifndef PINARRAY_H
#define PINARRAY_H

#include <Arduino.h>

class PinArray {
public:
    PinArray(const int* pins, int size);
    void begin();
    void readAll();
    void printAll(Stream& stream = Serial);

    int getValue(int index) const;
    int getSize() const;

private:
    const int* pinArr;
    int* valArr;
    int n;
};

#endif // PINARRAY_H

#ifndef IR_H
#define IR_H

class IR{
    private:
        int size;
        int* pinArray;
    public:
        IR(int size, int* pinArray);
        int* getAll();
        int getValue(int index);
        
};


#endif

#ifndef MOTORS_H
#define MOTORS_H

class motor{
    public:
        motor();
        void setLeftSpeed(int direction, int speed);
        void setRightSpeed(int direction, int speed);
    private:
        // left motor
        int _leftMotorPinA = 33;
        int _leftMotorPinB = 35;
        int _leftEnA = 4;
        int _leftEnB = 5;
        // right motor
        int _rightMotorPinA = 36;
        int _rightMotorPinB = 37;
        int _rightEnA = 21;
        int _rightEnB = 11;

        // threshold values
        int _forward_threshold = 255;
        int _backward_threshold = 255;
};

#endif
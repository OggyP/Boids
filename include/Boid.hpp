#include <math.h>

const double PIlib = 3.141592653589793238463;

class Boid {
    public:
        int boidNumber;
        //0 is x, 1 is y
        double cordinates[2] = {0, 0};
        double vector[2] = {0, 0};
        float test;
        double distanceToCenter = 400;
        // move the cordinates by the vector
        void move() {
            cordinates[0] += vector[0] + ((rand() % 6) - 3) / 10;
            cordinates[1] += vector[1] + ((rand() % 6) - 3) / 10;
        };

        double direction() {
            double direction = atan(vector[0] / vector[1]);
            if (vector[1] == 0) {
                if (vector[0] > 0) {
                    direction = PIlib / 2;
                } else {
                    direction = -PIlib / 2;
                }
            }
            else if (vector[1] < 0) {
                if (vector[0] > 0) {
                    direction = PIlib / 2 + (PIlib / 2 + direction);
                } else {
                    direction = - (PIlib / 2) - (PIlib / 2 - direction);
                }
            }
            return direction;
        };

        double magnitude() {
            const double magnitudeFromVector = sqrt(pow(vector[0], 2) + pow(vector[1], 2));
            return magnitudeFromVector;
        };

        void setVector(double newDirection, double newMagnitude) {
            vector[0] = round((newMagnitude * sin(newDirection)) * 10000) / 10000;
            vector[1] = round((newMagnitude * cos(newDirection)) * 10000) / 10000;
        }
};
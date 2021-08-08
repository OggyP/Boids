#include <SFML/Graphics.hpp>
#include "../include/Boid.hpp"
#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

const double PI = 3.141592653589793238463;

int main()
{
    const int windowWidth = 1100;
    const int windowHeight = 1100;

    // Draw scale
    const float boidDrawScale = 3;
    const float vectorDrawScale = 22;
    
    // Setting scales
    // radians are bad, but it uses them
    const float viewAmt = 2;
    const float alwaysAvoid = 3;
    const float borderDeflectionScale = 0.005;
    const float maxMagnitude = 0.75 ;
    const float collisionAvoidanceDistance = 17;
    const float avoidanceScale = 0.002;
    const float flockDetectionRadius = 50;
    const float alignmentScale = 0.01;
    const float cohesionAmt = 0.00004;

    bool showNums = false;
    bool collisionAvoidance = true;

    int boidAmt;
    cout << "How many boids should be displayed? \n";
    cin >> boidAmt;

    Boid boidList[boidAmt];

    /* initialize random seed: */
    srand (time(NULL));

    for (int i = 0; i < boidAmt; i++) {
        Boid currentBoid;
        currentBoid.boidNumber = i;
        currentBoid.cordinates[0] = rand() % (windowWidth - 100) + 50;
        currentBoid.cordinates[1] = rand() % (windowHeight - 100) + 50;
        currentBoid.vector[0] = (float)((rand() % 200) - 100) / 100;
        currentBoid.vector[1] = (float)((rand() % 200) - 100) / 100;
        boidList[i] = currentBoid;
    }
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "OggyP Boids!");
    window.setFramerateLimit(60);
    sf::CircleShape boidDraw(boidDrawScale);
    sf::VertexArray vectorDraw(sf::LinesStrip, 2);
    vectorDraw[0].color = sf::Color::Black;
    sf::Font font;
    if (!font.loadFromFile("/home/oggyp/Projects/C++/Boids/lib/Arial.ttf"))
    {
        cout << "Error loading font";
    }
    sf::Text drawBoidID;
    drawBoidID.setFillColor(sf::Color::White);
    drawBoidID.setCharacterSize(24);
    drawBoidID.setFont(font);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            showNums = true;
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            showNums = false;
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            collisionAvoidance = true;
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        {
            collisionAvoidance = false;
        }

        window.clear();

        for (int i = 0; i < boidAmt; i++) {
            Boid &boid = boidList[i];

            // Flock variables
            int boidsInFlockRange = 0;
            double totalFlockVector[2] = {0, 0};
            double averageFlockCords[2] = {0, 0}; 

            for (int j = 0; j < boidAmt; j++) {
                Boid &boidToCheck = boidList[j];

                if (boid.boidNumber != boidToCheck.boidNumber) {
                    const double vectorToBoid[2] = {boidToCheck.cordinates[0] - boid.cordinates[0], boidToCheck.cordinates[1] - boid.cordinates[1]};
                    const double distanceToBoid = sqrt(pow(vectorToBoid[0], 2) + pow(vectorToBoid[1], 2));
                    double directionToBoid = atan(vectorToBoid[0] / vectorToBoid[1]);
                    if (vectorToBoid[1] < 0) {
                        if (vectorToBoid[1] == 0) {
                            if (vectorToBoid[0] > 0) {
                                directionToBoid = PI / 2;
                            } else {
                                directionToBoid = -PI / 2;
                            }
                        }
                        else if (vectorToBoid[0] > 0) {
                            directionToBoid = PIlib / 2 + (PIlib / 2 + directionToBoid);
                        } else {
                            directionToBoid = - (PIlib / 2) - (PIlib / 2 - directionToBoid);
                        }
                    }

                    // // Avoid other boids code
                    if (distanceToBoid < collisionAvoidanceDistance && collisionAvoidance) {
                        const float boidDirection = boid.direction();
                        if (directionToBoid > boidDirection - viewAmt && directionToBoid < boidDirection + viewAmt) {
                            if (directionToBoid > boidDirection) {
                                boid.setVector(boidDirection - ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                            } else {
                                boid.setVector(boidDirection + ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                            }
                        } 
                        else if (PI + directionToBoid + PI - boidDirection < viewAmt) {
                            boid.setVector(boidDirection - ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                        }
                        else if (PI - directionToBoid + PI + boidDirection < viewAmt) {
                            boid.setVector(boidDirection + ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                        } else if (distanceToBoid < alwaysAvoid) {
                            if (directionToBoid > boidDirection) {
                                boid.setVector(boidDirection - ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                            } else {
                                boid.setVector(boidDirection + ((collisionAvoidanceDistance - distanceToBoid) * avoidanceScale), boid.magnitude());
                            }
                        }
                    }

                    if (distanceToBoid < flockDetectionRadius) {
                        boidsInFlockRange ++;
                        totalFlockVector[0] += boidToCheck.vector[0];
                        totalFlockVector[1] += boidToCheck.vector[1];
                        averageFlockCords[0] += boidToCheck.cordinates[0];
                        averageFlockCords[1] += boidToCheck.cordinates[1];
                    }
                }
            }

            if (boidsInFlockRange > 0) {
                boid.vector[0] += ((totalFlockVector[0] / boidsInFlockRange) -  boid.vector[0]) * alignmentScale;
                boid.vector[1] += ((totalFlockVector[1] / boidsInFlockRange) -  boid.vector[1]) * alignmentScale;
                boid.vector[0] += boid.vector[0] * alignmentScale;
                boid.vector[1] += boid.vector[1] * alignmentScale;
            }

            const double distanceToCenter = sqrt(pow((double)(boid.cordinates[0] - windowWidth / 2), (double)2) + pow((double)(boid.cordinates[1] - windowHeight / 2), (double)2));

            if (distanceToCenter > 450) {
                if (boid.cordinates[0] < windowWidth / 2) {
                    // left x
                    boid.vector[0] += borderDeflectionScale;
                } else {
                    boid.vector[0] -= borderDeflectionScale;
                }
                if (boid.cordinates[1] < windowHeight / 2) {
                    // left x
                    boid.vector[1] += borderDeflectionScale;
                } else {
                    boid.vector[1] -= borderDeflectionScale;
                }
                // for cicle thingo
                // if (boid.distanceToCenter < distanceToCenter) {
                //     boid.setVector(boid.direction() + borderDeflectionScale, maxMagnitude);
                //     boid.distanceToCenter = distanceToCenter;
                // }
            }

            if (boid.magnitude() > maxMagnitude) {
                boid.setVector(boid.direction(), maxMagnitude);
            }

            if (boidsInFlockRange > 0) {
                boid.vector[0] += cohesionAmt * ((averageFlockCords[0] / boidsInFlockRange) - boid.cordinates[0]);
                boid.vector[1] += cohesionAmt * ((averageFlockCords[1] / boidsInFlockRange) - boid.cordinates[1]);
            }
            
            boid.move();

            // Draw Boid
            boidDraw.setPosition(boid.cordinates[0] - boidDrawScale, boid.cordinates[1] - boidDrawScale);
            boidDraw.setFillColor(sf::Color{(sf::Uint8)((boidsInFlockRange * 2) + 40), (sf::Uint8)(boid.cordinates[0] / 8), (sf::Uint8)(boid.cordinates[1] / 8)});
            window.draw(boidDraw);

            // Draw Vector
            vectorDraw[0].position = sf::Vector2f(boid.cordinates[0], boid.cordinates[1]);
            vectorDraw[1].position = sf::Vector2f(boid.cordinates[0]+ (boid.vector[0] * vectorDrawScale), boid.cordinates[1] + (boid.vector[1] * vectorDrawScale));
            window.draw(vectorDraw);
            
            if (showNums) {
                drawBoidID.setString(to_string(boid.boidNumber));
                drawBoidID.setPosition(boid.cordinates[0], boid.cordinates[1]);
                window.draw(drawBoidID);
            }
        }
         
        window.display();
    }
 
    return 0;
}
/*************************************************************
 * 1. Name:
 *      -your name-
 * 2. Assignment Name:
 *      Practice 02: Physics simulator
 * 3. Assignment Description:
 *      Compute how the Apollo lander will move across the screen
 * 4. What was the hardest part? Be as specific as possible.
 *      -a paragraph or two about how the assignment went for you-
 * 5. How long did it take for you to complete the assignment?
 *      -total time in hours: reading the assignment, submitting, etc.
 **************************************************************/

#define _USE_MATH_DEFINES
#include <iostream>  // for CIN and COUT
#include <cmath>
//#include <string.h>
#include <string>
using namespace std;

#define WEIGHT   15103.000   // Weight in KG
#define GRAVITY     -1.625   // Vertical acceleration due to gravity, in m/s^2
#define THRUST   45000.000   // Thrust of main engine, in Newtons (kg m/s^2)


struct physics{
    double dx;                  // speed in the x direction
    double dy;                  // speed in the y direction
    double y;                   // y coordinate
    double x;                   // x coordinate
    double aDegrees;            // angle of the rovor in degrees
    double t;                   // time units
    double aRadians;            // Angle in radians
    double accelerationThrust;  // Acceleration due to thrust 
    double ddxThrust;           // Horizontal acceleration due to thrust
    double ddyThrust;           // Vertical acceleration due to thrust
    double ddx;                 // Total horizontal acceleration
    double ddy;                 // Total vertical acceleration
    double v;                   // Total velocity
    int time;                // keep track of the time the rover has been running
};
/***************************************************
 * COMPUTE DISTANCE
 * Apply inertia to compute a new position using the distance equation.
 * The equation is:
 *     s = s + v t + 1/2 a t^2
 * INPUT
 *     s : original position, in meters
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     s : new position, in meters
 **************************************************/
 // your function goes here
double computeDistance(double s, double v, double a, double t) {
    double newPosition = s + (v * t) + ((a * (t * t)) / 2);
    return newPosition;
}

/**************************************************
 * COMPUTE ACCELERATION
 * Find the acceleration given a thrust and mass.
 * This will be done using Newton's second law of motion:
 *     f = m * a
 * INPUT
 *     f : force, in Newtons (kg * m / s^2)
 *     m : mass, in kilograms
 * OUTPUT
 *     a : acceleration, in meters/second^2
 ***************************************************/
 // your function goes here
double computeAcceleration(double force, double mass) {
    double acceleration = force / mass;
    return acceleration;
}

/***********************************************
 * COMPUTE VELOCITY
 * Starting with a given velocity, find the new
 * velocity once acceleration is applied. This is
 * called the Kinematics equation. The
 * equation is:
 *     v = v + a t
 * INPUT
 *     v : velocity, in meters/second
 *     a : acceleration, in meters/second^2
 *     t : time, in seconds
 * OUTPUT
 *     v : new velocity, in meters/second
 ***********************************************/
 // your function goes here
double computeVelocity(double initialVelocity, double acceleration, double time) {
    double newVelocity = (initialVelocity + acceleration * time);
    return newVelocity;
}


/***********************************************
 * COMPUTE VERTICAL COMPONENT
 * Find the vertical component of a velocity or acceleration.
 * The equation is:
 *     cos(a) = y / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     y : the vertical component of the total
 ***********************************************/
 // your function goes here
double computeVertical(double a, double total) {
    double y = cos(a) * total;
    return y;
}

/***********************************************
 * COMPUTE HORIZONTAL COMPONENT
 * Find the horizontal component of a velocity or acceleration.
 * The equation is:
 *     sin(a) = x / total
 * This can be expressed graphically:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    |a/
 *    |/
 * INPUT
 *     a : angle, in radians
 *     total : total velocity or acceleration
 * OUTPUT
 *     x : the vertical component of the total
 ***********************************************/
 // your function goes here
double computeHorizontal(double a, double x) {
    double total = x * sin(a);
    return total;
}

/************************************************
 * COMPUTE TOTAL COMPONENT
 * Given the horizontal and vertical components of
 * something (velocity or acceleration), determine
 * the total component. To do this, use the Pythagorean Theorem:
 *    x^2 + y^2 = t^2
 * where:
 *      x
 *    +-----
 *    |   /
 *  y |  / total
 *    | /
 *    |/
 * INPUT
 *    x : horizontal component
 *    y : vertical component
 * OUTPUT
 *    total : total component
 ***********************************************/
 // your function goes here
double computeTotalComponent(double horizontalComp, double verticalComp) {
    double radicand = (horizontalComp * horizontalComp) + (verticalComp * verticalComp);
    double totalComp = sqrt(radicand);
    return totalComp;
}


/*************************************************
 * RADIANS FROM DEGEES
 * Convert degrees to radians:
 *     radians / 2pi = degrees / 360
 * INPUT
 *     d : degrees from 0 to 360
 * OUTPUT
 *     r : radians from 0 to 2pi
 **************************************************/
 // your function goes here
double radiansFromDegrees(double degrees) {
    double radians;
    radians = (degrees / 360.0) * (2 * M_PI);
    return radians;
}

/**************************************************
 * PROMPT
 * A generic function to prompt the user for a double
 * INPUT
 *      message : the message to display to the user
 * OUTPUT
 *      response : the user's response
 ***************************************************/
 // your function goes here
double prompt(string message) {
    double userInput;
    cout << message;
    cin >> userInput;
    return userInput;
}

/****************************************************************
 * Get initial Conditions
 * Prompt for initial conditions of the rover, save within a 
 * struct called physics
 ****************************************************************/
void get_rover_init(physics &rover) {
    rover.dx = prompt("What is your horizontal velocity (m/s)? ");
    rover.dy = prompt("What is your vertical velocity (m/s)? ");
    rover.y = prompt("what is your altitute (m)? ");
    rover.x = 0;
    rover.aDegrees = prompt("What is the angle of hte LM where 0 is up (degrees)? ");
    rover.t = 1;//prompt("What is the time interval (s)? ");
    rover.time = 0;
}

/****************************************************************
 * Rover Move
 * Have the Rover move one time unit
 ****************************************************************/
void rover_move(physics &rover) {
    rover.accelerationThrust = computeAcceleration(THRUST, WEIGHT);           //compute the acceleration due to thrust
    rover.aRadians = radiansFromDegrees(rover.aDegrees);                            //compute the angle in radians
    rover.ddxThrust = computeHorizontal(rover.aRadians, rover.accelerationThrust);
    rover.ddyThrust = computeVertical(rover.aRadians, rover.accelerationThrust) + GRAVITY;
    rover.ddx = computeHorizontal(rover.aRadians, rover.dx);
    rover.ddy = computeVertical(rover.aRadians, rover.dy);
    rover.dx = computeVelocity(rover.dx, rover.ddxThrust, rover.t);
    rover.dy = computeVelocity(rover.dy, rover.ddyThrust, rover.t);
    rover.v = computeTotalComponent(rover.dx, rover.dy);
    rover.x = computeDistance(rover.x, rover.dx, rover.ddxThrust, rover.t);
    rover.y = computeDistance(rover.y, rover.dy, rover.ddyThrust, rover.t);
    rover.time = rover.time + rover.t;
}

/****************************************************************
 * Display
 * Display the rovers position, component velocity and total
 * velocity
 ****************************************************************/
void display(physics &rover) {
    //string time = to_string(rover.time);
    cout.setf(ios::fixed | ios::showpoint);
    cout.precision(2);
    cout << rover.time << "s - x,y:(" << rover.x << ", " << rover.y << ")m  dx,dy:(" << rover.dx << ", "
        << rover.dy << ")m/s  speed:" << rover.v << "m/s  angle:" << rover.aDegrees << "\n";
}

/****************************************************************
 * RUN TESTS
 * Tests the functionality of the code, will not be shown in final
 * product
 ****************************************************************/
void run_tests() {
    physics test_1 = {}; //initialize the first rover test
    test_1.dx = -35.00;
    test_1.dy = -15.00;
    test_1.x = 0;
    test_1.y = 207;
    test_1.aDegrees = 90;
    test_1.t = 1;
    test_1.time = 0;
    physics test_2 = {}; //initilize the second rover test
    test_2.dx = 10;
    test_2.dy = -10;
    test_2.x = 0;
    test_2.y = 56.11;
    test_2.aDegrees = -42.185;
    test_2.t = 1;
    test_2.time = 0;
    int sec;
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) {
        rover_move(test_1);
        display(test_1);
    }
    test_1.aDegrees = 45;
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) {
        rover_move(test_1);
        display(test_1);
    }
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) {
        rover_move(test_2);
        display(test_2);
    }
    test_2.aDegrees = 0;
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) {
        rover_move(test_2);
        display(test_2);
    }

}

/****************************************************************
 * RUN
 * Controlls and runs the code for the physics portion
 ****************************************************************/
void run() {
    physics rover = {}; //create a blank physics struct to track the rovor
    get_rover_init(rover); //get inital conditions of the rover
    int sec;                //initialize counter
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) { //for every condition move and display 5 times
        rover_move(rover);
        display(rover);
    }
    cout << endl;
    rover.aDegrees = prompt("What is the new angle of the LM where 0 is up (degrees)? "); //get new rover angle
    cout << "\nFor the next 5 seconds with the main engine on, the position of the lander is: \n\n";
    for (sec = 0; sec < 5; sec++) { //move 5 more times
        rover_move(rover);
        display(rover);
    }
}
/****************************************************************
 * MAIN
 * Prompt for input, compute new position, and display output
 ****************************************************************/
int main()
{
    run();
    run_tests();
    return 0;
}
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

//Main window
const int windowWidth = 464; //29
const int windowHeight = 512; //32

//Pacman animation
float startAngle = 60.0f; // Start angle of the arc
float endAngle = 300.0f; // End angle of the arc
int animationSpeed = 5; // Animation speed in degrees per frame
int pacman_anim_state = 0; //0=close 1 = open
int pacman_angle = 0;
bool displayPacman = true;

//Map squares size
const int squareSize = 16;

//Food related
const int foodSize = 4;
const int food_offset = 16;

//Pacman move direction
int direction = 0; // R L U D

//Pacman position and speed
float pacman_origin[2] = { 232.0f, 224.0f };
float pac_man_pos_x = 232.0f;
float pac_man_pos_y = 224.0f;
float speed = 1.0f;
int pacman_lives = 3;
bool pacmanAwake = true;

//Pacman Rays
int offsetRay1 = 15;
int offsetRay2 = 7;

//Wall declation
struct Wall {
    int x;
    int y;
    int size;
};
std::vector<Wall> walls;

//Food declaration
struct Food {
    int x = 0;
    int y = 0;
    int size = 0;
};
std::vector<Food> foods;

//used as a bool to run a given function only when the program starts and not every frame
int awake = 0;

//Draws Rays if true
bool drawGizmos = false;

//False keypress Manager
int coyoteTime = 30;
int coyoteTimeCounter = 0;
int directionToMove = 0;

//Rays delation
struct Ray {
    int rayEndpointX = 0;
    int rayEndpointY = 0;
    int x_offset = 0;
    int y_offset = 0;
};

//player Rays
int rayLength = 16, rayLength_Food = 6;
Ray rightRay1, rightRay2, rightRay3, rightRay4, rightRay5;
Ray leftRay1, leftRay2, leftRay3, leftRay4, leftRay5;
Ray topRay1, topRay2, topRay3, topRay4, topRay5;
Ray bottomRay1, bottomRay2, bottomRay3, bottomRay4, bottomRay5;

//Right, Left, Up, Down
bool canMoveDirection[4] = { false, false, false, false };

//Ghost declaration
struct Ghost {
    int x;
    int y;
    float color[3];
    int direction = 0;
    bool canMove[4] = { false, false, false, false };
    bool prevCanMove[4] = { false, false, false, false };
    Ray Rays[4][5] = { {{0,0,0,15}, {0,0,0,-15}, {0,0,0,7}, {0,0,0,-7} , {0,0,0,0}},
                        {{0,0,0,15}, {0,0,0,-15}, {0,0,0,7}, {0,0,0,-7} , {0,0,0,0}},
                        {{0,0,15,0}, {0,0,-15,0}, {0,0,7,0}, {0,0,-7,0} , {0,0,0,0}},
                        {{0,0,15,0}, {0,0,-15,0}, {0,0,7,0}, {0,0,-7,0} , {0,0,0,0}}
    };
};

//Ghost related
int ghostSize = 22;
int ghostSpeed = 1;

//Initializing Ghosts
Ghost orangeGhost = { 22, 22,{ 1.0f, 0.7215f, 0.32156f },0};
Ghost cyanGhost = { 22, 470,{ 0.0f, 1.0f, 1.0f },0};
Ghost pinkGhost = { 400, 470,{ 1.0f, 0.721f, 1.0f },1};
Ghost redGhost = { 400, 22,{ 1.0f, 0.0f, 0.0f },1 };

//Map
const int map[32][29] = { //0 = playable & eatable area ; 1 = Walls ; 2 = non playable & eatable area
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1},
    {1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,0,1,1,1,1,2,2,1,2,2,1,1,1,1,0,0,1,1,1,1,1,1},
    {2,2,2,2,2,1,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,1,2,2,2,2,2},
    {2,2,2,2,2,1,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,1,2,2,2,2,2},
    {1,1,1,1,1,1,0,0,1,2,2,1,1,1,2,1,1,1,2,2,1,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,2,2,2,1,2,2,2,2,2,1,2,2,2,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,2,2,2,1,2,2,2,2,2,1,2,2,2,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,0,1,2,2,1,1,1,1,1,1,1,2,2,1,0,0,1,1,1,1,1,1},
    {2,2,2,2,2,1,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,1,2,2,2,2,2},
    {2,2,2,2,2,1,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,1,2,2,2,2,2},
    {1,1,1,1,1,1,0,0,1,2,2,1,1,1,1,1,1,1,2,2,1,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//Drawing Map
void drawMap02() {
    glColor3f(0.0f, 0.0f, 1.0f);
    int k = 0;
    for (int i = 31; i >= 0; i--, k++) {
        for (int j = 0; j < 29; j++) {
            if (map[k][j] == 1) {
                glBegin(GL_QUADS);
                glVertex2f(j * squareSize, i * squareSize);
                glVertex2f(j * squareSize + squareSize, i * squareSize);
                glVertex2f(j * squareSize + squareSize, i * squareSize + squareSize);
                glVertex2f(j * squareSize, i * squareSize + squareSize);
                glEnd();

                if (awake == 0) {
                    walls.push_back({ j * squareSize, i * squareSize, squareSize });
                }
            }
        }
    }
}

//Drawing Pacman Outline
void drawPacman() {
    glColor3f(255.0f, 242.0f, 0.0f); // Red color
    glPushMatrix();

    float centerX = pac_man_pos_x; // X-coordinate of the center
    float centerY = pac_man_pos_y; // Y-coordinate of the center
    float radius = 10.0f;
    int numSegments = 8; // Number of line segments to approximate the arc

    // Translate the object to the rotation point
    glTranslatef(centerX, centerY, 0.0f);

    // Apply rotation around the rotation point
    glRotatef(pacman_angle, 0.0f, 0.0f, 1.0f);

    // Translate back to the original position
    glTranslatef(-centerX, -centerY, 0.0f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center of the filled arc
    for (int i = 0; i <= numSegments; i++) {
        float angle = startAngle + (endAngle - startAngle) * float(i) / float(numSegments);
        float x = centerX + radius * cos(angle * 3.14159 / 180.0);
        float y = centerY + radius * sin(angle * 3.14159 / 180.0);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}

//Handles Pacman Animation
void pacMan_anim() {
    if (pacman_anim_state == 0) {
        startAngle -= animationSpeed;
        endAngle += animationSpeed;
    }
    else {
        startAngle += animationSpeed;
        endAngle -= animationSpeed;
    }
    if (startAngle == 0.0f) {
        pacman_anim_state = 1;
    }
    if (startAngle == 60.0f) {
        pacman_anim_state = 0;
    }
}

//Moves Pacman
void updatePacmanPos() {
    if (direction == 0 && canMoveDirection[0] == true) {
        pac_man_pos_x += speed;
        pacman_angle = 0;
    }if (direction == 1 && canMoveDirection[1] == true) {
        pac_man_pos_x -= speed;
        pacman_angle = 180;
    }if (direction == 2 && canMoveDirection[2] == true) {
        pac_man_pos_y += speed;
        pacman_angle = 90;
    }if (direction == 3 && canMoveDirection[3] == true) {
        pac_man_pos_y -= speed;
        pacman_angle = 270;
    }
}

//Draws Food Pellets
void drawFood() {
    glColor3f(255.0f, 184.0f, 151.0f);
    int k = 0;
    for (int i = 31; i >= 0 && awake == 0; i--, k++) {
        for (int j = 0; j < 29; j++) {
            if (map[k][j] == 0 && map[k - 1][j] == 0 && map[k - 1][j + 1] == 0 && map[k][j + 1] == 0) {

                foods.push_back({ j * squareSize + food_offset, i * squareSize + food_offset, foodSize });
            }
        }
    }

    for (const auto& food : foods) {
        glColor3f(255.0f, 184.0f, 151.0f);
        glBegin(GL_QUADS);
        glVertex2f(food.x, food.y);
        glVertex2f(food.x + foodSize, food.y);
        glVertex2f(food.x + foodSize, food.y + foodSize);
        glVertex2f(food.x, food.y + foodSize);
        glEnd();
    }
}

//Collision Detection of Walls for both player and ghost
//Takes in an array of lines or Rays and returns True if collided and false if not collided
//Function reference
bool detectCollisionForMultipleLines(Ray* rays);

//Ghost Start 
//Should implement ghost change direction
void ghostDisplay(Ghost ghost) {
    glColor3f(ghost.color[0], ghost.color[1], ghost.color[2]);
    
    //head semicircle
    float ghostStartAngle = 0.0f;
    float ghostEndAngle = 180.0f;
    int numSegments = 8;
    int head_radius = 10;
    int ghost_head_offset[2] = { 11,10 };
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ghost.x+ghost_head_offset[0], ghost.y + ghost_head_offset[1]); // Center of the filled arc
    for (int i = 0; i <= numSegments; i++) {
        float angle = ghostStartAngle + (ghostEndAngle - ghostStartAngle) * float(i) / float(numSegments);
        float x = ghost.x + ghost_head_offset[0] + head_radius * cos(angle * 3.14159 / 180.0);
        float y = ghost.y + +ghost_head_offset[1] + head_radius * sin(angle * 3.14159 / 180.0);
        glVertex2f(x, y);
    }
    glEnd();

    //body Square
    int ghostBodySize[2] = {10,22};
    int ghost_body_offset[2] = { 0, 0 };
    glBegin(GL_QUADS);
    glVertex2f(ghost.x + ghost_body_offset[0], ghost.y + ghost_body_offset[1]);
    glVertex2f(ghost.x + ghost_body_offset[0] + ghostBodySize[1], ghost.y + ghost_body_offset[1]);
    glVertex2f(ghost.x + ghost_body_offset[0] + ghostBodySize[1], ghost.y + ghostBodySize[0] + ghost_body_offset[1]);
    glVertex2f(ghost.x + ghost_body_offset[0], ghost.y + ghostBodySize[0] + ghost_body_offset[1]);
    glEnd();

    //bottom circles
    int bottom_circles_position[3][2] = { {ghost.x+5,ghost.y},{ghost.x + 12,ghost.y},{ghost.x + 17,ghost.y} };
    float ghostStartAngle_bottom = 180.0f;
    float ghostEndAngle_bottom = 360.0f;
    int numSegments_bottom = 5;
    int bottom_circle_radius = 3;
    for (int j = 0; j < 3; j++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(bottom_circles_position[j][0], bottom_circles_position[j][1]); // Center of the filled arc
        for (int i = 0; i <= numSegments_bottom; i++) {
            float angle = ghostStartAngle_bottom + (ghostEndAngle_bottom - ghostStartAngle_bottom) * float(i) / float(numSegments_bottom);
            float x = bottom_circles_position[j][0] + bottom_circle_radius * cos(angle * 3.14159 / 180.0);
            float y = bottom_circles_position[j][1] + bottom_circle_radius * sin(angle * 3.14159 / 180.0);
            glVertex2f(x, y);
        }
        glEnd();
    }

    //Eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    int ghost_eye_pos[2][2] = { {ghost.x+6,ghost.y+10},{ghost.x+16,ghost.y+10} };
    int ghost_eye_radius = 5;
    for (int j = 0; j < 3; j++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(ghost_eye_pos[j][0], ghost_eye_pos[j][1]); // Center of the filled arc
        for (int i = 0; i <= 20; i++) {
            float angle = 0.0f + (360.0f - 0.0f) * float(i) / float(20);
            float x = ghost_eye_pos[j][0] + ghost_eye_radius * cos(angle * 3.14159 / 180.0);
            float y = ghost_eye_pos[j][1] + ghost_eye_radius * sin(angle * 3.14159 / 180.0);
            glVertex2f(x, y);
        }
        glEnd();
    }

    //Eyeball
    glColor3f(0.0f, 0.0f, 0.0f);
    int ghost_eyeball_pos[2][2] = { {ghost.x + 6,ghost.y + 10},{ghost.x + 16,ghost.y + 10} };
    int ghost_eyeball_radius = 2;
    for (int j = 0; j < 3; j++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(ghost_eyeball_pos[j][0], ghost_eyeball_pos[j][1]); // Center of the filled arc
        for (int i = 0; i <= 20; i++) {
            float angle = 0.0f + (360.0f - 0.0f) * float(i) / float(20);
            float x = ghost_eyeball_pos[j][0] + ghost_eyeball_radius * cos(angle * 3.14159 / 180.0);
            float y = ghost_eyeball_pos[j][1] + ghost_eyeball_radius * sin(angle * 3.14159 / 180.0);
            glVertex2f(x, y);
        }
        glEnd();
    }
}
//Ray Endpoint updation
Ray updateRayEndpointPos_Ghost(Ghost ghost, Ray rayLine, int rayLength_x, int rayLength_y) {
    Ray ray = rayLine;
    ray.rayEndpointX = ghost.x + 10 + rayLength_x + ray.x_offset;
    ray.rayEndpointY = ghost.y + 10 + rayLength_y + ray.y_offset;
    return ray;
}
//ghost movement
Ghost ghostMoveFunction(Ghost ghost) {
    if (ghost.direction == 0 && ghost.canMove[0] == true) {
        ghost.x += ghostSpeed;
    }if (ghost.direction == 1 && ghost.canMove[1] == true) {
        ghost.x -= ghostSpeed;
    }if (ghost.direction == 2 && ghost.canMove[2] == true) {
        ghost.y += ghostSpeed;
    }if (ghost.direction == 3 && ghost.canMove[3] == true) {
        ghost.y -= ghostSpeed;
    }

    return ghost;
}
//Rays for Ghost
Ghost drawRays(Ghost ghost, int rayLength, int rayno) {
    for (int j = 0; j < 4; j++) {
        int length = sizeof(ghost.Rays[j]) / sizeof(ghost.Rays[j][0]);

        for (int i = 0; i < length; i++) {
            if (j == 0) {
                ghost.Rays[j][i] = updateRayEndpointPos_Ghost(ghost, ghost.Rays[j][i], rayLength, 0);
            }
            else if (j == 1) {
                ghost.Rays[j][i] = updateRayEndpointPos_Ghost(ghost, ghost.Rays[j][i], -rayLength, 0);
            }
            else if (j == 2) {
                ghost.Rays[j][i] = updateRayEndpointPos_Ghost(ghost, ghost.Rays[j][i], 0, rayLength);
            }
            else if (j == 3) {
                ghost.Rays[j][i] = updateRayEndpointPos_Ghost(ghost, ghost.Rays[j][i], 0, -rayLength);
            }
            if (drawGizmos) {
                if (ghost.canMove[j] != true) {
                    glColor3f(1.0f, 0.0f, 0.0f);
                }
                else {
                    glColor3f(0.0f, 1.0f, 0.0f);
                }
                glBegin(GL_LINES);
                glVertex2f(ghost.x + ghost.Rays[j][i].x_offset + 10, ghost.y + ghost.Rays[j][i].y_offset + 10);
                glVertex2f(ghost.Rays[j][i].rayEndpointX, ghost.Rays[j][i].rayEndpointY);
                glEnd();
            }
        }

        if (detectCollisionForMultipleLines(ghost.Rays[j]))
            ghost.canMove[j] = false;
        else
            ghost.canMove[j] = true;
    }
    return ghost;
}

//Function to copy an array from one var to another
Ghost copyArray(Ghost ghost) {
    for (int i = 0; i < 4; i++) {
        ghost.prevCanMove[i] = ghost.canMove[i];
    }
    return ghost;
}

//Function that gives an index value from a bool array that s true
int getRandomIndexWithTrueValue(const bool* arr) {
    int randomIndex;
    do {
        randomIndex = std::rand() % 4;
    } while (!arr[randomIndex]);
    return randomIndex;
}

//Function to check if two bool arrays are equal
bool areArraysEqual(bool arr1[], bool arr2[]) {
    for (int i = 0; i < 4; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}
//Ghost Cahnge Direction
Ghost ghost_change_direction(Ghost ghost) {
    if (!areArraysEqual(ghost.canMove, ghost.prevCanMove)) {

        ghost.direction = getRandomIndexWithTrueValue(ghost.canMove);
    }
    ghost = copyArray(ghost);
    return ghost;
}

//Main Ghost function
Ghost drawGhost(Ghost ghost) {
    ghostDisplay(ghost);
    ghost = drawRays(ghost, rayLength, 0);
    ghost = ghost_change_direction(ghost);
    ghost = ghostMoveFunction(ghost);
    return ghost;
}
//Ghost End

//Runs every frame
void update(int value) {

    pacMan_anim();

    // Redraw the scene
    glutPostRedisplay();

    // Set the next update
    glutTimerFunc(16, update, 0); // 16 milliseconds = 60 frames per second
}

//Handles key presses
void arrowFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        directionToMove = 0;
        break;
    case GLUT_KEY_LEFT:
        directionToMove = 1;
        break;
    case GLUT_KEY_UP:
        directionToMove = 2;
        break;
    case GLUT_KEY_DOWN:
        directionToMove = 3;
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') {
        // Exit the program
        exit(0);
    }
}

//False Time.
void coyoteTimeFunc() {
    if (directionToMove == direction) {
        coyoteTimeCounter = coyoteTime;
        return;
    }
    if (coyoteTimeCounter < 0) {
        directionToMove = direction;
    }
    else {
        coyoteTimeCounter -= 1;
    }
    if (canMoveDirection[directionToMove] == true && coyoteTimeCounter >= 0) {
        direction = directionToMove;
    }
}

//Collision detection for walls for a single Ray
bool detectCollision(Ray ray) {
    for (const auto& wall : walls) {
        if (ray.rayEndpointX >= wall.x && ray.rayEndpointX <= (wall.x + wall.size) && ray.rayEndpointY >= wall.y && ray.rayEndpointY <= (wall.y + wall.size)) {
            return true;
        }
    }
    return false;
}

//called function explained above
bool detectCollisionForMultipleLines(Ray* rays) {
    for (int i = 0; i < 5; i++) {
        Ray ray = rays[i];
        for (const auto& wall : walls) {
            if (ray.rayEndpointX >= wall.x && ray.rayEndpointX <= (wall.x + wall.size) && ray.rayEndpointY >= wall.y && ray.rayEndpointY <= (wall.y + wall.size)) {
                return true;
            }
        }
    }
    return false;
}

//Collision detection between pacman and ghost
bool detectCollisionForMultipleLines_Pacman_Ghost(Ray ray) {
        //orange Ghost
        if (ray.rayEndpointX >= orangeGhost.x && ray.rayEndpointX <= (orangeGhost.x + ghostSize) && ray.rayEndpointY >= orangeGhost.y && ray.rayEndpointY <= (orangeGhost.y + ghostSize)) {
            return true;
        }
        //pink Ghost
        if (ray.rayEndpointX >= pinkGhost.x && ray.rayEndpointX <= (pinkGhost.x + ghostSize) && ray.rayEndpointY >= pinkGhost.y && ray.rayEndpointY <= (pinkGhost.y + ghostSize)) {
            return true;
        }
        //cyan Ghost
        if (ray.rayEndpointX >= cyanGhost.x && ray.rayEndpointX <= (cyanGhost.x + ghostSize) && ray.rayEndpointY >= cyanGhost.y && ray.rayEndpointY <= (cyanGhost.y + ghostSize)) {
            return true;
        }
        //red Ghost
        if (ray.rayEndpointX >= redGhost.x && ray.rayEndpointX <= (redGhost.x + ghostSize) && ray.rayEndpointY >= redGhost.y && ray.rayEndpointY <= (redGhost.y + ghostSize)) {
            return true;
        }

    return false;
}

//same function but for food
Food detectCollisionForMultipleLines_Food(Ray ray) {
    for (const auto& food : foods) {
        if (ray.rayEndpointX >= food.x && ray.rayEndpointX <= (food.x + food.size) && ray.rayEndpointY >= food.y && ray.rayEndpointY <= (food.y + food.size)) {
            return food;
        }
    }

    Food emptyFood;
    return emptyFood;
}

//Update ray endpoints for player
Ray updateRayEndpointPos(Ray rayLine, int rayLength_x, int rayLength_y) {
    Ray ray = rayLine;
    ray.rayEndpointX = pac_man_pos_x + rayLength_x + ray.x_offset;
    ray.rayEndpointY = pac_man_pos_y + rayLength_y + ray.y_offset;
    return ray;
}

//For player
Ray drawLineFunc(Ray rayLine, int rayNo, int x_offset, int y_offset, int rayLengths) {

    Ray ray = rayLine;

    ray.x_offset = x_offset;
    ray.y_offset = y_offset;

    if (rayNo == 0)
        ray = updateRayEndpointPos(ray, rayLengths, 0);
    else if (rayNo == 1)
        ray = updateRayEndpointPos(ray, -rayLengths, 0);
    else if (rayNo == 2)
        ray = updateRayEndpointPos(ray, 0, rayLengths);
    else if (rayNo == 3)
        ray = updateRayEndpointPos(ray, 0, -rayLengths);

    if (drawGizmos) {
        if (detectCollision(rayLine)) {
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        else {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        glBegin(GL_LINES);
        glVertex2f(pac_man_pos_x + rayLine.x_offset, pac_man_pos_y + rayLine.y_offset);
        glVertex2f(rayLine.rayEndpointX, rayLine.rayEndpointY);
        glEnd();
    }

    return ray;
}

void pacman_dead_state() {
    pacman_lives--;
    if(pacman_lives >= 0)
        printf("Lives Left = %d\n", pacman_lives);
    if (pacman_lives == 0 && pacmanAwake) {
        printf("Pacman Dead\n");
        pacmanAwake = false;
        displayPacman = false;
    }
    pac_man_pos_x = pacman_origin[0];
    pac_man_pos_y = pacman_origin[1];
    directionToMove = 0;
    direction = 0;
}

void pacman_win_state() {
    int count = 0;
    for (const auto& food : foods) {
        count++;
    }
    if (count <= 0 && pacmanAwake) {
        printf("\n*********************Win*************************");
        pacmanAwake = false;
        orangeGhost.x = 700;
        orangeGhost.y = 700;

        cyanGhost.x = 800;
        cyanGhost.y = 800;
        
        pinkGhost.x = 900;
        pinkGhost.y = 900;
        
        redGhost.x = 900;
        redGhost.y = 900;
    }
}

//for player
void drawRays() {

    Food eaten;

    rightRay1 = drawLineFunc(rightRay1, 0, 0, offsetRay1, rayLength);
    rightRay2 = drawLineFunc(rightRay2, 0, 0, -offsetRay1, rayLength);
    rightRay3 = drawLineFunc(rightRay3, 0, 0, offsetRay2, rayLength);
    rightRay4 = drawLineFunc(rightRay4, 0, 0, -offsetRay2, rayLength);
    rightRay5 = drawLineFunc(rightRay5, 0, 0, 0, rayLength_Food);

    Ray rightRays[] = { rightRay1, rightRay2, rightRay3, rightRay4, rightRay5 };
    if (detectCollisionForMultipleLines(rightRays))
        canMoveDirection[0] = false;
    else
        canMoveDirection[0] = true;

    if (detectCollisionForMultipleLines_Pacman_Ghost(rightRay5))
        pacman_dead_state();

    eaten = detectCollisionForMultipleLines_Food(rightRay5);
    if (eaten.size != 0) {
        foods.erase(std::remove_if(foods.begin(), foods.end(), [&](const Food& p) { return p.x == eaten.x && p.y == eaten.y && p.size == eaten.size; }), foods.end());
    }

    leftRay1 = drawLineFunc(leftRay1, 1, 0, offsetRay1, rayLength);
    leftRay2 = drawLineFunc(leftRay2, 1, 0, -offsetRay1, rayLength);
    leftRay3 = drawLineFunc(leftRay3, 1, 0, offsetRay2, rayLength);
    leftRay4 = drawLineFunc(leftRay4, 1, 0, -offsetRay2, rayLength);
    leftRay5 = drawLineFunc(leftRay5, 1, 0, 0, rayLength_Food);

    Ray leftRays[] = { leftRay1, leftRay2, leftRay3, leftRay4, leftRay5 };
    if (detectCollisionForMultipleLines(leftRays))
        canMoveDirection[1] = false;
    else
        canMoveDirection[1] = true;

    if (detectCollisionForMultipleLines_Pacman_Ghost(leftRay5))
        pacman_dead_state();

    eaten = detectCollisionForMultipleLines_Food(leftRay5);
    if (eaten.size != 0) {
        foods.erase(std::remove_if(foods.begin(), foods.end(), [&](const Food& p) { return p.x == eaten.x && p.y == eaten.y && p.size == eaten.size; }), foods.end());
    }

    topRay1 = drawLineFunc(topRay1, 2, offsetRay1, 0, rayLength);
    topRay2 = drawLineFunc(topRay2, 2, -offsetRay1, 0, rayLength);
    topRay3 = drawLineFunc(topRay3, 2, offsetRay2, 0, rayLength);
    topRay4 = drawLineFunc(topRay4, 2, -offsetRay2, 0, rayLength);
    topRay5 = drawLineFunc(topRay5, 2, 0, 0, rayLength_Food);

    Ray topRays[] = { topRay1, topRay2, topRay3, topRay4, topRay5 };
    if (detectCollisionForMultipleLines(topRays))
        canMoveDirection[2] = false;
    else
        canMoveDirection[2] = true;

    if (detectCollisionForMultipleLines_Pacman_Ghost(topRay5))
        pacman_dead_state();

    eaten = detectCollisionForMultipleLines_Food(topRay5);
    if (eaten.size != 0) {
        foods.erase(std::remove_if(foods.begin(), foods.end(), [&](const Food& p) { return p.x == eaten.x && p.y == eaten.y && p.size == eaten.size; }), foods.end());
    }

    bottomRay1 = drawLineFunc(bottomRay1, 3, offsetRay1, 0, rayLength);
    bottomRay2 = drawLineFunc(bottomRay2, 3, -offsetRay1, 0, rayLength);
    bottomRay3 = drawLineFunc(bottomRay3, 3, offsetRay2, 0, rayLength);
    bottomRay4 = drawLineFunc(bottomRay4, 3, -offsetRay2, 0, rayLength);
    bottomRay5 = drawLineFunc(bottomRay5, 3, 0, 0, rayLength_Food);

    Ray bottomRays[] = { bottomRay1, bottomRay2, bottomRay3, bottomRay4, bottomRay5 };
    if (detectCollisionForMultipleLines(bottomRays))
        canMoveDirection[3] = false;
    else
        canMoveDirection[3] = true;

    if (detectCollisionForMultipleLines_Pacman_Ghost(bottomRay5))
        pacman_dead_state();

    eaten = detectCollisionForMultipleLines_Food(bottomRay5);
    if (eaten.size != 0) {
        foods.erase(std::remove_if(foods.begin(), foods.end(), [&](const Food& p) { return p.x == eaten.x && p.y == eaten.y && p.size == eaten.size; }), foods.end());
    }
}


void display() {

    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    drawMap02();
    if (displayPacman) {
        drawPacman();
        coyoteTimeFunc();

        updatePacmanPos();
        drawRays();
    }
    drawFood();

    orangeGhost = drawGhost(orangeGhost);
    cyanGhost = drawGhost(cyanGhost);
    pinkGhost = drawGhost(pinkGhost);
    redGhost = drawGhost(redGhost);

    pacman_win_state();



    glFlush();

    if(awake==0)
        printf("Lives Left = %d\n", pacman_lives);

    awake = 1;
}

void reshape(int width, int height)
{
    // Set the viewport to cover the entire window
    glViewport(0, 0, width, height);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    // Set the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(400, 150);
    glutCreateWindow("Pac-Man Game");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutSpecialFunc(arrowFunc);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}



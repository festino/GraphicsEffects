﻿
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <chrono>
#include <numeric>
#include <vector>
#include "Camera.h"
#include "Model.h"
#include "ModelCube.h"
#include "Omnilight.h"
#include "Skybox.h"

using namespace std;

int win_width = 1280;
int win_height = 720;
GLuint program;

bool paused = false;
float sensitivity_hor = 0.0012; // TODO: make pixel independent
float sensitivity_vert = 0.0015;
Camera cam = Camera(0, 0, glm::vec3(), 0, 0);

constexpr float MICROSEC = 1.0f / 1000000;
constexpr int TIMES_COUNT = 10;
float full_time = 0;
vector<float> times(TIMES_COUNT, 0.0f);
int times_index = 0;
chrono::steady_clock::time_point prevFrame;
bool initFrame = true;

GLuint mvpLoc, cameraLoc;

GLuint lightsLoc;
//vector<Omnilight> lights = vector<Omnilight>();
Omnilight lights[2];

Skybox skybox;
vector<Model*> models = vector<Model*>();

bool holdW = false, holdA = false, holdS = false, holdD = false;
bool holdSpace = false, holdShift = false;
glm::vec3 speed;


void makeCube(float size = 1.0f)
{
    Texture* texture = new Texture(program, "prev.png", "normalmap.bmp");
    Model* model = makeStaticCube(1, {0,0,0}, glm::identity<glm::mat4>(), texture);
    models.push_back(model);
}

void keyState(unsigned char key, int x, int y, bool down)
{
    key = tolower(key);
    if (key == 'w')
        holdW = down;
    if (key == 's')
        holdS = down;
    if (key == 'a')
        holdA = down;
    if (key == 'd')
        holdD = down;
    if (key == ' ')
        holdSpace = down;
    if (key == 112)
        holdShift = !holdShift;
    //holdShift = glutGetModifiers() & GLUT_ACTIVE_SHIFT;
}
void setPaused(bool val)
{
    paused = val;
    if (paused) {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    } else {
        glutSetCursor(GLUT_CURSOR_NONE);
    }
}
void keyDown(unsigned char key, int x, int y)
{
    if (key == 27)
        setPaused(!paused);
    keyState(key, x, y, true);
}
void keySpecial(int key, int x, int y)
{
    if (key == 27)
        setPaused(!paused);
    keyState(key, x, y, true);
}
void keyUp(unsigned char key, int x, int y)
{
    keyState(key, x, y, false);
}

void mouseMove(int mx, int my) {
    if (!paused) {
        int dx = mx - win_width / 2;
        int dy = my - win_height / 2;
        glutWarpPointer(win_width / 2, win_height / 2);
        cam.addAngle(dx * sensitivity_hor, dy * sensitivity_vert);
    }
}

void moveCamera(float time)
{
    speed = glm::vec3();
    if (holdW)
        speed.z -= 1;
    if (holdS)
        speed.z += 1;
    if (holdA)
        speed.x -= 1;
    if (holdD)
        speed.x += 1;
    if (holdSpace)
        speed.y += 1;
    if (holdShift)
        speed.y -= 1;

    speed *= time;
    cam.moveHor(speed.z, speed.x, speed.y);
}

void idle()
{
    chrono::steady_clock::time_point curFrame = chrono::steady_clock::now();
    if (initFrame) {
        initFrame = false;
    } else {
        double delta = double(chrono::duration_cast<chrono::microseconds>(curFrame - prevFrame).count());
        delta *= MICROSEC;
        full_time += delta;
        if (times.size() >= TIMES_COUNT)
            times[times_index] = delta;
        else
            times.push_back(delta);
        if (++times_index >= TIMES_COUNT)
            times_index = 0;

        double avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        moveCamera(avg);
        // animations
        float angle = full_time / 20;
        lights[1].light_pos = glm::vec4(2 * glm::sin(angle), 1, 2 * glm::cos(angle), 0);
    }
    prevFrame = curFrame;
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.updateMvp();
    glUseProgram(program);
    glm::vec3 pos = cam.getPosition();
    glUniform3fv(cameraLoc, 1, &pos[0]);
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, cam.getMvpLoc());
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsLoc);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(lights), &lights[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    for (Model *model : models)
    {
        model->draw();
    }

    skybox.draw(&cam);

    glFlush();
    glutSwapBuffers();
}
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    cam.updateProjSize(win_width, win_height);
    glViewport(0, 0, win_width, win_height);
    times.clear();
    times_index = 0;
    initFrame = true;
}

void initCamera()
{
    cam = Camera(0, 0, glm::vec3(0, 0, 2), win_width, win_height);
}

void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    skybox = Skybox();
    skybox.init();

    FileUtils::loadShaders(program, "scene_1.vert", "scene_1.frag");

    glUseProgram(program);
    mvpLoc = glGetUniformLocation(program, "mvp");
    cameraLoc = glGetUniformLocation(program, "camera");

    makeCube();
    glGenBuffers(1, &lightsLoc);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsLoc);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lights), &lights[0], GL_STATIC_READ);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightsLoc);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    lights[0] = { {1, 0.5, 1, 0}, {0.9, 1, 0.9, 0}, {0.9, 1, 0.9, 0}, 0.7, 0.2 };
    lights[1] = { {0, 1, 2, 0}, {0.9, 0, 0, 0}, {0.9, 0, 0, 0}, 0.7, 0.2 };

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE
}

int start_scene_1(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("OpenGL");

    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        printf("ERROR: %s", glewGetErrorString(GlewInitResult));
        exit(EXIT_FAILURE);
    }

    initGL();
    initCamera();
    setPaused(false);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutSpecialFunc(keySpecial);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutPassiveMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}
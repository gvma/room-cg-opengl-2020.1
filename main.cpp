#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./libs/stb_image.h"
#include "./libs/camera.h"
#include "./libs/build.h"
#include "./libs/texture.h"
#include "./libs/animation.h"
#include "./libs/solids.h"
#include "./libs/material.h"

#include "./libs/obj_loader.h"

static int windows_w = 1080, windows_h = 720;

Camera camera;

Texture floorTexture;
Texture wallTexture;
Texture windowTexture;
Texture doorTexture;
Texture doorFrameTexture;
Texture ricardoSemVergonhoTexture;
Texture woodTexture;
Texture whiteTexture;
Texture darkWoodTexture;
Texture someTexture;
Texture screenTexture;
Texture sheetTexture;
Texture vanGoghTexture;

mat::Material ambient(0.7, 0.7, 0.7, 0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 100);
mat::Material silver(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 51.2);

// Light Props

static GLfloat light0_ambient[]	    = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light0_diffuse[]	    = { 0.8, 0.8, 0.8, 1.0 };
static GLfloat light0_specular[]	= { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light0_position[]	= { 0.0, 0.0, 0.0, 1.0 };

static bool light1_enabled          = true;
static GLfloat light1_offset[]	    = { 19.4, 4, -19.2 };
static GLfloat light1_ambient[]	    = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light1_diffuse[]	    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_specular[]	= { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_position[]	= { 0.0, 3.0, 0.0, 1.0 };
static GLfloat light1_direction[]	= { 0.0, -1.0, 0.0 };
static GLfloat light1_angle	        = 20.0;
static GLfloat light1_exponent	    = 2.0;

objl::Loader lampLoader;
objl::MeshInfo lampMesh;

objl::Loader chairLoader;
objl::MeshInfo chairMesh;
objl::MeshInfo chairSeatMesh;

objl::Loader fanLoader;
objl::MeshInfo fanMesh;
objl::MeshInfo fanPropellerMesh;

objl::Loader monitorLoader;
objl::MeshInfo monitorMesh;
objl::MeshInfo monitorScreenMesh;
objl::MeshInfo monitorWebcamMesh;

objl::Loader keyboardLoader;
objl::MeshInfo keyboardMesh;
objl::MeshInfo keyboardLettersMesh;

objl::Loader bedLoader;
objl::MeshInfo bedMesh;
objl::MeshInfo bedMattressMesh;
objl::MeshInfo bedPillowMesh;

objl::Loader mouseLoader;
objl::MeshInfo mouseMesh;
objl::MeshInfo mousepadMesh;

objl::Loader bedsideLoader;
objl::MeshInfo bedsideMesh;

objl::Loader cactusLoader;
objl::MeshInfo cactusMesh;
objl::MeshInfo cactusPlantMesh;

objl::Loader tableLoader;
objl::MeshInfo tableMesh;

objl::Loader wardrobeLoader;
objl::MeshInfo wardrobeMesh;
objl::MeshInfo wardrobeAMesh;

static GLfloat lamp_offset[] = { 19, 3.3, -20 };

AnimationState doorAnimation = IDLE;
GLfloat doorAngle = 0.f;

AnimationState windowAnimation = IDLE;
GLfloat windowTranslation = 1.f;

AnimationState fanAnimation = FORWARDS;
const GLfloat FAN_LOW_SPEED = 8.0f;
const GLfloat FAN_MAX_SPEED = 16.0f;
GLfloat fanRotation = 1.f;
GLfloat fanRotationSpeed = FAN_LOW_SPEED;

using namespace std;

void loadTexture(const char* fileName, Texture* texture) 
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int nrChannels;

    texture->data = stbi_load(fileName, &texture->width, &texture->height, &nrChannels, 0);

    if (!texture->data) {
        cout << "Failed to load texture: " << fileName << endl;
        exit(1);
    } 
}

void setupTexture(Texture* texture)
{
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                    GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, 
                    texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                    texture->data);                  
}

void initTextures()
{
    loadTexture("./res/textures/wall.png", &wallTexture);
    setupTexture(&wallTexture); 

    loadTexture("./res/textures/blacktile.png", &floorTexture);
    setupTexture(&floorTexture);

    loadTexture("./res/textures/window.png", &windowTexture);
    setupTexture(&windowTexture);

    loadTexture("./res/textures/door2.png", &doorTexture);
    setupTexture(&doorTexture);

    loadTexture("./res/textures/door-frame.png", &doorFrameTexture);
    setupTexture(&doorFrameTexture);

    loadTexture("./res/textures/starry-night.png", &vanGoghTexture);
    setupTexture(&vanGoghTexture);

    loadTexture("./res/textures/ricardoComCaraDeSemVergonho.png", &ricardoSemVergonhoTexture);
    setupTexture(&ricardoSemVergonhoTexture);

    loadTexture("./objs/bed/wood.png", &woodTexture); 
    setupTexture(&woodTexture);

    loadTexture("./res/textures/grey-texture.png", &whiteTexture);
    setupTexture(&whiteTexture);

    loadTexture("./objs/bed/darkwood.png", &someTexture);
    setupTexture(&someTexture);

    loadTexture("./objs/bed/sheet copy.png", &sheetTexture);
    setupTexture(&sheetTexture);
}

void init(void)
{   
   glClearColor (0.31, 0.61, 0.85, 1.0);

   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Anti aliasing smoothing props 
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_BLEND);

   // Lighting and Shade setup 
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL); 
   glShadeModel(GL_SMOOTH);

   glEnable(GL_LIGHTING); 
    
   // Light 0 property vectors

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glEnable(GL_LIGHT0);

   // End Light 0
   // Light 1 property vectors 

   glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
   glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
   
   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);

   glEnable(GL_LIGHT1);

   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   initTextures();

   lampMesh = lampLoader.LoadedMeshes[0].setup();

   chairMesh = chairLoader.LoadedMeshes[0].setup();
   chairSeatMesh = chairLoader.LoadedMeshes[1].setup();

   fanMesh = fanLoader.LoadedMeshes[0].setup();
   fanPropellerMesh = fanLoader.LoadedMeshes[1].setup();

   monitorMesh = monitorLoader.LoadedMeshes[0].setup();
   monitorScreenMesh = monitorLoader.LoadedMeshes[1].setup();
   monitorWebcamMesh = monitorLoader.LoadedMeshes[2].setup();

   keyboardLettersMesh = keyboardLoader.LoadedMeshes[0].setup();
   keyboardMesh = keyboardLoader.LoadedMeshes[1].setup();

   bedMesh = bedLoader.LoadedMeshes[0].setup();
   bedMattressMesh = bedLoader.LoadedMeshes[1].setup();
   bedPillowMesh = bedLoader.LoadedMeshes[2].setup(false);

   mouseMesh = mouseLoader.LoadedMeshes[0].setup();
   mousepadMesh = mouseLoader.LoadedMeshes[1].setup();

   tableMesh = tableLoader.LoadedMeshes[0].setup();

   wardrobeMesh = wardrobeLoader.LoadedMeshes[0].setup();
   wardrobeAMesh = wardrobeLoader.LoadedMeshes[1].setup();

   cactusMesh = cactusLoader.LoadedMeshes[0].setup();
   cactusPlantMesh = cactusLoader.LoadedMeshes[1].setup();
}

void setupLighting()
{
    glEnable(GL_LIGHTING); 

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glTranslatef(roomWidth / 2, roomHeight - 0.5, -(roomWidth / 2));
        GLfloat light0_position[]	= {0, 0, 0, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();

    if (light1_enabled) {
        glEnable(GL_LIGHT1);
        glPushMatrix();
            glTranslatef(light1_offset[0], light1_offset[1], light1_offset[2]);

            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);
        glPopMatrix();
    }
}


void setupCamera() 
{
    glViewport(0, 0, (GLsizei) windows_w, (GLsizei) windows_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) windows_w/(GLfloat) windows_h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::mat3 lookUpMatrix = camera.getLookAtMatrix();

    gluLookAt(lookUpMatrix[0][0], lookUpMatrix[0][1], lookUpMatrix[0][2],
              lookUpMatrix[1][0], lookUpMatrix[1][1], lookUpMatrix[1][2],
              lookUpMatrix[2][0], lookUpMatrix[2][1], lookUpMatrix[2][2]);
}

void checkFanRotation()
{
    if (fanAnimation == FORWARDS || fanAnimation ==  BACKWARDS)
    {
        fanRotation += fanAnimation == FORWARDS ? fanRotationSpeed : -fanRotationSpeed;
        if (fanRotationSpeed > 360) fanRotationSpeed = 0;
        else if (fanRotationSpeed < 0) fanRotationSpeed = 360;
    }
}

void checkDoorAngle()
{
    if (doorAnimation == FORWARDS || doorAnimation == BACKWARDS)
    {
        doorAngle += doorAnimation == FORWARDS ? 1 : -1;
        if (doorAngle == 90.0f || doorAngle == 0.0f) doorAnimation = IDLE;
    }
}

void checkWindowTranslation()
{
    if (windowAnimation == FORWARDS || windowAnimation == BACKWARDS)
    {
        windowTranslation += windowAnimation == FORWARDS ? 0.02 : -0.02;
        if (windowTranslation >= 1.0f || windowTranslation <= 0.0f) windowAnimation = IDLE;
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0.31, 0.61, 0.85, 1.0);

    drawCrosshair(windows_w, windows_h);

    setupCamera();
    setupLighting();
    
    ambient.active();
    
    // Table
    glPushMatrix();
        glTranslatef(19.2f, 7.3f, -7.2f);
        glRotatef(180, 0, 1, 0);
        glScalef(0.8, 0.8, 0.8);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, whiteTexture.id);

        tableMesh.material.active();
        tableMesh.material.dye();

        glVertexPointer(3, GL_FLOAT, 0, &tableMesh.vertices_pointers[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &bedsideMesh.vertices_tex_coords[0]);
        glNormalPointer(GL_FLOAT, 0, &tableMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, tableMesh.indices_pointers.size(), GL_UNSIGNED_INT, &tableMesh.indices_pointers[0]);
        
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Bed
    glPushMatrix();
        glTranslatef(7.85, -2, -17);
        glRotatef(270, 0, 1, 0);
        glScalef(0.2, 0.2, 0.2);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, woodTexture.id);

        bedMesh.material.active();
        bedMesh.material.dye();

        glVertexPointer(3, GL_FLOAT, 0, &bedMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &bedMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, bedMesh.indices_pointers.size(), GL_UNSIGNED_INT, &bedMesh.indices_pointers[0]);

        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, sheetTexture.id);


            glVertexPointer(3, GL_FLOAT, 0, &bedMattressMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &bedMattressMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, bedMattressMesh.indices_pointers.size(), GL_UNSIGNED_INT, &bedMattressMesh.indices_pointers[0]);

            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glTranslatef(10, 0, -0.4);
        glRotatef(270, 1, 0, 0);
        glRotatef(180, 0, 0, 1);
        glScalef(0.08, 0.08, 0.08);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, woodTexture.id);

        glVertexPointer(3, GL_FLOAT, 0, &wardrobeMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &wardrobeMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, wardrobeMesh.indices_pointers.size(), GL_UNSIGNED_INT, &wardrobeMesh.indices_pointers[0]);
        
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            glBindTexture(GL_TEXTURE_2D, someTexture.id);

            wardrobeAMesh.material.active();
            wardrobeAMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &wardrobeAMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &wardrobeAMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, wardrobeAMesh.indices_pointers.size(), GL_UNSIGNED_INT, &wardrobeAMesh.indices_pointers[0]);

            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();

    // Mouse and Mousepad
    glPushMatrix();
        glTranslatef(18, 3.5, -13.7);
        glRotatef(-90, 1, 0, 0);
        glRotatef(-90, 0, 0, 1);
        glScalef(0.01, 0.01, 0.01);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        mouseMesh.material.active();
        mouseMesh.material.dye();

        glVertexPointer(3, GL_FLOAT, 0, &mouseMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &mouseMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, mouseMesh.indices_pointers.size(), GL_UNSIGNED_INT, &mouseMesh.indices_pointers[0]);

        glPushMatrix();
            mousepadMesh.material.active();
            mousepadMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &mousepadMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &mousepadMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, mousepadMesh.indices_pointers.size(), GL_UNSIGNED_INT, &mousepadMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

    // Keyboard
    glPushMatrix();
        glTranslatef(18, 3.5, -15);
        glRotatef(-90, 0, 1, 0);
        glScalef(1, 1, 1);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        keyboardMesh.material.active();
        keyboardMesh.material.dye();

        glColor3f(1, 1, 1);
        glVertexPointer(3, GL_FLOAT, 0, &keyboardMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &keyboardMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, keyboardMesh.indices_pointers.size(), GL_UNSIGNED_INT, &keyboardMesh.indices_pointers[0]);

        glPushMatrix();
            keyboardLettersMesh.material.active();
            keyboardLettersMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &keyboardLettersMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &keyboardLettersMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, keyboardLettersMesh.indices_pointers.size(), GL_UNSIGNED_INT, &keyboardLettersMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

    // Monitor
    glPushMatrix();
        glTranslatef(19.4, 3.5, -13);
        glRotatef(-90, 1, 0, 0);
        glRotatef(-90, 0, 0, 1);
        glScalef(0.1, 0.1, 0.1);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &monitorMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &monitorMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, monitorMesh.indices_pointers.size(), GL_UNSIGNED_INT, &monitorMesh.indices_pointers[0]);

        glPushMatrix();
            monitorScreenMesh.material.active();
            monitorScreenMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &monitorScreenMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &monitorScreenMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, monitorScreenMesh.indices_pointers.size(), GL_UNSIGNED_INT, &monitorScreenMesh.indices_pointers[0]);
        glPopMatrix();

        glPushMatrix();
            monitorWebcamMesh.material.active();
            monitorWebcamMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &monitorWebcamMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &monitorWebcamMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, monitorWebcamMesh.indices_pointers.size(), GL_UNSIGNED_INT, &monitorWebcamMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

    // Fan
    glPushMatrix();
        GLfloat fanTranslate[3] = {19.2, 5.3, -7.2};
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glPushMatrix();
            glTranslatef(fanTranslate[0], fanTranslate[1], fanTranslate[2]);
            glRotatef(-135, 0, 1, 0);
            glRotatef(-90, 1, 0, 0);
            glScalef(0.05, 0.05, 0.05);

            fanMesh.material.active();
            fanMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &fanMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &fanMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, fanMesh.indices_pointers.size(), GL_UNSIGNED_INT, &fanMesh.indices_pointers[0]);
        glPopMatrix();

        checkFanRotation();
        glTranslatef(fanTranslate[0], fanTranslate[1], fanTranslate[2]);
        glScalef(0.05, 0.05, 0.05);
        glRotatef(270, 0, 0, 1);
        glRotatef(45, 1, 0, 0);
        glRotatef(fanRotation, 0, 1, 0);

        glPushMatrix();
            fanPropellerMesh.material.active();
            fanPropellerMesh.material.dye();
        
            glVertexPointer(3, GL_FLOAT, 0, &fanPropellerMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &fanPropellerMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, fanPropellerMesh.indices_pointers.size(), GL_UNSIGNED_INT, &fanPropellerMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

    // Board
    glPushMatrix();
        glRotatef(270, 0, 1, 0);
        glTranslatef(-17.81, 6, -0.1);
        buildBoard(&ricardoSemVergonhoTexture, rgb(154, 149, 143), {1.5, 1.8618, 0.1});
    glPopMatrix();

    glPushMatrix();
        glRotatef(270, 0, 1, 0);
        glTranslatef(-9, 5.2, -0.1);
        buildBoard(&vanGoghTexture, rgb(154, 149, 143), {4.5, 3, 0.1});
    glPopMatrix();

    glPushMatrix();
        glTranslatef(lamp_offset[0], lamp_offset[1], lamp_offset[2]);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.05, 0.05, 0.05);

        glColor3f(0.2, 0.2, 0.2);
        lampMesh.material.active();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &lampMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &lampMesh.vertices_normals[0]);

        glDrawElements(GL_TRIANGLES, lampMesh.indices_pointers.size(), GL_UNSIGNED_INT, &lampMesh.indices_pointers[0]);
    glPopMatrix();

    // Chair
    glPushMatrix();
        glTranslatef(16, 0, -15);
        glRotatef(90, 0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.15, 0.15, 0.15);

        chairMesh.material.active();
        chairMesh.material.dye();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &chairMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &chairMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, chairMesh.indices_pointers.size(), GL_UNSIGNED_INT, &chairMesh.indices_pointers[0]);

        glPushMatrix();
            chairSeatMesh.material.active();
            chairSeatMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &chairSeatMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &chairSeatMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, chairSeatMesh.indices_pointers.size(), GL_UNSIGNED_INT, &chairSeatMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix(); 
    
    glPushMatrix();
        glEnable(GL_NORMALIZE);
        glDisable( GL_CULL_FACE );
        glShadeModel(GL_SMOOTH);

        glTranslatef(1, 0, -roomWidth + 1);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.15, 0.15, 0.15);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();

    // Cactus
    glPushMatrix();
        glTranslatef(19.2, 3.5, -10);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.01, 0.01, 0.01);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        cactusMesh.material.active();
        cactusMesh.material.dye();

        glVertexPointer(3, GL_FLOAT, 0, &cactusMesh.vertices_pointers[0]);
        glNormalPointer(GL_FLOAT, 0, &cactusMesh.vertices_normals[0]);
        glDrawElements(GL_TRIANGLES, cactusMesh.indices_pointers.size(), GL_UNSIGNED_INT, &cactusMesh.indices_pointers[0]);

        glPushMatrix();
            cactusPlantMesh.material.active();
            cactusPlantMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &cactusPlantMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &cactusPlantMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, cactusPlantMesh.indices_pointers.size(), GL_UNSIGNED_INT, &cactusPlantMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

    // textura aqui
    buildFrontWall(&wallTexture);
    buildBackWall(&wallTexture);
    buildLeftWall(&wallTexture);
    buildRightWall(&wallTexture);
    buildRoof(&wallTexture);
    buildFloor(&floorTexture);

    buildDoorFrame(&wallTexture);

    checkDoorAngle();
    buildDoor(&doorTexture, doorAngle);

    checkWindowTranslation();
    glPushMatrix();
        glTranslatef(windowPosition.x + 0.2, windowPosition.y + 0.2, -(roomWidth + 0.4));
        buildWindowGlass();
    glPopMatrix();

    glPushMatrix();
        GLfloat windowTranslationOffset = ((windowDimention.x - (2 * windowDimention.z)) / 2) * windowTranslation;
        glTranslatef((windowPosition.x) + windowTranslationOffset, windowPosition.y + 0.2, -(roomWidth + 0.6));
        buildWindowGlass();
    glPopMatrix();

    buildWindow(silver);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    windows_w = w;
    windows_h = h;

    glViewport(0, 0, (GLsizei) windows_w, (GLsizei) windows_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) windows_w/(GLfloat) windows_h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'w':
            camera.moveForward();
            break;
        case 's':
            camera.moveBackward();
            break;
        case 'a':
            camera.moveLeft();
            break;
        case 'd':
            camera.moveRight();
            break;
        case 'q':
            camera.moveUp();
            break;
        case 'e':
            camera.moveDown();
            break;
        case 'p':
            doorAnimation = doorAngle == 90.0f ? BACKWARDS : FORWARDS;
            break;
        case 'j':
            windowAnimation = windowTranslation == 1.0f ? BACKWARDS : FORWARDS;
            break;
        case 'l':
            light1_enabled = !light1_enabled;
            if (!light1_enabled) glDisable(GL_LIGHT1);
            break;
        case 'v':
            if (fanAnimation == IDLE) {
                fanRotationSpeed = FAN_LOW_SPEED;
                fanAnimation = FORWARDS;
            } else if (fanRotationSpeed == FAN_LOW_SPEED) {
                fanRotationSpeed = FAN_MAX_SPEED;
            } else if (fanRotationSpeed == FAN_MAX_SPEED) {
                fanAnimation = IDLE;
                fanRotationSpeed = FAN_LOW_SPEED;
            }
            break;
        default:
            break;
    }
}

void specialKeyInput(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_F1:
            glDisable(GL_LIGHT0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void motion(int x, int y)
{
    camera.mouseUpdate(glm::vec2(x, y));
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windows_w, windows_h);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Trabalho Final - CG");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyInput);
    glutMotionFunc(motion);
    glutTimerFunc(30, update, 0);

    lampLoader.LoadFile("./objs/lamp/lamp.obj");
    chairLoader.LoadFile("./objs/chair/chair.obj");
    fanLoader.LoadFile("./objs/fan/fan.obj");
    monitorLoader.LoadFile("./objs/monitor/monitor.obj");
    keyboardLoader.LoadFile("./objs/keyboard/keyboard.obj");
    bedLoader.LoadFile("./objs/bed/bed.obj");
    mouseLoader.LoadFile("./objs/mouse/mouse.obj");
    bedsideLoader.LoadFile("./objs/bedside/bedside.obj");
    cactusLoader.LoadFile("./objs/cactus/cactus.obj");
    tableLoader.LoadFile("./objs/table/table.obj");
    wardrobeLoader.LoadFile("./objs/wardrobe/wardrobe.obj");

    init();
    glutMainLoop();
    return 0; 
} 
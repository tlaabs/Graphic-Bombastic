//Graphic Term Project
//Title: Bombastic

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <glut.h>
#include <Windows.h>
#include <mmsystem.h>   
#include <Digitalv.h>
#include "Image.h"
#pragma comment(lib, "winmm.lib")

#define SOUND_FILE_NAME ".\\bgm.wav"

// ---------------------------------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------------------------------

#define TORSO_HEIGHT 5.0
#define TORSO_RADIUS 1.3

#define TYPE_BODY 0
#define TYPE_HEAD 1
#define TYPE_BIG_JOINTS 2
#define TYPE_LEFT_LOWER_ARM 3
#define TYPE_ELBOW_JOINTS 4
#define TYPE_LEFT_UPPER_ARM 5
#define TYPE_PALM 6

#define TYPE_RIGHT_LOWER_ARM 7
#define TYPE_RIGHT_UPPER_ARM 8

#define TYPE_LEG_KNEE_JOINTS 9
#define TYPE_UPPER_LEG 10
#define TYPE_LOWER_LEG 11

void timer(int t);
void square();
static int delay = 10;
float oldTimeSinceStart = 0;
int sync_flag = 0;
int sync_times = 0;
float deltaTime = 0;
float timess = 0;

//the array for texture
// back,floor,left,right
GLuint texture[4]; 

typedef struct _action
{
	char    type;                // 'R' or 'T' or 'N' or 'F'
	int     duration;            // how long?
	GLfloat param[3];            // parameters
} Action;

typedef struct _object
{
	GLfloat*        vertices;    // vertex array
	GLfloat*        colors;      // color array
	GLubyte*        indices;     // index
	int             nums;        // number of vertices
	GLfloat         theta[3];    // rotation parameter (x, y, z)
	GLfloat         trans[3];    // translation parameter

	Action*         action;
	int             action_idx;
	int             action_counter;

	int type;

	struct _object* next;
	struct _object* child;
} Object;

typedef struct _viewer
{
	GLfloat         eye[3];    // viewer position
	GLfloat         at[3];    // center
	GLfloat         up[3];    // up direction
} Viewer;

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------
GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mat_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mat_shininess = { 100.0 };

// actions
Action head_action[] = {
	//Ready 460
	{ 'N',460,0,0,0 },
	//Dance1 600
	{ 'N',600,0,0,0 },
	//Dance2 115
	{ 'R',10,0,40,0 },
	{ 'N',50,0,0,0 },
	{ 'R',10,0,-130,0 },
	{ 'N',45,0,0, },
	//Dance3 920
	{ 'R', 50, 0, 90 ,0 },
	{ 'N',770,0,0,0 },
	{ 'N',30,0,0,0 },
	{ 'R', 15, 30, 0 ,0 },
	{ 'N', 40, 0, 0 ,0 },
	{ 'R', 15, -70, 0 ,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R',1,45,0,0 },

	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },
	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },
	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },
	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },

	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },

	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },

	{ 'R',60,-45,0,0 },
	{ 'R',60,45,0,0 },

	{ 'N',5,0,0,0 },

	{ 'R',15,-45,0,0 },

	{ 'F',0,0,0,0 }
};

Action body_action[] = {
	//Ready 460
	{ 'N', 460,   0, 0,   0 },

	//Dance1 600
	{ 'R', 25, 0, 50, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },
	{ 'R', 25, 0, -50, 0 },

	//Dance2 115
	{ 'R', 15, 0, 90 ,0 },
	{ 'N', 65, 0, 0, 0 },
	{ 'N',35,0,0,0 },//

					 //Dance3 920
	{ 'N',180,0,0,0 },
	{ 'R', 30, 0, -180 ,0 },
	{ 'N',180,0,0,0 },
	{ 'R', 30, 0, 180 ,0 },
	{ 'N',180,0,0,0 },
	{ 'R', 30, 0, -180 ,0 },
	{ 'N',180,0,0,0 },
	{ 'R', 10, 0, 90 ,0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R',20,0,-60,0 },
	{ 'N',196,0,0,0 },
	{ 'R',200,0,120,0 },
	{ 'N',200,0,0,0 },
	{ 'R',190,0,-120,0 },
	{ 'R',20,0,60,0 },

	{ 'F',  0,    0,   0,   0 }
};

Action left_shoulder_joints_action[] = {
	//Ready 460
	{ 'R', 1,    0, 0,   90 },
	{ 'N', 320,   0, 0,   0 },
	{ 'R', 20,   -180, 0,  0 },
	{ 'N', 119,   0, 0,   0 },

	//Dance1 600
	{ 'R', 30,    0, 0,   -30 },
	{ 'N', 570,   0, 0,   0 },

	//Dance2 115
	{ 'R', 10, 200, 0, 0 },
	{ 'N',50,0,0,0 },
	{ 'R', 3, -20, 0, 0 },
	{ 'R', 3,0,0,-60 },
	{ 'N', 29,   0, 0,   0 },
	{ 'R', 20,    -60, 0,   90 },

	//Dance3 920
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//
	{ 'N', 30, 0, 0 ,0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//
	{ 'N', 30, 0, 0 ,0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//4

	{ 'N', 30, 0, 0 ,0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//pose
	{ 'R', 10,    50, 0,  -30 },
	{ 'N',60,0,0,0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R', 1,    0, 0,  30 },
	{ 'R', 27,    60, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����


	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����


	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����

							   //

	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����


	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����


	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    80, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -80, 0, 0 }, //�� �ڷ� ����

							   //END
	{ 'R', 15,    -60, 0, 0 }, //�� �ڷ� ����


	{ 'F',  0,    0,   0,   0 }
};

Action left_elbow_joints_action[] = {
	//Ready 460
	{ 'N', 460,   0, 0,   0 },

	//Dance1 600
	{ 'R',  10,    0,   0,   -40 },
	{ 'R', 590, 7200, 0, 0 },

	//Dance2 115
	{ 'R',10,-30,0,130 },
	{ 'N',50,0,0,0 },
	{ 'R',5,30,0,-90 },
	{ 'N', 50,   0, 0,   0 },

	//Dance3 920
	{ 'R', 8,    0, 0,   90 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },

	{ 'N', 52, 0, 0 ,0 },

	{ 'R', 30,    -720, 0,  0 },

	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },

	//
	{ 'R', 30,    810, 0,  0 },
	{ 'N', 52, 0, 0 ,0 },
	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },
	{ 'N',52,0,0,0 },

	//4
	{ 'R', 30,    -720, 0,  0 },

	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },
	{ 'N',52,0,0,0 },

	//pose
	{ 'R', 10,    -20, 0,  10 },
	{ 'N', 60,   0, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4

	{ 'R', 27,    -60, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

								//

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 120 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -120 }, //�� �ڷ� ����

	{ 'F',  0,    0,   0,   0 },
};

Action right_shoulder_joints_action[] = {
	//Ready 460
	{ 'R', 1,    0, 0,   -90 },
	{ 'N', 459,   0, 0,   0 },

	//Dance1 600
	{ 'R', 30,   0, 0,   50 },
	{ 'N', 570,   0, 0,   0 },

	//Dance2 115
	{ 'R', 30, 0,0,-20 },
	{ 'R',10,20,0,0 },
	{ 'N', 55,   0, 0,   0 },
	{ 'R', 20,    -90, 0,   -30 },
	//Dance3 920

	{ 'R', 14,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//
	{ 'N', 30, 0, 0 ,0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//
	{ 'N', 30, 0, 0 ,0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//4

	{ 'N', 30, 0, 0 ,0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },
	{ 'R', 15,    40, 0, 0 },
	{ 'R', 15,    -40, 0, 0 },

	//pose
	{ 'R', 10,    50, 0,  30 },
	{ 'N', 60,   0, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R', 1,    0, 0,  -30 },
	{ 'R', 27,    -60, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

							   //
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    -100, 0, 0 }, //�� �ڷ� ����
	{ 'R', 27,    100, 0, 0 }, //�� �ڷ� ����

							   //END
	{ 'R', 15,    60, 0, 0 }, //�� �ڷ� ����

	{ 'F',  0,    0,   0,   0 }
};

Action right_elbow_joints_action[] = {
	//Ready 460
	{ 'N', 460,   0, 0,   0 },

	//Dance1 600
	{ 'R', 30,   0, 0,   -90 },
	{ 'N', 570,   0, 0,   0 },

	//Dance2 115
	{ 'R',10,-30,0,0 },
	{ 'N', 105,   0, 0,   0 },

	//Dance3 920
	{ 'R', 8,    -60, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },

	{ 'R', 30,    810, 0,  0 },
	{ 'N', 52, 0, 0 ,0 },

	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    -90, 0,   0 },

	//
	{ 'R', 30,    -720, 0,  0 },
	{ 'R', 52,    0, 0,  0 },

	{ 'R', 8,    +90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    +90, 0,   0 },

	//4
	{ 'R', 30,    720, 0,  0 },
	{ 'N', 52, 0, 0 ,0 },

	{ 'R', 8,    -90, 0,   0 },
	{ 'N',52,0,0,0 },
	{ 'R', 8,    90, 0,   0 },

	//pose
	{ 'R', 10,    -20, 0,  -10 },
	{ 'N', 60,   0, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R', 27,    -60, 0, 10 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

							   //


	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����


	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, 90 }, //�� �ڷ� ����
	{ 'R', 27,    0, 0, -90 }, //�� �ڷ� ����

	{ 'F',  0,    0,   0,   0 }
};

Action left_leg_joints_action[] = {
	//Ready 460
	{ 'R', 30,    0, 0,   -10 },
	{ 'N', 430, 0, 0, 0 },

	//Dance1 600
	{ 'N', 600,0,0,0 },

	//Dance2 115
	{ 'N', 95,   0, 0,   0 },
	{ 'R', 20,    -30, 0,   0 },
	//Dance3 920
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60 - 3
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },


	//

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },


	//

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'N', 70,   0, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R', 1,    30, 0,   0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	//

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },


	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },

	{ 'F',  0,    0,   0,   0 }
};

Action left_knee_joints_action[] = {
	//Ready 460
	{ 'N', 460,   0, 0,   0 },

	//Dance1 600
	{ 'R', 30,  40, 0,   0 },
	{ 'N', 570, 0, 0, 0 },
	//Dance2 115
	{ 'R',30,-40,0,0 },
	{ 'N', 85,   0, 0,   0 },
	//Dance3 920
	{ 'R', 30,    30, 0,   0 },
	{ 'N', 790,0,0,0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

						 //

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

						 //end

	{ 'R', 1,    -30, 0,   0 },
	{ 'F',  0,    0,   0,   0 }
};


Action right_leg_joints_action[] = {
	//Ready 460
	{ 'R',30,    0,   0,   10 },
	{ 'N', 430,   0, 0,   0 },

	//Dance1 600
	{ 'R',30,    -30,   0,   0 },
	{ 'N', 570,   0, 0,   0 },

	//Dance2 115
	{ 'R',30,30,0,0 },
	{ 'N', 65,   0, 0,   0 },
	{ 'R', 20,    -25, 0,   0 },
	//Dance3 920
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60 -3
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },



	//

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	//60 -3
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },


	//

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },
	{ 'R', 15,    15, 0,   0 },
	{ 'R', 15,    -15, 0,   0 },

	{ 'N', 70,   0, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R', 1,    25, 0,   0 },
	{ 'N',27,0,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	//

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },

	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	{ 'R',27,60,0,0 },
	{ 'R',27,-60,0,0 },
	//END
	{ 'R',27,60,0,0 },
	{ 'F',  0,    0,   0,   0 }

};

Action right_knee_joints_action[] = {
	//Ready
	{ 'N', 460,   0, 0,   0 },

	//Dance1 600
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },
	{ 'R', 30,    40,0,   0 },
	{ 'R', 30,    -40,0,   0 },

	//Dance2 115
	{ 'N', 115,   0, 0,   0 },

	//Dance3 920
	{ 'R', 30,    30, 0,   0 },
	{ 'N', 790,    30, 0,   0 },
	{ 'N',100,0,0,0 },
	{ 'N', 30, 0, 0 ,0 },
	//Dance4
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

						 //

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

						 //

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����
	{ 'R',13,100,0,0 }, //����
	{ 'R',14,-100,0,0 }, //����

						 //END
	{ 'R', 1,    -30, 0,   0 },
	{ 'F',  0,    0,   0,   0 }


};

// ---------------------------------------------------------------------------------------------
// Objects
// ---------------------------------------------------------------------------------------------

Object body, head;
Object left_shoulder_joints, left_lower_arm, left_elbow_joints, left_upper_arm, left_palm;
Object right_shoulder_joints, right_lower_arm, right_elbow_joints, right_upper_arm, right_palm;
Object left_leg_joints, left_upper_leg, left_knee_joints, left_lower_leg;
Object right_leg_joints, right_upper_leg, right_knee_joints, right_lower_leg;

Viewer v;
GLUquadricObj *t;
int    play = 0;

//audio

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------
//void timer(int t)
//{
//   float timess = glutGet(GLUT_ELAPSED_TIME);
//   //printf("timess : %f\n", timess);
//   float deltaTime = timess - oldTimeSinceStart;
//   oldTimeSinceStart = timess;
//   printf("deletaTime : %f\n", deltaTime);
//   glutPostRedisplay(); // call display function which is set by glutDisplayFunc.
//   glutTimerFunc(delay, timer, t);
//}

void init()
{
	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position1[] = { 10.0, 10.0, 10.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular1);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//background-black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glOrtho(-3.0, 3.0, -3.0, 3.0, -10.0, 10.0);
	
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glEnable(GL_COLOR_MATERIAL);
}

void init_left_arm() {
	left_shoulder_joints.theta[0] = left_shoulder_joints.theta[1] = left_shoulder_joints.theta[2] = 0;
	left_shoulder_joints.trans[0] = -0.4;
	left_shoulder_joints.trans[1] = 1.20;
	left_shoulder_joints.trans[2] = 0;
	left_shoulder_joints.type = TYPE_BIG_JOINTS;

	left_shoulder_joints.action = &left_shoulder_joints_action[0];
	left_shoulder_joints.action_idx = 0;
	left_shoulder_joints.action_counter = 0;

	left_shoulder_joints.next = &right_shoulder_joints;
	left_shoulder_joints.child = &left_lower_arm;

	//left_lower_arm
	left_lower_arm.nums = 24;
	left_lower_arm.theta[0] = left_lower_arm.theta[1] = left_lower_arm.theta[2] = 0;
	left_lower_arm.trans[0] = -0.1;
	left_lower_arm.trans[1] = 0;
	left_lower_arm.trans[2] = 0;
	left_lower_arm.type = TYPE_LEFT_LOWER_ARM;

	//left_lower_arm.action = &action4[0];
	left_lower_arm.action_idx = 0;
	left_lower_arm.action_counter = 0;

	left_lower_arm.next = 0;
	left_lower_arm.child = &left_elbow_joints;

	//left_elbow_joints
	left_elbow_joints.theta[0] = left_lower_arm.theta[1] = left_lower_arm.theta[2] = 0;
	left_elbow_joints.trans[0] = -0.7;
	left_elbow_joints.trans[1] = 0;
	left_elbow_joints.trans[2] = 0;
	left_elbow_joints.type = TYPE_ELBOW_JOINTS;

	left_elbow_joints.action = &left_elbow_joints_action[0];
	left_elbow_joints.action_idx = 0;
	left_elbow_joints.action_counter = 0;

	left_elbow_joints.next = 0;
	left_elbow_joints.child = 0;

	left_elbow_joints.theta[0] = left_lower_arm.theta[1] = left_lower_arm.theta[2] = 0;
	left_elbow_joints.trans[0] = -0.7;
	left_elbow_joints.trans[1] = 0;
	left_elbow_joints.trans[2] = 0;
	left_elbow_joints.type = TYPE_ELBOW_JOINTS;

	left_elbow_joints.next = 0;
	left_elbow_joints.child = &left_upper_arm;

	//left_upper_arm
	left_upper_arm.theta[0] = left_lower_arm.theta[1] = left_lower_arm.theta[2] = 0;
	left_upper_arm.trans[0] = -0.1;
	left_upper_arm.trans[1] = 0;
	left_upper_arm.trans[2] = 0;
	left_upper_arm.type = TYPE_LEFT_UPPER_ARM;

	left_upper_arm.next = 0;
	left_upper_arm.child = &left_palm;

	left_palm.theta[0] = left_lower_arm.theta[1] = left_lower_arm.theta[2] = 0;
	left_palm.trans[0] = -0.45;
	left_palm.trans[1] = 0;
	left_palm.trans[2] = 0;
	left_palm.type = TYPE_PALM;

	left_palm.next = 0;
	left_palm.child = 0;
}
void init_right_arm() {

	right_shoulder_joints.nums = 24;
	right_shoulder_joints.theta[0] = right_shoulder_joints.theta[1] = right_shoulder_joints.theta[2] = 0;
	right_shoulder_joints.trans[0] = 0.4;
	right_shoulder_joints.trans[1] = 1.2;
	right_shoulder_joints.trans[2] = 0;
	right_shoulder_joints.type = TYPE_BIG_JOINTS;

	right_shoulder_joints.action = &right_shoulder_joints_action[0];
	right_shoulder_joints.action_idx = 0;
	right_shoulder_joints.action_counter = 0;

	right_shoulder_joints.next = &left_leg_joints;
	right_shoulder_joints.child = &right_lower_arm;

	//right_lower_arm

	right_lower_arm.nums = 24;
	right_lower_arm.theta[0] = right_lower_arm.theta[1] = right_lower_arm.theta[2] = 0;
	right_lower_arm.trans[0] = 0.1;
	right_lower_arm.trans[1] = 0;
	right_lower_arm.trans[2] = 0;
	right_lower_arm.type = TYPE_RIGHT_LOWER_ARM;

	/*right_lower_arm.action = &action4[0];*/
	right_lower_arm.action_idx = 0;
	right_lower_arm.action_counter = 0;

	right_lower_arm.next = 0;
	right_lower_arm.child = &right_elbow_joints;

	//right_elbow_joints
	right_elbow_joints.theta[0] = right_lower_arm.theta[1] = right_lower_arm.theta[2] = 0;
	right_elbow_joints.trans[0] = 0.7;
	right_elbow_joints.trans[1] = 0;
	right_elbow_joints.trans[2] = 0;
	right_elbow_joints.type = TYPE_ELBOW_JOINTS;

	right_elbow_joints.action = &right_elbow_joints_action[0];
	right_elbow_joints.action_idx = 0;
	right_elbow_joints.action_counter = 0;

	right_elbow_joints.next = 0;
	right_elbow_joints.child = 0;

	right_elbow_joints.theta[0] = right_lower_arm.theta[1] = right_lower_arm.theta[2] = 0;
	right_elbow_joints.trans[0] = 0.7;
	right_elbow_joints.trans[1] = 0;
	right_elbow_joints.trans[2] = 0;
	right_elbow_joints.type = TYPE_ELBOW_JOINTS;

	right_elbow_joints.next = 0;
	right_elbow_joints.child = &right_upper_arm;

	//right_upper_arm
	right_upper_arm.theta[0] = right_lower_arm.theta[1] = right_lower_arm.theta[2] = 0;
	right_upper_arm.trans[0] = 0.1;
	right_upper_arm.trans[1] = 0;
	right_upper_arm.trans[2] = 0;
	right_upper_arm.type = TYPE_RIGHT_UPPER_ARM;

	right_upper_arm.next = 0;
	right_upper_arm.child = &right_palm;

	right_palm.theta[0] = right_lower_arm.theta[1] = right_lower_arm.theta[2] = 0;
	right_palm.trans[0] = 0.45;
	right_palm.trans[1] = 0;
	right_palm.trans[2] = 0;
	right_palm.type = TYPE_PALM;

	right_palm.next = 0;
	right_palm.child = 0;
}
void init_left_leg() {

	left_leg_joints.theta[0] = left_leg_joints.theta[1] = left_leg_joints.theta[2] = 0;
	left_leg_joints.trans[0] = -0.32;
	left_leg_joints.trans[1] = 0.1;
	left_leg_joints.trans[2] = 0;
	left_leg_joints.type = TYPE_LEG_KNEE_JOINTS;

	left_leg_joints.action = &left_leg_joints_action[0];
	left_leg_joints.action_idx = 0;
	left_leg_joints.action_counter = 0;

	left_leg_joints.next = &right_leg_joints;
	left_leg_joints.child = &left_upper_leg;

	//--

	left_upper_leg.theta[0] = left_upper_leg.theta[1] = left_upper_leg.theta[2] = 0;
	left_upper_leg.trans[0] = 0;
	left_upper_leg.trans[1] = 0;
	left_upper_leg.trans[2] = 0;
	left_upper_leg.type = TYPE_UPPER_LEG;

	//left_shoulder_joints.action         = &action3[0];
	left_upper_leg.action_idx = 0;
	left_upper_leg.action_counter = 0;

	left_upper_leg.next = 0;
	left_upper_leg.child = &left_knee_joints;

	left_knee_joints.theta[0] = left_knee_joints.theta[1] = left_knee_joints.theta[2] = 0;
	left_knee_joints.trans[0] = 0;
	left_knee_joints.trans[1] = -0.7;
	left_knee_joints.trans[2] = 0;
	left_knee_joints.type = TYPE_LEG_KNEE_JOINTS;


	left_knee_joints.action = &left_knee_joints_action[0];
	left_knee_joints.action_idx = 0;
	left_knee_joints.action_counter = 0;

	left_knee_joints.action_idx = 0;
	left_knee_joints.action_counter = 0;

	left_knee_joints.next = 0;
	left_knee_joints.child = &left_lower_leg;

	//---
	left_lower_leg.theta[0] = left_lower_leg.theta[1] = left_lower_leg.theta[2] = 0;
	left_lower_leg.trans[0] = 0;
	left_lower_leg.trans[1] = 0;
	left_lower_leg.trans[2] = 0;
	left_lower_leg.type = TYPE_LOWER_LEG;

	left_lower_leg.action_idx = 0;
	left_lower_leg.action_counter = 0;

	left_lower_leg.next = 0;
	left_lower_leg.child = 0;
}
void init_right_leg() {

	right_leg_joints.theta[0] = right_leg_joints.theta[1] = right_leg_joints.theta[2] = 0;
	right_leg_joints.trans[0] = 0.32;
	right_leg_joints.trans[1] = 0.1;
	right_leg_joints.trans[2] = 0;
	right_leg_joints.type = TYPE_LEG_KNEE_JOINTS;

	right_leg_joints.action = &right_leg_joints_action[0];
	right_leg_joints.action_idx = 0;
	right_leg_joints.action_counter = 0;

	right_leg_joints.next = 0;
	right_leg_joints.child = &right_upper_leg;

	//--

	right_upper_leg.theta[0] = right_upper_leg.theta[1] = right_upper_leg.theta[2] = 0;
	right_upper_leg.trans[0] = 0;
	right_upper_leg.trans[1] = 0;
	right_upper_leg.trans[2] = 0;
	right_upper_leg.type = TYPE_UPPER_LEG;

	//right_shoulder_joints.action         = &action3[0];
	right_upper_leg.action_idx = 0;
	right_upper_leg.action_counter = 0;

	right_upper_leg.next = 0;
	right_upper_leg.child = &right_knee_joints;

	right_knee_joints.theta[0] = right_knee_joints.theta[1] = right_knee_joints.theta[2] = 0;
	right_knee_joints.trans[0] = 0;
	right_knee_joints.trans[1] = -0.7;
	right_knee_joints.trans[2] = 0;
	right_knee_joints.type = TYPE_LEG_KNEE_JOINTS;


	right_knee_joints.action = &right_knee_joints_action[0];
	right_knee_joints.action_idx = 0;
	right_knee_joints.action_counter = 0;

	right_knee_joints.next = 0;
	right_knee_joints.child = &right_lower_leg;

	//---
	right_lower_leg.theta[0] = right_lower_leg.theta[1] = right_lower_leg.theta[2] = 0;
	right_lower_leg.trans[0] = 0;
	right_lower_leg.trans[1] = 0;
	right_lower_leg.trans[2] = 0;
	right_lower_leg.type = TYPE_LOWER_LEG;

	right_lower_leg.action_idx = 0;
	right_lower_leg.action_counter = 0;

	right_lower_leg.next = 0;
	right_lower_leg.child = 0;
}
void init_object()
{
	// object initialization
	body.nums = 24;
	body.theta[0] = body.theta[1] = body.theta[2] = 0;
	body.trans[0] = body.trans[1] = body.trans[2] = 0;

	body.action = &body_action[0];
	body.action_idx = 0;
	body.action_counter = 0;
	body.type = TYPE_BODY;

	body.next = 0;
	body.child = &head;

	head.nums = 24;
	head.theta[0] = head.theta[1] = head.theta[2] = 0;
	head.trans[0] = 0;
	head.trans[1] = 1.55;
	head.trans[2] = 0;
	head.type = TYPE_HEAD;

	head.action = &head_action[0];
	head.action_idx = 0;
	head.action_counter = 0;

	head.next = &left_shoulder_joints;
	head.child = 0;

	init_left_arm();
	init_right_arm();
	init_left_leg();
	init_right_leg();



	t = gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);
	gluQuadricNormals(t, GLU_SMOOTH);
}

void spin()
{
	sync_flag = 1;

	timess = glutGet(GLUT_ELAPSED_TIME);

	deltaTime = timess - oldTimeSinceStart;

	oldTimeSinceStart = timess;

	printf("delta : %f\n", deltaTime);

	Sleep(16);


	glutPostRedisplay();
}

// ---------------------------------------------------------------------------------------------
// I/O
// ---------------------------------------------------------------------------------------------

void keyboard_handler(unsigned char key, int x, int y)
{
	if (key == 'x') v.eye[0] -= 0.1; if (key == 'X') v.eye[0] += 0.1;
	if (key == 'y') v.eye[1] -= 0.1; if (key == 'Y') v.eye[1] += 0.1;
	if (key == 'z') v.eye[2] -= 0.1; if (key == 'Z') v.eye[2] += 0.1;
	if (key == 'p') {
		play = 1 - play;
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
			(DWORD)(LPVOID)&m_mciPlayParms);
	}
	if (key == 'r')
	{
		play = 0;
		init_object();
	}
}

GLfloat clip(GLfloat x)
{
	if (x < 0) return x + 360.0;
	if (x > 360.0) return x - 360.0;
	return x;
}

void action(Object* p)
{
	if (p->action == 0) return;
	if (p->action[p->action_idx].type == 'F')
	{
		p->action_idx = 0;
		p->action_counter = 0;
		p->action = 0;
		return;
	}
	if (p->action[p->action_idx].type == 'R')
	{
		for (int i = 0; i<3; i++)
			p->theta[i] = clip(p->theta[i] + p->action[p->action_idx].param[i] / p->action[p->action_idx].duration);
		p->action_counter++;
	}
	if (p->action[p->action_idx].type == 'T')
	{
		for (int i = 0; i<3; i++)
			p->trans[i] += p->action[p->action_idx].param[i] / p->action[p->action_idx].duration;
		p->action_counter++;
	}
	if (p->action[p->action_idx].type == 'N')
	{
		p->action_counter++;
	}
	if (p->action_counter >= p->action[p->action_idx].duration)
	{
		p->action_idx++;
		p->action_counter = 0;
	}
}

void draw(Object* p)
{
	// store matrix
	glPushMatrix();

	// transformation
	glTranslatef(p->trans[0], p->trans[1], p->trans[2]);
	glRotatef(p->theta[0], 1.0, 0.0, 0.0);
	glRotatef(p->theta[1], 0.0, 1.0, 0.0);
	glRotatef(p->theta[2], 0.0, 0.0, 1.0);

	// draw
	if (p->type == TYPE_BODY) {
		glPushMatrix();
		glRotatef(-90.0, 1, 0, 0);
		glColor3f(1, 0, 0);
		gluCylinder(t, 0.3, 0.4, 1.4, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_HEAD) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(0.3, 10, 10);
		glColor3f(1, 1, 0);
		glRotatef(-5.0, 1, 0, 0);
		glutSolidCone(0.2, 0.5, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_BIG_JOINTS) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(0.2, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_LEFT_LOWER_ARM) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(-90.0, 0, 1, 0);
		gluCylinder(t, 0.14, 0.13, 0.8, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_RIGHT_LOWER_ARM) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(90.0, 0, 1, 0);
		gluCylinder(t, 0.14, 0.13, 0.8, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_ELBOW_JOINTS) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(0.18, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_LEFT_UPPER_ARM) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(-90.0, 0, 1, 0);
		gluCylinder(t, 0.12, 0.11, 0.4, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_RIGHT_UPPER_ARM) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(90.0, 0, 1, 0);
		gluCylinder(t, 0.12, 0.11, 0.4, 10, 10);
		glPopMatrix();
	}
	//��
	else if (p->type == TYPE_PALM) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(0.17, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_LEG_KNEE_JOINTS) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glutSolidSphere(0.2, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_UPPER_LEG) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(90.0, 1, 0, 0);
		gluCylinder(t, 0.14, 0.13, 0.8, 10, 10);
		glPopMatrix();
	}
	else if (p->type == TYPE_LOWER_LEG) {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glRotatef(90.0, 1, 0, 0);
		gluCylinder(t, 0.12, 0.18, 0.7, 10, 10);
		glPopMatrix();
	}
	else {
		glVertexPointer(3, GL_FLOAT, 0, p->vertices);
		glColorPointer(3, GL_FLOAT, 0, p->colors);
		glDrawElements(GL_QUADS, p->nums, GL_UNSIGNED_BYTE, p->indices);
	}
	// apply action
	if (play && sync_flag) action(p);

	// draw children
	if (p->child) draw(p->child);

	// restore matrix
	glPopMatrix();

	// draw siblings
	if (p->next) draw(p->next);
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}


//�߰�



void display()
{
	// clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//�ű�
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHT0);

	//�߰�
	glEnable(GL_TEXTURE_2D);
	//�߰�
	
	gluLookAt(v.eye[0], v.eye[1], v.eye[2], v.at[0], v.at[1], v.at[2], v.up[0], v.up[1], v.up[2]);
	

	//�߰�
	glTranslatef(0.0, 0.0, 10);
	square();
	glColor3f(1, 1, 1);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0.0, -0.9, 0.0);

	// draw
	draw(&body);



	// flush & swap buffers
	glFlush();
	glutSwapBuffers();
}



GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	Image g_cImg;

	g_cImg.loadFromBMP(filename);


	glGenTextures(1, &texture); //generate the texture with the loaded data 
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it��s array 


	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //set texture environment parameters 


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	//Generate the texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_cImg.getRGB());
	
	
	return texture; //return whether it was successfull 

}


int main(int argc, char* argv[])
{

	//PlaySound(TEXT(SOUND_FILE_NAME), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	mciOpen.lpstrElementName = L".\\bom.mp3"; // ���� ��� �Է�
	mciOpen.lpstrDeviceType = L"mpegvideo";  
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	dwID = mciOpen.wDeviceID;

	// GLUT initialization
	glutInit(&argc, (char**)argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(500, 500);

	glutCreateWindow("Bombastic");
	
 
	// call-back functions
	glutIdleFunc(spin);
	//glutTimerFunc(delay, timer, 0);
	glutKeyboardFunc(keyboard_handler);
	glutDisplayFunc(display);

	// enable color/vertex array
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// initialize object
	init_object();

	// initialize viewer
	v.eye[0] = 0.0; v.eye[1] = 0.0; v.eye[2] = 1.0;
	v.at[0] = 0.0; v.at[1] = 0.0; v.at[2] = 0.0;
	v.up[0] = 0.0; v.up[1] = 1.0; v.up[2] = 0.0;

	init();

	// top,floor,left,right
	//texture[3] = LoadTexture("side.bmp", 640, 480);      //left
	//texture[2] = LoadTexture("tex3df.bmp", 640, 480);      //right 
	//texture[1] = LoadTexture("wood2.bmp", 300, 161);      //floor 
	texture[0] = LoadTexture("curtain2.bmp", 435, 348);      //top    
	//texture[0] = LoadTexture("input.bmp", 895, 387);      //top 
	//texture[0] = LoadTexture("curtain8.bmp", 330, 348);      //top    

	glutMainLoop();

	return 0;
}

void square(void) {

	glPushMatrix();

	//background
	glBindTexture(GL_TEXTURE_2D, texture[0]); //bind our texture to our shape 

	glBegin(GL_QUADS);


	glTexCoord2f(0.0, 0.0); glVertex3f(-3, -3, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3, 3, -5);
	glTexCoord2f(1.0, 1.0); glVertex3f(3, 3, -5);
	glTexCoord2f(1.0, 0.0); glVertex3f(3, -3, -5);
	glEnd();


	////right 
	//glBindTexture(GL_TEXTURE_2D, texture[2]); //bind our texture to our shape 
	//glBegin(GL_QUADS);

	//glTexCoord2f(0.0, 0.0); glVertex3f(2.0, 2.0, 2.0);
	//glTexCoord2f(0.0, 1.0); glVertex3f(3, 3, -2.0);
	//glTexCoord2f(1.0, 1.0); glVertex3f(3, -3, -2.0);
	//glTexCoord2f(1.0, 0.0); glVertex3f(2, -1.5, 2.0);

	//glEnd();
	////left
	//glBindTexture(GL_TEXTURE_2D, texture[3]); //bind our texture to our shape 

	//glBegin(GL_QUADS);

	//glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, -3.0, -2.0);
	//glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, -1.5, 2.0);
	//glTexCoord2f(1.0, 1.0); glVertex3f(-2.0, 2.0, 2.0);
	//glTexCoord2f(1.0, 0.0); glVertex3f(-3.0, 3.0, -2.0);

	//glEnd();

	////floor
	//glBindTexture(GL_TEXTURE_2D, texture[1]); //bind our texture to our shape 

	//glBegin(GL_QUADS);

	//glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, -3.0, -2.0);
	//glTexCoord2f(0.0, 1.0); glVertex3f(3, -3, -2.0);
	//glTexCoord2f(1.0, 1.0); glVertex3f(2, -1.5, 2.0);
	//glTexCoord2f(1.0, 0.0); glVertex3f(-2, -1.5, 2.0);

	//glEnd();

	glPopMatrix();

}
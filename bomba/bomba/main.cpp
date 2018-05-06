// Prog35.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <math.h>
#include <glut.h>
#include <Windows.h>
#include <mmsystem.h>	
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")

#define SOUND_FILE_NAME ".\\bgm.wav"

// ---------------------------------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------------------------------
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

Action body_action[] = {
	{ 'N', 110,   0, 0,   0 },
	{ 'R', 100, 0, 50, 0},
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
	{ 'R', 25, 0, -100, 0 },
	{ 'R', 25, 0, 100, 0 },




	

	{ 'F',  0,    0,   0,   0 }
};

Action left_shoulder_joints_action[] = {
	{ 'R', 30,    0, 0,   -90 },
	{ 'F',  0,    0,   0,   0 }
};

Action left_elbow_joints_action[] = {
	{ 'N', 100,   0, 0,   0 },
	{ 'R',  10,    0,   0,   45 },
	{ 'R', 1000, 7200, 0, 0},
	{ 'F',  0,    0,   0,   0 },
};

Action right_shoulder_joints_action[] = {
	{ 'R', 30,    0, 0,   -90 },
	{ 'N', 100,   0, 0,   0 },
	{ 'R', 30,   0, 0,   50 },
	{ 'F',  0,    0,   0,   0 }
};

Action right_elbow_joints_action[] = {
	{ 'N', 130,   0, 0,   0 },
	{ 'R', 30,   0, 0,   -90 },
	{ 'F',  0,    0,   0,   0 }
};

Action left_leg_joints_action[] = {
	{ 'R', 30,    0, 0,   -10 },
	
	{ 'F',  0,    0,   0,   0 }
};

Action left_knee_joints_action[] = {
	{ 'N', 130,   0, 0,   0 },
	{ 'R', 30,    40, 0,   0 },
	{ 'F',  0,    0,   0,   0 }
};


Action right_leg_joints_action[] = {
	{ 'R',30,    0,   0,   10 },
	{ 'N', 130,   0, 0,   0 },
	{ 'R',30,    -30,   0,   0 },
	{ 'F',  0,    0,   0,   0 }
};

Action right_knee_joints_action[] = {
	{ 'N', 130,   0, 0,   0 },
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

	{ 'F',  0,    0,   0,   0 }

	
};



//Action action4[] = {
//	{ 'N',240,    0,   0,   0 },
//	{ 'R', 60,  720,   0,   0 },
//	{ 'N', 30,    0,   0,   0 },
//	{ 'R', 60,    0, 720,   0 },
//	{ 'F',  0,    0,   0,   0 }
//};

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

	glClearColor(1.0, 1.0, 1.0, 1.0);
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
		glRotatef(-90.0, 1, 0, 0);
		glutSolidCone(0.27, 0.8, 10, 10);
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
	if (play) action(p);

	// draw children
	if (p->child) draw(p->child);

	// restore matrix
	glPopMatrix();

	// draw siblings
	if (p->next) draw(p->next);
}

void display()
{
	// clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHT0);


	// setup viewer
	glLoadIdentity();
	gluLookAt(v.eye[0], v.eye[1], v.eye[2], v.at[0], v.at[1], v.at[2], v.up[0], v.up[1], v.up[2]);

	// draw
	draw(&body);

	// flush & swap buffers
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{

	//PlaySound(TEXT(SOUND_FILE_NAME), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	mciOpen.lpstrElementName = L".\\bom.mp3"; // 파일 경로 입력
	mciOpen.lpstrDeviceType = L"mpegvideo";

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	dwID = mciOpen.wDeviceID;

	// GLUT initialization
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Prog35: scripts");

	// call-back functions
	glutIdleFunc(spin);
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
	glutMainLoop();

	return 0;
}

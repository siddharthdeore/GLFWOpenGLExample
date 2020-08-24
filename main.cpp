#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static int WIDTH = 640;
static int HEIGHT = 480;
double PI = 3.1415;
double trans = 0;
float rotatex = 0, rotatey = 0, mousex = 0, mousey = 0,startx=0,starty=0;
float posx=0, posy=0, posz=0;
float dragx = 0, dragy = 0;
float zoom=1.0;
short dragging = 0;
int keyArr[350];

void drawAxis(float x, float y, float z, float scale) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLineWidth(2.0);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(scale, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0, scale, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0, 0, scale);

	glEnd();

	glPopMatrix();

}
void drawCube(float x, float y, float z, float dimX, float dimY, float dimZ, float psi, float theta = 0, float phi = 0, float r = 0.0, float g = 0.0, float b = 0.0)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotated(psi, 0, 0, 1); //rotate gamma around the z axis
	glRotated(theta, 0, 1, 0); //rotate gamma around the y axis
	glRotated(phi, 1, 0, 0); //rotate gamma around the x axis
	glScalef(dimX / 2, dimY / 2, dimZ / 2);

	GLdouble clrVec[6][3];

	for (int i = 0; i < 6; i++) {
		clrVec[i][0] = r + float(i) * 0.05; // red + shade
		clrVec[i][1] = g + float(i) * 0.05; // green + shade
		clrVec[i][2] = b + float(i) * 0.05; // blue + shade
	}


	glBegin(GL_QUADS);
	glColor3dv(clrVec[0]);    // Color Green - TOP
	glVertex3f(1.0, 1.0, 1.0); // TOP-RIGHT-NEAR
	glVertex3f(-1.0, 1.0, 1.0); // TOP-LEFT-NEAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-LEFT-FAR
	glVertex3f(1.0, 1.0, -1.0); // TOP-RIGHT-FAR

	glColor3dv(clrVec[1]); // Color RED - Bottom
	glVertex3f(1.0, -1.0, 1.0); //BOTTOM-RIGHT-NEAR
	glVertex3f(-1.0, -1.0, 1.0); //BOTTOM-LEFT-NEAR
	glVertex3f(-1.0, -1.0, -1.0); //BOTTOM-LEFT-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-RIGHT-FAR

	glColor3dv(clrVec[2]); // Color Yellow - Back
	glVertex3f(1.0, 1.0, -1.0); //TOP-RIGHT-FAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-LEFT-FAR
	glVertex3f(-1.0, -1.0, -1.0); //BOTTOM-LEFT-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-RIGHT-FAR

	glColor3dv(clrVec[3]); //Color Blue - RIGHT
	glVertex3f(1.0, 1.0, 1.0); //TOP-FRONT-NEAR
	glVertex3f(1.0, 1.0, -1.0); //TOP-BACK-FAR
	glVertex3f(1.0, -1.0, -1.0); //BOTTOM-BACK-FAR
	glVertex3f(1.0, -1.0, 1.0); //BOTTOM-FRONT-NEAR

	glColor3dv(clrVec[4]); //Color Orange - Left
	glVertex3f(-1.0, 1.0, 1.0); //TOP-FRONT-NEAR
	glVertex3f(-1.0, 1.0, -1.0); //TOP-BACK-FAR
	glVertex3f(-1.0, -1.0, -1.0);//BOTTOM-BACK-FAR
	glVertex3f(-1.0, -1.0, 1.0); //BOTTOM-FRONT-NEAR

	glColor3dv(clrVec[5]); // Color D - Front
	glVertex3f(1.0, 1.0, 1.0); //TOP-RIGHT-NEAR
	glVertex3f(-1.0, 1.0, 1.0); //TOP-LEFT-NEAR
	glVertex3f(-1.0, -1.0, 1.0); //BOTTOM-LEFT-NEAR
	glVertex3f(1.0, -1.0, 1.0); //BOTTOM-RIGHT-NEAR

	glEnd();
	drawAxis(x, y, z, dimX);

	glPopMatrix();
}
float randf() {
	return (float)std::rand() / RAND_MAX;
}
class Box
{
public:
	Box(GLfloat l=1, GLfloat w=1, GLfloat h=1) {
		setSize(l, w, h);
		setColor(randf(), randf(), randf());
	}
	~Box() {};
	void render() {
		drawCube(cx, cy, cz, l, w, h, rx, ry, rz, R, G, B);
	}
	void setSize(GLfloat l = 1, GLfloat w = 1, GLfloat h = 1) {
		this->l = l;
		this->w = w;
		this->h = h;
	}
	void setOrientation(GLfloat, GLfloat, GLfloat);
	void setPosition(GLfloat x=1, GLfloat y=1, GLfloat z=1) {
		cx = x; cy = y; cz = z;
	}
	void setColor(GLfloat r, GLfloat g, GLfloat b) {
		R = r, G = g, B = b;
	}
	GLfloat cx = 0, cy = 0, cz = 0;
	GLfloat rx = 0, ry = 0, rz = 0;
protected:
	GLfloat R = 0.5, G = 0.5, B = 0.5;
	GLfloat l = 0, w = 0, h = 0;

};

static void Initialize(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
static void Resize(GLFWwindow* window, int w, int h) {
	if (h < 1)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25.0f, (float)w / (float)h, 0.1f, 1000.0f);
	posx = 0, posy = 0, posz = 0;
	gluLookAt(posx, posy + 2.0f, posz + 10.0f, posx, posy, posz, 0, 3, 0);
	glMatrixMode(GL_MODELVIEW);

}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	keyArr[key] = action;
}

static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		startx = mousex;
		starty = mousey;
	}
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			dragging = 1;
			if (action == GLFW_RELEASE) {
				rotatex += dragx;
				rotatey += dragy;
				dragx = 0;
				dragy = 0;
				dragging = 0;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			dragging = 2;
			if (action == GLFW_RELEASE) {
				posx -= dragx * 0.1;
				posy += dragy * 0.1;
				dragx = 0;
				dragy = 0;
				dragging = 0;
			}
			break;
	}
}

static void MouseMotionCallback(GLFWwindow* window, double x, double y) {
		mousex = x;
		mousey = y;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom = zoom + yoffset * 0.1;
	zoom = zoom > 10 ? 10 : zoom;
	zoom = zoom < 0.2 ? 0.2 : zoom;

}


static void Update(GLFWwindow* window, float delta) {
	//std::cout << "delta:" << delta << std::endl;
	if (keyArr[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, 1);
	posx -= (keyArr[GLFW_KEY_LEFT] - keyArr[GLFW_KEY_RIGHT]) *0.1;
	posy -= (keyArr[GLFW_KEY_UP] - keyArr[GLFW_KEY_DOWN]) * 0.1;
	posz -= (keyArr[GLFW_KEY_Q] - keyArr[GLFW_KEY_A]) * 0.1;
}

static void grid(int w, int h) {
	glColor3f(0.4, 0.4, 0.4);
	glLineWidth(1.0);
	for (int i = -w / 2; i < w / 2; i++) {
		glBegin(GL_LINES);
		glVertex3f(i, 0, -h / 2);
		glVertex3f(i, 0, h / 2);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(-w / 2, 0, i);
		glVertex3f(w / 2, 0, i);
		glEnd();

	}

}
Box b[10];
static void RenderScene(GLFWwindow* window, float delta) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(posx,posy+2,posz+10,posx,posy,posz,0,3,0);
	glScaled(zoom, zoom, zoom);
	std::cout << dragging << std::endl;
	if (dragging == 1) {
		glRotatef(rotatex + dragx, 0, 1, 0);
		glRotatef(rotatey + dragy, 1, 0, 0);
	}
	else{
		glRotatef(rotatex , 0, 1, 0);
		glRotatef(rotatey , 1, 0, 0);
	}
	grid(20, 20);
	drawAxis(0, 0, 0, 2);


	drawCube(4, 4, 4, 2, 2, 2, 0, 0, 0, 0.3, 0.4, 0.6);

	for (int i = 0; i < 10; i++) {
		b[i].render();

		b[i].rx += randf();
		b[i].ry += randf();
		b[i].rz += randf();

	}
	glPopMatrix();

	glFlush();
}

int main(int argc, char** argv) {
	GLFWwindow* window;

	glfwInit();
	window = glfwCreateWindow(WIDTH, HEIGHT, argv[0], NULL, NULL);
	glfwMakeContextCurrent(window);

	Initialize();
	Resize(window, WIDTH, HEIGHT);

	glfwSetWindowSizeCallback(window, Resize);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseClickCallback);
	glfwSetCursorPosCallback(window, MouseMotionCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 10; i++) {
		b[i].setPosition(cos((float)i/5*PI) * 5, 0, sin((float)i / 5 * PI)*5);
	}
	while (!glfwWindowShouldClose(window)) {
		float delta = glfwGetTime();
		Update(window, delta);
		RenderScene(window, delta);
		if (dragging==1 || dragging==2) {
			dragx = (mousex - startx)/WIDTH * 180;
			dragy = (mousey - starty)/HEIGHT * 180;

			//std::cout << dragx << " " << dragy << std::endl;
		}
		glfwSetTime(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	return EXIT_SUCCESS;
}
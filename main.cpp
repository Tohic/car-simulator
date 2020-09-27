#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "opengl32.lib")
// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "Texture.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

class Car
{
private:
	Model *body, *fright, *fleft, *bright, *bleft, *fright1, *fleft1, *bright1, *bleft1, *map;
	float frontdegree, maxfrontdegree,frangle, frangle1;
	float rbody, rfright, rfleft, rbright, rbleft, sbody, sfright,sfleft, sbright, sbleft;
	float speed, speedchange, maxspeed, brake, rollforward, ds;
	float l, w;
	float x, y, z, xyz, x1, x2, x3, z1, z2, z3;
	int i, gear;
	float dx, dy, dz,dz1;
	glm::mat4 frw, frw1,qw, frw2;
	glm::mat4 ex;
public:
	Car()
	{
		map = new Model("res/models/map/map3.obj");
		Porsche();
		//Delorean();
	}
	void Porsche()
	{
		fright = new Model("res/models/porsche1/wheel.obj");
		fleft = new Model("res/models/porsche1/wheel.obj");
		bright = new Model("res/models/porsche1/wheel.obj");
		bleft = new Model("res/models/porsche1/wheel.obj");
		body = new Model("res/models/porsche1/body1.obj");//untitled.obj or body.obj
		dx = 2.0f;
		dy = 0.65f;
		dz = 3.75f;
		dz1 = -3.25f;
		ds = 0;
		for (i = 0; i < body->meshes.size(); i++)
		{
			body->meshes[i].trans = glm::translate(body->meshes[i].trans, glm::vec3(0.0f, -0.1f, 0.0f));
		}
		maxfrontdegree = 50;
		frontdegree = 0;
		frangle1= frangle = 0.015;
		speedchange = 0.005;
		maxspeed = 0.6;
		frw = glm::mat4(1.0f);
		frw1 = glm::mat4(1.0f);
		frw2 = glm::mat4(1.0f);
		qw = glm::mat4(1.0f);
		brake = 3;
		rollforward = 2;
		l = 6.9;
		w = 1.8;
		gear = 1;
	}
	void Delorean()
	{
		fright = new Model("res/models/delorean/wheel.obj");
		fleft = new Model("res/models/delorean/wheel.obj");
		bright = new Model("res/models/delorean/wheel1.obj");
		bleft = new Model("res/models/delorean/wheel1.obj");
		body = new Model("res/models/delorean/body.obj");
		dx =2.4f;
		dy = 0.75f;
		dz = 3.95f;
		dz1 = -3.85f;
		ds = 0;
		for (i = 0; i < body->meshes.size(); i++)
		{
			body->meshes[i].trans = glm::translate(body->meshes[i].trans, glm::vec3(0.0f, -0.2f, 0.0f));
		}
		maxfrontdegree = 50;
		frontdegree = 0;
		frangle1 = frangle = 0.015;
		speedchange = 0.005;
		maxspeed = 0.7;
		frw = glm::mat4(1.0f);
		frw1 = glm::mat4(1.0f);
		frw2 = glm::mat4(1.0f);
		qw = glm::mat4(1.0f);
		brake = 3;
		rollforward = 2;
		l = 4;
		w = 1;
		gear = 1;
	}
	void Draw(Shader shader)
	{
		for (i = 0; i < fright->meshes.size(); i++)
		{
			fright->meshes[i].trans = glm::translate(fright->meshes[i].trans, glm::vec3(-dx, dy, dz));
			fleft->meshes[i].trans = glm::translate(fleft->meshes[i].trans, glm::vec3(dx, dy, dz));
			fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			bright->meshes[i].trans = glm::translate(bright->meshes[i].trans, glm::vec3(-dx, dy, dz1));
			bleft->meshes[i].trans = glm::translate(bleft->meshes[i].trans, glm::vec3(dx, dy, dz1));
			bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));

			fright->meshes[i].trans = fright->meshes[i].trans * frw * frw1;
			fleft->meshes[i].trans = fleft->meshes[i].trans * frw * frw2;
			bright->meshes[i].trans = bright->meshes[i].trans * frw1;
			bleft->meshes[i].trans = bleft->meshes[i].trans * frw2;
		}
		body->Draw(shader);
		fright->Draw(shader);
		fleft->Draw(shader);
		bright->Draw(shader);
		bleft->Draw(shader);
		map->Draw(shader);
		for (i = 0; i < fright->meshes.size(); i++)
		{
			fright->meshes[i].trans = fright->meshes[i].trans / (frw * frw1);
			fleft->meshes[i].trans = fleft->meshes[i].trans / (frw * frw2);
			bright->meshes[i].trans = bright->meshes[i].trans / (frw1);
			bleft->meshes[i].trans = bleft->meshes[i].trans / (frw2);

			fright->meshes[i].trans = glm::translate(fright->meshes[i].trans, glm::vec3(dx, -dy, -dz));
			fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, -3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			fleft->meshes[i].trans = glm::translate(fleft->meshes[i].trans, glm::vec3(-dx, -dy, -dz));
			bright->meshes[i].trans = glm::translate(bright->meshes[i].trans, glm::vec3(dx, -dy, -dz1));
			bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, -3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			bleft->meshes[i].trans = glm::translate(bleft->meshes[i].trans, glm::vec3(-dx, -dy, -dz1));
		}
	}
	void FrontMovement(Shader shader)
	{
		if (keys[GLFW_KEY_D])
		{
			if (frontdegree > -maxfrontdegree)
			{
				frontdegree -= frangle*100;
			}
		}
		else
			if (keys[GLFW_KEY_A])
			{
				if (frontdegree < maxfrontdegree)
				{
					frontdegree += frangle*100;
				}
			}
			else 
				if (frontdegree < 0)
				{
					frontdegree += frangle * 100;
					if (frontdegree >= 0)
						frontdegree = 0;
				}
				else 
					if (frontdegree > 0)
					{
						frontdegree -= frangle*100;
						if (frontdegree <= 0)
							frontdegree = 0;
					}
		frw[2][2] = cosf(frontdegree / 100);
		frw[2][0] = sinf(frontdegree / 100);
		frw[0][2] = -sinf(frontdegree / 100);
		frw[0][0] = cosf(frontdegree / 100);
	}
	void BackMovement(Shader shader)
	{
		if (keys[GLFW_KEY_W])
		{
			if (speed < maxspeed)
			{
				speed = speed + speedchange;
				ds = ds + speedchange;
			}
		}
		else 
			if (keys[GLFW_KEY_S])
			{
				if (speed)
				{
					speed = speed - brake*speedchange;
					if (gear == 1 && speed <= speedchange)
					{
						speed = 0;
						ds = 0;
					}
					else 
						if (gear == -1 && speed >= speedchange)
						{
							speed = 0;
							ds = 0;
						}
				}
			}
			else 
				if (speed > 0)
				{
					speed = speed - abs(speedchange / rollforward);
					ds = ds + abs(speedchange / rollforward);
					if (speed <= 0)
					{
						speed = 0;
						ds = 0;
					}
				}
				else 
					if (speed < 0)
					{
						speed = speed + abs(speedchange / rollforward);
						ds = ds - abs(speedchange / rollforward);
						if (speed >= 0)
							speed = 0;
					}
		/*if (speed)
		{
			if (speedchange>speed)
				ds = ds + speed;
			else ds = ds + speedchange;
		}*/
		/*if (speed)
		{
			if (ds < speed)
				ds = speed;
			else if (ds > speed)
				ds = ds + (ds - speed);
			else ds = ds + speedchange;
		}
		else ds = 0;*/
		//ds = speed;
		if (speed)
		{

			if (frangle1 >= (speed/500))
				frangle = frangle1 - abs(speed/500);
			else frangle = frangle1;
		}
		//std::cout << speed << " " << frangle << endl;
		/*if (ds > 6.28) ds = ds - 6.28;
		if (ds < -6.28) ds = ds + 6.28;*/
		frw1[1][1] = cosf(-ds * 40);
		frw1[2][2] = cosf(-ds * 40);
		frw1[1][2] = -sinf(-ds * 40);
		frw1[2][1] = sinf(-ds * 40);
		frw2[1][1] = cosf(ds * 40);
		frw2[2][2] = cosf(ds * 40);
		frw2[1][2] = -sinf(ds * 40);
		frw2[2][1] = sinf(ds * 40);
	}
	void TopGear()
	{
		if (keys[GLFW_KEY_LEFT_SHIFT])
		{
			gear = 1;
			speedchange = abs(speedchange);
		}
		else
			if (keys[GLFW_KEY_LEFT_CONTROL])
			{
				gear = -1;
				speedchange = -abs(speedchange);
			}
	}
	bool checkbox()
	{
		ex = body->meshes[0].trans;
		ex = glm::translate(ex, glm::vec3(0.0f, 0.0f, 1.5 * speed));
		x = ex[0][0] * body->meshes[0].vertices[0].Position.x + ex[1][0] * body->meshes[0].vertices[0].Position.y + ex[2][0] * body->meshes[0].vertices[0].Position.z + ex[3][0];
		x1 = ex[0][0] * (body->meshes[0].vertices[0].Position.x - w) + ex[1][0] * body->meshes[0].vertices[0].Position.y + ex[2][0] * body->meshes[0].vertices[0].Position.z + ex[3][0];
		x2 = ex[0][0] * (body->meshes[0].vertices[0].Position.x - w) + ex[1][0] * body->meshes[0].vertices[0].Position.y + ex[2][0] * (body->meshes[0].vertices[0].Position.z + l) + ex[3][0];
		x3 = ex[0][0] * (body->meshes[0].vertices[0].Position.x) + ex[1][0] * body->meshes[0].vertices[0].Position.y + ex[2][0] * (body->meshes[0].vertices[0].Position.z + l) + ex[3][0];
		z = ex[0][2] * body->meshes[0].vertices[0].Position.x + ex[1][2] * body->meshes[0].vertices[0].Position.y + ex[2][2] * body->meshes[0].vertices[0].Position.z + ex[3][2];
		z1 = ex[0][2] * (body->meshes[0].vertices[0].Position.x - w) + ex[1][2] * body->meshes[0].vertices[0].Position.y + ex[2][2] * body->meshes[0].vertices[0].Position.z + ex[3][2];
		z2 = ex[0][2] * (body->meshes[0].vertices[0].Position.x - w) + ex[1][2] * body->meshes[0].vertices[0].Position.y + ex[2][2] * (body->meshes[0].vertices[0].Position.z + l) + ex[3][2];
		z3 = ex[0][2] * body->meshes[0].vertices[0].Position.x + ex[1][2] * body->meshes[0].vertices[0].Position.y + ex[2][2] * (body->meshes[0].vertices[0].Position.z + l) + ex[3][2];
		std::cout << "x " << x << " " << x1 << " " << x2 << " " << x3 << endl;
		std::cout << "z " << z << " " << z1 << " " << z2 << " " << z3 << endl;
		if ((x > -2.23 && x1 > -2.23 && x2 > -1.43 && x3 > -1.43) && (x < 39.23 && x1 < 39.23 && x2 < 38.43 && x3 < 38.43)
			&& (z < 44.73 && z1 < 44.73 && z2 < 43.93 && z3 < 43.93) && (z > -26.46 && z1 > -26.46 && z2 > -25.66 && z3 > -25.66))
			/*if ((x < 3.33 && x1 < 3.33 && x2 < 2.53 && x3 < 2.53) || (x > 3.95 && x1 > 3.95 && x2 > 4.75 && x3 > 4.75))
				return 1;
			else*/
			//if ((z < -11.86 && z1 < -11.86 && z2 < -12.66 && z3 < -12.66) || (z > 15.58 && z1 > 15.58 && z2 > 16.38 && z3 > 16.38))
		{
			if (((x > 17.68 && x1 > 17.68 || x2 > 16.78 && x3 > 16.78) && (x < 19.58 && x1 < 19.58 || x2 < 20.38 && x3 < 20.38))
				&& (z > 22.67 && z1 > 22.67 || z2 > 21.71 && z3 > 21.71) && (z < 24.45 && z1 < 24.45 || z2 < 25.41 && z3 < 25.41))
				return 0;
			if ((x > 3.33 && x1 > 3.33 || x2 > 2.54 && x3 > 2.54) 
				&& (x < 3.95 && x1 < 3.95 || x2 < 4.4 && x3 < 4.4)
				&& (z > -11.86 && z1 > -11.86 || z2 > -12.66 && z3 > -12.66) 
				&& (z < 15.58 && z1 < 15.58 || z2 < 16.38 && z3 < 16.38))
				return 0;
			//else { std::cout << "1" << endl; return 1; }
			return 1;
		}
		else { std::cout << "2" << endl; return 0; }
	}
	void Dvizh()
	{
		if (speed)
		{
			
			if (checkbox())
			{
				for (GLint i = 0; i < fright->meshes.size(); i++)
				{
					fright->meshes[i].trans = glm::translate(fright->meshes[i].trans, glm::vec3(0.0f, 0.0f, speed));
					fleft->meshes[i].trans = glm::translate(fleft->meshes[i].trans, glm::vec3(0.0f, 0.0f, speed));
					bright->meshes[i].trans = glm::translate(bright->meshes[i].trans, glm::vec3(0.0f, 0.0f, speed));
					bleft->meshes[i].trans = glm::translate(bleft->meshes[i].trans, glm::vec3(0.0f, 0.0f, speed));
					if (frontdegree < 0)
						if (speed > 0)
						{
							fright->meshes[i].trans = glm::rotate(fright->meshes[i].trans, -abs(frontdegree/10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, -abs(frontdegree/10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							bright->meshes[i].trans = glm::rotate(bright->meshes[i].trans, -abs(frontdegree/10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, -abs(frontdegree/10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
						}
						else
						{
							fright->meshes[i].trans = glm::rotate(fright->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							bright->meshes[i].trans = glm::rotate(bright->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
						}
					else
						if (frontdegree > 0)
							if (speed > 0)
							{
								fright->meshes[i].trans = glm::rotate(fright->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								bright->meshes[i].trans = glm::rotate(bright->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							}
							else
							{
								fright->meshes[i].trans = glm::rotate(fright->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								fleft->meshes[i].trans = glm::rotate(fleft->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								bright->meshes[i].trans = glm::rotate(bright->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
								bleft->meshes[i].trans = glm::rotate(bleft->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							}
				}
				for (GLint i = 0; i < body->meshes.size(); i++)
				{
					body->meshes[i].trans = glm::translate(body->meshes[i].trans, glm::vec3(0.0f, 0.0f, speed));
					if (frontdegree < 0)
						if (speed > 0)
							body->meshes[i].trans = glm::rotate(body->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
						else
							body->meshes[i].trans = glm::rotate(body->meshes[i].trans, -abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));

					else
						if (frontdegree > 0)
							if (speed > 0)
								body->meshes[i].trans = glm::rotate(body->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
							else
								body->meshes[i].trans = glm::rotate(body->meshes[i].trans, abs(frontdegree / 10)*frangle * speed, glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
			else
			{
				speed = 0;
			}
		}
	}
};

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
	Shader skyboxShader("res/shaders/skybox.vs", "res/shaders/skybox.frag");
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);
	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("res/images/skybox/hills_rt.tga");
	faces.push_back("res/images/skybox/hills_lf.tga");
	faces.push_back("res/images/skybox/hills_up.tga");
	faces.push_back("res/images/skybox/hills_dn.tga");
	faces.push_back("res/images/skybox/hills_bk.tga");
	faces.push_back("res/images/skybox/hills_ft.tga");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	//laod models
	Car ourModel;
	// Draw in wireframe
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection(1.0f);
	projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check and call events
		glfwPollEvents();
		DoMovement();
		// Clear the colorbuffer
		glClearColor(0.9f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		glm::mat4 view(1.0f);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model	
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.FrontMovement(shader);
		ourModel.BackMovement(shader);
		ourModel.TopGear();
		ourModel.Dvizh();
		ourModel.Draw(shader);
		
		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if ( keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if ( keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if ( keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


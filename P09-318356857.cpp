/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

bool tirar;

float h;
float hOffset;

float rotX2;
float rotX2Offset;

float rotY2;
float rotY2Offset;

float rotX;
float rotY;

int cara_random;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture dadoDiez;

Skybox skybox;

//Materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int caraRandom() {
	srand(time(NULL));
	int cara = rand();
	return (cara % 10);
}



void CrearDado()
{
	unsigned int cubo_indices[] = {

		0, 1, 2,

		3, 4, 5,

		6, 7, 8,

		9, 10, 11,

		12, 13, 14,

		15, 16, 17,

		18, 19, 20,

		21, 22, 23,

		24, 25, 26,

		27, 28, 29
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {

		//x		y		z		S		T			NX		NY		NZ
		0.79f, -0.22f,  3.0f,	0.328f,  0.177f,	0.0f,	0.0f,	-1.0f,	//0
		-2.13f, 1.81f,  0.0f,	0.283f,	0.097f,		0.0f,	0.0f,	-1.0f,	//1
		-2.16f, -2.5f,  0.0f,	0.378f,	0.098f,		0.0f,	0.0f,	-1.0f,	//2

		0.79f, -0.22f,  3.0f,	0.49f,  0.2f,		0.0f,	0.0f,	-1.0f,
		-2.16f, -2.5f,  0.0f,	0.444f,	0.12f,		0.0f,	0.0f,	-1.0f,
		1.92f, -3.85f,  0.0f,	0.538f,	0.12f,		0.0f,	0.0f,	-1.0f,

		0.79f, -0.22f,  3.0f,	0.572f,  0.302f,	0.0f,	0.0f,	-1.0f,
		1.92f, -3.85f,  0.0f,	0.529f,	0.223f,		0.0f,	0.0f,	-1.0f,
		4.48f, -0.39f,  0.0f,	0.623f,	0.223f,		0.0f,	0.0f,	-1.0f,

		0.79f, -0.22f,  3.0f,	0.527f,  0.425f,	0.0f,	0.0f,	-1.0f,
		4.48f, -0.39f,  0.0f,	0.48f,	0.342f,		0.0f,	0.0f,	-1.0f,
		1.99f, 3.0f,	0.0f,	0.571f,	0.342f,		0.0f,	0.0f,	-1.0f,//

		0.79f, -0.22f,  3.0f,	0.374f,  0.477f,	0.0f,	0.0f,	-1.0f,
		1.99f, 3.0f,	0.0f,	0.331f,	0.395f,		0.0f,	0.0f,	-1.0f,
		-2.13f, 1.81f,  0.0f,	0.425f,	0.395f,		0.0f,	0.0f,	-1.0f,

		0.79f, -0.22f,  -3.0f,	0.489f,  0.615f,	0.0f,	0.0f,	1.0f,	//0
		-2.13f, 1.81f,  0.0f,	0.537f,	0.533f,		0.0f,	0.0f,	1.0f,	//1
		-2.16f, -2.5f,  0.0f,	0.443f,	0.533f,		0.0f,	0.0f,	1.0f,	//2

		0.79f, -0.22f,  -3.0f,	0.635f,  0.663f,	0.0f,	0.0f,	1.0f,
		-2.16f, -2.5f,  0.0f,	0.682f,	0.583f,		0.0f,	0.0f,	1.0f,
		1.92f, -3.85f,  0.0f,	0.588f,	0.583f,		0.0f,	0.0f,	1.0f,

		0.79f, -0.22f,  -3.0f,	0.668f,  0.796f,	0.0f,	0.0f,	1.0f,
		1.92f, -3.85f,  0.0f,	0.625f,	0.716f,		0.0f,	0.0f,	1.0f,
		4.48f, -0.39f,  0.0f,	0.719f,	0.716f,		0.0f,	0.0f,	1.0f,

		0.79f, -0.22f,  -3.0f,	0.57f,  0.887f,		0.0f,	0.0f,	1.0f,
		4.48f, -0.39f,  0.0f,	0.618f,	0.805f,		0.0f,	0.0f,	1.0f,
		1.99f, 3.0f,	0.0f,	0.526f,	0.805f,		0.0f,	0.0f,	-1.0f,

		0.79f, -0.22f,  -3.0f,	0.404f,  0.915f,	0.0f,	0.0f,	1.0f,
		1.99f, 3.0f,	0.0f,	0.358f,	0.834f,		0.0f,	0.0f,	1.0f,
		-2.13f, 1.81f,  0.0f,	0.451f,	0.834f,		0.0f,	0.0f,	1.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 240, 30);//30 INDICES
	meshList.push_back(dado);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	
	CrearDado();
	

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	
	//Para el dado 10 caras
	dadoDiez = Texture("Textures/dadoDiez.tga");
	dadoDiez.LoadTextureA();
	




	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	h = 10.0f;
	hOffset = 0.1;

	rotX2 = 0.0f;
	rotX2Offset = 5.0f;

	rotY2 = 0.0f;
	rotY2Offset = 5.0f;

	tirar = true;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		// las posiciones para que determinada cara quede mirando hacia arriba 
		if (cara_random == 1) { //
			rotX = 360;
			rotY = 45;
		}
		else if (cara_random == 2) { //
			rotX = -400;
			rotY = 20;
		}
		else if (cara_random == 3) { //
			rotX = -385;
			rotY = -45;
		}
		else if (cara_random == 4) { //
			rotX = 385;
			rotY = -45;
		}
		else if (cara_random == 5) { //
			rotX = 400;
			rotY = 10;
		}
		else if (cara_random == 6) { //
			rotX = 180;
			rotY = -45;
		}
		else if (cara_random == 7) { //
			rotX = 220;
			rotY = -10;
		}
		else if (cara_random == 8) { //
			rotX = 205;
			rotY = 40;
		}
		else if (cara_random == 9) { //
			rotX = -205;
			rotY = 45;
		}
		else if (cara_random == 0) { //
			rotX = -220;
			rotY = -20;
		}

		// animacion dado
		if (mainWindow.getAnimacionDado()) {
			if (h > 1.5) {
				h -= hOffset * deltaTime;
			}
			if (rotX2 != rotX) {
				if (rotX2 <= rotX) {
					rotX2 += rotX2Offset * deltaTime;
				}
				else {
					rotX2 -= rotX2Offset * deltaTime;
				}
			}
			if (rotY2 != rotY) {
				if (rotY2 < rotY) {
					rotY2 += rotY2Offset * deltaTime;

				}
				else {
					rotY2 -= rotY2Offset * deltaTime;
				}
			}
		}
		else {
			rotX2 = 0.0f;
			rotY2 = 0.0f,
				h = 10.0f;
			cara_random = caraRandom();
		}
		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		

		// Dado de Opengl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, h, -20.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotX2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotY2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoDiez.UseTexture();
		meshList[4]->RenderMesh();

		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
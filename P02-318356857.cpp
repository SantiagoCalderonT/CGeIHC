//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* vShaderVerdeFuerte = "shaders/shaderVerdeFuerte.vert";
static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* vShaderVerdeClaro = "shaders/shaderVerdeClaro.vert";
static const char* vShaderCafe = "shaders/shaderCafe.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X		Y		Z		R		G		B
			/*-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			0.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	0.0f,*/

			//S
			-0.5f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.5f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.7f, 0.3f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.7f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.9f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.9f, -0.1f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.9f, -0.1f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, -0.1f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, 0.5f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.7f, 0.1f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.5f, 0.1f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.5f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.5f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, 0.1f, 0.0f,		0.0f,	0.0f,	1.0f,
				
			-0.7f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.9f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.7f, -0.3f, 0.0f,		0.0f,	0.0f,	1.0f,

			-0.7f, -0.3f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.9f, -0.5f, 0.0f,		0.0f,	0.0f,	1.0f,
			-0.9f, -0.3f, 0.0f,		0.0f,	0.0f,	1.0f,

					
			// C	
			-0.3f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.3f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,

			-0.1f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.3f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,

			-0.1f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			0.2f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, 0.3f, 0.0f,		1.0f,	0.0f,	0.0f,

			0.2f, 0.3f, 0.0f,		1.0f,	0.0f,	0.0f,
			0.2f, 0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, 0.3f, 0.0f,		1.0f,	0.0f,	0.0f,

			-0.1f, -0.3f, 0.0f,		1.0f,	0.0f,	0.0f,
			0.2f, -0.3f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,

			0.2f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,
			0.2f, -0.3f, 0.0f,		1.0f,	0.0f,	0.0f,
			-0.1f, -0.5f, 0.0f,		1.0f,	0.0f,	0.0f,

			// T		
			0.3f, 0.5f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.3f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.9f, 0.5f, 0.0f,		0.0f,	1.0f,	0.0f,

			0.9f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.3f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.9f, 0.5f, 0.0f,		0.0f,	1.0f,	0.0f,

			0.5f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.5f, -0.5f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.7f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f,

			0.7f, -0.5f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.5f, -0.5f, 0.0f,		0.0f,	1.0f,	0.0f,
			0.7f, 0.3f, 0.0f,		0.0f,	1.0f,	0.0f
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,324);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


	GLfloat vertices_trianguloAzul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	};

	MeshColor* trianguloAzul = new MeshColor();
	trianguloAzul->CreateMeshColor(vertices_trianguloAzul, 18);
	meshColorList.push_back(trianguloAzul);


	GLfloat vertices_trianguloVerde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
	};

	MeshColor* trianguloVerde = new MeshColor();
	trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
	meshColorList.push_back(trianguloVerde);


	GLfloat vertices_cuadradoRojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f
	};

	MeshColor* cuadradoRojo = new MeshColor();
	cuadradoRojo->CreateMeshColor(vertices_cuadradoRojo, 36);
	meshColorList.push_back(cuadradoRojo);


	GLfloat vertices_cuadradoVerdeClaro[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	2.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	2.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	2.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	2.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	2.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	2.0f,	0.0f
	};

	MeshColor* cuadradoVerdeClaro = new MeshColor();
	cuadradoVerdeClaro->CreateMeshColor(vertices_cuadradoVerdeClaro, 36);
	meshColorList.push_back(cuadradoVerdeClaro);


	GLfloat vertices_cuadradoCafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};

	MeshColor* cuadradoCafe = new MeshColor();
	cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
	meshColorList.push_back(cuadradoCafe);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); // shader color rojo 
	shader3->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); // shader color verde fuerte
	shader4->CreateFromFiles(vShaderVerdeFuerte, fShaderColor);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); // shader color azul
	shader5->CreateFromFiles(vShaderAzul, fShaderColor);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader(); // shader color cafe
	shader6->CreateFromFiles(vShaderCafe, fShaderColor);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader(); // shader color verde claro
	shader7->CreateFromFiles(vShaderVerdeClaro, fShaderColor);
	shaderList.push_back(*shader7);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[0]->RenderMeshColor();

		// Cubo rojo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glm::mat4 model20(1.0);
		model20 = glm::translate(model20, glm::vec3(0.0f, -0.8f, -4.0f));
		model20 = glm::scale(model20, glm::vec3(3.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model20));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide verde fuerte izquierdo
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glm::mat4 model3(1.0);
		model3 = glm::translate(model3, glm::vec3(-1.73f, -0.8f, -3.0f));
		model3 = glm::scale(model3, glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model3));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Piramide verde fuerte derecho
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glm::mat4 model4(1.0);
		model4 = glm::translate(model4, glm::vec3(1.73f, -0.8f, -3.0f));
		model4 = glm::scale(model4, glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Piramide azul
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glm::mat4 model5(1.0);
		model5 = glm::translate(model5, glm::vec3(0.0f, 1.0f, -3.0f));
		model5 = glm::scale(model5, glm::vec3(2.8f, 1.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model5));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Cubo cafe derecho
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glm::mat4 model6(1.0);
		model6 = glm::translate(model6, glm::vec3(2.3f, -2.0f, -4.0f));
		model6 = glm::scale(model6, glm::vec3(0.6f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model6));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo cafe izquierdo
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glm::mat4 model7(1.0);
		model7 = glm::translate(model7, glm::vec3(-2.3f, -2.0f, -4.0f));
		model7 = glm::scale(model7, glm::vec3(0.6f, 0.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model7));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde claro izquierdo
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glm::mat4 model8(1.0);
		model8 = glm::translate(model8, glm::vec3(-0.2f, -0.05f, -1.0f));
		model8 = glm::scale(model8, glm::vec3(0.25f, 0.25f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model8));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde claro derecho
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glm::mat4 model9(1.0);
		model9 = glm::translate(model9, glm::vec3(0.2f, -0.05f, -1.0f));
		model9 = glm::scale(model9, glm::vec3(0.25f, 0.25f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model9));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		// Cubo verde claro centro
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		glm::mat4 model10(1.0);
		model10 = glm::translate(model10, glm::vec3(0.0f, -0.45f, -1.0f));
		model10 = glm::scale(model10, glm::vec3(0.25f, 0.25f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model10));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

/*
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;
		
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//CUADRADO ROJO
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model2(1.0);
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.5f, -4.0f));
		model2 = glm::scale(model2, glm::vec3(0.8f, 1.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();

		//TRIANGULO AZUL
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model3(1.0);
		model3 = glm::translate(model3, glm::vec3(0.0f, 0.3f, -4.0f));
		model3 = glm::scale(model3, glm::vec3(0.5f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model3));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		//TRIANGULOS VERDES
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model4(1.0);
		model4 = glm::translate(model4, glm::vec3(-0.7f, -0.55f, -4.0f));
		model4 = glm::scale(model4, glm::vec3(0.15f, 0.25f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model5(1.0);
		model5 = glm::translate(model5, glm::vec3(0.7f, -0.55f, -4.0f));
		model5 = glm::scale(model5, glm::vec3(0.15f, 0.25f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model5));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		//CUADRADOS VERDES
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model6(1.0);
		model6 = glm::translate(model6, glm::vec3(0.0f, -0.85f, -1.0f));
		model6 = glm::scale(model6, glm::vec3(0.25f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model6));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model7(1.0);
		model7 = glm::translate(model7, glm::vec3(0.2f, -0.25f, -1.0f));
		model7 = glm::scale(model7, glm::vec3(0.25f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model7));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model8(1.0);
		model8 = glm::translate(model8, glm::vec3(-0.2f, -0.25f, -1.0f));
		model8 = glm::scale(model8, glm::vec3(0.25f, 0.3f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model8));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[6]->RenderMeshColor();

		//CUADRADOS CAFES
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model9(1.0);
		model9 = glm::translate(model9, glm::vec3(-0.7f, -0.9f, -4.0f));
		model9 = glm::scale(model9, glm::vec3(0.15f, 0.2f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model9));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();
		
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glm::mat4 model10(1.0);
		model10 = glm::translate(model10, glm::vec3(0.7f, -0.9f, -4.0f));
		model10 = glm::scale(model10, glm::vec3(0.15f, 0.2f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model10));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();
*/

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
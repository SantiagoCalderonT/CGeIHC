/* Semestre 2024-1
Animación por keyframes
La textura del skybox fue conseguida desde la página https://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
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


//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;


//////////////
Model canastaKF;
Model jugador;
Model pelotaKF;
//////////////

Skybox skybox;

//materiales
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

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false;

//NEW// Keyframes

float posXpelota = 60.5, posYpelota = 10.0f, posZpelota = 29.5;
float movPelota_z = 0.0f, movPelota_y = 0.0f;
float giroPelota = 0;


#define MAX_FRAMES 100
int i_max_steps = 90;
//////////////////////
int i_curr_steps = 30;
//////////////////////
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	
	float movPelota_z;			//Variable para PosicionZ
	float movPelota_y;			//Variable para PosicionY
	float movPelota_zInc;		//Variable para IncrementoZ
	float movPelota_yInc;		//Variable para IncrementoY
	float giroPelota;
	float giroPelotaInc;
	
}FRAME;

FRAME KeyFrame[MAX_FRAMES];

int FrameIndex = 0;		//introducir datos

bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].movPelota_z = movPelota_z;
	KeyFrame[FrameIndex].movPelota_y = movPelota_y;
	KeyFrame[FrameIndex].giroPelota = giroPelota;//completar
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	// Abre el archivo "frames.txt" en modo de adición
	std::ofstream framesFile("frames.txt", std::ios::app);

	if (framesFile.is_open()) {
		// Escribe el *keyframe* actual en el formato especificado
		framesFile << "KeyFrame[" << FrameIndex << "].movPelota_z = " << movPelota_z << ";\n";
		framesFile << "KeyFrame[" << FrameIndex << "].movPelota_y = " << movPelota_y << ";\n";
		framesFile << "KeyFrame[" << FrameIndex << "].giroPelota = " << giroPelota << ";\n\n";

		// Cierra el archivo después de escribir el *keyframe*
		framesFile.close();
	}
	else {
		std::cerr << "Error: No se pudo abrir el archivo frames.txt para guardar el frame.\n";
	}

	FrameIndex++;
}

void resetElements(void) //Tecla 0
{
	
	movPelota_z = KeyFrame[0].movPelota_z;
	movPelota_y = KeyFrame[0].movPelota_y;
	giroPelota = KeyFrame[0].giroPelota;
	
}

void interpolation(void)
{
	KeyFrame[playIndex].movPelota_zInc = (KeyFrame[playIndex + 1].movPelota_z - KeyFrame[playIndex].movPelota_z) / i_max_steps;
	KeyFrame[playIndex].movPelota_yInc = (KeyFrame[playIndex + 1].movPelota_y - KeyFrame[playIndex].movPelota_y) / i_max_steps;
	KeyFrame[playIndex].giroPelotaInc = (KeyFrame[playIndex + 1].giroPelota - KeyFrame[playIndex].giroPelota) / i_max_steps;
}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{
				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movPelota_z += KeyFrame[playIndex].movPelota_zInc;
			movPelota_y += KeyFrame[playIndex].movPelota_yInc;
			giroPelota += KeyFrame[playIndex].giroPelotaInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////



int main()
{
	mainWindow = Window(1280, 1024); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	
	canastaKF = Model();
	canastaKF.LoadModel("Models/canasta sin base.obj");
	jugador = Model();
	jugador.LoadModel("Models/simibasket.obj");
	pelotaKF = Model();
	pelotaKF.LoadModel("Models/pelota basket.obj");
	


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
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


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//////////////////////////////////////////////////////
	glm::vec3 posbasketball = glm::vec3(0.0f, 0.0f, 0.0f);
	//////////////////////////////////////////////////////

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
	///////////////////////////////////

	/*KeyFrame[0].movPelota_z = 0;
	KeyFrame[0].movPelota_y = 0;
	KeyFrame[0].giroPelota = 0;

	KeyFrame[1].movPelota_z = -1;
	KeyFrame[1].movPelota_y = 1;
	KeyFrame[1].giroPelota = 90;

	KeyFrame[2].movPelota_z = -5;
	KeyFrame[2].movPelota_y = 6;
	KeyFrame[2].giroPelota = 630;

	KeyFrame[3].movPelota_z = -8;
	KeyFrame[3].movPelota_y = 7;
	KeyFrame[3].giroPelota = 900;

	KeyFrame[4].movPelota_z = -13;
	KeyFrame[4].movPelota_y = 9;
	KeyFrame[4].giroPelota = 1350;

	KeyFrame[5].movPelota_z = -20;
	KeyFrame[5].movPelota_y = 9;
	KeyFrame[5].giroPelota = 1620;

	KeyFrame[6].movPelota_z = -23;
	KeyFrame[6].movPelota_y = 8;
	KeyFrame[6].giroPelota = 1800;

	KeyFrame[7].movPelota_z = -25;
	KeyFrame[7].movPelota_y = 7;
	KeyFrame[7].giroPelota = 1980;

	KeyFrame[8].movPelota_z = -26;
	KeyFrame[8].movPelota_y = 7;
	KeyFrame[8].giroPelota = 2070;

	KeyFrame[9].movPelota_z = -26;
	KeyFrame[9].movPelota_y = 3;
	KeyFrame[9].giroPelota = 2160;

	KeyFrame[10].movPelota_z = -26;
	KeyFrame[10].movPelota_y = -10;
	KeyFrame[10].giroPelota = 2250;

	KeyFrame[11].movPelota_z = -26;
	KeyFrame[11].movPelota_y = -6;
	KeyFrame[11].giroPelota = 2340;

	KeyFrame[12].movPelota_z = -26;
	KeyFrame[12].movPelota_y = -10;
	KeyFrame[12].giroPelota = 2340;

	KeyFrame[13].movPelota_z = -26;
	KeyFrame[13].movPelota_y = -8;
	KeyFrame[13].giroPelota = 2430;

	KeyFrame[14].movPelota_z = -26;
	KeyFrame[14].movPelota_y = -10;
	KeyFrame[14].giroPelota = 2430;

	KeyFrame[15].movPelota_z = -26;
	KeyFrame[15].movPelota_y = -9;
	KeyFrame[15].giroPelota = 2430;

	KeyFrame[16].movPelota_z = -26;
	KeyFrame[16].movPelota_y = -10;
	KeyFrame[16].giroPelota = 2430;*/

	//////////////////////////////////

	//Se agregan nuevos frames 

	printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en -Z\n6.-Presiona 2 para mover en Z");
	printf("\n7.-Presiona 5 para mover en -Y\n8.-Presiona 6 para mover en +Y");
	printf("\n9.-Presiona 7 para girar\n10.-Presiona M para habilitar mover");


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
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
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		

		// canasta basket
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.3f, -1.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		canastaKF.RenderModel();


		// simi basket
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.75f, -1.0f, 30.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jugador.RenderModel();


		// pelota basket
		model = glm::mat4(1.0);
		posbasketball = glm::vec3(posXpelota, posYpelota + movPelota_y, posZpelota + movPelota_z);
		model = glm::translate(model, posbasketball);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, giroPelota * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pelotaKF.RenderModel();

		


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora\n");
			reproduciranimacion = 0;

		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("\n");
			printf("movPelota_z es: %f\n", movPelota_z);
			printf("movPelota_y es: %f\n", movPelota_y);
			printf("giroPelota es: %f\n", giroPelota);
			printf("\n");
			printf("presiona P para habilitar guardar otro frame\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1 && guardoFrame>0)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L\n");
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			
			movPelota_z -= 1.0f;
			printf("\n movPelota_z es: %f\n", movPelota_z);
			
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla M para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_2])
	{
		if (ciclo < 1)
		{
			
			movPelota_z += 1.0f;
			printf("\n movPelota_z es: %f\n", movPelota_z);
			
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla M para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_M])
	{
		if (ciclo2 < 1 && ciclo>0)
		{
			ciclo = 0;
			printf("\n Ya puedes modificar tu variable\n");
		}
	}

	
	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			
			movPelota_y -= 1.0f;
			printf("\n movPelota_y es: %f\n", movPelota_y);
			
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla M para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			
			movPelota_y += 1.0f;
			printf("\n movPelota_y es: %f\n", movPelota_y);
			
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla M para poder habilitar la variable\n");
		}
	}

	if (keys[GLFW_KEY_7])
	{
		if (ciclo < 1)
		{
			giroPelota += 90.0f;
			printf("\n giroPelota es: %f\n", giroPelota);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla M para poder habilitar la variable\n");
		}

	}

	
}

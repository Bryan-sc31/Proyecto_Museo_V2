/*---------------------------------------------------------*/
/* ----------------  Proyecto Museo             -----------*/
/*-----------------    2024-1   ---------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 50.0f, 700.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

//Variables de posiciones de plataformas
float	movPlataforma1_y = 0.0f,
		orientacion1 = 0.0f,
		orientacion2 = 0.0f,
        orientacion3 = 0.0f;
bool	animacionPlataforma = false;
int estado_plataforma = 0;

//Variables de animacion de AC1
bool animacionAC1 = false;
float OrientaPuertas = 0.0f;//Permite el giro en los ejes determinados de cada parte del carro
float movHood_z = 0.0f;//Permite el desplazamiento del Hood de AC1

//Variables de animacion de AC2
bool animacionAC2 = false;
float OrientaPuertasAC2 = 0.0f;//Permite el giro de las puertas delanteras de AC2
float movDoor_xAC2 = 0.0f;//Permite el desplazamiento de la puerta de AC2 en x
float movDoor_zAC2 = 0.0f;//Permite el desplazamiento de la puerta de AC2 en z
float movDoor_xAC2R = 0.0f;//Permite el desplazamiento de la puerta de AC2 en x
float movDoor_zAC2R = 0.0f;//Permite el desplazamiento de la puerta de AC2 en z

//Variables de animacion de AC3
bool animacionAC3 = false;
float OrientaPuertasAC3 = 0.0f;//Permite el giro de las puertas delanteras de AC3
float movDoorL_xAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en x
float movDoorL_zAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en z
float movDoorR_xAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en x
float movDoorR_zAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en z

//Variable para movimiento de las plataformas F1
float orientacionF1 = 0.0f;

//Variables para movimiento de posiciones de los autos clasicos
float Desplazamiento = 0.0f;
float AC1mov_x = 0.0f;
float AC1mov_z = 0.0f;
float AC2mov_x = 0.0f;
float AC2mov_z = 0.0f;
float AC3mov_x = 0.0f;
float AC3mov_z = 0.0f;

float	movAuto_x = 0.0f,
		movAuto_x2 = 0.0f,
		movAuto_z = 0.0f,
		movAuto_z2 = 0.0f,
		orienta = 0.0f;
bool	animacion = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		giroCarro = 0.0f,
		rotEjeY = 0.0f;
		
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotYInc = 0.0f,
		giroCarroInc = 0.0f;

#define MAX_FRAMES 25
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotEjeY;
	float giroCarro;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 23;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	std::cout << "Pos X = " << posX << std::endl;
	std::cout << "Pos Y = " << posY << std::endl;
	std::cout << "Pos Z = " << posZ << std::endl;
	std::cout << "giroCarro = " << giroCarro << std::endl;
	std::cout << "rotEjeY = " << rotEjeY << std::endl;
	std::cout << "movAutox = " << movAuto_x << std::endl;
	std::cout << "movAutoz = " << movAuto_z << std::endl;
	std::cout << "movAutox2 = " << movAuto_x2 << std::endl;
	std::cout << "orienta = " << orienta << std::endl;
		
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].rotEjeY = rotEjeY;
	KeyFrame[FrameIndex].giroCarro = giroCarro;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	rotEjeY = KeyFrame[0].rotEjeY;
	giroCarro = KeyFrame[0].giroCarro;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	rotYInc = (KeyFrame[playIndex + 1].rotEjeY - KeyFrame[playIndex].rotEjeY) / i_max_steps;
	giroCarroInc = (KeyFrame[playIndex + 1].giroCarro - KeyFrame[playIndex].giroCarro) / i_max_steps;
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;
			rotEjeY += rotYInc;
			giroCarro += giroCarroInc;

			i_curr_steps++;
		}
	}

	//Rota las plataformas del cuarto de autos F1
	orientacionF1 += 1.0f;

	//Animacion de sala de autos modernos
	if (animacionPlataforma)
	{
		if (estado_plataforma == 0){
			movPlataforma1_y += 1.0f;
			
			if (movPlataforma1_y >= 150.0f) {
				estado_plataforma = 1;
			}
				
		}
		
		if (estado_plataforma == 1){
			orientacion1 += 5.0f;
			orientacion2 += 5.0f;
			orientacion3 += 5.0f;
			
			if (orientacion1 >= 585.0f) {
				orientacion1 = 585.0f;
			}
			if (orientacion2 >= 500.0f) {
				orientacion2 = 500.0f;
			}
			if (orientacion3 >= 405.0f) {
				orientacion3 = 405.0f;
			}
			if ((orientacion1==585.0f) && (orientacion2 == 500.0f)&& (orientacion3 == 405.0f)) {
				animacionPlataforma = false;
			}
		}
	}
	//--------------------------------------------------------------
	// Animaciones de sala de autos clasicos
	//--------------------------------------------------------------
	if (animacionAC1)
	{
		OrientaPuertas += 1.0f;
		movHood_z += 0.3f;

		if (OrientaPuertas >= 40.0f && movHood_z >= 12.0f) {
			animacionAC1 = false;
		}
	}

	if (animacionAC2)
	{
		OrientaPuertasAC2 += 1.0f;
		movDoor_xAC2 += 0.18f;//0.225
		movDoor_zAC2 += 0.16f;//0.2
		movDoor_xAC2R += 0.04f;//0.05
		movDoor_zAC2R += 0.24f;//0.3
		//40-9-8-2-12
		if (OrientaPuertasAC2 >= 40.0f && movDoor_xAC2 >= 7.2f && movDoor_zAC2 >= 6.4f && movDoor_xAC2R >= 1.6f && movDoor_zAC2R >= 9.6f) {
			animacionAC2 = false;
		}
	}

	if (animacionAC3)
	{
		OrientaPuertasAC3 += 1.0f;
		movDoorL_xAC3 += 0.1064f;//0.133
		movDoorL_zAC3 += 0.1064f;//0.133
		movDoorR_xAC3 += 0.24f;//0.3
		movDoorR_zAC3 += 0.2928f;//0.366
		//60-8-8-18-22
		if (OrientaPuertasAC3 >= 60.0f && movDoorL_xAC3 >= 6.4f && movDoorL_zAC3 >= 6.4f && movDoorR_xAC3 >= 14.4f && movDoorR_zAC3 >= 17.6f) {
			animacionAC3 = false;
		}
	}
	//--------------------------------------------------------------
	// Final animaciones de sala de autos clasicos
	//--------------------------------------------------------------
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Museo - CGeIHC 20241", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right2.jpg",
		"resources/skybox/left2.jpg",
		"resources/skybox/top2.jpg",
		"resources/skybox/bottom2.jpg",
		"resources/skybox/front2.jpg",
		"resources/skybox/back2.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// -------------------------------------------------------------------------------------------------------------------------
	// Carga de modelos
	// -------------------------------------------------------------------------------------------------------------------------
	Model piso("resources/objects/piso/piso2.obj");
		
	Model MuseoPiso("resources/objects/Museo/Piso.obj");
	Model MuseoFrente("resources/objects/Museo/Frente.obj");
	Model MuseoLetre("resources/objects/Museo/Letrero.obj");
	Model MuseoIzq("resources/objects/Museo/Izquierda.obj");
	Model MuseoDer("resources/objects/Museo/Derecha.obj");
	Model MuseoTrasera("resources/objects/Museo/Trasera.obj");
	Model MuseoEntrada("resources/objects/Museo/LlantaF.obj");
	Model MuseoRAC("resources/objects/Museo/RoomAC.obj");
	Model MuseoRAF1("resources/objects/Museo/RoomAF1.obj");
	Model MuseoRAM("resources/objects/Museo/RoomAM.obj");
	Model Plataforma("resources/objects/Plataforma/Plataforma.obj");
	
	Model PistaCircular("resources/objects/PistaCircular/PistaCircular.obj");
	Model PistaCompleja("resources/objects/PistaCompleja/PistaCompleja.obj");
	
	//Visitantes 
	ModelAnim josh("resources/objects/visitanteJosh/visitanteJosh.dae");
	josh.initShaders(animShader.ID);
	ModelAnim megan("resources/objects/visitanteMegan/visitanteMegan.dae");
	megan.initShaders(animShader.ID);
	
	//Autos clasicos
	Model AC1("resources/objects/AutosClasicos/33HotRod/Body.obj");
	Model AC1DoorL("resources/objects/AutosClasicos/33HotRod/DoorL.obj");
	Model AC1DoorR("resources/objects/AutosClasicos/33HotRod/DoorR.obj");
	Model AC1Hood("resources/objects/AutosClasicos/33HotRod/Hood.obj");
	Model AC1WheelFL("resources/objects/AutosClasicos/33HotRod/WheelFL.obj");
	Model AC1WheelRL("resources/objects/AutosClasicos/33HotRod/WheelRL.obj");
	Model AC2("resources/objects/AutosClasicos/80S/Body.obj");
	Model AC2DoorFL("resources/objects/AutosClasicos/80S/DoorFL.obj");
	Model AC2DoorFR("resources/objects/AutosClasicos/80S/DoorFR.obj");
	Model AC2DoorRL("resources/objects/AutosClasicos/80S/DoorRL.obj");
	Model AC2DoorRR("resources/objects/AutosClasicos/80S/DoorRR.obj");
	Model AC2Wheel("resources/objects/AutosClasicos/80S/Wheel.obj");
	Model AC3("resources/objects/AutosClasicos/SportCar57/Body.obj");
	Model AC3DoorL("resources/objects/AutosClasicos/SportCar57/DoorL.obj");
	Model AC3DoorR("resources/objects/AutosClasicos/SportCar57/DoorR.obj");
	Model AC3Hood("resources/objects/AutosClasicos/SportCar57/Hood.obj");
	Model AC3WheelFR("resources/objects/AutosClasicos/SportCar57/WheelFR.obj");
	Model AC3WheelRR("resources/objects/AutosClasicos/SportCar57/WheelRR.obj");
	
	//Autos F1
	Model AF1("resources/objects/AutosF1/Generic/Body.obj");
	Model AF1WheelFR("resources/objects/AutosF1/Generic/Wheel_FR.obj");
	Model AF1WheelRR("resources/objects/AutosF1/Generic/Wheel_RR.obj");
	Model AF2("resources/objects/AutosF1/Red-bull/Body.obj");
	Model AF2WheelFR("resources/objects/AutosF1/Red-bull/Wheel_FR.obj");
	Model AF2WheelRR("resources/objects/AutosF1/Red-bull/Wheel_RR.obj");
	Model AF3("resources/objects/AutosF1/Renault/Body.obj");
	
	//Autos modernos
	Model AM1("resources/objects/AutosModernos/FordGT/Body.obj");
	Model AM1WheelFR("resources/objects/AutosModernos/FordGT/Wheel_FR.obj");
	Model AM1WheelRR("resources/objects/AutosModernos/FordGT/Wheel_RR.obj");
	Model AM2("resources/objects/AutosModernos/Koenigs/Koenigs.obj");
	Model AM2Wheel("resources/objects/AutosModernos/Koenigs/WheelKoenig.obj");
	Model AM3("resources/objects/AutosModernos/Speciale95/Body.obj");
	Model AM3WheelL("resources/objects/AutosModernos/Speciale95/Wheel_L.obj");
	
	// -------------------------------------------------------------------------------------------------------------------------
	// Precarga de animaciones por KeyFrames
	// -------------------------------------------------------------------------------------------------------------------------

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotEjeY = 0;
		KeyFrame[i].giroCarro = 0;
	}
	
	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].giroCarro = 0.0f;
	KeyFrame[0].rotEjeY = 0.0f;

	KeyFrame[1].posX = 69.0f;
	KeyFrame[1].posY = 0.0f;
	KeyFrame[1].posZ = -12.0f;
	KeyFrame[1].giroCarro = 13.0f;
	KeyFrame[1].rotEjeY = 7.0f;
	
	KeyFrame[2].posX = 127.0f;
	KeyFrame[2].posY = 0.0f;
	KeyFrame[2].posZ = -55.0f;
	KeyFrame[2].giroCarro = 41.0f;
	KeyFrame[2].rotEjeY = -1.0f;

	KeyFrame[3].posX = 220.0f;
	KeyFrame[3].posY = 0.0f;
	KeyFrame[3].posZ = -113.0f;
	KeyFrame[3].giroCarro = 41.0f;
	KeyFrame[3].rotEjeY = -1.0f;

	KeyFrame[4].posX = 393.0f;
	KeyFrame[4].posY = 16.0f;
	KeyFrame[4].posZ = -160.0f;
	KeyFrame[4].giroCarro = 10.0f;
	KeyFrame[4].rotEjeY = -5.0f;
		
	KeyFrame[5].posX = 465.0f;
	KeyFrame[5].posY = 26.0f;
	KeyFrame[5].posZ = -142.0f;
	KeyFrame[5].giroCarro = 10.0f;
	KeyFrame[5].rotEjeY = -5.0f;

	KeyFrame[6].posX = 597.0f;
	KeyFrame[6].posY = 58.0f;
	KeyFrame[6].posZ = -86.0f;
	KeyFrame[6].giroCarro = -24.0f;
	KeyFrame[6].rotEjeY = 2.0f;
	
	KeyFrame[7].posX = 705.002f;
	KeyFrame[7].posY = 52.9999f;
	KeyFrame[7].posZ = -8.00008f;
	KeyFrame[7].giroCarro = -44.0f;
	KeyFrame[7].rotEjeY = 2.0f;
	
	KeyFrame[8].posX = 851.002f;
	KeyFrame[8].posY = 0.999928f;
	KeyFrame[8].posZ = -10.0001f;
	KeyFrame[8].giroCarro = 11.0f;
	KeyFrame[8].rotEjeY = 2.0f;

	KeyFrame[9].posX = 915.002f;
	KeyFrame[9].posY = -1.00007f;
	KeyFrame[9].posZ = -47.0001f;
	KeyFrame[9].giroCarro = 65.0f;
	KeyFrame[9].rotEjeY = 10.0f;

	KeyFrame[10].posX = 970.002f;
	KeyFrame[10].posY = -1.00007f;
	KeyFrame[10].posZ = -129.0f;
	KeyFrame[10].giroCarro = 86.0f;
	KeyFrame[10].rotEjeY = 10.0f;

	KeyFrame[11].posX = 937.002f;
	KeyFrame[11].posY = -1.00007f;
	KeyFrame[11].posZ = -309.0f;
	KeyFrame[11].giroCarro = 148.0f;
	KeyFrame[11].rotEjeY = 10.0f;

	KeyFrame[12].posX = 727.002f;
	KeyFrame[12].posY = 1.99993f;
	KeyFrame[12].posZ = -422.0f;
	KeyFrame[12].giroCarro = 168.0f;
	KeyFrame[12].rotEjeY = 2.0f;

	KeyFrame[13].posX = 605.002f;
	KeyFrame[13].posY = 22.9999f;
	KeyFrame[13].posZ = -424.0f;
	KeyFrame[13].giroCarro = 168.0f;
	KeyFrame[13].rotEjeY = 12.0f;

	KeyFrame[14].posX = 375.002f;
	KeyFrame[14].posY = 58.9999f;
	KeyFrame[14].posZ = -311.0f;
	KeyFrame[14].giroCarro = 214.0f;
	KeyFrame[14].rotEjeY = 2.0f;
	
	KeyFrame[15].posX = 160.002f;
	KeyFrame[15].posY = -7.24792e-05f;
	KeyFrame[15].posZ = -299.0f;
	KeyFrame[15].giroCarro = 147.0f;
	KeyFrame[15].rotEjeY = 2.0f;
	
	KeyFrame[16].posX = 86.0016f;
	KeyFrame[16].posY = -7.24792e-05f;
	KeyFrame[16].posZ = -354.0f;
	KeyFrame[16].giroCarro = 148.0f;
	KeyFrame[16].rotEjeY = 7.0f;

	KeyFrame[17].posX = 26.0069f;
	KeyFrame[17].posY = -0.000250578f;
	KeyFrame[17].posZ = -367.998f;
	KeyFrame[17].giroCarro = 206.999f;
	KeyFrame[17].rotEjeY = 7.00001f;

	KeyFrame[18].posX = -146.993f;
	KeyFrame[18].posY = -0.000250578f;
	KeyFrame[18].posZ = -309.998f;
	KeyFrame[18].giroCarro = 272.999f;
	KeyFrame[18].rotEjeY = -3.99999f;

	KeyFrame[19].posX = -202.993f;
	KeyFrame[19].posY = -0.000250578f;
	KeyFrame[19].posZ = -178.998f;
	KeyFrame[19].giroCarro = 308.999f;
	KeyFrame[19].rotEjeY = -2.99999f;

	KeyFrame[20].posX = -184.993f;
	KeyFrame[20].posY = -0.000250578f;
	KeyFrame[20].posZ = -104.998f;
	KeyFrame[20].giroCarro = 319.999f;
	KeyFrame[20].rotEjeY = -0.999992f;

	KeyFrame[21].posX = -107.993f;
	KeyFrame[21].posY = -0.000250578f;
	KeyFrame[21].posZ = -20.9984f;
	KeyFrame[21].giroCarro = 334.999f;
	KeyFrame[21].rotEjeY = 3.00001f;

	KeyFrame[22].posX = -11.9931f;
	KeyFrame[22].posY = -0.000250578f;
	KeyFrame[22].posZ = 4.00159f;
	KeyFrame[22].giroCarro = 360.999f;
	KeyFrame[22].rotEjeY = 3.00001f;
	// -------------------------------------------------------------------------------------------------------------------------
	// Fin de precarga de animaciones por KeyFrames
	// -------------------------------------------------------------------------------------------------------------------------

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);//lightDirection
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.6f, 0.6f, 0.6f));//ambientColor
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));//diffuseColor
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(-230.0f, 20.0f, 180.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(20.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(90.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0014f);
		staticShader.setFloat("spotLight[0].quadratic", 0.000007f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personajes Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);
		
		// Primer personaje Animacion
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-420.0f, 1.75f, -610.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		animShader.setMat4("model", model);
		josh.Draw(animShader);

		// Segundo personaje Animacion
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-440.0f, 1.75f, -610.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		animShader.setMat4("model", model);
		megan.Draw(animShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.35f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Estructura de museo
		// -------------------------------------------------------------------------------------------------------------------------
		//Piso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		MuseoPiso.Draw(staticShader);
		
		//Pared frente
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoFrente.Draw(staticShader);

		//Letrero de museo
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoLetre.Draw(staticShader);

		//Entrada de media llanta
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoEntrada.Draw(staticShader);

		//Pared izquierda
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoIzq.Draw(staticShader);

		//Pared derecha
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoDer.Draw(staticShader);
		
		//Pared trasera
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoTrasera.Draw(staticShader);
		
		//Cuarto carros clasicos
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoRAC.Draw(staticShader);
		
		//Cuarto carros F1
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoRAF1.Draw(staticShader);
		
		//Cuarto carros modernos
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		MuseoRAM.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin de la estructura de museo
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto carros clasicos
		// -------------------------------------------------------------------------------------------------------------------------
		//Funcion de comportamiento de desplazamiento de los autos clasicos
		AC1mov_x = 90.0f * sin(Desplazamiento + 0.0f);
		AC1mov_z = 90.0f * cos(Desplazamiento + 0.0f);
		AC2mov_x = 90.0f * sin(Desplazamiento + 90.0f);
		AC2mov_z = 90.0f * cos(Desplazamiento + 90.0f);
		AC3mov_x = 90.0f * sin(Desplazamiento + 180.0f);
		AC3mov_z = 90.0f * cos(Desplazamiento + 180.0f);

		//HotRod
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-90.0f + AC1mov_x, 0.0f, -245.0f + AC1mov_z));
		tmp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(OrientaPuertas), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1DoorL.Draw(staticShader);
		
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-OrientaPuertas), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1DoorR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f+ movHood_z));
		model = glm::rotate(model, glm::radians(-OrientaPuertas), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1Hood.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1WheelFL.Draw(staticShader);//Delantera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1WheelRL.Draw(staticShader);//Trasera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 46.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1WheelFL.Draw(staticShader);//Delantera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -51.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		AC1WheelRL.Draw(staticShader);//Trasera izquierda

		//80S
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-90.0f+ AC2mov_x, 0.0f, -245.0f + AC2mov_z));
		tmp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f - movDoor_xAC2, 0.0f, 0.0f - movDoor_zAC2));
		model = glm::rotate(model, glm::radians(OrientaPuertasAC2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2DoorFL.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f + movDoor_xAC2, 0.0f, 0.0f - movDoor_zAC2));
		model = glm::rotate(model, glm::radians(-OrientaPuertasAC2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2DoorFR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f + movDoor_xAC2R, 0.0f, 0.0f - movDoor_zAC2R));
		model = glm::rotate(model, glm::radians(OrientaPuertasAC2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2DoorRL.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f - movDoor_xAC2R, 0.0f, 0.0f - movDoor_zAC2R));
		model = glm::rotate(model, glm::radians(-OrientaPuertasAC2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2DoorRR.Draw(staticShader);
		
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2Wheel.Draw(staticShader);//Delantera derecha

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -37.6f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2Wheel.Draw(staticShader);//Trasera derecha

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -6.4f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2Wheel.Draw(staticShader);//Trasera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f , 0.0f, 30.4f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		AC2Wheel.Draw(staticShader);//Delantera izquierda
		
		//Sport57
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-90.0f + AC3mov_x, 0.0f, -245.0f + AC3mov_z));
		tmp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f - movDoorL_xAC3, 0.0f, 0.0f - movDoorL_zAC3));
		model = glm::rotate(model, glm::radians(OrientaPuertasAC3), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3DoorL.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f + movDoorR_xAC3, 0.0f, 0.0f - movDoorR_zAC3));
		model = glm::rotate(model, glm::radians(-OrientaPuertasAC3), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3DoorR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3Hood.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3WheelFR.Draw(staticShader);//Delantera derecha

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3WheelRR.Draw(staticShader);//Trasera derecha

		model = glm::translate(tmp, glm::vec3(10.8f, 0.0f, 39.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3WheelFR.Draw(staticShader);//Delantera Izquierda

		model = glm::translate(tmp, glm::vec3(10.8f, 0.0f, -39.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AC3WheelRR.Draw(staticShader);//Trasera izquierda

		//Pista
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-86.0f, 2.0f, -261.0f));
		model = glm::scale(model, glm::vec3(4.0f, 2.0f, 4.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		PistaCircular.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin cuarto carros Clasicos
		// -------------------------------------------------------------------------------------------------------------------------
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto carros F1
		// -------------------------------------------------------------------------------------------------------------------------
		//Plataforma Generic
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(308.0f, 0.0f, 186.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacionF1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);
				
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 4.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		AF1.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 4.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		AF1WheelFR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 4.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		AF1WheelRR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-1.0f, 12.0f, 44.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		AF1WheelFR.Draw(staticShader); //Delantera izquierda

		model = glm::translate(tmp, glm::vec3(-1.0f, 12.0f, 0.0f + -76.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		AF1WheelRR.Draw(staticShader); //Trasera izquierda

		//Plataforma Red-bull
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(250.0f, 0.0f, 50.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacionF1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);
		
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		AF2.Draw(staticShader);
		
		//Plataforma Renault
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(350.0f, 0.0f, 50.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacionF1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);
				
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		AF3.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin cuarto carros F1
		// -------------------------------------------------------------------------------------------------------------------------
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto carros Modernos
		// -------------------------------------------------------------------------------------------------------------------------
		//Plataforma FordGT
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-230.0f, -150.0f+movPlataforma1_y, 180.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacion1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);

		//FordGT
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		AM1.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		AM1WheelFR.Draw(staticShader);//Delantera derecha

		model = glm::translate(tmp, glm::vec3(-1.0f, 12.0f, -2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		AM1WheelRR.Draw(staticShader);//Delantera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		AM1WheelRR.Draw(staticShader);//Trasera drecha

		model = glm::translate(tmp, glm::vec3(-1.0f, 12.0f, -2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setMat4("model", model);
		AM1WheelFR.Draw(staticShader);//Trasera izquierda
					
		//Plataforma Koenigs
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-380.0f, -150.0f + movPlataforma1_y, 230.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacion2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);

		//Koenigs
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM2.Draw(staticShader);
		
		model = glm::translate(tmp, glm::vec3(-0.5f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Derecha delantera

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 22.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Izquierda delantera

		model = glm::translate(tmp, glm::vec3(-0.5f, 12.0f, -22.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Derecha trasera

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, -0.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Izquierda trasera

		//Plataforma Speciale95
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-380.0f, -150.0f + movPlataforma1_y, 40.0f));
		tmp = model = glm::rotate(model, glm::radians(orientacion3), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		Plataforma.Draw(staticShader);

		//Speciale95
		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM3.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM3WheelL.Draw(staticShader);//Delantera derecha

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, 22.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		AM3WheelL.Draw(staticShader);//Delantera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, -24.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		AM3WheelL.Draw(staticShader);//Trasera derecha

		model = glm::translate(tmp, glm::vec3(0.0f, 12.0f, -1.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		AM3WheelL.Draw(staticShader);//Trasera izquierda
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin cuarto carros Modernos
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Pista de carreras
		// -------------------------------------------------------------------------------------------------------------------------
		//Pista
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -760.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(9.0f, 5.0f, 9.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		PistaCompleja.Draw(staticShader);
		
		//Auto1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-390.0f+posX, 2.0f+posY, -574.0f+posZ));
		tmp = model = glm::rotate(model, glm::radians(90.0f+ giroCarro), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AM1.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotEjeY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AM1WheelFR.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-1.0f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEjeY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AM1WheelRR.Draw(staticShader);//Delantera izquierda

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AM1WheelRR.Draw(staticShader);//Trasera drecha

		model = glm::translate(tmp, glm::vec3(-1.0f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		AM1WheelFR.Draw(staticShader);//Trasera izquierda

		//Auto 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(-450.0f + posX, 2.0f + posY, -574.0f + +posZ));
		tmp = model = glm::rotate(model, glm::radians(90.0f + giroCarro), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		AM2.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(-0.75f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotEjeY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Derecha delantera

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 33.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotEjeY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Izquierda delantera

		model = glm::translate(tmp, glm::vec3(-0.75f, 0.0f, -33.75f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Derecha trasera

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -0.75f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		AM2Wheel.Draw(staticShader);//Izquierda trasera
		// -------------------------------------------------------------------------------------------------------------------------
		// Fin pista carreras
		// -------------------------------------------------------------------------------------------------------------------------
				
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		//-------------------------------------------------------------------------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Inicia la animacion de la sala de autos modernos
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animacionPlataforma ^= true;
	}
	
	//Reinicia la animacion de la sala de autos modernos
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		movPlataforma1_y = 0.0f;
		orientacion1 = 0.0f;
		orientacion2 = 0.0f;
		orientacion3 = 0.0f; 
		estado_plataforma = 0;
		animacionPlataforma = false;
	}
	
	//Inicia la animacion para abrir AC1
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		animacionAC1 ^= true;
	}
	
	//Reinicia la animacion de AC1
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		OrientaPuertas = 0.0f;
		movHood_z = 0.0f;
		animacionAC1 = false;
	}
	
	//Inicia la animacion para abrir AC2
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		animacionAC2 ^= true;
	}
	
	//Reinicia la animacion de AC2
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		OrientaPuertasAC2 = 0.0f;
		movDoor_xAC2 = 0.0f;
		movDoor_zAC2 = 0.0f;
		movDoor_xAC2R = 0.0f;
		movDoor_zAC2R = 0.0f;
		animacionAC2 = false;
	}

	//Inicia la animacion para abrir AC3
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		animacionAC3 ^= true;
	}

	//Reinicia la animacion de AC3
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		OrientaPuertasAC3 = 0.0f;//Permite el giro de las puertas delanteras de AC3
		movDoorL_xAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en x
		movDoorL_zAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en z
		movDoorR_xAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en x
		movDoorR_zAC3 = 0.0f;//Permite el desplazamiento de la puerta de AC3 en z
		animacionAC3 = false;
	}

	//Permiten el desplazamiento de los carros la sala de autos clasicos 
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Desplazamiento += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Desplazamiento -= 1.0f;
	
	//Inicia la animacion por KeyFrame de la carrera
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
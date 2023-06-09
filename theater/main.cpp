﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include<SOUND/irrKlang.h>

#include <iostream>
#include <vector>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

float X = 0.0f;
float Y = 0.0f;
float Z = 0.0f;
int frameAmt = 0;
bool render[] = { true,false,false };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//screen position and texture coords
float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float lightController = X;
glm::vec3 lightInten = glm::vec3(0.95f, 0.95f, 0.95f);
glm::vec3 lightOff = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 pointLightPositions[] = { glm::vec3(0.0f, 14.00f, 0.0f),glm::vec3(3.0f, 14.00f, 0.0f),glm::vec3(6.0f, 14.00f, 0.0f),glm::vec3(9.0f, 14.00f, 0.0f) };

void drawChairs(Shader shader, glm::mat4 model, Model object);
void drawCarpet(Shader shader, glm::mat4 model, Model object);
void drawLights(Shader shader, glm::mat4 model, Model object);
void drawStairs(Shader shader, glm::mat4 model, vector<Model> objects);
void drawMisc(Shader shader, glm::mat4 model, vector<Model> objects);
void drawWall(Shader shader, glm::mat4 model, Model object);
void drawCeiling(Shader shader, glm::mat4 model, Model object);
int getFPS(string name) {
	vector<string> nums;


	string tempCommand = "ffprobe -v error -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=r_frame_rate " + name + " > fps.txt";
	const char* command = tempCommand.c_str();
	//get fps and save to file
	system(command);



	//get fps and save to file

	//read from file
	std::ifstream myfile("fps.txt");
	string rawFPS;

	if (myfile.is_open()) { // always check whether the file is open
		myfile >> rawFPS; // pipe file's content into stream
	}

	
	string delimiter = "/";
	string token;
	size_t pos = 0;
	while ((pos = rawFPS.find(delimiter)) != std::string::npos) {
		token = rawFPS.substr(0, pos);
		rawFPS.erase(0, pos + delimiter.length());
		nums.push_back(token);
	}
	nums.push_back(rawFPS);

	double x1 = stod(nums[0]);
	double x2 = stod(nums[1]);
	
	
	double fps = x1 / x2;
	fps = round(fps);

	return fps;
}

void getFrames(string name) {
	//extracts frames
	string tempCommand = "ffmpeg -i " + name + " -vf scale=720:-1  %1d.jpg";
	const char* command = tempCommand.c_str();
	system(command);

	//extract audio
	 tempCommand = "ffmpeg -i " + name + " -q:a 0 -map a audio.mp3";
	command = tempCommand.c_str();
	system(command);
}

void delFrames() {
	system("del *jpg");
	
}
void delAudio() {
system("del *mp3");
}

double getLength(string name) {
	string tempCommand = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 " + name + " > length.txt";
	const char* command = tempCommand.c_str();
	system(command);

	//read from file
	std::ifstream myfile("length.txt");
	string temp;

	if (myfile.is_open()) { // always check whether the file is open
		myfile >> temp; // pipe file's content into stream
	}
	double length = stod(temp);
	return length;
}

int main()
{	
	//clear irrklang startup text
	system("clear");
	//get video
	string name = "";
	cout << "What video do you want to watch?";
	cin >> name;
	getFrames(name);
	int FPS = getFPS(name);
	double length = getLength(name);
	frameAmt = FPS * length;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader lightingShader("shaders/5.4.light_casters.vs", "shaders/5.4.light_casters.fs");
	Shader screenShader("shaders/screen.vs", "shaders/screen.fs");




	// load models
	// -----------
	Model chair("models/chair/theaterChair.obj");
	Model carpet("models/carpet/carpet.obj");
	Model stairNose("models/stairNose/stairNose.obj");
	Model railing("models/railing/metalRailing.obj");
	Model light("models/light/light.obj");
	//Model handRail("models/railing/handrail.obj");
	//Model handRail_Mirror("models/railing/handrail_mirror.obj");
	Model wall("models/walls/walls.obj");
	Model door("models/door/door.obj");
	Model soda("models/soda/soda.obj");
	Model popcorn("models/popcorn/popcorn.obj");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------

	//vector of objects for our misc function
	vector<Model> miscObjects = {railing,door,soda,popcorn};
	vector<Model> stairObjects = { carpet,stairNose };


	//screen VBO/VAO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	

	screenShader.use();
	screenShader.setInt("texture1", 0);
	int minFrameTime = 1 / getFPS(name);
	int count = 1;
	double lastTime = glfwGetTime();

	//start audio right before render
	SoundEngine->play2D("audio.mp3", true);

	while (!glfwWindowShouldClose(window))
	{
		if (count > frameAmt) {
			count = 1;
		}

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		//lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		if(render[0])lightingShader.setVec3("pointLights[0].ambient", lightInten);
		else {
			lightingShader.setVec3("pointLights[0].ambient", lightOff);
		}
		lightingShader.setVec3("pointLights[0].diffuse", 1.0f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		if (render[0])lightingShader.setVec3("pointLights[1].ambient", lightInten);
		else {
			lightingShader.setVec3("pointLights[1].ambient", lightOff);
		}
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		if (render[0])lightingShader.setVec3("pointLights[2].ambient", lightInten);
		else {
			lightingShader.setVec3("pointLights[2].ambient", lightOff);
		}
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		if (render[0])lightingShader.setVec3("pointLights[3].ambient", lightInten);
		else {
			lightingShader.setVec3("pointLights[3].ambient", lightOff);
		}
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);





		if (render[1]) {
			//flashlight
			lightingShader.setVec3("spotLight.position", camera.Position);
			lightingShader.setVec3("spotLight.direction", camera.Front);
			lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight.constant", 1.0f);
			lightingShader.setFloat("spotLight.linear", 0.09f);
			lightingShader.setFloat("spotLight.quadratic", 0.032f);
			lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		else {
			//flashlight
			lightingShader.setVec3("spotLight.position", 100.0f, 100.0f, 100.0f);
			lightingShader.setVec3("spotLight.direction", camera.Front);
			lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight.constant", 1.0f);
			lightingShader.setFloat("spotLight.linear", 0.09f);
			lightingShader.setFloat("spotLight.quadratic", 0.032f);
			lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
		}

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);




		drawChairs(lightingShader, model, chair);
		drawCarpet(lightingShader, model, carpet);
		drawStairs(lightingShader, model, stairObjects);
		drawMisc(lightingShader, model, miscObjects);
		drawLights(lightingShader, model, light);
		drawWall(lightingShader, model, wall);
		drawCeiling(lightingShader, model, wall);




		string tempFrame = to_string(count);
		tempFrame = tempFrame + ".jpg";
		const char* frame = tempFrame.c_str();

		unsigned int texture1;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load(frame, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << " " << count << std::endl;
		}
		stbi_image_free(data);






		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		screenShader.use();
		model = glm::scale(model, glm::vec3(2.0f,1.0f,1.0f));
		model = glm::translate(model, glm::vec3(0.74f, 1.57f, 1.82f));
		screenShader.setMat4("model", model);
		screenShader.setMat4("projection", projection);
		screenShader.setMat4("view", view);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		count++;

		while (glfwGetTime() < lastTime + 1.0 / FPS) {
			//do nothing
		}
		lastTime += 1.0 / FPS;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	delFrames();
	delAudio();
	return 0;
}


void drawChairs(Shader shader, glm::mat4 model, Model object) {
	shader.use();
	// render the loaded model
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));

	//first layer
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//reset and draw second row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	
	model = glm::translate(model, glm::vec3(0.0f, 0.35f, -0.80f)); 
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//third row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.0f, 0.70f, -1.60f));
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//4th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.0f, 1.05f, -2.40f));
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//5th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.0f, 1.40f, -3.20f));
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//6th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.0f, 1.75f, -4.00f));
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//7th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.0f, 2.10f, -4.80f));
	for (int i = 0; i < 10; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

}

void drawCarpet(Shader shader, glm::mat4 model, Model object) {
	shader.use();
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);
	object.Draw(shader);


	//draws carpets along the length of the first row of chairs
	for (int i = 0; i < 7; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0045f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 0.001, 181.0f));
	shader.setMat4("model", model);
	object.Draw(shader);


	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//2nd row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 40.68f, -187.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//3rd row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 81.14f, -374.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//4th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 121.6f, -561.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}
	//5th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 162.06f, -748.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//6th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 202.52f, -935.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//7th row
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0033f, 0.003f, 0.0015f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 242.98f, -1122.0f));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}

	//top floor
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down
	model = glm::translate(model, glm::vec3(0.0f, 242.98f, -732.0f));
	//model = glm::translate(model, glm::vec3(X, Y, Z));
	shader.setMat4("model", model);
	object.Draw(shader);
	for (int i = 0; i < 6; i++) {
		model = glm::translate(model, glm::vec3(159.813f, 0, 0));
		shader.setMat4("model", model);
		object.Draw(shader);
	}
}

void drawLights(Shader shader, glm::mat4 model, Model object) {
	shader.use();
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	model = glm::translate(model, glm::vec3(0.0f, 14.62f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	for (int i = 0; i < 4; i++) {
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		object.Draw(shader);
	}


}

void drawStairs(Shader shader, glm::mat4 model, vector<Model> objects) {
	//objects[0] = carpet
	//objects[1] = stairNose
	//drawing the stairs
	shader.use();
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.003f, 0.001f));
	model = glm::translate(model, glm::vec3(69.0f, -36.0f, -7.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);


	model = glm::translate(model, glm::vec3(0, -94.0f, -121.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.003f, 0.001f));
	model = glm::translate(model, glm::vec3(69.0f, -224.0f, -252.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.003f, 0.001f));
	model = glm::translate(model, glm::vec3(69.0f, -317.0f, -370.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.003f, 0.001f));
	model = glm::translate(model, glm::vec3(69.0f, -411.0f, -494.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.023f, 0.003f, 0.001f));
	model = glm::translate(model, glm::vec3(69.0f, -504.5f, -616.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);


	//stair noses
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 116.0f, -110.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 239.0f, -389.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 367.0f, -669.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 484.0f, -948.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 606.0f, -1232.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.035f, 0.001f, 0.001f));
	model = glm::translate(model, glm::vec3(42, 728.0f, -1511.0f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);
}

void drawMisc(Shader shader, glm::mat4 model, vector<Model> objects) {
	//draw guard up top
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.0036f, 0.003f, 0.003f));
	model = glm::translate(model, glm::vec3(406, 243.0f, -616.0f));
	shader.setMat4("model", model);
	objects[0].Draw(shader);

	////handrail
	//model = glm::mat4(1.0f);
	////model = glm::scale(model, glm::vec3(0.0038f, 0.003f, 0.003f));
	////model = glm::translate(model, glm::vec3(406, 243.0f, -616.0f));
	//shader.setMat4("model", model);
	//objects[1].Draw(shader);

	////handrail Mirrored
	//model = glm::mat4(1.0f);
	////model = glm::scale(model, glm::vec3(0.0038f, 0.003f, 0.003f));
	//model = glm::translate(model, glm::vec3(X, Y, Z));
	//shader.setMat4("model", model);
	//objects[2].Draw(shader);

	//door
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::translate(model, glm::vec3(15.2f, 7.4f, -25.5001f));
	shader.setMat4("model", model);
	objects[1].Draw(shader);

	//sodas
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(5.60f,7.99f,0.0f));
	shader.setMat4("model", model);
	objects[2].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(31.50f,7.99f,0.0f));
	shader.setMat4("model", model);
	objects[2].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(31.5f,16.2f,-19.30f));
	shader.setMat4("model", model);
	objects[2].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(68.69f,20.2f,-26.90f));
	shader.setMat4("model", model);
	objects[2].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::translate(model, glm::vec3(88.39,13.50f,-24.80f));
	shader.setMat4("model", model);
	objects[2].Draw(shader);

	//popcorn
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(0.34f,0.19f,0.0f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(2.09f,0.78f,0.0f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(8.49f,0.2f,-1.59f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(8.31f,1.49f,-1.59f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(4.41f, 0.88f, -1.59f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	model = glm::translate(model, glm::vec3(2.1f,1.49f,-1.44f));
	shader.setMat4("model", model);
	objects[3].Draw(shader);


	//TODO get light switch model
}

void drawWall(Shader shader, glm::mat4 model, Model object) {

	//looking from the screen to the back of the theater (LEFT)
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.01f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.93f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 3.84f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.87f, -0.01f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.87f, 1.93f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.87f, 3.84f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.9f, -0.01f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.9f, 1.93f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.9f, 3.84f, -0.25f));
	shader.setMat4("model", model);
	object.Draw(shader);

	//(BACK)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.25f, -0.01f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.25f, 1.93f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.25f, 3.84f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.69f, -0.01f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.69f, 1.93f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.69f, 3.84f, -2.56f));
	shader.setMat4("model", model);
	object.Draw(shader);

	//(RIGHT)

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-2.56f, -0.01f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-2.56f, 1.93f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-2.56f, 3.84f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.62f, -0.01f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.62f, 1.93f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.62f, 3.84f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.32f, -0.01f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.32f, 1.93f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.32f, 3.84f, -3.22f));
	shader.setMat4("model", model);
	object.Draw(shader);

	//(BACK)
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-3.21f, -0.01f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-3.21f, 1.93f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-3.21f, 3.84f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.28f, -0.01f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.28f, 1.93f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.28f, 3.84f, -1.34f));
	shader.setMat4("model", model);
	object.Draw(shader);

}

void drawCeiling(Shader shader, glm::mat4 model, Model object) {
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.27f, -0.54f,-4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.67f, -0.54f, -4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.25f, -2.47f, -4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-0.25f, -3.0f, -4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.67f, -2.47f, -4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0F), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.67f, -3.0f, -4.53f));
	shader.setMat4("model", model);
	object.Draw(shader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void printLoc() {
	std::cout << X << " " << Y << " " << Z << "\n";
}

const float SPEEDY = 0.01f;
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		X = X + SPEEDY;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		X = X - SPEEDY;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		Y = Y + SPEEDY;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		Y = Y - SPEEDY;
		printLoc();


	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		Z = Z + SPEEDY;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		Z = Z - SPEEDY;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		render[0] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		render[0] = false;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		render[1] = true;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		render[1] = false;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		lightController += 0.1f;
		cout << lightController << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.Position.x = 1.53f;
		camera.Position.y = 1.40f;
		camera.Position.z = -0.88f;
		camera.Pitch = 1.5f;
		camera.Yaw = 90.0f;
		std::cout << "\n";
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void drawFloor(float y) {

}
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

float X = 0.0f;
float Y = 0.0f;
float Z = 0.0f;

float rotater = 0.0f;
bool render[] = { false,false,false };

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


float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


glm::vec3 pointLightPositions[] = { glm::vec3(0.3f,1.42f,0) };

void drawChairs(Shader shader, glm::mat4 model, Model object);
void drawCarpet(Shader shader, glm::mat4 model, Model object);
void drawLights(Shader shader, glm::mat4 model, Model object);

int main()
{
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





	// load models
	// -----------
	Model chair("models/chair/theaterChair.obj");
	Model carpet("models/carpet/carpet.obj");
	Model light("models/light/light.obj");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
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

		if (render[0]) lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		else lightingShader.setVec3("pointLights[0].position", 100.0f, 100.0f, 100.0f);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);


		lightingShader.setVec3("pointLights[1].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[1].specular", 0.1f, 0.1f, 0.1f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);


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



		//lightingShader.use();
		//// render the loaded model
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		//lightingShader.setMat4("model", model);
		//chair.Draw(lightingShader);

		drawChairs(lightingShader, model, chair);
		drawCarpet(lightingShader, model, carpet);
		drawLights(lightingShader, model, light);


		// also draw the lamp object
		//lightCube.use();
		//lightCube.setMat4("projection", projection);
		//lightCube.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, pointLightPositions[0]);
		////model = glm::translate(model, glm::vec3(X,Y,Z));
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lightCube.setMat4("model", model);

		//glBindVertexArray(lightCubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f)); // translate it down so it's at the center of the scene
		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


void drawChairs(Shader shader, glm::mat4 model, Model object) {
	shader.use();
	// render the loaded model
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", model);

	for (int i = 0; i < 9; i++) {
		model = glm::translate(model, glm::vec3(0.80f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void printLoc() {
	std::cout << X << " " << Y << " " << Z << "\n";
}
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
		X = X + 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		X = X - 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		Y = Y + 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		Y = Y - 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		Z = Z + 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		Z = Z - 0.01f;
		printLoc();
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		rotater = rotater + 0.5f;
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		std::cout << camera.Position.x << "\n";
		std::cout << camera.Position.y << "\n";
		std::cout << camera.Position.z << "\n";
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
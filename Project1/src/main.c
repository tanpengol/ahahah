#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <time.h>
#include <math.h>


#include "stb_image.h"
#include "utils.h"
#include "transform.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// settings
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
VEC3 cameraPos;
float lastX = 0.0f;
float lastY = 0.0f;
BOOL firstMouse = TRUE;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

VEC3 lightPosition; 



//#ifndef _WIN32
int main()
//#else
//int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
//#endif // !1
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	lastX = mode->width / 2.0f;
	lastY = mode->height / 2.0f;
	lightPosition[0] = 1.2f;
	lightPosition[1] = 1.0f;
	lightPosition[2] = 2.0f;
	cameraPos[0] = 0.0f;
	cameraPos[1] = 0.0f;
	cameraPos[2] = 3.0f;
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGLWindow", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGLWindow", NULL, NULL);
	if (window == NULL)
	{
		printf_s("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf_s("Failed to initialize GLAD");
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	glEnable(GL_DEPTH_TEST);
	char *vertexShaderSource = LoadShader("vert1.shader");
	char *fragmentShaderSource = LoadShader("frag1.shader");
	char *lightvShaderSource = LoadShader("lightVert.shader");
	char *lightfShaderSource = LoadShader("lightFrag.shader");
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint lightvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(lightvertexShader, 1, &lightvShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(lightvertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf_s( "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s",  infoLog);
	}
	glGetShaderiv(lightvertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(lightvertexShader, 512, NULL, infoLog);
		printf_s("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}
	// fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint lightfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glShaderSource(lightfragmentShader, 1, &lightfShaderSource, NULL);
	glCompileShader(fragmentShader);
	glCompileShader(lightfragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf_s( "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}
	glGetShaderiv(lightfragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(lightfragmentShader, 512, NULL, infoLog);
		printf_s("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}
	if (vertexShaderSource != NULL)
	{
		free(vertexShaderSource);
		vertexShaderSource = NULL;
	}
	if (fragmentShaderSource != NULL)
	{
		free(fragmentShaderSource);
		fragmentShaderSource = NULL;	
	}
	if (lightvShaderSource != NULL)
	{
		free(lightvShaderSource);
		lightvShaderSource = NULL;
	}
	if (lightfShaderSource != NULL)
	{
		free(lightfShaderSource);
		lightfShaderSource = NULL;
	}
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf_s( "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	GLint lightshaderProgram = glCreateProgram();
	glAttachShader(lightshaderProgram, lightvertexShader);
	glAttachShader(lightshaderProgram, lightfragmentShader);
	glLinkProgram(lightshaderProgram);
	// check for linking errors
	glGetProgramiv(lightshaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(lightshaderProgram, 512, NULL, infoLog);
		printf_s("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}
	glDeleteShader(lightvertexShader);
	glDeleteShader(lightfragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 
	
	/*
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int texwidth, texheight, nrChannels;
	char *filename = getResourceName("container.jpg", "images");
	unsigned char *data = stbi_load(filename, &texwidth, &texheight, &nrChannels, 0);
	if (data != NULL)
	{
		GLint format = nrChannels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	free(filename);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
	
	// render loop
	// -----------*/

	MAT44 projectionMatrix, viewMatrix, modelMatrix;
	VEC3 front,lookCenter,upAxis;
	front[0] = 0.0f;front[1] = 0.0f;front[2] = -0.0f;
	upAxis[0] = 0.0f; upAxis[1] = 1.0f; upAxis[2] = 0.0f;

	perspective(45 * M_PI / 180, SCR_WIDTH / SCR_HEIGHT,0.1f,100.0f, projectionMatrix);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw our first triangle
		lookCenter[0] = cameraPos[0] + front[0]; lookCenter[1] = cameraPos[1] + front[1]; lookCenter[2] = cameraPos[2] + front[2];
		modelMatrix[0]  = 0; modelMatrix[1]  = 0; modelMatrix[2]  = 0; modelMatrix[3]  = 0;
		modelMatrix[4]  = 0; modelMatrix[5]  = 0; modelMatrix[6]  = 0; modelMatrix[7]  = 0;
		modelMatrix[8]  = 0; modelMatrix[9]  = 0; modelMatrix[10] = 0; modelMatrix[11] = 0;
		modelMatrix[12] = 0; modelMatrix[13] = 0; modelMatrix[14] = 0; modelMatrix[15] = 0;
		lookAt(cameraPos, lookCenter, upAxis, viewMatrix);
		glUseProgram(lightshaderProgram);
		GLint objectColor = glGetUniformLocation(lightshaderProgram, "objectColor");
		glUniform3f(objectColor, 1.0f, 0.5f, 0.31f);
		GLint lightColor = glGetUniformLocation(lightshaderProgram, "lightColor");
		glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
		GLint lightPos = glGetUniformLocation(lightshaderProgram, "lightPos");
		glUniform3fv(lightPos, 1, lightPosition);
		GLint viewPos = glGetUniformLocation(lightshaderProgram, "viewPos");
		glUniform3fv(viewPos,3, cameraPos);

		// view/projection transformations
		
		GLint projectionLight = glGetUniformLocation(lightshaderProgram, "projection");
		glUniformMatrix4fv(projectionLight, 1, GL_FALSE, projectionMatrix);
		GLint viewLight = glGetUniformLocation(lightshaderProgram, "view");
		glUniformMatrix4fv(viewLight, 1, GL_FALSE, viewMatrix);
		GLint modelLight = glGetUniformLocation(lightshaderProgram, "model");
		glUniformMatrix4fv(modelLight, 1, GL_FALSE, modelMatrix);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		glUseProgram(shaderProgram);

		GLint projection = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(projection, 1, GL_FALSE, projectionMatrix);
		GLint view = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(view, 1, GL_FALSE, viewMatrix);
		translatev(lightPosition, modelMatrix);
		scale(0.2f, modelMatrix);
		GLint model = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(model, 1, GL_FALSE, modelMatrix);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(cubeVAO);
		/*MAT44 R;
		rotate((float)glfwGetTime(), R);
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, R);*/
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	//free(vec);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, TRUE);
	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime); */
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = FALSE;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	//camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
}
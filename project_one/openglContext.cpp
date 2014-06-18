#include "openglContext.hpp"  
/* documentation */

static OpenGLContext& getInstance() // Singleton is accessed via getInstance()
{
	static OpenGLContext instance; // lazy singleton, instantiated on first use
	return instance;
}
OpenGLContext::OpenGLContext(void)
{
	createContext();
}

OpenGLContext::~OpenGLContext(void)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void OpenGLContext::drawSphere(void)
{
	Sphere *mySphere = new Sphere();
	mySphere->theta = 4.5f;
	mySphere->CreateSphere();
	char* file_name = "moonmap4k.jpg";
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data)
	{
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
	{
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
			);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++)
	{
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	unsigned int moontex = 0;
	/* reserve a name for the texture */
	glGenTextures(1, &moontex);
	glActiveTexture(GL_TEXTURE1);
	/* bind the name to the appropriate binding point */
	glBindTexture(GL_TEXTURE_2D, moontex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint tex_loc = glGetUniformLocation(texture_program, "basic_texture");
	glGenSamplers(1, &tex_loc);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	std::cout << "elements size: " << mySphere->elements.size() << "\n";
	std::cout << "vertices size: " << mySphere->vertices.size() << "\n";
	std::cout << "colours size: " << mySphere->colours.size() << "\n";
	std::cout << "texcoords size: " << mySphere->texcoords.size() << "\n";
	std::cout << "normals size: " << mySphere->normals.size() << "\n";
	glGenBuffers(1, &sphere_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mySphere->elements.size() * sizeof(GLuint), &mySphere->elements[0], GL_STATIC_DRAW);
	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);
	GLuint sphere_vbo = 0;
	glGenBuffers(1, &sphere_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, mySphere->vertices.size() * sizeof(glm::vec3), &mySphere->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(texture_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vt")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_position")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "basic_texture")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "texture_coordinates")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_normal")); // don't forget this!
	unsigned int colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, mySphere->texcoords.size() * sizeof(glm::vec2), &mySphere->texcoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(texture_program, "vt"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
	/* Enable the vertex attributes as they are DISABLED by default. This means the colours would be off and you get a black object! */
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vt")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_position")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "basic_texture")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "texture_coordinates")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_normal")); // don't forget this!
	unsigned int normals_vbo = 0;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, mySphere->normals.size() * sizeof(glm::vec3), &mySphere->normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(texture_program, "vertex_normal"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vt")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_position")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "basic_texture")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "texture_coordinates")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_normal")); // don't forget this!


	//std::cout << "elements size: " << mySphere->elements.size() << "\n";
	//std::cout << "vertices size: " << mySphere->vertices.size() << "\n";
	//std::cout << "colours size: " << mySphere->colours.size() << "\n";
	//glGenBuffers(1, &sphere_ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mySphere->elements.size() * sizeof(GLuint), &mySphere->elements[0], GL_STATIC_DRAW);
	//glGenVertexArrays(1, &sphere_vao);
	//glBindVertexArray(sphere_vao);
	//GLuint sphere_vbo = 0;
	//glGenBuffers(1, &sphere_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	//glBufferData(GL_ARRAY_BUFFER, mySphere->vertices.size() * sizeof(glm::vec3), &mySphere->vertices[0], GL_STATIC_DRAW);
	//unsigned int colours_vbo = 0;
	//glGenBuffers(1, &colours_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	//glBufferData(GL_ARRAY_BUFFER, mySphere->colours.size() * sizeof(glm::vec3), &mySphere->colours[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	//glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	//glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_colour"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	///* Enable the vertex attributes as they are DISABLED by default. This means the colours would be off and you get a black object! */
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
}
void OpenGLContext::drawSkybox(void)
{
	spaceskybox = new Skybox();
	std::cout << "Created skybox " << spaceskybox->skybox_vao << "\n";
	spaceskybox->createCubeMap(
		"skybox_back6.png",
		"skybox_front5.png",
		"skybox_top3.png",
		"skybox_bottom4.png",
		"skybox_left2.png",
		"skybox_right1.png",
		&tex_cube
		);
	glEnableVertexAttribArray(glGetAttribLocation(skybox_program, "cube_texture")); // don't forget this!
}
void OpenGLContext::drawTexturedQuad(void)
{
	GLuint elements[] = {
		0, 1, 2, 0, 2, 3
	};
	glGenBuffers(1, &quad_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);
	char* file_name = "img_test.png";
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data)
	{
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
	{
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
			);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++)
	{
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	printf("first eight data: %i %i %i %i %i %i %i %i\n", image_data[0], image_data[1], image_data[2], image_data[3], image_data[4], image_data[5], image_data[6], image_data[7] );
	unsigned int tex = 0;
	/* reserve a name for the texture */
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	/* bind the name to the appropriate binding point */
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);
	glGenerateMipmap(GL_TEXTURE_2D); 
	GLuint tex_loc = glGetUniformLocation(texture_program, "basic_texture");
	glGenSamplers(1, &tex_loc);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	float points[] = {
		5.0f, 5.0f, 0.0f, 
		5.0f, -5.0f, 0.0f, 
		-5.0f, -5.0f, 0.0f,
		-5.0f, 5.0f, 0.0f,
	};
	float colours[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	GLuint quad_vbo = 0;
	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof (float), points, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(texture_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vt")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_position")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "basic_texture")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "texture_coordinates")); // don't forget this!
	float texcoords[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	};
	GLuint vt_vbo = 0;
	glGenBuffers(1, &vt_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	glBufferData(GL_ARRAY_BUFFER, 2*4*sizeof (float), texcoords, GL_STATIC_DRAW);
	// note: this is your existing VAO
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	// note: I assume that vertex positions are location 0
	glVertexAttribPointer(glGetAttribLocation(texture_program, "vt"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vt")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "vertex_position")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "basic_texture")); // don't forget this!
	glEnableVertexAttribArray(glGetAttribLocation(texture_program, "texture_coordinates")); // don't forget this!

}
void OpenGLContext::drawSpaceship(void)
{
	float points[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
	};
	float colours[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &pyramid_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof (float), points, GL_STATIC_DRAW);
	unsigned int pyramid_colours_vbo = 0;
	glGenBuffers(1, &pyramid_colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof (float), colours, GL_STATIC_DRAW);
	glGenVertexArrays(1, &pyramid_vao);
	glBindVertexArray(pyramid_vao);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colours_vbo);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_colour"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	/* Enable the vertex attributes as they are DISABLED by default. This means the colours would be off and you get a black object! */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}
void OpenGLContext::drawUnitAxes(void)
{
	/* axes lines */
	float lines[] = { 
		0.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, 0.0f, 10.0f, };
	float line_colour[] = { 
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};
	GLuint axes_elements[] = {
		0, 1, 
		0, 2,
		0, 3
	};
	glGenBuffers(1, &axes_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axes_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axes_elements), axes_elements, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &lines_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lines_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), lines, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	unsigned int axes_colours_vbo = 0;
	glGenBuffers(1, &axes_colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, axes_colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), line_colour, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_colour"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	/* Enable the vertex attributes as they are DISABLED by default. This means the colours would be off and you get a black object! */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}
/* call-back functions */
/* You cannot use regular methods as callbacks, as GLFW is a C library and doesn’t know about objects and this pointers. 
If you wish to receive callbacks to a C++ object, use static methods or regular functions as callbacks, store the pointer 
to the object you wish to call in some location reachable from the callbacks and use it to call methods on your object. */
static void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	g_gl_width = width;
	g_gl_height = height;
}


static void error_callback(int error, const char* description)
{
	gl_log_err("GLFW Error: code %i msg: %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F9 && action == GLFW_PRESS)
		FPS_COUNT = !FPS_COUNT;
}

bool OpenGLContext::createContext()
{
	/* Start GL context and OS window using the GLFW library*/
	if (!glfwInit())
	{
		gl_log("ERROR: couldn't start GLFW3\n");
		exit(EXIT_FAILURE);
		return 1;
	}

	/* Put glfw hints here if necessary*/
	/* Set the minimum opengl version to 4.0 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	/* Enable 4x antialiasing */
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create the GLFW resource */
	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	//g_gl_height = vmode->height;
	//g_gl_width = vmode->width;
	window = glfwCreateWindow(g_gl_width, g_gl_height, "Project One", NULL, NULL);
	if (!window)
	{
		gl_log("ERROR: could not open window with GLFW3\n");
		exit(EXIT_FAILURE);
		return 1;
	}
	glfwMakeContextCurrent(window);

	/* Set callbacks */
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	/* Ensure we can capture the escape key being pressed below */
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	/* Start GLEW extension handler */
	glewExperimental = true;
	glewInit();

	/* Get version info and gl context parameters and log them */
	const GLubyte* opengl_renderer = glGetString(GL_RENDERER);
	const GLubyte* opengl_version = glGetString(GL_VERSION);
	gl_log("Renderer: %s\n", opengl_renderer);
	gl_log("OpenGL version supported: %s\n", opengl_version);
	log_gl_params();

	/* Tell GL to only draw onto a pixel if it is closest to the viewer */
	glEnable(GL_DEPTH_TEST); //enable depth testing
	glDepthFunc(GL_LESS); //smaller values are closest
	/* Tell GL to cull back faces, and that CW is front, CCW is back */
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CW); // GL_CW for clockwise, GL_CCW for counter clock-wise

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f); //blank canvas colour
	return true; // We have successfully created a context, return true  
}
void OpenGLContext::setupScene(void)
{
	//glClearColor(0.4f, 0.6f, 0.9f, 0.0f); // Set the clear color based on Microsofts CornflowerBlue (default in XNA) 
	/*generate and load shaders*/
	/* Create and compile our GLSL program from the shaders */
	texture_program = LoadShaders("texture_vs.glsl", "texture_fs.glsl");
	shader_program = LoadShaders("test_vs.glsl", "test_fs.glsl");
	skybox_program = LoadShaders("skybox_vs.glsl", "skybox_fs.glsl");
	drawUnitAxes();
	drawSphere();
	drawSkybox();
	monkey = new Spaceship();
	State monkeyState = {
		glm::vec4(10.0f, 10.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	Derivative monkeyDerivative = {
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	monkey->createSpaceship(monkeyState, monkeyDerivative);
	monkey->shader_program = shader_program;
	drawSpaceship();

	std::cout << "pyramid_vao " << pyramid_vao << "\n ";
	std::cout << "quad_vao " << quad_vao << "\n ";
	std::cout << "sphere_vao " << sphere_vao << "\n ";
}
void OpenGLContext::renderScene(void)
{
	//render scene, the display method
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Update the viewport size for when window changes size */
	glViewport(0, 0, g_gl_width, g_gl_height);

	glDepthMask(GL_FALSE);
	glUseProgram(skybox_program);
	int tex_loc = glGetUniformLocation(skybox_program, "cube_texture");
	/* assocaite the sampler in the shader with the desired texture unit */
	glUniform1i(tex_loc, 2); // use active texture 0
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	glBindVertexArray(spaceskybox->skybox_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	//monkey draw
	glUseProgram(shader_program);
	setModelMatrix(shader_program, glm::translate(
		glm::mat4(1.0f),
		glm::vec3(monkey->state.position.xyz)));
	glBindVertexArray(pyramid_vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glUseProgram(0);
	/* draw 10u axes lines */
	/* Set the active shader */
	glUseProgram(shader_program);
	setModelMatrix(shader_program, glm::translate(
		glm::mat4(1.0f),
		glm::vec3(0.0f)));
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axes_ebo);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glUseProgram(0);
	/* draw textured quad */
	glUseProgram(texture_program);
	tex_loc = glGetUniformLocation(texture_program, "basic_texture");
	/* assocaite the sampler in the shader with the desired texture unit */
	glUniform1i(tex_loc, 0); // use active texture 0
	//glBindVertexArray(quad_vao);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(0);
	glUseProgram(texture_program);
	tex_loc = glGetUniformLocation(texture_program, "basic_texture");
	/* assocaite the sampler in the shader with the desired texture unit */
	glUniform1i(tex_loc, 1); // use active texture 0
	glBindVertexArray(sphere_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo);
	glDrawElements(GL_TRIANGLES, 19200, GL_UNSIGNED_INT, 0);
	// update other events like input handling 
	glfwPollEvents();
	// put the stuff we've been drawing onto the display
	glfwSwapBuffers(window);
}

void OpenGLContext::setViewMatrix(GLuint shader, glm::mat4 view_matrix)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
}
void OpenGLContext::setProjMatrix(GLuint shader, glm::mat4 proj_matrix)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(proj_matrix));
}
void OpenGLContext::setModelMatrix(GLuint shader, glm::mat4 model_matrix)
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
}


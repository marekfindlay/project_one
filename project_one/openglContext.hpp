#ifndef OPENGLCONTEXT_HPP
#define OPENGLCONTEXT_HPP

/* Define function prototypes */
#include "stdafx.h"
#include "Skybox.h"
extern bool FPS_COUNT;
extern int g_gl_height;
extern int g_gl_width;
static const glm::vec4 ambientLight[2] =
{
	glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
	glm::vec4(0.5f, 0.2f, 0.2f, 1.0f)
};
static const glm::vec4 lightColours[2] =
{
	glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
};
static const float shininess = 20.0f;
static const float strength = 10.0f;
class OpenGLContext
{
public:
	GLFWwindow* window; //The window handler
	OpenGLContext(void); //default constructor
	~OpenGLContext(void); // Destructor for cleaning up
	bool createContext(); //creation of our opengl context
	void setupScene(void); //All scene info setup here
	void renderScene(void); //render scene, the display method
	void drawUnitAxes(void); //draws RGB XYZ unit axes
	void drawSpaceship(void); // draws square based pyramid
	void drawTexturedQuad(void); 
	void drawSkybox(void);
	void drawSphere(void);
	void setViewMatrix(GLuint shader, glm::mat4 view_matrix); //Updates the view matrix in the provided shader
	void setProjMatrix(GLuint shader, glm::mat4 proj_matrix); //Updates teh projection matrix in the provided shader
	void setModelMatrix(GLuint shader, glm::mat4 model_matrix); //Updates the model (world) matrix in the provided shader
	void window_size_callback(GLFWwindow* window, int width, int height); //implementation of the window size callback
	unsigned int lines_vbo = 0;
	unsigned int pyramid_vbo = 0;

	GLuint vao = 0;
	GLuint pyramid_vao = 0;
	GLuint moon_vao = 0;
	GLuint quad_vao = 0;
	GLuint quad_ebo = 0;
	GLuint axes_ebo = 0;

	GLuint sphere_vao = 0;
	GLuint sphere_ebo = 0;
	/* keep track of window size for things like the viewport and the mouse cursor */

	unsigned int vaoID[1]; // Our Vertex Array Object  
	unsigned int vboID[1]; // Our Vertex Buffer Object  
	GLuint shader_program;
	GLuint texture_program;
	GLuint skybox_program;
	GLuint tex_cube;

	/* scene objects */
	Skybox *spaceskybox;
	Spaceship *monkey;
private:
protected:

};
#endif //OPENGLCONTEXT_HPP
#pragma once
#include "Application.h"

/*OpenGL dependencies*/
#include <gl_core_4_4.h>
#include <glm\glm.hpp>

/*AIE Gizmos and Input class*/
#include <Gizmos.h>
#include "Input.h"

/*Render and shader classes*/
#include "Shader.h"
#include "RenderData.h"
#include "GeometryHelper.h"
#include "Texture.h"

#include "imgui_glfw3.h"
#include "Camera.h"
#include "FlyCamera.h"

#include "GUI.h"

using glm::vec3;
using glm::vec3;
using glm::mat4;
using aie::Gizmos;
using aie::Input;
using GeometryHelper::GLInfo;

class Assessment : public Application
{
public:
	Assessment();
	~Assessment();

	bool startup();
	void shutdown();

	void update(float deltaTime);
	void draw();

	void DrawGizmoGrid();

	/*Used for testing lighting, temp functions*/
	void HandleInput();
	void RotateLight();
	void ControlLight(float deltaTime);

protected:
	unsigned int		m_uiProgramID;

	/*Need to wrap these into a mesh class*/
	Shader*         m_pSoulSpearShader;
	Texture*        m_pSoulSpearDiffuse;
	Texture*        m_pSoulSpearNormal;
	Texture*		m_pSoulSpearSpecular;
	std::vector<RenderData*>        m_pSoulSpear;
	/*                                        */

	vec3				m_lightPos;
	FlyCamera*			m_pFlyCamera;
	Input*				m_pInput;

	//ANIMATION
	Shader*				m_handShader;
	Texture*			m_handTexture;
	std::vector<GLInfo> m_hand;

	GUI*				m_GUI;
	void RenderSoulSpear();
	void RenderHand();


	//BUNNY
	Shader*						m_useShadowProgram;
	Shader*						m_shadowGen;
	std::vector<RenderData*>	m_bunny;
	RenderData*					m_plane;

	//Shadow Generation
	void SetupShadowMap();
	void DrawShadows();

	GLuint						m_fbo;
	GLuint						m_fboDepth;

	vec3						m_lightDirection;
	mat4						m_lightMatrix;

	mat4 m_modelMatrixBunny;
};


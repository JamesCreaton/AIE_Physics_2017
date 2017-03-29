#include "Assessment.h"



Assessment::Assessment()
	: m_uiProgramID(0)
{
}

Assessment::~Assessment()
{
}

bool Assessment::startup()
{
	Gizmos::create(64000, 64000, 64000, 64000);

	//Set up camera
	m_pFlyCamera = new FlyCamera(*m_window);
	m_pFlyCamera->SetMoveSpeed(25.0f);
	m_pFlyCamera->SetRotationSpeed(0.1f);

	m_pFlyCamera->SetPerspective(glm::pi<float>() * 0.25f, (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
	m_pFlyCamera->SetLookAt(glm::vec3(35, 35, 35), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//MODEL LOADING
	m_pSoulSpear = GeometryHelper::LoadOBJFromDisk("./soulspear/soulspear.obj");
	m_pSoulSpearShader = new Shader("./shaders/Assessment/AdvancedLighting.vert", "./shaders/Assessment/AdvancedLighting.frag");

	//TEXTURING AND LIGHTING
	m_pSoulSpearDiffuse = new Texture("./soulspear/soulspear_diffuse.tga");
	m_pSoulSpearNormal = new Texture("./soulspear/soulspear_normal.tga");
	m_pSoulSpearSpecular = new Texture("./soulspear/soulspear_specular.tga");

	//ANIMATION
	m_handShader = new Shader("./shaders/Animation/hand.vert", "./shaders/Animation/hand.frag");
	m_handTexture = new Texture("./models/hand/hand.png");
	GeometryHelper::LoadHandTest(m_hand);

	//SHADOWING
	m_bunny = GeometryHelper::LoadOBJFromDisk("./models/Bunny.obj");
	m_plane = GeometryHelper::CreateGrid(25, 25, 25, 25, vec4(0.75f, 0.75f, 0.75f, 1));
	m_useShadowProgram = new Shader("./shaders/Shadows/shadow.vert", "./shaders/Shadows/shadow.frag");
	m_shadowGen = new Shader("./shaders/Shadows/generateShadow.vert", "./shaders/Shadows/generateShadow.frag");

	SetupShadowMap();
	m_modelMatrixBunny[3] = glm::vec4(vec3(5.5f, 0, -5.5f), 1);
	m_modelMatrixBunny = glm::scale(m_modelMatrixBunny, vec3(0.25f));

	
	//INPUT AND GUI
	m_pInput = Input::getInstance();
	m_GUI = new GUI();
	m_GUI->GetProperties().m_clearColour = vec4(0.25f, 0.25f, 0.25f, 1.0f);

	return true;
}

void Assessment::shutdown()
{
	//SoulSpear
	delete m_pSoulSpearShader;
	delete m_pSoulSpearDiffuse;
	delete m_pSoulSpearNormal;
	delete m_pSoulSpearSpecular;

	//Hand 
	delete m_handShader;
	delete m_handTexture;

	//Bunny
	for (auto* renderData : m_bunny)
	{
		delete renderData;
	}

	//Shadows
	delete m_useShadowProgram;
	delete m_shadowGen;
	delete m_plane;

	//Camera
	delete m_pFlyCamera;

	//Delete SoulSpear Renderdata
	for (auto* renderData : m_pSoulSpear)
	{
		delete renderData;
	}

	//Clear up Gizmos
	Gizmos::destroy();
}

void Assessment::update(float deltaTime)
{
	m_GUI->Update();

	// query time since application started
	float time = getTime();

	//Update the camera
	m_pFlyCamera->Update(deltaTime);

	HandleInput();

	// wipe the gizmos clean for this frame
	aie::Gizmos::clear();

	DrawGizmoGrid();
	//ControlLight(deltaTime);
	RotateLight();
}

void Assessment::draw()
{
	clearScreen();
	setBackgroundColour(m_GUI->GetProperties().m_clearColour);
	DrawShadows();
	RenderSoulSpear();
	RenderHand();
	

	Gizmos::draw(m_pFlyCamera->GetProjectionView());
}

void Assessment::DrawGizmoGrid()
{
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		aie::Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
}


//Deffered Rendering

//void Assessment::SetupGPASSBuffer()
//{
//	// setup gpass framebuffer 
//	glGenFramebuffers(1, &m_gpassFBO); 
//	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO); 
//
//	glGenTextures(1, &m_albedoTexture); 
//	glBindTexture(GL_TEXTURE_2D, m_albedoTexture); 
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
//
//	glGenTextures(1, &m_positionTexture); 
//	glBindTexture(GL_TEXTURE_2D, m_positionTexture); 
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
//	
//	glGenTextures(1, &m_normalTexture); 
//	glBindTexture(GL_TEXTURE_2D, m_normalTexture); 
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, 1280, 720); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
//
//	glGenRenderbuffers(1, &m_gpassDepth); 
//	glBindRenderbuffer(GL_RENDERBUFFER, m_gpassDepth); 
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720); 
//	
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_albedoTexture, 0); 
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_positionTexture, 0); 
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_normalTexture, 0); 
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gpassDepth); 
//	
//	GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; 
//	glDrawBuffers(3, gpassTargets); 
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//void Assessment::SetupLightBuffer()
//{
//	// setup light framebuffer 
//	glGenFramebuffers(1, &m_lightFBO); 
//	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO); 
//	
//	glGenTextures(1, &m_lightTexture); 
//	glBindTexture(GL_TEXTURE_2D, m_lightTexture); 
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
//
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightTexture, 0); 
//	
//	GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, lightTargets); 
//	
//	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (status != GL_FRAMEBUFFER_COMPLETE) 
//		printf("Framebuffer Error!\n"); 
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//void Assessment::DrawDirectionalLight(const glm::vec3& direction, const glm::vec3& diffuse) 
//{ 
//	glm::vec4 viewSpaceLight = m_pFlyCamera->GetView() * glm::vec4(glm::normalize(direction), 0); 
//
//	int loc = glGetUniformLocation(m_directionalLightShader->GetProgramID(), "lightDirection");
//	glUniform3fv(loc, 1, &viewSpaceLight[0]); 
//	loc = glGetUniformLocation(m_directionalLightShader->GetProgramID(), "lightDiffuse");
//	glUniform3fv(loc, 1, &diffuse[0]); 
//	glBindVertexArray(m_quadVAO); 
//	glDrawArrays(GL_TRIANGLES, 0, 6); 
//}
//void Assessment::UseDeferredRendering()
//{
//	//First Pass
//	glEnable(GL_DEPTH_TEST);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glUseProgram(m_gpassShader->GetProgramID());
//
//	//bind camera transforms, including view matrix separate
//	int loc = glGetUniformLocation(m_gpassShader->GetProgramID(), "projectionView");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pFlyCamera->GetProjectionView()[0][0]));
//
//	loc = glGetUniformLocation(m_gpassShader->GetProgramID(), "view");
//	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pFlyCamera->GetView()[0][0]));
//
//
//	//Second Pass
//	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_ONE);
//
//	glUseProgram(m_directionalLightShader->GetProgramID());
//
//	loc = glGetUniformLocation(m_directionalLightShader->GetProgramID(), "positionTexture");
//
//	glUniform1i(loc, 5);
//	glActiveTexture(GL_TEXTURE5);
//	glBindTexture(GL_TEXTURE_2D, m_positionTexture);
//
//	loc = glGetUniformLocation(m_directionalLightShader->GetProgramID(), "normalTexture");
//
//	glUniform1i(loc, 6);
//	glActiveTexture(GL_TEXTURE6);
//	glBindTexture(GL_TEXTURE_2D, m_normalTexture);
//
//	// draw directional lights as fullscreen quads 
//	DrawDirectionalLight(glm::vec3(-1), glm::vec3(1));
//	glDisable(GL_BLEND);
//
//	//Third Pass
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glUseProgram(m_compositeShader->GetProgramID());
//
//	loc = glGetUniformLocation(m_compositeShader->GetProgramID(), "albedoTexture");
//	glUniform1i(loc, 7);
//	glActiveTexture(GL_TEXTURE7);
//	glBindTexture(GL_TEXTURE_2D, m_albedoTexture);
//
//	loc = glGetUniformLocation(m_compositeShader->GetProgramID(), "lightTexture");
//	glUniform1i(loc, 8);
//	glActiveTexture(GL_TEXTURE8);
//	glBindTexture(GL_TEXTURE_2D, m_lightTexture);
//
//	glBindVertexArray(m_quadVAO);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//void Assessment::CreateFBO()
//{
//	// setup and bind a framebuffer 
//	glGenFramebuffers(1, &m_gpassFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_gpassFBO);
//
//	// create a texture and bind it 
//	glGenTextures(1, &m_fboTexture);
//	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
//
//	// specify texture format for storage 
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	// attach it to the framebuffer as the first colour attachment 
//	// the FBO MUST still be bound 
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);
//
//	// setup and bind a 24bit depth buffer as a render buffer 
//	glGenRenderbuffers(1, &m_fboDepth);
//	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//
//	// while the FBO is still bound 
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
//
//	// setup framebuffer 
//	glGenFramebuffers(1, &m_fbo);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
//	glGenTextures(1, &m_fboTexture);
//	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 512, 512);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);
//	glGenRenderbuffers(1, &m_fboDepth);
//	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
//
//	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, drawBuffers);
//
//	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//	if (status != GL_FRAMEBUFFER_COMPLETE)
//		printf("Framebuffer Error!\n");
//
//	// unbind the FBO so that we can render to the back buffer 
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//	float vertexData[] = { -5, 0, -5, 1, 0, 0, 5, 0, -5, 1, 1, 0, 5, 10, -5, 1, 1, 1, -5, 10, -5, 1, 0, 1, };
//	unsigned int indexData[] = { 0, 1, 2, 0, 2, 3, };
//
//	glGenVertexArrays(1, &m_vao);
//	glBindVertexArray(m_vao);
//	glGenBuffers(1, &m_vbo);
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
//	glGenBuffers(1, &m_ibo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}

//Moved to clear up Draw() function
void Assessment::RenderSoulSpear()
{
	m_pSoulSpearShader->Draw();

	//Vertex Uniforms
	m_pSoulSpearShader->SetUniform(std::string("projectionViewGlobalMatrix"), m_pFlyCamera->GetProjectionView());
	mat4 modelMatrix;
	modelMatrix[3] = glm::vec4(vec3(-5.5f, 0, -5.5f), 1);
	m_pSoulSpearShader->SetUniform(std::string("modelMatrix"), modelMatrix);

	//Fragment Uniforms
	m_pSoulSpearShader->SetUniform(std::string("lightColour"), vec4(1, 1, 1, 1));

	if (m_GUI->GetProperties().m_bUseDynamicLight) {
		m_pSoulSpearShader->SetUniform(std::string("lightDir"), m_lightPos);
	}
	else {
		m_pSoulSpearShader->SetUniform(std::string("lightDir"), vec3(1.0f, 1.0f, 1.0f));
	}


	m_pSoulSpearShader->SetUniform(std::string("ambientLight"), vec3(0.25f, 0.25f, 0.25f));
	m_pSoulSpearShader->SetUniform(std::string("cameraPos"), vec3(m_pFlyCamera->GetTransform()[3].x,
															 m_pFlyCamera->GetTransform()[3].y,
														     m_pFlyCamera->GetTransform()[3].z));
	m_pSoulSpearShader->SetUniform(std::string("specPow"), m_GUI->GetProperties().m_fSpecularPower);
	//m_pSoulSpearShader->SetUniform(std::string("distort"), m_GUI->GetProperties().m_bDistort);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_pSoulSpearDiffuse->GetTexture());
	GLuint DiffuseTextureID = glGetUniformLocation(m_pSoulSpearShader->GetProgramID(), "diffuse");
	glUniform1i(DiffuseTextureID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_pSoulSpearSpecular->GetTexture());
	GLuint SpecularTextureID = glGetUniformLocation(m_pSoulSpearShader->GetProgramID(), "specular");
	glUniform1i(SpecularTextureID, 1);

	if (m_GUI->GetProperties().m_bUseNormalMap) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, *m_pSoulSpearNormal->GetTexture());
		GLuint NormalTextureID = glGetUniformLocation(m_pSoulSpearShader->GetProgramID(), "normal");
		glUniform1i(NormalTextureID, 2);
	}
	if (m_GUI->GetProperties().m_bDisplayLight) {
		Gizmos::addSphere(m_lightPos, 0.25f, 30, 30, vec4(1, 0, 0, 1));
	}

	for (auto renderData : m_pSoulSpear) {
		if (m_pFlyCamera->isBoundsInFrustrum(renderData->GetBoundsAsConst()))
		{
			renderData->Render();
		}
	}
}
void Assessment::RenderHand()
{
	m_handShader->Draw();

	//Vertex Uniforms
	m_handShader->SetUniform("projectionViewGlobalMatrix", m_pFlyCamera->GetProjectionView());
	mat4 modelMatrix;
	modelMatrix[3] = glm::vec4(vec3(5.5f, 0, 5.5f), 1);
	m_handShader->SetUniform("modelMatrix", modelMatrix);

	//Fragment Uniforms
	if (m_GUI->GetProperties().m_bUseDynamicLight) {
		m_handShader->SetUniform(std::string("lightDir"), m_lightPos);
	}
	else {
		m_handShader->SetUniform(std::string("lightDir"), vec3(0, 1, 0));
	}

	m_handShader->SetUniform("keyTime", cosf(getTime() * 3) * 0.5f + 0.5f);
	m_handShader->SetUniform("lightColour", vec3(1, 1, 1));
	m_handShader->SetUniform("ambientLight", vec3(0.2f, 0.2f, 0.2f));
	m_handShader->SetUniform("specPow", m_GUI->GetProperties().m_fSpecularPower);
	m_handShader->SetUniform("cameraPos", vec3(m_pFlyCamera->GetTransform()[3].x,
										   m_pFlyCamera->GetTransform()[3].y,
											   m_pFlyCamera->GetTransform()[3].z
											   ));


	//Bind Texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *m_handTexture->GetTexture());
	GLuint DiffuseTextureID = glGetUniformLocation(m_handShader->GetProgramID(), "handTexture");
	glUniform1i(DiffuseTextureID, 2);
	GeometryHelper::DrawHand(m_hand);

	if (m_GUI->GetProperties().m_bDisplayLight) {
		Gizmos::addSphere(m_lightPos, 0.25f, 30, 30, vec4(1, 0, 0, 1));
	}
}

//Used to test lighting
void Assessment::HandleInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_O)) {
		m_GUI->GetProperties().m_fSpecularPower += 1.0f;
	}

	if (glfwGetKey(m_window, GLFW_KEY_P)) {
		m_GUI->GetProperties().m_fSpecularPower -= 1.0f;
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE)) {

	}
}
void Assessment::RotateLight()
{
	m_lightPos = vec3(sin(glfwGetTime() + 15), 3, cos(glfwGetTime() + 15));
}
void Assessment::ControlLight(float deltaTime)
{
	vec3 vRight = vec3(1, 0, 0);
	vec3 vUp = vec3(0, 1, 0);
	vec3 vForward = vec3(0, 0, 1);
	vec3 moveDir(0.0f);

	if (glfwGetKey(m_window, GLFW_KEY_UP)) {
		moveDir -= vForward;
	}

	if (glfwGetKey(m_window, GLFW_KEY_DOWN)) {
		moveDir += vForward;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT)) {
		moveDir -= vRight;
	}

	if (glfwGetKey(m_window, GLFW_KEY_RIGHT)) {
		moveDir += vRight;
	}

	float fLength = glm::length(moveDir);
	if (fLength > 0.01f)
	{
		moveDir = ((float)deltaTime * 25.0f) * glm::normalize(moveDir);
		m_lightPos += moveDir;
	}
}


//Shadowing
void Assessment::SetupShadowMap()
{
	// setup shadow map buffer 
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fboDepth);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	// texture uses a 16-bit depth component format 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attached as a depth attachment to capture depth not colour 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_fboDepth, 0);

	// no colour targets are used 
	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Assessment::DrawShadows()
{
	// within the draw() method 
	// shadow pass: bind our shadow map target and clear the depth 
	m_lightDirection = glm::normalize(glm::vec3(4, 2.5f, 4));

	glm::mat4 lightProjection = glm::ortho<float>(-20, 20, -20, 20, -20, 20);
	glm::mat4 lightView = glm::lookAt(m_lightDirection, glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightMatrix = lightProjection * lightView;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_shadowGen->Draw();

	int loc = glGetUniformLocation(m_shadowGen->GetProgramID(), "lightMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_lightMatrix[0][0]));

	// DRAW ALL SHADOW CASTING GEROMETRY HERE!! 
	for (auto* renderData : m_bunny)
	{
		renderData->Render();
	}

	// final pass: bind back-buffer and clear colour and depth 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080); // screen resolution
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_useShadowProgram->Draw();

	// bind the camera 
	loc = glGetUniformLocation(m_useShadowProgram->GetProgramID(), "projectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_pFlyCamera->GetProjectionView()[0][0]));

	// bind the light matrix 
	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	glm::mat4 lightMatrix = textureSpaceOffset * m_lightMatrix;

	m_useShadowProgram->SetUniform("lightMatrix", lightMatrix);
	m_useShadowProgram->SetUniform("lightDir", m_lightDirection);
	m_useShadowProgram->SetUniform("shadowMap", 0);
	m_useShadowProgram->SetUniform("shadowBias", 0.01f);
	m_useShadowProgram->SetUniform("modelMatrix", m_modelMatrixBunny);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboDepth);

	// DRAW THE SHADOW RECEIVING GEOMETRY!
	for (auto* renderData : m_bunny)
	{
		renderData->Render();
	}

	m_plane->Render();
}
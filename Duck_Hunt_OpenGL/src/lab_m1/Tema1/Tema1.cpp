#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <random>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/myObject2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
	modelMatrix = glm::mat3(1);
	modelMatrix1 = glm::mat3(1);
	modelMatrix2 = glm::mat3(1);

	wing0X = 0;
	wing0Y = 0;
	wing1X = 0;
	wing1Y = 0;

	recA = glm::vec3(0, 0, 0);
	recB = glm::vec3(0, 0, 0);
	recC = glm::vec3(0, 0, 0);
	recD = glm::vec3(0, 0, 0);

	cx = 50;
	cy = 50;

	translateX = 0;
	translateY = 0;

	dirX = 3;
	dirY = 2;
	inv = 0;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist6(-55, 55);
	randAng = RADIANS(dist6(rng));
	std::uniform_int_distribution<std::mt19937::result_type> dist7(500, 1000);
	randX = dist7(rng);
	std::uniform_real_distribution<float> dist8(0, 4);
	lucky = dist8(rng);

	angularStepWing0 = 0;
	angularStepWing1 = 0;
	neckX = 0;
	neckY = 0;

	resX = 0;
	resY = 0;

	speed = 265;
	scoreX = 200;
	scoreY = 50;
	scaleScoreX = 1;

	bulletX = scoreX - 150;
	bulletY = scoreY + 20;

	verif0 = true;
	cont0 = 1;
	verif1 = true;
	cont1 = 1;
	firstTime = false;
	bulCnt = 0;
	heartCnt = 0;
	numKills = 0;
	escape = false;
	killed = false;
	gameOver = false;
	calc = false;
	helper = false;

}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	window->HidePointer();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	glm::vec3 corner = glm::vec3(0, 0, 0);
	glm::vec3 cornerY = glm::vec3(0, resolution.y / 5, 0);
	resX = resolution.x;
	resY = resolution.y;
	float squareSide = 100;

	Mesh* body = myObject2D::CreateTriangle("body", corner, squareSide, glm::vec3(0.5, 0.35, 0.05), true);
	Mesh* luckyDuck = myObject2D::CreateTriangle("lucky", corner, squareSide, glm::vec3(1, 0.5, 0), true);
	vector<Mesh*> wings(2);
	for (int i = 0; i < 2; i++)
	{
		wings[i] = myObject2D::CreateTriangle("wing" + to_string(i), corner, (float)0.45 * squareSide, glm::vec3(0.5, 0.35, 0.05), true);
		AddMeshToList(wings[i]);
	}
	Mesh* sky = myObject2D::CreateSquareRectangle("sky", cornerY, (float)resolution.x, 4 * (float)resolution.y / 5, glm::vec3(0, 0, 1), true);
	Mesh* ground = myObject2D::CreateSquareRectangle("ground", corner, (float)resolution.x, (float)resolution.y / 5, glm::vec3(0, 1, 0), true);
	Mesh* died = myObject2D::CreateSquareRectangle("died", corner, (float)resolution.x, 4 * (float)resolution.y, glm::vec3(1, 0, 0), true);
	Mesh* cursor = myObject2D::CreateSquareRectangle("cursor", glm::vec3(-32 / 2, -32 / 2, 0), (float)35, (float)35, glm::vec3(255, 255, 255));
	Mesh* cursorCirc = myObject2D::CreateCircle("cursCir", glm::vec3(255, 255, 255));
	Mesh* head = myObject2D::CreateCircle("head", glm::vec3(0, 0.5, 0));
	Mesh* beak = myObject2D::CreateTriangle("beak", corner, squareSide, glm::vec3(1, 1, 0), true);
	vector<Mesh*> hearts(3);
	for (int i = 0; i < 3; i++)
	{
		hearts[i] = myObject2D::CreateCircle("heart" + to_string(i), glm::vec3(1, 0, 0));;
		AddMeshToList(hearts[i]);
	}
	vector<Mesh*> bullets(3);
	for (int i = 0; i < 3; i++)
	{
		bullets[i] = myObject2D::CreateSquareRectangle("bullet" + to_string(i), glm::vec3(resX - 350, resY - 175, 0), bulletX,
			bulletY, glm::vec3(1, 1, 0), true);
		AddMeshToList(bullets[i]);
	}
	Mesh* skel_score = myObject2D::CreateSquareRectangle("skel_score", glm::vec3(resX - 250, resY - 250, 0), scoreX, scoreY, glm::vec3(255, 255, 255));
	Mesh* score = myObject2D::CreateSquareRectangle("score", corner, scaleScoreX, scoreY, glm::vec3(0, 0, 0.25), true);
	Mesh* target = myObject2D::CreateSquareRectangle("target", corner, squareSide + 25, squareSide + 43, glm::vec3(255, 255, 255));

	AddMeshToList(target);
	AddMeshToList(score);
	AddMeshToList(skel_score);
	AddMeshToList(body);
	AddMeshToList(sky);
	AddMeshToList(ground);
	AddMeshToList(head);
	AddMeshToList(cursor);
	AddMeshToList(died);
	AddMeshToList(beak);
	AddMeshToList(luckyDuck);
	AddMeshToList(cursorCirc);
	begin = std::chrono::steady_clock::now();
}


void Tema1::FrameStart()
{

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
	if (!gameOver)
	{

		translateX += speed * deltaTimeSeconds * sin(RADIANS((float)M_PI) - randAng);
		translateY += speed * deltaTimeSeconds * cos(RADIANS((float)M_PI) - randAng);

		RenderMesh2D(meshes["cursor"], shaders["VertexColor"], transform2D::Translate((float) window->GetCursorPosition().x, (float)resY - window->GetCursorPosition().y));
		RenderMesh2D(meshes["cursCir"], shaders["VertexColor"],
			transform2D::Translate((float)window->GetCursorPosition().x, (float) resY - window->GetCursorPosition().y) * transform2D::Scale(10, 10));
		RenderMesh2D(meshes["ground"], shaders["VertexColor"], glm::mat3(1));

		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(neckX, neckY);
			modelMatrix *= transform2D::Scale(17, 17);
			RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
		}

		{
			modelMatrix = glm::mat3(1);
			modelMatrix1 = glm::mat3(1);
			modelMatrix *= transform2D::Translate(cx, cy);
			modelMatrix *= transform2D::Rotate(randAng);
			modelMatrix *= transform2D::Translate(-cx, -cy);
			modelMatrix *= transform2D::Scale(0.30, 0.40);
			glm::vec3 vecBeak = modelMatrix * glm::vec3((meshes["beak"]->vertices[0].position.x + meshes["beak"]->vertices[1].position.x) / 2,
				(meshes["beak"]->vertices[0].position.y + meshes["beak"]->vertices[1].position.y) / 2, 1);
			modelMatrix1 *= transform2D::Translate(neckX - vecBeak.x, neckY - vecBeak.y);
			modelMatrix1 *= transform2D::Translate(cx, cy);
			modelMatrix1 *= transform2D::Rotate(randAng);
			modelMatrix1 *= transform2D::Translate(-cx, -cy);
			modelMatrix1 *= transform2D::Scale(0.30, 0.40);
			RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix1);
			vecBeak = modelMatrix1 * glm::vec3(meshes["beak"]->vertices[2].position.x, meshes["beak"]->vertices[2].position.y, 1);
			modelMatrix = glm::mat3(1);
			modelMatrix1 = glm::mat3(1);
			modelMatrix *= transform2D::Translate(cx + 25, cy + 42);
			modelMatrix *= transform2D::Rotate(randAng);
			modelMatrix *= transform2D::Translate(-cx - 25, -cy - 42);
			glm::vec3 vecTarget = modelMatrix * glm::vec3((meshes["target"]->vertices[3].position.x +
				meshes["target"]->vertices[2].position.x) / 2,
				(meshes["target"]->vertices[3].position.y + meshes["target"]->vertices[2].position.y) / 2, 1);
			float offsetX = vecBeak.x - vecTarget.x;
			float offsetY = vecBeak.y - vecTarget.y;
			modelMatrix1 *= transform2D::Translate(offsetX, offsetY);
			modelMatrix1 *= transform2D::Translate(cx + 25, cy + 42);
			modelMatrix1 *= transform2D::Rotate(randAng);
			modelMatrix1 *= transform2D::Translate(-cx - 25, -cy - 42);

			recA.x = modelMatrix1[0][0] * meshes["target"]->vertices[0].position.x +
				modelMatrix1[1][0] * meshes["target"]->vertices[0].position.y + modelMatrix1[2][0];
			recA.y = modelMatrix1[1][1] * meshes["target"]->vertices[0].position.y +
				modelMatrix1[0][1] * meshes["target"]->vertices[0].position.x + modelMatrix1[2][1];

			recB.x = modelMatrix1[0][0] * meshes["target"]->vertices[1].position.x +
				modelMatrix1[1][0] * meshes["target"]->vertices[1].position.y + modelMatrix1[2][0];
			recB.y = modelMatrix1[1][1] * meshes["target"]->vertices[1].position.y +
				modelMatrix1[0][1] * meshes["target"]->vertices[1].position.x + modelMatrix1[2][1];

			recC.x = modelMatrix1[0][0] * meshes["target"]->vertices[2].position.x +
				modelMatrix1[1][0] * meshes["target"]->vertices[2].position.y + modelMatrix1[2][0];
			recC.y = modelMatrix1[1][1] * meshes["target"]->vertices[2].position.y +
				modelMatrix1[0][1] * meshes["target"]->vertices[2].position.x + modelMatrix1[2][1];

			recD.x = modelMatrix1[0][0] * meshes["target"]->vertices[3].position.x +
				modelMatrix1[1][0] * meshes["target"]->vertices[3].position.y + modelMatrix1[2][0];
			recD.y = modelMatrix1[1][1] * meshes["target"]->vertices[3].position.y +
				modelMatrix1[0][1] * meshes["target"]->vertices[3].position.x + modelMatrix1[2][1];
			calc = true;
		}

		{
			if (verif0)
			{
				angularStepWing0 += 2 * deltaTimeSeconds;
				if (angularStepWing0 > 0.3)
					verif0 = false;
			}
			else
			{
				angularStepWing0 -= 2 * deltaTimeSeconds;
				if (angularStepWing0 < -0.3)
				{
					verif0 = true;
				}
			}
			if (cont0)
			{
				angularStepWing0 = 0;
				cont0 = 0;
			}
			modelMatrix = glm::mat3(1);
			modelMatrix1 = glm::mat3(1);
			modelMatrix *= transform2D::Translate(wing0X, wing0Y);
			modelMatrix *= transform2D::Rotate(angularStepWing0);
			modelMatrix *= transform2D::Rotate(randAng);
			modelMatrix *= transform2D::Rotate(-90);
			modelMatrix *= transform2D::Translate(wing0X, wing0Y);
			glm::vec3 vecWing0 = modelMatrix * glm::vec3(meshes["wing0"]->vertices[2].position.x, meshes["wing0"]->vertices[2].position.y, 1);
			modelMatrix1 *= transform2D::Translate(wing0X - vecWing0.x, wing0Y - vecWing0.y);
			modelMatrix1 *= transform2D::Translate(wing0X, wing0Y);
			modelMatrix1 *= transform2D::Rotate(angularStepWing0);
			modelMatrix1 *= transform2D::Rotate(randAng);
			modelMatrix1 *= transform2D::Rotate(-90);
			modelMatrix1 *= transform2D::Translate(wing0X, wing0Y);
			RenderMesh2D(meshes["wing0"], shaders["VertexColor"], modelMatrix1);

			if (verif1)
			{
				angularStepWing1 -= 2 * deltaTimeSeconds;
				if (angularStepWing1 < -0.3)
					verif1 = false;
			}
			else
			{
				angularStepWing1 += 2 * deltaTimeSeconds;
				if (angularStepWing1 > 0.3)
				{
					verif1 = true;
				}
			}
			if (cont1)
			{
				angularStepWing1 = 0;
				cont1 = 0;
			}
			modelMatrix = glm::mat3(1);
			modelMatrix1 = glm::mat3(1);
			modelMatrix *= transform2D::Translate(wing1X, wing1Y);
			modelMatrix *= transform2D::Rotate(angularStepWing1);
			modelMatrix *= transform2D::Rotate(randAng);
			modelMatrix *= transform2D::Rotate(90);
			modelMatrix *= transform2D::Translate(wing1X, wing1Y);
			glm::vec3 vecWing1 = modelMatrix * glm::vec3(meshes["wing1"]->vertices[2].position.x, meshes["wing1"]->vertices[2].position.y, 1);
			modelMatrix1 *= transform2D::Translate(wing1X - vecWing1.x, wing1Y - vecWing1.y);
			modelMatrix1 *= transform2D::Translate(wing1X, wing1Y);
			modelMatrix1 *= transform2D::Rotate(angularStepWing1);
			modelMatrix1 *= transform2D::Rotate(randAng);
			modelMatrix1 *= transform2D::Rotate(90);
			modelMatrix1 *= transform2D::Translate(wing1X, wing1Y);
			RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix1);

		}

		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate((float)randX, (float)(-6 * cy));
			modelMatrix *= transform2D::Translate(translateX, translateY);
			modelMatrix *= transform2D::Translate(cx, cy);
			modelMatrix *= transform2D::Rotate(randAng);
			modelMatrix *= transform2D::Translate(-cx, -cy);
			modelMatrix *= transform2D::Scale(0.5, 1);
			glm::vec3 vecNeck = (modelMatrix)*glm::vec3(meshes["body"]->vertices[2].position.x, meshes["body"]->vertices[2].position.y, 1);
			glm::vec3 vecWing0 = (modelMatrix)*glm::vec3(meshes["body"]->vertices[0].position.x, meshes["body"]->vertices[0].position.y, 1);
			glm::vec3 vecWing1 = (modelMatrix)*glm::vec3(meshes["body"]->vertices[1].position.x, meshes["body"]->vertices[1].position.y, 1);
			wing0X = (vecNeck.x + vecWing0.x) / 2;
			wing0Y = (vecNeck.y + vecWing0.y) / 2;
			wing1X = (vecNeck.x + vecWing1.x) / 2;
			wing1Y = (vecNeck.y + vecWing1.y) / 2;
			neckX = vecNeck.x;
			neckY = vecNeck.y;
			glm::vec3 vecCol;
			modelMatrix2 = glm::mat3(1);
			modelMatrix2 *= transform2D::Translate(neckX, neckY);
			modelMatrix2 *= transform2D::Scale(17, 17);
			vecCol = modelMatrix2 * glm::vec3(meshes["head"]->vertices[NUM_TRIANGLES / 2].position.x,
				meshes["head"]->vertices[NUM_TRIANGLES / 2].position.y, 1);
			if (!escape && !killed)
				end = std::chrono::steady_clock::now();
			if (!(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() >= 18 || escape || bulCnt == 3 || killed))
			{
				if (((int)vecCol.x < 5) && (int)vecCol.y != 0)
				{

					dirX = -dirX;
					randAng = inv + atan(dirY / dirX);
					firstTime = true;
					modelMatrix *= transform2D::Translate(vecCol.x - neckX, vecCol.y - neckY);
				}
				else
					if (((int)vecCol.x != 0 && (int)vecCol.y > resY - 10))
					{
						dirX = -dirX;
						inv = (float)M_PI;
						randAng = inv + atan(dirY / dirX);
						firstTime = true;
						modelMatrix *= transform2D::Translate(vecCol.x - neckX, vecCol.y - neckY);
					}
					else
						if (((int)vecCol.x > resX - 10 && (int)vecCol.y != 0))
						{
							dirY = -dirY;
							randAng = inv + atan(dirY / dirX);
							firstTime = true;
							modelMatrix *= transform2D::Translate(vecCol.x - neckX, vecCol.y - neckY);
						}
						else
							if (((int)vecCol.x != 0 && (int)vecCol.y < 5))
							{
								if (firstTime)
								{
									dirX = -dirX;
									inv = 0;
									randAng = inv + atan(dirY / dirX);
									modelMatrix *= transform2D::Translate(vecCol.x - neckX, vecCol.y - neckY);
								}
							}
				if(lucky == 1)
					RenderMesh2D(meshes["lucky"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
			}
			else
			{
				if (lucky == 1)
					RenderMesh2D(meshes["lucky"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
				if (killed && !helper)
				{
					begin = std::chrono::steady_clock::now();
					randAng = RADIANS(-180);
					helper = true;
				}
				else
					if (!escape && !helper)
					{
						randAng = 0;
						begin = std::chrono::steady_clock::now();
						escape = true;
						heartCnt++;
					}
				RenderMesh2D(meshes["died"], shaders["VertexColor"], glm::mat3(1));
				end = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() >= 2.75f)
				{
					if (lucky == 1 && !escape && heartCnt != 0)
						heartCnt--;
					std::random_device dv;
					std::mt19937 rn(dv());
					std::uniform_real_distribution<float> dist6(-55, 55);
					randAng = RADIANS(dist6(rn));
					std::uniform_int_distribution<std::mt19937::result_type> dist7(500, 1000);
					randX = dist7(rn);
					std::uniform_real_distribution<float> dist8(0, 4);
					lucky = dist8(rn);
					begin = std::chrono::steady_clock::now();
					angularStepWing0 = 0;
					angularStepWing1 = 0;

					neckX = 0;
					neckY = 0;

					verif0 = true;
					cont0 = 1;
					verif1 = true;
					cont1 = 1;
					if (numKills >= 5 && numKills % 5 == 0)
					{
						speed += 95;
					}
					if (heartCnt == 3 || scaleScoreX == scoreX + 1)
						gameOver = true;
					else
						bulCnt = 0;
					translateX = 0;
					translateY = 0;

					dirX = 3;
					dirY = 2;
					inv = 0;

					firstTime = false;
					escape = false;
					killed = false;
					helper = false;
				}
			}

		}

		{
			int count = 1049;
			int count_bullets = 100;
			for (int i = bulCnt; i < 3; i++)
			{
				modelMatrix1 = glm::mat3(1);
				modelMatrix1 *= transform2D::Translate((float)(count_bullets + 75 * i), (float)0);
				RenderMesh2D(meshes["bullet" + to_string(i)], shaders["VertexColor"], modelMatrix1);
			}
			for (int i = heartCnt; i < 3; i++)
			{
				modelMatrix = glm::mat3(1);
				modelMatrix *= transform2D::Translate((float)(count + 75 * i), (float)650);
				modelMatrix *= transform2D::Scale(25, 25);
				RenderMesh2D(meshes["heart" + to_string(i)], shaders["VertexColor"], modelMatrix);
			}
		}

		{
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate((float)(resX - 250), (float)(resY - 250));
			modelMatrix *= transform2D::Scale(scaleScoreX, 1);
			RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["skel_score"], shaders["VertexColor"], glm::mat3(1));

		}

		RenderMesh2D(meshes["sky"], shaders["VertexColor"], glm::mat3(1));
	}

}


void Tema1::FrameEnd()
{

}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{

}

float area(int x1, int y1, int x2, int y2,
	int x3, int y3)
{
	return (float)abs((x1 * (y2 - y3) + x2 * (y3 - y1) +
		x3 * (y1 - y2)) / 2.0);
}

bool check(int x1, int y1, int x2, int y2, int x3,
	int y3, int x4, int y4, int x, int y)
{
	float Area = area(x1, y1, x2, y2, x3, y3) +
		area(x1, y1, x4, y4, x3, y3);

	float Area1 = area(x, y, x1, y1, x2, y2);
	float Area2 = area(x, y, x2, y2, x3, y3);
	float Area3 = area(x, y, x3, y3, x4, y4);
	float Area4 = area(x, y, x1, y1, x4, y4);
	return (Area >= (Area1 + Area2 + Area3 + Area4));
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	bulCnt++;
	if (check((int)recA.x, (int)recA.y, (int)recB.x, (int)recB.y, (int)recC.x, (int)recC.y,
		(int)recD.x, (int)recD.y, mouseX, resY - mouseY) && bulCnt <= 3 && calc && !killed && !escape)
	{
		killed = true;
		if (scaleScoreX < scoreX)
			scaleScoreX += 10;
		numKills++;
	}
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}


void Tema1::OnWindowResize(int width, int height)
{

}
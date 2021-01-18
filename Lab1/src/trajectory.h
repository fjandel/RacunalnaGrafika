#pragma once

#include "mesh.h"
#include <math.h>

class Trajectory
{
public:
	std::vector<Vertex> R;
	const glm::mat4 B = glm::mat4(-1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0);
	const glm::mat4x3 BDer = glm::mat4x3(-1, 2, -1, 3, -4, 0, -3, 2, 1, 1, 0, 0);

	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// constructor, expects a filepath to a trajectory.
	Trajectory(string const& path)
	{
		loadTrajectory(path);
		setupTrajectory();
	}

	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupTrajectory()
	{
		calculateVertices();

		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glBindVertexArray(0);
	}

	void calculateVertices()
	{
		std::vector<Vertex> path;
		std::vector<unsigned int> currentIndices;
		for (int i = 1; i < R.size() - 2; i++)
		{
			for (float t = 0.f; t < 1.f; t += 0.1f)
			{
				Vertex v = calculateSingleVertex(i, t);
				path.push_back(v);
			}
		}

		for (size_t i = 0; i < path.size(); i++)
			currentIndices.push_back(i);

		this->vertices = path;
		this->indices = currentIndices;
	}

	Vertex calculateSingleVertex(__int64 i, float t)
	{
		glm::mat3x4 matR = glm::mat3x4(R[i - 1].Position.x, R[i].Position.x, R[i + 1].Position.x, R[i + 2].Position.x,
			R[i - 1].Position.y, R[i].Position.y, R[i + 1].Position.y, R[i + 2].Position.y,
			R[i - 1].Position.z, R[i].Position.z, R[i + 1].Position.z, R[i + 2].Position.z);

		Vertex vertex;
		vertex.Position = calculatePosition(i, t, matR);
		vertex.Tangent = calculateTangent(i, t, matR);

		return vertex;
	}

	glm::vec3 calculatePosition(__int64 i, float t, glm::mat3x4 matR)
	{
		glm::vec4 vecT = glm::vec4(t * t * t, t * t, t, 1);
		glm::vec3 position = vecT * (1.0f / 6.0f) * B * matR;

		return position;
	}

	glm::vec3 calculateTangent(__int64 i, float t, glm::mat3x4 matR)
	{
		glm::vec3 vecTDer = glm::vec3(t * t, t, 1);
		glm::vec3 tangent = vecTDer * 0.5f * BDer * matR;
		
		return tangent;
	}

	void loadTrajectory(std::string const& path)
	{
		std::ifstream trajectoryFile(path);
		std::string tmp;
		float x, y, z;
		std::vector<Vertex> currentR;
		while (std::getline(trajectoryFile, tmp))
		{
			sscanf_s(tmp.c_str(), "%f %f %f", &x, &y, &z);
			Vertex vertex;
			glm::vec3 vector;
			vector.x = x / 5.0f;
			vector.y = y / 5.0f;
			vector.z = z / 5.0f;
			vertex.Position = vector;

			currentR.push_back(vertex);
		}

		this->R = currentR;
	}
};
#pragma once
#ifdef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	// create a program ID
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		// step 1: get the vertex and fragment source code from the filepath

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		// base case: throw an exception incase any errors
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			
			// read files buffer contents into streams
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			// close the files
			vertexShaderFile.close();
			fragementShaderFile.close();

			// convert the stream into string
			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();

		}
		catch (std::ifstream::failure &e) {
			std::cout << "ERROR::SHADER::FILE_ERROR" << e.what() << std::endl;
		}

		// convert the string into char
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();





		// Step 2: compile the shaders -------------------------------------------------------------------------------------------------------------------------------------
		unsigned int vertex;
		unsigned int fragment;

		// set up vertex shaders:
		vertex = glCreateShader(GL_VERTEX_SHADER); 
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompilerShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// set up fragment shaders:
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		checkCompileErrors(fragment, "FRAGMENT");

		// attach and link the vertex and fragment shaders to the shader program
		ID = glCreateProgram();
		glAttatchShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// finally delete the shaders after linking
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}



	// Helper functions for later ------------------------------------------------------------------------------------------------------------------------------------------
	void use() const
	{
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}




	// print error log ----------------------------------------------------------------------------------------------------------------------------------------------------- 
private:
	// utility function for checking shader compilation and linking errors
	void checkCompileErrors(unsigned int Shader, std::string type) {
		int success;
		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetShaderiv(Shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

#endif // SHADER_H


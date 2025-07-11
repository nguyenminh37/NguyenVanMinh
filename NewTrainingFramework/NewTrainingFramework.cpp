// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>

GLuint vboId;
unsigned int iboId;

GLuint posLocation;

Shaders myShaders;
GLuint textureID;
int Init(ESContext* esContext)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    //triangle data (heap)
    Vertex verticesData[3];

    verticesData[0].pos.x = 0.0f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
    verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
    verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;

    /*verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f;
    verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f;
    verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f;*/

    verticesData[0].uv.x = 0.5f;  verticesData[0].uv.y = 1.0f;
    verticesData[1].uv.x = 0.0f;  verticesData[1].uv.y = 0.0f;
    verticesData[2].uv.x = 0.0f;  verticesData[2].uv.y = 1.0f;

    //buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    int iWidth, iHeight, bpp;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    char* imageData = LoadTGA("../Resources/Textures/Woman1.tga", &iWidth, &iHeight, &bpp);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int indicesData[] = { 0 ,1 ,2 };
    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //creation of shaders and program 
    return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw(ESContext* esContext)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(myShaders.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int iTextureLoc = glGetUniformLocation(myShaders.program, "u_texture");
    glUniform1i(iTextureLoc, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    // take location value from vertex/fragment shader. With Position we have location = 0
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));
    }

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);


}

void Update(ESContext* esContext, float deltaTime)
{

}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{

}

void CleanUp()
{
    glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
    ESContext esContext;

    esInitContext(&esContext);

    esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (Init(&esContext) != 0)
        return 0;

    esRegisterDrawFunc(&esContext, Draw);
    esRegisterUpdateFunc(&esContext, Update);
    esRegisterKeyFunc(&esContext, Key);

    esMainLoop(&esContext);
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    //releasing OpenGL resources
    CleanUp();

    //identifying memory leaks
    MemoryDump();
    printf("Press any key...\n");
    _getch();

    return 0;
}

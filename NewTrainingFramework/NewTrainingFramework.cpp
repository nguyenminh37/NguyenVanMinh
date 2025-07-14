// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include "NFG.h"
#include <iostream>
GLuint vboId;              
GLuint iboId;             
GLuint posLocation;        

Shaders myShaders;
GLuint textureID;
Vertex* verticesData;
unsigned int* indices;

int vertexCount = 0, indexCount = 0;
bool LoadNFG(const char* filename)
{
    FILE* f;

    if (fopen_s(&f, filename, "rb") != 0)
    {
        std::cout << "Cannot open file.\n";
        return false;
    }

    char temp[256];
    int nrVertices = 0;
    fscanf_s(f, "%s %d", temp, sizeof(temp), &nrVertices); // NrVertices: <num>

    verticesData = new Vertex[nrVertices];

    for (int i = 0; i < nrVertices; i++)
    {
        int index;
        Vertex v;

        fscanf_s(f, "%d.", &index);
        fscanf_s(f, " pos:[%f, %f, %f];", &v.pos.x, &v.pos.y, &v.pos.z);
        fscanf_s(f, " norm:[%f, %f, %f];", &v.normal.x, &v.normal.y, &v.normal.z);
        fscanf_s(f, " binorm:[%f, %f, %f];", &v.binormal.x, &v.binormal.y, &v.binormal.z);
        fscanf_s(f, " tgt:[%f, %f, %f];", &v.tangent.x, &v.tangent.y, &v.tangent.z);
        fscanf_s(f, " uv:[%f, %f];", &v.uv.x, &v.uv.y);

        verticesData[i] = v;
    }

    int nrIndices = 0;
    fscanf_s(f, "%s %d", temp, sizeof(temp), &nrIndices); // NrIndices: <num>

    indices = new unsigned int[nrIndices];

    for (int i = 0; i < nrIndices / 3; i++)
    {
        int idxLine;
        unsigned int a, b, c;

        fscanf_s(f, "%d.", &idxLine);
        fscanf_s(f, " %d, %d, %d", &a, &b, &c);

        indices[i * 3 + 0] = a;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = c;
    }

    fclose(f);

    indexCount = nrIndices;

    // Upload to GPU
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, nrVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

int Init(ESContext* esContext)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    LoadNFG("../Resources/Models/Woman1.nfg");
 
   
    // === Load texture ===
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

    
   

    // === Shader loading ===
    return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Draw(ESContext* esContext)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(myShaders.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(myShaders.program, "u_texture"), 0);

 
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)0 + sizeof(Vector3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)12 + sizeof(Vector3));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)24 + sizeof(Vector3));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)36 + sizeof(Vector3)); 
    

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
    // UNUSED - no update logic for now
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
    // UNUSED - no keyboard input for now
}

void CleanUp()
{

}

int _tmain(int argc, _TCHAR* argv[])
{
    ESContext esContext;

    esInitContext(&esContext);
    esCreateWindow(&esContext, "Model Viewer", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (Init(&esContext) != 0)
        return 0;

    esRegisterDrawFunc(&esContext, Draw);
    esRegisterUpdateFunc(&esContext, Update);
    esRegisterKeyFunc(&esContext, Key);

    esMainLoop(&esContext);

    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    CleanUp();

    MemoryDump();
    printf("Press any key...\n");
    _getch();

    return 0;
}

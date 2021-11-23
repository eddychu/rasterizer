#pragma once
#include "Scene.h"
#include "Rasterizer.h"
struct PBRScene : public Scene
{
    PBRScene()
        : camera(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0)), shader()
    {
    }
    void Init()
    {
        // auto head = new Mesh("assets/african_head", "african_head");
        auto mesh = new Mesh("assets/african_head", "african_head");

        auto eyeinner = new Mesh("assets/african_head", "african_head_eye_inner");
        auto eyeOuter = new Mesh("assets/african_head", "african_head_eye_outer");
        meshes.push_back(mesh);
        // meshes.push_back(eyeOuter);
        meshes.push_back(eyeinner);
    }

    void Update(float delta)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = camera.GetProjectionMatrix();
        auto mv = view * model;
        shader.uniforms.mvp = proj * view * model;
        shader.uniforms.mv = mv;
        shader.isMetal = true;
        shader.roughness = 0.5f;
        shader.color = vec3(1.f, 0.71f, 0.29f);
        vec4 lightPos = view * glm::vec4(0, 0, 1, 2.0f);
        shader.lightPos = lightPos;
        shader.uniforms.normalMatrix = glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
    }

    void Render(PixelBuffer &pixelBuffer, DepthBuffer &depthBuffer)
    {
        vec3 vertices[3];
        vec3 normals[3];
        vec2 uvs[3];
        vec3 faceNormal;
        vec4 clipCoords[3];

        for (auto &mesh : meshes)
        {
            shader.samplers.diffuse = mesh->GetDiffuse();
            shader.samplers.specular = mesh->GetSpecular();
            shader.samplers.normal = mesh->GetNormal();
            // shader.tangentTexture = mesh->GetTangent();
            for (int i = 0; i < mesh->GetFaceNumbers(); i++)
            {
                vec3 tangent = mesh->tangent(i);
                vec3 bitangent = mesh->bitangent(i);
                for (int j = 0; j < 3; j++)
                {
                    vertices[j] = mesh->vertice(i, j);
                    uvs[j] = mesh->uv(i, j);
                    normals[j] = mesh->normal(i, j);
                }

                for (int j = 0; j < 3; j++)
                {
                    clipCoords[j] = shader.Vertex(VertexAttributes(vertices[j], normals[j], uvs[j], tangent, bitangent), j);
                }

                if (Rasterizer::Clipping(clipCoords))
                    continue;

                Rasterizer::PerspectiveDivide(clipCoords);
                Rasterizer::ViewPortTransform(pixelBuffer, clipCoords);
                if (Rasterizer::FaceCulling(clipCoords))
                    continue;
                Rasterizer::Rasterize(pixelBuffer, depthBuffer, &shader, clipCoords);
            }
        }
    }

    void ProcessInput(SDL_Event &event)
    {
        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            camera.OnMouseMove(event.motion.xrel, event.motion.yrel);
            break;
        }
    }

    ~PBRScene()
    {
        for (auto &mesh : meshes)
        {
            delete mesh;
        }
        meshes.clear();
    }

    std::vector<Mesh *> meshes;
    PBRShader shader;
    Camera camera;
};

#include "Core/Renderer.h"
#include "Scenes/BlinnPhongScene.h"
#include "Scenes/PBRScene.h"
int main()
{
    Renderer renderer("Rasterizer v1.0", 1600, 1200);
    // Scene *scene = new BlinnPhongScene();
    Scene *scene = new PBRScene();
    renderer.Init(scene);
    renderer.Run();
}

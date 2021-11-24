
#include "Core/Renderer.h"
#include "Scenes/BlinnPhongScene.h"
int main()
{
	Renderer renderer("BabyRaster v1.0", 1600, 1200);
    Scene* scene = new BlinnPhongScene();
    renderer.Init(scene);
	renderer.Run();
}

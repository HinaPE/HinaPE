#include "renderer/app.h"
#include "_geometry/sphere3.h"

class Surface : public Kasumi::Api
{
public:
	void prepare() final
	{
		Api::prepare();
		auto sphere = std::make_shared<HinaPE::Sphere3>();

		_sphere_obj = std::make_shared<Kasumi::SceneObject>(std::move(std::make_shared<Kasumi::Model>("sphere", Color::RED)));
	}
	void step(float dt) final
	{
		Api::step(dt);
	}
	void ui_sidebar() final
	{
		Api::ui_sidebar();
	}

private:
	Kasumi::SceneObjectPtr _sphere_obj;
};

auto main() -> int
{
	std::make_shared<Kasumi::Renderer>("empty.txt")
			->load_api(std::make_shared<Surface>())
			->launch();
	return 0;
}
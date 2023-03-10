#include "backends/api.h"

class SimpleModelViewer final : public Kasumi::App
{
public:
	explicit SimpleModelViewer(const std::string &model_path, real scale = 1)
	{
		_model = std::make_shared<Kasumi::Model>(model_path, scale);
		close_inspector();
		close_benchmark();
	}

protected:
	void update(double dt) final { _model->render(); }
	Kasumi::ModelPtr _model;
};

auto main() -> int
{
	std::make_shared<SimpleModelViewer>(std::string(BackendsModelDir) + "Hatsune_Miku_V4X/Hatsune_Miku_V4X.pmx", 0.2)->launch();
	return 0;
}
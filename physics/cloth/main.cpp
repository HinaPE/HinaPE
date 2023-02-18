#include "backends/app.h"

#include "pbd/solver.h"

#include "scene.h"
#include "objects/cloth.h"

class App : public Kasumi::App
{
protected:
	void prepare() final
	{
		// physics
		_solver = std::make_shared<HinaPE::PBDClothSolver>();

		// renderer
		_scene = std::make_shared<Kasumi::Scene3D>();
		_cloth = std::make_shared<Kasumi::ClothObject>();

		std::vector<Kasumi::Mesh::Vertex> vertices_;
		for (auto &v: _solver->_data->_positions)
		{
			Kasumi::Mesh::Vertex v_;
			v_.position = v;
			v_.normal = mVector3(0, 1, 0);
			v_.tex_coord = {0.5f - v.x() / static_cast<float>(_solver->_data->_opt.width), 0.5f - v.y() / static_cast<float>(_solver->_data->_opt.height)};
			vertices_.emplace_back(v_);
		}

		std::vector<Kasumi::Mesh::Index> indices_;
		for (auto &i : _solver->_data->_init_indices)
		{
			indices_.emplace_back(i);
		}

		std::map<std::string, std::vector<Kasumi::TexturePtr>> textures_;
		textures_["diffuse"].push_back(std::make_shared<Kasumi::Texture>(std::string(MyAssetDir) + "miku.jpg"));
		_cloth->init(std::move(vertices_), std::move(indices_), std::move(textures_));
		_scene->add(_cloth);
	}
	void update(double dt) final
	{
		_timer.reset();
		_solver->step(dt);
		sync();
		_last_update_time = _timer.duration();
	}
	void sync()
	{
		_cloth->update(_solver->_data->_positions);
		_scene->draw();
	}
	void key(int key, int scancode, int action, int mods) final { _scene->key(key, scancode, action, mods); }
	void mouse_button(int button, int action, int mods) final { _scene->mouse_button(button, action, mods); }
	void mouse_scroll(double x_offset, double y_offset) final { _scene->mouse_scroll(x_offset, y_offset); }
	void mouse_cursor(double x_pos, double y_pos) final { _scene->mouse_cursor(x_pos, y_pos); }

private:
	// physics
	HinaPE::PBDClothSolverPtr _solver;

	// renderer
	Kasumi::Scene3DPtr _scene;
	Kasumi::ClothObjectPtr _cloth;
};

auto main() -> int
{
	std::make_shared<App>()->launch();
	return 0;
}

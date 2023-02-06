#include "renderer/app.h"

#include "GLFW/glfw3.h"
#include "nfd/nfd.h"
#include "imgui.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

#include "util/parallel.h"

class XyzViewer : public Kasumi::Api
{
public:
	void prepare() final
	{
		char *path = nullptr;
		nfdresult_t result = NFD_OpenDialog("xyz", nullptr, &path);
		std::filesystem::path const p(path);
		if (result != NFD_OKAY)
			return;

		auto directory = p.parent_path().string();

		std::map<int, std::filesystem::path> temp;
		for (const auto &entry: std::filesystem::directory_iterator(directory))
		{
			if (entry.path().extension().string() != ".xyz")
				continue;

			auto filename = entry.path().stem().string();

			auto pos = filename.find_first_of("0123456789");
			if (pos == std::string::npos)
				return;

			auto digits = filename.substr(pos, filename.size());
			if (!std::all_of(digits.begin(), digits.end(), ::isdigit))
				return;
			int index = std::stoi(digits);

			temp[index] = entry.path();

			std::vector<mMatrix4x4> poses;
			_particles_frames[index] = poses;
		}
		_current_frame = _particles_frames.begin();

		HinaPE::Util::parallelFor((size_t) 0, _particles_frames.size(), [&](size_t i)
		{
			auto begin_iter = _particles_frames.begin();
			for (int j = 0; j < i; ++j)
				begin_iter++;

			auto filepath = temp[begin_iter->first];
			std::ifstream file(filepath);
			std::string line;
			while (std::getline(file, line))
			{
				std::stringstream ss(line);
				mVector3 point;
				ss >> point.x() >> point.y() >> point.z();

				Kasumi::Pose pose;
				pose.position = {point.x(), point.y(), point.z()};
				pose.scale = {0.01, 0.01, 0.01};
				begin_iter->second.push_back(pose.get_model_matrix().transposed());
			}
			std::cout << "Loaded file: " << filepath << std::endl;
		});

		auto fluid_model = std::make_shared<Kasumi::Model>("cube", HinaPE::Color::RED);
		fluid_model->instancing();
		_fluid_model = _scene->get_object(_scene->add_object(fluid_model));

		sync();
	}

	void key(int key, int scancode, int action, int mods) final
	{
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			_current_frame--;
			if (std::distance(_particles_frames.begin(), _current_frame) <= 0)
			{
				_current_frame = _particles_frames.begin();
				return;
			}
			sync();
		} else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			_current_frame++;
			if (std::distance(_current_frame, _particles_frames.end()) <= 0)
			{
				_current_frame = (--_particles_frames.end());
				return;
			}
			sync();
		}
	}

	void ui_sidebar() final
	{
		if (_particles_frames.empty())
			return;
		ImGui::Text("Frame: %d", _current_frame->first);
	}

	void step(float dt) override
	{
		_current_frame++;
		if (std::distance(_current_frame, _particles_frames.end()) <= 0)
		{
			_current_frame = (--_particles_frames.end());
			return;
		}
		sync();
	}

	void sync() const
	{
		_fluid_model->get_model()->clear_instances();
		_fluid_model->get_model()->set_instance_matrices(_current_frame->second);
	}

	std::map<int, std::vector<mMatrix4x4>> _particles_frames;
	std::map<int, std::vector<mMatrix4x4>>::iterator _current_frame = _particles_frames.begin();
	Kasumi::SceneObjectPtr _fluid_model;
};

auto main() -> int
{
	auto renderer = std::make_shared<Kasumi::Renderer>("empty.txt");
	renderer->get_scene()->get_camera()->_opt.radius = 7;
	renderer->load_api(std::make_shared<XyzViewer>());
	renderer->launch();
	return 0;
}
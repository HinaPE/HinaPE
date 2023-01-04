#include "renderer/app.h"

#include "GLFW/glfw3.h"
#include "nfd/nfd.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include "imgui.h"

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

		for (const auto &entry: std::filesystem::directory_iterator(directory))
		{
			auto filename = entry.path().stem().string();
			std::cout << "Loading file: " << filename << std::endl;

			auto pos = filename.find_first_of("0123456789");
			if (pos == std::string::npos)
				return;

			auto digits = filename.substr(pos, filename.size());
			if (!std::all_of(digits.begin(), digits.end(), ::isdigit))
				return;
			int index = std::stoi(digits);

			std::ifstream file(entry.path());
			std::string line;
			std::vector<Kasumi::Pose> poses;
			while (std::getline(file, line))
			{
				std::stringstream ss(line);
				mVector3 point;
				ss >> point.x >> point.y >> point.z;

				Kasumi::Pose pose;
				pose.position = {point.x, point.y, point.z};
				pose.scale = {0.01, 0.01, 0.01};
				poses.push_back(pose);
			}
			_particles_frames[index] = poses;
		}
		_current_frame = _particles_frames.begin();

		auto fluid_model = std::make_shared<Kasumi::Model>("cube", Color::RED);
		fluid_model->instancing();
		_fluid_model = _scene->get_object(_scene->add_object(fluid_model));

		sync();
	}

	void key(int key, int scancode, int action, int mods) final
	{
		if (key == GLFW_KEY_LEFT)
		{
			if (_current_frame == _particles_frames.begin())
				return;
			_current_frame--;
			sync();
		} else if (key == GLFW_KEY_RIGHT)
		{
			if (_current_frame == _particles_frames.end())
				return;
			_current_frame++;
			sync();
		}
	}

	void ui_sidebar() final
	{
		ImGui::Text("Frame: %d", _current_frame->first);
	}

	void sync() const
	{
		_fluid_model->get_model()->clear_instances();
		_fluid_model->get_model()->add_instances(_current_frame->second);
	}

	std::map<int, std::vector<Kasumi::Pose>> _particles_frames;
	std::map<int, std::vector<Kasumi::Pose>>::const_iterator _current_frame;
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
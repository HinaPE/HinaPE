#include "renderer/app.h"

#include <iostream>
auto main() -> int
{
	std::cout << "Hello Kasumi~" << std::endl;

	auto kasumi = std::make_shared<Kasumi::Renderer>();

	auto scene = kasumi->get_scene();

	auto miku = std::make_shared<Kasumi::Model>(std::string(MyAssetsDir) + "miku.pmx");

	scene->add_object(miku);

	kasumi->launch();
	return 0;
}
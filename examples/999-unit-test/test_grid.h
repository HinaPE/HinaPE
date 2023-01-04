#ifndef HINAPE_TEST_GRID_H
#define HINAPE_TEST_GRID_H

#include "renderer/app.h"
#include "grid/cell_centered_scalar_grid3.h"

class TestGrid : public Kasumi::Api
{
public:
	void prepare() final;

private:
	// scene part
	Kasumi::SceneObjectPtr _grid_obj;
};

#endif //HINAPE_TEST_GRID_H

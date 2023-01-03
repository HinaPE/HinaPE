#ifndef HINAPE_TEST_PCISPH_H
#define HINAPE_TEST_PCISPH_H

#include "renderer/app.h"

class TestPciSPH : public Kasumi::Api
{
public:
	void prepare() final;
	void key(int key, int scancode, int action, int mods) final;

protected:
	void sync() const;
	void one_frame() const;

private:

};

#endif //HINAPE_TEST_PCISPH_H

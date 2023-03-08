#include "cloth.h"

HinaPE::ClothObject::ClothObject(real width, real height, size_t rows, size_t cols) : _width(width), _height(height), _rows(rows), _cols(cols)
{
	NAME = "Cloth";
	_shader = Kasumi::Shader::DefaultMeshShader;
	build();
}
void HinaPE::ClothObject::build()
{
	_inv_masses.clear();
	_init_vertices.clear();
	_init_indices.clear();
	_init_edges.clear();

	_positions.clear();
	_pre_positions.clear();
	_velocities.clear();
	_forces.clear();

	auto &_vertices = _init_vertices;
	auto &_indices = _init_indices;
	auto &_edges = _init_edges;


	_vertices.resize(_cols * _rows);
	_indices.resize((_cols - 1) * (_rows - 1) * 6);
	for (int j = 0; j < _rows; ++j)
		for (int i = 0; i < _cols; ++i)
			_vertices[j * _cols + i] = mVector3(_width * (static_cast<real>(i) / (static_cast<real>(_cols) - 1.f)), _height * (static_cast<real>(j) / (static_cast<real>(_rows) - 1.f)), 0.f) - mVector3{_width / 2.f, _height / 2.f, 0};
	for (int j = 0; j < _rows - 1; ++j)
	{
		for (int i = 0; i < _cols - 1; ++i)
		{
			_indices[(j * (_cols - 1) + i) * 6 + 0] = j * _cols + i;
			_indices[(j * (_cols - 1) + i) * 6 + 1] = j * _cols + i + 1;
			_indices[(j * (_cols - 1) + i) * 6 + 2] = (j + 1) * _cols + i;
			_indices[(j * (_cols - 1) + i) * 6 + 3] = (j + 1) * _cols + i;
			_indices[(j * (_cols - 1) + i) * 6 + 4] = j * _cols + i + 1;
			_indices[(j * (_cols - 1) + i) * 6 + 5] = (j + 1) * _cols + i + 1;
		}
	}
	for (int i = 0; i < _indices.size(); i += 3)
	{
		std::pair<unsigned int, unsigned int> e1 = std::make_pair(_indices[i], _indices[i + 1]);
		std::pair<unsigned int, unsigned int> e2 = std::make_pair(_indices[i + 1], _indices[i + 2]);
		std::pair<unsigned int, unsigned int> e3 = std::make_pair(_indices[i + 2], _indices[i]);
		if (std::none_of(_edges.begin(), _edges.end(), [&e1](const std::pair<unsigned int, unsigned int> &e) { return (e.first == e1.first && e.second == e1.second) || (e.first == e1.second && e.second == e1.first); }))
			_edges.emplace_back(e1);
		if (std::none_of(_edges.begin(), _edges.end(), [&e2](const std::pair<unsigned int, unsigned int> &e) { return (e.first == e2.first && e.second == e2.second) || (e.first == e2.second && e.second == e2.first); }))
			_edges.emplace_back(e2);
		if (std::none_of(_edges.begin(), _edges.end(), [&e3](const std::pair<unsigned int, unsigned int> &e) { return (e.first == e3.first && e.second == e3.second) || (e.first == e3.second && e.second == e3.first); }))
			_edges.emplace_back(e3);
	}

	_positions.resize(_vertices.size());
	_pre_positions.resize(_vertices.size());
	_velocities.resize(_vertices.size());
	_forces.resize(_vertices.size());
	_inv_masses.resize(_vertices.size());
	std::copy(_vertices.begin(), _vertices.end(), _positions.begin());
	_pre_positions = _positions;
	std::fill(_velocities.begin(), _velocities.end(), mVector3(0.f, 0.f, 0.f));
	std::fill(_forces.begin(), _forces.end(), mVector3(0.f, 0.f, 0.f));
	std::fill(_inv_masses.begin(), _inv_masses.end(), 1.f);
	_inv_masses[_cols * (_rows - 1)] = 0;
	_inv_masses[_cols * _rows - 1] = 0;

	// setup mesh
	std::vector<Kasumi::Mesh::Vertex> vertices_;
	for (auto &v: _positions)
	{
		Kasumi::Mesh::Vertex v_;
		v_.position = v;
		v_.normal = mVector3(0, 1, 0);
		v_.tex_coord = {0.5f - v.x() / static_cast<float>(_width), 0.5f - v.y() / static_cast<float>(_height)};
		vertices_.emplace_back(v_);
	}

	std::vector<Kasumi::Mesh::Index> indices_;
	for (auto &i: _init_indices)
	{
		indices_.emplace_back(i);
	}

	std::map<std::string, std::vector<Kasumi::TexturePtr>> textures_;
	textures_["diffuse"].push_back(std::make_shared<Kasumi::Texture>(std::string(MyAssetDir) + "miku.jpg"));

	_init(std::move(vertices_), std::move(indices_), std::move(textures_));
}
void HinaPE::ClothObject::_update_surface()
{
	auto &_v = _mesh->vertices();
	assert(_v.size() == _positions.size());
	for (int i = 0; i < _v.size(); ++i)
		_v[i].position = _positions[i];
}

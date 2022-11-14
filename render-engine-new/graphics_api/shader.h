#ifndef HINAPE_RENDER_ENGINE_PRO_SHADER_H
#define HINAPE_RENDER_ENGINE_PRO_SHADER_H

#include "math_api.h"

#include <vector>
#include <string>

namespace HinaPE::RenderEngine
{
class Shader final
{
public:
    unsigned int ID;

public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);
    Shader(const std::string &vertex_path, const std::string &fragment_path, const std::string &geometry_path);
    Shader(const Shader &src) = delete;
    Shader(Shader &&src) noexcept = default;
    ~Shader();
    void operator=(const Shader &src) = delete;
    auto operator=(Shader &&src) noexcept -> Shader & = default;

public:
    void bind() const;
    void uniform(const std::string &name, bool value) const;
    void uniform(const std::string &name, int value) const;
    void uniform(const std::string &name, unsigned int value) const;
    void uniform(const std::string &name, float value) const;
    void uniform(const std::string &name, const mVector2& value) const;
    void uniform(const std::string &name, const mVector3& value) const;
    void uniform(const std::string &name, const mVector4& value) const;
    void uniform(const std::string &name, mMatrix3x3 value) const;
    void uniform(const std::string &name, mMatrix4x4 value) const;

private:
    static auto validate(unsigned int id) -> bool;
};
using ShaderPtr = std::shared_ptr<Shader>;
}

#endif //HINAPE_RENDER_ENGINE_PRO_SHADER_H

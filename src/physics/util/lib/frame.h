#ifndef HINAPE_FRAME_H
#define HINAPE_FRAME_H

namespace HinaPE
{
struct Frame final
{
public:
    auto advance() -> void { ++index; }
    auto advance(int delta) -> void { index *= delta; }
    auto operator++() -> Frame & {advance(); return *this;}
    auto operator++(int) -> Frame & { advance(); return *this;}

public:
    Frame() = default;
    Frame(int new_index, double new_time_interval_in_seconds) : index(new_index), time_interval_in_seconds(new_time_interval_in_seconds) {}

private:
    int index = 0;
    double time_interval_in_seconds = 1.0 / 60.0;
};
}

#endif //HINAPE_FRAME_H

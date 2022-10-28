#ifndef HINAPE_PHYSICS_ANIMATION_H_
#define HINAPE_PHYSICS_ANIMATION_H_

#include "animation.h"

namespace HinaPE
{

//!
//! \brief      Abstract base class for physics-based animation.
//!
//! This class represents physics-based animation by adding time-integration
//! specific functions to Animation class.
//!
class PhysicsAnimation : public Animation
{
public:
    //! Default constructor.
    PhysicsAnimation();

    //! Destructor.
    ~PhysicsAnimation() override;

    //!
    //! \brief      Returns true if fixed sub-timestepping is used.
    //!
    //! When performing a time-integration, it is often required to take
    //! sub-timestepping for better results. The sub-stepping can be either
    //! fixed rate or adaptive, and this function returns which feature is
    //! currently selected.
    //!
    //! \return     True if using fixed sub time steps, false otherwise.
    //!
    auto isUsingFixedSubTimeSteps() const -> bool;

    //!
    //! \brief      Sets true if fixed sub-timestepping is used.
    //!
    //! When performing a time-integration, it is often required to take
    //! sub-timestepping for better results. The sub-stepping can be either
    //! fixed rate or adaptive, and this function sets which feature should be
    //! selected.
    //!
    //! \param[in]   isUsing True to enable fixed sub-stepping.
    //!
    void setIsUsingFixedSubTimeSteps(bool isUsing);

    //!
    //! \brief      Returns the number of fixed sub-timesteps.
    //!
    //! When performing a time-integration, it is often required to take
    //! sub-timestepping for better results. The sub-stepping can be either
    //! fixed rate or adaptive, and this function returns the number of fixed
    //! sub-steps.
    //!
    //! \return     The number of fixed sub-timesteps.
    //!
    auto numberOfFixedSubTimeSteps() const -> unsigned int;

    //!
    //! \brief      Sets the number of fixed sub-timesteps.
    //!
    //! When performing a time-integration, it is often required to take
    //! sub-timestepping for better results. The sub-stepping can be either
    //! fixed rate or adaptive, and this function sets the number of fixed
    //! sub-steps.
    //!
    //! \param[in]  numberOfSteps The number of fixed sub-timesteps.
    //!
    void setNumberOfFixedSubTimeSteps(unsigned int numberOfSteps);

    //! Advances a single frame.
    void advanceSingleFrame();

    //!
    //! \brief      Returns current frame.
    //!
    auto currentFrame() const -> Frame;

    //!
    //! \brief      Sets current frame cursor (but do not invoke update()).
    //!
    void setCurrentFrame(const Frame &frame);

    //!
    //! \brief      Returns current time in seconds.
    //!
    //! This function returns the current time which is calculated by adding
    //! current frame + sub-timesteps it passed.
    //!
    auto currentTimeInSeconds() const -> double;

protected:
    //!
    //! \brief      Called when a single time-step should be advanced.
    //!
    //! When Animation::update function is called, this class will internally
    //! subdivide a frame into sub-steps if needed. Each sub-step, or time-step,
    //! is then taken to move forward in time. This function is called for each
    //! time-step, and a subclass that inherits PhysicsAnimation class should
    //! implement this function for its own physics model.
    //!
    //! \param[in]  timeIntervalInSeconds The time interval in seconds
    //!
    virtual void onAdvanceTimeStep(double timeIntervalInSeconds) = 0;

    //!
    //! \brief      Returns the required number of sub-timesteps for given time
    //!             interval.
    //!
    //! The required number of sub-timestep can be different depending on the
    //! physics model behind the implementation. Override this function to
    //! implement own logic for model specific sub-timestepping for given
    //! time interval.
    //!
    //! \param[in]  timeIntervalInSeconds The time interval in seconds.
    //!
    //! \return     The required number of sub-timesteps.
    //!
    virtual auto numberOfSubTimeSteps(double timeIntervalInSeconds) const -> unsigned int;

    //!
    //! \brief      Called at frame 0 to initialize the physics state.
    //!
    //! Inheriting classes can override this function to setup initial condition
    //! for the simulation.
    //!
    virtual void onInitialize();

private:
    Frame _currentFrame;
    bool _isUsingFixedSubTimeSteps = true;
    unsigned int _numberOfFixedSubTimeSteps = 1;
    double _currentTime = 0.0;

    void onUpdate(const Frame &frame) final;

    void advanceTimeStep(double timeIntervalInSeconds);

    void initialize();
};

using PhysicsAnimationPtr = std::shared_ptr<PhysicsAnimation>;

}  // namespace HinaPE

#endif  // HINAPE_PHYSICS_ANIMATION_H_

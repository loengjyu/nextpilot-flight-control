/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/**
 * @file StickYaw.hpp
 * @brief Generate yaw and angular yawspeed setpoints from stick input
 * @author Matthias Grob <maetugr@gmail.com>
 */

#pragma once

#include <lib/mathlib/math/filter/AlphaFilter.hpp>
#include <px4_platform_common/module_params.h>

class StickYaw : public ModuleParams {
public:
    StickYaw(ModuleParams *parent);
    ~StickYaw() = default;

    void generateYawSetpoint(float &yawspeed_setpoint, float &yaw_setpoint, float stick_yaw, float yaw,
                             bool is_yaw_good_for_control, float deltatime);

private:
    AlphaFilter<float> _yawspeed_filter;

    /**
     * Lock yaw when not currently turning
     * When applying a yawspeed the vehicle is turning, when the speed is
     * set to zero the vehicle needs to slow down and then lock at the yaw
     * it stops at to not drift over time.
     * @param yawspeed current yaw rotational rate state
     * @param yaw current yaw rotational rate state
     * @param yawspeed_setpoint rotation rate at which to turn around yaw axis
     * @param yaw current yaw setpoint which then will be overwritten by the return value
     * @return yaw setpoint to execute to have a yaw lock at the correct moment in time
     */
    static float updateYawLock(float yaw, float yawspeed_setpoint, float yaw_setpoint, bool is_yaw_good_for_control);

    DEFINE_PARAMETERS(
        (ParamFloat<px4::params::MPC_MAN_Y_MAX>)_param_mpc_man_y_max, ///< Maximum yaw speed with full stick deflection
        (ParamFloat<px4::params::MPC_MAN_Y_TAU>)_param_mpc_man_y_tau  ///< time constant for yaw speed filtering
    )
};
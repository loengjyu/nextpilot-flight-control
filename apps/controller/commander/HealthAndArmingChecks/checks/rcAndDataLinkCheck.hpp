/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#pragma once

#include "../Common.hpp"

#include <uORB/Subscription.hpp>
#include <uORB/topics/manual_control_setpoint.h>

class RcAndDataLinkChecks : public HealthAndArmingCheckBase {
public:
    RcAndDataLinkChecks()  = default;
    ~RcAndDataLinkChecks() = default;

    void checkAndReport(const Context &context, Report &reporter) override;

private:
    uORB::Subscription _manual_control_setpoint_sub{ORB_ID(manual_control_setpoint)};

    hrt_abstime _last_valid_manual_control_setpoint{0};

    DEFINE_PARAMETERS_CUSTOM_PARENT(HealthAndArmingCheckBase,
                                    (ParamFloat<px4::params::COM_RC_LOSS_T>)_param_com_rc_loss_t,
                                    (ParamInt<px4::params::NAV_DLL_ACT>)_param_nav_dll_act)
};
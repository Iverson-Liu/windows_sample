//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "Scenario2_Launch.g.h"

namespace winrt::SDKTemplate::implementation
{
    struct Scenario2_Launch : Scenario2_LaunchT<Scenario2_Launch>
    {
        Scenario2_Launch();

        // These methods are public so they can be called by binding.
        void LaunchAtSize_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::SDKTemplate::factory_implementation
{
    struct Scenario2_Launch : Scenario2_LaunchT<Scenario2_Launch, implementation::Scenario2_Launch>
    {
    };
}

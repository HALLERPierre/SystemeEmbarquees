﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "PivotPage.xaml.h"




void ::App4::PivotPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///PivotPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///PivotPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Pivot named 'pivot'
    pivot = safe_cast<::Windows::UI::Xaml::Controls::Pivot^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"pivot"));
    // Get the AppBarButton named 'AddAppBarButton'
    AddAppBarButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"AddAppBarButton"));
    // Get the AppBarButton named 'StartAppBarButton'
    StartAppBarButton = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"StartAppBarButton"));
    // Get the Button named 'buttonStart'
    buttonStart = safe_cast<::Windows::UI::Xaml::Controls::Button^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"buttonStart"));
    // Get the Button named 'button'
    button = safe_cast<::Windows::UI::Xaml::Controls::Button^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"button"));
}

void ::App4::PivotPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::App4::PivotPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&PivotPage::AddAppBarButton_Click);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::App4::PivotPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&PivotPage::StartAppBarButton_Click);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}


// AutolinkedNativeModules.g.cpp contents generated by "react-native autolink-windows"
// clang-format off
#include "pch.h"
#include "AutolinkedNativeModules.g.h"

// Includes from react-native-linear-gradient
#include <winrt/BVLinearGradient.h>

// Includes from react-native-screens
#include <winrt/RNScreens.h>

// Includes from react-native-svg
#include <winrt/RNSVG.h>

namespace winrt::Microsoft::ReactNative
{

void RegisterAutolinkedNativeModulePackages(winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::ReactNative::IReactPackageProvider> const& packageProviders)
{ 
    // IReactPackageProviders from react-native-linear-gradient
    packageProviders.Append(winrt::BVLinearGradient::ReactPackageProvider());
    // IReactPackageProviders from react-native-screens
    packageProviders.Append(winrt::RNScreens::ReactPackageProvider());
    // IReactPackageProviders from react-native-svg
    packageProviders.Append(winrt::RNSVG::ReactPackageProvider());
}

}

# Velcent AdMob

Runtime AdMob wrapper for Unreal mobile builds.

## Features

- Android banner, interstitial, and rewarded ads.
- iOS banner, interstitial, and rewarded bridge when the Google Mobile Ads SDK is available.
- Blueprint-accessible `UVelcentAdMobSubsystem`.
- Central event stream through `OnAdEvent`.
- Test ad unit IDs enabled by default.
- Safe no-op behavior in editor and unsupported desktop runtime.

## Android

The Android bridge is injected through `VelcentAdMob_APL.xml`.

It adds:

- `com.google.android.gms:play-services-ads:25.4.0`
- `com.google.android.gms.ads.APPLICATION_ID` manifest metadata
- `INTERNET` and `ACCESS_NETWORK_STATE` permissions

Set production IDs in:

- `Project Settings > Plugins > Velcent AdMob`
- or `Config/DefaultEngine.ini`

## iOS

The plugin injects `GADApplicationIdentifier` and the Google SKAdNetwork ID through `VelcentAdMob_IOS_UPL.xml`.

The Google Mobile Ads SDK itself is not vendored in this repository. Add it to the generated Xcode project with one of Google's supported methods:

- Swift Package Manager
- CocoaPods: `pod 'Google-Mobile-Ads-SDK'`
- Manual `GoogleMobileAds.xcframework`

When the SDK headers are not available, the iOS bridge compiles as a safe fallback and reports native bridge unavailable.

## Blueprint Flow

1. Get `Velcent AdMob Subsystem`.
2. Bind `OnAdEvent`.
3. Call `Initialize AdMob`.
4. Call `Show Default Banner`, `Load Default Interstitial`, or `Load Default Rewarded`.
5. Wait for `Loaded`, then call `Show Interstitial` or `Show Rewarded`.

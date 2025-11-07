#include "Mouse.h"
#include <GameLibrary/Utility.h>

using namespace GameLibrary;
using namespace DirectX;

/// <summary>
/// このクラスのインスタンスを解放します。
/// </summary>
Mouse::~Mouse()
{
	if (device) {
		device->Unacquire();
		device.Reset();
	}
}

/// <summary>
/// このクラスのインスタンスを初期化します。
/// </summary>
/// <param name="directInput"></param>
/// <param name="hWnd">ウィンドウ ハンドル</param>
void Mouse::Initialize(IDirectInput8W* directInput, HWND hWnd)
{
	this->hWnd = hWnd;
	ThrowIfFailed(directInput->CreateDevice(GUID_SysMouse, &device, nullptr));
	ThrowIfFailed(device->SetDataFormat(&c_dfDIMouse2));
	ThrowIfFailed(device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
}

/// <summary>
/// 現在のマウス入力状態を更新します。
/// </summary>
void Mouse::Update(bool(&currentButtons)[DigitalInput::NumDigitalInputs]) noexcept
{
	if (SUCCEEDED(device->Acquire())) {
		device->GetDeviceState(sizeof mouseState, &mouseState);
	}

	for (size_t digitalInput = DigitalInput::Mouse0; digitalInput <= DigitalInput::Mouse7; ++digitalInput) {
		currentButtons[digitalInput] = GetButton(static_cast<DigitalInput>(digitalInput));
	}
}

bool Mouse::GetButton(DigitalInput digitalInput) const noexcept
{
	return (mouseState.rgbButtons[digitalInput - DigitalInput::Mouse0] > 0);
}

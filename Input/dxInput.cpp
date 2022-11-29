#include"dxInput.h"

dxinput::dxinput()
{
}

void dxinput::init(WNDCLASSEX w, HWND hwnd)
{
	//�L�[�{�[�h
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//�}�E�X
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput_mouse, nullptr);

	result = dinput_mouse->CreateDevice(GUID_SysMouse, &devmouse, NULL);

	result = devmouse->SetDataFormat(&c_dfDIMouse);

	result = devmouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void dxinput::Update(HWND hwnd)
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

	result = devkeyboard->Acquire();
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	oldmouse = mouse;

	result = devmouse->Acquire();
	result = devmouse->GetDeviceState(sizeof(mouse), &mouse);

	mouseMoveVecrocity = { (float)mouse.lX,(float)mouse.lY };

	oldmouse_p = mouse_p;

	GetCursorPos(&mouse_p);
	ScreenToClient(hwnd, &mouse_p);

	mouse_position = { (float)mouse_p.x,(float)mouse_p.y,0.0f };
	old_mouse_position = { (float)oldmouse_p.x,(float)oldmouse_p.y,0.0f };
}

bool dxinput::Triger(BYTE keyCode)
{
	return key[keyCode] && !oldkey[keyCode];
}

bool dxinput::push(BYTE keyCode)
{
	return key[keyCode];
}

bool dxinput::Mouse_LeftTriger()
{
	return mouse.rgbButtons[0] & (0x80) && !(oldmouse.rgbButtons[0] & (0x80));
}

bool dxinput::Mouse_LeftPush()
{
	return mouse.rgbButtons[0] & (0x80);
}

bool dxinput::Mouse_LeftRelease()
{
	return !(mouse.rgbButtons[0] & (0x80)) && (oldmouse.rgbButtons[0] & (0x80));
}

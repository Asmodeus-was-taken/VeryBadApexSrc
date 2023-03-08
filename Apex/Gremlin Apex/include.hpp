#pragma once

#pragma once
#pragma warning( disable : 4307 ) 
#pragma warning( disable : 4244 ) 
#pragma warning( disable : 4800 ) 
#pragma warning( disable : 4838 ) 

#include "Windows.h"
#include <iostream>
#include <string>
#include <vector>
#include <comdef.h>
#include <mutex>
#include <array>
#include <algorithm>
#include <cmath>
#include <future>
#include <thread>
#include <dwmapi.h>
#include "driver.hpp"

//directX
#include <d3dx9core.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "imgui/imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")
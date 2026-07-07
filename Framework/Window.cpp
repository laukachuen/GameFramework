#include "Window.h"

Window::Window(int width, int height, const std::wstring& title, const std::wstring& shaderPath)
    : m_width(width), m_height(height), m_hInstance(GetModuleHandle(nullptr))
{
    // 1. Register Window Class inside Windows OS
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC; // Allocate a unique device context for this window instance
    wc.lpfnWndProc = HandleMsgSetup; // Assign the setup route handler
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // Use standard hardware arrow cursor
    wc.hbrBackground = nullptr; // Handled by DirectX later, no standard GDI background brush needed
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = m_windowClassName.c_str();
    wc.hIconSm = nullptr;

    RegisterClassExW(&wc);

    // 2. Calculate window size based on desired screen resolution (client region size)
    RECT wr;
    wr.left = 100;
    wr.right = m_width + wr.left;
    wr.top = 100;
    wr.bottom = m_height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    // 3. Create the Window and pass 'this' pointer into the last parameter (lpParam)
    m_hWnd = CreateWindowExW(
        0,
        m_windowClassName.c_str(),
        title.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        m_hInstance,
        this // Crucial: Passing pointer of this current C++ class instance
    );

    // 4. Display the window on the screen
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    // Initialize Graphics using std::make_unique
    m_pGfx = std::make_unique<Graphics>(m_hWnd, m_width, m_height, shaderPath);
}

Window::~Window()
{
    DestroyWindow(m_hWnd);
    UnregisterClassW(m_windowClassName.c_str(), m_hInstance);
}

bool Window::ProcessMessages()
{
    MSG msg;
    // PeekMessage checks the queue and returns immediately without locking up the game loop
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false; // Time to close the game loop
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true; // Keep running the game
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // Catch WM_NCCREATE message, which is sent before the window is fully created
    if (msg == WM_NCCREATE)
    {
        // Extract the creation data structure containing our passed pointer
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        // Recover our 'this' pointer that we passed into CreateWindowExW
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // Store our class instance pointer securely inside Windows API internal storage slot
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Change the window's main message router function to Thunk handler from now on
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // Forward the message to the proper class instance handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // Retrieve our stored C++ class instance pointer from Windows internal user data slot
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    // Redirect the message straight to our class member function
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

        // ================= KEYBOARD MESSAGES =================
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: // Also catch system keys like ALT
        // Filter out auto-repeated key messages if the key is already held down
        if (!(lParam & 0x40000000))
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        return 0;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        return 0;

        // Lose focus should clear all keys so character doesn't get stuck running forever
    case WM_KILLFOCUS:
        kbd.ClearState();
        return 0;

        // ================== MOUSE MESSAGES ==================
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        // Handle boundary logging when mouse enters/leaves window client area
        if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
        {
            mouse.OnMouseMove(pt.x, pt.y);
            if (!mouse.IsInWindow())
            {
                SetCapture(hWnd); // Keep tracking mouse slightly outside window if dragging
                mouse.OnMouseEnter();
            }
        }
        else
        {
            // If dragging, we still want to record updates outside bounds
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }
        return 0;
    }
    case WM_LBUTTONDOWN:
        mouse.OnLeftPressed();
        return 0;
    case WM_LBUTTONUP:
        mouse.OnLeftReleased();
        return 0;
    case WM_RBUTTONDOWN:
        mouse.OnRightPressed();
        return 0;
    case WM_RBUTTONUP:
        mouse.OnRightReleased();
        return 0;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
# Security Risk Assessment Report

This report documents the security risks, logical vulnerabilities, and stability issues identified within the **EndlessShooter** (SFML C++ game) codebase. Each vulnerability is analyzed by threat category, impact, and actionable remediation steps.

---

## Executive Summary

A comprehensive review of the codebase was conducted. The codebase contains several issues ranging from information disclosure and local Denial of Service (DoS) risks to platform-specific vulnerabilities and logic flaws. Addressing these issues will significantly improve the stability, security, portability, and performance of the application.

### Risk Level Breakdown
*   **High Severity:** 1 issue (Information Disclosure & Hardcoded Paths)
*   **Medium Severity:** 3 issues (Resource Exhaustion, Uncapped Frame Rates, and Logical Loops)
*   **Low Severity:** 3 issues (Improper Input Validation, Platform Dependencies, and Missing Hardening Flags)

---

## Detailed Findings

### 1. Information Disclosure & Hardcoded Absolute Paths (High)

*   **Location:** `src/logic.cpp:13`
    ```cpp
    if (!font.openFromFile("C:\\Users\\Ali Mohamed\\Desktop\\SFML\\assets\\font\\GameFont.TTF"))
    ```
*   **Vulnerability Type:** CWE-532 (Information Exposure) / CWE-426 (Untrusted Search Path)
*   **Impact:**
    *   **Information Leakage:** The compiled binary will contain the developer's private username (`Ali Mohamed`) and directory structure (`Desktop/SFML...`), leaking metadata to third parties.
    *   **Application Denial of Service (DoS):** This hardcoded absolute path will not exist on other machines. The application will immediately fail to load the font and exit (`return`), preventing the game from starting for any player other than the developer.
    *   **Hijacking Risk:** If the path is recreated on a multi-user environment or via standard directories, an attacker could supply a malicious font file (exploiting potential font-parsing vulnerabilities in underlying libraries like FreeType) to execute code.
*   **Remediation:** 
    Replace the absolute path with a relative path or use a configuration parameter.
    ```cpp
    if (!font.openFromFile("assets/font/GameFont.TTF"))
    ```

---

### 2. CPU and Resource Exhaustion - Uncapped Frame Rate (Medium)

*   **Location:** `src/logic.cpp` (Game loop / `window` initialization)
*   **Vulnerability Type:** CWE-400 (Uncontrolled Resource Consumption)
*   **Impact:**
    *   The SFML game loop does not restrict its frame rate. It will execute as fast as the system's CPU and GPU allow (often thousands of frames per second).
    *   This causes **100% utilization of a CPU core** and high GPU workloads, resulting in:
        *   Excessive electrical power consumption.
        *   Thermal throttling on mobile/laptop machines.
        *   Hardware stress and premature device degradation.
        *   Unpredictable game mechanics, since movement calculations (e.g., `positionX += 0.1f`) are completely frame-rate dependent.
*   **Remediation:**
    Configure a frame rate limit or enable vertical synchronization (V-Sync) right after initializing the render window:
    ```cpp
    sf::RenderWindow window(...);
    window.setFramerateLimit(60); // Caps frame rate at 60 FPS
    // OR
    window.setVerticalSyncEnabled(true); // Syncs with monitor refresh rate
    ```

---

### 3. CPU Exhaustion - Non-yielding Infinite Loop (Medium)

*   **Location:** `screenres.cpp:9-11`
    ```cpp
    int main() {
        while(true){
            
        }
        return 0;
    }
    ```
*   **Vulnerability Type:** CWE-835 (Loop with Unreachable Exit Condition)
*   **Impact:**
    *   If this standalone file is compiled and executed, the empty `while(true)` loop contains no sleep or yield conditions.
    *   It will instantly consume **100% of a CPU core**, leading to high thermal output, lag, and localized system Denial of Service (DoS) on lower-end systems.
*   **Remediation:**
    If the file is unused, delete it. If it is meant to run as a persistent background helper, add a sleeping yield to free CPU resources:
    ```cpp
    #include <thread>
    #include <chrono>
    
    int main() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return 0;
    }
    ```

---

### 4. Logical Vulnerability - False Cast & Unintentional Termination (Medium)

*   **Location:** `src/Player.cpp:52-54`
    ```cpp
    if((bool)sf::Mouse::Button::Left){
        return true;
    }
    ```
*   **Vulnerability Type:** CWE-670 (Always-Incorrect Control Flow) / Logical Flaw
*   **Impact:**
    *   The code casts the enum constant `sf::Mouse::Button::Left` (which evaluates to standard enum values like `0` or `1`) directly to a boolean, instead of calling an input detection method.
    *   **Case A (Enum is non-zero):** The statement is a compile-time constant `true`. The player update function will immediately return `true` on the very first frame, prompting `Logic::run()` to close the window and terminate the program instantly (causing a Denial of Service to the player).
    *   **Case B (Enum is zero):** The statement is a compile-time constant `false`. The code is completely dead, failing to capture actual mouse clicks.
*   **Remediation:**
    Utilize the standard SFML input polling mechanism to query the state of the mouse button:
    ```cpp
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        return true;
    }
    ```

---

### 5. Improper Return Value Validation (Low-Medium)

*   **Location:** `src/main.cpp:5-6`
    ```cpp
    float screenX = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
    float screenY = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));
    ```
*   **Vulnerability Type:** CWE-252 (Unchecked Return Value) / CWE-20 (Improper Input Validation)
*   **Impact:**
    *   The code retrieves screen dimensions from the Win32 API function `GetSystemMetrics` but fails to validate that the return value is greater than zero.
    *   In headless environments, virtual machines, remote desktop sessions, or sandboxed application containers, `GetSystemMetrics` can return `0`.
    *   Passing `0` or negative values to SFML's `VideoMode` can lead to **invalid window dimensions** or unexpected OpenGL context creation failures, resulting in memory violations or graphics driver crashes (local DoS).
*   **Remediation:**
    Provide a safe fallback in case the operating system API returns invalid data:
    ```cpp
    int rawX = GetSystemMetrics(SM_CXSCREEN);
    int rawY = GetSystemMetrics(SM_CYSCREEN);

    float screenX = (rawX > 0) ? static_cast<float>(rawX) : 1280.f;
    float screenY = (rawY > 0) ? static_cast<float>(rawY) : 720.f;
    ```

---

### 6. Platform Portability & Windows Dependency (Low)

*   **Location:** Multiple files
    *   `src/main.cpp`: Inclusion of `<windows.h>` and call to `GetSystemMetrics`.
    *   `src/bullet.cpp` & `src/main.cpp`: Backslashes in include directives (`"..\include\bullet.hpp"`).
*   **Vulnerability Type:** CWE-710 (Improper Coding Standards) / Lack of Portability
*   **Impact:**
    *   **Compilation Failures:** SFML is inherently cross-platform (supporting macOS, Linux, and Windows). However, these OS-specific Windows APIs and inclusion styles prevent compilation on Unix-like compilers, restricting development and deployment flexibility.
    *   **Slash Inconsistency:** Backslashes inside `#include` directives are proprietary Windows behaviors. GCC and Clang compilers on macOS/Linux will fail to locate the headers, breaking standard build pipelines.
*   **Remediation:**
    *   Replace Windows-specific screen dimensions queries with SFML's native cross-platform `sf::VideoMode` query:
        ```cpp
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        float screenX = static_cast<float>(desktop.size.x);
        float screenY = static_cast<float>(desktop.size.y);
        ```
    *   Standardize all `#include` headers to use standard forward slashes (`/`):
        ```cpp
        #include "../include/bullet.hpp"
        ```

---

### 7. Missing Security Hardening & Warnings in Build Configuration (Low)

*   **Location:** `CMakeLists.txt`
*   **Vulnerability Type:** CWE-733 (Compiler Optimization/Hardening Flaws)
*   **Impact:**
    *   The C++ build system does not specify strict compiler warnings (`-Wall -Wextra` or `/W4`). Logical bugs, unsafe casts, and unitialized variable errors will compile silently, introducing high technical debt and hidden vulnerabilities.
    *   No security hardening features like **ASLR** (Address Space Layout Randomization), **DEP** (Data Execution Prevention), or **Stack Canaries** (e.g., `/GS` on MSVC or `-fstack-protector-strong` on GCC/Clang) are requested, meaning any memory bug that does arise is easier for an attacker to exploit.
*   **Remediation:**
    Configure strict warning and standard platform-specific security flags in your `CMakeLists.txt`:
    ```cmake
    # Enable high-level compiler warnings
    if(MSVC)
        target_compile_options(EndlessShooter PRIVATE /W4 /WX)
    else()
        target_compile_options(EndlessShooter PRIVATE -Wall -Wextra -Werror)
    endif()
    ```

---

## Conclusion & Action Plan

By implementing the remediation steps detailed above, you can secure and optimize the **EndlessShooter** application. 

### Recommended Action Sequence:
1.  **Immediate Fix:** Change the hardcoded font path in `src/logic.cpp` to a relative path and replace the backslashes in `#include` directives.
2.  **Stability Fix:** Update `src/Player.cpp` to correctly poll the mouse button state rather than casting the enum value to bool.
3.  **Performance/Hardware Fix:** Limit the frame rate in the game loop to prevent hardware overwork.
4.  **Portability Fix:** Remove the dependency on `<windows.h>` and `GetSystemMetrics` by utilizing SFML's cross-platform window system.

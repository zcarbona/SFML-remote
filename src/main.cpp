#include "..\include\logic.hpp"
int main()
{
    

    float screenX = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
    float screenY = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

    Logic logic;
    logic.run(screenX,screenY);

    
    return 0;
}
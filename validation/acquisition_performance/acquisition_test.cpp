//
//#include <windows.h>
//
//#include <chrono>
//#include <fstream>
//#include <iostream>
//#include <stdexcept>
//#include <string>
//
//int main()
//{
//    constexpr int SAMPLE_COUNT = 200;
//    constexpr DWORD BAUD_RATE = 9600;
//
//    HANDLE port = CreateFileA(
//        R"(\\.\COM7)",
//        GENERIC_READ | GENERIC_WRITE,
//        0, nullptr, OPEN_EXISTING, 0, nullptr
//    );
//
//    if (port == INVALID_HANDLE_VALUE)
//    {
//        std::cerr << "Cannot open COM port.\n";
//        return 1;
//    }
//
//    DCB dcb = {};
//    dcb.DCBlength = sizeof(dcb);
//
//    if (!GetCommState(port, &dcb))
//    {
//        CloseHandle(port);
//        return 1;
//    }
//
//    dcb.BaudRate = BAUD_RATE;
//    dcb.ByteSize = 8;
//    dcb.Parity = NOPARITY;
//    dcb.StopBits = ONESTOPBIT;
//
//    if (!SetCommState(port, &dcb))
//    {
//        CloseHandle(port);
//        return 1;
//    }
//
//    COMMTIMEOUTS timeouts = {};
//    timeouts.ReadIntervalTimeout = 100;
//    timeouts.ReadTotalTimeoutConstant = 100;
//    SetCommTimeouts(port, &timeouts);
//
//    
//    Sleep(2000);
//    PurgeComm(port, PURGE_RXCLEAR);
//
//    std::ofstream csv("./validation/acquisition_performance/acquisition.csv");
//
//    if (!csv)
//    {
//        CloseHandle(port);
//        return 1;
//    }
//
//    csv << "sample_id,arduino_time_ms,pc_time_ms,"
//        "temperature_c\n";
//
//    std::string line;
//    int received = 0;
//
//    using Clock = std::chrono::steady_clock;
//    auto start = Clock::now();
//
//    while (received < SAMPLE_COUNT)
//    {
//        char character = '\0';
//        DWORD bytesRead = 0;
//
//        if (!ReadFile(port, &character, 1, &bytesRead, nullptr))
//        {
//            std::cerr << "Serial read failed.\n";
//            break;
//        }
//
//        if (bytesRead == 0)
//            continue;
//
//        if (character == '\r')
//            continue;
//
//        if (character != '\n')
//        {
//            line += character;
//
//            if (line.size() > 100)
//                line.clear();
//
//            continue;
//        }
//
//        auto receiveTime = Clock::now();
//
//        double pcTimeMs =
//            std::chrono::duration<double, std::milli>(
//                receiveTime - start
//            ).count();
//
//        try
//        {
//            size_t p1 = line.find(',');
//            size_t p2 = line.find(',', p1 + 1);
//
//            if (p1 == std::string::npos ||
//                p2 == std::string::npos)
//            {
//                throw std::runtime_error("Invalid frame");
//            }
//
//            unsigned long id =
//                std::stoul(line.substr(0, p1));
//
//            unsigned long arduinoMs =
//                std::stoul(
//                    line.substr(p1 + 1, p2 - p1 - 1)
//                );
//
//            double temperature =
//                std::stod(line.substr(p2 + 1));
//
//            csv << id << ','
//                << arduinoMs << ','
//                << pcTimeMs << ','
//                << temperature << '\n';
//
//            received++;
//
//            if (received % 20 == 0)
//            {
//                std::cout
//                    << "Received: " << received
//                    << "/" << SAMPLE_COUNT << '\n';
//            }
//        }
//        catch (const std::exception&)
//        {
//            std::cerr << "Invalid frame: " << line << '\n';
//        }
//
//        line.clear();
//    }
//
//    csv.close();
//    CloseHandle(port);
//
//    std::cout << "Test finished. Samples: "
//        << received << '\n';
//
//    return 0;
//}

#include "downloader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#endif

bool downloadFile(std::string url, const std::string& outputPath) {
    if (url.rfind("https://", 0) == 0) {
       // std::cerr << "[!] HTTPS not supported in this build. Attempting HTTP fallback...\n";
        url = "http://" + url.substr(8);  
    }
    if (url.substr(0, 7) != "http://") {
        std::cerr << "[X] Only HTTP is supported. URL: " << url << "\n";
        return false;
    }

    std::string address = url.substr(7);
    size_t slashPos = address.find('/');
    std::string host = address.substr(0, slashPos);
    std::string path = (slashPos != std::string::npos) ? address.substr(slashPos) : "/";

#ifdef _WIN32
    HINTERNET hInternet = InternetOpen("MinecraftProfileCreator", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hFile = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return false;
    }

    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        out.write(buffer, bytesRead);
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);
    out.close();
#else
    addrinfo hints{}, *res;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) return false;

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0 || connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
        freeaddrinfo(res);
        return false;
    }
    freeaddrinfo(res);

    std::ostringstream req;

    std::string request = req.str();
    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096];
    int received;
    std::ostringstream response;
    while ((received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        response.write(buffer, received);
    }
    close(sock);

    std::string fullResponse = response.str();
    size_t bodyPos = fullResponse.find("\r\n\r\n");
    if (bodyPos == std::string::npos) return false;

    std::ofstream out(outputPath, std::ios::binary);
    out.write(fullResponse.c_str() + bodyPos + 4, fullResponse.size() - bodyPos - 4);
    out.close();
#endif
return true;
}


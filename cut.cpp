#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream inputFile("ecg_1_to_4.txt_discretized"); // 打开输入文件
    if (!inputFile.is_open()) {
        std::cerr << "无法打开输入文件！" << std::endl;
        return 1;
    }

    std::string line;
    int lineCount = 0;
    int fileCount = 0;
    std::ofstream outputFile;

    while (getline(inputFile, line)) {
        if (lineCount % 203763 == 0) { // 每1000行更换一次输出文件
            if (outputFile.is_open()) {
                outputFile.close(); // 关闭当前输出文件
            }
            outputFile.open("output_" + std::to_string(++fileCount) + ".txt"); // 打开新的输出文件
            if (!outputFile.is_open()) {
                std::cerr << "无法创建输出文件！" << std::endl;
                return 1;
            }
        }
        outputFile << line << std::endl; // 写入当前行到输出文件
        ++lineCount;
    }

    if (outputFile.is_open()) {
        outputFile.close(); // 关闭最后一个输出文件
    }

    return 0;
}

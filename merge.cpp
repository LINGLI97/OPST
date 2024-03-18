#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int main() {
    // 假设我们有一个文件名列表
    std::vector<std::string> fileNames = {"output_1.txt", "output_2.txt"};
    std::ofstream outputFile("mergedOutput2.txt", std::ios::out | std::ios::app); // 创建输出文件

    if (!outputFile.is_open()) {
        std::cerr << "无法打开输出文件！" << std::endl;
        return 1;
    }

    for (const auto& fileName : fileNames) {
        std::ifstream inputFile(fileName); // 顺序打开每个文件

        if (!inputFile.is_open()) {
            std::cerr << "无法打开输入文件：" << fileName << std::endl;
            continue; // 如果无法打开文件，则跳过该文件
        }

        outputFile << inputFile.rdbuf(); // 将文件内容直接复制到输出文件
        inputFile.close(); // 关闭当前输入文件
    }

    outputFile.close(); // 关闭输出文件
    std::cout << "文件合并完成。" << std::endl;

    return 0;
}

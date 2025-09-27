#include "learn01.hpp"

namespace learn01 {
    void learn01(){
        std::cout << "Hello Learn01!\n";

        // 1. Hello World
        // cv::Mat img = cv::Mat::zeros(400, 600, CV_8UC3);
        // cv::putText(img, "Hello OpenCV", cv::Point(50, 200),
        //         cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 255, 0), 3);

        // cv::imshow("Hello OpenCV", img);
        // cv::waitKey(0);   // 等待按键，不然窗口会直接闪退

        // 2. 创建一个 400x400 的彩色图像，背景白色
        // cv::Mat img(400, 400, CV_8UC3, cv::Scalar(255, 255, 255));

        // // 在图像上画一个蓝色圆
        // cv::circle(img, cv::Point(200, 200), 100, cv::Scalar(255, 0, 0), -1);

        // // 在图像上写文字
        // cv::putText(img, "Hello OpenCV", cv::Point(50, 380),
        //             cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);

        // // 显示窗口
        // cv::imshow("Hello OpenCV", img);

        // // // 等待按键
        // cv::waitKey(0);
        
        // 1. 读取图像文件
        cv::Mat image = cv::imread("C:\\Users\\29051\\Downloads\\image2.png", cv::IMREAD_COLOR); // 替换为你的图片路径
        
        // 检查图像是否加载成功
        if (image.empty()) {
            std::cerr << "Error: Could not load image!" << std::endl;
            return;
        }

        // 2. 创建窗口并显示图像
        cv::namedWindow("Hello OpenCV", cv::WINDOW_AUTOSIZE);
        cv::imshow("Hello OpenCV", image);

        // 3. 等待按键退出
        cv::waitKey(0); // 参数为0表示无限等待

        // 4. 销毁窗口
        cv::destroyAllWindows();
    }
}
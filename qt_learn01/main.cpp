#include "learn001.hpp"
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QRandomGenerator>
#include <QFont>
#include <QColor>
#include <vector>

// 存储所有弹窗指针，用于程序退出时清理
std::vector<QDialog*> allPopups;

// 生成随机弹窗
void createRandomPopup(QScreen* targetScreen = nullptr, QWidget* parent = nullptr) {
    // 1. 创建弹窗（无模态，允许操作其他窗口）
    QDialog* popup = new QDialog(parent);
    popup->setWindowTitle("随机弹窗");
    popup->setModal(false); // 非模态弹窗
    popup->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动释放内存

    // 2. 随机弹窗大小（100-200px 宽，80-150px 高）
    int width = QRandomGenerator::global()->bounded(100, 201);
    int height = QRandomGenerator::global()->bounded(80, 151);
    popup->resize(width, height);

    // 3. 随机位置（避免弹窗超出屏幕范围）
    // QScreen* screen = QApplication::primaryScreen();
    // QRect screenRect = screen->availableGeometry(); // 屏幕可用区域（排除任务栏）
    // int maxX = screenRect.width() - width;
    // int maxY = screenRect.height() - height;
    // int x = QRandomGenerator::global()->bounded(0, maxX + 1);
    // int y = QRandomGenerator::global()->bounded(0, maxY + 1);
    // popup->move(x, y);
    QScreen* screen = targetScreen;
    if (!screen) {
        QList<QScreen*> screens = QApplication::screens();
        screen = screens.isEmpty() ? nullptr : screens.first(); // 默认主屏幕
    }

    // 3. 在目标屏幕内随机位置（避免超出屏幕）
    if (screen != nullptr) {
        QRect screenRect = screen->availableGeometry(); // 屏幕可用区域（排除任务栏）
        int maxX = screenRect.width() - width;
        int maxY = screenRect.height() - height;
        int x = QRandomGenerator::global()->bounded(0, maxX + 1);
        int y = QRandomGenerator::global()->bounded(0, maxY + 1);
        
        // 移动弹窗到目标屏幕的随机位置
        popup->move(screenRect.topLeft() + QPoint(x, y));
        // 强制弹窗归属到目标屏幕（部分系统需要这一步）
        popup->setScreen(screen);
    }

    // 4. 随机内容（从预设文本中选择）
    QStringList tips = {
        "多喝水哦~", "要开开心心吖~", "每天都要元气满满~",
        "记得多吃水果~", "保持好心情~", "照顾好自己~",
        "顺顺利利~", "早点休息~", "别熬夜~", "今晚过得开心~"
    };
    QString randomTip = tips[QRandomGenerator::global()->bounded(tips.size())];

    // 5. 随机字体大小（12-16px）
    QFont font;
    font.setPointSize(QRandomGenerator::global()->bounded(12, 17));

    // 6. 随机文字颜色
    QColor textColor(
        QRandomGenerator::global()->bounded(0, 128), // R（深色系，避免与白色背景冲突）
        QRandomGenerator::global()->bounded(0, 128), // G
        QRandomGenerator::global()->bounded(0, 128)  // B
    );

    // 7. 弹窗内容布局
    QLabel* label = new QLabel(randomTip);
    label->setFont(font);
    label->setStyleSheet(QString("color: %1;").arg(textColor.name()));
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->addWidget(label);
    popup->setLayout(layout);

    // 8. 随机弹窗背景色（浅色系）
    QColor bgColor(
        QRandomGenerator::global()->bounded(200, 256), // R
        QRandomGenerator::global()->bounded(200, 256), // G
        QRandomGenerator::global()->bounded(200, 256)  // B
    );
    popup->setStyleSheet(
        QString("background-color: %1; border-radius: 8px;").arg(bgColor.name())
    );

    // 9. 保存弹窗指针（可选，用于强制关闭）
    allPopups.push_back(popup);

    // 10. 显示弹窗
    popup->show();

    // 11. 弹窗自动关闭（可选：3-8秒后自动关闭）
    int autoCloseDelay = QRandomGenerator::global()->bounded(3000, 8001); // 3-8秒
    QTimer::singleShot(autoCloseDelay, popup, &QDialog::close);
}

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QList<QScreen*> allScreens = QApplication::screens();
    QScreen* secondScreen = allScreens.size() >= 2 ? allScreens[1] : nullptr;
    // 主按钮：点击后开始弹出100个随机弹窗
    QPushButton mainButton("弹出100个随机弹窗");
    mainButton.resize(300, 80);
    mainButton.setFont(QFont("Arial", 14));
    mainButton.move(50, 50);
    mainButton.show();
    mainButton.setScreen(secondScreen);

    // 定时器：控制弹窗弹出频率（每隔100ms弹出一个）
    QTimer popupTimer;
    int popupCount = 0;
    const int totalPopups = 100; // 总弹窗数量

    // 按钮点击事件：启动定时器
    QObject::connect(&mainButton, &QPushButton::clicked, [&]() {
        mainButton.setEnabled(false); // 禁用按钮，防止重复点击
        popupCount = 0; // 重置计数

        // 定时器触发：创建单个弹窗
        QObject::connect(&popupTimer, &QTimer::timeout, [&]() {
            if (popupCount < totalPopups) {
                createRandomPopup(secondScreen, &mainButton);
                popupCount++;
            } else {
                popupTimer.stop(); // 达到100个，停止定时器
                mainButton.setEnabled(true); // 启用按钮
                QMessageBox::information(&mainButton, "完成", "100个弹窗已全部弹出！");
            }
        });

        popupTimer.start(100); // 每隔100ms弹出一个
    });

    // 程序退出时清理所有弹窗（避免内存泄漏）
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        for (QDialog* popup : allPopups) {
            if (popup->isVisible()) {
                popup->close();
            }
        }
        allPopups.clear();
    });

    return app.exec();
}
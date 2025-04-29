#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QFont>
#include <QPalette>
#include <QFrame>
#include <QDesktopServices>
#include <QUrl>
#include <QFontMetrics>
#include <QMap>
#include <QVector>

class ToolCard : public QFrame {
    Q_OBJECT
public:
    ToolCard(const QString &title, const QString &description = "", bool isLongest = false, QWidget *parent = nullptr)
    : QFrame(parent) {
        // Keep original red text size (14/18px)
        int titleSize = isLongest ? 18 : 14;
        // Make turquoise text smaller (10px)
        int descSize = 10;
        // Reduce padding
        int padding = 8;
        // Make boxes narrower (400px)
        int width = 400;

        setStyleSheet(QString(R"(
            ToolCard {
                background-color: black;
                border: 2px solid #24ffff;
                padding: %1px;
                border-radius: 8px;
                margin-bottom: 8px;
            }
            QLabel#title {
                color: #ff0000;
                font-weight: bold;
                font-size: %2px;
            }
            QLabel#description {
                color: #24ffff;
                font-size: %3px;
            }
        )").arg(padding).arg(titleSize).arg(descSize));

        setFixedWidth(width);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(5, 5, 5, 5);
        layout->setSpacing(4);
        layout->setAlignment(Qt::AlignHCenter);

        titleLabel = new QLabel(title, this);
        titleLabel->setObjectName("title");
        titleLabel->setWordWrap(false);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        layout->addWidget(titleLabel);

        if (!description.isEmpty()) {
            QLabel *descLabel = new QLabel(description, this);
            descLabel->setObjectName("description");
            descLabel->setWordWrap(false);
            descLabel->setAlignment(Qt::AlignCenter);
            descLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            layout->addWidget(descLabel);
        }

        setLayout(layout);
        setCursor(Qt::PointingHandCursor);
    }

    void setLink(const QString &url) {
        this->url = url;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!url.isEmpty()) {
            QDesktopServices::openUrl(QUrl(url));
        }
        QFrame::mousePressEvent(event);
    }

private:
    QLabel *titleLabel;
    QString url;
};

class SectionWidget : public QWidget {
    Q_OBJECT
public:
    SectionWidget(const QString &title, const QString &emoji = "", QWidget *parent = nullptr)
    : QWidget(parent) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(10);
        mainLayout->setAlignment(Qt::AlignHCenter);

        QHBoxLayout *titleLayout = new QHBoxLayout;
        titleLayout->setContentsMargins(0, 0, 0, 0);
        titleLayout->setSpacing(8);
        titleLayout->setAlignment(Qt::AlignHCenter);

        if (!emoji.isEmpty()) {
            QLabel *emojiLabel = new QLabel(emoji, this);
            emojiLabel->setStyleSheet("font-size: 18px; color: #24ffff;");
            titleLayout->addWidget(emojiLabel);
        }

        QLabel *titleLabel = new QLabel(title, this);
        titleLabel->setStyleSheet(R"(
            color: #ff0000;
            font-size: 22px;
            font-weight: bold;
        )");
        titleLabel->setWordWrap(false);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLayout->addWidget(titleLabel);

        mainLayout->addLayout(titleLayout);

        toolContainer = new QWidget(this);
        toolLayout = new QVBoxLayout(toolContainer);
        toolLayout->setContentsMargins(0, 0, 0, 0);
        toolLayout->setSpacing(8);
        toolLayout->setAlignment(Qt::AlignHCenter);

        mainLayout->addWidget(toolContainer);

        setLayout(mainLayout);
    }

    void addTool(const QString &title, const QString &url, const QString &description = "", bool isLongest = false) {
        ToolCard *card = new ToolCard(title, description, isLongest, this);
        card->setLink(url);
        toolLayout->addWidget(card, 0, Qt::AlignHCenter);
    }

private:
    QWidget *toolContainer;
    QVBoxLayout *toolLayout;
};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Claudemods's Custom Mods");
        resize(1000, 800);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::white);
        setPalette(pal);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(20);
        mainLayout->setAlignment(Qt::AlignHCenter);

        // Header
        QWidget *header = new QWidget(this);
        QVBoxLayout *headerLayout = new QVBoxLayout(header);
        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(10);
        headerLayout->setAlignment(Qt::AlignHCenter);

        QLabel *title = new QLabel("Claudemods's Custom Mods", header);
        title->setStyleSheet(R"(
            color: #ff0000;
            font-size: 36px;
            font-weight: bold;
        )");
        title->setWordWrap(false);
        title->setAlignment(Qt::AlignCenter);

        QLabel *subtitle = new QLabel("A collection of tools, scripts, and applications for Linux enthusiasts.", header);
        subtitle->setStyleSheet(R"(
            color: #00568f;
            font-size: 18px;
        )");
        subtitle->setWordWrap(false);
        subtitle->setAlignment(Qt::AlignCenter);

        QFrame *divider = new QFrame(header);
        divider->setFrameShape(QFrame::HLine);
        divider->setFrameShadow(QFrame::Sunken);
        divider->setStyleSheet("color: #00568f;");

        headerLayout->addWidget(title);
        headerLayout->addWidget(subtitle);
        headerLayout->addWidget(divider);
        header->setLayout(headerLayout);

        mainLayout->addWidget(header);

        // Scroll area for content
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        QWidget *content = new QWidget(scrollArea);
        QVBoxLayout *contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(10, 10, 10, 10);
        contentLayout->setSpacing(20);
        contentLayout->setAlignment(Qt::AlignHCenter);

        // Add all sections with automatic longest option detection
        addAllSections(contentLayout);

        content->setLayout(contentLayout);
        scrollArea->setWidget(content);
        mainLayout->addWidget(scrollArea);

        setLayout(mainLayout);
    }

private:
    struct ToolInfo {
        QString title;
        QString url;
        QString description;
        QString section;
    };

    void addAllSections(QVBoxLayout *layout) {
        // Collect all tool information
        QVector<ToolInfo> allTools = {
            // Incus System Containers
            {"Google Drive Resources", "https://drive.google.com/drive/folders/1-6eOluk8Zws0PhXDHFea_qMYayjwUopB", "", "Incus System Containers"},

            // Isos To Build From
            {"Google Drive ISO Collection", "https://drive.google.com/drive/folders/1rm-s7avP_G9NkhXK0tKkTh1a_UJ6YIYl", "", "Isos To Build From"},

            // Claudemods Distributions
            {"Custom Distributions", "https://drive.google.com/drive/folders/1PsEbYVgRC8RP8SX7nfJle6CM4OjeK9HJ", "", "Claudemods Distributions"},

            // Container Tools
            {"ACCU", "https://github.com/claudemods/ACCU", "Advanced Container Creation Utility", "Container Tools"},

            // ISO Creator Tools
            {"Arch Incus ISO Creator", "https://github.com/claudemods/Arch-Incus-Iso-Creator-Script", "Script for creating Arch Linux ISOs", "ISO Creator Tools"},
            {"Apex Arch ISO Creator (GUI)", "https://github.com/claudemods/ApexArchIsoCreatorGuiAppImage", "Graphical Arch ISO creator (AppImage)", "ISO Creator Tools"},
            {"Apex Arch ISO Creator (Script)", "https://github.com/claudemods/ApexArchIsoCreatorScriptAppImage", "Script version of Arch ISO creator", "ISO Creator Tools"},
            {"Multi-ISO Konsole Script", "https://github.com/claudemods/claudemods-multi-iso-konsole-script", "Create Debian/Ubuntu ISOs", "ISO Creator Tools"},

            // Cloning Tools
            {"Chrome OS Cloner", "https://github.com/claudemods/claudemods-chromeoscloner", "", "Cloning Tools"},
            {"Clone Linux System To Archives", "https://github.com/claudemods/CS2A", "", "Cloning Tools"},
            {"Plasma 6 Cloner", "https://github.com/claudemods/plasma6cloner", "", "Cloning Tools"},

            // Installers
            {"Arch Installer (GUI, ext4)", "https://github.com/claudemods/ApexArchInstallerAppImage", "", "Installers"},
            {"Arch Installer (Script, Btrfs)", "https://github.com/claudemods/Apex-InstallerBtrfs", "", "Installers"},
            {"Debian Installer (ext4)", "https://github.com/claudemods/claudemods-DebianInstaller", "", "Installers"},

            // AppImages - Browsers
            {"Apex Browser", "https://github.com/claudemods/ApexBrowserAppImage", "", "Browsers"},
            {"Cachy Browser", "https://github.com/claudemods/CachyBrowserAppImage", "", "Browsers"},
            {"Firefox", "https://github.com/claudemods/FireFoxAppImage", "", "Browsers"},
            {"Microsoft Edge", "https://github.com/claudemods/MicroSoftEdgeAppImage", "", "Browsers"},
            {"Brave Browser", "https://github.com/claudemods/BraveBrowserAppImage", "", "Browsers"},
            {"Chromium", "https://github.com/claudemods/ChromiumAppImage", "", "Browsers"},
            {"Opera", "https://github.com/claudemods/OperaAppImage", "", "Browsers"},

            // AppImages - Multimedia
            {"VLC", "https://github.com/claudemods/VlcAppImage", "", "Multimedia"},
            {"Kdenlive", "https://www.pling.com/p/2259804", "", "Multimedia"},
            {"Shotcut", "https://www.pling.com/p/2259392", "", "Multimedia"},
            {"Krita", "https://www.pling.com/p/2259793", "", "Multimedia"},

            // AppImages - Graphics
            {"GIMP", "https://github.com/claudemods/GimpAppImage", "", "Graphics"},
            {"Inkscape", "https://github.com/claudemods/InkscapeAppImage", "", "Graphics"},

            // AppImages - AI Tools
            {"Deepseek", "https://github.com/claudemods/DeepSeekAppImage", "", "AI Tools"},
            {"Qwen AI", "https://github.com/claudemods/QwenAiAppimage", "", "AI Tools"},
            {"Gemini", "https://github.com/claudemods/GeminiAppImage", "", "AI Tools"},
            {"ChatGPT", "https://github.com/claudemods/ChatGptAppImage", "", "AI Tools"},

            // AppImages - Utilities
            {"Custom Bottle For Gamers", "https://github.com/claudemods/Custom-Bottle-For-Gamers", "", "Utilities"},
            {"YouTube Downloader", "https://github.com/claudemods/YoutubeAndDownloader", "", "Utilities"},
            {"qBittorrent", "https://www.pling.com/p/2259406", "", "Utilities"},
            {"Arch Auto Chroot", "https://github.com/claudemods/AutoArchChrootQt6Appimage", "", "Utilities"},
            {"Arch Mirror Changer", "https://github.com/claudemods/ArchMirrorChanger", "", "Utilities"},

            // AppImages - Social
            {"Facebook", "https://www.pling.com/p/2195889", "", "Social"},
            {"Facebook Messenger", "https://www.pling.com/p/2195882", "", "Social"},
            {"Discord", "https://github.com/claudemods/DiscordAppImage", "", "Social"},
            {"WhatsApp", "https://www.pling.com/p/2195838", "", "Social"},

            // KDE Tools
            {"11menu", "https://github.com/claudemods/11menu", "Custom KDE menu", "KDE Tools"},
            {"Dolphin as Root", "https://github.com/claudemods/Dolphin-As-Root-Plasma-5-and-Plasma-6", "Root file manager integration", "KDE Tools"},
            {"KDE Store", "https://www.pling.com/p/2195815", "", "KDE Tools"},

            // Wallpapers
            {"OutKast", "https://www.pling.com/p/2284874/", "", "Wallpapers"},
            {"Rift", "https://www.pling.com/p/2284610/", "", "Wallpapers"},
            {"commandeer", "https://www.pling.com/p/2284894/", "", "Wallpapers"},
            {"Refractor", "https://www.pling.com/p/2284909/", "", "Wallpapers"},
            {"NightFires", "https://www.pling.com/p/2284908/", "", "Wallpapers"},
            {"Cyber", "https://www.pling.com/p/2284871/", "", "Wallpapers"},
            {"Fortune", "https://www.pling.com/p/2284879/", "", "Wallpapers"},
            {"Connect", "https://www.pling.com/p/2284893/", "", "Wallpapers"},
            {"Nemisis", "https://www.pling.com/p/2284885/", "", "Wallpapers"},
            {"Escape", "https://www.pling.com/p/2284618/", "", "Wallpapers"},
            {"July", "https://www.pling.com/p/2284615/", "", "Wallpapers"},
            {"Today", "https://www.pling.com/p/2284611/", "", "Wallpapers"},
            {"Tomorrow", "https://www.pling.com/p/2284612/", "", "Wallpapers"},
            {"Yesterday", "https://www.pling.com/p/2284613/", "", "Wallpapers"},
            {"June", "https://www.pling.com/p/2284614/", "", "Wallpapers"},
            {"Soon", "https://www.pling.com/p/2284616/", "", "Wallpapers"},
            {"Wraft", "https://www.pling.com/p/2284617/", "", "Wallpapers"},
            {"DayOne", "https://www.pling.com/p/2284620/", "", "Wallpapers"}
        };

        // Find the longest option in each section
        QMap<QString, QString> longestOptions;
        for (const ToolInfo &tool : allTools) {
            if (!longestOptions.contains(tool.section) ||
                tool.title.length() > longestOptions[tool.section].length()) {
                longestOptions[tool.section] = tool.title;
                }
        }

        // Add sections with tools
        QMap<QString, SectionWidget*> sections;

        // Create all sections first
        for (const QString &section : longestOptions.keys()) {
            QString emoji = "";
            if (section == "Isos To Build From") emoji = "📀";
            else if (section == "Claudemods Distributions") emoji = "📀";
            else if (section == "Container Tools") emoji = "📦";
            else if (section == "ISO Creator Tools") emoji = "📀";
            else if (section == "Cloning Tools") emoji = "💾";
            else if (section == "Installers") emoji = "🛠️";
            else if (section == "Browsers") emoji = "🖥️";
            else if (section == "Multimedia") emoji = "🖥️";
            else if (section == "Graphics") emoji = "🖥️";
            else if (section == "AI Tools") emoji = "🖥️";
            else if (section == "Utilities") emoji = "🖥️";
            else if (section == "Social") emoji = "🖥️";
            else if (section == "KDE Tools") emoji = "🖱️";
            else if (section == "Wallpapers") emoji = "🖼️";

            SectionWidget *sectionWidget = new SectionWidget(section, emoji, this);
            sections[section] = sectionWidget;
        }

        // Add tools to sections
        for (const ToolInfo &tool : allTools) {
            bool isLongest = (tool.title == longestOptions[tool.section]);
            sections[tool.section]->addTool(tool.title, tool.url, tool.description, isLongest);
        }

        // Add sections to layout in order
        QStringList sectionOrder = {
            "Incus System Containers",
            "Isos To Build From",
            "Claudemods Distributions",
            "Container Tools",
            "ISO Creator Tools",
            "Cloning Tools",
            "Installers",
            "Browsers",
            "Multimedia",
            "Graphics",
            "AI Tools",
            "Utilities",
            "Social",
            "KDE Tools",
            "Wallpapers"
        };

        for (const QString &section : sectionOrder) {
            if (sections.contains(section)) {
                layout->addWidget(sections[section], 0, Qt::AlignHCenter);
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application font
    QFont font("Segoe UI", 10);
    app.setFont(font);

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
